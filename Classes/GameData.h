#ifndef __game_data_h__
#define __game_data_h__

#include <cocos2d.h>
#include "global.h"
#include "LevelData.h"

class GameData
{
public:
	static GameData * getInstance(); 
	void init();
	void init(int level);
	inline int getMaxStars() { return MAX_LEVEL * 3; }

	inline int getScore() { return m_score; }
	inline void setScore(int s) { m_score = s; }
	void addScore(int s);

	inline int getLevel() { return m_level; }
	inline void setLevel(int l) { m_level = l; }

	inline int getHisLevel() { return m_hisLevel; }
	void setHisLevel(int level);
	
	inline int getEnergy() { return m_energy; }
	void addEnergy(int e);
	inline int getMaxEnergy() { return 25; }

	int getStar(int level);
	void setStar();
	void setStar(int level, int star);
	int getStarSum();
	inline int getStar() { return m_star; }
	inline int getMaxStarNum() { return MAX_LEVEL * 3; }

	bool getLotteryFlag(int level);
	void setLotteryFlag(bool lottery, int level);
	inline bool getLotteryFlag() { return m_lotteryed; }

	inline int getTimelimit() { return m_timeLimit; }

	inline void setTime(int t) { m_time = t; }
	inline int getTime() { return m_time; }
	inline void addTime(int t) { m_time += t; }
	inline void setElapseTime(int t) { m_elapseTime = t; }
	inline int getElapseTime() { return m_elapseTime; }
	inline void addElapseTime(int t) { m_elapseTime+=t; }
	
	inline int getTargetScore() { return m_targetScore; }
	inline int getBossType() { return m_boss; }
	int getBossType(int level);

	inline int getToolCrossNum() { return m_special_cross; }
	inline int getToolTimeNum() { return m_special_time; }
	inline int getToolTipNum() { return m_special_tip; }
	inline int getToolBombNum() { return m_special_bomb; }

	void addToolCross(int n);
	void addToolTime(int n);
	void addToolTip(int n);
	void addToolBomb(int n);


	int getTargetStar(int level);
	bool isLevelUnlock(int level);
	void setLevelUnlock(int level, bool unLocked);

	static const int MAX_LEVEL	= 124;
	static const int MAX_LOCK	= 5;
	void clearData();

private:
	GameData();
	~GameData();
	
	void loadStarsData();
	void saveStarsData();

	void loadLotteryData();
	void saveLotteryData();

	void loadEnergyData();
	void saveEnergyData();

	void loadToolData();

	void loadLevelData();
	void saveLevelData();

	void loadLevelLockData();
	void saveLevelLockData();

	void loadBossData();

private:
	int m_score;		//��ǰ����
	int m_targetScore;	//Ŀ�����
	int m_level;		//��ǰ�ؿ�
	int m_timeLimit;	//ʱ��
	int m_time;			//��ǰʱ��
	int m_star;			//��ǰ�ؿ�����
	int m_boss;			//boss��
	bool m_lotteryed;	//��ǰ�ؿ��Ƿ�����
	int m_elapseTime;

	int m_special_cross;	//ʮ�ֵ���
	int m_special_time;		//ʱ�����
	int m_special_tip;		//���ѵ���
	int m_special_bomb;		//ը������


	LevelData m_levelData;

	int m_energy; //����ֵ
	int m_hisLevel;	//��ʷ�ؿ���
	unsigned char m_stars[MAX_LEVEL]; //�Ǽ�����
	unsigned char m_lotteryFlag[MAX_LEVEL];//�齱��־��ÿ��ֻ�ܳ�һ��
	unsigned char m_bossType[MAX_LEVEL];//BOSS��ʶ
	unsigned char m_levelLockFlag[MAX_LOCK];
	cocos2d::Data m_starsData;
	cocos2d::Data m_lotteryData;
	cocos2d::Data m_levelLockData;
};

#endif