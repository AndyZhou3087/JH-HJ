#include "GameScene.h"
#include "GameEvent.h"
#include "global.h"
#include "GameUtil.h"
#include "Advertisement.h"
#include "Analytics.h"
#include "GameUiManager.h"

#include "GreyLayer.h"
#include "GameTopLayerController.h"
#include "GameBottomLayerController.h"
#include "GameLayerController.h"
#include "GameLevelLayerController.h"
#include "GameTargetLayerController.h"
#include "GameLotteryLayerController.h"
#include "GameSuccessLayerController.h"
#include "GameFailLayerController.h"
#include "GameEnergyTipLayerController.h"
#include "GameBuyTimeLayerController.h"
#include "GamePauseLayerController.h"
#include "GameTimeoutLayerController.h"
#include "GamePurchaseLayerController.h"
#include "GameIntroLayerController.h"
#include "BgLayerController.h"
#include "GameLevelDemoLayerController.h"
#include "GameBossLayerController.h"

#include "URookieLayerController.h"
#include "ULuckyLayerController.h"
#include "UAwardLayerController.h"
#include "UDiscountLayerController.h"
#include "USupriseLayerController.h"
#include "UGiftLayerController.h"
#include "UBigAwardLayerController.h"
#include "UExitLayerController.h"
#include "ULuckyGiftLayerController.h"
#include "UUnlockLayerController.h"
#include "UIntroAwardLayerController.h"
#include "UTipWinLayerController.h"
#include "UToolConfirmLayerController.h"
#include "UGiftMoreLayerController.h"
#include "UExitGiftLayerController.h"
#include "UGuideTipLayerController.h"

#include "cocostudio/CocoStudio.h"
#include "Resource.h"
#include "Audio.h"
#include "GameManager.h"
#include "GameData.h"
#include "EnergyManager.h"
#include "UserData.h"
#include "LuckyTimeManager.h"
#include "SettingManager.h"
#include "Pay.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//#include "PluginHelper.h"
#endif

USING_NS_CC;

#define LAYER_BG_Z_ORDER			-20

#define LAYER_GAME_Z_ORDER			-17
#define LAYER_BOTTOM_Z_ORDER		-18
#define LAYER_GAME_DEMO_Z_ORDER		-17
#define LAYER_TOP_Z_ORDER			-16
#define LAYER_LEVEL_Z_ORDER			-15
#define LAYER_LEVEL_DEMO_Z_ORDER	-14
#define LAYER_BG_GREY_Z_ORDER		-13
#define LAYER_DEMO_BG_Z_ORDER		-12
#define LAYER_DEMO_Z_ORDER			-11
#define LAYER_PANEL_Z_ORDER			-10
#define LAYER_PAUSE_MENU_Z_ORDER	-9
#define LAYER_OVER_MENU_Z_ORDER		10
#define LAYER_EXIT_Z_ORDER			11

#define TAG_EXIT_DLG  100


#define CREATE_NODE(avar, csb) \
if (nullptr == avar) { \
	avar = CSLoader::createNode(csb); \
	avar->retain(); \
	return false; \
}

cocos2d::Node *GameScene::m_topLayer			= NULL;
cocos2d::Node *GameScene::m_bottomLayer			= NULL;
cocos2d::Node *GameScene::m_gameLayer			= NULL;
cocos2d::Node *GameScene::m_levelLayer			= NULL;
cocos2d::Node *GameScene::m_targetLayer			= NULL;
cocos2d::Node *GameScene::m_targetBossLayer		= NULL;
cocos2d::Node *GameScene::m_lotteryLayer		= NULL;
cocos2d::Node *GameScene::m_successLayer		= NULL;
cocos2d::Node *GameScene::m_failLayer			= NULL;
cocos2d::Node *GameScene::m_buytimeLayer		= NULL;
cocos2d::Node *GameScene::m_energytipLayer		= NULL;
cocos2d::Node *GameScene::m_purchaseLayer		= NULL;
cocos2d::Node *GameScene::m_pauseLayer			= NULL;
cocos2d::Node *GameScene::m_timeoutLayer		= NULL;
cocos2d::Node *GameScene::m_giftLayer			= NULL;
cocos2d::Node *GameScene::m_gameBossLayer		= NULL;
cocos2d::Node *GameScene::m_introLayer			= NULL;

cocos2d::Node *GameScene::m_welcome				= NULL;
cocos2d::Node *GameScene::m_welcomeEnd			= NULL;
cocos2d::Node *GameScene::m_crossGuide			= NULL;
cocos2d::Node *GameScene::m_bombGuide			= NULL;
cocos2d::Node *GameScene::m_timeGuide			= NULL;
cocos2d::Node *GameScene::m_tipGuide			= NULL;
cocos2d::Node *GameScene::m_specialPetGuide		= NULL;
cocos2d::Node *GameScene::m_bossTalk			= NULL;
cocos2d::Node *GameScene::m_girlTalk			= NULL;
cocos2d::Node *GameScene::m_bossTip				= NULL;
cocos2d::Node *GameScene::m_touchErr			= NULL;

cocos2d::Node *GameScene::m_uiRookieLayer		= NULL;
cocos2d::Node *GameScene::m_uiLuckyLayer		= NULL;
cocos2d::Node *GameScene::m_uiAwardLayer		= NULL;
cocos2d::Node *GameScene::m_uiDiscountLayer		= NULL;
cocos2d::Node *GameScene::m_uiSupriseLayer		= NULL;
cocos2d::Node *GameScene::m_uiBigAwardLayer		= NULL;
cocos2d::Node *GameScene::m_uiExitLayer			= NULL;
cocos2d::Node *GameScene::m_uiExitGiftLayer		= NULL;
cocos2d::Node *GameScene::m_uiLuckyGiftLayer	= NULL;
cocos2d::Node *GameScene::m_uiUnlockLayer		= NULL;
cocos2d::Node *GameScene::m_uiIntroAwardLayer	= NULL;
cocos2d::Node *GameScene::m_uiTipwinLayer		= NULL;
cocos2d::Node *GameScene::m_uiToolConfirmLayer	= NULL;
cocos2d::Node *GameScene::m_uiGiftMoreLayer		= NULL;

GameScene::GameScene(void):
m_bg(NULL),
m_bgGround(NULL),
m_bggreyLayer(NULL),
m_introBgLayer(NULL),
m_levelDemoLayer(NULL),

m_topLayerController(NULL),
m_bottomLayerController(NULL),
m_gameLayerController(NULL),
m_levelLayerController(NULL),
m_targetLayerController(NULL),
m_targetBossLayerController(NULL),
m_lotteryLayerController(NULL),
m_successLayerController(NULL),
m_failLayerController(NULL),
m_buytimeLayerController(NULL),
m_energytipLayerController(NULL),
m_pauseLayerController(NULL),
m_timeoutLayerController(NULL),
m_giftLayerController(NULL),
m_purchaseLayerController(NULL),
m_greyLayerController(NULL),
m_introLayerController(NULL),
m_introBgLayerController(NULL),
m_levelDemoLayerController(NULL),
m_gameBossLayerController(NULL),

