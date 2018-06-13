#include "LevelColorPetGuideController.h"
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

#include "LevelStepColorPet.h"
#include "LevelStepWin.h"
#include "LevelStepTouchErr.h"

#define DEMO_Z_ORDER -10
#define TOUCH_Z_ORDER	12
#define MASK_Z_ORDER	10
#define TIP_Z_ORDER		11

LevelColorPetGuideController::LevelColorPetGuideController(Node *layer):
m_stepColorPetGuide(nullptr),
m_stepColorPetManual(nullptr),
m_stepWin(nullptr),
LevelController(layer)
{

}

LevelColorPetGuideController::~LevelColorPetGuideController() {
	clearSteps();
}

bool LevelColorPetGuideController::init() {
	if (!LevelController::init()) {
		return false;
	}

	initial();
	showColorPetDetail(false);
	createSteps();
	return true;
}

void LevelColorPetGuideController::enter() {
	UMengAnalytics::onBegin(ROOKIE_LEVEL_COLORPET);
}

void LevelColorPetGuideController::exit() {
	unloadPetMatrix();
	clearGuide();
}

void LevelColorPetGuideController::initial() {
	if (nullptr == m_layer) {
		return;
	}

	m_redTip = Sprite::create(s_tip_red);
	m_layer->addChild(m_redTip);
	m_redTip->setVisible(false);
}

void LevelColorPetGuideController::showColorPetDetail(bool visible) {
	auto special = GameUiManager::getInstance()->getSpecailPetLayer();
	if (nullptr != special) {
		auto txt_bombpet	= special->getChildByName("girl")->getChildByName("bg_panel")->getChildByName("txt_bombpet");
		auto txt_colorpet	= special->getChildByName("girl")->getChildByName("bg_panel")->getChildByName("txt_colorpet");
		txt_bombpet->setVisible(false);
		txt_colorpet->setVisible(true);
		GameUiManager::getInstance()->showLayer(special, visible);
	}
}

void LevelColorPetGuideController::createSteps() {
	clearSteps();

	m_stepColorPetGuide		= new LevelStepColorPetGuide(this);
	m_stepColorPetManual	= new LevelStepColorPetManual(this);
	m_stepWin				= new LevelStepWin(this);
	m_stepTouchErr			= new LevelStepTouchErr(this);
	m_currentStep			= m_stepColorPetGuide;

	m_stepColorPetGuide->setNextStep(m_stepColorPetManual);
	m_stepColorPetManual->setNextStep(m_stepWin);
	m_stepTouchErr->setNextStep(m_stepColorPetManual);
	m_stepWin->setNextStep(nullptr);
}

void LevelColorPetGuideController::clearSteps() {
	if (nullptr != m_stepColorPetGuide) {
		delete m_stepColorPetGuide;
		m_stepColorPetGuide = nullptr;
	}

	if (nullptr != m_stepColorPetManual) {
		delete m_stepColorPetManual;
		m_stepColorPetManual = nullptr;
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

bool LevelColorPetGuideController::onTouchBegan(Touch * touch,Event *e) {
	if (nullptr == m_petMatrix || nullptr == touch) {
		return true;
	}

	if (nullptr == m_currentStep) {
		return true;
	}

	if (m_currentStep->getStep() == Lev_Step_TouchErr) {
		return true;
	}

	if (m_currentStep->getStep() == Lev_Step_ColorPet_Manual) {
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

void LevelColorPetGuideController::onTouch(const Point&pos) {
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

void LevelColorPetGuideController::onTipWinCompleted() {
	LevelController::onTipWinCompleted();
}

void LevelColorPetGuideController::onGameReady() {
	LevelController::onGameReady();
	unloadPetMatrix();
	m_maskPetVec.clear();
	m_clearablePetVec.clear();
	m_unclearablePetVec.clear();

	loadPetMatrix();
	enterPetMatrix();
	GameManager::getInstance()->start();
}

void LevelColorPetGuideController::onGameStart() {
	m_currentStep = m_stepColorPetGuide;	
	if (nullptr != m_currentStep) {
		m_currentStep->enter();
	}

	stopTime();
}

void LevelColorPetGuideController::onGameOver() {
	clearGuide();
	if (GameData::getInstance()->getScore() >= GameData::getInstance()->getTargetScore()) {
		UserData::setGuidePass(5, true);
		UMengAnalytics::onComplete(ROOKIE_LEVEL_COLORPET, GameData::getInstance()->getTime(), GameData::getInstance()->getScore());
		enterNextStep();
	} else {
		UMengAnalytics::onFailed(ROOKIE_LEVEL_COLORPET, GameData::getInstance()->getScore(), GameData::getInstance()->getTargetScore());
	}
}

void LevelColorPetGuideController::enterColorPetGuide() {
	showColorPetDetail(true);
	guide();
}

void LevelColorPetGuideController::enterColorPetManual() {
	clearGuide();
	showColorPetDetail(false);
	startTime();
}

void LevelColorPetGuideController::guide() {
	m_maskPetVec.clear();

	if (nullptr != m_petMatrix) {
		if (m_petMatrix->getClearablePets(m_maskPetVec, m_clearablePetVec, m_unclearablePetVec, m_touchPetVec2, Pet_Everyok)) {
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

void LevelColorPetGuideController::nextGuide() {
	if (nullptr != m_currentStep && m_currentStep->getStep() == Lev_Step_ColorPet_Guide) {
		enterNextStep();
	}
}

void LevelColorPetGuideController::enterMask(const Point&pos) {
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

void LevelColorPetGuideController::enterMask(std::vector<PetVec2>&mask) {
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



void LevelColorPetGuideController::clearGuide() {
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

void LevelColorPetGuideController::loadPetMatrix() {
	if (nullptr == m_layer) {
		return;
	}

	std::vector<DemoPet> demo;
	DemoPetManager::getLev3DemoPetData(demo);

	m_petMatrix = DemoPetMatrix::createPetMatrix(demo);
	m_layer->addChild(m_petMatrix,10);
	m_petMatrix->setPosition(20, 184);
}

bool LevelColorPetGuideController::isManualStep() {
	if (nullptr != m_currentStep && m_currentStep->getStep() == Lev_Step_ColorPet_Manual) {
		return true;
	}

	return false;
}