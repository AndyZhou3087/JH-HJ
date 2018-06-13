#include "GameIntroLayerController.h"
#include "cocos2d.h"
#include "GameManager.h"
#include "Analytics.h"
#include "UserData.h"
USING_NS_CC;

GameIntroLayerController::GameIntroLayerController():
m_ball_red(NULL),
m_ball_yellow(NULL)
{

}

GameIntroLayerController::~GameIntroLayerController() {
}

void GameIntroLayerController::initial() {
	if (NULL == m_layer) {
		return;
	}

	auto closeBtn = dynamic_cast<Button*>(m_layer->getChildByName("Btn_close"));
	if (nullptr != closeBtn) {
		closeBtn->addClickEventListener([=](Ref*ref){
			Audio::getInstance()->playClick();
			exit();
			if (GameManager::getInstance()->isStarted() && GameManager::getInstance()->isPaused()) {
				GameManager::getInstance()->resume();
			} else if (nullptr != m_nextLayer) {
				m_nextLayer->enter();
			}

			if (!UserData::getIntroPass()) {
				UMengAnalytics::onComplete(ROOKIE_LEVEL_INTRO, 0, 0);
				UserData::setIntroPass(true);
			}	
		});
	}

	/*auto pageView = dynamic_cast<PageView*>(m_layer->getChildByName("page"));
	pageView->addEventListener([=](Ref*ref,PageView::EventType e){
		const int index = pageView->getCurPageIndex();
		if (0 == index) {
			m_ball_red->setPositionX(m_leftX);
			m_ball_yellow->setPositionX(m_rightX);
		} else {
			m_ball_red->setPositionX(m_rightX);
			m_ball_yellow->setPositionX(m_leftX);
		}
	});*/

	m_ball_red = dynamic_cast<Sprite*>(m_layer->getChildByName("ball_1"));
	m_leftX = m_ball_red->getPositionX();
	m_ball_yellow = dynamic_cast<Sprite*>(m_layer->getChildByName("ball_2"));
	m_rightX = m_ball_yellow->getPositionX();
}

void GameIntroLayerController::enter() {
	CenterLayerController::enter();
	if (!UserData::getIntroPass()) {
		UMengAnalytics::onBegin(ROOKIE_LEVEL_INTRO);
	}
}

void GameIntroLayerController::exit() {
	if (NULL == m_layer) {
		return;
	}

	exitBg();
	const Size size = Director::getInstance()->getVisibleSize();
	m_layer->ignoreAnchorPointForPosition(false);
	m_layer->setAnchorPoint(Vec2(0.5, 0));
	m_layer->setPositionY(size.height * 2);
	m_bEnter = false;
}