m_uiRookieLayerController(NULL),
m_uiLuckyLayerController(NULL),
m_uiAwardLayerController(NULL),
m_uiDiscountLayerController(NULL),
m_uiSupriseLayerController(NULL),
m_uiBigAwardLayerController(NULL),
m_uiExitLayerController(NULL),
m_uiLuckyGiftLayerController(NULL),
m_uiUnlockLayerController(NULL),
m_uiIntroAwardLayerController(NULL),
m_uiTipwinLayerController(NULL),
m_uiToolConfirmLayerController(NULL),
m_uiGiftMoreLayerController(NULL),
m_uiExitGiftLayerController(NULL),
m_uiGuideTipLayerController(NULL),

m_GameStartLis(NULL),
m_GameRestartLis(NULL),
m_GameOverLis(NULL),
m_GameResumeLis(NULL),
m_GamePauseLis(NULL),
m_ExitCancelLis(NULL),
m_GameReadyLis(NULL),
m_GameReliveLis(NULL),
m_GameScoreChangeLis(NULL),
m_GameToolChangeLis(NULL),
m_ReleaseCrossLis(NULL),
m_EnrgyChangeLis(NULL),
m_CloseIntroLis(NULL),
m_LevelUnlockLis(NULL),
m_TimeChangeLis(NULL),
m_GameWinLis(NULL),
m_AttrackBossLis(NULL),
m_GameFinishLis(NULL),

m_touchEventListener(NULL),
m_keyboradEventListener(NULL),
m_backTouched(false)
{
    m_currentTime = getCurrentTime();
}


GameScene::~GameScene(void)
{
	Pay::removeAllLis();

	delete m_topLayerController;
	delete m_bottomLayerController;
	delete m_gameLayerController;
	delete m_levelLayerController;
	delete m_targetLayerController;
	delete m_targetBossLayerController;
	delete m_lotteryLayerController;
	delete m_successLayerController;
	delete m_failLayerController;
	delete m_buytimeLayerController;
	delete m_timeoutLayerController;
	delete m_energytipLayerController;
	delete m_pauseLayerController;
	delete m_giftLayerController;
	delete m_greyLayerController;
	delete m_purchaseLayerController;
	delete m_introLayerController;
	delete m_introBgLayerController;
	delete m_levelDemoLayerController;
	delete m_gameBossLayerController;

	delete m_uiRookieLayerController;
	delete m_uiLuckyLayerController;
	delete m_uiAwardLayerController;
	delete m_uiDiscountLayerController;
	delete m_uiSupriseLayerController;
	delete m_uiBigAwardLayerController;
	delete m_uiExitLayerController;
	delete m_uiLuckyGiftLayerController;
	delete m_uiUnlockLayerController;
	delete m_uiIntroAwardLayerController;
	delete m_uiTipwinLayerController;
	delete m_uiToolConfirmLayerController;
	delete m_uiGiftMoreLayerController;
	delete m_uiExitGiftLayerController;
	delete m_uiGuideTipLayerController;

	GameUiManager::getInstance()->unregAllUiController();
	m_topLayer			= NULL;
	m_bottomLayer		= NULL;
	m_gameLayer			= NULL;
	m_levelLayer		= NULL;
	m_targetLayer		= NULL;
	m_targetBossLayer	= NULL;
	m_lotteryLayer		= NULL;
	m_successLayer		= NULL;
	m_failLayer			= NULL;
	m_buytimeLayer		= NULL;
	m_energytipLayer	= NULL;
	m_pauseLayer		= NULL;
	m_timeoutLayer		= NULL;
	m_pauseLayer		= NULL;
	m_giftLayer			= NULL;
	m_gameBossLayer		= NULL;
	m_introLayer		= NULL;

	m_welcome			= NULL;
	m_welcomeEnd		= NULL;
	m_crossGuide		= NULL;
	m_bombGuide			= NULL;
	m_timeGuide			= NULL;
	m_tipGuide			= NULL;
	m_specialPetGuide	= NULL;
	m_bossTalk			= NULL;
	m_girlTalk			= NULL;
	m_bossTip			= NULL;
	m_touchErr			= NULL;

	m_uiRookieLayer			= NULL;
	m_uiLuckyLayer			= NULL;
	m_uiAwardLayer			= NULL;
	m_uiDiscountLayer		= NULL;
	m_uiSupriseLayer		= NULL;
	m_uiBigAwardLayer		= NULL;
	m_uiExitLayer			= NULL;
	m_uiExitGiftLayer		= NULL;
	m_uiLuckyGiftLayer		= NULL;
	m_uiUnlockLayer			= NULL;
	m_uiIntroAwardLayer		= NULL;
	m_uiTipwinLayer			= NULL;
	m_uiToolConfirmLayer	= NULL;
	m_uiGiftMoreLayer		= NULL;

}

Scene * GameScene::createScene() {
	auto scene = Scene::create();
	auto mainLayer = GameScene::create();
	scene->addChild(mainLayer);
	return scene;
}

bool GameScene::init() {
	if (!CCLayer::init()) {
		return false;
	}

	do 
	{
		initBg();
		initUi();
		initController();
		EnergyManager::getInstance()->init();
		return true;
	} while (0);

	return false;

}

void GameScene::initBg() {
	const Size size = Director::getInstance()->getVisibleSize();
	m_bg = Sprite::create(s_bg_game);
	addChild(m_bg, LAYER_BG_Z_ORDER);
	//不能拉伸 否则宠物放的位置就不对了
	//float scaleX = size.width/m_bg->getContentSize().width;
	//float scaleY = size.height/m_bg->getContentSize().height;
	//m_bg->setScale(scaleX, scaleY);
	m_bg->setAnchorPoint(Vec2(0.5, 0));
	m_bg->setPosition(size.width * 0.5, 0);

	m_bgGround = Sprite::create(s_bg_ground);
	m_bg->addChild(m_bgGround);
	m_bgGround->setPosition(m_bg->getContentSize().width * 0.5, 144 + m_bgGround->getContentSize().height * 0.5);

	auto tree = Sprite::create(s_bg_tree);
	m_bg->addChild(tree);
	tree->setAnchorPoint(Vec2(1,0.5));
	tree->setPosition(size.width, 120);

}

