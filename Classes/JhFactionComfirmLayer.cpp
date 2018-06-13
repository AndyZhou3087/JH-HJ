#include "JhFactionComfirmLayer.h"
#include "JhSoundManager.h"
#include "JhCommonFuncs.h"
#include "JhConst.h"
#include "MD5.h"
#include "JhHintBox.h"
#include "JhWaitingProgress.h"
#include "JhGameScene.h"
#include "JhFactionMainLayer.h"
#include "JhFactionMemberLayer.h"

bool JhFactionComfirmLayer::init(FactionListData *fldata)
{
    if (!Layer::init()) 
	{
		return false;
    }
	f_action = F_NONE;
	m_fldata = fldata;
	Node* csbnode = CSLoader::createNode("jhfactionComfirmLayer.csb");
	this->addChild(csbnode);

	cocos2d::ui::Button* okbtn = (cocos2d::ui::Button*)csbnode->getChildByName("okbtn");
	okbtn->addTouchEventListener(CC_CALLBACK_2(JhFactionComfirmLayer::onOk, this));

	cocos2d::ui::Button* cancelbtn = (cocos2d::ui::Button*)csbnode->getChildByName("cancelbtn");
	cancelbtn->addTouchEventListener(CC_CALLBACK_2(JhFactionComfirmLayer::onCancel, this));

	cocos2d::ui::Text* desctext = (cocos2d::ui::Text*)csbnode->getChildByName("content");
	std::string str;
	if (JhGlobalData::mytitle == 1)
	{
		str = StringUtils::format("%s%s%s", JhCommonFuncs::gbk2utf("是否确认花费50金元宝解散").c_str(), m_fldata->factionname.c_str(), JhCommonFuncs::gbk2utf("？").c_str());
	}
	else
	{
		str = StringUtils::format("%s%s%s", JhCommonFuncs::gbk2utf("是否确认退出").c_str(), m_fldata->factionname.c_str(), JhCommonFuncs::gbk2utf("？").c_str());

	}
	desctext->setString(str);
	//////layer 点击事件，屏蔽下层事件
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};
	//点击任何位置移除掉
	listener->onTouchEnded = [=](Touch *touch, Event *event)
	{
		return;
	};
	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    return true;
}

JhFactionComfirmLayer* JhFactionComfirmLayer::create(FactionListData *fldata)
{
	JhFactionComfirmLayer *pRet = new JhFactionComfirmLayer();
	if (pRet && pRet->init(fldata))
	{
		pRet->autorelease();
	}
	else
	{
		delete pRet;
		pRet = NULL;
	}
	return pRet;
}

void JhFactionComfirmLayer::onOk(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		if (JhGlobalData::mytitle == 1)
		{
			int mygold = JhGlobalData::getMyGoldCount();

			if (mygold >= 50)
			{
				if (JhGlobalData::getMD5MyGoldCount().compare(md5(mygold)) != 0)
				{
					JhGlobalData::dataIsModified = true;
					JhGlobalData::setMyGoldCount(0);
					JhHintBox* hint = JhHintBox::create(JhCommonFuncs::gbk2utf("发现有作弊行为，金元宝清零作为处罚！！"));
					this->addChild(hint);
					return;
				}

				f_action = F_RELEASE;
				JhWaitingProgress* waitbox = JhWaitingProgress::create("处理中...");
				Director::getInstance()->getRunningScene()->addChild(waitbox, 1, "waitbox");
				JhServerDataSwap::init(this)->leaveFaction(1, m_fldata->id, g_hero->getHeadID());
			}
			else
			{
				JhHintBox* hint = JhHintBox::create(JhCommonFuncs::gbk2utf("金元宝不足！！"));
				Director::getInstance()->getRunningScene()->addChild(hint, 1);
				removSelf();
			}
		}
		else
		{
			f_action = F_LEAVE;

			JhWaitingProgress* waitbox = JhWaitingProgress::create("加载中...");
			Director::getInstance()->getRunningScene()->addChild(waitbox, 1, "waitbox");
			JhServerDataSwap::init(this)->leaveFaction(0, m_fldata->id, g_hero->getHeadID());
		}
	}
}

void JhFactionComfirmLayer::onCancel(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		removSelf();
	}
}

void JhFactionComfirmLayer::removSelf()
{
	this->removeFromParentAndCleanup(true);
}

void JhFactionComfirmLayer::onSuccess()
{
	if (f_action == F_RELEASE)
	{
		JhGlobalData::setMyGoldCount(JhGlobalData::getMyGoldCount() - 50);
		int usegold = JhGlobalData::getUseGold() + 50;
		JhGlobalData::setUseGold(usegold);
	}
	Director::getInstance()->getRunningScene()->removeChildByName("waitbox");
	JhGlobalData::myFaction = 0;
	JhGlobalData::mytitle = 0;
	f_action = F_NONE;
	removSelf();

	JhFactionMemberLayer* flayer = (JhFactionMemberLayer*)g_gameLayer->getChildByName("factionmemberlayer");
	if (flayer != NULL)
	{
		flayer->removeFromParentAndCleanup(true);
	}

	JhFactionMainLayer* fmlayer = (JhFactionMainLayer*)g_gameLayer->getChildByName("factionmainlayer");
	if (fmlayer != NULL)
	{
		fmlayer->getFactionListData();
	}
}

void JhFactionComfirmLayer::onErr(int errcode)
{
	f_action = F_NONE;
	Director::getInstance()->getRunningScene()->removeChildByName("waitbox");
	JhHintBox * box = JhHintBox::create(JhCommonFuncs::gbk2utf("数据处理异常，请检查网络连接！！"));
	Director::getInstance()->getRunningScene()->addChild(box,1);
	removSelf();
}

