#ifndef __level_step_boss_detail_controller_h__
#define __level_step_boss_detail_controller_h__

#include "LevelStep.h"

class LevelStepBossEnter:public LevelStep
{
public:
	LevelStepBossEnter(LevelController * lev);
	virtual ~LevelStepBossEnter();

	virtual void init();
	inline virtual EnumLevelStep getStep() override { return Lev_Step_Boss_Enter; }

protected:
	virtual void onEnter();
	virtual void onExit();
};

//////////////////////////////////////////////////////////////////////////////////////////
class LevelStepBossTalk:public LevelStep
{
public:
	LevelStepBossTalk(LevelController * lev);
	virtual ~LevelStepBossTalk();

	virtual void init();
	inline virtual EnumLevelStep getStep() override { return Lev_Step_Boss_Talk; }

protected:
	virtual void onEnter();
	virtual void onExit();
};
//////////////////////////////////////////////////////////////////////////////////////////
class LevelStepGirlTalk:public LevelStep
{
public:
	LevelStepGirlTalk(LevelController * lev);
	virtual ~LevelStepGirlTalk();

	virtual void init();
	inline virtual EnumLevelStep getStep() override { return Lev_Step_Girl_Talk; }

protected:
	virtual void onEnter();
	virtual void onExit();
};
//////////////////////////////////////////////////////////////////////////////////////////
class LevelStepBossTip:public LevelStep
{
public:
	LevelStepBossTip(LevelController * lev);
	virtual ~LevelStepBossTip();

	virtual void init();
	inline virtual EnumLevelStep getStep() override { return Lev_Step_Boss_Tip; }

protected:
	virtual void onEnter();
	virtual void onExit();
};
//////////////////////////////////////////////////////////////////////////////////////////
class LevelStepBossManual:public LevelStep
{
public:
	LevelStepBossManual(LevelController * lev);
	virtual ~LevelStepBossManual();

	virtual void init();
	inline virtual EnumLevelStep getStep() override { return Lev_Step_Boss_Manual; }

protected:
	virtual void onEnter();
	virtual void onExit();
};

#endif