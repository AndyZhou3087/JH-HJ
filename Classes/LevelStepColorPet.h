#ifndef __level_step_colorpet_detail_controller_h__
#define __level_step_colorpet_detail_controller_h__

#include "LevelStep.h"


class LevelStepColorPetGuide:public LevelStep
{
public:
	LevelStepColorPetGuide(LevelController * lev);
	virtual ~LevelStepColorPetGuide();

	virtual void init();
	inline virtual EnumLevelStep getStep() override { return Lev_Step_ColorPet_Guide; }

protected:
	virtual void onEnter();
	virtual void onExit();
};

//////////////////////////////////////////////////////////////////////////////////////////
class LevelStepColorPetManual:public LevelStep
{
public:
	LevelStepColorPetManual(LevelController * lev);
	virtual ~LevelStepColorPetManual();

	virtual void init();
	inline virtual EnumLevelStep getStep() override { return Lev_Step_ColorPet_Manual; }

protected:
	virtual void onEnter();
	virtual void onExit();
};

#endif