bool GameScene::loadUi() {
	CREATE_NODE(m_gameLayer,		"GameLayer.csb")
	CREATE_NODE(m_topLayer,			"GameTopLayer.csb")
	CREATE_NODE(m_bottomLayer,		"GameBottomLayer.csb")
	CREATE_NODE(m_levelLayer,		"GameLevelLayer.csb")
	CREATE_NODE(m_giftLayer,		"UGiftLayer.csb")
	CREATE_NODE(m_targetLayer,		"GameTargetLayer.csb")
	CREATE_NODE(m_targetBossLayer,	"GameBossTargetLayer.csb")
	CREATE_NODE(m_lotteryLayer,		"GameLotteryLayer.csb")
	CREATE_NODE(m_successLayer,		"GameSuccessLayer.csb")
	CREATE_NODE(m_buytimeLayer,		"GameBuyTimeLayer.csb")
	CREATE_NODE(m_purchaseLayer,	"GamePurchaseLayer.csb")
	CREATE_NODE(m_energytipLayer,	"GameEnergyTipLayer.csb")
	CREATE_NODE(m_pauseLayer,		"GamePauseLayer.csb")
	CREATE_NODE(m_timeoutLayer,		"GameTimeoutLayer.csb")
	CREATE_NODE(m_gameBossLayer,	"GameBossLayer.csb")
	CREATE_NODE(m_introLayer,		"GameIntroLayer.csb")
	CREATE_NODE(m_welcome,			"UStepWelcomeLayer.csb")
	CREATE_NODE(m_welcomeEnd,		"UWelcomeEndLayer.csb")
	CREATE_NODE(m_crossGuide,		"UStepCrossLayer.csb")
	CREATE_NODE(m_bombGuide,		"UStepBombLayer.csb")
	CREATE_NODE(m_timeGuide,		"UStepTimeLayer.csb")
	CREATE_NODE(m_tipGuide,			"UStepTipLayer.csb")
	CREATE_NODE(m_specialPetGuide,	"UStepSpecialPetLayer.csb")
	CREATE_NODE(m_bossTalk,			"UStepBossSpeakLayer.csb")
	CREATE_NODE(m_girlTalk,			"UStepGirlSpeakLayer.csb")
	CREATE_NODE(m_bossTip,			"UStepBossTipLayer.csb")
	CREATE_NODE(m_touchErr,			"UStepTouchErrLayer.csb")

	CREATE_NODE(m_uiRookieLayer,		"URookieLayer.csb")
	CREATE_NODE(m_uiLuckyLayer,			"ULuckyLayer.csb")
	CREATE_NODE(m_uiAwardLayer,			"UAwardLayer.csb")
	CREATE_NODE(m_uiSupriseLayer,		"USupriseLayer.csb")
	CREATE_NODE(m_uiBigAwardLayer,		"UBigAwardLayer.csb")
	CREATE_NODE(m_uiLuckyGiftLayer,		"ULuckyGiftLayer.csb")
	CREATE_NODE(m_uiUnlockLayer,		"UUnlockLayer.csb")
	CREATE_NODE(m_uiIntroAwardLayer,	"UIntroAwardLayer.csb")
	CREATE_NODE(m_uiTipwinLayer,		"UTipWinLayer.csb")

	if (SettingManager::getInstance()->isOpen(Setting_nextgame)) {
		CREATE_NODE(m_failLayer,		"GameFailLayer.csb")
	} else {
		CREATE_NODE(m_failLayer,		"GameFail2Layer.csb")
	}

	if (SettingManager::getInstance()->isOpen(Setting_discount)) {
		CREATE_NODE(m_uiDiscountLayer,		"UDiscountLayer.csb")
	}

	if (0 == SettingManager::getInstance()->getExitSetting()) {
		//没有计费点
		CREATE_NODE(m_uiExitLayer,			"UExit2Layer.csb")
	} else if (1 == SettingManager::getInstance()->getExitSetting()) {
		//计费点和退出UI合并
		CREATE_NODE(m_uiExitLayer,			"UExitLayer.csb")
	} else if (2 == SettingManager::getInstance()->getExitSetting()) {
		//计费点和退出UI分开
		CREATE_NODE(m_uiExitLayer,			"UExit2Layer.csb")
		CREATE_NODE(m_uiExitGiftLayer,		"UExitGiftLayer.csb")
	}

	if (SettingManager::getInstance()->isOpen(Setting_toolconfirm)) {
		CREATE_NODE(m_uiToolConfirmLayer,	"UToolConfirmLayer.csb")
	}

	if (SettingManager::getInstance()->isOpen(Setting_giftmore)) {
		CREATE_NODE(m_uiGiftMoreLayer,		"UGiftMoreLayer.csb")
	}

	return true;
}
void GameScene::initUi() {
	m_bggreyLayer	= GreyLayer::create();
	addChild(m_bggreyLayer, LAYER_BG_GREY_Z_ORDER);

	//m_gameLayer		= CSLoader::createNode("GameLayer.csb");
	addChild(m_gameLayer, LAYER_GAME_Z_ORDER);	
	m_gameLayer->release();

	m_introBgLayer	= BgLayer::create();
	addChild(m_introBgLayer, LAYER_DEMO_BG_Z_ORDER);	

	//m_topLayer		= CSLoader::createNode("GameTopLayer.csb");
	addChild(m_topLayer, LAYER_TOP_Z_ORDER);
	m_topLayer->release();

	//m_bottomLayer	= CSLoader::createNode("GameBottomLayer.csb");
	addChild(m_bottomLayer, LAYER_BOTTOM_Z_ORDER);
	m_bottomLayer->release();

	//m_levelLayer	= CSLoader::createNode("GameLevelLayer.csb");
	addChild(m_levelLayer, LAYER_LEVEL_Z_ORDER);
	m_levelLayer->release();

	m_levelDemoLayer	= GameLevelDemoLayer::create();
	addChild(m_levelDemoLayer, LAYER_LEVEL_DEMO_Z_ORDER);

	//m_giftLayer	= CSLoader::createNode("UGiftLayer.csb");
	addChild(m_giftLayer, LAYER_PANEL_Z_ORDER);
	m_giftLayer->release();

	//m_targetLayer	= CSLoader::createNode("GameTargetLayer.csb");
	addChild(m_targetLayer, LAYER_PANEL_Z_ORDER);
	m_targetLayer->release();

	//m_targetBossLayer = CSLoader::createNode("GameBossTargetLayer.csb");
	addChild(m_targetBossLayer, LAYER_PANEL_Z_ORDER);
	m_targetBossLayer->release();

	//m_lotteryLayer	= CSLoader::createNode("GameLotteryLayer.csb");
	addChild(m_lotteryLayer, LAYER_PANEL_Z_ORDER);
	m_lotteryLayer->release();

	//m_successLayer	= CSLoader::createNode("GameSuccessLayer.csb");
	addChild(m_successLayer, LAYER_PANEL_Z_ORDER);
	m_successLayer->release();

	//if (SettingManager::getInstance()->isOpen(Setting_nextgame)) {
	//	m_failLayer	= CSLoader::createNode("GameFailLayer.csb");
	//} else {
	//	m_failLayer	= CSLoader::createNode("GameFail2Layer.csb");
	//}
	addChild(m_failLayer, LAYER_PANEL_Z_ORDER);
	m_failLayer->release();

	//m_buytimeLayer	= CSLoader::createNode("GameBuyTimeLayer.csb");
	addChild(m_buytimeLayer, LAYER_PANEL_Z_ORDER);
	m_buytimeLayer->release();

	//m_energytipLayer	= CSLoader::createNode("GameEnergyTipLayer.csb");
	addChild(m_energytipLayer, LAYER_PANEL_Z_ORDER);
	m_energytipLayer->release();

	//m_pauseLayer	= CSLoader::createNode("GamePauseLayer.csb");
	addChild(m_pauseLayer, LAYER_PANEL_Z_ORDER);
	m_pauseLayer->release();

	//m_timeoutLayer	= CSLoader::createNode("GameTimeoutLayer.csb");
	addChild(m_timeoutLayer, LAYER_PANEL_Z_ORDER);
	m_timeoutLayer->release();

	//m_purchaseLayer	= CSLoader::createNode("GamePurchaseLayer.csb");
	addChild(m_purchaseLayer, LAYER_PANEL_Z_ORDER);
	m_purchaseLayer->release();

	//m_gameBossLayer	= CSLoader::createNode("GameBossLayer.csb");
	addChild(m_gameBossLayer, LAYER_TOP_Z_ORDER);
	m_gameBossLayer->release();

	initJFBg();
	initIntroUi();	
	initGuideUi();
}

