#ifndef __level_controller_h__
#define __level_controller_h__

#include "cocos2d.h"
#include "Pet.h"
#include "PetMatrix.h"

USING_NS_CC;

#define CREATE_LEVEL_CONTR_FUNC(__TYPE__) \
	static __TYPE__* create(Node* layer) \
{ \
	__TYPE__ *pRet = new __TYPE__(layer); \
	if (pRet && pRet->init()) \
{ \
	pRet->autorelease(); \
	return pRet; \
} \
	else \
{ \
	delete pRet; \
	pRet = NULL; \
	return NULL; \
} \
}

class DemoPetMatrix;
class UToolConfirmLayerController;
class GameTargetLayerController;
class UTipWinLayerController;
class LevelStep;
class LevelStepTouchErr;

class LevelController:public cocos2d::Ref
{
public:
	LevelController(Node *layer);
	virtual ~LevelController();

	virtual bool init();
	virtual void enter();
	virtual void exit();
	virtual void onGameReady();
	virtual void onGameStart()	= 0;
	virtual void onGameOver()	= 0;
	virtual void onGameFinish();

	virtual void showToolConfirm();
	virtual void showTarget();
	virtual void showTargetBoss();
	virtual void enterManual();
	virtual void showWinTip();
	virtual void enterTouchErr();

	virtual void showWelcome(bool visible) {}
	virtual void showRule(bool visible) {}
	virtual void showWelcomeEnd() {}

	virtual void showCrossDetail(bool visible)	{}
	virtual void enterCrossFocus() {}
	virtual void enterCrossGuide() {}
	virtual void enterCrossManual() {}

	virtual void showBombDetail(bool visible)	{}
	virtual void enterBombFocus() {}
	virtual void enterBombManual() {}

	virtual void showTimeDetail(bool visible)	{}
	virtual void enterTimeFocus() {}
	virtual void enterTimeGuide() {}
	virtual void enterTimeManual() {}

	virtual void showTipDetail(bool visible)	{}
	virtual void enterTipFocus() {}
	virtual void enterTipGuide() {}
	virtual void enterTipManual() {}

	virtual void enterBomPetGuide() {}
	virtual void enterBomPetManual() {}

	virtual void enterColorPetGuide() {}
	virtual void enterColorPetManual() {}

	virtual void showBoss() {}
	virtual void showBossTalk(bool visible) {}
	virtual void showGirlTalk(bool visible) {}
	virtual void showBossTip(bool visible) {}
	virtual void enterBossManual() {}

	virtual bool onTouchBegan(Touch * touch,Event *e);
	virtual void onTouch(const Point&pos) {}
	virtual void onTipWinCompleted();
	virtual void guide() {}
	virtual bool isManualStep() { return false; }

	virtual bool usingBomb();
	virtual bool usingTip();
	virtual bool usingCross()	{ return false; }
	virtual bool usingTime()	{ return false; }

	virtual bool isBossLevel();

	void enterPetMatrix();
	void finish();
	void tipPets();
	void blinkRedTip();

	void startTime();
	void stopTime();

	void enterTouchErrStep();

protected:
	void initTouchLayer();
	void enterNextStep();
	void checkPetMatrix();

	cocos2d::Sprite*  createMaskedSprite(std::vector<PetVec2>&mask);
	cocos2d::Sprite*  createMaskedSprite(const char* maskFile, const cocos2d::Point&maskPos);
	bool isTouchFocus(const cocos2d::Point&pos, PetVec2 v);

	virtual void loadPetMatrix() = 0;
	void loadPetMatrix(int level);
	void loadPetMatrix(const LevelData&data);
	void unloadPetMatrix();

	void createBombArrowTip();
	void createCrossArrowTip();
	void hideArrowTip();
	void clearArrowTip();

protected:
	cocos2d::Node *m_layer;
	DemoPetMatrix *m_petMatrix;
	LevelStep *m_currentStep;
	cocos2d::Sprite *m_redTip;
	cocos2d::Sprite *m_arrow;
	cocos2d::Sprite *m_tipLabel;

	LevelStepTouchErr *m_stepTouchErr;

	LevelData m_levelData;
	bool m_bClearAlling;

	cocos2d::EventListenerTouchOneByOne *m_eventLis;

	
};

#endif