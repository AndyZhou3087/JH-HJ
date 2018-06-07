#ifndef __level_step_bomb_detail_controller_h__
#define __level_step_bomb_detail_controller_h__

#include "LevelStep.h"

class LevelStepBombDetail:public LevelStep
{
public:
	LevelStepBombDetail(LevelController * lev);
	virtual ~LevelStepBombDetail();

	virtual void init();
	inline virtual EnumLevelStep getStep() override { return Lev_Step_Bomb_Detail; }

protected:
	virtual void onEnter();
	virtual void onExit();
};

//////////////////////////////////////////////////////////////////////////////////////////
class LevelStepBombFocus:public LevelStep
{
public:
	LevelStepBombFocus(LevelController * lev);
	virtual ~LevelStepBombFocus();

	virtual void init();
	inline virtual EnumLevelStep getStep() override { return Lev_Step_Bomb_Focus; }

protected:
	virtual void onEnter();
	virtual void onExit();
};
//////////////////////////////////////////////////////////////////////////////////////////
class LevelStepBombManual:public LevelStep
{
public:
	LevelStepBombManual(LevelController * lev);
	virtual ~LevelStepBombManual();

	virtual void init();
	inline virtual EnumLevelStep getStep() override { return Lev_Step_Bomb_Manual; }

protected:
	virtual void onEnter();
	virtual void onExit();
};

#endif