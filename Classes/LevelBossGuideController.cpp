#include "LevelBossGuideController.h"
#include "GameUiManager.h"
#include "GameManager.h"
#include "GameData.h"
#include "Resource.h"
#include "DemoPetData.h"
#include "DemoPetMatrix.h"
#include "UserData.h"
#include "GameEvent.h"
#include "Analytics.h"
#include "prefix.h"
#include "GameTopLayerController.h"
#include "GameBossLayerController.h"

#include "LevelStepBoss.h"
#include "LevelStepWin.h"
#include "LevelStepTouchErr.h"

#define DEMO_Z_ORDER -10
#define TOUCH_Z_ORDER	12
#define MASK_Z_ORDER	10
#define TIP_Z_ORDER		11

LevelBossGuideController::LevelBossGuideController(Node *layer):
m_stepBossEnter(nullptr),
m_stepBossTalk(nullptr),
m_stepGirlTalk(nullptr),
m_stepBossTip(nullptr),
m_stepBossManual(nullptr),
m_stepWin(nullptr),
LevelController(layer)
{

}

LevelBossGuideController::~LevelBossGuideController() {
	clearSteps();
}

bool LevelBossGuideController::init() {
	if (!LevelController::init()) {
		return false;
	}

	initial();
	createSteps();
	return true;
}

void LevelBossGuideController::enter() {
	UMengAnalytics::onBegin(ROOKIE_LEVEL_BOSS);
}

void LevelBossGuideController::exit() {
	unloadPetMatrix();
}

void LevelBossGuideController::initial() {
	if (nullptr == m_layer) {
		return;
	}
}

void LevelBossGuideController::showBoss() {
	//
}

void LevelBossGuideController::showBossTalk(bool visible) {
	auto bossTalk = GameUiManager::getInstance()->getBossTalkLayer();
	if (nullptr != bossTalk) {
		GameUiManager::getInstance()->showLayer(bossTalk, visible);
		if (visible) {
			stopTime();
		} else {
			startTime();
		}
	}
}

void LevelBossGuideController::showGirlTalk(bool visible) {
	auto girlTalk = GameUiManager::getInstance()->getGirlTalkLayer();
	if (nullptr != girlTalk) {
		GameUiManager::getInstance()->showLayer(girlTalk, visible);
		if (visible) {
			stopTime();
		} else {
			startTime();
		}
	}
}

void LevelBossGuideController::showBossTip(bool visible) {
	auto bossTip = GameUiManager::getInstance()->getBossTipLayer();
	if (nullptr != bossTip) {
		GameUiManager::getInstance()->showLayer(bossTip, visible);
		if (visible) {
			stopTime();
		} else {
			startTime();
		}
	}
}

void LevelBossGuideController::enterBossManual() {
	startTime();
}

void LevelBossGuideController::createSteps() {
	clearSteps();

	m_stepBossEnter		= new LevelStepBossEnter(this);
	m_stepBossTalk		= new LevelStepBossTalk(this);
	m_stepGirlTalk		= new LevelStepGirlTalk(this);
	m_stepBossTip		= new LevelStepBossTip(this);
	m_stepBossManual	= new LevelStepBossManual(this);
	m_stepWin			= new LevelStepWin(this);
	m_stepTouchErr		= new LevelStepTouchErr(this);
	m_currentStep		= m_stepBossEnter;

	m_stepBossEnter->setNextStep(m_stepBossTalk);
	m_stepBossTalk->setNextStep(m_stepGirlTalk);
	m_stepGirlTalk->setNextStep(m_stepBossTip);
	m_stepBossTip->setNextStep(m_stepBossManual);
	m_stepBossManual->setNextStep(m_stepWin);
	m_stepTouchErr->setNextStep(m_stepBossManual);
	m_stepWin->setNextStep(nullptr);
}

