#include "UserData.h"
#include <cocos2d.h>

USING_NS_CC;
#define USER_DATA_SOUND_SWITCH				"user_data_sound_switch"
#define USER_DATA_FIRST_USED_FLAG			"user_data_first_used" 
#define USER_DATA_KEY_BEST_SCORE			"user_data_best_score"
#define USER_DATA_GAME_SCORE				"user_data_game_score"
#define USER_DATA_GAME_STARS_DATA			"user_data_game_stars_data"
#define USER_DATA_GAME_LOTTERY_DATA			"user_data_game_lottery_data"
#define USER_DATA_GAME_HIS_LEV				"user_data_game_his_lev"
#define USER_DATA_ENERGY_NUM				"user_data_energy_num"
#define USER_DATA_ENERGY_TIME				"user_data_energy_time"
#define USER_DATA_LUCKY_TIME				"user_data_lucky_time"

#define USER_DATA_TOOL_CROSS_NUM			"user_data_tool_cross"
#define USER_DATA_TOOL_TIME_NUM				"user_data_tool_time"
#define USER_DATA_TOOL_TIP_NUM				"user_data_tool_tip"
#define USER_DATA_TOOL_BOMB_NUM				"user_data_tool_bomb"

#define USER_DATA_FIRST_LUCKY_FLAG			"user_data_first_lucky"
#define USER_DATA_LEVEL_LOCK_FLAG			"user_data_lock_flag"

#define USER_DATA_TOUCH_ERR_TIP_NUM			"user_data_touch_err_tip_num"

#define USER_DATA_GUIDE_1					"user_data_guide_1"
#define USER_DATA_GUIDE_2					"user_data_guide_2"
#define USER_DATA_GUIDE_3					"user_data_guide_3"
#define USER_DATA_GUIDE_4					"user_data_guide_4"
#define USER_DATA_GUIDE_5					"user_data_guide_5"
#define USER_DATA_GUIDE_6					"user_data_guide_6"
#define USER_DATA_GUIDE_7					"user_data_guide_7"

#define USER_DATA_GUIDE_INTRO				"user_data_guide_intro"

bool UserData::getFirstUsedFlag() {
	return CCUserDefault::sharedUserDefault()->getBoolForKey(USER_DATA_FIRST_USED_FLAG, true);
}

void UserData::setFirstUsedFlag(bool firstUsed) {
	CCUserDefault::sharedUserDefault()->setBoolForKey(USER_DATA_FIRST_USED_FLAG, firstUsed);
}

bool UserData::getSoundSwitch(){
	return CCUserDefault::sharedUserDefault()->getBoolForKey(USER_DATA_SOUND_SWITCH, true);
}

void UserData::setSoundSwitch(bool soundSwitch){
	return CCUserDefault::sharedUserDefault()->setBoolForKey(USER_DATA_SOUND_SWITCH, soundSwitch);
}

int UserData::getBestScore() {
	return CCUserDefault::sharedUserDefault()->getIntegerForKey(USER_DATA_KEY_BEST_SCORE, 0);
}

void UserData::setBestScore(int score) {
	CCUserDefault::sharedUserDefault()->setIntegerForKey(USER_DATA_KEY_BEST_SCORE, score);
}

int UserData::getScore() {
	return CCUserDefault::sharedUserDefault()->getIntegerForKey(USER_DATA_GAME_SCORE, 0);
}

void UserData::setScore(int score) {
	CCUserDefault::sharedUserDefault()->setIntegerForKey(USER_DATA_GAME_SCORE, score);
}

Data UserData::getStarsData() {
	return CCUserDefault::sharedUserDefault()->getDataForKey(USER_DATA_GAME_STARS_DATA, Data::Null);
}

void UserData::setStarsData(const Data &data) {
	return CCUserDefault::sharedUserDefault()->setDataForKey(USER_DATA_GAME_STARS_DATA, data);
}

Data UserData::getLotteryData() {
	return CCUserDefault::sharedUserDefault()->getDataForKey(USER_DATA_GAME_LOTTERY_DATA, Data::Null);
}

void UserData::setLotteryData(const cocos2d::Data& data) {
	return CCUserDefault::sharedUserDefault()->setDataForKey(USER_DATA_GAME_LOTTERY_DATA, data);
}

int UserData::getHisLevel() {
	return CCUserDefault::sharedUserDefault()->getIntegerForKey(USER_DATA_GAME_HIS_LEV, 0);
}

void UserData::setHisLevel(int level) {
	CCUserDefault::sharedUserDefault()->setIntegerForKey(USER_DATA_GAME_HIS_LEV, level);
}

int UserData::getToolCrossNum() {
	return CCUserDefault::sharedUserDefault()->getIntegerForKey(USER_DATA_TOOL_CROSS_NUM, 0);
}