void GameScene::initIntroUi() {
	//m_introLayer = CSLoader::createNode("GameIntroLayer.csb");
	addChild(m_introLayer, LAYER_DEMO_Z_ORDER);
	m_introLayer->release();
}

void GameScene::initGuideUi() {
	//auto welcome = CSLoader::createNode("UStepWelcomeLayer.csb");
	addChild(m_welcome, LAYER_PANEL_Z_ORDER);
	GameUiManager::getInstance()->regStepWelcome(m_welcome);
	m_welcome->release();

	//auto welcomeEnd = CSLoader::createNode("UWelcomeEndLayer.csb");
	addChild(m_welcomeEnd, LAYER_PANEL_Z_ORDER);
	GameUiManager::getInstance()->regWelcomeEnd(m_welcomeEnd);
	m_welcomeEnd->release();

	//auto crossGuide = CSLoader::createNode("UStepCrossLayer.csb");
	addChild(m_crossGuide, LAYER_PANEL_Z_ORDER);
	GameUiManager::getInstance()->regCrossGuide(m_crossGuide);
	m_crossGuide->release();

	//auto bombGuide = CSLoader::createNode("UStepBombLayer.csb");
	addChild(m_bombGuide, LAYER_PANEL_Z_ORDER);
	GameUiManager::getInstance()->regBombGuide(m_bombGuide);
	m_bombGuide->release();

	//auto timeGuide = CSLoader::createNode("UStepTimeLayer.csb");
	addChild(m_timeGuide, LAYER_PANEL_Z_ORDER);
	GameUiManager::getInstance()->regTimeGuide(m_timeGuide);
	m_timeGuide->release();

	//auto tipGuide = CSLoader::createNode("UStepTipLayer.csb");
	addChild(m_tipGuide, LAYER_PANEL_Z_ORDER);
	m_tipGuide->release();

	//auto specialPetGuide = CSLoader::createNode("UStepSpecialPetLayer.csb");
	addChild(m_specialPetGuide, LAYER_PANEL_Z_ORDER);
	GameUiManager::getInstance()->regSpecailPet(m_specialPetGuide);
	m_specialPetGuide->release();

	//auto bossTalk = CSLoader::createNode("UStepBossSpeakLayer.csb");
	addChild(m_bossTalk, LAYER_PANEL_Z_ORDER);
	GameUiManager::getInstance()->regBossTalk(m_bossTalk);
	m_bossTalk->release();

	//auto girlTalk = CSLoader::createNode("UStepGirlSpeakLayer.csb");
	addChild(m_girlTalk, LAYER_PANEL_Z_ORDER);
	GameUiManager::getInstance()->regGirlTalk(m_girlTalk);
	m_girlTalk->release();

	//auto bossTip = CSLoader::createNode("UStepBossTipLayer.csb");
	addChild(m_bossTip, LAYER_PANEL_Z_ORDER);
	GameUiManager::getInstance()->regBossTip(m_bossTip);
	m_bossTip->release();

	//auto touchErr = CSLoader::createNode("UStepTouchErrLayer.csb");
	addChild(m_touchErr, LAYER_PANEL_Z_ORDER);
	GameUiManager::getInstance()->regTouchErr(m_touchErr);
	m_touchErr->release();
}

