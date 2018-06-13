#ifndef __level_step_time_controller_h__
#define __level_step_time_controller_h__

#include "LevelStep.h"

class LevelStepTimeDetail:public LevelStep
{
public:
	LevelStepTimeDetail(LevelController * lev);
	virtual ~LevelStepTimeDetail();

	virtual void init();
	inline virtual EnumLevelStep getStep() override { return Lev_Step_Time_Detail; }

protected:
	virtual void onEnter();
	virtual void onExit();
};

//////////////////////////////////////////////////////////////////////////////////////////
class LevelStepTimeFocus:public LevelStep
{
public:
	LevelStepTimeFocus(LevelController * lev);
	virtual ~LevelStepTimeFocus();

	virtual void init();
	inline virtual EnumLevelStep getStep() override { return Lev_Step_Time_Focus; }

protected:
	virtual void onEnter();
	virtual void onExit();
};

//////////////////////////////////////////////////////////////////////////////////////////
class LevelStepTimeGuide:public LevelStep
{
public:
	LevelStepTimeGuide(LevelController * lev);
	virtual ~LevelStepTimeGuide();

	virtual void init();
	inline virtual EnumLevelStep getStep() override { return Lev_Step_Time_Guide; }

protected:
	virtual void onEnter();
	virtual void onExit();
};

//////////////////////////////////////////////////////////////////////////////////////////
class LevelStepTimeManual:public LevelStep
{
public:
	LevelStepTimeManual(LevelController * lev);
	virtual ~LevelStepTimeManual();

	virtual void init();
	inline virtual EnumLevelStep getStep() override { return Lev_Step_Time_Manual; }

protected:
	virtual void onEnter();
	virtual void onExit();
};

#endif