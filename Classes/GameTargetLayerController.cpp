#include "GameTargetLayerController.h"
#include "GameData.h"
#include "GameManager.h"

#include "cocos2d.h"
USING_NS_CC;

GameTargetLayerController::GameTargetLayerController():
m_target(NULL),
m_time(NULL),
m_levelNum(NULL),
m_bar(NULL),
m_timeOriginPosX(0),
m_ready(NULL),
m_go(NULL)
{

}

GameTargetLayerController::~GameTargetLayerController() {

}

void GameTargetLayerController::initial() {
	if (NULL == m_layer) {
		return;
	}

	const Size size = Director::getInstance()->getVisibleSize();

	m_bar = m_layer->getChildByName("bg_bar");

	m_target = dynamic_cast<TextAtlas*>(m_layer->getChildByName("bg_bar")
												->getChildByName("num_target"));

	m_time = dynamic_cast<TextAtlas*>(m_layer->getChildByName("bg_bar")
												->getChildByName("bg_time")
												->getChildByName("num_time"));

	if (nullptr != m_time) {
		m_timeOriginPosX = m_time->getPositionX();
	}

	m_levelNum = dynamic_cast<TextAtlas*>(m_layer->getChildByName("bg_bar")
													->getChildByName("level_num"));
	m_ready = Sprite::createWithSpriteFrameName(s_label_ready);
	m_layer->addChild(m_ready);
	m_ready->setPosition(size.width * 0.5, size.height * 0.5);
	m_ready->setVisible(false);
	m_go = Sprite::createWithSpriteFrameName(s_label_go);
	m_layer->addChild(m_go);
	m_go->setPosition(size.width * 0.5, size.height * 0.5);
	m_go->setVisible(false);
}

void GameTargetLayerController::enter() {
	if (NULL == m_layer) {
		return;
	}
	enterBg();
	const Size size = Director::getInstance()->getVisibleSize();
	m_layer->ignoreAnchorPointForPosition(false);
	m_layer->setAnchorPoint(Vec2(0.5,0.5));
	m_layer->setPosition(Vec2(size.width * 0.5, size.height * 0.5));

	updateInfo();
	enterBar();
	m_bEnter = true;
}

void GameTargetLayerController::exit() {
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

void GameTargetLayerController::updateInfo() {
	const int target = GameData::getInstance()->getTargetScore();
	const int time = GameData::getInstance()->getTimelimit();
	const int level = GameData::getInstance()->getLevel() + 1;

	if (nullptr != m_target) {
		m_target->setString(String::createWithFormat("%d", target)->_string);
	}

	if (nullptr != m_time) {
		m_time->setString(String::createWithFormat("%d/", time)->_string);
		m_time->setPositionX(m_timeOriginPosX + (55 - m_time->getContentSize().width) * 0.5);
	}

	if (nullptr != m_levelNum) {
		m_levelNum->setString(String::createWithFormat("%d", level)->_string);
	}
}

void GameTargetLayerController::enterBar() {
	if (nullptr == m_bar) {
		return;
	}

	const Size size = Director::getInstance()->getVisibleSize();
	m_bar->setPositionY(size.height + m_bar->getContentSize().height);
	m_bar->runAction(Sequence::create(
		EaseSineOut::create(MoveTo::create(0.5, Vec2(size.width * 0.5, size.height * 0.5))),
		DelayTime::create(1),
		EaseSineIn::create(MoveTo::create(0.5, Vec2(size.width * 0.5, -m_bar->getContentSize().height))),
		CallFunc::create([=](){ 
			showReady();
		}),
		DelayTime::create(1.5),
		CallFunc::create([=](){ 
			exitBg();
			GameManager::getInstance()->start();
		}),
		NULL));
}
void GameTargetLayerController::showReady() {
	if (NULL == m_go) {
		return;
	}
	m_ready->setVisible(true);
	Audio::getInstance()->playReady();
	m_ready->runAction(Sequence::create(
		DelayTime::create(0.6),
		CallFunc::create([=](){ 
			m_ready->setVisible(false);
			showGo();
		}),
		NULL
		));
}
void GameTargetLayerController::showGo() {
	if (NULL == m_go) {
		return;
	}
	m_go->setVisible(true);
	m_go->setScale(0.5);
	m_go->runAction(Sequence::create(
		ScaleTo::create(0.4, 1, 1, 1),
		DelayTime::create(0.4),
		CallFunc::create([=](){ 
			m_go->setVisible(false);
		}),
		NULL
		));
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

GameTargetBossLayerController::GameTargetBossLayerController()
{

}

GameTargetBossLayerController::~GameTargetBossLayerController() {

}

void GameTargetBossLayerController::initial() {
	if (NULL == m_layer) {
		return;
	}

	m_bar = m_layer->getChildByName("bg_bar");
}