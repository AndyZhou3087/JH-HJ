#include "GameBottomLayerController.h"
#include "GameData.h"
#include "GameManager.h"
#include "GamePurchaseLayerController.h"
#include "GameLayerController.h"
#include "GameUtil.h"
#include "HitManager.h"

#define CHECK_HIT_TAG "checkHit"

GameBottomLayerController::GameBottomLayerController():
m_cross(NULL),
m_time(NULL),
m_tip(NULL),
m_bomb(NULL),
m_cross_num(NULL),
m_time_num(NULL),
m_tip_num(NULL),
m_bomb_num(NULL),
m_bSelectCross(false),
m_crossPosY(0),
m_purchaseLayer(NULL),
m_gameLayer(NULL)
{

}

GameBottomLayerController::~GameBottomLayerController() {

}

void GameBottomLayerController::initial() {
	if (NULL == m_layer) {
		return;
	}

	m_cross = dynamic_cast<Button*>(m_layer->getChildByName("Panel_1")->getChildByName("Btn_cross"));
	if (nullptr != m_cross) {
		m_crossPosY = m_cross->getPositionY();
		m_cross->addClickEventListener([=](Ref*ref){
			Audio::getInstance()->playClick();
			onClickCross();		
		});
	}

	m_cross_num = dynamic_cast<TextAtlas*>(m_layer->getChildByName("Panel_1")
													->getChildByName("bg_num_cross")
													->getChildByName("num_cross"));

	m_time = dynamic_cast<Button*>(m_layer->getChildByName("Panel_1")->getChildByName("Btn_time"));
	if (nullptr != m_time) {
		m_time->addClickEventListener([=](Ref*ref){
			Audio::getInstance()->playClick();
			onClickTime();		
		});
	}

	m_time_num = dynamic_cast<TextAtlas*>(m_layer->getChildByName("Panel_1")
													->getChildByName("bg_num_time")
													->getChildByName("num_time"));

	m_tip = dynamic_cast<Button*>(m_layer->getChildByName("Panel_1")->getChildByName("Btn_tip"));
	if (nullptr != m_tip) {
		m_tip->addClickEventListener([=](Ref*ref){
			Audio::getInstance()->playClick();
			onClickTip();		
		});
	}

	m_tip_num = dynamic_cast<TextAtlas*>(m_layer->getChildByName("Panel_1")
												->getChildByName("bg_num_tip")
												->getChildByName("num_tip"));

	m_bomb = dynamic_cast<Button*>(m_layer->getChildByName("Panel_1")->getChildByName("Btn_bomb"));
	if (nullptr != m_bomb) {
		m_bomb->addClickEventListener([=](Ref*ref){
			Audio::getInstance()->playClick();
			onClickBomb();		
		});
	}

	m_bomb_num = dynamic_cast<TextAtlas*>(m_layer->getChildByName("Panel_1")
													->getChildByName("bg_num_bomb")
													->getChildByName("num_bomb"));

	updateInfo();
}

void GameBottomLayerController::enter() {
	if (NULL == m_layer) {
		return;
	}

	static const Size size = Director::getInstance()->getVisibleSize();
	m_layer->ignoreAnchorPointForPosition(false);
	m_layer->setAnchorPoint(Vec2(0.5,0));
	m_layer->setPosition(Vec2(size.width * 0.5, 0));
	m_bEnter = true;

	updateInfo();
	updateBeat(0);
}

void GameBottomLayerController::exit() {

}

void GameBottomLayerController::onGameToolChanged() {
	updateInfo();
}

void GameBottomLayerController::updateInfo() {
	if (nullptr != m_cross_num) {
		m_cross_num->setString(String::createWithFormat("%d", GameData::getInstance()->getToolCrossNum())->_string);
	}

	if (nullptr != m_time_num) {
		m_time_num->setString(String::createWithFormat("%d", GameData::getInstance()->getToolTimeNum())->_string);
	}

	if (nullptr != m_tip_num) {
		m_tip_num->setString(String::createWithFormat("%d", GameData::getInstance()->getToolTipNum())->_string);
	}

	if (nullptr != m_bomb_num) {
		m_bomb_num->setString(String::createWithFormat("%d", GameData::getInstance()->getToolBombNum())->_string);
	}
}

