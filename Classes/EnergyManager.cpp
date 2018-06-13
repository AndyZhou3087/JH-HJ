#include "EnergyManager.h"
#include "UserData.h"
#include "global.h"
#include "cocos2d.h"
#include "GameData.h"
#include "GameEvent.h"
#include "GameUtil.h"
USING_NS_CC;

EnergyManager::EnergyManager():
m_lastSaveTime(0),
m_countdown(s_energy_count_down)
{

}

EnergyManager::~EnergyManager() {

}

EnergyManager* EnergyManager::m_instance = nullptr;
EnergyManager* EnergyManager::getInstance(){
	if(m_instance == nullptr){
		m_instance = new EnergyManager();
		m_instance->init();
	}
	return m_instance;
}

void EnergyManager::init() {
	m_countdown = s_energy_count_down;
	loadTime();
}

void EnergyManager::loadTime() {
	std::string ts = UserData::getEnergyTime();
	m_lastSaveTime = atol(ts.c_str());

	long nowTime = GameUtil::getNowTime();
	if (m_lastSaveTime <= 0 || m_lastSaveTime > nowTime) {
		m_lastSaveTime = nowTime;
		saveTime();
	} else {
		long dt = nowTime - m_lastSaveTime;
		int add_times = dt/s_energy_count_down;
		if (add_times > 0 && GameData::getInstance()->getEnergy() < s_energy_recover_limit) {
			const int df = add_times * s_energy_recover;
			if (GameData::getInstance()->getEnergy() +  df> s_energy_recover_limit) {
				GameData::getInstance()->addEnergy(s_energy_recover_limit - GameData::getInstance()->getEnergy());
			} else {
				GameData::getInstance()->addEnergy(df);
			}			
		}
		m_countdown = s_energy_count_down - dt%s_energy_count_down;
		m_lastSaveTime = nowTime - (s_energy_count_down - m_countdown);
		saveTime();
	}
}

void EnergyManager::saveTime() {
	UserData::setEnergyTime(String::createWithFormat("%ld", m_lastSaveTime)->_string);
}

void EnergyManager::on1sTimer() {
	if (m_countdown-- <= 0) {
		//»Ö¸´ÌåÁ¦
		if (GameData::getInstance()->getEnergy() < s_energy_recover_limit) {
			GameData::getInstance()->addEnergy(s_energy_recover);
		}
		
		m_countdown = s_energy_count_down;
		m_lastSaveTime = GameUtil::getNowTime();
		saveTime();
		GameEvent::throwEvent(Event_Energy_Change, NULL);
	}
}