#ifndef __level_data_h__
#define __level_data_h__

#include "cocos2d.h"
#include "tinyxml2/tinyxml2.h"
using namespace tinyxml2;

class LevelData {
public:
	LevelData();
	~LevelData();

	int iLevel;			//关卡ID
	int iTargetScore;	//目标分数
	int i1StarScore;	//1星分数
	int i2StarSec;		//2星剩余秒数
	int i3StarSec;		//3星剩余秒数
	int iTime;			//秒数
	int iClear3x3;
	int iShade;
	int iFace;
	int iIce;
	int iChange;
	int iEveryok;
	int iBoss;

};

class LevelDataManager {
public:
	static LevelDataManager * getInstance(); 
	bool getLevelData(LevelData& data, int level);

private:
	LevelDataManager();
	~LevelDataManager();

	static LevelDataManager * s_levelDataManager;
	void init();

private:
	tinyxml2::XMLDocument *m_levelDataDoc;
};

#endif