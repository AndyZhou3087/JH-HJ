#include "LevelCrossGuideController.h"
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

#include "LevelStepCross.h"
#include "LevelStepWin.h"
#include "LevelStepTouchErr.h"

#define DEMO_Z_ORDER -10
#define TOUCH_Z_ORDER	12
#define MASK_Z_ORDER	10
#define TIP_Z_ORDER		11

LevelCrossGuideController::LevelCrossGuideController(Node *layer):
m_stepCrossDetail(nullptr),
m_stepCrossFocus(nullptr),
m_stepCrossGuide(nullptr),
m_stepCrossManual(nullptr),
m_stepWin(nullptr),
m_mask(nullptr),
LevelController(layer)
{

}

LevelCrossGuideController::~LevelCrossGuideController() {
	clearSteps();
}

bool LevelCrossGuideController::init() {
	if (!LevelController::init()) {
		return false;
	}

	initial();
	showCrossDetail(false);
	createSteps();
	return true;
}

void LevelCrossGuideController::enter() {
	UMengAnalytics::onBegin(ROOKIE_LEVEL_CROSS);
}

void LevelCrossGuideController::exit() {
	unloadPetMatrix();
	clearGuide();
}

void LevelCrossGuideController::initial() {
	if (nullptr == m_layer) {
		return;
	}

	createCrossArrowTip();
}

void LevelCrossGuideController::showCrossDetail(bool visible) {
	auto crossGuide = GameUiManager::getInstance()->getCrossGuideLayer();
	if (nullptr != crossGuide) {
		auto yesBtn = dynamic_cast<Button*>(crossGuide->getChildByName("bg_panel")->getChildByName("Btn_yes"));
		if (nullptr != yesBtn) {
			yesBtn->addClickEventListener([=](Ref*ref){
				Audio::getInstance()->playClick();
				GameUiManager::getInstance()->showLayer(crossGuide, false);
				enterNextStep();
			});
		}

		GameUiManager::getInstance()->showLayer(crossGuide, true);	
	}
}

void LevelCrossGuideController::enterCrossFocus() {
	enterMaskCross();
}

void LevelCrossGuideController::enterCrossGuide() {
	hideArrowTip();
	m_maskPetVec.clear();
	if (nullptr!= m_petMatrix) {
		m_petMatrix->getCrossPets(m_maskPetVec, m_touchPetVec2);
		enterMask(m_maskPetVec);
		m_petMatrix->playTouchEffect(m_touchPetVec2.iRow, m_touchPetVec2.iCol);
	}
}

