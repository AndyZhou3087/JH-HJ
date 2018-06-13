#include "HomeScene.h"
#include "GameEvent.h"
#include "Resource.h"
#include "HomeMenuController.h"
#include "cocostudio/CocoStudio.h"
#include "URookieLayerController.h"
#include "Audio.h"
#include "GameIntroLayerController.h"
#include "UExitGiftLayerController.h"
#include "BgLayerController.h"
#include "GreyLayer.h"
#include "UExitLayerController.h"
#include "SettingManager.h"
#include "Pay.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//#include "PluginHelper.h"
#endif

USING_NS_CC;

#define LAYER_BG_Z_ORDER			-10
#define LAYER_HOME_MENU_Z_ORDER		-9
#define LAYER_GAME_BG_Z_ORDER		-8
#define LAYER_BG_GREY_Z_ORDER		-7
#define LAYER_INTRO_Z_ORDER			-6
#define LAYER_PANEL_Z_ORDER			-5
#define LAYER_EXIT_Z_ORDER			10

#define TAG_EXIT_DLG  100

HomeScene::HomeScene(void):
m_bg(NULL),
m_homeMenuLayer(NULL),
m_keyboradEventListener(NULL),
m_backTouched(false),
m_uiRookieLayer(NULL),
m_introLayer(NULL),
m_bgLayer(NULL),
m_bggreyLayer(NULL),
m_uiExitLayer(NULL),
m_uiExitGiftLayer(NULL),

m_uiRookieLayerController(NULL),
m_introLayerController(NULL),
m_bgLayerController(NULL),
m_greyLayerController(NULL),
m_uiExitLayerController(NULL),
m_uiExitGiftLayerController(NULL),

m_ExitCancelLis(NULL),
m_CloseIntroLis(NULL)
{
}


HomeScene::~HomeScene(void)
{
	delete m_homeMenuContrl;
	delete m_introLayerController;
	delete m_bgLayerController;
	delete m_uiRookieLayerController;
	delete m_greyLayerController;
	delete m_uiExitLayerController;
	delete m_uiExitGiftLayerController;

	Pay::unregRookieLis(m_uiRookieLayerController);
	Pay::unregHaoliLis(m_uiExitLayerController);
	Pay::unregHaoli2Lis(m_uiExitGiftLayerController);
}

Scene * HomeScene::createScene() {
	auto scene = Scene::create();
	auto mainLayer = HomeScene::create();
	scene->addChild(mainLayer);
	return scene;
}

bool HomeScene::init() {
	if (!CCLayer::init()) {
		return false;
	}

	do 
	{
		initBg();
		initUi();
		initController();
		return true;
	} while (0);

	return false;

}

void HomeScene::initBg() {
	const Size size = Director::getInstance()->getVisibleSize();
	m_bg = Sprite::create(s_bg_home);
	addChild(m_bg, LAYER_BG_Z_ORDER);
	float scaleX = size.width/m_bg->getContentSize().width;
	m_bg->setScaleX(scaleX);
	m_bg->setAnchorPoint(Vec2(0.5, 0));
	m_bg->setPosition(size.width * 0.5, 0);
}

void HomeScene::initUi() {


	m_bggreyLayer	= GreyLayer::create();
	addChild(m_bggreyLayer, LAYER_BG_GREY_Z_ORDER);
	m_homeMenuLayer	= CSLoader::createNode("HomeMenuLayer.csb");
	addChild(m_homeMenuLayer, LAYER_HOME_MENU_Z_ORDER);	
	m_bgLayer = BgLayer::create();
	addChild(m_bgLayer, LAYER_GAME_BG_Z_ORDER);
	m_introLayer	= CSLoader::createNode("GameIntroLayer.csb");
	addChild(m_introLayer, LAYER_INTRO_Z_ORDER);

	//退出界面
	if (0 == SettingManager::getInstance()->getExitSetting()) {
		//没有计费点
		m_uiExitLayer = CSLoader::createNode("UExit2Layer.csb");
	} else if (1 == SettingManager::getInstance()->getExitSetting()) {
		//计费点和退出UI合并
		m_uiExitLayer = CSLoader::createNode("UExitLayer.csb");
	} else if (2 == SettingManager::getInstance()->getExitSetting()) {
		//计费点和退出UI分开
		m_uiExitLayer = CSLoader::createNode("UExit2Layer.csb");
		m_uiExitGiftLayer = CSLoader::createNode("UExitGiftLayer.csb");
		addChild(m_uiExitGiftLayer, LAYER_EXIT_Z_ORDER);
	}
	addChild(m_uiExitLayer, LAYER_EXIT_Z_ORDER);
	initJFBg();
}

