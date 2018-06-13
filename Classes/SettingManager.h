#ifndef __setting_manager_h__
#define __setting_manager_h__

#include "cocos2d.h"

enum EnumSetting {
	Setting_rookie			= 1,//��ʵ��
	Setting_happy			= 2,//��ʵ��
	Setting_luckylottery	= 3,//��ʵ��
	Setting_addenergy		= 4,
	Setting_discount		= 5,//��ʵ��
	Setting_addtime			= 6,
	Setting_nextgame		= 7,//��ʵ��
	Setting_biglihe			= 8,
	Setting_allopen			= 9,
	Setting_suprise			= 10,//��ʵ��
	Setting_biglibao		= 11,//���������ʵ��
	Setting_buytime			= 12,
	Setting_buytip			= 13,
	Setting_buybomb			= 14,
	Setting_buycross		= 15,
	Setting_unlock			= 16,
	Setting_fetchall		= 17,
	Setting_exit			= 18,//��ʵ��
	Setting_buyenergy		= 19,//
	Setting_toolconfirm		= 20,//����ȷ��
	Setting_giftmore		= 21,//������
	Setting_guidetip		= 22,//�鿴����
	
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