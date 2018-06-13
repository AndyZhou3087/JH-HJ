#ifndef __level_step_bompet_detail_controller_h__
#define __level_step_bompet_detail_controller_h__

#include "LevelStep.h"


class LevelStepBomPetGuide:public LevelStep
{
public:
	LevelStepBomPetGuide(LevelController * lev);
	virtual ~LevelStepBomPetGuide();

	virtual void init();
	inline virtual EnumLevelStep getStep() override { return Lev_Step_BomPet_Guide; }

protected:
	virtual void onEnter();
	virtual void onExit();
};

//////////////////////////////////////////////////////////////////////////////////////////
class LevelStepBomPetManual:public LevelStep
{
public:
	LevelStepBomPetManual(LevelController * lev);
	virtual ~LevelStepBomPetManual();

	virtual void init();
	inline virtual EnumLevelStep getStep() override { return Lev_Step_BomPet_Manual; }

protected:
	virtual void onEnter();
	virtual void onExit();
};

#endif