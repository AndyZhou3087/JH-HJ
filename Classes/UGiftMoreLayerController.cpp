#include "UGiftMoreLayerController.h"
#include "GameData.h"
#include "GameManager.h"
#include "LuckyTimeManager.h"
#include "cocos2d.h"
USING_NS_CC;

UGiftMoreLayerController::UGiftMoreLayerController():
m_callback(nullptr)
{

}

UGiftMoreLayerController::~UGiftMoreLayerController() {

}

void UGiftMoreLayerController::initial() {
	if (NULL == m_layer) {
		return;
	}

	auto closeBtn = dynamic_cast<Button*>(m_layer->getChildByName("bg_mid")
												->getChildByName("bg_top")
												->getChildByName("Btn_close"));

	if (nullptr != closeBtn) {
		closeBtn->addClickEventListener([=](Ref*ref){
			Audio::getInstance()->playClick();
			exit();	

			if (nullptr != m_callback) {
				m_callback();
			}
		});
	}

	auto yesBtn = dynamic_cast<Button*>(m_layer->getChildByName("bg_mid")
												->getChildByName("bg_bottom")
												->getChildByName("Btn_yes"));
	if (nullptr != yesBtn) {
		yesBtn->addClickEventListener([=](Ref*ref){
			Audio::getInstance()->playClick();
			exit();
			startPayment(Setting_giftmore);
		});

		m_effectNode1 = yesBtn;
	}
}

void UGiftMoreLayerController::onPayGiftMoreComplete(bool success) {
	if (success) {
		Audio::getInstance()->playBuy();
		GameData::getInstance()->addToolTip(3);
		GameData::getInstance()->addToolTime(3);
	}

	if (nullptr != m_callback) {
		m_callback();
	}
}