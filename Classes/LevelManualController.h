#ifndef __level_manual_controller_h__
#define __level_manual_controller_h__

#include "LevelController.h"
#include "prefix.h"
#include "Pet.h"
#include "PetMatrix.h"

class DemoPetMatrix;
class LevelStepToolConfirm;
class LevelStepTarget;
class LevelStepManual;
class LevelStepWin;

class LevelManualController:public LevelController
{
public:
	LevelManualController(Node *lev);
	virtual ~LevelManualController();
	CREATE_LEVEL_CONTR_FUNC(LevelManualController);

	virtual bool init() override;
	virtual void enter() override;
	virtual void exit() override;
	virtual void onGameReady() override;
	virtual void onGameStart() override;
	virtual void onGameOver() override;

	virtual void showToolConfirm() override;
	virtual void enterManual() override;

	virtual bool onTouchBegan(Touch * touch,Event *e);
	virtual void onTouch(const Point&pos);

	virtual bool isManualStep() override;

	void initial();
	void initTouchLayer();

protected:
	virtual void loadPetMatrix() override;

private:
	void createSteps();
	void clearSteps();

private:
	LevelStepToolConfirm	*m_stepToolConfirm;
	LevelStepTarget			*m_stepTarget;
	LevelStepManual			*m_stepManual;
	LevelStepWin			*m_stepWin;

	LevelData m_levelData;
};

#endif