void HomeScene::initController() {
	m_homeMenuContrl = new HomeMenuController();
	m_homeMenuContrl->regMenu(m_homeMenuLayer);
	m_homeMenuContrl->enter();

	m_bgLayerController = new BgLayerController();
	m_bgLayerController->regLayer(m_bgLayer);
	m_bgLayerController->exit();

	m_greyLayerController = new GreyLayerController();
	m_greyLayerController->regLayer(m_bggreyLayer);
	m_greyLayerController->exit();

	m_introLayerController = new GameIntroLayerController();
	m_introLayerController->regLayer(m_introLayer);
	m_introLayerController->regBgLayer(m_greyLayerController);
	m_introLayerController->exit();
	m_homeMenuContrl->regIntroLayer(m_introLayerController);

	initJFController();
}

void HomeScene::initJFBg() {
	//新手礼包
	m_uiRookieLayer	= CSLoader::createNode("URookieLayer.csb");
	addChild(m_uiRookieLayer, LAYER_PANEL_Z_ORDER);
}

void HomeScene::initJFController() {
	m_uiRookieLayerController = new URookieLayerController();
	m_uiRookieLayerController->regLayer(m_uiRookieLayer);
	m_homeMenuContrl->regRookieLayer(m_uiRookieLayerController);
	m_uiRookieLayerController->regBgLayer(m_greyLayerController);
	m_uiRookieLayerController->exit();
	Pay::regRookieLis(m_uiRookieLayerController);

	//退出界面
	if (0 == SettingManager::getInstance()->getExitSetting()
		|| 1 == SettingManager::getInstance()->getExitSetting()) {
			//没有计费点
			//计费点和退出UI合并
			m_uiExitLayerController = new UExitLayerController();
			m_uiExitLayerController->regLayer(m_uiExitLayer);
			m_uiExitLayerController->exit();
			Pay::regHaoliLis(m_uiExitLayerController);
	} else if (2 == SettingManager::getInstance()->getExitSetting()) {
		//计费点和退出UI分开
		m_uiExitLayerController = new UExitLayerController();
		m_uiExitLayerController->regLayer(m_uiExitLayer);
		m_uiExitLayerController->exit();

		m_uiExitGiftLayerController = new UExitGiftLayerController();
		m_uiExitGiftLayerController->regLayer(m_uiExitGiftLayer);
		m_uiExitGiftLayerController->regNextLayer(m_uiExitLayerController);
		m_uiExitGiftLayerController->exit();
		Pay::regHaoli2Lis(m_uiExitGiftLayerController);
	}

}

void HomeScene::onEnter() {
	CCLayer::onEnter();

	regGameEvent();
	regSysEvent();

	Audio::getInstance()->playHomeBGM();
}

void HomeScene::onExit() {
	CCLayer::onExit();
	unregGameEvent();
	unregSysEvent();

	Audio::getInstance()->stopHomeBGM();
}

void HomeScene::regGameEvent() {
	//注册事件
	m_ExitCancelLis	= GameEvent::regEvent(Event_Exit_Cancel,	CC_CALLBACK_1(HomeScene::onExitCancel, this));
	m_CloseIntroLis	= GameEvent::regEvent(Event_Close_Intro,	CC_CALLBACK_1(HomeScene::onCloseIntro, this));
}

void HomeScene::unregGameEvent() {
	GameEvent::unregEvent(m_ExitCancelLis);
	GameEvent::unregEvent(m_CloseIntroLis);
}

void HomeScene::regSysEvent() {
	m_keyboradEventListener = EventListenerKeyboard::create();
	m_keyboradEventListener->onKeyReleased =[&](EventKeyboard::KeyCode keyCode, Event* event)
	{
		//返回键
		if (!m_backTouched) {
			if (nullptr != m_uiExitGiftLayerController) {
				m_uiExitGiftLayerController->enter();
			} else if (nullptr != m_uiExitLayerController) {
				m_uiExitLayerController->enter();
			}
			
			m_backTouched = true;
		} 
	};

	//很奇怪，用这种方式 真机上捕捉不到back
	//m_keyboradEventListener->onKeyReleased = CC_CALLBACK_2(MainScene::onKeyReleased,this);
	_eventDispatcher->addEventListenerWithFixedPriority(m_keyboradEventListener, 11);
}

void HomeScene::unregSysEvent() {
	_eventDispatcher->removeEventListener(m_keyboradEventListener);
}

void HomeScene::onExitCancel(cocos2d::EventCustom* event) {
	if (m_backTouched) {
		m_backTouched = false;
	}
}

void HomeScene::onCloseIntro(cocos2d::EventCustom*) {
	if (nullptr != m_introLayerController) {
		m_introLayerController->exit();
	}
}
