#ifndef _GAME_UTIL_H_
#define _GAME_UTIL_H_

#include <cocos2d.h>
#include "global.h"

class GameUtil {
public:
	static void initRandSeed();
	static time_t getNowTime();
	static long long getNowTimeMs();
	static bool isBeforeToday(time_t sec);
	static long long getTodayLeftSec();

	static bool getRandomBoolean(float rate);
	static bool getRandomBoolean();
	static int getRandomNum(int range);
	static int getRandomNum(int rangeStart, int rangeEnd);
	
	static void shake(cocos2d::Node * node, float scaleLarge, float scaleSmall); 
	static void shake(cocos2d::Node * node);

	static void jump(cocos2d::Node *node, float dt, bool repeat = false, float intrval = 0);
	static void jellyJump(cocos2d::Node *node, float dt, bool repeat = false, float intrval = 0, int tag = 0);
	static void petJump(cocos2d::Node *node, float dt, bool repeat = false, float intrval = 0, int tag = 0, cocos2d::ActionInterval *ac = nullptr);
	static void jelly(cocos2d::Node *node, bool repeat = false, float intrval = 0, bool delay = false, int tag = 0);

	static void jumpDown(cocos2d::Node *node, float dt);
	
	static bool isPhone();
};

class HelpObj:public cocos2d::Ref {
public:
	~HelpObj();
	static HelpObj* getInstance();
private:
	HelpObj();
private:
	static HelpObj* m_instance;
};
class TestObj:public cocos2d::Ref {
public:
	~TestObj();
	static TestObj* getInstance();
private:
	TestObj();
private:
	static TestObj* m_instance;
};
#endif
