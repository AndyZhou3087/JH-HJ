#include "GameManager.h"
#include "GameData.h"
#include "GameEvent.h"
#include "Audio.h"
#include "GameUtil.h"
#include "UserData.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "PluginHelper.h"
#endif

USING_NS_CC;

GameManager * gManager = NULL;

GameManager::GameManager():
	m_bGameStarted(false),
	m_bGamePaused(false),
	m_bGameOver(false),
	m_bSelectCross(false)
{
}

GameManager::~GameManager() {
}

GameManager * GameManager::getInstance() {
	if (NULL == gManager) {
		gManager = new GameManager();
		gManager->init();
	}

	return gManager;
}

bool GameManager::init() {
	return true;
}

void GameManager::ready(int level) {
	GameData::getInstance()->init(level);
	GameData::getInstance()->setLevel(level);
	GameEvent::throwEvent(Event_Game_Ready, NULL);
}

void GameManager::start() {
	if (isStarted()) {
		return;
	}

	m_bGameStarted		= true;
	m_bGamePaused		= false;
	m_bGameOver			= false;

	m_bSelectCross	= false;

	if (GameData::getInstance()->getLevel() > 6
		|| UserData::getGuidePass(GameData::getInstance()->getLevel())) {
		GameData::getInstance()->addEnergy(-s_energy_use);
	}
	
	GameEvent::throwEvent(Event_Game_Start, NULL);
}

void GameManager::restart() {
	if (isOver() || isPaused()) {

		m_bGameStarted		= false;
		m_bGamePaused		= false;
		m_bGameOver			= false;

		m_bSelectCross	= false;
		
		ready(GameData::getInstance()->getLevel());
	}
}

void GameManager::next() {
	if (isOver() || isPaused()) {

		m_bGameStarted		= false;
		m_bGamePaused		= false;
		m_bGameOver			= false;

		m_bSelectCross	= false;

		const int nextLevel = GameData::getInstance()->getLevel() + 1;
		if (GameData::getInstance()->isLevelUnlock(nextLevel) && nextLevel >= GameData::MAX_LEVEL) {
			//未解锁或关卡到头了
		} else {
			ready(GameData::getInstance()->getLevel() + 1);
		}	
	}
}

void GameManager::pause() {
	if (m_bGamePaused) {
		return;
	}

	m_bGamePaused = true;
	GameEvent::throwEvent(Event_Game_Pause, NULL);
}

void GameManager::resume() {
	if (!m_bGamePaused) {
		return;
	}

	m_bGamePaused = false;
	GameEvent::throwEvent(Event_Game_Resume, NULL);
}

void GameManager::relive() {
	m_bGameStarted		= true;
	m_bGamePaused		= false;
	m_bGameOver			= false;

	Audio::getInstance()->playGameBGM();
	GameEvent::throwEvent(Event_Game_Relive, NULL);
}

void GameManager::over() {
	if (!isStarted()) {
		return;
	}

	m_bGameStarted		= false;
	m_bGamePaused		= false;
	m_bGameOver			= true;
 
	GameData::getInstance()->setStar();

	if (GameData::getInstance()->getStar() >= 3) {
		if (UserData::getGuidePass(GameData::getInstance()->getLevel())) {
			GameData::getInstance()->addEnergy(s_3star_energy_add);
		}	
	} else if (GameData::getInstance()->getStar() >= 2) {
		if (UserData::getGuidePass(GameData::getInstance()->getLevel())) {
			GameData::getInstance()->addEnergy(s_2star_energy_add);
		}	
	} else if (GameData::getInstance()->getStar() >= 1) {
		if (UserData::getGuidePass(GameData::getInstance()->getLevel())) {
			GameData::getInstance()->addEnergy(s_1star_energy_add);
		}
	}

	//发送GameOver事件
	GameEvent::throwEvent(Event_Game_Over, NULL);
}

void GameManager::overWithoutNotice() {
	m_bGameStarted		= false;
	m_bGamePaused		= false;
	m_bGameOver			= true;
	GameEvent::throwEvent(Event_Game_Finish, NULL);
}

bool GameManager::isStarted() {
	return m_bGameStarted;
}

bool GameManager::isPaused() {
	return m_bGamePaused;
}

bool GameManager::isOver() {
	return m_bGameOver;
}

void GameManager::addScore(int score) {
	if (score <= 0 || isOver()) {
		return;
	}

	GameData::getInstance()->addScore(score);
	
	if (GameData::getInstance()->getScore() >= GameData::getInstance()->getTargetScore()) {
		over();
		GameData::getInstance()->setHisLevel(GameData::getInstance()->getLevel());
	}

	GameEvent::throwEvent(Event_Score_Change, NULL);
}

void GameManager::selectCross(bool selected) {
	if (m_bSelectCross != selected) {
		m_bSelectCross = selected;
		if (!selected) {
			GameEvent::throwEvent(Event_Release_Cross, NULL);
		}
	}
}