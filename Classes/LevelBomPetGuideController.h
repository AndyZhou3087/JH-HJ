#ifndef __level_bompet_guide_controller_h__
#define __level_bompet_guide_controller_h__

#include "LevelController.h"

class DemoPetMatrix;
class LevelStepBomPetGuide;
class LevelStepBomPetManual;
class LevelStepWin;

class LevelBomPetGuideController:public LevelController
{
public:
	LevelBomPetGuideController(Node *lev);
	virtual ~LevelBomPetGuideController();
	CREATE_LEVEL_CONTR_FUNC(LevelBomPetGuideController);

	virtual bool init() override;
	virtual void enter() override;
	virtual void exit() override;
	virtual void onGameReady() override;
	virtual void onGameStart() override;
	virtual void onGameOver() override;

	virtual bool onTouchBegan(Touch * touch,Event *e);
	virtual void onTouch(const Point&pos);
	virtual void onTipWinCompleted();

	virtual void enterBomPetGuide() override;
	virtual void enterBomPetManual() override;

	virtual void guide();
	virtual void nextGuide();

	virtual bool isManualStep() override;

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
	
	void showBomPetDetail(bool visible);

private:
	LevelStepBomPetGuide		*m_stepBomPetGuide;
	LevelStepBomPetManual		*m_stepBomPetManual;
	LevelStepWin				*m_stepWin;

	cocos2d::Sprite *m_mask;
	cocos2d::Sprite *m_redTip;


	std::vector<PetVec2> m_maskPetVec;
	std::vector<PetVec2> m_clearablePetVec;
	std::vector<PetVec2> m_unclearablePetVec;
	PetVec2 m_touchPetVec2;
};

#endif