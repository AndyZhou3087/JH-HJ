#ifndef __level_step_welcome_controller_h__
#define __level_step_welcome_controller_h__

#include "LevelStep.h"

class LevelStepWelcome:public LevelStep
{
public:
	LevelStepWelcome(LevelController * lev);
	virtual ~LevelStepWelcome();

	virtual void init();
	inline virtual EnumLevelStep getStep() override { return Lev_Step_Welcome; }

protected:
	virtual void onEnter();
	virtual void onExit();
};

class LevelStepWelcomeEnd:public LevelStep
{
public:
	LevelStepWelcomeEnd(LevelController * lev);
	virtual ~LevelStepWelcomeEnd();

	virtual void init();
	inline virtual EnumLevelStep getStep() override { return Lev_Step_WelcomeEnd; }

protected:
	virtual void onEnter();
	virtual void onExit();
};




#endif