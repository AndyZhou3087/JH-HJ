#ifndef __level_step_target_controller_h__
#define __level_step_target_controller_h__

#include "LevelStep.h"

class LevelStepTarget:public LevelStep
{
public:
	LevelStepTarget(LevelController * lev);
	virtual ~LevelStepTarget();

	virtual void init();
	inline virtual EnumLevelStep getStep() override { return Lev_Step_Target; }

protected:
	virtual void onEnter();
	virtual void onExit();
};

#endif