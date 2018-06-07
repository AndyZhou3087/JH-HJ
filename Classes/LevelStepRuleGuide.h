#ifndef __level_step_rule_guide_controller_h__
#define __level_step_rule_guide_controller_h__

#include "LevelStep.h"

class LevelStepRuleGuide:public LevelStep
{
public:
	LevelStepRuleGuide(LevelController * lev);
	virtual ~LevelStepRuleGuide();

	virtual void init();
	inline virtual EnumLevelStep getStep() override { return Lev_Step_RuleGuide; }

protected:
	virtual void onEnter();
	virtual void onExit();
};

#endif