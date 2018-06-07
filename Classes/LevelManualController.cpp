#include "LevelManualController.h"
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
#include "UToolConfirmLayerController.h"

#include "LevelStepManual.h"
#include "LevelStepTarget.h"
#include "LevelStepWin.h"
#include "LevelStepTouchErr.h"

#define DEMO_Z_ORDER -10
#define TOUCH_Z_ORDER	12
#define MASK_Z_ORDER	10
#define TIP_Z_ORDER		11

LevelManualController::LevelManualController(Node *layer):
m_stepToolConfirm(nullptr),
m_stepTarget(nullptr),
m_stepManual(nullptr),
m_stepWin(nullptr),
LevelController(layer)
{

}

LevelManualController::~LevelManualController() {
	clearSteps();
}

bool LevelManualController::init() {
	if (!LevelController::init()) {
		return false;
	}

	initial();
	createSteps();
	return true;
}

void LevelManualController::enter() {
}

void LevelManualController::exit() {
	unloadPetMatrix();
}

void LevelManualController::initial() {
	if (nullptr == m_layer) {
		return;
	}

}

void LevelManualController::showToolConfirm() {
	auto toolConfirm = GameUiManager::getInstance()->getToolConfirmLayerController();
	if (nullptr != toolConfirm) {
		toolConfirm->setExitCallback([=](){
			enterNextStep();
			toolConfirm->setExitCallback(nullptr);
		});

		toolConfirm->enterDisfree();
	} else {
		enterNextStep();
	}
}

void LevelManualController::enterManual() {

}


void LevelManualController::createSteps() {
	clearSteps();

	m_stepToolConfirm	= new LevelStepToolConfirm(this);
	m_stepTarget		= new LevelStepTarget(this);
	m_stepManual		= new LevelStepManual(this);
	m_stepWin			= new LevelStepWin(this);
	m_stepTouchErr		= new LevelStepTouchErr(this);
	m_currentStep		= m_stepToolConfirm;

	m_stepToolConfirm->setNextStep(m_stepTarget);
	m_stepTarget->setNextStep(m_stepManual);
	m_stepTouchErr->setNextStep(m_stepManual);
	m_stepManual->setNextStep(m_stepWin);
	m_stepWin->setNextStep(nullptr);
}

void LevelManualController::clearSteps() {
	if (nullptr != m_stepToolConfirm) {
		delete m_stepToolConfirm;
		m_stepToolConfirm = nullptr;
	}

	if (nullptr != m_stepTarget) {
		delete m_stepTarget;
		m_stepTarget = nullptr;
	}

	if (nullptr != m_stepManual) {
		delete m_stepManual;
		m_stepManual = nullptr;
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

bool LevelManualController::onTouchBegan(Touch * touch,Event *e) {
	if (nullptr!= m_currentStep && m_currentStep->getStep() == Lev_Step_TouchErr) {
		return true;
	}

	return false;
}

void LevelManualController::onTouch(const Point&pos) {
	bool bDelay = false;
	bool bErr;
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
		}),NULL));
	} else {
		checkPetMatrix();
	}
}

void LevelManualController::onGameReady() {
	unloadPetMatrix();
	loadPetMatrix();
	enterPetMatrix();
	m_currentStep = m_stepToolConfirm;
	if (nullptr != m_currentStep) {
		m_currentStep->enter();
	}
}

void LevelManualController::onGameStart() {
	m_currentStep = m_stepManual;	
	if (nullptr != m_currentStep) {
		m_currentStep->enter();
	}
}

void LevelManualController::onGameOver() {
	if (GameData::getInstance()->getScore() >= GameData::getInstance()->getTargetScore()
		&& (nullptr != m_currentStep && m_currentStep->getStep() == Lev_Step_Manual)) {
		enterNextStep();
	}
}

void LevelManualController::loadPetMatrix() {
	LevelController::loadPetMatrix(GameData::getInstance()->getLevel());
}

bool LevelManualController::isManualStep() {
	if (nullptr != m_currentStep && m_currentStep->getStep() == Lev_Step_Manual) {
		return true;
	}

	return false;
}