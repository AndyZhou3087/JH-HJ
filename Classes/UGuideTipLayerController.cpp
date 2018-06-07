#include "UGuideTipLayerController.h"
#include "GameData.h"
#include "GameManager.h"
#include "SettingManager.h"
#include "GameUtil.h"
#include "cocos2d.h"
USING_NS_CC;

UGuideTipLayerController::UGuideTipLayerController():
m_callback(nullptr)
{

}

UGuideTipLayerController::~UGuideTipLayerController() {

}

void UGuideTipLayerController::initial() {
	if (nullptr == m_layer) {
		return;
	}

	auto yesBtn = dynamic_cast<Button*>(m_layer->getChildByName("bg_panel")->getChildByName("Btn_yes"));
	if (nullptr != yesBtn) {
		yesBtn->addClickEventListener([=](Ref*ref){
			Audio::getInstance()->playClick();
			startPayment(Setting_guidetip);
		});
	}

	auto closeBtn = dynamic_cast<Button*>(m_layer->getChildByName("bg_panel")->getChildByName("Btn_close"));
	if (nullptr != closeBtn) {
		closeBtn->addClickEventListener([=](Ref*ref){
			Audio::getInstance()->playClick();
			exit();
			if (nullptr != m_callback) {	
				auto scheduleCallback = [=](float dt){
					m_callback();
				};
				auto _schedule = Director::sharedDirector()->getRunningScene()->getScheduler();
				_schedule->schedule(scheduleCallback, HelpObj::getInstance(), 0.0f,0,0.2f, false, "next");
			}
		});
	}
}

void UGuideTipLayerController::enter() {
	CenterLayerController::enter();
}

void UGuideTipLayerController::exit() {
	CenterLayerController::exit();
}


void UGuideTipLayerController::onPayGuideTipComplete(bool success) {
	exit();
	if (success) {
		GameData::getInstance()->addToolTip(5);
		Audio::getInstance()->playBuy();
	}

	if (nullptr != m_callback) {	
		auto scheduleCallback = [=](float dt){
			m_callback();
		};
		auto _schedule = Director::sharedDirector()->getRunningScene()->getScheduler();
		_schedule->schedule(scheduleCallback, HelpObj::getInstance(), 0.0f,0,0.2f, false, "next");
	}
}