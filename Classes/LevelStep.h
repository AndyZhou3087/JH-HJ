#ifndef __level_step_controller_h__
#define __level_step_controller_h__

#include "cocos2d.h"
#include "LevelController.h"

USING_NS_CC;

enum EnumLevelStep {
	Lev_Step_ToolConfirm,
	Lev_Step_Target,
	Lev_Step_Manual,
	Lev_Step_Win,
	Lev_Step_TouchErr,

	Lev_Step_Welcome,
	Lev_Step_RuleGuide,
	Lev_Step_WelcomeEnd,

	Lev_Step_Cross_Detail,
	Lev_Step_Cross_Focus,
	Lev_Step_Cross_Guide,
	Lev_Step_Cross_Manual,

	Lev_Step_Bomb_Detail,
	Lev_Step_Bomb_Focus,
	Lev_Step_Bomb_Guide,
	Lev_Step_Bomb_Manual,

	Lev_Step_GiveTool,
	Lev_Step_Time_Detail,
	Lev_Step_Time_Focus,
	Lev_Step_Time_Guide,
	Lev_Step_Time_Manual,

	Lev_Step_Tip_Detail,
	Lev_Step_Tip_Focus,
	Lev_Step_Tip_Guide,
	Lev_Step_Tip_Manual,

	Lev_Step_BomPet_Guide,
	Lev_Step_BomPet_Manual,

	Lev_Step_ColorPet_Guide,
	Lev_Step_ColorPet_Manual,

	Lev_Step_Boss_Enter,
	Lev_Step_Boss_Talk,
	Lev_Step_Girl_Talk,
	Lev_Step_Boss_Tip,
	Lev_Step_Boss_Manual,
};

class LevelStep
{
public:
	LevelStep(LevelController *);
	virtual ~LevelStep();

	virtual void init();
	void enter();
	void exit();
	void finish();
	void setNextStep(LevelStep * next) { m_next = next; }
	inline bool isEnter() { return m_bEnter; }
	virtual void onTouch() {}
	virtual EnumLevelStep getStep() = 0;
	inline LevelStep *getNextStep() { return m_next; }

protected:
	virtual void onEnter() = 0;
	virtual void onExit() = 0;
	void showBgGrey(bool visible);
	void setBgGreyTouchable(bool enable);

protected:
	bool m_bEnter;
	LevelStep *m_next;
	LevelController *m_levelController;
};

#endif