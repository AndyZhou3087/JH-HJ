#include "GameUnlockLayerController.h"
#include "GameData.h"
#include "GameManager.h"

#include "cocos2d.h"
USING_NS_CC;

GameUnlockLayerController::GameUnlockLayerController():
m_unlockStar(NULL)
{

}

GameUnlockLayerController::~GameUnlockLayerController() {

}

void GameUnlockLayerController::initial() {
	if (NULL == m_layer) {
		return;
	}

	const Size size = Director::getInstance()->getVisibleSize();

	m_unlockStar = dynamic_cast<TextAtlas*>(m_layer->getChildByName("bg_mid_1")
													->getChildByName("bg_top")
													->getChildByName("panel_1")
													->getChildByName("num_star"));


	auto starUnlockBtn = dynamic_cast<Button*>(m_layer->getChildByName("bg_mid_1")
														->getChildByName("bg_top")
														->getChildByName("panel_1")
														->getChildByName("Btn_unlock"));
	if (nullptr != starUnlockBtn) {
		starUnlockBtn->addClickEventListener([=](Ref*ref){
			Audio::getInstance()->playClick();
			ActionInterval * action = Sequence::create(
				EaseSineIn::create(MoveTo::create(0.5, Vec2(size.width * 0.5, - size.height))),
				CallFunc::create(
				[=](){
					exitBg();
	
			}
			),NULL);
			m_layer->runAction(action);			
		});

		m_effectNode1 = starUnlockBtn;
	}

	auto purchaseUnlockBtn = dynamic_cast<Button*>(m_layer->getChildByName("bg_mid_1")
															->getChildByName("panel_2")
															->getChildByName("Btn_unlock"));
	if (nullptr != purchaseUnlockBtn) {
		purchaseUnlockBtn->addClickEventListener([=](Ref*ref){
			Audio::getInstance()->playClick();
			ActionInterval * action = Sequence::create(
				EaseSineIn::create(MoveTo::create(0.5, Vec2(size.width * 0.5, - size.height))),
				CallFunc::create(
				[=](){
					exitBg();

			}),NULL);
			m_layer->runAction(action);			
		});

		m_effectNode2 = purchaseUnlockBtn;
	}

	auto closeBtn = dynamic_cast<Button*>(m_layer->getChildByName("bg_mid_1")
		->getChildByName("bg_top")
		->getChildByName("Btn_close"));
	if (nullptr != closeBtn) {
		closeBtn->addClickEventListener([=](Ref*ref){
			Audio::getInstance()->playClick();
			exit();

			if (nullptr != m_nextLayer && !m_nextLayer->isEnter()) {
				m_nextLayer->enter();
			}
		});
	}
}

void GameUnlockLayerController::enter() {
	if (NULL == m_layer) {
		return;
	}
	enterBg();
	const Size size = Director::getInstance()->getVisibleSize();
	m_layer->ignoreAnchorPointForPosition(false);
	m_layer->setAnchorPoint(Vec2(0.5,0.5));
	m_layer->setPosition(Vec2(size.width * 0.5, size.height * 2));

	m_bEnter = true;

	ActionInterval * action = EaseSineOut::create(MoveTo::create(0.5, Vec2(size.width * 0.5, size.height * 0.5)));
	m_layer->runAction(action);
}

void GameUnlockLayerController::exit() {
	if (NULL == m_layer) {
		return;
	}
	exitBg();
	const Size size = Director::getInstance()->getVisibleSize();
	m_layer->ignoreAnchorPointForPosition(false);
	m_layer->setAnchorPoint(Vec2(0.5,0.5));
	m_layer->setPositionY(size.height * 2);
	m_bEnter = false;
}