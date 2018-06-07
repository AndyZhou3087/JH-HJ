#include "JhDeathLayer.h"
#include "JhCommonFuncs.h"
#include "JhStartScene.h"
#include "JhSoundManager.h"
#include "JhConst.h"
#include "JhAnalyticUtil.h"
#include "JhGameScene.h"
JhDeathLayer::JhDeathLayer()
{

}


JhDeathLayer::~JhDeathLayer()
{

}

bool JhDeathLayer::init()
{
	LayerColor* color = LayerColor::create(Color4B(0, 0, 0, 255));
	this->addChild(color);

	Node* csbnode = CSLoader::createNode("jhdeathLayer.csb");
	this->addChild(csbnode);

	csbnode->runAction(FadeIn::create(3.0f));

	////layer 点击事件，屏蔽下层事件
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->onTouchEnded = [=](Touch *touch, Event *event)
	{
		Director::getInstance()->replaceScene(JhStartScene::createScene());
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	JhSoundManager::getInstance()->playBackMusic(JhSoundManager::MUSIC_ID_DEATH);

#ifdef ANALYTICS
	std::string pastdaystr = StringUtils::format("pastday%d", g_nature->getPastDays());
	JhAnalyticUtil::onEvent(pastdaystr.c_str());
#endif

	return true;
}

JhDeathLayer* JhDeathLayer::create()
{
	JhDeathLayer *pRet = new JhDeathLayer();
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