#include "LuckyTimeManager.h"
#include "UserData.h"
#include "global.h"
#include "cocos2d.h"
#include "GameData.h"
#include "GameEvent.h"
#include "GameUtil.h"
USING_NS_CC;

LuckyTimeManager::LuckyTimeManager():
m_lastLuckyTime(0),
m_countdown(0),
m_bLotteryFlag(false)
{

}

LuckyTimeManager::~LuckyTimeManager() {

}

LuckyTimeManager* LuckyTimeManager::m_instance = nullptr;
LuckyTimeManager* LuckyTimeManager::getInstance(){
	if(m_instance == nullptr){
		m_instance = new LuckyTimeManager();
		m_instance->init();
	}
	return m_instance;
}

void LuckyTimeManager::init() {
	loadTime();
}

void LuckyTimeManager::loadTime() {
	std::string ts = UserData::getLuckyTime();
	m_lastLuckyTime = atol(ts.c_str());

	time_t nowTime = GameUtil::getNowTime();
	if (0 >= m_lastLuckyTime || nowTime - m_lastLuckyTime >= 3600) {
		m_countdown = 0;
		m_bLotteryFlag = false;
	} else {
		m_bLotteryFlag = true;
	
		m_countdown = m_lastLuckyTime + 3600 - nowTime;
	}
}

void LuckyTimeManager::saveTime() {
	UserData::setLuckyTime(String::createWithFormat("%ld", GameUtil::getNowTime())->_string);
	init();
}

void LuckyTimeManager::on1sTimer() {
	if (m_bLotteryFlag && m_countdown > 0) {
		m_countdown--;
		if (0 == m_countdown) {
			m_bLotteryFlag = false;
		}
	}
}
