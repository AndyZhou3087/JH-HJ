#ifndef __level_colorpet_guide_controller_h__
#define __level_colorpet_guide_controller_h__

#include "LevelController.h"

class DemoPetMatrix;
class LevelStepColorPetGuide;
class LevelStepColorPetManual;
class LevelStepWin;

class LevelColorPetGuideController:public LevelController
{
public:
	LevelColorPetGuideController(Node *lev);
	virtual ~LevelColorPetGuideController();
	CREATE_LEVEL_CONTR_FUNC(LevelColorPetGuideController);

	virtual bool init() override;
	virtual void enter() override;
	virtual void exit() override;
	virtual void onGameReady() override;
	virtual void onGameStart() override;
	virtual void onGameOver() override;

	virtual bool onTouchBegan(Touch * touch,Event *e);
	virtual void onTouch(const Point&pos);
	virtual void onTipWinCompleted();

	virtual void enterColorPetGuide() override;
	virtual void enterColorPetManual() override;

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
	
	void showColorPetDetail(bool visible);

private:
	LevelStepColorPetGuide		*m_stepColorPetGuide;
	LevelStepColorPetManual		*m_stepColorPetManual;
	LevelStepWin				*m_stepWin;

	cocos2d::Sprite *m_mask;
	cocos2d::Sprite *m_redTip;


	std::vector<PetVec2> m_maskPetVec;
	std::vector<PetVec2> m_clearablePetVec;
	std::vector<PetVec2> m_unclearablePetVec;
	PetVec2 m_touchPetVec2;
};

#endif