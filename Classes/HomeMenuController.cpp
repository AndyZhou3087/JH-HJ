#include "HomeMenuController.h"
#include "Audio.h"
#include "prefix.h"
#include "GameScene.h"
#include "GameManager.h"
#include "URookieLayerController.h"
#include "GameIntroLayerController.h"
#include "GameUtil.h"
#include "LoadGameScene.h"
#include "SettingManager.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "PluginHelper.h"
#endif

HomeMenuController::HomeMenuController():
m_sound_on(NULL),
m_sound_off(NULL),
m_gift(NULL),
m_giftPosY( -100 ),
m_uiRookieLayer(NULL),
m_introLayer(NULL),
m_logo(NULL)
{

}

HomeMenuController::~HomeMenuController() {

}

void HomeMenuController::initial() {
	MenuController::initial();

	if (NULL == m_Menu) {
		return;
	}

	m_logo = m_Menu->getChildByName("logo");
	m_logoPosY = m_logo->getPositionY();

	auto startBtn = dynamic_cast<Button*>(m_Menu->getChildByName("Btn_start"));
	if (nullptr != startBtn) {
		startBtn->addClickEventListener([this](Ref*ref){
			Audio::getInstance()->playClick();
			Director::sharedDirector()->replaceScene(LoadGameScene::createScene());
		});

		m_effectNode1 = startBtn;
	}

	auto helpBtn = dynamic_cast<Button*>(m_Menu->getChildByName("Panel_1")->getChildByName("Btn_help"));
	helpBtn->addClickEventListener([this](Ref*ref){
		Audio::getInstance()->playClick();
		if (nullptr != m_introLayer && !m_introLayer->isEnter()) {
			m_introLayer->enter();
		}

		
	});

	m_gift = dynamic_cast<Button*>(m_Menu->getChildByName("Panel_1")->getChildByName("Btn_gift"));
	m_gift->addClickEventListener([this](Ref*ref){
		Audio::getInstance()->playClick();
		if (nullptr != m_uiRookieLayer) {
			m_uiRookieLayer->enterRookie();
		}
	});
	m_giftPosY = m_gift->getPositionY();
	if (!SettingManager::getInstance()->isOpen(Setting_rookie)) {
		m_gift->setVisible(false);
	}

	
	m_sound_on = dynamic_cast<Button*>(m_Menu->getChildByName("Panel_1")->getChildByName("Btn_sound_on"));
	m_sound_on->addClickEventListener([this](Ref*ref){
		Audio::getInstance()->playClick();
		Audio::getInstance()->setSoundSwitch(false, true);
		m_sound_on->setVisible(false);
		m_sound_off->setVisible(true);
	});


	m_sound_off = dynamic_cast<Button*>(m_Menu->getChildByName("Panel_1")->getChildByName("Btn_sound_off"));
	m_sound_off->addClickEventListener([this](Ref*ref){
		Audio::getInstance()->playClick();
		Audio::getInstance()->setSoundSwitch(true, true);
		m_sound_on->setVisible(true);
		m_sound_off->setVisible(false);
	});

}

void HomeMenuController::enter() {
	if (NULL == m_Menu) {
		return;
	}

	static const Size size = Director::getInstance()->getVisibleSize();
	m_Menu->ignoreAnchorPointForPosition(false);
	m_Menu->setAnchorPoint(Vec2(0.5,0.5));
	m_Menu->setPosition(Vec2(size.width * 0.5, size.height * 0.5));
	m_bEnter = true;

	bool soundOn = (Audio::getInstance()->isSoundOpened());
	if (NULL != m_sound_on) {
		m_sound_on->setVisible(soundOn);
	}

	if (NULL != m_sound_off) {
		m_sound_off->setVisible(!soundOn);
	}

	playGiftEffect();
	playEffectNode();
	GameUtil::jumpDown(m_logo, 400);
}

void HomeMenuController::exit() {
	if (NULL == m_Menu) {
		return;
	}

	static const Size size = Director::getInstance()->getVisibleSize();
	m_Menu->setPositionX(2 * size.width);
	m_bEnter = false;

	stopGiftEffect();
	stopEffectNode();

	if (nullptr != m_logo) {
		m_logo->stopAllActions();
		m_logo->setPositionY(m_logoPosY);
	}
}

void HomeMenuController::playGiftEffect() {
	if (NULL != m_gift) {
		m_gift->setPositionY(getGiftPosY());
		GameUtil::jellyJump(m_gift, 30, true, 1);
	}
}

void HomeMenuController::stopGiftEffect() {
	if (NULL != m_gift) {
		m_gift->stopAllActions();
	}
}