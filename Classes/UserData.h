#ifndef __TOWER_USERDATA_H__
#define __TOWER_USERDATA_H__

#include "global.h"
#include "cocos2d.h"


class UserData {
public:
	static bool getFirstUsedFlag();
	static void setFirstUsedFlag(bool firstFlag);

	static bool getSoundSwitch();
	static void setSoundSwitch(bool soundSwitch);

	static int getBestScore();
	static void setBestScore(int score);

	static int getScore();
	static void setScore(int score);

	static cocos2d::Data getStarsData();
	static void setStarsData(const cocos2d::Data&);

	static cocos2d::Data getLotteryData();
	static void setLotteryData(const cocos2d::Data&);

	static int getHisLevel();
	static void setHisLevel(int level);

	static int getToolCrossNum();
	static void setToolCrossNum(int n);

	static int getToolTimeNum();
	static void setToolTimeNum(int n);

	static int getToolTipNum();
	static void setToolTipNum(int n);

	static int getToolBombNum();
	static void setToolBombNum(int n);

	static int getEnergyNum();
	static void setEnergyNum(int n);

	static std::string getEnergyTime();
	static void setEnergyTime(std::string &t);

	static std::string getLuckyTime();
	static void setLuckyTime(std::string &t);

	static bool getFirstLucky();
	static void setFirstLucky(bool f);

	static cocos2d::Data getLevelLockData();
	static void setLevelLockData(const cocos2d::Data&);

	static int getTouchErrTipNum();
	static void setTouchErrTipNum(int n);

	static bool getGuidePass(int level);
	static void setGuidePass(int level, bool pass);

	static bool getIntroPass();
	static void setIntroPass(bool pass);
};

#endif