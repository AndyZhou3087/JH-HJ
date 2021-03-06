﻿#include "JhHelpDescLayer.h"
#include "JhCommonFuncs.h"
#include "JhConst.h"

JhHelpDescLayer::JhHelpDescLayer()
{
	
}


JhHelpDescLayer::~JhHelpDescLayer()
{
	
}

bool JhHelpDescLayer::init( std::string title, std::string text)
{
	LayerColor* color = LayerColor::create(Color4B(0, 0, 0, OPACITY));
	this->addChild(color);

	Node* csbnode = CSLoader::createNode("jhhelpDescLayer.csb");
	this->addChild(csbnode);

	cocos2d::ui::Text* titleTxt = (cocos2d::ui::Text*)csbnode->getChildByName("title");
	titleTxt->setString(title);

	cocos2d::ui::Text* contentTxt = (cocos2d::ui::Text*) csbnode->getChildByName("content");
	contentTxt->setString(text);

	cocos2d::ui::Widget *okbtn = (cocos2d::ui::Widget*)csbnode->getChildByName("okbtn");
	okbtn->addTouchEventListener(CC_CALLBACK_2(JhHelpDescLayer::onOk, this));

	//layer 点击事件，屏蔽下层事件
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	return true;
}

JhHelpDescLayer* JhHelpDescLayer::create(std::string title, std::string text)
{
	JhHelpDescLayer *pRet = new JhHelpDescLayer();
	if (pRet && pRet->init(title, text))
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

void JhHelpDescLayer::onOk(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
	}
}