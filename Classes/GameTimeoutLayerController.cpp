#include "GameTimeoutLayerController.h"
#include "GameData.h"
#include "GameManager.h"
#include "GamePurchaseLayerController.h"
#include "GameEnergyTipLayerController.h"
#include "cocos2d.h"
USING_NS_CC;

GameTimeoutLayerController::GameTimeoutLayerController():
m_timeNum(NULL),
m_purchaseLayer(NULL)
{

}

GameTimeoutLayerController::~GameTimeoutLayerController() {

}

void GameTimeoutLayerController::initial() {
	if (NULL == m_layer) {
		return;
	}

	//m_timeNum = dynamic_cast<TextAtlas*>(m_layer->getChildByName("bg")
	//												->getChildByName("time_image")
	//												->getChildByName("bg_num")
	//												->getChildByName("num_time"));

	auto yesBtn = dynamic_cast<Button*>(m_layer->getChildByName("bg_mid")
												->getChildByName("bg_bottom")
												->getChildByName("Btn_yes"));
	if (nullptr != yesBtn) {
		yesBtn->addClickEventListener([=](Ref*ref){
			Audio::getInstance()->playClick();
			exit();

			if (GameData::getInstance()->getToolTimeNum() > 0) {
				GameData::getInstance()->addToolTime(-1);
				GameData::getInstance()->addTime(s_time_add);
				GameManager::getInstance()->relive();
			} else {
				CCASSERT(false,"tool time must can't be 0");
				if (nullptr != m_purchaseLayer) {
					m_purchaseLayer->enter();
				}
			}
		});

		m_effectNode1 = yesBtn;
	}

	auto closeBtn = dynamic_cast<Button*>(m_layer->getChildByName("bg_mid")
												->getChildByName("bg_top")
												->getChildByName("Btn_close"));
	if (nullptr != closeBtn) {
		closeBtn->addClickEventListener([=](Ref*ref){
			Audio::getInstance()->playClick();
			exit();

			if (nullptr != m_nextLayer) {
				m_nextLayer->enter();
			}
		});
	}
}

void GameTimeoutLayerController::enter() {
	if (NULL == m_layer) {
		return;
	}
	enterBg();
	const Size size = Director::getInstance()->getVisibleSize();
	m_layer->ignoreAnchorPointForPosition(false);
	m_layer->setAnchorPoint(Vec2(0.5,0.5));
	m_layer->setPosition(Vec2(size.width * 0.5, size.height * 0.5));

	m_bEnter = true;

	Audio::getInstance()->playTimeup();
	playEffectNode();
}

void GameTimeoutLayerController::exit() {
	if (NULL == m_layer) {
		return;
	}
	exitBg();
	const Size size = Director::getInstance()->getVisibleSize();
	m_layer->ignoreAnchorPointForPosition(false);
	m_layer->setAnchorPoint(Vec2(0.5,0.5));
	m_layer->setPositionX(size.width * 2);
	m_bEnter = false;
	stopEffectNode();
}