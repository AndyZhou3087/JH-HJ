#ifndef __level_time_tip_guide_controller_h__
#define __level_time_tip_guide_controller_h__

#include "LevelController.h"
#include "prefix.h"
#include "Pet.h"
#include "PetMatrix.h"

class DemoPetMatrix;
class LevelStepGiveTool;
class LevelStepTimeDetail;
class LevelStepTimeFocus;
class LevelStepTimeGuide;
class LevelStepTimeManual;
class LevelStepTipDetail;
class LevelStepTipFocus;
class LevelStepTipGuide;
class LevelStepTipManual;
class LevelStepWin;

class LevelTimeTipGuideController:public LevelController
{
public:
	LevelTimeTipGuideController(Node *lev);
	virtual ~LevelTimeTipGuideController();
	CREATE_LEVEL_CONTR_FUNC(LevelTimeTipGuideController);

	virtual bool init() override;
	virtual void enter() override;
	virtual void exit() override;
	virtual void onGameReady() override;
	virtual void onGameStart() override;
	virtual void onGameOver() override;

	virtual void showToolConfirm() override;

	virtual void showTimeDetail(bool visible) override;
	virtual void enterTimeFocus() override;
	virtual void enterTimeGuide() override;
	virtual void enterTimeManual() override;

	virtual void showTipDetail(bool visible) override;
	virtual void enterTipFocus() override;
	virtual void enterTipGuide() override;
	virtual void enterTipManual() override;

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

	void enterMaskTime();
	void enterMaskTip();
	void playArrowEffect();
	void stopArrowEffect();
	void onTouchTime();
	void onTouchTip();
	void nextGuide();

private:
	LevelStepGiveTool		*m_stepGiveTool;
	LevelStepTimeDetail		*m_stepTimeDetail;
	LevelStepTimeFocus		*m_stepTimeFocus;
	LevelStepTimeGuide		*m_stepTimeGuide;
	LevelStepTimeManual		*m_stepTimeManual;
	LevelStepTipDetail		*m_stepTipDetail;
	LevelStepTipFocus		*m_stepTipFocus;
	LevelStepTipGuide		*m_stepTipGuide;
	LevelStepTipManual		*m_stepTipManual;
	LevelStepWin			*m_stepWin;

	cocos2d::Sprite *m_mask;
	LevelData m_levelData;

	std::vector<PetVec2> m_maskPetVec;
	std::vector<PetVec2> m_clearablePetVec;
	std::vector<PetVec2> m_unclearablePetVec;
	PetVec2 m_touchPetVec2;
};

#endif