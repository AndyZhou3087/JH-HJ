#ifndef _HITMANAGER_H_
#define _HITMANAGER_H_

#include "cocos2d.h"

#define HIT_INTRVAL_2	2000
#define HIT_INTRVAL_3	1500
#define HIT_INTRVAL_4	1200
#define HIT_INTRVAL_5	1000
#define HIT_INTRVAL_6	1000
#define HIT_INTRVAL_7	1000

class HitManager {
public:
	~HitManager();
	static HitManager* getInstance();
	void init();
	inline void reset() { m_hitCount = 0; m_lastHitTime = 0; }
	int hit();
	int getHit();

private:
	HitManager();

	void updateHit();

private:
	static HitManager* m_instance;

	int m_hitCount;
	long long m_lastHitTime;
};
#endif