#ifndef __level_step_give_tool_controller_h__
#define __level_step_give_tool_controller_h__

#include "LevelStep.h"

class LevelStepGiveTool:public LevelStep
{
public:
	LevelStepGiveTool(LevelController * lev);
	virtual ~LevelStepGiveTool();

	virtual void init();
	inline virtual EnumLevelStep getStep() override { return Lev_Step_GiveTool; }

protected:
	virtual void onEnter();
	virtual void onExit();
};


#endif