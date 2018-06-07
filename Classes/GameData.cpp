#include "GameData.h"
#include "UserData.h"
#include "LevelData.h"
#include "GameEvent.h"

USING_NS_CC;

GameData * gGameData = NULL;

GameData::GameData():
m_level(0),
m_score(0),
m_targetScore(0),
m_boss(0),
m_timeLimit(0),
m_energy(0),
m_hisLevel(0),
m_special_cross(0),
m_special_time(0),
m_special_tip(0),
m_special_bomb(0),
m_elapseTime(0)
{

}

GameData::~GameData() {
}

GameData * GameData::getInstance() {
	if (NULL == gGameData) {
		gGameData = new GameData();
		gGameData->init();
	}

	return gGameData;
}

void GameData::init() {
	m_score			= 0;
	m_targetScore	= 0;
	m_timeLimit		= 0;
	m_level			= 0;
	m_boss			= 0;
	m_lotteryed		= false;

	m_special_cross	= 0;
	m_special_time	= 0;
	m_special_tip	= 0;
	m_special_bomb	= 0;

	loadStarsData();
	loadLotteryData();
	loadToolData();
	loadEnergyData();
	loadLevelData();
	loadLevelLockData();
	loadBossData();
	m_level = m_hisLevel;
}

void GameData::init(int level) {
	if (LevelDataManager::getInstance()->getLevelData(m_levelData, level)) {
		m_score			= 0;
		m_targetScore	= m_levelData.iTargetScore;
		m_timeLimit		= m_levelData.iTime;
		m_boss			= m_levelData.iBoss;
		m_time			= m_timeLimit;
		m_star			= 0;
		m_lotteryed		= getLotteryFlag(level);
	} else {
		CCASSERT(false, "level init error");
	}
}

void GameData::loadStarsData() {
	Data starsData = UserData::getStarsData();
	unsigned char *data = starsData.getBytes();
	const int size = starsData.getSize();
	if (NULL != data && size >= MAX_LEVEL) {
		for (int i=0; i<MAX_LEVEL; i++) {
			m_stars[i] = data[i];
		}
	} else {
		for (int i=0; i<MAX_LEVEL; i++) {
			m_stars[i] = 0;
		}
	}
}

void GameData::saveStarsData() {
	m_starsData.copy(m_stars, sizeof(m_stars));
	UserData::setStarsData(m_starsData);
}

int GameData::getStar(int level) {
	if (level >= MAX_LEVEL || level <0) {
		CCASSERT(false,"level error");
		return 0;
	}

	return m_stars[level];
}

void GameData::setStar() {
	if (m_level >= MAX_LEVEL || m_level <0) {
		CCASSERT(false,"level error");
		return ;
	}

	m_star = 0;
	
	if (m_time >= m_levelData.i3StarSec) {
		m_star = 3;
	} else if (m_time >= m_levelData.i2StarSec) {
		m_star = 2;
	} else if (m_score >= m_targetScore) {
		m_star = 1;
	} else {
		m_star = 0;
	}

	const int hisStar = m_stars[m_level];
	if (m_star > hisStar) {
		m_stars[m_level] = m_star;
		saveStarsData();
	}
}

void GameData::setStar(int level, int star) {
	if (level >= MAX_LEVEL || level <0) {
		CCASSERT(false,"level error");
		return ;
	}

	const int hisStar = m_stars[level];
	if (star > hisStar) {
		m_stars[level] = star;
		saveStarsData();
	}
}

void GameData::addEnergy(int e) { 
	m_energy += e; 
	
	if (m_energy < 0) {
		m_energy = 0;
	}

	saveEnergyData();
	GameEvent::throwEvent(Event_Energy_Change, NULL);
}

void GameData::addScore(int s) {
	m_score += s;
}

void GameData::loadLotteryData() {
	Data lotteryData = UserData::getLotteryData();
	unsigned char *data = lotteryData.getBytes();
	const int size = lotteryData.getSize();
	if (NULL != data && size >= MAX_LEVEL) {
		for (int i=0; i<MAX_LEVEL; i++) {
			m_lotteryFlag[i] = data[i];
		}
	} else {
		for (int i=0; i<MAX_LEVEL; i++) {
			m_lotteryFlag[i] = 0;
		}
	}
}

void GameData::saveLotteryData() {
	m_lotteryData.copy(m_lotteryFlag, sizeof(m_lotteryFlag));
	UserData::setStarsData(m_lotteryData);
}

bool GameData::getLotteryFlag(int level) {
	if (level <0 || level >= MAX_LEVEL) {
		CCASSERT(false, "level error");
		return false;
	}

	return m_lotteryFlag[level];
}
void GameData::setLotteryFlag(bool lotteryed, int level) {
	if (level <0 || level >= MAX_LEVEL) {
		CCASSERT(false, "level error");
		return;
	}

	m_lotteryFlag[level] = lotteryed;
	saveLotteryData();
}

void GameData::loadToolData() {
	m_special_cross = UserData::getToolCrossNum();
	m_special_time	= UserData::getToolTimeNum();
	m_special_tip	= UserData::getToolTipNum();
	m_special_bomb	= UserData::getToolBombNum();
}

void GameData::addToolCross(int n) {
	m_special_cross += n;
	if (m_special_cross < 0) {
		m_special_cross = 0;
	}

	UserData::setToolCrossNum(m_special_cross);
	GameEvent::throwEvent(Event_Tool_Change, NULL);
}

