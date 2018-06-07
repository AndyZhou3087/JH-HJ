#include "GameScoreLayerController.h"

GameScoreLayerController::GameScoreLayerController():
m_target(NULL),
m_score(NULL),
m_time(NULL),
m_star1(NULL),
m_star2(NULL),
m_star3(NULL)
{

}

GameScoreLayerController::~GameScoreLayerController() {

}

void GameScoreLayerController::initial() {
	if (nullptr == m_layer) {
		return;
	}

	m_star1		= dynamic_cast<Sprite *>(m_layer->getChildByName("Panel_2")->getChildByName("score_bg")->getChildByName("star_grey_1"));
	m_star2		= dynamic_cast<Sprite *>(m_layer->getChildByName("Panel_2")->getChildByName("score_bg")->getChildByName("star_grey_2"));
	m_star3		= dynamic_cast<Sprite *>(m_layer->getChildByName("Panel_2")->getChildByName("score_bg")->getChildByName("star_grey_3"));

	m_target	= dynamic_cast<TextAtlas *>(m_layer->getChildByName("Panel_2")->getChildByName("target_score")->getChildByName("target"));
	m_time		= dynamic_cast<TextAtlas *>(m_layer->getChildByName("Panel_2")->getChildByName("bg_time")->getChildByName("game_time"));
	m_target	= dynamic_cast<TextAtlas *>(m_layer->getChildByName("Panel_2")->getChildByName("score_bg")->getChildByName("score"));
}

void GameScoreLayerController::enter() {
	if (NULL == m_layer) {
		return;
	}

	static const Size size = Director::getInstance()->getVisibleSize();
	m_layer->ignoreAnchorPointForPosition(false);
	m_layer->setAnchorPoint(Vec2(0.5,1));
	m_layer->setPosition(Vec2(size.width * 0.5, size.height));
	m_bEnter = true;
}

void GameScoreLayerController::exit() {

}