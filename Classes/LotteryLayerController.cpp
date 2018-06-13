#include "LotteryLayerController.h"
#include "GameData.h"
#include "GameManager.h"

#include "cocos2d.h"
USING_NS_CC;

LotteryLayerController::LotteryLayerController()
{

}

LotteryLayerController::~LotteryLayerController() {

}

void LotteryLayerController::initial() {
	if (NULL == m_layer) {
		return;
	}

	auto yesBtn = dynamic_cast<Button*>(m_layer->getChildByName("bg_mid_1")
		->getChildByName("bg_mid_2")
		->getChildByName("bg_mid_3")
		->getChildByName("bg_bottom")
		->getChildByName("Btn_yes"));
	if (nullptr != yesBtn) {
		yesBtn->addClickEventListener([=](Ref*ref){
			Audio::getInstance()->playClick();
			exit();
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
		});
	}
}

void LotteryLayerController::enter() {
	if (NULL == m_layer) {
		return;
	}

	const Size size = Director::getInstance()->getVisibleSize();
	m_layer->ignoreAnchorPointForPosition(false);
	m_layer->setAnchorPoint(Vec2(0.5,0.5));
	m_layer->setPosition(Vec2(size.width * 0.5, size.height * 0.5));

	m_bEnter = true;
	playEffectNode();
}

void LotteryLayerController::exit() {
	if (NULL == m_layer) {
		return;
	}

	const Size size = Director::getInstance()->getVisibleSize();
	m_layer->ignoreAnchorPointForPosition(false);
	m_layer->setAnchorPoint(Vec2(0.5,0.5));
	m_layer->setPositionY(size.height * 2);
	m_bEnter = false;
	stopEffectNode();
}