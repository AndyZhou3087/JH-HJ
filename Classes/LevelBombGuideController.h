#ifndef __level_bomb_guide_controller_h__
#define __level_bomb_guide_controller_h__

#include "LevelController.h"
#include "prefix.h"
#include "Pet.h"
#include "PetMatrix.h"

class DemoPetMatrix;
class LevelStepBombDetail;
class LevelStepBombFocus;
class LevelStepBombManual;
class LevelStepWin;

class LevelBombGuideController:public LevelController
{
public:
	LevelBombGuideController(Node *lev);
	virtual ~LevelBombGuideController();
	CREATE_LEVEL_CONTR_FUNC(LevelBombGuideController);

	virtual bool init() override;
	virtual void enter() override;
	virtual void exit() override;
	virtual void onGameReady() override;
	virtual void onGameStart() override;
	virtual void onGameOver() override;

	virtual void showBombDetail(bool visible) override;
	virtual void enterBombFocus() override;
	virtual void enterBombManual() override;

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

	void enterMaskBomb();
	void playArrowEffect();
	void stopArrowEffect();
	void onTouchBomb();
	void nextGuide();

private:
	LevelStepBombDetail		*m_stepBombDetail;
	LevelStepBombFocus		*m_stepBombFocus;
	LevelStepBombManual		*m_stepBombManual;
	LevelStepWin			*m_stepWin;

	cocos2d::Sprite *m_mask;
	LevelData m_levelData;

	std::vector<PetVec2> m_maskPetVec;
	std::vector<PetVec2> m_clearablePetVec;
	std::vector<PetVec2> m_unclearablePetVec;
	PetVec2 m_touchPetVec2;
};

#endif