#include "LevelController.h"
#include "GameUiManager.h"
#include "GameTargetLayerController.h"
#include "GameTopLayerController.h"
#include "UTipWinLayerController.h"
#include "GameBossLayerController.h"
#include "GameEvent.h"
#include "Resource.h"
#include "LevelStep.h"
#include "DemoPetData.h"
#include "DemoPetMatrix.h"
#include "UserData.h"
#include "LevelStepTouchErr.h"
#include "GameData.h"

#define DEMO_Z_ORDER -10
#define TOUCH_Z_ORDER	12
#define MASK_Z_ORDER	10
#define TIP_Z_ORDER		11

LevelController::LevelController(Node *layer):
m_layer(layer),
m_currentStep(nullptr),
m_eventLis(nullptr),
m_redTip(nullptr),
m_petMatrix(nullptr),
m_bClearAlling(false),
m_stepTouchErr(nullptr),
m_arrow(nullptr),
m_tipLabel(nullptr)
{

}

LevelController::~LevelController() {
	if (nullptr != m_eventLis && nullptr != m_layer) {
		m_layer->getEventDispatcher()->removeEventListener(m_eventLis);
	}
}

bool LevelController::init() {
	m_redTip = Sprite::create(s_tip_red);
	m_layer->addChild(m_redTip);
	m_redTip->setVisible(false);

	initTouchLayer();
	return true;
}

void LevelController::enter() {

}

void LevelController::exit() {
	unloadPetMatrix();
	finish();
}

void LevelController::onGameReady() {
	auto top = GameUiManager::getInstance()->getTopLayerController();
	if (nullptr != top) {
		top->enter();
	}
	
	auto boss = GameUiManager::getInstance()->getBossLayerController();
	if (nullptr != boss) {
		boss->exit();
	}
}

void LevelController::onGameFinish() {
	finish();
}

void LevelController::showToolConfirm() {
}

void LevelController::showTarget() {
	auto target = GameUiManager::getInstance()->getTargetLayerController();
	if (nullptr != target) {
		target->enter();
	}
}

void LevelController::showTargetBoss() {
	auto target = GameUiManager::getInstance()->getTargetBossLayerController();
	if (nullptr != target) {
		target->enter();
	}
}

void LevelController::enterManual() {

}

void LevelController::enterTouchErr() {
	stopTime();
	const int tipNum = UserData::getTouchErrTipNum();
	if (tipNum > 3) {
		startTime();
		enterNextStep();
		return;
	}

	auto touchErr = GameUiManager::getInstance()->getTouchErrLayer();
	if (nullptr != touchErr) {
		auto yesBtn = dynamic_cast<Button*>(touchErr->getChildByName("bg_panel")->getChildByName("Btn_yes"));
		if (nullptr != yesBtn) {
			yesBtn->addClickEventListener([=](Ref*ref){
				Audio::getInstance()->playClick();
				GameUiManager::getInstance()->showLayer(touchErr, false);
				startTime();
				enterNextStep();
			});
		}

		GameUiManager::getInstance()->showLayer(touchErr, true);	
		UserData::setTouchErrTipNum(tipNum + 1);
	}
}

void LevelController::showWinTip() {
	auto winTip = GameUiManager::getInstance()->getTipWinLayerController();
	if (nullptr != winTip) {
		winTip->enter();
		if (nullptr != m_layer) {
			m_layer->runAction(Sequence::create(
					DelayTime::create(2),
					CallFunc::create([=](){
						winTip->exit();
						onTipWinCompleted();
				}),NULL));
		}
	}
}

void LevelController::initTouchLayer() {
	m_eventLis = EventListenerTouchOneByOne::create();
	m_eventLis->onTouchBegan = [this](Touch * touch,Event *e)	
	{	
		return onTouchBegan(touch,e);
	};
	m_eventLis->setSwallowTouches(true);
	m_layer->getEventDispatcher()->addEventListenerWithSceneGraphPriority(m_eventLis, m_layer);
}

bool LevelController::onTouchBegan(Touch * touch,Event *e) {
	return true;
}

void LevelController::onTipWinCompleted() {
	GameEvent::throwEvent(Event_Game_Win, NULL);
}

void LevelController::startTime() {
	auto top = GameUiManager::getInstance()->getTopLayerController();
	if (nullptr != top) {
		top->startTime();
	}

	auto boss = GameUiManager::getInstance()->getBossLayerController();
	if (nullptr != boss) {
		boss->startTime();
	}
}

