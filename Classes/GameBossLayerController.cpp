#include "GameBossLayerController.h"
#include "GameData.h"
#include "GameManager.h"
#include "LuckyTimeManager.h"
#include "UTipWinLayerController.h"
#include "cocos2d.h"

USING_NS_CC;

#define ACTION_JUMP			500
#define ACTION_SHAKE_HAND	501
#define ACTION_SHAKE_HEAD	502

GameBossLayerController::GameBossLayerController():
m_bloodBar(nullptr),
m_time(nullptr),
m_emitterBomb(nullptr),
m_bossOriginPosY(10000),
m_bossOriginPosX(0),
m_tipwinLayer(nullptr),

m_boss(nullptr),
m_normalBoss_head(nullptr),
m_normallBoss_hand_left(nullptr),
m_normallBoss_hand_right(nullptr),

m_hurtBoss_head(nullptr),
m_hurtBoss_hand_left(nullptr),
m_hurtBoss_hand_right(nullptr),
m_hurtBoss_body(nullptr),
m_bStop(false)
{

}

GameBossLayerController::~GameBossLayerController() {

}

void GameBossLayerController::initial() {
	if (NULL == m_layer) {
		return;
	}

	m_layer->ignoreAnchorPointForPosition(false);
	m_layer->setAnchorPoint(Vec2(0.5,1));

	initalBoss();

	m_bloodBar = dynamic_cast<Sprite*>(m_layer->getChildByName("bg_cloud")->getChildByName("bg_blood")->getChildByName("blood"));
	m_time = dynamic_cast<TextAtlas*>(m_layer->getChildByName("bg_panel")->getChildByName("time"));

	auto pauseBtn = dynamic_cast<Button*>(m_layer->getChildByName("Btn_pause"));
	if (nullptr != pauseBtn) {
		pauseBtn->addClickEventListener([this](Ref*ref){
			Audio::getInstance()->playClick();
			GameManager::getInstance()->pause();
		});
	}

	if (nullptr != m_boss) {
		m_bossOriginPosY	= m_boss->getPositionY();
		m_bossOriginPosX	= m_boss->getPositionX(); 
	}

	initBossBombParticleSystem();
}

void GameBossLayerController::enter() {
	if (NULL == m_layer) {
		return;
	}

	const Size size = Director::getInstance()->getVisibleSize();
	m_layer->setPosition(Vec2(size.width * 0.5, size.height));
	m_bEnter = true;

	reset();
}

void GameBossLayerController::exit() {
	if (NULL == m_layer) {
		return;
	}

	const Size size = Director::getInstance()->getVisibleSize();
	m_layer->setPosition(Vec2(size.width * 0.5, size.height * 2));
	m_bEnter = false;
}

void GameBossLayerController::reset() {
	initTime();
	updateBloodBar();
	resetBoss();
	GameData::getInstance()->setElapseTime(0);
}

void GameBossLayerController::onGameStart() {
	if (isEnter()) {
		playBossShowEffect();
	}
}

void GameBossLayerController::updateBloodBar() {
	if (!isEnter() || nullptr == m_bloodBar) {
		return;
	}

	float rate = 0;
	if (GameData::getInstance()->getTargetScore() > 0) {
		rate = 1- GameData::getInstance()->getScore() * 1.0f /GameData::getInstance()->getTargetScore();
	}

	if (nullptr != m_bloodBar) {
		static const Vec2 offSize = m_bloodBar->getTextureRect().origin;
		static const float h = m_bloodBar->getContentSize().height;
		static const float w = m_bloodBar->getContentSize().width;
		float width = rate * w; 
		m_bloodBar->setTextureRect(CCRectMake(offSize.x, offSize.y, width, h));
	}
}


void GameBossLayerController::initTime() {
	int s = GameData::getInstance()->getTime();
	if (nullptr != m_time) {
		m_time->setString(String::createWithFormat("%d", s)->_string);
	}
}

void GameBossLayerController::updateTime() {
	if (GameManager::getInstance()->isStarted() && !GameManager::getInstance()->isPaused()) {
		int s = GameData::getInstance()->getTime();
		s -= 1;
		if (s <= s_time_count_down_tip && s>=0) {
			Audio::getInstance()->playDida();
		}

		if (s <= 0) {
			s = 0;
			GameManager::getInstance()->over();
		} else {
			GameData::getInstance()->addElapseTime(1);
		}

		if (nullptr != m_time) {
			m_time->setString(String::createWithFormat("%d", s)->_string);
		}

		GameData::getInstance()->setTime(s);
	}
}

void GameBossLayerController::resetBoss() {
	initalBoss();

	if (nullptr != m_boss) {
		m_boss->setPositionY(m_bossOriginPosY + 500);
		m_boss->setOpacity(255);
	}

	setHurtBossVisible(false);
}

void GameBossLayerController::onScoreChange() {
	if (!isEnter()) {
		return;
	}

	updateBloodBar();
}

void GameBossLayerController::on1sTimer() {
	if (!isEnter()) {
		return;
	}
	
	if (m_bStop) {
		return;
	}

	updateTime();
}

