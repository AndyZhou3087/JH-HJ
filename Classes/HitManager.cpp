#include "HitManager.h"
#include "GameUtil.h"

USING_NS_CC;

HitManager* HitManager::m_instance = nullptr;
HitManager* HitManager::getInstance(){
	if(m_instance == nullptr){
		m_instance = new HitManager();
	}
	return m_instance;
}

HitManager::HitManager():
m_hitCount(0),
m_lastHitTime(0)
{
	init();
}

HitManager::~HitManager() {

}

void HitManager::init() {

}

int HitManager::hit() {
	if ( m_hitCount < 1) {
		m_hitCount = 1;
		m_lastHitTime = GameUtil::getNowTimeMs();
		return m_hitCount;
	}

	long long thisHitTime = GameUtil::getNowTimeMs();

	const int iTimeIntrvalArray[] = {HIT_INTRVAL_2, HIT_INTRVAL_3, HIT_INTRVAL_4, HIT_INTRVAL_5, HIT_INTRVAL_6, HIT_INTRVAL_7};
	int iTimeIntrval = HIT_INTRVAL_7;
	if (m_hitCount - 1 < sizeof(iTimeIntrval)/sizeof(iTimeIntrvalArray[0])) {
		iTimeIntrval = iTimeIntrvalArray[m_hitCount - 1];
	}

	if (thisHitTime - m_lastHitTime <= iTimeIntrval) {
		m_hitCount++;
	} else {
		m_hitCount = 1;
	}

	m_lastHitTime = thisHitTime;
	return m_hitCount;
}

int HitManager::getHit() {
	if (m_hitCount > 0) {
		updateHit();
	}

	return m_hitCount;
}

void HitManager::updateHit() {
	if (m_hitCount < 1) {
		return;
	}

	long long thisHitTime = GameUtil::getNowTimeMs();

	const int iTimeIntrvalArray[] = {HIT_INTRVAL_2, HIT_INTRVAL_3, HIT_INTRVAL_4, HIT_INTRVAL_5, HIT_INTRVAL_6, HIT_INTRVAL_7};
	int iTimeIntrval = HIT_INTRVAL_7;
	if (m_hitCount - 1 < sizeof(iTimeIntrval)/sizeof(iTimeIntrvalArray[0])) {
		iTimeIntrval = iTimeIntrvalArray[m_hitCount - 1];
	}

	if (thisHitTime - m_lastHitTime > iTimeIntrval) {
		reset();
	}
}
