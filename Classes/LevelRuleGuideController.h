#ifndef __level_rule_guide_controller_h__
#define __level_rule_guide_controller_h__

#include "LevelController.h"

class DemoPetMatrix;
class LevelStepWelcome;
class LevelStepRuleGuide;
class LevelStepWin;
class LevelStepWelcomeEnd;

class LevelRuleGuideController:public LevelController
{
public:
	LevelRuleGuideController(Node *lev);
	virtual ~LevelRuleGuideController();
	CREATE_LEVEL_CONTR_FUNC(LevelRuleGuideController);

	virtual bool init() override;
	virtual void enter() override;
	virtual void exit() override;
	virtual void onGameReady() override;
	virtual void onGameStart() override;
	virtual void onGameOver() override;

	virtual void showWelcome(bool visible) override;
	virtual void showRule(bool visible) override;
	virtual void showWelcomeEnd() override;

	virtual bool onTouchBegan(Touch * touch,Event *e);
	virtual void onTouch(const Point&pos);
	virtual void onTipWinCompleted();

	virtual void guide();
	virtual void nextGuide();

	void initial();
	void initTouchLayer();

	void clearGuide();

protected:
	virtual void loadPetMatrix() override;

private:
	void createSteps();
	void clearSteps();
	
	void enterMask(const cocos2d::Point&pos);
	void enterMask(std::vector<PetVec2>&mask);

private:
	LevelStepWelcome			*m_stepWelcome;
	LevelStepRuleGuide			*m_stepRuleGuide;
	LevelStepWin				*m_stepWin;
	LevelStepWelcomeEnd			*m_stepWelcomeEnd;

	cocos2d::Sprite *m_mask;

	std::vector<PetVec2> m_maskPetVec;
	std::vector<PetVec2> m_clearablePetVec;
	std::vector<PetVec2> m_unclearablePetVec;
	PetVec2 m_touchPetVec2;
};

#endif