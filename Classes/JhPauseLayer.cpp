#include "JhPauseLayer.h"
#include "JhGlobalData.h"
#include "JhSoundManager.h"
#include "JhStartScene.h"
#include "JhCommonFuncs.h"
#include "JhMapLayer.h"
#include "JhGameScene.h"
#include "JhConst.h"

bool JhPauseLayer::init()
{
	
    if (!Layer::init()) 
	{
		return false;
    }

	LayerColor* color = LayerColor::create(Color4B(0, 0, 0, OPACITY));
	this->addChild(color);

	Node* csbnode = CSLoader::createNode("jhpauseLayer.csb");
	this->addChild(csbnode);

	cocos2d::ui::Button* continuebtn = (cocos2d::ui::Button*)csbnode->getChildByName("continuebtn");
	continuebtn->addTouchEventListener(CC_CALLBACK_2(JhPauseLayer::onContinue, this));

	cocos2d::ui::Button* gohomebtn = (cocos2d::ui::Button*)csbnode->getChildByName("gohomebtn");
	gohomebtn->addTouchEventListener(CC_CALLBACK_2(JhPauseLayer::onGoHome, this));


	//////layer 点击事件，屏蔽下层事件
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};
	//点击任何位置移除掉
	listener->onTouchEnded = [=](Touch *touch, Event *event)
	{
		removSelf();
	};
	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    return true;
}

JhPauseLayer* JhPauseLayer::create()
{
	JhPauseLayer *pRet = new JhPauseLayer();
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

void JhPauseLayer::onContinue(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		removSelf();
	}
}

void JhPauseLayer::onGoHome(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		if (JhGlobalData::isOnline)
			JhServerDataSwap::init()->postOneData(JhGlobalData::getUId());
		removSelf();
		Director::getInstance()->replaceScene(JhStartScene::createScene());
	}
}

void JhPauseLayer::removSelf()
{
	this->removeFromParentAndCleanup(true);
	JhGlobalData::g_gameStatus = GAMESTART;
	if (g_maplayer != NULL && g_hero->getIsOut())
		g_maplayer->heroResumeMoving();
}

