#include "GameBuyTimeLayerController.h"
#include "GameData.h"
#include "GameManager.h"
#include "GameLevelLayerController.h"
#include "SettingManager.h"
#include "cocos2d.h"
USING_NS_CC;

GameBuyTimeLayerController::GameBuyTimeLayerController()
{

}

GameBuyTimeLayerController::~GameBuyTimeLayerController() {

}

void GameBuyTimeLayerController::initial() {
	if (NULL == m_layer) {
		return;
	}

	auto yesBtn = dynamic_cast<Button*>(m_layer->getChildByName("bg_mid")
												->getChildByName("bg_bottom")
												->getChildByName("Btn_yes"));
	if (nullptr != yesBtn) {
		yesBtn->addClickEventListener([=](Ref*ref){
			Audio::getInstance()->playClick();
			exit();
			startPayment(Setting_addtime);
		});

		m_effectNode1 = yesBtn;
	}

	auto closeBtn = dynamic_cast<Button*>(m_layer->getChildByName("bg_mid")
													->getChildByName("bg_top")
													->getChildByName("Btn_close"));
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

void GameBuyTimeLayerController::enter() {
	if (NULL == m_layer) {
		return;
	}

	enterBg();
	const Size size = Director::getInstance()->getVisibleSize();
	m_layer->ignoreAnchorPointForPosition(false);
	m_layer->setAnchorPoint(Vec2(0.5,0.5));
	m_layer->setPosition(Vec2(size.width * 0.5, size.height * 0.5));

	m_bEnter = true;

	Audio::getInstance()->playTimeup();
	playEffectNode();
}

void GameBuyTimeLayerController::exit() {
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

void GameBuyTimeLayerController::onPayAddTimeComplete(bool success) {
	if (success) {
		Audio::getInstance()->playBuy();
		GameData::getInstance()->addToolTime(4);
		GameData::getInstance()->addTime(s_time_add);
		GameManager::getInstance()->relive();
		if (GameManager::getInstance()->isStarted() && GameManager::getInstance()->isPaused()) {
			GameManager::getInstance()->resume();
		}
	} else {
		if (nullptr != m_nextLayer) {
			m_nextLayer->enter();
		}
	}
}