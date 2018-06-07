#ifndef __level_step_toucherr_controller_h__
#define __level_step_toucherr_controller_h__

#include "LevelStep.h"

class LevelStepTouchErr:public LevelStep
{
public:
	LevelStepTouchErr(LevelController * lev);
	virtual ~LevelStepTouchErr();

	virtual void init();
	inline virtual EnumLevelStep getStep() override { return Lev_Step_TouchErr; }

protected:
	virtual void onEnter();
	virtual void onExit();
};

#endif