void LevelCrossGuideController::enterCrossManual() {
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

void LevelCrossGuideController::createSteps() {
	clearSteps();

	m_stepCrossDetail	= new LevelStepCrossDetail(this);
	m_stepCrossFocus	= new LevelStepCrossFocus(this);
	m_stepCrossGuide	= new LevelStepCrossGuide(this);
	m_stepCrossManual	= new LevelStepCrossManual(this);
	m_stepWin			= new LevelStepWin(this);
	m_stepTouchErr		= new LevelStepTouchErr(this);
	m_currentStep		= m_stepCrossDetail;


	m_stepCrossDetail->setNextStep(m_stepCrossFocus);
	m_stepCrossFocus->setNextStep(m_stepCrossGuide);
	m_stepCrossGuide->setNextStep(m_stepCrossManual);
	m_stepCrossManual->setNextStep(m_stepWin);
	m_stepTouchErr->setNextStep(m_stepCrossManual);
	m_stepWin->setNextStep(nullptr);
}

void LevelCrossGuideController::clearSteps() {
	if (nullptr != m_stepCrossDetail) {
		delete m_stepCrossDetail;
		m_stepCrossDetail = nullptr;
	}

	if (nullptr != m_stepCrossFocus) {
		delete m_stepCrossFocus;
		m_stepCrossFocus = nullptr;
	}

	if (nullptr != m_stepCrossGuide) {
		delete m_stepCrossGuide;
		m_stepCrossGuide = nullptr;
	}

	if (nullptr != m_stepCrossManual) {
		delete m_stepCrossManual;
		m_stepCrossManual = nullptr;
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

bool LevelCrossGuideController::onTouchBegan(Touch * touch,Event *e) {
	if (nullptr == m_petMatrix || nullptr == touch) {
		return true;
	}

	if (nullptr == m_currentStep) {
		return true;
	}

	if (m_currentStep->getStep() == Lev_Step_TouchErr) {
		return true;
	}

	if (m_currentStep->getStep() == Lev_Step_Cross_Manual) {
		return false;
	}

	if (m_currentStep->getStep() == Lev_Step_Cross_Focus &&
		touch->getLocation().x >0 
		&& touch->getLocation().x < Director::getInstance()->getVisibleSize().width * 0.25 
		&& touch->getLocation().y < 144 ) {
			onTouchCross();
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

void LevelCrossGuideController::onTouch(const Point&pos) {
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

void LevelCrossGuideController::onGameReady() {
	LevelController::onGameReady();
	unloadPetMatrix();
	m_maskPetVec.clear();
	m_clearablePetVec.clear();
	m_unclearablePetVec.clear();

	loadPetMatrix();
	enterPetMatrix();
	GameManager::getInstance()->start();
}

void LevelCrossGuideController::onGameStart() {
	m_currentStep = m_stepCrossDetail;	
	if (nullptr != m_currentStep) {
		m_currentStep->enter();
	}

	stopTime();
}

void LevelCrossGuideController::onGameOver() {
	clearGuide();
	if (GameData::getInstance()->getScore() >= GameData::getInstance()->getTargetScore()) {
		UserData::setGuidePass(1, true);
		UMengAnalytics::onComplete(ROOKIE_LEVEL_CROSS, GameData::getInstance()->getTime(), GameData::getInstance()->getScore());
		enterNextStep();
	} else {
		UMengAnalytics::onFailed(ROOKIE_LEVEL_CROSS, GameData::getInstance()->getScore(), GameData::getInstance()->getTargetScore());
	}
}

void LevelCrossGuideController::enterMaskCross() {
	enterMask(Point(72, 48));
	if (nullptr != m_arrow) {
		m_arrow->setPosition(72, 90);
		m_arrow->setVisible(true);
		playArrowEffect();
	}

	if (nullptr != m_tipLabel) {
		m_tipLabel->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(s_tip_cross));
		m_tipLabel->setPosition(72, m_arrow->getPositionY() + m_arrow->getContentSize().height);
		m_tipLabel->setVisible(true);
	}
}

void LevelCrossGuideController::playArrowEffect() {
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

void LevelCrossGuideController::stopArrowEffect() {
	if (nullptr == m_arrow) {
		return;
	}

	m_arrow->stopAllActions();
}

void LevelCrossGuideController::onTouchCross() {
	enterNextStep();
}

void LevelCrossGuideController::nextGuide() {
	if (nullptr != m_currentStep && m_currentStep->getStep() == Lev_Step_Cross_Guide) {
		enterNextStep();
	}	
}

void LevelCrossGuideController::enterMask(const Point&pos) {
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

void LevelCrossGuideController::enterMask(std::vector<PetVec2>&mask) {
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

void LevelCrossGuideController::clearMask() {
	if (nullptr != m_mask) {
		m_layer->removeChild(m_mask, true);
		m_mask = nullptr;
	}
}

void LevelCrossGuideController::clearGuide() {
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

void LevelCrossGuideController::loadPetMatrix() {
	LevelController::loadPetMatrix(1);
}

bool LevelCrossGuideController::isManualStep() {
	if (nullptr != m_currentStep && m_currentStep->getStep() == Lev_Step_Cross_Manual) {
		return true;
	}

	return false;
}