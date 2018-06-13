#ifndef __TOWER_ANALYTICS_H__
#define __TOWER_ANALYTICS_H__
#include "cocos2d.h"

class Analytics {
public:
	static void setAccount();
	static void onBegin(int level);
	static void onComplete(int level);
	static void onFailed(int level);
};

#define ROOKIE_LEVEL_INTRO		1000
#define ROOKIE_LEVEL_RULE		1001
#define ROOKIE_LEVEL_CROSS		1002
#define ROOKIE_LEVEL_BOMB		1003
#define ROOKIE_LEVEL_TIMETIP	1004
#define ROOKIE_LEVEL_BOMPET		1005
#define ROOKIE_LEVEL_COLORPET	1006
#define ROOKIE_LEVEL_BOSS		1007

class UMengAnalytics {
public:
	static void onBegin(int level);
	static void onComplete(int level, int sec, int targetSec);
	static void onFailed(int level, int score, int targetScore);
	static void onFailedBeforeSuccess(int level, int score, int targetScore);
};

#endif