void GameBossLayerController::onTimeChange() {
	if (!isEnter()) {
		return;
	}

	updateTime();
}

void GameBossLayerController::onAttrackBoss() {
	playAttrackEffect();
}

void GameBossLayerController::playAttrackEffect() {
	Audio::getInstance()->playBossHurt();
	m_emitterBomb->resetSystem();
	if (nullptr!= m_boss) {
		ActionInterval *action = Sequence::create(
			CallFunc::create([=](){ setHurtBossVisible(true); }),
			DelayTime::create(0.2),
			CallFunc::create([=](){ setHurtBossVisible(false); }),
			NULL);

		m_boss->runAction(action);

		ActionInterval * shakeAction = Sequence::create(
			ScaleTo::create(0.1, 1.4, 1.4, 1),
			ScaleTo::create(0.1, 1, 1, 1),
			NULL);

		m_boss->runAction(shakeAction);
	} 
}

void GameBossLayerController::playBossShowEffect(CallFunc * callback) {
	if (nullptr == m_boss) {
		return;
	}

	Audio::getInstance()->playBossShow();
	const Size size = Director::getInstance()->getVisibleSize();
	Point midPos = Vec2(size.width * 0.5, size.height * 0.5);
	ccBezierConfig config;
	config.endPosition = Point(m_bossOriginPosX, m_bossOriginPosY);

	config.controlPoint_1 = Point(midPos.x, midPos.y + 50);
	config.controlPoint_2 = Point(midPos.x, midPos.y + 100);


	ActionInterval * showAction = Sequence::create(
		EaseSineOut::create(MoveTo::create(0.8, midPos)),
		ScaleTo::create(0.2, 5, 5, 1),
		Spawn::create(
			ScaleTo::create(0.8, 1, 1, 1),
			BezierTo::create(0.8, config),
			NULL
		),
		CallFunc::create([=](){
			playBossActiveEffect();
		}),
		callback,
		NULL);
	m_boss->runAction(showAction);
}

void GameBossLayerController::playBossDeathEffect() {
	Audio::getInstance()->playBossBomb();
	playBombEffect();
}

void GameBossLayerController::initBossBombParticleSystem() {
	if (nullptr == m_boss) {
		return;
	}

	m_emitterBomb = ParticleSystemQuad::create(s_particle_plist_boss_bomb);
	m_emitterBomb->setTexture(Director::getInstance()->getTextureCache()->addImage(s_particle_star));
	m_boss->addChild(m_emitterBomb, 100);
	m_emitterBomb->setPosition(Vec2(m_boss->getContentSize().width * 0.5, m_boss->getContentSize().height * 0.5));
	m_emitterBomb->stopSystem();
}

void GameBossLayerController::onGameOver() {
	stopBossActiveEffect();
	playBossDeathEffect();
}

void GameBossLayerController::playBombEffect() {
	if (nullptr == m_boss || nullptr == m_layer) {
		return;
	}
	
	Texture2D * txt2d = TextureCache::getInstance()->addImage(s_ani_bossbomb);
	if (nullptr == txt2d) {
		return;
	}

	float w = txt2d->getContentSize().width/s_ani_bossbomb_n;
	float h = txt2d->getContentSize().height;

	Animation *ani = Animation::create();
	ani->setDelayPerUnit(0.2);
	for (int i=0; i<s_ani_bomb_w_n; i++) {
		ani->addSpriteFrameWithTexture(txt2d, Rect(i*w, i*h, w, h));
	}

	Sprite * sprite = Sprite::create(s_ani_bossbomb, Rect(0,0,w,h));
	m_layer->addChild(sprite, m_boss->getZOrder()+1);
	sprite->setPosition(m_boss->getPositionX(), m_boss->getPositionY());

	sprite->runAction(Sequence::create(
		Animate::create(ani),
		CallFunc::create([=](){	
			m_layer->removeChild(sprite,true);
			if (nullptr != m_tipwinLayer) {
					m_tipwinLayer->enter();
			}
		}),
		NULL));

	m_boss->runAction(FadeOut::create(0.8));
}

