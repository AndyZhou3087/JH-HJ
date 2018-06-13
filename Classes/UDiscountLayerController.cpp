#include "UDiscountLayerController.h"
#include "GameData.h"
#include "GameManager.h"
#include "SettingManager.h"
#include "cocos2d.h"
USING_NS_CC;

UDiscountLayerController::UDiscountLayerController():
m_time1(NULL),
m_time2(NULL),
m_sec(10),
m_millSec(59)
{

}

UDiscountLayerController::~UDiscountLayerController() {

}

void UDiscountLayerController::initial() {
	if (NULL == m_layer) {
		return;
	}


	m_time1 = dynamic_cast<TextAtlas*>(m_layer->getChildByName("light")
												->getChildByName("light2")
												->getChildByName("bg_time_1")
												->getChildByName("time_1"));

	m_time2 = dynamic_cast<TextAtlas*>(m_layer->getChildByName("light")
												->getChildByName("light2")
												->getChildByName("bg_time_2")
												->getChildByName("time_2"));

	auto yesBtn = dynamic_cast<Button*>(m_layer->getChildByName("bg_mid_1")
		->getChildByName("bg_mid_2")
		->getChildByName("bg_bottom")
		->getChildByName("Btn_yes"));
	if (nullptr != yesBtn) {
		yesBtn->addClickEventListener([=](Ref*ref){
			Audio::getInstance()->playClick();
			exit();
			startPayment(Setting_discount);
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

void UDiscountLayerController::enter() {
	CenterLayerController::enter();

	if (nullptr == m_time1 || nullptr == m_time2) {
		return;
	}

	m_sec = 10;
	m_time1->setString("10");
	m_time1->runAction(Sequence::create(Repeat::create(
		Sequence::create(
		CallFunc::create([=](){
				m_time1->setString(String::createWithFormat("%02d", m_sec)->_string);
				m_sec--;
				if (m_sec < 0) {
					m_sec = 0;
				}
				playMillSecEffect();
			}),
			DelayTime::create(1),
			NULL
			),11),
		CallFunc::create(
			[=]() {
				exit();
				if (GameManager::getInstance()->isPaused()) {
					GameManager::getInstance()->resume();
				}
			}
		),
		NULL));	
}

void UDiscountLayerController::exit() {
	CenterLayerController::exit();

	if (nullptr != m_time1)  {
		m_time1->stopAllActions();
	}
		
	if ( nullptr != m_time2) {
		m_time2->stopAllActions();
	}
}

void UDiscountLayerController::playMillSecEffect() {
	if (nullptr != m_time2) {
		m_millSec = 59;
		m_time2->setString("59");
		m_time2->runAction(Repeat::create(
			Sequence::create(
			DelayTime::create(0.016),
			CallFunc::create([=](){
				m_time2->setString(String::createWithFormat("%02d", m_millSec)->_string);
				m_millSec--;
				if (m_millSec < 0) {
					m_millSec = 0;
				}
			}),
			NULL
			),60));
	}
}

void UDiscountLayerController::onPayDiscountComplete(bool success) {
	if (success) {
		GameData::getInstance()->addToolCross(3);
		GameData::getInstance()->addToolTime(3);
		GameData::getInstance()->addToolTip(3);
		GameData::getInstance()->addToolBomb(3);
		Audio::getInstance()->playBuy();
	}

	if (GameManager::getInstance()->isPaused()) {
		GameManager::getInstance()->resume();
	}
}