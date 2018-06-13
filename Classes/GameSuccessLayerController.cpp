#include "GameSuccessLayerController.h"
#include "GameData.h"
#include "GameManager.h"
#include "GameLevelLayerController.h"
#include "UUnlockLayerController.h"
#include "cocos2d.h"
#include "GameEnergyTipLayerController.h"
#include "GameUiManager.h"
#include "UGiftMoreLayerController.h"
#include "GameUtil.h"
#include "global.h"
USING_NS_CC;

GameSuccessLayerController::GameSuccessLayerController():
m_title(NULL),
m_star1(NULL),
m_star2(NULL),
m_star3(NULL),
m_energyNum(NULL),
m_unlockLayer(NULL),
m_rank(NULL),
m_time(NULL),
m_percent(NULL),
m_energyTipLayer(NULL)
{

}

GameSuccessLayerController::~GameSuccessLayerController() {

}

void GameSuccessLayerController::initial() {
	if (NULL == m_layer) {
		return;
	}

	m_energyNum = dynamic_cast<TextAtlas *>(m_layer->getChildByName("bg_mid")
													->getChildByName("bg_panel")
													->getChildByName("num_energy"));

	m_title = dynamic_cast<Sprite *>(m_layer->getChildByName("bg_mid")
											->getChildByName("bg_top")
											->getChildByName("bg_title"));

	if (nullptr != m_title) {
		m_star1 = dynamic_cast<Sprite *>(m_title->getChildByName("star1"));
		if (nullptr != m_star1) {
			m_star1OriginPos = m_star1->getPosition();
		}

		m_star2 = dynamic_cast<Sprite *>(m_title->getChildByName("star2"));
		if (nullptr != m_star2) {
			m_star2OriginPos = m_star2->getPosition();
		}

		m_star3 = dynamic_cast<Sprite *>(m_title->getChildByName("star3"));
		if (nullptr != m_star3) {
			m_star3OriginPos = m_star3->getPosition();
		}
	}

	auto restartBtn = dynamic_cast<Button*>(m_layer->getChildByName("bg_mid")
													->getChildByName("bg_bottom")
													->getChildByName("Btn_restart"));
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

	auto nextBtn = dynamic_cast<Button*>(m_layer->getChildByName("bg_mid")
												->getChildByName("bg_bottom")
												->getChildByName("Btn_next"));
	if (nullptr != nextBtn) {
		nextBtn->addClickEventListener([=](Ref*ref){
			Audio::getInstance()->playClick();
			exit();

			if (!GameData::getInstance()->isLevelUnlock(GameData::getInstance()->getLevel() + 1)) {
				if (nullptr != m_unlockLayer) {
					m_unlockLayer->enter(GameData::getInstance()->getLevel() + 1);
				}

			} else {
				if (GameData::getInstance()->getEnergy() >= s_energy_use) {
					GameManager::getInstance()->next();
				} else {
					if (nullptr != m_energyTipLayer) {
						m_energyTipLayer->enter(EnergyTip_next);
					}
				}	
			}		
		});
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
	m_rank = dynamic_cast<TextAtlas*>(m_layer->getChildByName("bg_mid")
												->getChildByName("bg_panel")
												->getChildByName("num_rank"));
	m_time = dynamic_cast<TextAtlas*>(m_layer->getChildByName("bg_mid")
												->getChildByName("bg_panel")
												->getChildByName("bg_rank")->getChildByName("num_time"));
	m_percent = dynamic_cast<TextAtlas*>(m_layer->getChildByName("bg_mid")
												->getChildByName("bg_panel")->getChildByName("bg_rank")->getChildByName("num_percent"));
}

void GameSuccessLayerController::initStar() {
	if (nullptr != m_star1) {
		m_star1->setPosition(m_star1OriginPos);
		m_star1->setVisible(false);
	}

	if (nullptr != m_star2) {
		m_star2->setPosition(m_star2OriginPos);
		m_star2->setVisible(false);
	}

	if (nullptr != m_star3) {
		m_star3->setPosition(m_star3OriginPos);
		m_star3->setVisible(false);
	}
}

void GameSuccessLayerController::enter() {
	if (NULL == m_layer) {
		return;
	}

	if (nullptr != m_energyNum) {
		m_energyNum->setVisible(true);

		if ( 1 == GameData::getInstance()->getStar() ) {
			m_energyNum->setString(String::createWithFormat(".%d", s_1star_energy_add)->_string);
		} else if ( 2 == GameData::getInstance()->getStar() ) {
			m_energyNum->setString(String::createWithFormat(".%d", s_2star_energy_add)->_string);
		} else if ( 3 == GameData::getInstance()->getStar() ) {
			m_energyNum->setString(String::createWithFormat(".%d", s_3star_energy_add)->_string);
		} else {
			m_energyNum->setVisible(false);
		}
	}

	enterBg();
	const Size size = Director::getInstance()->getVisibleSize();
	m_layer->ignoreAnchorPointForPosition(false);
	m_layer->setAnchorPoint(Vec2(0.5,0.5));
	m_layer->setPosition(Vec2(size.width * 0.5, size.height * 0.5));

	m_bEnter = true;

	initStar();
	playStar1Effect();
	updateRankInfo();
	Audio::getInstance()->stopGameBGM();
	Audio::getInstance()->playWin();
	playEffectNode();

	//好礼多多
	if (SettingManager::getInstance()->isOpen(Setting_giftmore)) {
		auto gift = GameUiManager::getInstance()->getGiftMoreLayerController();
		if (nullptr != gift) {
			m_layer->runAction(Sequence::create(
				DelayTime::create(1),
				CallFunc::create([=](){
					if (isEnter()) {
						m_layer->setVisible(false);
						gift->setExitCallback([=](){
							m_layer->setVisible(true);
							gift->setExitCallback(nullptr);
						});
						gift->enter();
					}
				}),
				NULL));
		}
	}
}

void GameSuccessLayerController::exit() {
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

	//好礼多多
	if (SettingManager::getInstance()->isOpen(Setting_giftmore)) {
		auto gift = GameUiManager::getInstance()->getGiftMoreLayerController();
		if (nullptr != gift) {
			if (gift->isEnter()) {
				gift->setExitCallback(nullptr);
				gift->exit();
			}
		}
	}
}

void GameSuccessLayerController::updateRankInfo() {
	if (nullptr != m_rank) {
		int rank =  GameUtil::getRandomNum(1, 10000) + GameData::getInstance()->getTime() * 100000 /GameData::getInstance()->getTimelimit();
		m_rank->setString(String::createWithFormat("%d", rank)->_string);
	}

	if (nullptr != m_time) {
		m_time->setString(String::createWithFormat("%d", GameData::getInstance()->getElapseTime())->_string);
	}

	if (nullptr != m_percent) {
		int rank =  100 - GameData::getInstance()->getElapseTime() * 100 /GameData::getInstance()->getTimelimit();
		if (rank < 0) {
			rank = 1;
		}

		m_percent->setString(String::createWithFormat("%d/", rank)->_string);
	}
}

void GameSuccessLayerController::playStar1Effect() {
	if (nullptr == m_star1) {
		return;
	}

	m_star1->setScale(1.2);
	m_star1->setVisible(true);

	m_star1->runAction(Sequence::create(	
		ScaleTo::create(0.1, 1.5, 1.5, 1),
		Spawn::create(
			CallFunc::create([=](){
				Audio::getInstance()->playImpact();
			}),

			ScaleTo::create(0.1, 1, 1, 1),
			NULL),
		CallFunc::create([=](){
			playStarBombEffect(m_star1, -20);
			}),
		DelayTime::create(0.2),
		CallFunc::create([=](){
				if (GameData::getInstance()->getStar() > 1) {
					playStar2Effect();
				}
			}),
		NULL
		));
} 

void GameSuccessLayerController::playStar2Effect() {
	if (nullptr == m_star2) {
		return;
	}

	m_star2->setScale(1.2);
	m_star2->setVisible(true);

	m_star2->runAction(Sequence::create(
		ScaleTo::create(0.1, 1.5, 1.5, 1),
		Spawn::create(
		CallFunc::create([=](){
			Audio::getInstance()->playImpact();
		}),
		ScaleTo::create(0.1, 1, 1, 1),
		NULL),
		CallFunc::create([=](){
			playStarBombEffect(m_star2, 0);
		}),
		DelayTime::create(0.2),
		CallFunc::create([=](){
			if (GameData::getInstance()->getStar() > 2) {
				playStar3Effect();
			}
		}),
		NULL
		));
}

void GameSuccessLayerController::playStar3Effect() {
	if (nullptr == m_star3) {
		return;
	}

	m_star3->setScale(1.2);
	m_star3->setVisible(true);

	m_star3->runAction(Sequence::create(
		ScaleTo::create(0.1, 1.5, 1.5, 1),
		Spawn::create(
		CallFunc::create([=](){
			Audio::getInstance()->playImpact();
		}),
		ScaleTo::create(0.1, 1, 1, 1),
		NULL),
		CallFunc::create([=](){
			playStarBombEffect(m_star3, 20);
		}),
		NULL
		));
}
void GameSuccessLayerController::playStarBombEffect(cocos2d::Node *star, int offset) {
	if (nullptr == star) {
		return;
	}

	Texture2D * txt2d = TextureCache::getInstance()->addImage(s_ani_star_bomb);
	if (nullptr == txt2d) {
		return;
	}

	float w = txt2d->getContentSize().width/s_ani_starbomb_n;
	float h = txt2d->getContentSize().height;

	Animation * animation = Animation::create();
	animation->setDelayPerUnit(0.1);
	for (int i=0; i<s_ani_starbomb_n; i++) {
		animation->addSpriteFrameWithTexture(txt2d, Rect(i*w, 0, w, h));
	}

	auto bomb = Sprite::create(s_ani_star_bomb, Rect(0,0,w,h));
	star->getParent()->addChild(bomb, star->getZOrder() + 1);
	bomb->setPosition(star->getPosition().x + offset, star->getPosition().y + star->getContentSize().height * 0.5);
	bomb->runAction(Sequence::create(
		Animate::create(animation),
		CallFunc::create([=](){
			bomb->removeFromParentAndCleanup(true);
		}),
		NULL));
}