void GameData::addToolTime(int n) {
	m_special_time += n;
	if (m_special_time < 0) {
		m_special_time = 0;
	}

	UserData::setToolTimeNum(m_special_time);
	GameEvent::throwEvent(Event_Tool_Change, NULL);
}

void GameData::addToolTip(int n) {
	m_special_tip += n;
	if (m_special_tip < 0) {
		m_special_tip = 0;
	}

	UserData::setToolTipNum(m_special_tip);
	GameEvent::throwEvent(Event_Tool_Change, NULL);
}

void GameData::addToolBomb(int n) {
	m_special_bomb += n;
	if (m_special_bomb < 0) {
		m_special_bomb = 0;
	}

	UserData::setToolBombNum(m_special_bomb);
	GameEvent::throwEvent(Event_Tool_Change, NULL);
}

int GameData::getStarSum() {
	int sum = 0;
	for (int i=0; i<MAX_LEVEL; i++) {
		sum += m_stars[i];
	}

	return sum;
}

void GameData::loadEnergyData() {
	m_energy = UserData::getEnergyNum();
}

void GameData::saveEnergyData() {
	UserData::setEnergyNum(m_energy);
}

void GameData::setHisLevel(int level) {
	if (level > m_hisLevel) {
		m_hisLevel = level;
		saveLevelData();
	}
}

void GameData::loadLevelData() {
	m_hisLevel = UserData::getHisLevel();
	if (m_hisLevel < 0) {
		m_hisLevel = 0;
	}

	if (m_hisLevel >= MAX_LEVEL) {
		m_hisLevel = MAX_LEVEL - 1;
	}
}

void GameData::saveLevelData() {
	if (m_hisLevel < 0) {
		m_hisLevel = 0;
	}

	if (m_hisLevel >= MAX_LEVEL) {
		m_hisLevel = MAX_LEVEL - 1;
	}

	UserData::setHisLevel(m_hisLevel);
}

void GameData::clearData() {
	memset(m_stars, 0, sizeof(m_stars));
	m_starsData.copy(m_stars, sizeof(m_stars));
	UserData::setStarsData(m_starsData);
	UserData::setHisLevel(0);

	memset(m_levelLockFlag, 0, sizeof(m_levelLockFlag));
	m_levelLockData.copy(m_levelLockFlag, sizeof(m_levelLockFlag));
	UserData::setLevelLockData(m_levelLockData);

	memset(m_lotteryFlag, 0, sizeof(m_lotteryFlag));
	m_lotteryData.copy(m_lotteryFlag, sizeof(m_lotteryFlag));
	UserData::setLotteryData(m_lotteryData);

	std::string zero("0");
	UserData::setLuckyTime(zero);

	UserData::setToolBombNum(0);
	UserData::setToolCrossNum(0);
	UserData::setToolTimeNum(0);
	UserData::setToolTipNum(0);

	UserData::setEnergyNum(25);
	UserData::setTouchErrTipNum(0);

	UserData::setGuidePass(0, false);
	UserData::setGuidePass(1, false);
	UserData::setGuidePass(2, false);
	UserData::setGuidePass(3, false);
	UserData::setGuidePass(4, false);
	UserData::setGuidePass(5, false);
	UserData::setGuidePass(6, false);
	UserData::setIntroPass(false);
}

int GameData::getTargetStar(int level) {
	if (level < 50) {
		return 50;//平均1个星星+5个星星
	} else if (level < 75) {
		return 100;//平均2个星星
	} else if (level < 100) {
		return 150 + 65;//平均2个星星+65关达到3星
	} else {
		return 300 - 5;//全部3星-5个星星
	}
}

void GameData::loadLevelLockData() {
	Data lockData = UserData::getLevelLockData();
	unsigned char *data = lockData.getBytes();
	const int size = lockData.getSize();
	if (NULL != data && size >= MAX_LOCK) {
		for (int i=0; i<MAX_LOCK; i++) {
			m_levelLockFlag[i] = data[i];
		}
	} else {
		for (int i=0; i<MAX_LOCK; i++) {
			m_levelLockFlag[i] = 0;
		}
	}
}

void GameData::loadBossData() {
	for (int i=0; i<MAX_LEVEL; i++) {
		LevelData tempData;
		if (LevelDataManager::getInstance()->getLevelData(tempData, i)) {
			m_bossType[i] = tempData.iBoss;
		} else {
			CCASSERT(false, "level init error");
			m_bossType[i] = 0;
		}
	}
}

void GameData::saveLevelLockData() {	
	m_levelLockData.copy(m_levelLockFlag, sizeof(m_levelLockFlag));
	UserData::setLevelLockData(m_levelLockData);
}

bool GameData::isLevelUnlock(int level) {
	if (level < 25) {
		return true;
	}

	int index = level/25;
	if (index >= MAX_LOCK) {
		index = MAX_LOCK - 1;
	}

	return m_levelLockFlag[index];
}

void GameData::setLevelUnlock(int level, bool unLocked) {
	if (level < 25) {
		return;
	}

	int index = level/25;
	if (index >= MAX_LOCK) {
		index = MAX_LOCK - 1;
	}

	m_levelLockFlag[index] = unLocked;
	saveLevelLockData();
}

int GameData::getBossType(int level) {
	if (level < 0 || level >= MAX_LEVEL) {
		return 0;
	}

	return m_bossType[level];
}