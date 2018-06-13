#include "LevelBomPetGuideController.h"
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

#include "LevelStepBomPet.h"
#include "LevelStepWin.h"
#include "LevelStepTouchErr.h"

#define DEMO_Z_ORDER -10
#define TOUCH_Z_ORDER	12
#define MASK_Z_ORDER	10
#define TIP_Z_ORDER		11

LevelBomPetGuideController::LevelBomPetGuideController(Node *layer):
m_stepBomPetGuide(nullptr),
m_stepBomPetManual(nullptr),
m_stepWin(nullptr),
LevelController(layer)
{

}

LevelBomPetGuideController::~LevelBomPetGuideController() {
	clearSteps();
}

bool LevelBomPetGuideController::init() {
	if (!LevelController::init()) {
		return false;
	}

	initial();
	showBomPetDetail(false);
	createSteps();
	return true;
}

void LevelBomPetGuideController::enter() {
	UMengAnalytics::onBegin(ROOKIE_LEVEL_BOMPET);
}

void LevelBomPetGuideController::exit() {
	unloadPetMatrix();
	clearGuide();
}

void LevelBomPetGuideController::initial() {
	if (nullptr == m_layer) {
		return;
	}

	m_redTip = Sprite::create(s_tip_red);
	m_layer->addChild(m_redTip);
	m_redTip->setVisible(false);
}

void LevelBomPetGuideController::showBomPetDetail(bool visible) {
	auto special = GameUiManager::getInstance()->getSpecailPetLayer();
	if (nullptr != special) {
		auto txt_bombpet	= special->getChildByName("girl")->getChildByName("bg_panel")->getChildByName("txt_bombpet");
		auto txt_colorpet	= special->getChildByName("girl")->getChildByName("bg_panel")->getChildByName("txt_colorpet");
		txt_bombpet->setVisible(true);
		txt_colorpet->setVisible(false);
		GameUiManager::getInstance()->showLayer(special, visible);
	}
}

void LevelBomPetGuideController::createSteps() {
	clearSteps();

	m_stepBomPetGuide	= new LevelStepBomPetGuide(this);
	m_stepBomPetManual	= new LevelStepBomPetManual(this);
	m_stepWin			= new LevelStepWin(this);
	m_stepTouchErr		= new LevelStepTouchErr(this);
	m_currentStep		= m_stepBomPetGuide;

	m_stepBomPetGuide->setNextStep(m_stepBomPetManual);
	m_stepBomPetManual->setNextStep(m_stepWin);
	m_stepTouchErr->setNextStep(m_stepBomPetManual);
	m_stepWin->setNextStep(nullptr);
}

void LevelBomPetGuideController::clearSteps() {
	if (nullptr != m_stepBomPetGuide) {
		delete m_stepBomPetGuide;
		m_stepBomPetGuide = nullptr;
	}

	if (nullptr != m_stepBomPetManual) {
		delete m_stepBomPetManual;
		m_stepBomPetManual = nullptr;
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

bool LevelBomPetGuideController::onTouchBegan(Touch * touch,Event *e) {
	if (nullptr == m_petMatrix || nullptr == touch) {
		return true;
	}

	if (nullptr == m_currentStep) {
		return true;
	}

	if (m_currentStep->getStep() == Lev_Step_TouchErr) {
		return true;
	}

	if (m_currentStep->getStep() == Lev_Step_BomPet_Manual) {
		return false;
	}

	if (nullptr != touch) {
		if (isTouchFocus(touch->getLocation(), m_touchPetVec2))  {
			onTouch(touch->getLocation());
			GameEvent::throwEvent(Event_Time_Change, NULL);
		}
	}
	return true;
}

void LevelBomPetGuideController::onTouch(const Point&pos) {
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
		}),
			NULL
			));
	} else {
		checkPetMatrix();
		nextGuide();
	}	
}

void LevelBomPetGuideController::onTipWinCompleted() {
	LevelController::onTipWinCompleted();
}

void LevelBomPetGuideController::onGameReady() {
	LevelController::onGameReady();
	unloadPetMatrix();
	m_maskPetVec.clear();
	m_clearablePetVec.clear();
	m_unclearablePetVec.clear();

	loadPetMatrix();
	enterPetMatrix();
	GameManager::getInstance()->start();
}

void LevelBomPetGuideController::onGameStart() {
	m_currentStep = m_stepBomPetGuide;	
	if (nullptr != m_currentStep) {
		m_currentStep->enter();
	}

	stopTime();
}

void LevelBomPetGuideController::onGameOver() {
	clearGuide();

	if (GameData::getInstance()->getScore() >= GameData::getInstance()->getTargetScore()) {
		UserData::setGuidePass(4, true);
		UMengAnalytics::onComplete(ROOKIE_LEVEL_BOMPET, GameData::getInstance()->getTime(), GameData::getInstance()->getScore());
		enterNextStep();
	} else {
		UMengAnalytics::onFailed(ROOKIE_LEVEL_BOMPET, GameData::getInstance()->getScore(), GameData::getInstance()->getTargetScore());
	}
}

