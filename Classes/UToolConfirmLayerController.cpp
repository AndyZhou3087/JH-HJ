#include "UToolConfirmLayerController.h"
#include "GameData.h"
#include "cocos2d.h"

USING_NS_CC;

UToolConfirmLayerController::UToolConfirmLayerController():
m_close(NULL),
m_disfreeTip(NULL),
m_freeTip(NULL),
m_toolNum_1(NULL),
m_toolNum_2(NULL),
m_type(ToolConfirm_Free),
m_callback(nullptr)
{

}

UToolConfirmLayerController::~UToolConfirmLayerController() {

}

void UToolConfirmLayerController::initial() {
	if (NULL == m_layer) {
		return;
	}

	m_disfreeTip = m_layer->getChildByName("bg_mid")->getChildByName("bg_bottom")
												    ->getChildByName("tip_disfree");

	m_freeTip = m_layer->getChildByName("bg_mid")->getChildByName("bg_bottom")
													->getChildByName("tip_free");

	m_toolNum_1 = m_layer->getChildByName("bg_mid")->getChildByName("bg_top")
												   ->getChildByName("panel")->getChildByName("tool_time")->getChildByName("num_tool");

	m_toolNum_2 = m_layer->getChildByName("bg_mid")->getChildByName("bg_top")
												   ->getChildByName("panel")->getChildByName("tool_tip")->getChildByName("num_tool");

	auto yesBtn = dynamic_cast<Button*>(m_layer->getChildByName("bg_mid")
												->getChildByName("bg_bottom")
												->getChildByName("Btn_yes"));
	if (nullptr != yesBtn) {
		yesBtn->addClickEventListener([=](Ref*ref){
			Audio::getInstance()->playClick();
			exit();
			if (ToolConfirm_Free == m_type) {
				GameData::getInstance()->addToolTime(1);
				GameData::getInstance()->addToolTip(1);
			} else {
				startPayment(Setting_toolconfirm);
			}

			if (nullptr != m_callback) {
				m_callback();
			}
		});

		m_effectNode1 = yesBtn;
	}

	m_close = dynamic_cast<Button*>(m_layer->getChildByName("bg_mid")
												->getChildByName("bg_top")
												->getChildByName("Btn_close"));
	if (nullptr != m_close) {
		m_close->addClickEventListener([=](Ref*ref){
			Audio::getInstance()->playClick();
			exit();
			if (nullptr != m_callback) {
				m_callback();
			}
		});
	}
}

void UToolConfirmLayerController::enter() {
}

void UToolConfirmLayerController::enterFree() {
	if (nullptr != m_close) {
		m_close->setVisible(false);
	}

	if (nullptr != m_toolNum_1) {
		m_toolNum_1->setVisible(false);
	}

	if (nullptr != m_toolNum_2) {
		m_toolNum_2->setVisible(false);
	}

	if (nullptr != m_disfreeTip) {
		m_disfreeTip->setVisible(false);
	}

	if (nullptr != m_freeTip) {
		m_freeTip->setVisible(true);
	}

	m_type = ToolConfirm_Free;
	CenterLayerController::enter();
}

void UToolConfirmLayerController::enterDisfree() {
	if (nullptr != m_close) {
		m_close->setVisible(true);
	}

	if (nullptr != m_toolNum_1) {
		m_toolNum_1->setVisible(false);
	}

	if (nullptr != m_toolNum_2) {
		m_toolNum_2->setVisible(false);
	}

	if (nullptr != m_disfreeTip) {
		m_disfreeTip->setVisible(true);
	}

	if (nullptr != m_freeTip) {
		m_freeTip->setVisible(false);
	}

	m_type = ToolConfirm_Disfree;
	CenterLayerController::enter();
}

void UToolConfirmLayerController::onPayToolConfirmComplete(bool success) {
	if (success) {
		GameData::getInstance()->addToolTime(3);
		GameData::getInstance()->addToolTip(3);
		Audio::getInstance()->playBuy();
	}

	if (nullptr != m_callback) {
		m_callback();
	}
}