#include "UHappyLayerController.h"
#include "GameData.h"
#include "GameManager.h"
#include "SettingManager.h"

#include "cocos2d.h"
USING_NS_CC;

UHappyLayerController::UHappyLayerController()
{

}

UHappyLayerController::~UHappyLayerController() {

}

void UHappyLayerController::initial() {
	exit();
}

void UHappyLayerController::enter() {
	if (isEnter()) {
		return;
	}

	enterBg();
	m_bEnter = true;
	startPayment(Setting_happy);
}

void UHappyLayerController::exit() {
	exitBg();
	m_bEnter = false;
}


void UHappyLayerController::onPayHappyComplete(bool success) {
	if (success) {
		GameData::getInstance()->addToolCross(3);
		GameData::getInstance()->addToolBomb(3);
		GameData::getInstance()->addToolTime(3);
		GameData::getInstance()->addToolTip(3);
		Audio::getInstance()->playBuy();
	}

	exit();
	if (GameManager::getInstance()->isPaused()) {
		GameManager::getInstance()->resume();
	}
}