void GameBossLayerController::initalBoss() {
	EnumBoss bossType = Boss_unknow;
	switch (GameData::getInstance()->getBossType()) {
	case Boss_Snow:
		bossType = Boss_Snow;
		break;
	case Boss_Bear:
		bossType = Boss_Bear;
		break;
	default:
		bossType = Boss_Snow;
		break;
	}

	auto bossSnow = dynamic_cast<Sprite*>(m_layer->getChildByName("boss"));
	auto bossBear = dynamic_cast<Sprite*>(m_layer->getChildByName("boss2"));

	if (Boss_Snow == bossType) {
		m_boss = bossSnow;
		bossSnow->setVisible(true);
		bossBear->setVisible(false);
		
		m_normalBoss_head = dynamic_cast<Sprite*>(m_layer->getChildByName("boss")
																	->getChildByName("normal_body")
																	->getChildByName("normal_head"));
		m_normallBoss_hand_left	= dynamic_cast<Sprite*>(m_layer->getChildByName("boss")
																	->getChildByName("normal_hand_left"));

		m_normallBoss_hand_right = dynamic_cast<Sprite*>(m_layer->getChildByName("boss")
																	->getChildByName("normal_hand_right"));

		m_hurtBoss_head = dynamic_cast<Sprite*>(m_normalBoss_head->getChildByName("hurt_head"));
		m_hurtBoss_hand_left = dynamic_cast<Sprite*>(m_normallBoss_hand_left->getChildByName("hurt_hand_left"));
		m_hurtBoss_hand_right = dynamic_cast<Sprite*>(m_normallBoss_hand_right->getChildByName("hurt_hand_right"));
		m_hurtBoss_body = dynamic_cast<Sprite*>(m_layer->getChildByName("boss")
														->getChildByName("normal_body")
														->getChildByName("hurt_body"));
	}

	if (Boss_Bear == bossType) {
		m_boss = bossBear;
		bossSnow->setVisible(false);
		bossBear->setVisible(true);
		m_normalBoss_head = dynamic_cast<Sprite*>(m_layer->getChildByName("boss2")
														->getChildByName("normal_body")
														->getChildByName("normal_head"));
		m_normallBoss_hand_left	= dynamic_cast<Sprite*>(m_layer->getChildByName("boss2")
																->getChildByName("normal_hand_left"));

		m_normallBoss_hand_right = dynamic_cast<Sprite*>(m_layer->getChildByName("boss2")
																->getChildByName("normal_hand_right"));

		m_hurtBoss_head = dynamic_cast<Sprite*>(m_normalBoss_head->getChildByName("hurt_head"));
		m_hurtBoss_hand_left = dynamic_cast<Sprite*>(m_normallBoss_hand_left->getChildByName("hurt_hand_left"));
		m_hurtBoss_hand_right = dynamic_cast<Sprite*>(m_normallBoss_hand_right->getChildByName("hurt_hand_right"));
		m_hurtBoss_body = dynamic_cast<Sprite*>(m_layer->getChildByName("boss2")
			->getChildByName("normal_body")
			->getChildByName("hurt_body"));
	}

	if (nullptr != m_normallBoss_hand_left) {
		m_normallBoss_hand_left->setRotation(-20);
	}

	if (nullptr != m_normallBoss_hand_right) {
		m_normallBoss_hand_right->setRotation(20);
	}


}

void GameBossLayerController::setHurtBossVisible(bool isVisible) {
	if (nullptr != m_hurtBoss_body) {
		m_hurtBoss_body->setVisible(isVisible);
	}

	if (nullptr != m_hurtBoss_head) {
		m_hurtBoss_head->setVisible(isVisible);
	}

	if (nullptr != m_hurtBoss_hand_left) {
		m_hurtBoss_hand_left->setVisible(isVisible);
	}

	if (nullptr != m_hurtBoss_hand_right) {
		m_hurtBoss_hand_right->setVisible(isVisible);
	}
}

void GameBossLayerController::playBossActiveEffect() {
	stopBossActiveEffect();

	if (nullptr != m_boss) {
		ActionInterval * jumpAction = RepeatForever::create(
			Sequence::create(
				MoveBy::create(0.1, Vec2(0, 5)),
				MoveBy::create(0.1, Vec2(0, -10)),
				MoveBy::create(0.1, Vec2(0, 5)),
			NULL) 
			);
		jumpAction->setTag(ACTION_JUMP);
		m_boss->runAction(jumpAction);
	}

	if (nullptr != m_normallBoss_hand_left) {
		ActionInterval * shakeAction = RepeatForever::create(
			Sequence::create(
			RotateBy::create(0.2, 10),
			RotateBy::create(0.2, -50),
			RotateBy::create(0.2, 40),
			DelayTime::create(1),
			NULL)
			);
		shakeAction->setTag(ACTION_SHAKE_HAND);
		m_normallBoss_hand_left->runAction(shakeAction);
	}

	if (nullptr != m_normallBoss_hand_right) {
		ActionInterval * shakeAction = RepeatForever::create(
			Sequence::create(
			RotateBy::create(0.2, -10),
			RotateBy::create(0.2, 50),
			RotateBy::create(0.2, -40),
			DelayTime::create(1),
			NULL)
			);
		shakeAction->setTag(ACTION_SHAKE_HAND);
		m_normallBoss_hand_right->runAction(shakeAction);
	}
}

void GameBossLayerController::stopBossActiveEffect() {
	if (nullptr != m_boss) {
		m_boss->stopActionByTag(ACTION_JUMP);
	}

	if (nullptr != m_normallBoss_hand_left) {
		m_normallBoss_hand_left->stopActionByTag(ACTION_SHAKE_HAND);
		m_normallBoss_hand_left->setRotation(-20);
	}

	if (nullptr != m_normallBoss_hand_right) {
		m_normallBoss_hand_right->stopActionByTag(ACTION_SHAKE_HAND);
		m_normallBoss_hand_right->setRotation(20);
	}
}