void GameScene::initController() {
	m_greyLayerController = new GreyLayerController();
	m_greyLayerController->regLayer(m_bggreyLayer);
	m_greyLayerController->exit();

	m_gameLayerController = new GameLayerController();
	m_gameLayerController->regLayer(m_gameLayer);
	m_gameLayerController->exit();

	m_introBgLayerController = new BgLayerController();
	m_introBgLayerController->regLayer(m_introBgLayer);
	m_introBgLayerController->exit();

	m_topLayerController = new GameTopLayerController();
	m_topLayerController->regLayer(m_topLayer);
	m_topLayerController->enter();

	m_bottomLayerController = new GameBottomLayerController();
	m_bottomLayerController->regLayer(m_bottomLayer);
	m_bottomLayerController->regGameLayer(m_gameLayerController);
	m_gameLayerController->regBottomLayer(m_bottomLayerController);
	m_bottomLayerController->enter();

	m_giftLayerController = new UGiftLayerController();
	m_giftLayerController->regLayer(m_giftLayer);
	m_giftLayerController->regBgLayer(m_greyLayerController);
	m_giftLayerController->exit();
	Pay::regZhizunLis(m_giftLayerController);

	m_levelLayerController = new GameLevelLayerController();
	m_levelLayerController->regLayer(m_levelLayer);
	m_levelLayerController->exit();

	m_levelDemoLayerController = new GameLevelDemoLayerController();
	m_levelDemoLayerController->regLayer(m_levelDemoLayer);
	m_levelLayerController->regDemoLayer(m_levelDemoLayerController);
	m_levelDemoLayerController->exit();

	m_targetLayerController = new GameTargetLayerController();
	m_targetLayerController->regLayer(m_targetLayer);
	m_targetLayerController->regNextLayer(m_levelLayerController);
	m_targetLayerController->regBgLayer(m_greyLayerController);
	m_targetLayerController->exit();

	m_targetBossLayerController = new GameTargetBossLayerController();
	m_targetBossLayerController->regLayer(m_targetBossLayer);
	m_targetBossLayerController->regBgLayer(m_greyLayerController);
	m_targetBossLayerController->exit();

	m_lotteryLayerController = new GameLotteryLayerController();
	m_lotteryLayerController->regLayer(m_lotteryLayer);
	m_lotteryLayerController->exit();
	m_lotteryLayerController->regBgLayer(m_greyLayerController);
	m_lotteryLayerController->regNextLayer(m_levelLayerController);
	Pay::regAllopenLis(m_lotteryLayerController);
	Pay::regFetchallLis(m_lotteryLayerController);

	m_successLayerController = new GameSuccessLayerController();
	m_successLayerController->regLayer(m_successLayer);
	m_successLayerController->regNextLayer(m_levelLayerController);
	m_successLayerController->regBgLayer(m_greyLayerController);
	m_successLayerController->exit();

	m_failLayerController = new GameFailLayerController();
	m_failLayerController->regLayer(m_failLayer);
	m_failLayerController->regNextLayer(m_levelLayerController);
	m_failLayerController->regBgLayer(m_greyLayerController);
	m_failLayerController->exit();
	Pay::regPassgameLis(m_failLayerController);

	m_buytimeLayerController = new GameBuyTimeLayerController();
	m_buytimeLayerController->regLayer(m_buytimeLayer);
	m_buytimeLayerController->regNextLayer(m_failLayerController);
	m_buytimeLayerController->regBgLayer(m_greyLayerController);
	m_buytimeLayerController->exit();
	Pay::regAddtimeLis(m_buytimeLayerController);

	m_energytipLayerController = new GameEnergyTipLayerController();
	m_energytipLayerController->regLayer(m_energytipLayer);
	m_energytipLayerController->regExitLayer(m_levelLayerController);
	m_energytipLayerController->regBgLayer(m_greyLayerController);
	m_energytipLayerController->exit();
	m_levelLayerController->regEnergyTipLayer(m_energytipLayerController);
	m_successLayerController->regEnergyTipLayer(m_energytipLayerController);
	m_failLayerController->regEnergyTipLayer(m_energytipLayerController);
	Pay::regEnergytipLis(m_energytipLayerController);

	m_pauseLayerController = new GamePauseLayerController();
	m_pauseLayerController->regLayer(m_pauseLayer);
	m_pauseLayerController->regBgLayer(m_greyLayerController);
	m_pauseLayerController->regLevelLayer(m_levelLayerController);
	m_pauseLayerController->exit();
	m_pauseLayerController->regEnergyTipLayer(m_energytipLayerController);

	m_timeoutLayerController = new GameTimeoutLayerController();
	m_timeoutLayerController->regLayer(m_timeoutLayer);
	m_timeoutLayerController->regNextLayer(m_failLayerController);
	m_timeoutLayerController->regEnergytipLayer(m_energytipLayerController);
	m_timeoutLayerController->regPurchaseLayer(m_purchaseLayerController);
	m_timeoutLayerController->regBgLayer(m_greyLayerController);
	m_timeoutLayerController->exit();	

	m_purchaseLayerController = new GamePurchaseLayerController();
	m_purchaseLayerController->regLayer(m_purchaseLayer);
	m_purchaseLayerController->regBgLayer(m_greyLayerController);
	m_purchaseLayerController->exit();
	m_bottomLayerController->regPurchaseLayer(m_purchaseLayerController);
	m_levelLayerController->regPurchaseLayer(m_purchaseLayerController);
	Pay::regBuytimeLis(m_purchaseLayerController);
	Pay::regBuytipLis(m_purchaseLayerController);
	Pay::regBuybombLis(m_purchaseLayerController);
	Pay::regBuycrossLis(m_purchaseLayerController);
	Pay::regBuyenergyLis(m_purchaseLayerController);

	m_gameBossLayerController = new GameBossLayerController();
	m_gameBossLayerController->regLayer(m_gameBossLayer);
	m_gameBossLayerController->exit();

	initJFController();
	initIntroController();	

	GameUiManager::getInstance()->regBossLayerController(m_gameBossLayerController);
	GameUiManager::getInstance()->regTargetLayerController(m_targetLayerController);
	GameUiManager::getInstance()->regTargetBossLayerController(m_targetBossLayerController);
	GameUiManager::getInstance()->regTipWinLayerController(m_uiTipwinLayerController);
	GameUiManager::getInstance()->regToolConfirmLayerController(m_uiToolConfirmLayerController);
	GameUiManager::getInstance()->regTopLayerController(m_topLayerController);
	GameUiManager::getInstance()->regBgGreyLayerController(m_greyLayerController);
	GameUiManager::getInstance()->regGiftMoreLayerController(m_uiGiftMoreLayerController);
	GameUiManager::getInstance()->regTipGuideLayerController(m_uiGuideTipLayerController);
}

void GameScene::initJFBg() {
	//新手礼包
	//m_uiRookieLayer	= CSLoader::createNode("URookieLayer.csb");
	addChild(m_uiRookieLayer, LAYER_PANEL_Z_ORDER);
	m_uiRookieLayer->release();

	//幸运大转盘
	//m_uiLuckyLayer	= CSLoader::createNode("ULuckyLayer.csb");
	addChild(m_uiLuckyLayer, LAYER_PANEL_Z_ORDER);
	m_uiLuckyLayer->release();

	//领取奖品
	//m_uiAwardLayer	= CSLoader::createNode("UAwardLayer.csb");
	addChild(m_uiAwardLayer, LAYER_PANEL_Z_ORDER);
	m_uiAwardLayer->release();

	//限时优惠
	if (SettingManager::getInstance()->isOpen(Setting_discount)) {
		//m_uiDiscountLayer	= CSLoader::createNode("UDiscountLayer.csb");
		addChild(m_uiDiscountLayer, LAYER_PANEL_Z_ORDER);
		m_uiDiscountLayer->release();
	}

	//惊喜宝箱
	//m_uiSupriseLayer	= CSLoader::createNode("USupriseLayer.csb");
	addChild(m_uiSupriseLayer, LAYER_PANEL_Z_ORDER);
	m_uiSupriseLayer->release();

	//中大奖
	//m_uiBigAwardLayer = CSLoader::createNode("UBigAwardLayer.csb");
	addChild(m_uiBigAwardLayer, LAYER_PANEL_Z_ORDER);
	m_uiBigAwardLayer->release();

	//退出界面
	if (0 == SettingManager::getInstance()->getExitSetting()) {
		//没有计费点
		//m_uiExitLayer = CSLoader::createNode("UExit2Layer.csb");
	} else if (1 == SettingManager::getInstance()->getExitSetting()) {
		//计费点和退出UI合并
		//m_uiExitLayer = CSLoader::createNode("UExitLayer.csb");
	} else if (2 == SettingManager::getInstance()->getExitSetting()) {
		//计费点和退出UI分开
		//m_uiExitLayer = CSLoader::createNode("UExit2Layer.csb");
		//m_uiExitGiftLayer = CSLoader::createNode("UExitGiftLayer.csb");
		addChild(m_uiExitGiftLayer, LAYER_EXIT_Z_ORDER);
		m_uiExitGiftLayer->release();
	}

	addChild(m_uiExitLayer, LAYER_EXIT_Z_ORDER);
	m_uiExitLayer->release();

	//转盘大礼包界面
	//m_uiLuckyGiftLayer = CSLoader::createNode("ULuckyGiftLayer.csb");
	addChild(m_uiLuckyGiftLayer, LAYER_PANEL_Z_ORDER);
	m_uiLuckyGiftLayer->release();

	//解锁界面
	//m_uiUnlockLayer = CSLoader::createNode("UUnlockLayer.csb");
	addChild(m_uiUnlockLayer, LAYER_PANEL_Z_ORDER);
	m_uiUnlockLayer->release();

	//新手引导奖品
	//m_uiIntroAwardLayer = CSLoader::createNode("UIntroAwardLayer.csb");
	addChild(m_uiIntroAwardLayer, LAYER_PANEL_Z_ORDER);
	m_uiIntroAwardLayer->release();

	//恭喜胜利
	//m_uiTipwinLayer = CSLoader::createNode("UTipWinLayer.csb");
	addChild(m_uiTipwinLayer, LAYER_PANEL_Z_ORDER);
	m_uiTipwinLayer->release();

	//进关前的道具确认
	if (SettingManager::getInstance()->isOpen(Setting_toolconfirm)) {
		//m_uiToolConfirmLayer = CSLoader::createNode("UToolConfirmLayer.csb");
		addChild(m_uiToolConfirmLayer, LAYER_PANEL_Z_ORDER);
		m_uiToolConfirmLayer->release();
	}

	//结算前的好礼多多
	if (SettingManager::getInstance()->isOpen(Setting_giftmore)) {
		//m_uiGiftMoreLayer = CSLoader::createNode("UGiftMoreLayer.csb");
		addChild(m_uiGiftMoreLayer, LAYER_PANEL_Z_ORDER);
		m_uiGiftMoreLayer->release();
	}
}

