#include "JhSpecialHintLayer.h"
#include "JhGlobalData.h"
#include "JhSoundManager.h"
#include "JhCommonFuncs.h"
#include "JhGameScene.h"
#include "JhHeroProperNode.h"
#include "JhGameDataSave.h"

bool JhSpecialHintLayer::init()
{
	
    if (!Layer::init()) 
	{
		return false;
    }

	Node* csbnode = CSLoader::createNode("jhsepcialLayer.csb");
	this->addChild(csbnode);

	cocos2d::ui::Button* okbtn = (cocos2d::ui::Button*)csbnode->getChildByName("okbtn");
	okbtn->addTouchEventListener(CC_CALLBACK_2(JhSpecialHintLayer::onOk, this));

	cocos2d::ui::Button* cancelbtn = (cocos2d::ui::Button*)csbnode->getChildByName("cancelbtn");
	cancelbtn->addTouchEventListener(CC_CALLBACK_2(JhSpecialHintLayer::onCancel, this));

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

JhSpecialHintLayer* JhSpecialHintLayer::create()
{
	JhSpecialHintLayer *pRet = new JhSpecialHintLayer();
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

void JhSpecialHintLayer::onOk(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		if (g_hero->getHeadID() != 4)
		{
			g_hero->setSex(S_NONE);
			JhGameDataSave::getInstance()->setHeroSex(S_NONE);
		}

		JhHeroProperNode* heroProperNode = (JhHeroProperNode*)g_gameLayer->getChildByName("JhHeroStateUILayer")->getChildByName("csbnode")->getChildByName("JhHeroProperNode");
		heroProperNode->selectCarryData();
		removSelf();
	}
}

void JhSpecialHintLayer::removSelf()
{
	this->removeFromParentAndCleanup(true);
}

void JhSpecialHintLayer::onCancel(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		removSelf();
	}
}

