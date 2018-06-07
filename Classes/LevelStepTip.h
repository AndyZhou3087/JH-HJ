#ifndef __level_step_tip_controller_h__
#define __level_step_tip_controller_h__

#include "LevelStep.h"

class LevelStepTipDetail:public LevelStep
{
public:
	LevelStepTipDetail(LevelController * lev);
	virtual ~LevelStepTipDetail();

	virtual void init();
	inline virtual EnumLevelStep getStep() override { return Lev_Step_Tip_Detail; }

protected:
	virtual void onEnter();
	virtual void onExit();
};

//////////////////////////////////////////////////////////////////////////////////////////
class LevelStepTipFocus:public LevelStep
{
public:
	LevelStepTipFocus(LevelController * lev);
	virtual ~LevelStepTipFocus();

	virtual void init();
	inline virtual EnumLevelStep getStep() override { return Lev_Step_Tip_Focus; }

protected:
	virtual void onEnter();
	virtual void onExit();
};

//////////////////////////////////////////////////////////////////////////////////////////
class LevelStepTipGuide:public LevelStep
{
public:
	LevelStepTipGuide(LevelController * lev);
	virtual ~LevelStepTipGuide();

	virtual void init();
	inline virtual EnumLevelStep getStep() override { return Lev_Step_Tip_Guide; }

protected:
	virtual void onEnter();
	virtual void onExit();
};

//////////////////////////////////////////////////////////////////////////////////////////
class LevelStepTipManual:public LevelStep
{
public:
	LevelStepTipManual(LevelController * lev);
	virtual ~LevelStepTipManual();

	virtual void init();
	inline virtual EnumLevelStep getStep() override { return Lev_Step_Tip_Manual; }

protected:
	virtual void onEnter();
	virtual void onExit();
};

#endif