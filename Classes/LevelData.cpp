#include "LevelData.h"
#include "Resource.h"
#include "GameData.h"
USING_NS_CC;

LevelData::LevelData():
iLevel(0),
iTargetScore(0),
i1StarScore(0),
i2StarSec(0),
i3StarSec(0),
iTime(0),
iClear3x3(0),
iShade(0),
iFace(0),
iIce(0),
iChange(0),
iEveryok(0),
iBoss(0)
{

}

LevelData::~LevelData() {

}

LevelDataManager * LevelDataManager::s_levelDataManager = nullptr;

LevelDataManager::LevelDataManager():
m_levelDataDoc(NULL)
{
}

LevelDataManager::~LevelDataManager() {
	if (NULL != m_levelDataDoc) {
		delete m_levelDataDoc;
		m_levelDataDoc = NULL;
	}
}

LevelDataManager * LevelDataManager::getInstance() {
	if (nullptr == s_levelDataManager) {
		s_levelDataManager = new LevelDataManager();
		s_levelDataManager->init();
	}

	return s_levelDataManager;
}

void LevelDataManager::init() {
	m_levelDataDoc = new tinyxml2::XMLDocument();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	ssize_t size;
	char * content=(char *)FileUtils::getInstance()->getFileData(s_file_level_data,"r",&size); 
	if(content){
		m_levelDataDoc->Parse(content,size+1);
	}
#endif


#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	XMLError errorId = m_levelDataDoc->LoadFile(FileUtils::getInstance()->fullPathForFilename(s_file_level_data).c_str());
	if (errorId != 0) {
		delete m_levelDataDoc;
		m_levelDataDoc = NULL;
		CCASSERT(false, "xml error");
		return;
	}
#endif

	
}


bool LevelDataManager::getLevelData(LevelData& data, int level) {
	if (NULL == m_levelDataDoc) {
		return false;
	}

	
	XMLElement *rootEle = m_levelDataDoc->RootElement();
	if (NULL == rootEle) {
		return false;
	}

	XMLElement *listEle = rootEle->FirstChildElement("level_list");
	if (NULL == listEle) {
		return false;
	}

	data.iLevel = level;
	level += 1;
	XMLElement *levelEle = listEle->FirstChildElement("level");
	while(NULL != levelEle) {
		const XMLAttribute *attribute = levelEle->FirstAttribute();
		if (NULL != attribute) {
			if (atoi(attribute->Value()) == level) {
				XMLElement *item = levelEle->FirstChildElement("onestar");
				if (NULL != item) {
					data.i1StarScore = atoi(item->GetText());
				} else {
					return false;
				}

				item = levelEle->FirstChildElement("twostar");
				if (NULL != item) {
					data.i2StarSec = atoi(item->GetText());
				} else {
					return false;
				}

				item = levelEle->FirstChildElement("threestar");
				if (NULL != item) {
					data.i3StarSec = atoi(item->GetText());
				} else {
					return false;
				}

				item = levelEle->FirstChildElement("number");
				if (NULL != item) {
					data.iTargetScore = atoi(item->GetText());
				} else {
					return false;
				}

				item = levelEle->FirstChildElement("time");
				if (NULL != item) {
					data.iTime = atoi(item->GetText());
				} else {
					return false;
				}

				item = levelEle->FirstChildElement("special");
				if (NULL != item) {
					data.iClear3x3 = atoi(item->GetText());
				} else {
					return false;
				}

				item = levelEle->FirstChildElement("arrmor");
				if (NULL != item) {
					data.iShade = atoi(item->GetText());
				} else {
					return false;
				}

				item = levelEle->FirstChildElement("akuma");
				if (NULL != item) {
					data.iFace = atoi(item->GetText());
				} else {
					return false;
				}

				item = levelEle->FirstChildElement("door");
				if (NULL != item) {
					data.iIce = atoi(item->GetText());
				} else {
					return false;
				}

				item = levelEle->FirstChildElement("color");
				if (NULL != item) {
					data.iChange = atoi(item->GetText());
				} else {
					return false;
				}

				item = levelEle->FirstChildElement("everyok");
				if (NULL != item) {
					data.iEveryok = atoi(item->GetText());
				} else {
					return false;
				}

				item = levelEle->FirstChildElement("boss");
				if (NULL != item) {
					data.iBoss = atoi(item->GetText());
				} else {
					return false;
				}

				return true;

			} else {
				levelEle = levelEle->NextSiblingElement("level");
				continue;
			}
		} else {
			break;
		} 
	}

	return false;
}