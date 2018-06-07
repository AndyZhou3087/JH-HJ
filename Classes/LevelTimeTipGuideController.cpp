#include "LevelTimeTipGuideController.h"
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

#include "LevelStepGiveTool.h"
#include "LevelStepTime.h"
#include "LevelStepTip.h"
#include "LevelStepWin.h"
#include "LevelStepTouchErr.h"
#include "UGuideTipLayerController.h"
#define DEMO_Z_ORDER -10
#define TOUCH_Z_ORDER	12
#define MASK_Z_ORDER	10
#define TIP_Z_ORDER		11

LevelTimeTipGuideController::LevelTimeTipGuideController(Node *layer):
m_stepGiveTool(nullptr),
m_stepTimeDetail(nullptr),
m_stepTimeFocus(nullptr),
m_stepTimeGuide(nullptr),
m_stepTimeManual(nullptr),
m_stepTipDetail(nullptr),
m_stepTipFocus(nullptr),
m_stepTipGuide(nullptr),
m_stepTipManual(nullptr),
m_stepWin(nullptr),
m_mask(nullptr),
LevelController(layer)
{

}

LevelTimeTipGuideController::~LevelTimeTipGuideController() {
	clearSteps();
}

bool LevelTimeTipGuideController::init() {
	if (!LevelController::init()) {
		return false;
	}

	initial();
	showTimeDetail(false);
	showTipDetail(false);
	createSteps();
	return true;
}

void LevelTimeTipGuideController::enter() {
	UMengAnalytics::onBegin(ROOKIE_LEVEL_TIMETIP);
}

void LevelTimeTipGuideController::exit() {
	unloadPetMatrix();
	clearGuide();
}

void LevelTimeTipGuideController::initial() {
	if (nullptr == m_layer) {
		return;
	}

	createBombArrowTip();
}

void LevelTimeTipGuideController::showToolConfirm() {
	if (0 == GameData::getInstance()->getToolTimeNum()
		&& 0 == GameData::getInstance()->getToolTipNum()) {
		auto toolConfirm = GameUiManager::getInstance()->getToolConfirmLayerController();
		if (nullptr != toolConfirm) {
			toolConfirm->setExitCallback([=](){
				enterNextStep();
				toolConfirm->setExitCallback(nullptr);
			});

			toolConfirm->enterFree();
		} else {
			enterNextStep();
		}
	} else {
		enterNextStep();
	}
}

void LevelTimeTipGuideController::showTimeDetail(bool visible) {
	auto timeGuide = GameUiManager::getInstance()->getTimeGuideLayer();
	if (nullptr != timeGuide) {
		auto yesBtn = dynamic_cast<Button*>(timeGuide->getChildByName("bg_panel")->getChildByName("Btn_yes"));
		if (nullptr != yesBtn) {
			yesBtn->addClickEventListener([=](Ref*ref){
				Audio::getInstance()->playClick();
				GameUiManager::getInstance()->showLayer(timeGuide, false);
				enterNextStep();
			});
		}

		GameUiManager::getInstance()->showLayer(timeGuide, visible);	
	}
}

void LevelTimeTipGuideController::enterTimeFocus() {
	enterMaskTime();
}

void LevelTimeTipGuideController::enterTimeGuide() {
	enterTimeManual();
}