void GameBottomLayerController::onClickCross() {
	if (nullptr == m_cross 
		|| !GameManager::getInstance()->isStarted()
		|| GameManager::getInstance()->isPaused()
		|| GameManager::getInstance()->isOver()) {
			return;
	}

	if (nullptr != m_gameLayer) {
		m_gameLayer->usingCross();
	}

	if (m_bSelectCross) {
		m_bSelectCross = false;
		m_cross->stopAllActions();
		m_cross->setPositionY(m_crossPosY);
		GameManager::getInstance()->selectCross(false);
		return;
	}

	if (GameData::getInstance()->getToolCrossNum() > 0) {
		m_cross->runAction(RepeatForever::create(
			Sequence::create(
			MoveBy::create(0.5, Vec2(0,-20)),
			MoveBy::create(0.5, Vec2(0,20)),
			NULL)
			));
		GameData::getInstance()->addToolCross(-1);
		GameManager::getInstance()->selectCross(true);
		m_bSelectCross = true;
	} else {	
		if (nullptr != m_purchaseLayer) {
			m_purchaseLayer->purchase(Tool_Cross);
		}
	}
	
}

void GameBottomLayerController::onClickTime() {
	if (nullptr == m_time 
		|| !GameManager::getInstance()->isStarted()
		|| GameManager::getInstance()->isPaused()
		|| GameManager::getInstance()->isOver()) {
			return;
	}

	if (nullptr != m_gameLayer) {
		m_gameLayer->usingTime();
	}

	if (GameData::getInstance()->getToolTimeNum() > 0) {
		GameData::getInstance()->addToolTime(-1);
		GameData::getInstance()->addTime(s_time_add);
	} else {
		if (nullptr != m_purchaseLayer) {
			m_purchaseLayer->purchase(Tool_Time);
		}
	}
}

void GameBottomLayerController::onClickTip() {
	if (nullptr == m_tip 
		|| !GameManager::getInstance()->isStarted()
		|| GameManager::getInstance()->isPaused()
		|| GameManager::getInstance()->isOver()) {
			return;
	}

	if (GameData::getInstance()->getToolTipNum() > 0) {
		if (nullptr != m_gameLayer && m_gameLayer->usingTip()) {
			GameData::getInstance()->addToolTip(-1);
		}	
	} else {
		if (nullptr != m_purchaseLayer) {
			m_purchaseLayer->purchase(Tool_Tip);
		}
	}
}

void GameBottomLayerController::onClickBomb() {
	if (nullptr == m_bomb 
		|| !GameManager::getInstance()->isStarted()
		|| GameManager::getInstance()->isPaused()
		|| GameManager::getInstance()->isOver()) {
		return;
	}

	if (GameData::getInstance()->getToolBombNum() > 0) {

		if (nullptr != m_gameLayer && m_gameLayer->usingBomb()) {
			GameData::getInstance()->addToolBomb(-1);
		}
	} else {
		if (nullptr != m_purchaseLayer) {
			m_purchaseLayer->purchase(Tool_Bomb);
		}
	}
}

void GameBottomLayerController::onGameReady() {
	updateBeat(0);
}

void GameBottomLayerController::onGameStart() {
	if (nullptr != m_layer) {
		m_layer->unschedule(CHECK_HIT_TAG);
		m_layer->schedule([=](float){ updateBeat(HitManager::getInstance()->getHit()); }, 0.2, CHECK_HIT_TAG);
	}
}

void GameBottomLayerController::onGameRestart() {
	onGameStart();
}

void GameBottomLayerController::onGameRelive() {
	onGameStart();
}

void GameBottomLayerController::onGameOver() {
	releaseCross();
	m_layer->unschedule(CHECK_HIT_TAG);
}

void GameBottomLayerController::releaseCross() {
	if (nullptr != m_cross) {
		m_cross->stopAllActions();
		m_cross->setPositionY(m_crossPosY);
		m_bSelectCross = false;
	}
}

void GameBottomLayerController::tipBomb() {
	if (nullptr != m_bomb) {
		GameUtil::jump(m_bomb, 20);
	}
}

void GameBottomLayerController::tipTip() {
	if (nullptr != m_tip) {
		GameUtil::jump(m_tip, 20);
	}
}

void GameBottomLayerController::updateBeat(int beatCount) {
	if (nullptr == m_layer) {
		return;
	}

	const int maxBeat = 7;
	int i=0;
	for (; i<beatCount && i<maxBeat; i++) {
		auto beatSprite = dynamic_cast<Sprite*>(m_layer->getChildByName("Panel_1")->getChildByName("bg_beat")->getChildByName(String::createWithFormat("beat_light_%d", i+1)->_string));
		if (nullptr != beatSprite) {
			beatSprite->setVisible(true);
		}
	}

	for (; i<maxBeat; i++) {
		auto beatSprite = dynamic_cast<Sprite*>(m_layer->getChildByName("Panel_1")->getChildByName("bg_beat")->getChildByName(String::createWithFormat("beat_light_%d", i+1)->_string));
		if (nullptr != beatSprite) {
			beatSprite->setVisible(false);
		}
	}
}
