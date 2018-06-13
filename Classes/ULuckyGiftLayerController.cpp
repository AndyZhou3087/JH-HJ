#include "ULuckyGiftLayerController.h"
#include "GameData.h"
#include "GameManager.h"
#include "cocos2d.h"
#include "SettingManager.h"
USING_NS_CC;

ULuckyGiftLayerController::ULuckyGiftLayerController()
{

}

ULuckyGiftLayerController::~ULuckyGiftLayerController() {

}

void ULuckyGiftLayerController::initial() {
	if (NULL == m_layer) {
		return;
	}

	auto closeBtn = dynamic_cast<Button*>(m_layer->getChildByName("bg_mid_1")
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

	auto yesBtn = dynamic_cast<Button*>(m_layer->getChildByName("bg_mid_1")
		->getChildByName("bg_mid_2")
		->getChildByName("bg_bottom")
		->getChildByName("Btn_yes"));
	if (nullptr != yesBtn) {
		yesBtn->addClickEventListener([=](Ref*ref){
			Audio::getInstance()->playClick();
			exit();
			startPayment(Setting_luckylottery);
		});

		m_effectNode1 = yesBtn;
	}
}

void ULuckyGiftLayerController::onPayLuckyComplete(bool success) {
	if (success) {
		GameData::getInstance()->addToolCross(5);
		GameData::getInstance()->addToolBomb(3);
		GameData::getInstance()->addToolTime(5);
		GameData::getInstance()->addToolTip(3);
		GameData::getInstance()->addEnergy(10);
		Audio::getInstance()->playBuy();
	}

	if (nullptr != m_nextLayer) {
		m_nextLayer->enter();
	}
}

void ULuckyGiftLayerController::enter() {
	CenterLayerController::enter();
}

void ULuckyGiftLayerController::exit() {
	CenterLayerController::exit();
}