void GameScene::initJFController() {
	m_uiRookieLayerController = new URookieLayerController();
	m_uiRookieLayerController->regLayer(m_uiRookieLayer);
	m_uiRookieLayerController->regBgLayer(m_greyLayerController);
	m_levelLayerController->regHappyLayer(m_uiRookieLayerController);
	m_pauseLayerController->regGiftLayer(m_uiRookieLayerController);
	m_uiRookieLayerController->exit();
	Pay::regRookieLis(m_uiRookieLayerController);
	Pay::regHappyLis(m_uiRookieLayerController);

	m_uiLuckyLayerController = new ULuckyLayerController();
	m_uiLuckyLayerController->regLayer(m_uiLuckyLayer);
	m_uiLuckyLayerController->regBgLayer(m_greyLayerController);
	m_levelLayerController->regLuckyLayer(m_uiLuckyLayerController);
	m_uiLuckyLayerController->exit();

	m_uiAwardLayerController = new UAwardLayerController();
	m_uiAwardLayerController->regLayer(m_uiAwardLayer);
	m_uiAwardLayerController->regBgLayer(m_greyLayerController);
	m_uiLuckyLayerController->regAwardLayer(m_uiAwardLayerController);
	m_uiAwardLayerController->exit();

	if (SettingManager::getInstance()->isOpen(Setting_discount)) {
		m_uiDiscountLayerController = new UDiscountLayerController();
		m_uiDiscountLayerController->regLayer(m_uiDiscountLayer);
		m_uiDiscountLayerController->regBgLayer(m_greyLayerController);
		m_purchaseLayerController->regNextLayer(m_uiDiscountLayerController);
		m_uiDiscountLayerController->exit();
		Pay::regDiscountLis(m_uiDiscountLayerController);
	} 

	m_uiSupriseLayerController = new USupriseLayerController();
	m_uiSupriseLayerController->regLayer(m_uiSupriseLayer);
	m_uiSupriseLayerController->regBgLayer(m_greyLayerController);
	m_gameLayerController->regSupriseLayer(m_uiSupriseLayerController);
	m_uiSupriseLayerController->exit();
	Pay::regSupriseLis(m_uiSupriseLayerController);

	m_uiBigAwardLayerController = new UBigAwardLayerController();
	m_uiBigAwardLayerController->regLayer(m_uiBigAwardLayer);
	m_uiBigAwardLayerController->regBgLayer(m_greyLayerController);
	m_lotteryLayerController->regBgiAwardLayer(m_uiBigAwardLayerController);
	m_uiBigAwardLayerController->regNextLayer(m_successLayerController);
	m_uiBigAwardLayerController->exit();
	Pay::regZunxiangLis(m_uiBigAwardLayerController);

	m_uiLuckyGiftLayerController = new ULuckyGiftLayerController();
	m_uiLuckyGiftLayerController->regLayer(m_uiLuckyGiftLayer);
	m_uiLuckyGiftLayerController->regBgLayer(m_greyLayerController);
	m_uiLuckyLayerController->regGiftLayer(m_uiLuckyGiftLayerController);
	m_uiLuckyGiftLayerController->exit();
	Pay::regLuckyLis(m_uiLuckyGiftLayerController);

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

	m_uiUnlockLayerController = new UUnlockLayerController();
	m_uiUnlockLayerController->regLayer(m_uiUnlockLayer);
	m_uiUnlockLayerController->regBgLayer(m_greyLayerController);
	m_uiUnlockLayerController->regNextLayer(m_levelLayerController);
	m_levelLayerController->regUnlockLayer(m_uiUnlockLayerController);
	m_successLayerController->regUnlockLayer(m_uiUnlockLayerController);
	m_failLayerController->regUnlockLayer(m_uiUnlockLayerController);
	m_uiUnlockLayerController->exit();
	Pay::regUnlockLis(m_uiUnlockLayerController);

	m_uiIntroAwardLayerController = new UIntroAwardLayerController();
	m_uiIntroAwardLayerController->regLayer(m_uiIntroAwardLayer);
	m_uiIntroAwardLayerController->regBgLayer(m_greyLayerController);
	m_uiIntroAwardLayerController->regLevelLayer(m_levelLayerController);
	m_uiIntroAwardLayerController->exit();

	m_uiTipwinLayerController = new UTipWinLayerController();
	m_uiTipwinLayerController->regLayer(m_uiTipwinLayer);
	m_uiTipwinLayerController->exit();
	m_gameBossLayerController->regTipWinLayer(m_uiTipwinLayerController);

	//道具确认
	if (SettingManager::getInstance()->isOpen(Setting_toolconfirm)) {
		m_uiToolConfirmLayerController = new UToolConfirmLayerController();
		m_uiToolConfirmLayerController->regLayer(m_uiToolConfirmLayer);
		m_uiToolConfirmLayerController->regBgLayer(m_greyLayerController);
		m_uiToolConfirmLayerController->exit();
		Pay::regToolconfirmLis(m_uiToolConfirmLayerController);
	} 

	//好礼多多
	if (SettingManager::getInstance()->isOpen(Setting_giftmore)) {
		m_uiGiftMoreLayerController = new UGiftMoreLayerController();
		m_uiGiftMoreLayerController->regLayer(m_uiGiftMoreLayer);
		m_uiGiftMoreLayerController->exit();
		Pay::regGiftMoreLis(m_uiGiftMoreLayerController);
	} 

	m_uiGuideTipLayerController = new UGuideTipLayerController();
	m_uiGuideTipLayerController->regLayer(m_tipGuide);
	m_uiGuideTipLayerController->exit();
	Pay::regGuideTipLis(m_uiGuideTipLayerController);
}

void GameScene::initIntroController() {
	m_introLayerController = new GameIntroLayerController();
	m_introLayerController->regLayer(m_introLayer);
	m_introLayerController->regNextLayer(m_levelLayerController);
	m_pauseLayerController->regIntroLayer(m_introLayerController);
	m_introLayerController->regBgLayer(m_greyLayerController);
	m_introLayerController->exit();
}