void LevelBossGuideController::clearSteps() {
	if (nullptr != m_stepBossEnter) {
		delete m_stepBossEnter;
		m_stepBossEnter = nullptr;
	}

	if (nullptr != m_stepBossTalk) {
		delete m_stepBossTalk;
		m_stepBossTalk = nullptr;
	}

	if (nullptr != m_stepGirlTalk) {
		delete m_stepGirlTalk;
		m_stepGirlTalk = nullptr;
	}

	if (nullptr != m_stepBossTip) {
		delete m_stepBossTip;
		m_stepBossTip = nullptr;
	}

	if (nullptr != m_stepBossManual) {
		delete m_stepBossManual;
		m_stepBossManual = nullptr;
	}

	if (nullptr != m_stepTouchErr) {
		delete m_stepTouchErr;
		m_stepTouchErr = nullptr;
	}

	if (nullptr != m_stepWin) {
		delete m_stepWin;
		m_stepWin = nullptr;
	}

	m_currentStep = nullptr;
}

bool LevelBossGuideController::onTouchBegan(Touch * touch,Event *e) {
	if (nullptr == m_petMatrix || nullptr == touch) {
		return true;
	}

	if (nullptr == m_currentStep) {
		return true;
	}

	switch (m_currentStep->getStep()) {
	case Lev_Step_Boss_Talk:
	case Lev_Step_Girl_Talk:
	case Lev_Step_Boss_Tip:
		enterNextStep();
		return true;
	default:
		break;
	}
	if (m_currentStep->getStep() == Lev_Step_TouchErr) {
		return true;
	}

	if (m_currentStep->getStep() == Lev_Step_Boss_Manual) {
		return false;
	}

	nextGuide();
	return true;
}

void LevelBossGuideController::onTouch(const Point&pos) {
	bool bDelay = false;
	bool bErr = false;
	if (!(m_petMatrix->onTouch(pos, bErr))) {
		if (bErr) {
			enterTouchErrStep();
			return;
		}
	} else {
		bDelay = true;
	}

	if (bDelay) {
		m_petMatrix->runAction(Sequence::create(
			DelayTime::create(1),
			CallFunc::create([=](){
				checkPetMatrix();
				nextGuide();		
		}),NULL));
	} else {
		checkPetMatrix();
		nextGuide();
	}
}

void LevelBossGuideController::onGameReady() {
	auto top = GameUiManager::getInstance()->getTopLayerController();
	if (nullptr != top) {
		top->exit();
	}

	auto boss = GameUiManager::getInstance()->getBossLayerController();
	if (nullptr != boss) {
		boss->enter();
	}

	unloadPetMatrix();
	loadPetMatrix();
	enterPetMatrix();
	GameManager::getInstance()->start();
}

void LevelBossGuideController::onGameStart() {
	m_currentStep = m_stepBossEnter;	
	if (nullptr != m_currentStep) {
		m_currentStep->enter();
	}

	if (nullptr != m_layer) {
		m_layer->runAction(Sequence::create(
			DelayTime::create(2),
			CallFunc::create([=](){
				enterNextStep();
		}),NULL));
	}

	stopTime();
}

void LevelBossGuideController::onGameOver() {
	if (GameData::getInstance()->getScore() >= GameData::getInstance()->getTargetScore()) {
		UserData::setGuidePass(6, true);
		UMengAnalytics::onComplete(ROOKIE_LEVEL_BOSS, GameData::getInstance()->getTime(), GameData::getInstance()->getScore());
		enterNextStep();
	} else {
		UMengAnalytics::onFailed(ROOKIE_LEVEL_BOSS, GameData::getInstance()->getScore(), GameData::getInstance()->getTargetScore());
	}
}

void LevelBossGuideController::nextGuide() {
	if (nullptr != m_currentStep 
		&& m_currentStep->getStep() == Lev_Step_Boss_Talk
		&& m_currentStep->getStep() == Lev_Step_Girl_Talk
		&& m_currentStep->getStep() == Lev_Step_Boss_Tip) {
		enterNextStep();
	}	
}

void LevelBossGuideController::loadPetMatrix() {
	LevelController::loadPetMatrix(6);
}

bool LevelBossGuideController::isManualStep() {
	if (nullptr != m_currentStep && m_currentStep->getStep() == Lev_Step_Boss_Manual) {
		return true;
	}

	return false;
}