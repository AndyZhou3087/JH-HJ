#include "GameEnergyTipLayerController.h"
#include "GameData.h"
#include "GameManager.h"
#include "GameLevelLayerController.h"
#include "SettingManager.h"
#include "cocos2d.h"
USING_NS_CC;

GameEnergyTipLayerController::GameEnergyTipLayerController():
m_type(EnergyTip_this)
{

}

GameEnergyTipLayerController::~GameEnergyTipLayerController() {

}

void GameEnergyTipLayerController::initial() {
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
			startPayment(Setting_addenergy);
			//GameManager::getInstance()->ready(GameData::getInstance()->getLevel());
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

			if (nullptr != m_exitLayer) {
				m_exitLayer->enter();
			}
		});
	}
}

void GameEnergyTipLayerController::enter() {
	if (NULL == m_layer) {
		return;
	}
	m_type = EnergyTip_this;
	enterBg();
	const Size size = Director::getInstance()->getVisibleSize();
	m_layer->ignoreAnchorPointForPosition(false);
	m_layer->setAnchorPoint(Vec2(0.5,0.5));
	m_layer->setPosition(Vec2(size.width * 0.5, size.height * 0.5));

	m_bEnter = true;
	playEffectNode();
}

void GameEnergyTipLayerController::enter(EnumEnergyTipType type) {
	enter();
	m_type = type;
}

void GameEnergyTipLayerController::exit() {
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

void GameEnergyTipLayerController::onPayAddEnergyComplete(bool success) {
	if (success) {
		GameData::getInstance()->addEnergy(25);
		Audio::getInstance()->playBuy();
		if (EnergyTip_this == m_type) {
			GameManager::getInstance()->ready(GameData::getInstance()->getLevel());
		} else {
			GameManager::getInstance()->ready(GameData::getInstance()->getLevel() + 1);
		}	
	}
}