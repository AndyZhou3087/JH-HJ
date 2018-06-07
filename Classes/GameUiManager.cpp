#include "GameUiManager.h"

GameUiManager* GameUiManager::m_instance = nullptr;
GameUiManager* GameUiManager::getInstance(){
	if(m_instance == nullptr){
		m_instance = new GameUiManager();
	}
	return m_instance;
}

GameUiManager::GameUiManager():
m_TargetLayerController(NULL),
m_TargetBossLayerController(NULL),
m_BossLayerController(NULL),
m_TopLayerController(NULL),

m_TipWinLayerController(NULL),
m_ToolConfirmLayerController(NULL),
m_BgGreyLayerController(NULL),
m_GiftMoreLayerController(NULL),
m_TipGuideLayerController(NULL),

m_StepWelcomeLayer(NULL),
m_WelcomeEndLayer(NULL),
m_CrossGuideLayer(NULL),
m_BombGuideLayer(NULL),
m_TimeGuideLayer(NULL),
m_ToolConfirmLayer(NULL),
m_SpecailPetLayer(NULL),
m_GirlTalkLayer(NULL),
m_BossTalkLayer(NULL),
m_BossTipLayer(NULL),
m_TouchErrLayer(NULL)
{

}

GameUiManager::~GameUiManager() {

}

void GameUiManager::init() {

}

void GameUiManager::unregAllUiController() {
	m_TargetLayerController = NULL;
	m_BossLayerController	= NULL;
	m_TopLayerController	= NULL;
	m_BgGreyLayerController	= NULL;
	m_TargetBossLayerController = NULL;

	m_TipWinLayerController			= NULL;
	m_ToolConfirmLayerController	= NULL;
	m_GiftMoreLayerController		= NULL;
	m_TipGuideLayerController		= NULL;

	m_StepWelcomeLayer	= NULL;
	m_WelcomeEndLayer	= NULL;
	m_CrossGuideLayer	= NULL;
	m_BombGuideLayer	= NULL;
	m_TimeGuideLayer	= NULL;
	m_ToolConfirmLayer	= NULL;
	m_SpecailPetLayer	= NULL;
	m_GirlTalkLayer		= NULL;
	m_BossTalkLayer		= NULL;
	m_BossTipLayer		= NULL;
	m_TouchErrLayer		= NULL;
}

void GameUiManager::showLayer(Node * layer, bool visible, EnumShow show) {
	if (nullptr == layer) {
		return;
	}

	if (visible) {
		switch(show) {
		case Show_Bottom:
			showLayerBottom(layer);
			break;
		case Show_Center:
		default:
			showLayerCenter(layer);
			break;
		}
	} else {
		layer->setPositionX(-Director::getInstance()->getVisibleSize().width * 2);
	}
}

void GameUiManager::showLayerBottom(Node *layer) {
	if (nullptr == layer) {
		return;
	}

	const Size size = Director::getInstance()->getVisibleSize();
	layer->ignoreAnchorPointForPosition(false);
	layer->setAnchorPoint(Vec2(0.5,0));
	layer->setPosition(Vec2(size.width * 0.5, 0));
}

void GameUiManager::showLayerCenter(Node *layer) {
	if (nullptr == layer) {
		return;
	}

	const Size size = Director::getInstance()->getVisibleSize();
	layer->ignoreAnchorPointForPosition(false);
	layer->setAnchorPoint(Vec2(0.5,0.5));
	layer->setPosition(Vec2(size.width * 0.5, size.height * 0.5));
}