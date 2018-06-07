#include "JhHuafeiAwardListLayer.h"
#include "JhCommonFuncs.h"
#include "JhGameScene.h"
#include "JhHintBox.h"
#include "JhWaitingProgress.h"
#include "JhConst.h"
#include "JhSoundManager.h"

JhHuafeiAwardListLayer::JhHuafeiAwardListLayer()
{
}


JhHuafeiAwardListLayer::~JhHuafeiAwardListLayer()
{
}


JhHuafeiAwardListLayer* JhHuafeiAwardListLayer::create()
{
	JhHuafeiAwardListLayer *pRet = new JhHuafeiAwardListLayer();
	if (pRet && pRet->init())
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

bool JhHuafeiAwardListLayer::init()
{
	Node* csbnode = CSLoader::createNode("jhhuafeiAwardListLayer.csb");
	this->addChild(csbnode);

	cocos2d::ui::Widget *backbtn = (cocos2d::ui::Widget*)csbnode->getChildByName("backbtn");
	backbtn->addTouchEventListener(CC_CALLBACK_2(JhHuafeiAwardListLayer::onBack, this));

	resultlbl = (cocos2d::ui::Text*)csbnode->getChildByName("resultlbl");
	resultlbl->setString("");

	cocos2d::ui::Text* desclbl = (cocos2d::ui::Text*)csbnode->getChildByName("desc");
	desclbl->setString(JhGlobalData::couponinfo);

	getData();

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};
	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void JhHuafeiAwardListLayer::onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}
void JhHuafeiAwardListLayer::getData()
{
	JhWaitingProgress* waitbox = JhWaitingProgress::create("加载中...");
	Director::getInstance()->getRunningScene()->addChild(waitbox, 1, "waitbox");

	JhServerDataSwap::init(this)->getKajuanAwardList();

}

void JhHuafeiAwardListLayer::delayShowData(float dt)
{
	Director::getInstance()->getRunningScene()->removeChildByName("waitbox");
	std::string str;
	if (JhGlobalData::myLastHuafeiRank > 0)
	{
		str = StringUtils::format("恭喜你！打败扫地僧排名第%d名！", JhGlobalData::myLastHuafeiRank);
	}
	else
	{
		str = "很遗憾！未排名上榜，再接再励！";
		resultlbl->setTextColor(Color4B(27, 141, 0, 255));
	}

	resultlbl->setString(JhCommonFuncs::gbk2utf(str.c_str()));
}

void JhHuafeiAwardListLayer::onSuccess()
{
	this->scheduleOnce(schedule_selector(JhHuafeiAwardListLayer::delayShowData), 0.1f);
}

void JhHuafeiAwardListLayer::onErr(int errcode)
{
	Director::getInstance()->getRunningScene()->removeChildByName("waitbox");
	JhHintBox * box = JhHintBox::create(JhCommonFuncs::gbk2utf("数据获取异常，请检查网络连接！！"));
	this->addChild(box);
}