void GameScene::onEnter() {
	CCLayer::onEnter();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//	plugin_authenticateLocalPlayer();
#endif

	regGameEvent();
	regSysEvent();

	if (NULL != m_levelLayerController) {
		m_levelLayerController->enter();
	}

	//第一次进游戏不再弹出玩法说明
	if (NULL != m_introLayerController && !UserData::getIntroPass()) {
		m_introLayerController->enter();
	}

	LuckyTimeManager::getInstance()->init();
	schedule(schedule_selector(GameScene::on1sTimer), 1.0f);
}

void GameScene::onExit() {
	CCLayer::onExit();
	unregGameEvent();
	unregSysEvent();
	unschedule(schedule_selector(GameScene::on1sTimer));
}

void GameScene::regGameEvent() {
	//注册事件
	m_GameStartLis				= GameEvent::regEvent(Event_Game_Start,			CC_CALLBACK_1(GameScene::onGameStart, this));
	m_GameRestartLis			= GameEvent::regEvent(Event_Game_Restart,		CC_CALLBACK_1(GameScene::onGameRestart, this));
	m_GameOverLis				= GameEvent::regEvent(Event_Game_Over,			CC_CALLBACK_1(GameScene::onGameOver, this));
	m_GameResumeLis				= GameEvent::regEvent(Event_Game_Resume,		CC_CALLBACK_1(GameScene::onGameResume, this));
	m_GamePauseLis				= GameEvent::regEvent(Event_Game_Pause,			CC_CALLBACK_1(GameScene::onGamePause, this));
	m_GameReadyLis				= GameEvent::regEvent(Event_Game_Ready,			CC_CALLBACK_1(GameScene::onGameReady, this));
	m_GameReliveLis				= GameEvent::regEvent(Event_Game_Relive,		CC_CALLBACK_1(GameScene::onGameRelive, this));
	m_ExitCancelLis				= GameEvent::regEvent(Event_Exit_Cancel,		CC_CALLBACK_1(GameScene::onExitCancel, this));
	m_GameScoreChangeLis		= GameEvent::regEvent(Event_Score_Change,		CC_CALLBACK_1(GameScene::onGameScoreChanged, this));
	m_GameToolChangeLis			= GameEvent::regEvent(Event_Tool_Change,		CC_CALLBACK_1(GameScene::onGameToolChanged, this));
	m_ReleaseCrossLis			= GameEvent::regEvent(Event_Release_Cross,		CC_CALLBACK_1(GameScene::onReleaseCross, this));
	m_EnrgyChangeLis			= GameEvent::regEvent(Event_Energy_Change,		CC_CALLBACK_1(GameScene::onEnergyChange, this));
	m_CloseIntroLis				= GameEvent::regEvent(Event_Close_Intro,		CC_CALLBACK_1(GameScene::onCloseIntro, this));
	m_LevelUnlockLis			= GameEvent::regEvent(Event_Level_Unlock,		CC_CALLBACK_1(GameScene::onLevelUnlock, this));
	m_TimeChangeLis				= GameEvent::regEvent(Event_Time_Change,		CC_CALLBACK_1(GameScene::onTimeChange, this));
	m_GameWinLis				= GameEvent::regEvent(Event_Game_Win,			CC_CALLBACK_1(GameScene::onGameWin, this));
	m_AttrackBossLis			= GameEvent::regEvent(Event_Attrack_Boss,		CC_CALLBACK_1(GameScene::onAttrackBoss, this));
	m_GameFinishLis				= GameEvent::regEvent(Event_Game_Finish,		CC_CALLBACK_1(GameScene::onGameFinish, this));
}

void GameScene::unregGameEvent() {
	GameEvent::unregEvent(m_GameStartLis);
	GameEvent::unregEvent(m_GameRestartLis);
	GameEvent::unregEvent(m_GameOverLis);
	GameEvent::unregEvent(m_GameResumeLis);
	GameEvent::unregEvent(m_GamePauseLis);
	GameEvent::unregEvent(m_GameReadyLis);
	GameEvent::unregEvent(m_GameReliveLis);
	GameEvent::unregEvent(m_ExitCancelLis);
	GameEvent::unregEvent(m_GameScoreChangeLis);
	GameEvent::unregEvent(m_GameToolChangeLis);
	GameEvent::unregEvent(m_ReleaseCrossLis);
	GameEvent::unregEvent(m_EnrgyChangeLis);
	GameEvent::unregEvent(m_CloseIntroLis);
	GameEvent::unregEvent(m_LevelUnlockLis);
	GameEvent::unregEvent(m_TimeChangeLis);
	GameEvent::unregEvent(m_GameWinLis);
	GameEvent::unregEvent(m_AttrackBossLis);
	GameEvent::unregEvent(m_GameFinishLis);
}

