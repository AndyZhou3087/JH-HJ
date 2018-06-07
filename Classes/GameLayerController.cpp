#include "GameLayerController.h"
#include "Analytics.h"
#include "PetMatrix.h"
#include "prefix.h"
#include "GameData.h"
#include "GameManager.h"
#include "USupriseLayerController.h"
#include "GameData.h"
#include "GameUtil.h"
#include "UserData.h"
#include "GameBottomLayerController.h"
#include "SettingManager.h"

#include "LevelRuleGuideController.h"
#include "LevelCrossGuideController.h"
#include "LevelBombGuideController.h"
#include "LevelTimeTipGuideController.h"
#include "LevelBomPetGuideController.h"
#include "LevelColorPetGuideController.h"
#include "LevelBossGuideController.h"
#include "LevelManualController.h"

USING_NS_CC;

#define MAX_TIP_SEC 5
#define MAX_TIP_SEC_FREE 4
#define LAYER_PANEL_Z_ORDER	1000


GameLayerController::GameLayerController():
m_supriseLayer(NULL),
m_bShowSuprise(false),
m_bClearAlling(false),
m_timeCount(0),
m_bottomLayer(NULL),
m_tipTimeCount(0),
m_LevelController(NULL)
{

}

GameLayerController::~GameLayerController() {
}

void GameLayerController::initial() {
	if (nullptr == m_layer) {
		return;
	}
}

void GameLayerController::onGameReady() {
	if (nullptr != m_LevelController) {
		m_LevelController->exit();
		m_LevelController->release();
		m_LevelController = nullptr;
	}

	if (!isEnter()) {
		enter();
	}

	const int level			= GameData::getInstance()->getLevel();
	const bool bGuideNoPass	= !(UserData::getGuidePass(level));

	if (0 == level && bGuideNoPass) {
		m_LevelController = LevelRuleGuideController::create(m_layer);
	} else if (1 == level && bGuideNoPass) {
		m_LevelController = LevelCrossGuideController::create(m_layer);		
	} else if (2 == level && bGuideNoPass) {
		m_LevelController = LevelBombGuideController::create(m_layer);	
	} else if (3 == level && bGuideNoPass) {
		m_LevelController = LevelTimeTipGuideController::create(m_layer);	
	} else if (4 == level && bGuideNoPass) {
		m_LevelController = LevelBomPetGuideController::create(m_layer);	
	} else if (5 == level && bGuideNoPass) {
		m_LevelController = LevelColorPetGuideController::create(m_layer);	
	} else if (6 == level && bGuideNoPass) {
		m_LevelController = LevelBossGuideController::create(m_layer);	
	} else {
		m_LevelController = LevelManualController::create(m_layer);
	}

	if (nullptr != m_LevelController) {
		m_LevelController->retain();
		m_LevelController->onGameReady();
		m_LevelController->enter();
	}
	
	m_bShowSuprise = false;

}

void GameLayerController::onGameStart() {
	if (nullptr != m_LevelController) {
		m_LevelController->onGameStart();
	}

	m_bShowSuprise = false;
	m_timeCount = 0;
	m_tipTimeCount = 0;
}

void GameLayerController::onGameOver() {
	if (nullptr != m_LevelController) {
		m_LevelController->onGameOver();
	}

	m_timeCount = 0;
	m_tipTimeCount = 0;
}

void GameLayerController::onGameFinish() {
	if (nullptr != m_LevelController) {
		m_LevelController->onGameFinish();
	}

	m_timeCount = 0;
	m_tipTimeCount = 0;
}

void GameLayerController::enter() {
	if (NULL == m_layer) {
		return;
	}

	static const Size size = Director::getInstance()->getVisibleSize();
	m_layer->ignoreAnchorPointForPosition(false);
	m_layer->setAnchorPoint(Vec2(0.5,0));
	m_layer->setPosition(Vec2(size.width * 0.5, 0));
	m_bEnter = true;
}

void GameLayerController::exit() {
	if (NULL == m_layer) {
		return;
	}

	const Size size = Director::getInstance()->getVisibleSize();
	m_layer->ignoreAnchorPointForPosition(false);
	m_layer->setAnchorPoint(Vec2(0.5, 0));
	m_layer->setPositionX(size.width * 2);
	m_bEnter = false;
}

void GameLayerController::on1sTimer() {
	if (!GameManager::getInstance()->isStarted() || GameManager::getInstance()->isPaused()) {
		return;
	}

	if (GameData::getInstance()->getTime() <= s_time_count_down_tip) {
		showSuprise();
		if (nullptr != m_LevelController) {
			m_LevelController->blinkRedTip();
		}
	}

	bool existTip = false;

	if (GameData::getInstance()->getLevel() < 3 && GameData::getInstance()->getLevel() > 0) {
		if (++m_tipTimeCount > MAX_TIP_SEC_FREE) {
			m_tipTimeCount = 0;
			if (nullptr != m_LevelController) {
				m_LevelController->tipPets();
				existTip = true;
			}
		}
	} 

	if (++m_timeCount > MAX_TIP_SEC) {
		m_timeCount -= 2;
		if (nullptr != m_bottomLayer && !existTip) {
			m_bottomLayer->tipTip();
		}
	}
	
}

void GameLayerController::onTouch(const Point&pos) {
	if (!GameManager::getInstance()->isStarted() || GameManager::getInstance()->isPaused()) {
		return;
	}

	m_timeCount = 0;
	m_tipTimeCount = 0;
	if (nullptr != m_LevelController) {
		m_LevelController->onTouch(pos);
		return;
	}
}

bool GameLayerController::usingBomb() {
	if (!GameManager::getInstance()->isStarted() || GameManager::getInstance()->isPaused()) {
		return false;
	}

	bool toolUsed = false;
	if (nullptr != m_LevelController) {
		toolUsed = m_LevelController->usingBomb();
	}

	m_timeCount = 0;
	m_tipTimeCount = 0;
	return toolUsed;
}

bool GameLayerController::usingTip() {
	if (!GameManager::getInstance()->isStarted() || GameManager::getInstance()->isPaused()) {
		return false;
	}

	bool toolUsed = false;
	if (nullptr != m_LevelController) {
		toolUsed = m_LevelController->usingTip();
	}

	m_timeCount = 0;
	m_tipTimeCount = 0;
	return toolUsed;
}

bool GameLayerController::usingCross() {
	if (nullptr != m_LevelController) {
		m_LevelController->usingCross();
	}
	
	m_timeCount = 0;
	m_tipTimeCount = 0;
	return false;
}

bool GameLayerController::usingTime() {
	if (nullptr != m_LevelController) {
		m_LevelController->usingTime();
	}

	m_timeCount = 0;
	m_tipTimeCount = 0;
	return false;
}

void GameLayerController::showSuprise() {
	if (!SettingManager::getInstance()->isOpen(Setting_suprise)) {
		return;
	}

	if (GameData::getInstance()->getLevel() < 2 || m_bShowSuprise) {
		return;
	}

	if (GameManager::getInstance()->isOver() || GameManager::getInstance()->isPaused()) {
		return;
	}

	if (nullptr != m_supriseLayer) {
		m_supriseLayer->enter();
		GameManager::getInstance()->pause();
		m_bShowSuprise = true;
	}
}
