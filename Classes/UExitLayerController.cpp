#include "UExitLayerController.h"
#include "GameData.h"
#include "GameManager.h"
#include "GameEvent.h"
#include "cocos2d.h"
#include "UserData.h"
#include "SettingManager.h"
USING_NS_CC;

#define TAG_BG 1000

UExitLayerController::UExitLayerController()
{

}

UExitLayerController::~UExitLayerController() {

}

void UExitLayerController::initial() {
	if (NULL == m_layer) {
		return;
	}

	const Size size = Director::getInstance()->getVisibleSize();
	auto bg = Sprite::create(s_bg_grey);
	bg->setScaleY(size.height/bg->getContentSize().height);
	m_layer->addChild(bg, -100);
	bg->setTag(TAG_BG);
	bg->setAnchorPoint(Vec2(0.5, 0));
	bg->setPosition(size.width * 0.5, 0);

	auto fetchBtn = dynamic_cast<Button*>(m_layer->getChildByName("bg_mid_1")
													->getChildByName("bg_mid_2")
													->getChildByName("Btn_get"));
	if (nullptr != fetchBtn) {
		fetchBtn->addClickEventListener([=](Ref*ref){
			Audio::getInstance()->playClick();
			exit();
			startPayment(Setting_exit);

			if (s_game_test_flag) {
				GameData::getInstance()->clearData();
			}
			//if (GameManager::getInstance()->isPaused()) {
			//	GameManager::getInstance()->resume();
			//}
			//Audio::getInstance()->playBuy();
			//GameData::getInstance()->clearData();
		});

		m_effectNode1 = fetchBtn;
	}

	auto yesBtn = dynamic_cast<Button*>(m_layer->getChildByName("bg_mid_1")
												->getChildByName("bg_mid_2")
												->getChildByName("bg_bottom")
												->getChildByName("Btn_ok"));
	if (nullptr != yesBtn) {
		yesBtn->addClickEventListener([=](Ref*ref){
			Audio::getInstance()->playClick();
			exit();
			if (s_game_test_flag) {
				GameData::getInstance()->clearData();
			}

			CCDirector::sharedDirector()->end();
		});
	}

	auto noBtn = dynamic_cast<Button*>(m_layer->getChildByName("bg_mid_1")
												->getChildByName("bg_mid_2")
												->getChildByName("bg_bottom")
												->getChildByName("Btn_no"));
	if (nullptr != noBtn) {
		noBtn->addClickEventListener([=](Ref*ref){
			Audio::getInstance()->playClick();
			exit();

			if (GameManager::getInstance()->isPaused()) {
				GameManager::getInstance()->resume();
			}

			GameEvent::throwEvent(Event_Exit_Cancel, NULL);
		});
	}
}

void UExitLayerController::enter() {
	CenterLayerController::enter();
	if (nullptr != m_layer) {
		auto bg = m_layer->getChildByTag(TAG_BG);
		if (nullptr != bg) {
			float dt = m_layer->getPositionY() - m_layer->getContentSize().height * 0.5;
			bg->setPositionY(-dt);
		}
	}

	if (GameManager::getInstance()->isStarted() && !GameManager::getInstance()->isPaused()) {
		GameManager::getInstance()->pause();
	}
}

void UExitLayerController::exit() {
	CenterLayerController::exit();
}

void UExitLayerController::onPayHaoliComplete(bool success) {
	if (success) {
		GameData::getInstance()->addToolCross(4);
		GameData::getInstance()->addToolBomb(4);
		Audio::getInstance()->playBuy();	
	}

	if (GameManager::getInstance()->isPaused()) {
		GameManager::getInstance()->resume();
	}

	GameEvent::throwEvent(Event_Exit_Cancel, NULL);
}