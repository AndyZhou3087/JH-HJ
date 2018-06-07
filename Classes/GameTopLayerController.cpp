#include "GameTopLayerController.h"
#include "Audio.h"
#include "prefix.h"
#include "GameData.h"
#include "GameManager.h"
#include "UserData.h"
#include "GameUtil.h"
GameTopLayerController::GameTopLayerController():
m_numScore(NULL),
m_numTarget(NULL),
m_numTime(NULL),
m_timeBar(NULL),
m_bStop(false)
{

}

GameTopLayerController::~GameTopLayerController() {

}

void GameTopLayerController::initial() {
	if (NULL == m_layer) {
		return;
	}

	m_layer->ignoreAnchorPointForPosition(false);
	m_layer->setAnchorPoint(Vec2(0.5,1));

	m_numScore = dynamic_cast<TextAtlas*>(m_layer->getChildByName("bg_top")->	\
								getChildByName("bg_time")->	\
								getChildByName("bg_score")->	\
								getChildByName("num_score"));

	m_numTarget = dynamic_cast<TextAtlas*>(m_layer->getChildByName("bg_top")->	\
								getChildByName("label_target")->	\
								getChildByName("num_target"));

	m_numTime = dynamic_cast<TextAtlas*>(m_layer->getChildByName("bg_top")->	\
								getChildByName("bg_time")->	\
								getChildByName("bg_time_unactive")->	\
								getChildByName("time_active")->	\
								getChildByName("num_time"));

	m_timeBar = dynamic_cast<Sprite*>(m_layer->getChildByName("bg_top")->	\
								getChildByName("bg_time")->	\
								getChildByName("bg_time_unactive")->	\
								getChildByName("time_active"));

	auto pauseBtn = dynamic_cast<Button*>(m_layer->getChildByName("bg_top")->	\
								getChildByName("Btn_pause"));
	if (nullptr != pauseBtn) {
		pauseBtn->addClickEventListener([this](Ref*ref){
				Audio::getInstance()->playClick();
				GameManager::getInstance()->pause();
		});
	}
}

void GameTopLayerController::enter() {
	if (NULL == m_layer) {
		return;
	}

	const Size size = Director::getInstance()->getVisibleSize();
	m_layer->setPosition(Vec2(size.width * 0.5, size.height));
	m_bEnter = true;

	updateTimeBar();
}

void GameTopLayerController::exit() {
	if (NULL == m_layer) {
		return;
	}

	const Size size = Director::getInstance()->getVisibleSize();
	m_layer->setPosition(Vec2(size.width * 0.5, size.height * 2));
	m_bEnter = false;
}

void GameTopLayerController::onGameReady() {
	GameData::getInstance()->setElapseTime(0);
	updateInfo();
	updateTimeBar();
}

void GameTopLayerController::onGameStart() {
	
}

void GameTopLayerController::onGameRestart() {
	onGameRestart();
}

void GameTopLayerController::on1sTimer() {
	if (!isEnter()) {
		return;
	}

	if (m_bStop) {
		return;
	}

	updateTime();
}

void GameTopLayerController::onScoreChange() {
	if (!isEnter()) {
		return;
	}

	if (nullptr != m_numScore) {
		m_numScore->setString(String::createWithFormat("%d",GameData::getInstance()->getScore())->_string);

		GameUtil::shake(m_numScore, 1.5, 0.9);
	}
}

void GameTopLayerController::updateInfo() {
	if (!isEnter()) {
		return;
	}

	if (nullptr != m_numTime) {
		m_numTime->setString(String::createWithFormat("%d/",GameData::getInstance()->getTimelimit())->_string);
	}

	if (nullptr != m_numTarget) {
		m_numTarget->setString(String::createWithFormat("%d",GameData::getInstance()->getTargetScore())->_string);
	}

	if (nullptr != m_numScore) {
		m_numScore->setString("0");
	}
}

void GameTopLayerController::updateTimeBar() {
	if (!isEnter()) {
		return;
	}

	float rate = GameData::getInstance()->getTime() * 1.0f /GameData::getInstance()->getTimelimit();

	if (nullptr != m_timeBar) {
		static const Vec2 offSize = m_timeBar->getTextureRect().origin;
		static const float h = m_timeBar->getContentSize().height;
		static const float w = m_timeBar->getContentSize().width;
		float width = rate * w; 
		m_timeBar->setTextureRect(CCRectMake(offSize.x, offSize.y, width, h));
	}
}

void GameTopLayerController::initTime() {
	int s = GameData::getInstance()->getTime();

	if (nullptr != m_numTime) {
		m_numTime->setString(String::createWithFormat("%d/",s)->_string);		
	}
}

void GameTopLayerController::updateTime() {
	if (GameManager::getInstance()->isStarted() && !GameManager::getInstance()->isPaused()) {
		int s = GameData::getInstance()->getTime();
		s -= 1;
		if (s <= s_time_count_down_tip && s>=0) {
			Audio::getInstance()->playDida();
		}

		if (s <= 0) {
			s = 0;
		} else {
			GameData::getInstance()->addElapseTime(1);
		}
		GameData::getInstance()->setTime(s);
		if (s <= 0) {
			GameManager::getInstance()->over();
		}

		if (nullptr != m_numTime) {
			m_numTime->setString(String::createWithFormat("%d/",s)->_string);		
		}

		updateTimeBar();
	}
}

void GameTopLayerController::onTimeChange() {
	updateTime();
}