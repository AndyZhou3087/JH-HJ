#ifndef __level_step_manual_controller_h__
#define __level_step_manual_controller_h__

#include "LevelStep.h"

class LevelStepToolConfirm:public LevelStep
{
public:
	LevelStepToolConfirm(LevelController * lev);
	virtual ~LevelStepToolConfirm();

	virtual void init();
	inline virtual EnumLevelStep getStep() override { return Lev_Step_ToolConfirm; }

protected:
	virtual void onEnter();
	virtual void onExit();
};
/////////////////////////////////////////////////////////////////////////////////////
class LevelStepManual:public LevelStep
{
public:
	LevelStepManual(LevelController * lev);
	virtual ~LevelStepManual();

	virtual void init();
	inline virtual EnumLevelStep getStep() override { return Lev_Step_Manual; }

protected:
	virtual void onEnter();
	virtual void onExit();
};



#endif