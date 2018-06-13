#ifndef __setting_manager_h__
#define __setting_manager_h__

#include "cocos2d.h"

enum EnumSetting {
	Setting_rookie			= 1,//已实现
	Setting_happy			= 2,//已实现
	Setting_luckylottery	= 3,//已实现
	Setting_addenergy		= 4,
	Setting_discount		= 5,//已实现
	Setting_addtime			= 6,
	Setting_nextgame		= 7,//已实现
	Setting_biglihe			= 8,
	Setting_allopen			= 9,
	Setting_suprise			= 10,//已实现
	Setting_biglibao		= 11,//尊享礼包已实现
	Setting_buytime			= 12,
	Setting_buytip			= 13,
	Setting_buybomb			= 14,
	Setting_buycross		= 15,
	Setting_unlock			= 16,
	Setting_fetchall		= 17,
	Setting_exit			= 18,//已实现
	Setting_buyenergy		= 19,//
	Setting_toolconfirm		= 20,//道具确认
	Setting_giftmore		= 21,//好礼多多
	Setting_guidetip		= 22,//查看引导
	
	MAX_Setting,
};

class SettingManager {
public:
	static SettingManager * getInstance(); 
	bool isOpen(int id);
	int getZunxiangSetting();
	int getZhizunSetting();
	int getExitSetting();

private:
	SettingManager();
	~SettingManager();

	static SettingManager * s_settingManager;
	void init();
	bool readSettingData();
	void setValue(int id, int value);

	static const int MAX_ID = MAX_Setting;

private:
	int m_setting[MAX_ID];
};

#endif