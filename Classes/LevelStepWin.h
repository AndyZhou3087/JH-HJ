#ifndef __level_step_win_controller_h__
#define __level_step_win_controller_h__

#include "LevelStep.h"

class LevelStepWin:public LevelStep
{
public:
	LevelStepWin(LevelController * lev);
	virtual ~LevelStepWin();

	virtual void init();
	inline virtual EnumLevelStep getStep() override { return Lev_Step_Win; }

protected:
	virtual void onEnter();
	virtual void onExit();
};


#endif