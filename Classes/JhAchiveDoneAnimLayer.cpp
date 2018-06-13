#include "JhAchiveDoneAnimLayer.h"
#include "JhGlobalData.h"
#include "JhCommonFuncs.h"
#include "JhGameScene.h"
bool JhAchiveDoneAnimLayer::init()
{
	
    if (!Layer::init()) 
	{
		return false;
    }

	//蒙版
	LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 150));
	this->addChild(color);

	showAchiveDoneAnim(0);
	//////layer 点击事件，屏蔽下层事件
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};
	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    return true;
}

JhAchiveDoneAnimLayer* JhAchiveDoneAnimLayer::create()
{
	JhAchiveDoneAnimLayer *pRet = new JhAchiveDoneAnimLayer();
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

void JhAchiveDoneAnimLayer::showAchiveDoneAnim(float dt)
{
	this->removeChildByName("achiveanim");
	if (JhGlobalData::vec_showAchiveNames.size() <= 0)
	{
		JhGlobalData::saveAchiveAnimData();
		if (g_gameLayer != NULL)
			g_gameLayer->schedule(schedule_selector(JhGameScene::checkAchiveIsDone), 1.0f);
		removSelf();
		return;
	}

	Node* csbnode = CSLoader::createNode("jhachiveNodeAnim.csb");
	csbnode->setPosition(Vec2(360, 720));
	this->addChild(csbnode, 0, "achiveanim");
	cocos2d::ui::Text* textname = (cocos2d::ui::Text*)csbnode->getChildByName("name");
	textname->setString(JhGlobalData::vec_showAchiveNames[0]);
	auto action = CSLoader::createTimeline("jhachiveNodeAnim.csb");
	csbnode->runAction(action);
	action->gotoFrameAndPlay(0, false);

	JhGlobalData::vec_showAchiveNames.erase(JhGlobalData::vec_showAchiveNames.begin());
	float delaytime = action->getEndFrame()*1.0f / (action->getTimeSpeed() * 60);
	this->schedule(schedule_selector(JhAchiveDoneAnimLayer::showAchiveDoneAnim), delaytime, JhGlobalData::vec_showAchiveNames.size(), 0);
}

void JhAchiveDoneAnimLayer::removSelf()
{
	this->removeFromParentAndCleanup(true);
}

