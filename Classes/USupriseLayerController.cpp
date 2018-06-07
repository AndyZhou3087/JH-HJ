#include "USupriseLayerController.h"
#include "GameData.h"
#include "GameManager.h"
#include "SettingManager.h"
#include "cocos2d.h"
USING_NS_CC;

USupriseLayerController::USupriseLayerController()
{

}

USupriseLayerController::~USupriseLayerController() {

}

void USupriseLayerController::initial() {
	if (NULL == m_layer) {
		return;
	}


	auto yesBtn = dynamic_cast<Button*>(m_layer->getChildByName("bg_mid_1")
		->getChildByName("bg_mid_2")
		->getChildByName("bg_bottom")
		->getChildByName("Btn_yes"));
	if (nullptr != yesBtn) {
		yesBtn->addClickEventListener([=](Ref*ref){
			Audio::getInstance()->playClick();
			exit();
			startPayment(Setting_suprise);
		});

		m_effectNode1 = yesBtn;
	}

	auto closeBtn = dynamic_cast<Button*>(m_layer->getChildByName("bg_mid_1")
		->getChildByName("bg_top")
		->getChildByName("Btn_close"));
	if (nullptr != closeBtn) {
		closeBtn->addClickEventListener([=](Ref*ref){
			Audio::getInstance()->playClick();
			exit();

			if (GameManager::getInstance()->isPaused()) {
				GameManager::getInstance()->resume();
			}
		});
	}
}

void USupriseLayerController::onPaySupriseBoxComplete(bool success) {
	if (success) {
		GameData::getInstance()->addToolTime(5);
		GameData::getInstance()->addToolTip(5);
		GameData::getInstance()->addEnergy(25);
		Audio::getInstance()->playBuy();
	} 

	if (GameManager::getInstance()->isStarted() && GameManager::getInstance()->isPaused()) {
		GameManager::getInstance()->resume();
	}
}