void LevelBomPetGuideController::enterBomPetGuide() {
	showBomPetDetail(true);
	guide();
}

void LevelBomPetGuideController::enterBomPetManual() {
	clearGuide();
	showBomPetDetail(false);
	startTime();
}

void LevelBomPetGuideController::guide() {
	m_maskPetVec.clear();

	if (nullptr != m_petMatrix) {
		if (m_petMatrix->getClearablePets(m_maskPetVec, m_clearablePetVec, m_unclearablePetVec, m_touchPetVec2, Pet_Clear3x3)) {
			enterMask(m_maskPetVec);
			m_petMatrix->playTouchEffect(m_touchPetVec2.iRow, m_touchPetVec2.iCol);
			m_petMatrix->clearMarkSprite();
			//same link
			for (int i=0; i<m_clearablePetVec.size(); i++) {
				auto v = m_clearablePetVec.at(i);
				if (!v.isValid()) { continue; }
				if (v.iCol == m_touchPetVec2.iCol) {
					if (v.iRow < m_touchPetVec2.iRow) {
						m_petMatrix->playTopLinkEffect(m_touchPetVec2.iRow, v.iRow, v.iCol, false);
					} else {
						m_petMatrix->playBottomLinkEffect(m_touchPetVec2.iRow, v.iRow, v.iCol, false);
					}
				} else {
					if (v.iCol < m_touchPetVec2.iCol) {
						m_petMatrix->playLeftLinkEffect(m_touchPetVec2.iCol, v.iCol, v.iRow, false);
					} else {
						m_petMatrix->playRightLinkEffect(m_touchPetVec2.iCol, v.iCol, v.iRow, false);
					}
				}

				m_petMatrix->markSprite(v.iRow, v.iCol, true);
			}

			//diff link
			for (int i=0; i<m_unclearablePetVec.size(); i++) {
				auto v = m_unclearablePetVec.at(i);
				if (!v.isValid()) { continue; }
				if (v.iCol == m_touchPetVec2.iCol) {
					if (v.iRow < m_touchPetVec2.iRow) {
						m_petMatrix->playTopLinkEffect(m_touchPetVec2.iRow, v.iRow, v.iCol, true);
					} else {
						m_petMatrix->playBottomLinkEffect(m_touchPetVec2.iRow, v.iRow, v.iCol, true);
					}
				} else {
					if (v.iCol < m_touchPetVec2.iCol) {
						m_petMatrix->playLeftLinkEffect(m_touchPetVec2.iCol, v.iCol, v.iRow, true);
					} else {
						m_petMatrix->playRightLinkEffect(m_touchPetVec2.iCol, v.iCol, v.iRow, true);
					}
				}

				m_petMatrix->markSprite(v.iRow, v.iCol, false);
			}
		} 
	}
}

void LevelBomPetGuideController::nextGuide() {
	if (nullptr != m_currentStep && m_currentStep->getStep() == Lev_Step_BomPet_Guide) {
		enterNextStep();
	}
}

void LevelBomPetGuideController::enterMask(const Point&pos) {
	if (nullptr == m_layer) {
		return;
	}

	if (nullptr != m_mask) {
		m_layer->removeChild(m_mask, true);
		m_mask = nullptr;
	}

	m_mask = createMaskedSprite("mask_80.png", pos);
	m_layer->addChild(m_mask,MASK_Z_ORDER);
	m_mask->setAnchorPoint(Vec2(0.5,0));
	m_mask->setPosition(Director::getInstance()->getVisibleSize().width * 0.5, 0);
}

void LevelBomPetGuideController::enterMask(std::vector<PetVec2>&mask) {
	if (nullptr == m_layer) {
		return;
	}

	if (nullptr != m_mask) {
		m_layer->removeChild(m_mask, true);
		m_mask = nullptr;
	}

	m_mask = createMaskedSprite(mask);
	m_layer->addChild(m_mask,MASK_Z_ORDER);
	m_mask->setAnchorPoint(Vec2(0.5,0));
	m_mask->setPosition(Director::getInstance()->getVisibleSize().width * 0.5, 0);
}



void LevelBomPetGuideController::clearGuide() {
	if (nullptr != m_mask) {
		m_layer->removeChild(m_mask, true);
		m_mask = nullptr;
	}

	if (nullptr != m_petMatrix) {
		m_petMatrix->clearMarkSprite();
		m_petMatrix->stopTouchEffect();
		m_petMatrix->clearAllBall();
	}
}

void LevelBomPetGuideController::loadPetMatrix() {
	if (nullptr == m_layer) {
		return;
	}

	std::vector<DemoPet> demo;
	DemoPetManager::getLev2DemoPetData(demo);

	m_petMatrix = DemoPetMatrix::createPetMatrix(demo);
	m_layer->addChild(m_petMatrix,10);
	m_petMatrix->setPosition(20, 184);
}

bool LevelBomPetGuideController::isManualStep() {
	if (nullptr != m_currentStep && m_currentStep->getStep() == Lev_Step_BomPet_Manual) {
		return true;
	}

	return false;
}