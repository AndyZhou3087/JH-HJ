#ifndef __level_boss_guide_controller_h__
#define __level_boss_guide_controller_h__

#include "LevelController.h"
#include "prefix.h"
#include "Pet.h"
#include "PetMatrix.h"

class DemoPetMatrix;
class LevelStepBossEnter;
class LevelStepBossTalk;
class LevelStepGirlTalk;
class LevelStepBossTip;
class LevelStepBossManual;
class LevelStepWin;

class LevelBossGuideController:public LevelController
{
public:
	LevelBossGuideController(Node *lev);
	virtual ~LevelBossGuideController();
	CREATE_LEVEL_CONTR_FUNC(LevelBossGuideController);

	virtual bool init() override;
	virtual void enter() override;
	virtual void exit() override;
	virtual void onGameReady() override;
	virtual void onGameStart() override;
	virtual void onGameOver() override;

	virtual void showBoss() override;
	virtual void showBossTalk(bool visible) override;
	virtual void showGirlTalk(bool visible) override;
	virtual void showBossTip(bool visible) override;
	virtual void enterBossManual() override;

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
 
	void nextGuide();

private:
	LevelStepBossEnter		*m_stepBossEnter;
	LevelStepBossTalk		*m_stepBossTalk;
	LevelStepGirlTalk		*m_stepGirlTalk;
	LevelStepBossTip		*m_stepBossTip;
	LevelStepBossManual		*m_stepBossManual;
	LevelStepWin			*m_stepWin;
};

#endif