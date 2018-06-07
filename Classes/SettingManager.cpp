#include "SettingManager.h"
#include "Resource.h"
#include "GameData.h"
#include "cocostudio/CocoStudio.h"
USING_NS_CC;


SettingManager * SettingManager::s_settingManager = nullptr;

SettingManager::SettingManager()
{
}

SettingManager::~SettingManager() {
}

SettingManager * SettingManager::getInstance() {
	if (nullptr == s_settingManager) {
		s_settingManager = new SettingManager();
		s_settingManager->init();
	}

	return s_settingManager;
}

void SettingManager::init() {
	for (int i=0; i<MAX_ID; i++) {
		m_setting[i] = false;
	}

	readSettingData();
}

bool SettingManager::isOpen(int id) {
	if (id <= 0 || id > MAX_ID) {
		return false;
	}

	return (m_setting[id] != 0);
}

void SettingManager::setValue(int id, int value) {
	if (id < 0 || id > MAX_ID) {
		return;
	}

	m_setting[id] = value;
}

bool SettingManager::readSettingData() {

	//json 文档
	rapidjson::Document _doc;
	bool bRet = false;
	ssize_t size = 0;
	unsigned char *pBytes = NULL;
	do {
		pBytes = cocos2d::CCFileUtils::sharedFileUtils()->getFileData(s_file_setting_data, "r", &size);
		CC_BREAK_IF(pBytes == NULL || strcmp((char*)pBytes, "") == 0);
		std::string load_str((const char*)pBytes, size);
		CC_SAFE_DELETE_ARRAY(pBytes);
		_doc.Parse<0>(load_str.c_str());
		CC_BREAK_IF(_doc.HasParseError());			
		//生成json文档对像

		if(!_doc.IsObject())
			return false;

		//是否有此成员
		if(!_doc.HasMember("settings"))
			return false;

		//--------------------解析消除物信息-------------------------------
		const rapidjson::Value &pArray = _doc["settings"];
		if(pArray.IsArray()) {
			for (rapidjson::SizeType i = 0; i < pArray.Size(); i++)
			{
				const rapidjson::Value &p = pArray[i];				
				if(p.HasMember("setting"))
				{
					const rapidjson::Value &valueEnt = p["setting"];
					if(valueEnt.HasMember("id") && valueEnt.HasMember("value"))
					{
						const rapidjson::Value &idEnt = valueEnt["id"];
						int id = idEnt.GetInt();

						const rapidjson::Value &switchEnt = valueEnt["value"];
						int value = switchEnt.GetInt();

						setValue(id, value);
					}
				}
			}
		}

		bRet = true;

	} while (0);

	return bRet;
}

int SettingManager::getZunxiangSetting() {
	return m_setting[Setting_biglibao];
}

int SettingManager::getZhizunSetting() {
	return m_setting[Setting_biglihe];
}

int SettingManager::getExitSetting() {
	return m_setting[Setting_exit];
}