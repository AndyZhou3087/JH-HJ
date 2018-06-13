#include "GamePauseLayerController.h"
#include "GameData.h"
#include "GameManager.h"
#include "URookieLayerController.h"
#include "GameIntroLayerController.h"
#include "GameLevelLayerController.h"
#include "cocos2d.h"
#include "HomeScene.h"
#include "SettingManager.h"
#include "GameEnergyTipLayerController.h"
USING_NS_CC;

GamePauseLayerController::GamePauseLayerController():
m_sound_on(NULL),
m_sound_off(NULL),
m_giftLayer(NULL),
m_introLayer(NULL),
m_levelLayer(NULL),
m_energyTipLayer(NULL)
{

}

GamePauseLayerController::~GamePauseLayerController() {

}

void GamePauseLayerController::initial() {
	if (NULL == m_layer) {
		return;
	}

	auto resumeBtn = dynamic_cast<Button*>(m_layer->getChildByName("Panel_1")->getChildByName("Btn_resume"));
	if (nullptr != resumeBtn) {
		resumeBtn->addClickEventListener([=](Ref*ref){
			Audio::getInstance()->playClick();
			exit();
			GameManager::getInstance()->resume();
		});
	}

	auto homeBtn = dynamic_cast<Button*>(m_layer->getChildByName("Panel_1")->getChildByName("Btn_home"));
	if (nullptr != homeBtn) {
		homeBtn->addClickEventListener([=](Ref*ref){
			Audio::getInstance()->playClick();
			exit();
			GameManager::getInstance()->overWithoutNotice();
			if (nullptr != m_levelLayer) {
				m_levelLayer->enter();
			}
		});
	}

	auto restartBtn = dynamic_cast<Button*>(m_layer->getChildByName("Panel_1")->getChildByName("Btn_restart"));
	if (nullptr != restartBtn) {
		restartBtn->addClickEventListener([=](Ref*ref){
			Audio::getInstance()->playClick();		
			exit();
	
			if (GameData::getInstance()->getEnergy() >= s_energy_use) {
				GameManager::getInstance()->restart();
			} else {
				if (nullptr != m_energyTipLayer) {
					m_energyTipLayer->enter();
				}
			}
		});

	}

	auto helpBtn = dynamic_cast<Button*>(m_layer->getChildByName("Panel_1")->getChildByName("Btn_help"));
	if (nullptr != helpBtn) {
		helpBtn->addClickEventListener([=](Ref*ref){
			Audio::getInstance()->playClick();	
			exit();
			if (nullptr != m_introLayer) {
				m_introLayer->enter();
			}
		});
	}

	m_sound_on = dynamic_cast<Button*>(m_layer->getChildByName("Btn_sound_on"));
	if (nullptr != m_sound_on) {
		m_sound_on->addClickEventListener([this](Ref*ref){
			Audio::getInstance()->playClick();
			Audio::getInstance()->setSoundSwitch(false, false);
			m_sound_on->setVisible(false);
			m_sound_off->setVisible(true);
		});

	}
	

	m_sound_off = dynamic_cast<Button*>(m_layer->getChildByName("Btn_sound_off"));
	if (nullptr != m_sound_off) {
		m_sound_off->addClickEventListener([this](Ref*ref){
			Audio::getInstance()->playClick();
			Audio::getInstance()->setSoundSwitch(true, false);
			m_sound_on->setVisible(true);
			m_sound_off->setVisible(false);
		});

	}

	auto giftBtn = dynamic_cast<Button*>(m_layer->getChildByName("Panel_1")->getChildByName("Btn_gift"));
	if (nullptr != giftBtn) {
		giftBtn->addClickEventListener([=](Ref*ref){
			Audio::getInstance()->playClick();
			exit();
			if (nullptr != m_giftLayer && !m_giftLayer->isEnter()) {
				m_giftLayer->enterHappy();
			}
		});

		if (!SettingManager::getInstance()->isOpen(Setting_happy)) {
			giftBtn->setVisible(false);
		}
	}
	
}

void GamePauseLayerController::enter() {
	if (NULL == m_layer) {
		return;
	}
	enterBg();
	const Size size = Director::getInstance()->getVisibleSize();
	m_layer->ignoreAnchorPointForPosition(false);
	m_layer->setAnchorPoint(Vec2(0.5,0.5));
	m_layer->setPosition(Vec2(size.width * 0.5, size.height * 0.5));

	m_bEnter = true;

	bool soundOn = (Audio::getInstance()->isSoundOpened());
	if (NULL != m_sound_on) {
		m_sound_on->setVisible(soundOn);
	}

	if (NULL != m_sound_off) {
		m_sound_off->setVisible(!soundOn);
	}

	playEffectNode();
}

void GamePauseLayerController::exit() {
	if (NULL == m_layer) {
		return;
	}
	exitBg();
	const Size size = Director::getInstance()->getVisibleSize();
	m_layer->ignoreAnchorPointForPosition(false);
	m_layer->setAnchorPoint(Vec2(0.5,0.5));
	m_layer->setPositionY(size.height * 2);
	m_bEnter = false;

	stopEffectNode();
}
