#include "JhExerciseCancelLayer.h"
#include "JhSoundManager.h"
#include "JhBuildingUILayer.h"
#include "JhCommonFuncs.h"
bool JhExerciseCancelLayer::init()
{
	
    if (!Layer::init()) 
	{
		return false;
    }

	Node* csbnode = CSLoader::createNode("jhexerciseCancelLayer.csb");
	this->addChild(csbnode);

	cocos2d::ui::Button* okbtn = (cocos2d::ui::Button*)csbnode->getChildByName("okbtn");
	okbtn->addTouchEventListener(CC_CALLBACK_2(JhExerciseCancelLayer::onOk, this));

	cocos2d::ui::Button* cancelbtn = (cocos2d::ui::Button*)csbnode->getChildByName("cancelbtn");
	cancelbtn->addTouchEventListener(CC_CALLBACK_2(JhExerciseCancelLayer::onCancel, this));
	
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

void JhExerciseCancelLayer::onOk(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		JhBuildingUILayer* parent = (JhBuildingUILayer*)this->getParent();
		parent->resetExercise();
		removSelf();
	}
}

void JhExerciseCancelLayer::onCancel(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		removSelf();
	}
}

void JhExerciseCancelLayer::removSelf()
{
	this->removeFromParentAndCleanup(true);
}

