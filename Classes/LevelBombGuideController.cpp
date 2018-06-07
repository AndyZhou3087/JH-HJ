#include "LevelBombGuideController.h"
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

#include "LevelStepBomb.h"
#include "LevelStepWin.h"
#include "LevelStepTouchErr.h"

#define DEMO_Z_ORDER -10
#define TOUCH_Z_ORDER	12
#define MASK_Z_ORDER	10
#define TIP_Z_ORDER		11

LevelBombGuideController::LevelBombGuideController(Node *layer):
m_stepBombDetail(nullptr),
m_stepBombFocus(nullptr),
m_stepBombManual(nullptr),
m_stepWin(nullptr),
m_mask(nullptr),
LevelController(layer)
{

}

LevelBombGuideController::~LevelBombGuideController() {
	clearSteps();
}

bool LevelBombGuideController::init() {
	if (!LevelController::init()) {
		return false;
	}

	initial();
	showBombDetail(false);
	createSteps();
	return true;
}

void LevelBombGuideController::enter() {
	UMengAnalytics::onBegin(ROOKIE_LEVEL_BOMB);
}

void LevelBombGuideController::exit() {
	unloadPetMatrix();
	clearGuide();
}

void LevelBombGuideController::initial() {
	if (nullptr == m_layer) {
		return;
	}

	createBombArrowTip();
}

void LevelBombGuideController::showBombDetail(bool visible) {
	auto bombGuide = GameUiManager::getInstance()->getBombGuideLayer();
	if (nullptr != bombGuide) {
		auto yesBtn = dynamic_cast<Button*>(bombGuide->getChildByName("bg_panel")->getChildByName("Btn_yes"));
		if (nullptr != yesBtn) {
			yesBtn->addClickEventListener([=](Ref*ref){
				Audio::getInstance()->playClick();
				GameUiManager::getInstance()->showLayer(bombGuide, false);
				enterNextStep();
			});
		}

		GameUiManager::getInstance()->showLayer(bombGuide, true);	
	}
}

void LevelBombGuideController::enterBombFocus() {
	enterMaskBomb();
}

void LevelBombGuideController::enterBombManual() {
	m_maskPetVec.clear();

	if (GameManager::getInstance()->isOver()) {
		return;
	}

	startTime();
	clearMask();
	hideArrowTip();
	if (nullptr != m_petMatrix) {
		m_petMatrix->stopTouchEffect();
	}
}

void LevelBombGuideController::createSteps() {
	clearSteps();

	m_stepBombDetail	= new LevelStepBombDetail(this);
	m_stepBombFocus		= new LevelStepBombFocus(this);
	m_stepBombManual	= new LevelStepBombManual(this);
	m_stepWin			= new LevelStepWin(this);
	m_stepTouchErr		= new LevelStepTouchErr(this);
	m_currentStep		= m_stepBombDetail;

	m_stepBombDetail->setNextStep(m_stepBombFocus);
	m_stepBombFocus->setNextStep(m_stepBombManual);
	m_stepBombManual->setNextStep(m_stepWin);
	m_stepTouchErr->setNextStep(m_stepBombManual);
	m_stepWin->setNextStep(nullptr);
}