void LevelTimeTipGuideController::enterTimeManual() {
	m_maskPetVec.clear();

	if (nullptr != m_stepTouchErr) {
		m_stepTouchErr->setNextStep(m_stepTimeManual);
	}

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

void LevelTimeTipGuideController::showTipDetail(bool visible) {
	auto tipGuide = GameUiManager::getInstance()->getTipGuideLayerController();
	if (nullptr != tipGuide) {
		tipGuide->setExitCallback([=](){
			enterNextStep();
			tipGuide->setExitCallback(nullptr);
		});
		if (visible) {
			tipGuide->enter();
		}
	}
}

void LevelTimeTipGuideController::enterTipFocus() {
	enterMaskTip();
}

void LevelTimeTipGuideController::enterTipGuide() {
	enterTipManual();
}

void LevelTimeTipGuideController::enterTipManual() {
	m_maskPetVec.clear();

	if (nullptr != m_stepTouchErr) {
		m_stepTouchErr->setNextStep(m_stepTipManual);
	}

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

void LevelTimeTipGuideController::createSteps() {
	clearSteps();

	m_stepGiveTool		= new LevelStepGiveTool(this);
	m_stepTimeDetail	= new LevelStepTimeDetail(this);
	m_stepTimeFocus		= new LevelStepTimeFocus(this);
	m_stepTimeGuide		= new LevelStepTimeGuide(this);
	m_stepTimeManual	= new LevelStepTimeManual(this);
	m_stepTipDetail		= new LevelStepTipDetail(this);
	m_stepTipFocus		= new LevelStepTipFocus(this);
	m_stepTipGuide		= new LevelStepTipGuide(this);
	m_stepTipManual		= new LevelStepTipManual(this);
	m_stepWin			= new LevelStepWin(this);
	m_stepTouchErr		= new LevelStepTouchErr(this);
	m_currentStep		= m_stepGiveTool;

	m_stepGiveTool->setNextStep(m_stepTimeDetail);
	m_stepTimeDetail->setNextStep(m_stepTimeFocus);
	m_stepTimeFocus->setNextStep(m_stepTimeGuide);
	m_stepTimeGuide->setNextStep(m_stepTimeManual);
	m_stepTimeManual->setNextStep(m_stepTipDetail);
	m_stepTipDetail->setNextStep(m_stepTipFocus);
	m_stepTipFocus->setNextStep(m_stepTipGuide);
	m_stepTipGuide->setNextStep(m_stepTipManual);
	m_stepTipManual->setNextStep(m_stepWin);
	m_stepWin->setNextStep(nullptr);
}

void LevelTimeTipGuideController::clearSteps() {
	if (nullptr != m_stepGiveTool) {
		delete m_stepGiveTool;
		m_stepGiveTool = nullptr;
	}

	if (nullptr != m_stepTimeDetail) {
		delete m_stepTimeDetail;
		m_stepTimeDetail = nullptr;
	}

	if (nullptr != m_stepTimeFocus) {
		delete m_stepTimeFocus;
		m_stepTimeFocus = nullptr;
	}

	if (nullptr != m_stepTimeGuide) {
		delete m_stepTimeGuide;
		m_stepTimeGuide = nullptr;
	}

	if (nullptr != m_stepTimeManual) {
		delete m_stepTimeManual;
		m_stepTimeManual = nullptr;
	}

	if (nullptr != m_stepTipDetail) {
		delete m_stepTipDetail;
		m_stepTipDetail = nullptr;
	}

	if (nullptr != m_stepTipFocus) {
		delete m_stepTipFocus;
		m_stepTipFocus = nullptr;
	}

	if (nullptr != m_stepTipGuide) {
		delete m_stepTipGuide;
		m_stepTipGuide = nullptr;
	}

	if (nullptr != m_stepTipManual) {
		delete m_stepTipManual;
		m_stepTipManual = nullptr;
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

bool LevelTimeTipGuideController::onTouchBegan(Touch * touch,Event *e) {
	if (nullptr == m_petMatrix || nullptr == touch) {
		return true;
	}

	if (nullptr == m_currentStep) {
		return true;
	}

	if (m_currentStep->getStep() == Lev_Step_TouchErr) {
		return true;
	}

	if (m_currentStep->getStep() == Lev_Step_Time_Manual
		|| m_currentStep->getStep() == Lev_Step_Time_Guide
		|| m_currentStep->getStep() == Lev_Step_Tip_Manual
		|| m_currentStep->getStep() == Lev_Step_Tip_Guide) {
		return false;
	}

	if (m_currentStep->getStep() == Lev_Step_Time_Focus &&
		touch->getLocation().x < Director::getInstance()->getVisibleSize().width * 0.5
		&& touch->getLocation().x > Director::getInstance()->getVisibleSize().width * 0.25
		&& touch->getLocation().y < 144 ) {
			onTouchTime();
			return false;
	}

	if (m_currentStep->getStep() == Lev_Step_Tip_Focus &&
		touch->getLocation().x < Director::getInstance()->getVisibleSize().width * 0.75
		&& touch->getLocation().x > Director::getInstance()->getVisibleSize().width * 0.5
		&& touch->getLocation().y < 144 ) {
			onTouchTip();
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

void LevelTimeTipGuideController::onTouch(const Point&pos) {
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

void LevelTimeTipGuideController::onGameReady() {
	LevelController::onGameReady();
	unloadPetMatrix();
	m_maskPetVec.clear();
	m_clearablePetVec.clear();
	m_unclearablePetVec.clear();

	loadPetMatrix();
	enterPetMatrix();
	GameManager::getInstance()->start();
}

void LevelTimeTipGuideController::onGameStart() {
	m_currentStep = m_stepGiveTool;	
	if (nullptr != m_currentStep) {
		m_currentStep->enter();
	}

	stopTime();
}

void LevelTimeTipGuideController::onGameOver() {
	clearGuide();

	if (GameData::getInstance()->getScore() >= GameData::getInstance()->getTargetScore()) {
		UserData::setGuidePass(3, true);
		UMengAnalytics::onComplete(ROOKIE_LEVEL_TIMETIP, GameData::getInstance()->getTime(), GameData::getInstance()->getScore());
		enterNextStep();
	} else {
		UMengAnalytics::onFailed(ROOKIE_LEVEL_TIMETIP, GameData::getInstance()->getScore(), GameData::getInstance()->getTargetScore());
	}
}

void LevelTimeTipGuideController::enterMaskTime() {
	enterMask(Point(180, 48));

	if (nullptr != m_arrow) {
		m_arrow->setPosition(180, 90);
		m_arrow->setVisible(true);
		playArrowEffect();
	}

	if (nullptr != m_tipLabel) {
		m_tipLabel->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(s_tip_time));
		m_tipLabel->setPosition(180, m_arrow->getPositionY() + m_arrow->getContentSize().height);
		m_tipLabel->setVisible(true);
	}

	if (nullptr != m_petMatrix) {
		m_petMatrix->stopTouchEffect();
	}
}

void LevelTimeTipGuideController::enterMaskTip() {
	enterMask(Point(306, 48));

	if (nullptr != m_arrow) {
		m_arrow->setPosition(306, 90);
		m_arrow->setVisible(true);
		playArrowEffect();
	}

	if (nullptr != m_tipLabel) {
		m_tipLabel->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(s_tip_tip));
		m_tipLabel->setPosition(306, m_arrow->getPositionY() + m_arrow->getContentSize().height);
		m_tipLabel->setVisible(true);
	}

	if (nullptr != m_petMatrix) {
		m_petMatrix->stopTouchEffect();
	}
}

void LevelTimeTipGuideController::playArrowEffect() {
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

void LevelTimeTipGuideController::stopArrowEffect() {
	if (nullptr == m_arrow) {
		return;
	}

	m_arrow->stopAllActions();
}

void LevelTimeTipGuideController::onTouchTime() {
	enterNextStep();
}

void LevelTimeTipGuideController::onTouchTip() {
	enterNextStep();
}

void LevelTimeTipGuideController::nextGuide() {
	if (nullptr != m_currentStep 
		&& (m_currentStep->getStep() == Lev_Step_Time_Guide
		|| m_currentStep->getStep() == Lev_Step_Time_Manual
		|| m_currentStep->getStep() == Lev_Step_Tip_Guide)) {
		enterNextStep();
	}	
}

void LevelTimeTipGuideController::enterMask(const Point&pos) {
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

void LevelTimeTipGuideController::enterMask(std::vector<PetVec2>&mask) {
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

void LevelTimeTipGuideController::clearMask() {
	if (nullptr != m_mask) {
		m_layer->removeChild(m_mask, true);
		m_mask = nullptr;
	}
}

void LevelTimeTipGuideController::clearGuide() {
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

void LevelTimeTipGuideController::loadPetMatrix() {
	LevelController::loadPetMatrix(3);
}

bool LevelTimeTipGuideController::isManualStep() {
	if (nullptr != m_currentStep 
		&& (m_currentStep->getStep() == Lev_Step_Time_Manual
		|| m_currentStep->getStep() == Lev_Step_Tip_Manual)) {
		return true;
	}

	return false;
}