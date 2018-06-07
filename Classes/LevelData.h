#ifndef __level_data_h__
#define __level_data_h__

#include "cocos2d.h"
#include "tinyxml2/tinyxml2.h"
using namespace tinyxml2;

class LevelData {
public:
	LevelData();
	~LevelData();

	int iLevel;			//�ؿ�ID
	int iTargetScore;	//Ŀ�����
	int i1StarScore;	//1�Ƿ���
	int i2StarSec;		//2��ʣ������
	int i3StarSec;		//3��ʣ������
	int iTime;			//����
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