#ifndef __level_step_cross_detail_controller_h__
#define __level_step_cross_detail_controller_h__

#include "LevelStep.h"

class LevelStepCrossDetail:public LevelStep
{
public:
	LevelStepCrossDetail(LevelController * lev);
	virtual ~LevelStepCrossDetail();

	virtual void init();
	inline virtual EnumLevelStep getStep() override { return Lev_Step_Cross_Detail; }

protected:
	virtual void onEnter();
	virtual void onExit();
};

//////////////////////////////////////////////////////////////////////////////////////////
class LevelStepCrossFocus:public LevelStep
{
public:
	LevelStepCrossFocus(LevelController * lev);
	virtual ~LevelStepCrossFocus();

	virtual void init();
	inline virtual EnumLevelStep getStep() override { return Lev_Step_Cross_Focus; }

protected:
	virtual void onEnter();
	virtual void onExit();
};

//////////////////////////////////////////////////////////////////////////////////////////
class LevelStepCrossGuide:public LevelStep
{
public:
	LevelStepCrossGuide(LevelController * lev);
	virtual ~LevelStepCrossGuide();

	virtual void init();
	inline virtual EnumLevelStep getStep() override { return Lev_Step_Cross_Guide; }

protected:
	virtual void onEnter();
	virtual void onExit();
};

//////////////////////////////////////////////////////////////////////////////////////////
class LevelStepCrossManual:public LevelStep
{
public:
	LevelStepCrossManual(LevelController * lev);
	virtual ~LevelStepCrossManual();

	virtual void init();
	inline virtual EnumLevelStep getStep() override { return Lev_Step_Cross_Manual; }

protected:
	virtual void onEnter();
	virtual void onExit();
};

#endif