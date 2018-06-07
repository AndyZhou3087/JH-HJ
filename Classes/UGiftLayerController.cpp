#include "UGiftLayerController.h"
#include "GameData.h"
#include "GameManager.h"
#include "GameUtil.h"
#include "cocos2d.h"
#include "SettingManager.h"
USING_NS_CC;

UGiftLayerController::UGiftLayerController():
m_closeBtn(NULL),
m_freeTip(NULL),
m_disfreeTip(NULL),
m_disfreePayTip(NULL),
m_bFree(true),
m_tipNum(NULL)
{

}

UGiftLayerController::~UGiftLayerController() {

}

void UGiftLayerController::initial() {
	if (NULL == m_layer) {
		return;
	}

	initialFrame();
}

void UGiftLayerController::initialFrameless() {
}

void UGiftLayerController::initialFrame() {
	auto yesBtn = dynamic_cast<Button*>(m_layer->getChildByName("Btn_yes"));
	if (nullptr != yesBtn) {
		yesBtn->addClickEventListener([=](Ref*ref){
			Audio::getInstance()->playClick();
			if (m_bFree) {
				GameData::getInstance()->addToolTip(1);
				if (2 == SettingManager::getInstance()->getZunxiangSetting()) {
					enterDisfree();
				} else {
					exit();

					if (GameManager::getInstance()->isPaused()) {
						GameManager::getInstance()->resume();
					}

					if (nullptr != m_nextLayer) {
						m_nextLayer->enter();
					}
				}	
			} else {
				exit();
				startPayment(Setting_biglihe);
			}	
		});

		m_effectNode1 = yesBtn;
	}

	m_closeBtn = dynamic_cast<Button*>(m_layer->getChildByName("Btn_close"));
	if (nullptr != m_closeBtn) {
		m_closeBtn->addClickEventListener([=](Ref*ref){
			Audio::getInstance()->playClick();
			exit();

			if (GameManager::getInstance()->isPaused()) {
				GameManager::getInstance()->resume();
			}

			if (nullptr != m_nextLayer) {
				m_nextLayer->enter();
			}
		});
	}

	m_tipNum		= dynamic_cast<TextAtlas*>(m_layer->getChildByName("bg_tip")->getChildByName("num"));
	m_freeTip		= (m_layer->getChildByName("free"));
	m_disfreeTip	= (m_layer->getChildByName("disfree"));
	m_disfreePayTip = (m_layer->getChildByName("tip_disfree"));
}

void UGiftLayerController::enter() {
	CenterLayerController::enter();
	if (GameUtil::getRandomBoolean()) {
		enterFree();
	} else {
		if (1 == SettingManager::getInstance()->getZhizunSetting()
			|| 2 == SettingManager::getInstance()->getZhizunSetting()) {
			enterDisfree();
		} else {
			exit();
			if (nullptr != m_nextLayer) {
				m_nextLayer->enter();
			}
		}
	}
}

void UGiftLayerController::exit() {
	CenterLayerController::exit();
}

void UGiftLayerController::enterFree() {
	if (nullptr != m_closeBtn) {
		m_closeBtn->setVisible(false);
	}

	if (nullptr != m_freeTip) {
		m_freeTip->setVisible(true);
	}

	if (nullptr != m_disfreeTip) {
		m_disfreeTip->setVisible(false);
	}

	if (nullptr != m_disfreePayTip) {
		m_disfreePayTip->setVisible(false);
	}

	if (nullptr != m_tipNum) {
		m_tipNum->setString(String::createWithFormat(".1")->_string);
	}

	m_bFree = true;
}

void UGiftLayerController::enterDisfree() {
	if (nullptr != m_closeBtn) {
		m_closeBtn->setVisible(true);
	}

	if (nullptr != m_freeTip) {
		m_freeTip->setVisible(false);
	}

	if (nullptr != m_disfreeTip) {
		m_disfreeTip->setVisible(true);
	}

	if (nullptr != m_disfreePayTip) {
		m_disfreePayTip->setVisible(true);
	}

	if (nullptr != m_tipNum) {
		m_tipNum->setString(String::createWithFormat(".5")->_string);
	}

	m_bFree = false;
}

void UGiftLayerController::onPayZhiZunComplete(bool success) {
	if (success) {
		GameData::getInstance()->addToolTip(5);
		Audio::getInstance()->playBuy();
	}

	if (nullptr != m_nextLayer) {
		m_nextLayer->enter();
	}
}