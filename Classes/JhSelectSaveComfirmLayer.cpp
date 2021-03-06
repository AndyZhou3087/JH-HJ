﻿#include "JhSelectSaveComfirmLayer.h"
#include "JhGlobalData.h"
#include "JhSoundManager.h"
#include "JhGameDataSave.h"
#include "JhGameScene.h"
#include "JhCommonFuncs.h"
#include "JhConst.h"
bool JhSelectSaveComfirmLayer::init(int index)
{
	
    if (!Layer::init()) 
	{
		return false;
    }

	LayerColor* color = LayerColor::create(Color4B(0, 0, 0, OPACITY));
	this->addChild(color);

	Node* csbnode = CSLoader::createNode("jhselectSaveComfirmLayer.csb");
	this->addChild(csbnode);

	cocos2d::ui::Button* okbtn = (cocos2d::ui::Button*)csbnode->getChildByName("okbtn");
	okbtn->addTouchEventListener(CC_CALLBACK_2(JhSelectSaveComfirmLayer::onOk, this));
	okbtn->setTag(index);
	cocos2d::ui::Button* cancelbtn = (cocos2d::ui::Button*)csbnode->getChildByName("cancelbtn");
	cancelbtn->addTouchEventListener(CC_CALLBACK_2(JhSelectSaveComfirmLayer::onCancel, this));

	cocos2d::ui::Text* nametxt = (cocos2d::ui::Text*)csbnode->getChildByName("name");
	nametxt->setString(JhCommonFuncs::gbk2utf(heroname[index].c_str()));

	cocos2d::ui::ImageView* headimg = (cocos2d::ui::ImageView*)csbnode->getChildByName("headimg");
	std::string headstr = StringUtils::format("ui/tophero%d.png", index + 1);
	headimg->loadTexture(headstr, cocos2d::ui::TextureResType::PLIST);

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

JhSelectSaveComfirmLayer* JhSelectSaveComfirmLayer::create(int index)
{
	JhSelectSaveComfirmLayer *pRet = new JhSelectSaveComfirmLayer();
	if (pRet && pRet->init(index))
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


void JhSelectSaveComfirmLayer::onOk(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		Node* node = (Node*)pSender;
		int tag = node->getTag();
		std::string suid = JhGlobalData::getSaveListId().at(tag);
		JhGlobalData::setUId(suid);

		Scene* scene = JhGameScene::createScene();

		Director::getInstance()->replaceScene(scene);
	}
}

void JhSelectSaveComfirmLayer::onCancel(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{	
		removSelf();
	}
}

void JhSelectSaveComfirmLayer::removSelf()
{
	this->removeFromParentAndCleanup(true);
}