void LevelBombGuideController::clearSteps() {
	if (nullptr != m_stepBombDetail) {
		delete m_stepBombDetail;
		m_stepBombDetail = nullptr;
	}

	if (nullptr != m_stepBombFocus) {
		delete m_stepBombFocus;
		m_stepBombFocus = nullptr;
	}

	if (nullptr != m_stepBombManual) {
		delete m_stepBombManual;
		m_stepBombManual = nullptr;
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

bool LevelBombGuideController::onTouchBegan(Touch * touch,Event *e) {
	if (nullptr == m_petMatrix || nullptr == touch) {
		return true;
	}

	if (nullptr == m_currentStep) {
		return true;
	}

	if (m_currentStep->getStep() == Lev_Step_TouchErr) {
		return true;
	}

	if (m_currentStep->getStep() == Lev_Step_Bomb_Manual) {
		return false;
	}

	if (m_currentStep->getStep() == Lev_Step_Bomb_Focus &&
		touch->getLocation().x < Director::getInstance()->getVisibleSize().width
		&& touch->getLocation().x > Director::getInstance()->getVisibleSize().width * 0.75
		&& touch->getLocation().y < 144 ) {
			onTouchBomb();
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

void LevelBombGuideController::onTouch(const Point&pos) {
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

void LevelBombGuideController::onGameReady() {
	LevelController::onGameReady();
	unloadPetMatrix();
	m_maskPetVec.clear();
	m_clearablePetVec.clear();
	m_unclearablePetVec.clear();

	loadPetMatrix();
	enterPetMatrix();
	GameManager::getInstance()->start();
}

void LevelBombGuideController::onGameStart() {
	m_currentStep = m_stepBombDetail;	
	if (nullptr != m_currentStep) {
		m_currentStep->enter();
	}

	stopTime();
}

void LevelBombGuideController::onGameOver() {
	clearGuide();

	if (GameData::getInstance()->getScore() >= GameData::getInstance()->getTargetScore()) {
		UserData::setGuidePass(2, true);
		UMengAnalytics::onComplete(ROOKIE_LEVEL_BOMB, GameData::getInstance()->getTime(), GameData::getInstance()->getScore());
		enterNextStep();
	} else {
		UMengAnalytics::onFailed(ROOKIE_LEVEL_BOMB, GameData::getInstance()->getScore(), GameData::getInstance()->getTargetScore());
	}
}

void LevelBombGuideController::enterMaskBomb() {
	enterMask(Point(420, 48));

	if (nullptr != m_arrow) {
		m_arrow->setPosition(420, 90);
		m_arrow->setVisible(true);
		playArrowEffect();
	}

	if (nullptr != m_tipLabel) {
		m_tipLabel->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(s_tip_bomb));
		m_tipLabel->setPosition(420, m_arrow->getPositionY() + m_arrow->getContentSize().height);
		m_tipLabel->setVisible(true);
	}

	if (nullptr != m_petMatrix) {
		m_petMatrix->stopTouchEffect();
	}
}

void LevelBombGuideController::playArrowEffect() {
	if (nullptr == m_arrow) {
		return;
	}

	m_arrow->stopAllActions();
	m_arrow->runAction(RepeatForever::create(
		Sequence::create(
		MoveBy::create(0.6, Vec2(0, -10)),
		MoveBy::create(0.6, Vec2(0,10)),
		NULL
		)
		));
}

void LevelBombGuideController::stopArrowEffect() {
	if (nullptr == m_arrow) {
		return;
	}

	m_arrow->stopAllActions();
}


void LevelBombGuideController::onTouchBomb() {
	enterNextStep();
}

void LevelBombGuideController::nextGuide() {
	if (nullptr != m_currentStep && m_currentStep->getStep() == Lev_Step_Cross_Guide) {
		enterNextStep();
	}	
}

void LevelBombGuideController::enterMask(const Point&pos) {
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

void LevelBombGuideController::enterMask(std::vector<PetVec2>&mask) {
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

void LevelBombGuideController::clearMask() {
	if (nullptr != m_mask) {
		m_layer->removeChild(m_mask, true);
		m_mask = nullptr;
	}
}

void LevelBombGuideController::clearGuide() {
	if (nullptr != m_mask) {
		m_layer->removeChild(m_mask, true);
		m_mask = nullptr;
	}

	if (nullptr != m_petMatrix) {
		m_petMatrix->clearMarkSprite();
		m_petMatrix->stopTouchEffect();
		m_petMatrix->clearAllBall();
	}

	clearArrowTip();
}

void LevelBombGuideController::loadPetMatrix() {
	LevelController::loadPetMatrix(2);
}

bool LevelBombGuideController::isManualStep() {
	if (nullptr != m_currentStep && m_currentStep->getStep() == Lev_Step_Bomb_Manual) {
		return true;
	}

	return false;
}