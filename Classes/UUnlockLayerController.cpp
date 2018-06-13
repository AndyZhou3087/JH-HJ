#include "UUnlockLayerController.h"
#include "GameData.h"
#include "GameManager.h"
#include "GameEvent.h"
#include "SettingManager.h"
#include "cocos2d.h"
USING_NS_CC;

UUnlockLayerController::UUnlockLayerController():
m_starNum(NULL),
m_panel_free(NULL),
m_panel_disfree(NULL),
m_pay_tip(NULL),
m_closeBtn(NULL),
m_yesButton(NULL),
m_unlockButton(NULL),
m_levelRequireUnlock(-1),
m_uiType(Ui_NotEnough_Star)
{

}

UUnlockLayerController::~UUnlockLayerController() {

}

void UUnlockLayerController::initial() {
	if (NULL == m_layer) {
		return;
	}

	m_closeBtn = dynamic_cast<Button*>(m_layer->getChildByName("bg_mid_1")
		->getChildByName("bg_top")
		->getChildByName("Btn_close"));
	if (nullptr != m_closeBtn) {
		m_closeBtn->addClickEventListener([=](Ref*ref){
			Audio::getInstance()->playClick();

			if (Ui_Enough_Star == m_uiType) {
				enterUiPayUnlock();
			} else if (Ui_Pay_Unlock == m_uiType) {
				exit();
				if (GameManager::getInstance()->isPaused()) {
					GameManager::getInstance()->resume();
				} else {
					if (nullptr != m_nextLayer) {
						m_nextLayer->enter();
					}
				}
			} else if (Ui_NotEnough_Star == m_uiType) {
				CCASSERT(false, "unlock ui error");
			}
		});
	}

	m_unlockButton = dynamic_cast<Button*>(m_layer->getChildByName("bg_mid_1")
														->getChildByName("bg_bottom")
														->getChildByName("Btn_unlock"));
	if (nullptr != m_unlockButton) {
		m_unlockButton->addClickEventListener([=](Ref*ref){
			Audio::getInstance()->playClick();
			exit();
			GameData::getInstance()->setLevelUnlock(m_levelRequireUnlock, true);
			GameEvent::throwEvent(Event_Level_Unlock, NULL);
			//ÐÇÐÇ½âËø
			if (GameManager::getInstance()->isPaused()) {
				GameManager::getInstance()->resume();
			} else {
				if (nullptr != m_nextLayer) {
					m_nextLayer->enter();
				}
			}
		});
	}

	m_yesButton = dynamic_cast<Button*>(m_layer->getChildByName("bg_mid_1")
														->getChildByName("bg_bottom")
														->getChildByName("Btn_yes"));
	if (nullptr != m_yesButton) {
		m_yesButton->addClickEventListener([=](Ref*ref){
			if (Ui_NotEnough_Star == m_uiType) {
				enterUiPayUnlock();
			} else if (Ui_Pay_Unlock == m_uiType) {
				Audio::getInstance()->playClick();
				exit();
				//¹ºÂò½âËø
				startPayment(Setting_unlock);
			} else {
				CCASSERT(false, "unlock ui error");
				exit();
			}	
		});

		m_effectNode1 = m_yesButton;
	}


	m_panel_free = m_layer->getChildByName("bg_mid_1")
						->getChildByName("bg_top")
						->getChildByName("panel_free");

	m_starNum = dynamic_cast<TextAtlas*>(m_layer->getChildByName("bg_mid_1")
												->getChildByName("bg_top")
												->getChildByName("panel_free")
												->getChildByName("num_star"));

	m_panel_disfree = m_layer->getChildByName("bg_mid_1")
							->getChildByName("bg_top")
							->getChildByName("panel_disfree");

	m_pay_tip = m_layer->getChildByName("bg_mid_1")
						->getChildByName("bg_bottom")
						->getChildByName("tip_pay");

}

void UUnlockLayerController::enter(int unlockLevel) {
	if (nullptr == m_starNum) {
		return;
	}

	m_levelRequireUnlock = unlockLevel;
	const int star = GameData::getInstance()->getTargetStar(unlockLevel);
	m_starNum->setString(String::createWithFormat("%d", star)->_string);

	if (GameData::getInstance()->getStarSum() >= star) {
		enterUiEnoughStar();
	} else {
		enterUiNotEnoughStar();
	}

	CenterLayerController::enter();
}

void UUnlockLayerController::onPayUnlockComplete(bool success) {
	if (success) {
		GameData::getInstance()->setLevelUnlock(m_levelRequireUnlock, true);
		GameEvent::throwEvent(Event_Level_Unlock, NULL);
		Audio::getInstance()->playBuy();

		if (GameManager::getInstance()->isPaused()) {
			GameManager::getInstance()->resume();
		} 
	}

	if (nullptr != m_nextLayer) {
		m_nextLayer->enter();
	}
}

void UUnlockLayerController::enterUiNotEnoughStar() {
	m_uiType = Ui_NotEnough_Star;
	if (nullptr != m_closeBtn) {
		m_closeBtn->setVisible(false);
	}

	if (nullptr != m_panel_free) {
		m_panel_free->setVisible(true);
	}

	if (nullptr != m_panel_disfree) {
		m_panel_disfree->setVisible(false);
	}

	if (nullptr != m_yesButton) {
		m_yesButton->setVisible(true);
	}

	if (nullptr != m_unlockButton) {
		m_unlockButton->setVisible(false);
	}

	if (nullptr != m_pay_tip) {
		m_pay_tip->setVisible(false);
	}
}

void UUnlockLayerController::enterUiEnoughStar() {
	m_uiType = Ui_Enough_Star;
	if (nullptr != m_closeBtn) {
		m_closeBtn->setVisible(true);
	}

	if (nullptr != m_panel_free) {
		m_panel_free->setVisible(true);
	}

	if (nullptr != m_panel_disfree) {
		m_panel_disfree->setVisible(false);
	}

	if (nullptr != m_yesButton) {
		m_yesButton->setVisible(false);
	}

	if (nullptr != m_unlockButton) {
		m_unlockButton->setVisible(true);
	}

	if (nullptr != m_pay_tip) {
		m_pay_tip->setVisible(false);
	}
}

void UUnlockLayerController::enterUiPayUnlock() {
	m_uiType = Ui_Pay_Unlock;
	if (nullptr != m_closeBtn) {
		m_closeBtn->setVisible(true);
	}

	if (nullptr != m_panel_free) {
		m_panel_free->setVisible(false);
	}

	if (nullptr != m_panel_disfree) {
		m_panel_disfree->setVisible(true);
	}

	if (nullptr != m_yesButton) {
		m_yesButton->setVisible(true);
	}

	if (nullptr != m_unlockButton) {
		m_unlockButton->setVisible(false);
	}

	if (nullptr != m_pay_tip) {
		m_pay_tip->setVisible(true);
	}
}