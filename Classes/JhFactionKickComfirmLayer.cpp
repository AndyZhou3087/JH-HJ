#include "JhFactionKickComfirmLayer.h"
#include "JhSoundManager.h"
#include "JhCommonFuncs.h"
#include "JhConst.h"
#include "MD5.h"
#include "JhHintBox.h"
#include "JhWaitingProgress.h"
#include "JhGameScene.h"
#include "JhFactionMemberLayer.h"

bool JhFactionKickComfirmLayer::init(Node* target, FactionMemberData *fmdata)
{
    if (!Layer::init()) 
	{
		return false;
    }

	m_data = fmdata;
	m_target = target;
	Node* csbnode = CSLoader::createNode("jhfactionComfirmLayer.csb");
	this->addChild(csbnode);

	cocos2d::ui::Button* okbtn = (cocos2d::ui::Button*)csbnode->getChildByName("okbtn");
	okbtn->addTouchEventListener(CC_CALLBACK_2(JhFactionKickComfirmLayer::onOk, this));

	cocos2d::ui::Button* cancelbtn = (cocos2d::ui::Button*)csbnode->getChildByName("cancelbtn");
	cancelbtn->addTouchEventListener(CC_CALLBACK_2(JhFactionKickComfirmLayer::onCancel, this));

	cocos2d::ui::Text* desctext = (cocos2d::ui::Text*)csbnode->getChildByName("content");
	std::string str = StringUtils::format("%s%s%s", JhCommonFuncs::gbk2utf("是否确认逐出").c_str(), fmdata->nickname.c_str(), JhCommonFuncs::gbk2utf("？").c_str());

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

JhFactionKickComfirmLayer* JhFactionKickComfirmLayer::create(Node* target, FactionMemberData *fmdata)
{
	JhFactionKickComfirmLayer *pRet = new JhFactionKickComfirmLayer();
	if (pRet && pRet->init(target, fmdata))
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

void JhFactionKickComfirmLayer::onOk(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{

		JhWaitingProgress* waitbox = JhWaitingProgress::create("处理中...");
		Director::getInstance()->getRunningScene()->addChild(waitbox, 1, "waitbox");
		JhServerDataSwap::init(this)->kickFaction(m_data->factionid, m_data->userid, m_data->herotype);
	}
}

void JhFactionKickComfirmLayer::onCancel(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		removSelf();
	}
}

void JhFactionKickComfirmLayer::removSelf()
{
	this->removeFromParentAndCleanup(true);
}

void JhFactionKickComfirmLayer::onSuccess()
{
	for (unsigned int i = 0; i < JhGlobalData::vec_factionListData.size(); i++)
	{
		if (JhGlobalData::vec_factionListData[i].id == m_data->factionid)
		{
			JhGlobalData::vec_factionListData[i].membercount--;
			break;
		}
	}
	FactionMemberItem* fmitem = (FactionMemberItem*)m_target;
	fmitem->removeItem();
	removSelf();
}

void JhFactionKickComfirmLayer::onErr(int errcode)
{
	Director::getInstance()->getRunningScene()->removeChildByName("waitbox");
	JhHintBox * box = JhHintBox::create(JhCommonFuncs::gbk2utf("数据处理异常，请检查网络连接！！"));
	Director::getInstance()->getRunningScene()->addChild(box,1);
	removSelf();
}

