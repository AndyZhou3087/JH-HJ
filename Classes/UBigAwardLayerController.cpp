#include "UBigAwardLayerController.h"
#include "GameData.h"
#include "GameManager.h"
#include "cocos2d.h"
#include "SettingManager.h"
USING_NS_CC;

UBigAwardLayerController::UBigAwardLayerController()
{

}

UBigAwardLayerController::~UBigAwardLayerController() {

}

void UBigAwardLayerController::initial() {
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
												->getChildByName("bg_mid_3")
												->getChildByName("bg_bottom")
												->getChildByName("Btn_yes"));
	if (nullptr != yesBtn) {
		yesBtn->addClickEventListener([=](Ref*ref){
			Audio::getInstance()->playClick();
			exit();
			startPayment(Setting_biglibao);
		});

		m_effectNode1 = yesBtn;
	}
}

void UBigAwardLayerController::enter() {
	CenterLayerController::enter();
	Audio::getInstance()->playBigAward();
}

void UBigAwardLayerController::exit() {
	CenterLayerController::exit();
}

void UBigAwardLayerController::onPayZunXiangComplete(bool success) {
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