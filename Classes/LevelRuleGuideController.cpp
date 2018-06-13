#include "LevelRuleGuideController.h"
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

#include "LevelStepWelcome.h"
#include "LevelStepWin.h"
#include "LevelStepRuleGuide.h"

#define DEMO_Z_ORDER -10
#define TOUCH_Z_ORDER	12
#define MASK_Z_ORDER	10
#define TIP_Z_ORDER		11

LevelRuleGuideController::LevelRuleGuideController(Node *layer):
m_stepWelcome(nullptr),
m_stepRuleGuide(nullptr),
m_stepWin(nullptr),
m_stepWelcomeEnd(nullptr),
LevelController(layer)
{

}

LevelRuleGuideController::~LevelRuleGuideController() {
	clearSteps();
}

bool LevelRuleGuideController::init() {
	if (!LevelController::init()) {
		return false;
	}

	initial();
	showWelcome(false);
	createSteps();
	return true;
}

void LevelRuleGuideController::enter() {
	UMengAnalytics::onBegin(ROOKIE_LEVEL_RULE);
}

void LevelRuleGuideController::exit() {
	unloadPetMatrix();
	clearGuide();
}

void LevelRuleGuideController::initial() {
	if (nullptr == m_layer) {
		return;
	}
}

void LevelRuleGuideController::showWelcome(bool visible) {
	auto welcome = GameUiManager::getInstance()->getStepWelcomeLayer();
	if (nullptr != welcome) {
		auto txt_welcome = welcome->getChildByName("girl")->getChildByName("bg_panel")->getChildByName("welcome");
		auto txt_rule = welcome->getChildByName("girl")->getChildByName("bg_panel")->getChildByName("rule");
		txt_welcome->setVisible(true);
		txt_rule->setVisible(false);
		GameUiManager::getInstance()->showLayer(welcome, visible);
		if (visible) {
			if (nullptr != m_layer) {
				m_layer->runAction(Sequence::create(
					DelayTime::create(1.5),
					CallFunc::create([=](){
					GameUiManager::getInstance()->showLayer(welcome, false);
					if (nullptr != m_currentStep && m_currentStep->getStep() == Lev_Step_Welcome) {
						enterNextStep();
					}
				}),NULL));
			}
		}
	}
}

void LevelRuleGuideController::showRule(bool visible) {
	auto welcome = GameUiManager::getInstance()->getStepWelcomeLayer();
	if (nullptr != welcome) {
		auto txt_welcome = welcome->getChildByName("girl")->getChildByName("bg_panel")->getChildByName("welcome");
		auto txt_rule = welcome->getChildByName("girl")->getChildByName("bg_panel")->getChildByName("rule");
		txt_welcome->setVisible(false);
		txt_rule->setVisible(true);
		GameUiManager::getInstance()->showLayer(welcome, visible);
	}
}

void LevelRuleGuideController::showWelcomeEnd() {
	auto welcomeEnd = GameUiManager::getInstance()->getWelcomeEndLayer();
	if (nullptr != welcomeEnd) {
		auto yesBtn = dynamic_cast<Button*>(welcomeEnd->getChildByName("bg_mid")
												->getChildByName("bg_bottom")
												->getChildByName("Btn_yes"));
		if (nullptr != yesBtn) {
			yesBtn->addClickEventListener([=](Ref*ref){
				Audio::getInstance()->playClick();
				GameUiManager::getInstance()->showLayer(welcomeEnd, false);
				GameData::getInstance()->addToolCross(2);
				GameData::getInstance()->addToolBomb(2);
				enterNextStep();

				GameManager::getInstance()->ready(1);
			});
		}

		GameUiManager::getInstance()->showLayer(welcomeEnd, true);	
	}
}

void LevelRuleGuideController::createSteps() {
	clearSteps();

	m_stepWelcome		= new LevelStepWelcome(this);
	m_stepRuleGuide		= new LevelStepRuleGuide(this);
	m_stepWin			= new LevelStepWin(this);
	m_stepWelcomeEnd	= new LevelStepWelcomeEnd(this);
	m_currentStep		= m_stepWelcome;

	m_stepWelcome->setNextStep(m_stepRuleGuide);
	m_stepRuleGuide->setNextStep(m_stepWin);
	m_stepWin->setNextStep(m_stepWelcomeEnd);
	m_stepWelcomeEnd->setNextStep(nullptr);
}