void GameScene::regSysEvent() {
	m_touchEventListener = EventListenerTouchOneByOne::create();
	m_touchEventListener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
	m_touchEventListener->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, this);
	m_touchEventListener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithFixedPriority(m_touchEventListener, 12);

	m_keyboradEventListener = EventListenerKeyboard::create();
	m_keyboradEventListener->onKeyReleased =[&](EventKeyboard::KeyCode keyCode, Event* event)
	{
		if (nullptr == m_levelLayerController || !m_levelLayerController->isEnter()) {
			if (GameManager::getInstance()->isStarted() && !GameManager::getInstance()->isPaused())  {
				GameManager::getInstance()->pause();
			} 
			return;
		}

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

void GameScene::unregSysEvent() {
	_eventDispatcher->removeEventListener(m_touchEventListener);
	_eventDispatcher->removeEventListener(m_keyboradEventListener);
}

bool GameScene::onTouchBegan(Touch* touch, Event  *event) {
	if (nullptr != m_gameLayerController && nullptr != touch) {
		m_gameLayerController->onTouch(touch->getLocation());
	}

	return true;
}

void GameScene::onTouchMoved(Touch* touch, Event  *event) {

} 

void GameScene::onTouchEnded(Touch* touch, Event  *event) {

}

void GameScene::onTouchCancelled(Touch* touch, Event  *event) {

}

void GameScene::onGameReady(cocos2d::EventCustom* event) {

	if (nullptr != m_gameLayerController) {
		m_gameLayerController->onGameReady();
	}

	if (GameData::getInstance()->getBossType() > 0) {
		if (nullptr != m_gameBossLayerController) {
			m_gameBossLayerController->enter();
		}

		if (nullptr != m_topLayerController) {
			m_topLayerController->exit();
		}
	} else {
		if (nullptr != m_gameBossLayerController) {
			m_gameBossLayerController->exit();
		}

		if (nullptr != m_topLayerController) {
			m_topLayerController->enter();
			m_topLayerController->onGameReady();
		}
	}

	Audio::getInstance()->playGameBGM();
}

void GameScene::onGameStart(cocos2d::EventCustom* event) {
	if (nullptr != m_gameLayerController) {
		m_gameLayerController->onGameStart();
	}

	if (nullptr != m_topLayerController) {
		m_topLayerController->onGameStart();
	}

	if (nullptr != m_bottomLayerController) {
		m_bottomLayerController->onGameStart();
	}

	if (nullptr != m_gameBossLayerController) {
		m_gameBossLayerController->onGameStart();
	}

	UMengAnalytics::onBegin(GameData::getInstance()->getLevel() + 1);
}

void GameScene::onGameRestart(cocos2d::EventCustom* event) {
	m_gameLayerController->onGameRestart();

	if (nullptr != m_topLayerController) {
		m_topLayerController->onGameRestart();
	}

	if (nullptr != m_bottomLayerController) {
		m_bottomLayerController->onGameRestart();
	}
}

void GameScene::onGamePause(cocos2d::EventCustom* event) {
	m_gameLayerController->onGamePause();

	if (nullptr != m_uiExitLayerController && m_uiExitLayerController->isEnter()) {
		return;
	}

	if (nullptr != m_uiSupriseLayerController && m_uiSupriseLayerController->isEnter()) {
		return;
	}

	if (nullptr != m_purchaseLayerController && m_purchaseLayerController->isEnter()) {
		return;
	}

	if (NULL != m_pauseLayerController) {
		m_pauseLayerController->enter();
	}
}

void GameScene::onGameResume(cocos2d::EventCustom* event) {
	if (nullptr != m_gameLayerController) {
		m_gameLayerController->onGameResume();
	}	
}

void GameScene::onGameRelive(cocos2d::EventCustom*) {
	if (nullptr != m_bottomLayerController) {
		m_bottomLayerController->onGameRelive();
	}
}

void GameScene::onGameOver(cocos2d::EventCustom* event) {
	bool success = (GameData::getInstance()->getScore() >= GameData::getInstance()->getTargetScore());

	if (success) {
		if (nullptr != m_purchaseLayerController) {
			m_purchaseLayerController->exit();
		}

		if (nullptr != m_uiSupriseLayerController && m_uiSupriseLayerController->isEnter()) {
			m_uiSupriseLayerController->exit();
		}

		if (nullptr != m_gameBossLayerController && m_gameBossLayerController->isEnter()) {
			m_gameBossLayerController->onGameOver();
		} 
		
		UMengAnalytics::onComplete(GameData::getInstance()->getLevel() + 1, GameData::getInstance()->getTime(), GameData::getInstance()->getTimelimit());
	} else {
		Audio::getInstance()->stopGameBGM();	
		//还有时间道具
		if (GameData::getInstance()->getToolTimeNum()) {
			if (nullptr != m_timeoutLayerController) {
				m_timeoutLayerController->enter();
			}
		} else {
			if (nullptr != m_buytimeLayerController) {
				m_buytimeLayerController->enter();
			}
		}

		UMengAnalytics::onFailed(GameData::getInstance()->getLevel() + 1, GameData::getInstance()->getScore(), GameData::getInstance()->getTargetScore());

		if (GameData::getInstance()->getStar(GameData::getInstance()->getLevel()) <= 0) {
			UMengAnalytics::onFailedBeforeSuccess(GameData::getInstance()->getLevel() + 1, GameData::getInstance()->getScore(), GameData::getInstance()->getTargetScore());
		}
	}

	if (NULL != m_gameLayerController) {
		m_gameLayerController->onGameOver();
	}

	if (nullptr != m_bottomLayerController) {
		m_bottomLayerController->onGameOver();
	}
}

void GameScene::onGameScoreChanged(cocos2d::EventCustom* event) {
	if (nullptr != m_topLayerController) {
		m_topLayerController->onScoreChange();
	}

	if (nullptr != m_gameBossLayerController) {
		m_gameBossLayerController->onScoreChange();
	}
}

void GameScene::onGameToolChanged(cocos2d::EventCustom*) {
	if (nullptr != m_bottomLayerController) {
		m_bottomLayerController->onGameToolChanged();
	}
}

void GameScene::onEnergyChange(cocos2d::EventCustom*) {
	if (nullptr != m_levelLayerController) {
		m_levelLayerController->onEnergyChange();
	}
}

void GameScene::onExitCancel(cocos2d::EventCustom* event) {
	if (m_backTouched) {
		m_backTouched = false;
	}
}

void GameScene::onCloseIntro(cocos2d::EventCustom*) {
	if (nullptr != m_introLayerController) {
		m_introLayerController->exit();
	}
}

void GameScene::onLevelUnlock(cocos2d::EventCustom*) {
	if (nullptr != m_levelLayerController) {
		m_levelLayerController->onLevelUnlock();
	}
}

void GameScene::onTimeChange(cocos2d::EventCustom*) {
	if (nullptr != m_topLayerController) {
		m_topLayerController->onTimeChange();
	}

	if (nullptr != m_gameBossLayerController) {
		m_gameBossLayerController->onTimeChange();
	}
}

long GameScene::getCurrentTime()
{
    struct timeval tv;
    gettimeofday(&tv,NULL);
    return tv.tv_sec;
}

void GameScene::on1sTimer(float dt) {
	if (NULL != m_topLayerController) {
		m_topLayerController->on1sTimer();
	}

	if (NULL != m_gameBossLayerController) {
		m_gameBossLayerController->on1sTimer();
	}

	EnergyManager::getInstance()->on1sTimer();
	LuckyTimeManager::getInstance()->on1sTimer();

	if (nullptr != m_levelLayerController) {
		m_levelLayerController->on1sTimer();
	}

	if (nullptr != m_gameLayerController) {
		m_gameLayerController->on1sTimer();
	}
}

void GameScene::onReleaseCross(cocos2d::EventCustom*) {
	if (nullptr != m_bottomLayerController) {
		m_bottomLayerController->releaseCross();
	}
}

void GameScene::onGameWin(cocos2d::EventCustom*) {
	//强制引导 不弹中奖界面
	if(GameData::getInstance()->getLevel() < 7 && !UserData::getGuidePass(GameData::getInstance()->getLevel())) {
		if (nullptr != m_successLayerController) {
			Audio::getInstance()->stopGameBGM();
			m_successLayerController->enter();
		}

		//已经引导
		UserData::setGuidePass(GameData::getInstance()->getLevel(), true);
	} else {
		//1星获得至尊礼包
		if (nullptr != m_giftLayerController && 1== GameData::getInstance()->getStar()) {
			m_giftLayerController->regNextLayer(m_successLayerController);
			m_giftLayerController->enter();
		} else if (NULL != m_lotteryLayerController
			&& !GameData::getInstance()->getLotteryFlag()
			&& GameData::getInstance()->getStar() >= 2) {//首次 2颗星，可以抽奖
				m_lotteryLayerController->regNextLayer(m_successLayerController);
				m_lotteryLayerController->enter();
		} else if (nullptr != m_successLayerController) {
			Audio::getInstance()->stopGameBGM();
			m_successLayerController->enter();
		}
	}	
}

void GameScene::onAttrackBoss(cocos2d::EventCustom*) {
	if (nullptr != m_gameBossLayerController) {
		m_gameBossLayerController->onAttrackBoss();
	}
}

void GameScene::onGameFinish(cocos2d::EventCustom*) {
	if (nullptr != m_gameLayerController) {
		m_gameLayerController->onGameFinish();
	}
}
