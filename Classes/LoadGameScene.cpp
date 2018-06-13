#include "LoadGameScene.h"
#include "cocos2d.h"
#include "ULoadGameLayerController.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "GameScene.h"
using namespace ui;
using namespace cocostudio;

USING_NS_CC;


LoadGameScene::LoadGameScene():
m_loadLayerController(NULL),
m_timeCount(0),
m_loadUiFinished(false)
{

}

LoadGameScene::~LoadGameScene()
{
	delete m_loadLayerController;
}

Scene* LoadGameScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = LoadGameScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

void LoadGameScene::onEnter() {
	LayerColor::onEnter();
	m_timeCount = 0;
	schedule(schedule_selector(LoadGameScene::onTimer), 0.5);
	schedule(schedule_selector(LoadGameScene::onLoadUi), 0.1);
}

void LoadGameScene::onExit()  {
	LayerColor::onExit();
	unschedule(schedule_selector(LoadGameScene::onTimer));
	unschedule(schedule_selector(LoadGameScene::onLoadUi));
}

// on "init" you need to initialize your instance
bool LoadGameScene::init()
{

	if ( !CCLayerColor::initWithColor(Color4B::BLACK) )
    {
        return false;
    }
    
	auto loadLayer	= CSLoader::createNode("ULoadGameLayer.csb");
	addChild(loadLayer);
	
	m_loadLayerController = new ULoadGameLayerController();
	m_loadLayerController->regLayer(loadLayer);
	m_loadLayerController->enter();

    return true;
}

void LoadGameScene::onTimer(float dt) {
	if (m_timeCount>3) {
		m_timeCount = 0;
	}

	if (nullptr != m_loadLayerController) {
		m_loadLayerController->updatePot(m_timeCount);
	}
	m_timeCount++;
}

void LoadGameScene::onLoadUi(float dt) {
	if (!m_loadUiFinished && GameScene::loadUi()) {
		m_loadUiFinished = true;
	Director::sharedDirector()->replaceScene(GameScene::createScene());
	}
}