void UserData::setToolCrossNum(int n) {
	CCUserDefault::sharedUserDefault()->setIntegerForKey(USER_DATA_TOOL_CROSS_NUM, n);
}

int UserData::getToolTimeNum() {
	return CCUserDefault::sharedUserDefault()->getIntegerForKey(USER_DATA_TOOL_TIME_NUM, 0);
}

void UserData::setToolTimeNum(int n) {
	CCUserDefault::sharedUserDefault()->setIntegerForKey(USER_DATA_TOOL_TIME_NUM, n);
}

int UserData::getToolTipNum() {
	return CCUserDefault::sharedUserDefault()->getIntegerForKey(USER_DATA_TOOL_TIP_NUM, 0);
}

void UserData::setToolTipNum(int n) {
	CCUserDefault::sharedUserDefault()->setIntegerForKey(USER_DATA_TOOL_TIP_NUM, n);
}

int UserData::getToolBombNum() {
	return CCUserDefault::sharedUserDefault()->getIntegerForKey(USER_DATA_TOOL_BOMB_NUM, 0);
}

void UserData::setToolBombNum(int n) {
	CCUserDefault::sharedUserDefault()->setIntegerForKey(USER_DATA_TOOL_BOMB_NUM, n);
}

int UserData::getEnergyNum() {
	return CCUserDefault::sharedUserDefault()->getIntegerForKey(USER_DATA_ENERGY_NUM, 25);
}

void UserData::setEnergyNum(int n) {
	CCUserDefault::sharedUserDefault()->setIntegerForKey(USER_DATA_ENERGY_NUM, n);
}

std::string UserData::getEnergyTime() {
	return CCUserDefault::sharedUserDefault()->getStringForKey(USER_DATA_ENERGY_TIME, "0");
}

void UserData::setEnergyTime(std::string & s) {
	CCUserDefault::sharedUserDefault()->setStringForKey(USER_DATA_ENERGY_TIME, s);
}

std::string UserData::getLuckyTime() {
	return CCUserDefault::sharedUserDefault()->getStringForKey(USER_DATA_LUCKY_TIME, "0");
}

void UserData::setLuckyTime(std::string & s) {
	CCUserDefault::sharedUserDefault()->setStringForKey(USER_DATA_LUCKY_TIME, s);
}

bool UserData::getFirstLucky() {
	return CCUserDefault::sharedUserDefault()->getBoolForKey(USER_DATA_FIRST_LUCKY_FLAG, true);
}

void UserData::setFirstLucky(bool f) {
	CCUserDefault::sharedUserDefault()->setBoolForKey(USER_DATA_FIRST_LUCKY_FLAG, f);
}

Data UserData::getLevelLockData() {
	return CCUserDefault::sharedUserDefault()->getDataForKey(USER_DATA_LEVEL_LOCK_FLAG, Data::Null);
}

void UserData::setLevelLockData(const cocos2d::Data& data) {
	return CCUserDefault::sharedUserDefault()->setDataForKey(USER_DATA_LEVEL_LOCK_FLAG, data);
}


int UserData::getTouchErrTipNum() {
	return CCUserDefault::sharedUserDefault()->getIntegerForKey(USER_DATA_TOUCH_ERR_TIP_NUM, 0);
}

void UserData::setTouchErrTipNum(int n) {
	CCUserDefault::sharedUserDefault()->setIntegerForKey(USER_DATA_TOUCH_ERR_TIP_NUM, n);
}

bool UserData::getGuidePass(int level) {
	const char * flag[] = {
		USER_DATA_GUIDE_1,
		USER_DATA_GUIDE_2,
		USER_DATA_GUIDE_3,
		USER_DATA_GUIDE_4,
		USER_DATA_GUIDE_5,
		USER_DATA_GUIDE_6,
		USER_DATA_GUIDE_7,
	};

	if (level >= sizeof(flag)/sizeof(flag[0])) {
		return true;
	}


	return CCUserDefault::sharedUserDefault()->getBoolForKey(flag[level], false);
}

void UserData::setGuidePass(int level, bool pass) {
	const char * flag[] = {
		USER_DATA_GUIDE_1,
		USER_DATA_GUIDE_2,
		USER_DATA_GUIDE_3,
		USER_DATA_GUIDE_4,
		USER_DATA_GUIDE_5,
		USER_DATA_GUIDE_6,
		USER_DATA_GUIDE_7,
	};

	if (level < sizeof(flag)/sizeof(flag[0])) {
		CCUserDefault::sharedUserDefault()->setBoolForKey(flag[level], pass);
	}
}

bool UserData::getIntroPass() {
	return CCUserDefault::sharedUserDefault()->getBoolForKey(USER_DATA_GUIDE_INTRO, false);
}

void UserData::setIntroPass(bool pass) {
	CCUserDefault::sharedUserDefault()->setBoolForKey(USER_DATA_GUIDE_INTRO, pass);
}