void LevelController::stopTime() {
	auto top = GameUiManager::getInstance()->getTopLayerController();
	if (nullptr != top) {
		top->stopTime();
	}

	auto boss = GameUiManager::getInstance()->getBossLayerController();
	if (nullptr != boss) {
		boss->stopTime();
	}
}

void LevelController::enterNextStep() {
	if (nullptr != m_currentStep) {
		m_currentStep->exit();
		m_currentStep = m_currentStep->getNextStep();
	}
}

void LevelController::loadPetMatrix(int level) {
	LevelDataManager::getInstance()->getLevelData(m_levelData, level);
	loadPetMatrix(m_levelData);
}

void LevelController::loadPetMatrix(const LevelData&data) {
	if (nullptr == m_layer) {
		return;
	}

	m_petMatrix = DemoPetMatrix::createPetMatrix(data);
	m_layer->addChild(m_petMatrix);
	m_petMatrix->setPosition(20, 184);

	if (nullptr != m_redTip) {
		m_redTip->setZOrder(m_petMatrix->getZOrder() + 1);
		m_redTip->setPosition(m_petMatrix->getPositionX() + Pet::getWidth() * PetMatrix::COL_NUM * 0.5,
			m_petMatrix->getPositionY() + Pet::getHeight() * PetMatrix::ROW_NUM * 0.5);
	}
}

void LevelController::unloadPetMatrix() {
	if (nullptr == m_layer || nullptr == m_petMatrix) {
		return;
	}

	m_petMatrix->stopTouchEffect();
	m_layer->removeChild(m_petMatrix, true);
	m_petMatrix = nullptr;
}

Sprite*  LevelController::createMaskedSprite(std::vector<PetVec2>&mask) 
{ 
	const Size size = Director::getInstance()->getVisibleSize();
	CCRenderTexture * rt = CCRenderTexture::create(size.width, size.height); 

	auto src = Sprite::create(s_bg_grey);
	float scaleRate = size.height/src->getContentSize().height;
	src->setScaleY(scaleRate);
	src->setAnchorPoint(Vec2(0.5, 0));
	src->setPosition(size.width * 0.5, 0);
	ccBlendFunc blendFunc2 = { GL_ONE, GL_ZERO }; 
	src->setBlendFunc(blendFunc2);

	Vector<Sprite*> vMaskPets;
	for (int i=0; i<mask.size(); i++) {
		PetVec2 demoMaskPet = mask.at(i);
		if (demoMaskPet.iCol <0 || demoMaskPet.iCol >= PetMatrix::COL_NUM
			|| demoMaskPet.iRow <0 || demoMaskPet.iRow >= PetMatrix::ROW_NUM) {
				continue;
		}

		auto maskPet = Sprite::create("mask_74.png");
		maskPet->setAnchorPoint(Vec2(0.5, 0.5));

		maskPet->setPosition(20 + (demoMaskPet.iCol + 0.5)*Pet::getWidth(), 
			(184 + (PetMatrix::ROW_NUM - 1 - demoMaskPet.iRow + 0.5) * Pet::getHeight()));

		ccBlendFunc blendFunc3 = { GL_ZERO, GL_ONE_MINUS_SRC_ALPHA }; 
		maskPet->setBlendFunc(blendFunc3); 
		vMaskPets.pushBack(maskPet);
	}


	rt->begin(); 
	src->visit();
	for (auto maskPet:vMaskPets) {
		maskPet->visit();
	}

	rt->end(); 
	vMaskPets.clear();
	CCSprite * retval = CCSprite::createWithTexture(rt->getSprite()->getTexture()); 
	retval->setFlipY(true);
	return retval; 
} 

Sprite*  LevelController::createMaskedSprite(const char* maskFile, const Point&maskPos) 
{ 
	if (nullptr == maskFile) {
		return nullptr;
	}

	const Size size = Director::getInstance()->getVisibleSize();
	CCRenderTexture * rt = CCRenderTexture::create(size.width, size.height); 

	auto src = Sprite::create(s_bg_grey);
	float scaleRate = size.height/src->getContentSize().height;
	src->setScaleY(scaleRate);
	src->setAnchorPoint(Vec2(0.5, 0));
	src->setPosition(size.width * 0.5, 0);
	ccBlendFunc blendFunc2 = { GL_ONE, GL_ZERO }; 
	src->setBlendFunc(blendFunc2);


	auto mask = Sprite::create(maskFile);
	mask->setAnchorPoint(Vec2(0.5, 0.5));
	mask->setPosition(maskPos);
	ccBlendFunc blendFunc3 = { GL_ZERO, GL_ONE_MINUS_SRC_ALPHA }; 
	mask->setBlendFunc(blendFunc3); 

	rt->begin(); 
	src->visit();
	mask->visit();
	rt->end(); 

	CCSprite * retval = CCSprite::createWithTexture(rt->getSprite()->getTexture()); 
	retval->setFlipY(true);
	return retval; 
} 

