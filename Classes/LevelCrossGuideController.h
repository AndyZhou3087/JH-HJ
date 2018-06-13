#ifndef __level_cross_guide_controller_h__
#define __level_cross_guide_controller_h__

#include "LevelController.h"
#include "prefix.h"
#include "Pet.h"
#include "PetMatrix.h"

class DemoPetMatrix;
class LevelStepCrossDetail;
class LevelStepCrossFocus;
class LevelStepCrossGuide;
class LevelStepCrossManual;
class LevelStepWin;

class LevelCrossGuideController:public LevelController
{
public:
	LevelCrossGuideController(Node *lev);
	virtual ~LevelCrossGuideController();
	CREATE_LEVEL_CONTR_FUNC(LevelCrossGuideController);

	virtual bool init() override;
	virtual void enter() override;
	virtual void exit() override;
	virtual void onGameReady() override;
	virtual void onGameStart() override;
	virtual void onGameOver() override;

	virtual void showCrossDetail(bool visible) override;
	virtual void enterCrossFocus() override;
	virtual void enterCrossGuide() override;
	virtual void enterCrossManual() override;

	virtual bool onTouchBegan(Touch * touch,Event *e);
	virtual void onTouch(const Point&pos);

	virtual bool isManualStep() override;

	void initial();
	void initTouchLayer();

	void clearMask();
	void clearGuide();

protected:
	virtual void loadPetMatrix() override;

private:
	void createSteps();
	void clearSteps();
 
	void enterMask(const cocos2d::Point&pos);
	void enterMask(std::vector<PetVec2>&mask);	

	void enterMaskCross();
	void playArrowEffect();
	void stopArrowEffect();
	void onTouchCross();
	void nextGuide();

private:
	LevelStepCrossDetail	*m_stepCrossDetail;
	LevelStepCrossFocus		*m_stepCrossFocus;
	LevelStepCrossGuide		*m_stepCrossGuide;
	LevelStepCrossManual	*m_stepCrossManual;
	LevelStepWin			*m_stepWin;

	cocos2d::Sprite *m_mask;

	std::vector<PetVec2> m_maskPetVec;
	std::vector<PetVec2> m_clearablePetVec;
	std::vector<PetVec2> m_unclearablePetVec;
	PetVec2 m_touchPetVec2;
};

#endif