void LevelRuleGuideController::clearSteps() {
	if (nullptr != m_stepWelcome) {
		delete m_stepWelcome;
		m_stepWelcome = nullptr;
	}

	if (nullptr != m_stepRuleGuide) {
		delete m_stepRuleGuide;
		m_stepRuleGuide = nullptr;
	}

	if (nullptr != m_stepWin) {
		delete m_stepWin;
		m_stepWin = nullptr;
	}

	if (nullptr != m_stepWelcomeEnd) {
		delete m_stepWelcomeEnd;
		m_stepWelcomeEnd = nullptr;
	}

	m_currentStep = nullptr;
}

bool LevelRuleGuideController::onTouchBegan(Touch * touch,Event *e) {
	if (nullptr == m_petMatrix || nullptr == touch) {
		return true;
	}

	if (nullptr == m_currentStep) {
		return true;
	}

	if (m_currentStep->getStep() == Lev_Step_Welcome) {
		enterNextStep();
		return true;
	}

	if (nullptr != touch) {
		if (isTouchFocus(touch->getLocation(), m_touchPetVec2))  {
			onTouch(touch->getLocation());
			GameEvent::throwEvent(Event_Time_Change, NULL);
		}
	}
	return true;
}

void LevelRuleGuideController::onTouch(const Point&pos) {
	bool bDelay = false;
	bool bErr = false;
	if (!(m_petMatrix->onTouch(pos,bErr))) {
	} else {
		bDelay = true;
	}

	if (bDelay) {
		m_petMatrix->runAction(Sequence::create(
			DelayTime::create(1),
			CallFunc::create([=](){
				nextGuide();		
		}),
			NULL
			));
	} else {
		nextGuide();
	}	
}

void LevelRuleGuideController::onTipWinCompleted() {
	enterNextStep();
}

void LevelRuleGuideController::onGameReady() {
	LevelController::onGameReady();
	unloadPetMatrix();
	m_maskPetVec.clear();
	m_clearablePetVec.clear();
	m_unclearablePetVec.clear();
	loadPetMatrix();
	enterPetMatrix();
	GameManager::getInstance()->start();
}

void LevelRuleGuideController::onGameStart() {
	m_currentStep = m_stepWelcome;	
	if (nullptr != m_currentStep) {
		m_currentStep->enter();
	}

	stopTime();
}

void LevelRuleGuideController::onGameOver() {
	clearGuide();
	enterNextStep();
	UMengAnalytics::onComplete(ROOKIE_LEVEL_RULE, 0, 0);
	UserData::setGuidePass(0, true);
}

void LevelRuleGuideController::guide() {
	m_maskPetVec.clear();

	if (nullptr != m_petMatrix) {
		if (m_petMatrix->getClearablePets(m_maskPetVec, m_clearablePetVec, m_unclearablePetVec, m_touchPetVec2)) {
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

void LevelRuleGuideController::nextGuide() {
	m_maskPetVec.clear();

	if (GameManager::getInstance()->isOver()) {
		return;
	}

	if (nullptr != m_petMatrix) {
		if (m_petMatrix->getClearablePets(m_maskPetVec, m_clearablePetVec, m_unclearablePetVec, m_touchPetVec2)) {
			enterMask(m_maskPetVec);
			m_petMatrix->playTouchEffect(m_touchPetVec2.iRow, m_touchPetVec2.iCol);
			m_petMatrix->clearMarkSprite();
			m_petMatrix->clearAllBall();
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

void LevelRuleGuideController::enterMask(const Point&pos) {
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

void LevelRuleGuideController::enterMask(std::vector<PetVec2>&mask) {
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



void LevelRuleGuideController::clearGuide() {
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

void LevelRuleGuideController::loadPetMatrix() {
	if (nullptr == m_layer) {
		return;
	}

	std::vector<DemoPet> demo;
	DemoPetManager::getLev1DemoPetData(demo);

	m_petMatrix = DemoPetMatrix::createPetMatrix(demo);
	m_layer->addChild(m_petMatrix,10);
	m_petMatrix->setPosition(20, 184);
}