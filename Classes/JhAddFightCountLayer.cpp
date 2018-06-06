#include "JhAddFightCountLayer.h"
#include "JhCommonFuncs.h"
#include "JhHintBox.h"
#include "WaitingProgress.h"
#include "JhConst.h"
#include "SoundManager.h"
#include "MD5.h"
#include "JhHSLJMainLayer.h"
#include "JhGameScene.h"

JhAddFightCountLayer::JhAddFightCountLayer()
{
	buycount = 0;
	m_matchtype = 0;
}


JhAddFightCountLayer::~JhAddFightCountLayer()
{

}


JhAddFightCountLayer* JhAddFightCountLayer::create(int matchtype)
{
	JhAddFightCountLayer *pRet = new JhAddFightCountLayer();
	if (pRet && pRet->init(matchtype))
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

bool JhAddFightCountLayer::init(int matchtype)
{
	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 200));
	this->addChild(color);

	m_csbnode = CSLoader::createNode("jhaddChallengeCountLayer.csb");
	this->addChild(m_csbnode);

	cocos2d::ui::Widget *closebtn = (cocos2d::ui::Widget*)m_csbnode->getChildByName("closebtn");
	closebtn->addTouchEventListener(CC_CALLBACK_2(JhAddFightCountLayer::onBack, this));


	cocos2d::ui::Widget *buy1btn = (cocos2d::ui::Button*)m_csbnode->getChildByName("buy1btn");
	buy1btn->addTouchEventListener(CC_CALLBACK_2(JhAddFightCountLayer::onAddCount, this));
	buy1btn->setTag(1);

	cocos2d::ui::Widget *buy5btn = (cocos2d::ui::Button*)m_csbnode->getChildByName("buy5btn");
	buy5btn->addTouchEventListener(CC_CALLBACK_2(JhAddFightCountLayer::onAddCount, this));
	buy5btn->setTag(5);

	m_matchtype = matchtype;

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};
	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void JhAddFightCountLayer::onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}

void JhAddFightCountLayer::onAddCount(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		Node* btnnode = (Node*)pSender;
		buycount = btnnode->getTag();
		int needgold = 20;

		if (buycount == 5)
			needgold = 80;
		int mygold = JhGlobalData::getMyGoldCount();
		if (mygold >= needgold)
		{
			if (JhGlobalData::getMD5MyGoldCount().compare(md5(mygold)) != 0)
			{
				JhGlobalData::dataIsModified = true;
				JhGlobalData::setMyGoldCount(0);
				JhHintBox* hint = JhHintBox::create(JhCommonFuncs::gbk2utf("发现有作弊行为，金元宝清零作为处罚！！"));
				this->addChild(hint);
				return;
			}
		}
		else
		{
			JhHintBox* hbox = JhHintBox::create(JhCommonFuncs::gbk2utf("金元宝不足！"));
			this->addChild(hbox);
			return;
		}

		WaitingProgress* waitbox = WaitingProgress::create("处理中...");
		Director::getInstance()->getRunningScene()->addChild(waitbox, 1, "waitbox");

		ServerDataSwap::init(this)->getFightCount(m_matchtype, buycount);
	}
}

void JhAddFightCountLayer::onSuccess()
{
	Director::getInstance()->getRunningScene()->removeChildByName("waitbox");
	if (m_matchtype == 0)
	{
		JhGlobalData::myTotalFihgtCount += buycount;
		JhGlobalData::myFihgtCount += buycount;
	}
	else if (m_matchtype == 1)
	{
		JhGlobalData::myMatchInfo.leftcount += buycount;
		JhHSLJMainLayer* layer = (JhHSLJMainLayer*)g_gameLayer->getChildByName("hsljmainlayer");
		if (layer != NULL)
			layer->updateMyFightCount();
	}
	int needgold = 20;
	int mygold = JhGlobalData::getMyGoldCount();
	if (buycount == 5)
	{
		needgold = 80;
	}
	JhGlobalData::setMyGoldCount(mygold - needgold);
}

void JhAddFightCountLayer::onErr(int errcode)
{
	Director::getInstance()->getRunningScene()->removeChildByName("waitbox");
	JhHintBox * box = JhHintBox::create(JhCommonFuncs::gbk2utf("数据获取异常，请检查网络连接！！"));
	this->addChild(box);
}