bool LevelController::isTouchFocus(const cocos2d::Point&pos, PetVec2 v) {
	if (NULL == m_petMatrix) {
		return false;
	}

	PetVec2 touchVec2 = m_petMatrix->getTouchPetVec2(pos);
	if (touchVec2.iRow == v.iRow && touchVec2.iCol == v.iCol) {
		return true;
	}

	return false;
}

bool LevelController::usingBomb() {
	if (nullptr != m_petMatrix && m_petMatrix->bombClear()) {
		m_petMatrix->runAction(Sequence::create(
			DelayTime::create(0.5),
			CallFunc::create([=](){
				checkPetMatrix();
		}),NULL));
		return true;
	}

	return false;
}

bool LevelController::usingTip() {
	if (nullptr != m_petMatrix && m_petMatrix->tipPets()) {
		return true;
	}

	return false;
}

void LevelController::checkPetMatrix() {
	if (nullptr == m_petMatrix) {
		return;
	}

	if (!m_bClearAlling && !m_petMatrix->checkPets()) {
		m_bClearAlling = true;
		m_petMatrix->clearAllPet();

		m_layer->runAction(Sequence::create(
			DelayTime::create(1),
			CallFunc::create([=](){
				unloadPetMatrix();
				loadPetMatrix();
				if (nullptr != m_petMatrix) {
					m_petMatrix->enter();
				}
				m_bClearAlling = false;
		}),NULL));
	}
}

void LevelController::tipPets() {
	if (nullptr != m_petMatrix && isManualStep()) {
		m_petMatrix->tipPets();
	}
}

void LevelController::blinkRedTip() {
	if (nullptr == m_redTip) {
		return;
	}

	m_redTip->setVisible(true);
	m_redTip->setOpacity(255);
	m_redTip->runAction(FadeOut::create(1));
}

void LevelController::enterTouchErrStep() {
	if (nullptr != m_stepTouchErr) {
		m_currentStep = m_stepTouchErr;
		m_stepTouchErr->enter();
	}
}

bool LevelController::isBossLevel() {
	if (m_levelData.iBoss > 0) {
		return true;
	}

	return false;
}

void LevelController::enterPetMatrix() {
	if (nullptr != m_petMatrix) {
		m_petMatrix->enter();
	}
}

void LevelController::finish() {
	if (nullptr != m_currentStep) {
		m_currentStep->finish();
	}
}

void LevelController::createBombArrowTip() {
	if (nullptr == m_layer) {
		return;
	}

	m_tipLabel = Sprite::createWithSpriteFrameName(s_tip_bomb);
	m_layer->addChild(m_tipLabel, TIP_Z_ORDER);
	m_tipLabel->setAnchorPoint(Vec2(0.5, 0));
	m_tipLabel->setVisible(false);

	m_arrow = Sprite::createWithSpriteFrameName(s_icon_arrow2);
	m_layer->addChild(m_arrow, TIP_Z_ORDER);
	m_arrow->setAnchorPoint(Vec2(0.5,0));
	m_arrow->setVisible(false);
}

void LevelController::createCrossArrowTip() {
	if (nullptr == m_layer) {
		return;
	}

	m_tipLabel = Sprite::createWithSpriteFrameName(s_tip_cross);
	m_layer->addChild(m_tipLabel, TIP_Z_ORDER);
	m_tipLabel->setAnchorPoint(Vec2(0.5, 0));
	m_tipLabel->setVisible(false);

	m_arrow = Sprite::createWithSpriteFrameName(s_icon_arrow2);
	m_layer->addChild(m_arrow, TIP_Z_ORDER);
	m_arrow->setAnchorPoint(Vec2(0.5,0));
	m_arrow->setVisible(false);
}

void LevelController::hideArrowTip() {
	if (nullptr != m_arrow) {
		m_arrow->stopAllActions();
		m_arrow->setVisible(false);
	}

	if (nullptr != m_tipLabel) {
		m_tipLabel->setVisible(false);
	}
}

void LevelController::clearArrowTip() {
	if (nullptr == m_layer) {
		return;
	}

	if (nullptr != m_arrow) {
		m_arrow->stopAllActions();
		m_layer->removeChild(m_arrow, true);
		m_arrow = nullptr;
	}

	if (nullptr != m_tipLabel) {
		m_layer->removeChild(m_tipLabel, true);
		m_tipLabel = nullptr;
	}
}