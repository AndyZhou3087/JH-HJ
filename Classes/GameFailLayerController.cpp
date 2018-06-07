#include "GameFailLayerController.h"
#include "GameData.h"
#include "GameManager.h"
#include "GameData.h"
#include "GameEnergyTipLayerController.h"
#include "cocos2d.h"
#include "SettingManager.h"
#include "UUnlockLayerController.h"

USING_NS_CC;

GameFailLayerController::GameFailLayerController():
m_energytipLayer(NULL),
m_unlockLayer(NULL)
{

}

GameFailLayerController::~GameFailLayerController() {

}

void GameFailLayerController::initial() {
	if (NULL == m_layer) {
		return;
	}

	auto retryBtn = dynamic_cast<Button*>(m_layer->getChildByName("Btn_retry"));
	if (nullptr != retryBtn) {
		retryBtn->addClickEventListener([=](Ref*ref){
			Audio::getInstance()->playClick();
			exit();
			if (GameData::getInstance()->getEnergy() >= s_energy_use) {
				GameManager::getInstance()->restart();
			} else {
				if (nullptr != m_energytipLayer) {
					m_energytipLayer->enter();
				}
			}
		});
	}

	auto nextBtn = dynamic_cast<Button*>(m_layer->getChildByName("Btn_next"));
	if (nullptr != nextBtn) {
		nextBtn->addClickEventListener([=](Ref*ref){
			Audio::getInstance()->playClick();
			exit();
			startPayment(Setting_nextgame);
		});

		m_effectNode1 = nextBtn;
	}

	auto closeBtn = dynamic_cast<Button*>(m_layer->getChildByName("Btn_close"));
	if (nullptr != closeBtn) {
		closeBtn->addClickEventListener([=](Ref*ref){
			Audio::getInstance()->playClick();
			exit();

			if (nullptr != m_nextLayer) {
				m_nextLayer->enter();
			}
		});
	}
}

void GameFailLayerController::enter() {
	if (NULL == m_layer) {
		return;
	}
	enterBg();
	const Size size = Director::getInstance()->getVisibleSize();
	m_layer->ignoreAnchorPointForPosition(false);
	m_layer->setAnchorPoint(Vec2(0.5,0.5));
	m_layer->setPosition(Vec2(size.width * 0.5, size.height * 0.5));

	m_bEnter = true;
	Audio::getInstance()->playFail();
	playEffectNode();
}

void GameFailLayerController::exit() {
	if (NULL == m_layer) {
		return;
	}
	exitBg();
	const Size size = Director::getInstance()->getVisibleSize();
	m_layer->ignoreAnchorPointForPosition(false);
	m_layer->setAnchorPoint(Vec2(0.5,0.5));
	m_layer->setPositionX(size.width * 2);
	m_bEnter = false;
	stopEffectNode();
}

void GameFailLayerController::onPayPassGameComplete(bool success) {
	if (success) {
		GameData::getInstance()->setStar(GameData::getInstance()->getLevel(), 1);
		Audio::getInstance()->playBuy();
		if (!GameData::getInstance()->isLevelUnlock(GameData::getInstance()->getLevel() + 1)) {
			if (nullptr != m_unlockLayer) {
				m_unlockLayer->enter(GameData::getInstance()->getLevel() + 1);
			} else if (nullptr != m_nextLayer) {
				m_nextLayer->enter();
			}

		} else {
			GameData::getInstance()->setHisLevel(GameData::getInstance()->getLevel());
			GameManager::getInstance()->next();
		}
	} else {
		if (nullptr != m_nextLayer) {
			m_nextLayer->enter();
		}
	}
}