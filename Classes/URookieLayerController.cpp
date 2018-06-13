#include "URookieLayerController.h"
#include "GameData.h"
#include "GameManager.h"
#include "SettingManager.h"

#include "cocos2d.h"
USING_NS_CC;

URookieLayerController::URookieLayerController():
m_rookie(NULL),
m_happy(NULL),
m_type(Rookie_xinshou),
m_baofa(NULL)
{

}

URookieLayerController::~URookieLayerController() {

}

void URookieLayerController::initial() {
	if (NULL == m_layer) {
		return;
	}

	m_baofa = m_layer->getChildByName("bg_mid_1")
							->getChildByName("bg_top")
							->getChildByName("label_baofa");

	m_rookie = dynamic_cast<Sprite*>(m_layer->getChildByName("bg_mid_1")
											->getChildByName("bg_top")
											->getChildByName("bg_title")
											->getChildByName("rookie"));

	m_happy = dynamic_cast<Sprite*>(m_layer->getChildByName("bg_mid_1")
											->getChildByName("bg_top")
											->getChildByName("bg_title")
											->getChildByName("happy"));

	auto yesBtn = dynamic_cast<Button*>(m_layer->getChildByName("bg_mid_1")
		->getChildByName("bg_mid_2")
		->getChildByName("bg_bottom")
		->getChildByName("Btn_yes"));
	if (nullptr != yesBtn) {
		yesBtn->addClickEventListener([=](Ref*ref){
			Audio::getInstance()->playClick();
			exit();

			if (Rookie_xinshou == m_type) {
				startPayment(Setting_rookie);
			} else {
				startPayment(Setting_happy);
			}
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

void URookieLayerController::enter() {
	CenterLayerController::enter();
}

void URookieLayerController::exit() {
	CenterLayerController::exit();
}

void URookieLayerController::enterRookie() {
	if (nullptr != m_rookie) {
		m_rookie->setVisible(true);
	}

	if (nullptr != m_happy) {
		m_happy->setVisible(false);
	}

	if (nullptr != m_baofa) {
		m_baofa->setVisible(false);
	}

	m_type = Rookie_xinshou;
	enter();
}

void URookieLayerController::enterHappy() {
	if (nullptr != m_rookie) {
		m_rookie->setVisible(false);
	}

	if (nullptr != m_happy) {
		m_happy->setVisible(true);
	}

	if (nullptr != m_baofa) {
		m_baofa->setVisible(true);
	}

	m_type = Rookie_huanle;
	enter();
}

void URookieLayerController::onPayRookieComplete(bool success) {
	if (success) {
		GameData::getInstance()->addToolCross(3);
		GameData::getInstance()->addToolBomb(3);
		GameData::getInstance()->addToolTime(3);
		GameData::getInstance()->addToolTip(3);
		Audio::getInstance()->playBuy();
	}

	if (GameManager::getInstance()->isPaused()) {
		GameManager::getInstance()->resume();
	}
}

void URookieLayerController::onPayHappyComplete(bool success) {
	if (success) {
		GameData::getInstance()->addToolCross(3);
		GameData::getInstance()->addToolBomb(3);
		GameData::getInstance()->addToolTime(3);
		GameData::getInstance()->addToolTip(3);
		Audio::getInstance()->playBuy();
	}

	if (GameManager::getInstance()->isPaused()) {
		GameManager::getInstance()->resume();
	}
}