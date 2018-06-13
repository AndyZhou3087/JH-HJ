#include "GameLevelLayerController.h"
#include "GameManager.h"
#include "GameTopLayerController.h"
#include "GameData.h"
#include "URookieLayerController.h"
#include "GameEnergyTipLayerController.h"
#include "EnergyManager.h"
#include "GamePurchaseLayerController.h"
#include "GameLevelDemoLayerController.h"
#include "ULuckyLayerController.h"
#include "UserData.h"
#include "GameUtil.h"
#include "LuckyTimeManager.h"
#include "UUnlockLayerController.h"
#include "SettingManager.h"
USING_NS_CC;

#define TAG_1STAR 100
#define TAG_2STAR 101
#define TAG_3STAR 102

#define TAG_LOCK "Btn_unlock"
#

GameLevelLayerController::GameLevelLayerController():
m_bgNode(NULL),
m_energyIcon(NULL),
m_bg1(NULL),
m_bg2(NULL),
m_bg3(NULL),
m_bg4(NULL),
m_bg5(NULL),
m_bg6(NULL),
m_happy(NULL),
m_lucky(NULL),
m_topPanel(NULL),
m_energyBar(NULL),
m_numEnergy(NULL),
m_starsBar(NULL),
m_numStars(NULL),
m_happyPosY(-100),
m_luckyPosY(-100),
m_rookieLayer(NULL),
m_energyTipLayer(NULL),
m_countdown(NULL),
m_arrowSprite(NULL),
m_purchaseLayer(NULL),
m_levelDemoLayer(NULL),
m_demoButton(NULL),
m_luckyLayer(NULL),
m_luckyCountdown(NULL),
m_luckyNode(NULL),
m_ring(NULL)
{

}

GameLevelLayerController::~GameLevelLayerController() {

}

void GameLevelLayerController::initial() {
	if (nullptr == m_layer) {
		return;
	}

	m_countdown = dynamic_cast<TextAtlas *> (m_layer->getChildByName("bg_top")
													->getChildByName("bg_energy")
													->getChildByName("countdown"));

	m_energyIcon = m_layer->getChildByName("bg_top")
							->getChildByName("bg_energy")
							->getChildByName("icon_energy");

	m_luckyCountdown = dynamic_cast<TextAtlas *> (m_layer->getChildByName("lucky_node")->getChildByName("label_lucky")
															->getChildByName("lucky_time"));

	m_scroll = dynamic_cast<ScrollView *> (m_layer->getChildByName("scroll"));
	if (nullptr != m_scroll) {
		m_scroll->scrollToBottom(0, false);
		m_bg6	= dynamic_cast<Sprite *>(m_scroll->getChildByName("bg_6"));	
		m_bg5	= dynamic_cast<Sprite *>(m_bg6->getChildByName("bg_5"));	
		m_bg4	= dynamic_cast<Sprite *>(m_bg5->getChildByName("bg_4"));
		m_bg3	= dynamic_cast<Sprite *>(m_bg4->getChildByName("bg_3"));
		m_bg2	= dynamic_cast<Sprite *>(m_bg3->getChildByName("bg_2"));
		m_bg1	= dynamic_cast<Sprite *>(m_bg2->getChildByName("bg_1"));
	}

	m_topPanel = dynamic_cast<Sprite *>(m_layer->getChildByName("bg_top"));
	if (nullptr != m_topPanel) {
		m_topPanel->setPositionY(Director::getInstance()->getVisibleSize().height);
		auto purchaseBtn = dynamic_cast<Button *>(m_topPanel->getChildByName("bg_energy")->getChildByName("Btn_purchase"));
		if (nullptr != purchaseBtn) {
			purchaseBtn->addClickEventListener([=](Ref*ref){
				Audio::getInstance()->playClick();
				if (nullptr != m_purchaseLayer) {
					m_purchaseLayer->purchase(Tool_Energy);
				}
			});

			if (!SettingManager::getInstance()->isOpen(Setting_addenergy)) {
				purchaseBtn->setVisible(false);
			}
		}
	}

	m_energyBar = dynamic_cast<Sprite *>(m_layer->getChildByName("bg_top")->getChildByName("bg_energy")->getChildByName("energy_bar"));
	m_numEnergy = dynamic_cast<TextAtlas *>(m_layer->getChildByName("bg_top")->getChildByName("bg_energy")->getChildByName("num_energy"));
	m_starsBar = dynamic_cast<Sprite *>(m_layer->getChildByName("bg_top")->getChildByName("bg_stars")->getChildByName("stars_bar"));
	m_numStars = dynamic_cast<TextAtlas *>(m_layer->getChildByName("bg_top")->getChildByName("bg_stars")->getChildByName("num_stars"));

	m_happy = dynamic_cast<Button*>(m_layer->getChildByName("happy_node")->getChildByName("Btn_happy"));
	m_happy->addClickEventListener([this](Ref*ref){
		Audio::getInstance()->playClick();
		if (nullptr != m_rookieLayer) {
			m_rookieLayer->enterHappy();
		}
	});

	m_happyPosY = m_happy->getPositionY();
	if (!SettingManager::getInstance()->isOpen(Setting_happy)) {
		m_layer->getChildByName("happy_node")->setVisible(false);
	}

	m_luckyNode = m_layer->getChildByName("lucky_node");
	m_lucky = dynamic_cast<Button*>(m_layer->getChildByName("lucky_node")->getChildByName("Btn_lucky"));
	m_lucky->addClickEventListener([this](Ref*ref){
		Audio::getInstance()->playClick();
		if (nullptr != m_luckyLayer) {
			m_luckyLayer->enter();
		}
	});

	m_luckyPosY = m_lucky->getPositionY();
	if (!SettingManager::getInstance()->isOpen(Setting_luckylottery)) {
		m_luckyNode->setVisible(false);
	}

	initBg();
	updateLuckyBtn();
}

void GameLevelLayerController::enter() {
	if (NULL == m_layer) {
		return;
	}

	const Size size = Director::getInstance()->getVisibleSize();
	m_layer->ignoreAnchorPointForPosition(false);
	m_layer->setAnchorPoint(Vec2(0.5,0));
	m_layer->setPosition(Vec2(size.width * 0.5, 0));

	updateBg();
	updateTopInfo();
	updateCountdown();
	updateLuckyBtn();
	playArrowEffect();
	playGiftEffect(m_happy, m_happyPosY);
	m_scroll->setVisible(true);
	m_bEnter = true;

	enterDemo();
	Audio::getInstance()->stopGameBGM();
	Audio::getInstance()->playHomeBGM();
}

void GameLevelLayerController::exit() {
	if (NULL == m_layer) {
		return;
	}

	if (nullptr != m_scroll) {
		m_scroll->setInertiaScrollEnabled(true);
	}

	const Size size = Director::getInstance()->getVisibleSize();
	m_layer->setPositionX(size.width * 2);

	m_bEnter = false;

	stopGiftEffect(m_happy, m_happyPosY);
	stopGiftEffect(m_lucky, m_luckyPosY);

	stopArrowEffect();
	m_scroll->setVisible(false);
	exitDemo();
	Audio::getInstance()->stopGameBGM();
}

void GameLevelLayerController::initBg() {
	if (nullptr == m_bg1 || nullptr == m_bg2 || nullptr == m_bg3 || nullptr == m_bg4 || nullptr == m_bg5 || nullptr == m_bg6) {
		return;
	}
	
	if (nullptr != m_scroll) {
		m_scroll->jumpToBottom();
	}

	Sprite *bgArray[] = {m_bg1, m_bg2, m_bg3, m_bg4, m_bg5};

	int lev = 0;
	for (int i=0; i<5; i++) {
		Sprite *bg = bgArray[i];
		for (int j=0; j<25; j++) {
			if (lev < GameData::MAX_LEVEL) {
				auto btn = dynamic_cast<Button*>(bg->getChildByName(String::createWithFormat("level_%d", j+1)->_string));
				if (0 == lev) {
					m_demoButton = btn;
				}
				initButton(lev, btn);
				lev++;
			}		
		}
		
	}
}

void GameLevelLayerController::updateTopInfo() {
	const int energy = GameData::getInstance()->getEnergy();
	const int maxEnergy = GameData::getInstance()->getMaxEnergy();
	
	if (nullptr != m_numEnergy) {
		m_numEnergy->setString(String::createWithFormat("%d/%d", energy, maxEnergy)->_string);
	}

	if (nullptr != m_energyBar) {
		static const Vec2 offSize = m_energyBar->getTextureRect().origin;
		static const float h = m_energyBar->getContentSize().height;
		static const float w = m_energyBar->getContentSize().width;
		float width = (energy * 1.0f/maxEnergy) * w; 
		if (width > w) {
			width = w;
		}
		m_energyBar->setTextureRect(CCRectMake(offSize.x, offSize.y, width, h));
	}

	const int starSum = GameData::getInstance()->getStarSum();
	const int maxStar = GameData::getInstance()->getMaxStarNum();

	if (nullptr != m_numStars) {
		m_numStars->setString(String::createWithFormat("%d/%d", starSum, maxStar)->_string);
	}

	if (nullptr != m_starsBar) {
		static const Vec2 offSize = m_starsBar->getTextureRect().origin;
		static const float h = m_starsBar->getContentSize().height;
		static const float w = m_starsBar->getContentSize().width;
		float width = (starSum * 1.0f/maxStar) * w; 
		m_starsBar->setTextureRect(CCRectMake(offSize.x, offSize.y, width, h));
	}

}

void GameLevelLayerController::updateLevelShow(cocos2d::Node *bg, int rate) {
	
}

void GameLevelLayerController::selectLevel(int level, Node * node) {
	if (!UserData::getGuidePass(level)) {
		if (isEnter()) {
			exit();
			GameManager::getInstance()->ready(level);
		}
	} else {
		if (GameData::getInstance()->getEnergy() >= s_energy_use) {
			playEnergyFlyEffect(node, CallFunc::create(
				[=](){
					if (isEnter()) {
						exit();
						GameManager::getInstance()->ready(level);
					}			
			}
			));

		} else {
			if (nullptr != m_energyTipLayer) {
				exit();
				m_energyTipLayer->enter();
			}
		}
	}
	
}

void GameLevelLayerController::initButton(int level, Button *btn) {
	if (nullptr == btn) {
		return;
	}

	btn->addClickEventListener([=](Ref*ref){
		Audio::getInstance()->playClick();
		selectLevel(level, btn);		
	});

	//LOCK
	if (level > 0 && (0 == level%25) && (level/25 <GameData::MAX_LOCK)) {
		auto lock = dynamic_cast<Button *>(btn->getParent()->getChildByName(TAG_LOCK));

		if (GameData::getInstance()->isLevelUnlock(level) && level - GameData::getInstance()->getHisLevel() < 25) {
			lock->setVisible(true);
		} else {
			lock->setVisible(false);
		}

		lock->addClickEventListener([=](Ref*ref){
			Audio::getInstance()->playClick();
			if (nullptr != m_unlockLayer) {
				m_unlockLayer->enter(level);
			}
		});
	}

	auto s1 = btn->getChildByTag(TAG_1STAR);
	if (nullptr == s1) {
		s1 = Sprite::createWithSpriteFrameName(s_icon_star_blue);
		btn->addChild(s1);
		s1->setAnchorPoint(Vec2(0.5, 0));
		s1->setTag(TAG_1STAR);
		s1->setPosition(btn->getContentSize().width * 0.5 - 15, btn->getContentSize().height + 5);
		s1->setVisible(false);
	} else {
		s1->setVisible(false);
	}

	auto s2 = btn->getChildByTag(TAG_2STAR);
	if (nullptr == s2) {
		s2 = Sprite::createWithSpriteFrameName(s_icon_star_blue);
		btn->addChild(s2);
		s2->setAnchorPoint(Vec2(0.5, 0));
		s2->setTag(TAG_2STAR);
		s2->setPosition(btn->getContentSize().width * 0.5, btn->getContentSize().height + 5);
		s2->setVisible(false);
	} else {
		s2->setVisible(false);
	}

	auto s3 = btn->getChildByTag(TAG_3STAR);
	if (nullptr == s3) {
		s3 = Sprite::createWithSpriteFrameName(s_icon_star_blue);
		btn->addChild(s3);
		s3->setAnchorPoint(Vec2(0.5, 0));
		s3->setTag(TAG_3STAR);
		s3->setPosition(btn->getContentSize().width * 0.5 + 15, btn->getContentSize().height + 5);
		s3->setVisible(false);
	} else {
		s3->setVisible(false);
	}
}

void GameLevelLayerController::updateBg() {
	Sprite *bgArray[] = {m_bg1, m_bg2, m_bg3, m_bg4, m_bg5};

	int lev = 0;
	for (int i=0; i<5; i++) {
		Sprite *bg = bgArray[i];
		for (int j=0; j<25; j++) {
			if (lev < GameData::MAX_LEVEL) {
				auto btn = dynamic_cast<Button*>(bg->getChildByName(String::createWithFormat("level_%d", j+1)->_string));
				updateButton(lev, btn);			
			}	

			lev++;
		}

	}
}

void GameLevelLayerController::updateButton(int level, Button *btn) {
	if (nullptr == btn || level < 0 || level >= GameData::MAX_LEVEL) {
		return;
	}

	const int star			= GameData::getInstance()->getStar(level);
	const int hisLevel		= GameData::getInstance()->getHisLevel();
	const int hisLevelStar	= GameData::getInstance()->getStar(hisLevel);
	const bool bUnlock		= GameData::getInstance()->isLevelUnlock(level);
	const int bossType		= GameData::getInstance()->getBossType(level);

	if (bossType > 0) {
		btn->loadTextures(s_btn_boss_normal, s_btn_boss_press, s_btn_boss_disable, TextureResType::PLIST);
		auto num = btn->getChildByName("num_level");
		if (num != nullptr) {
			num->setVisible(false);
		}
	}

	//LOCK
	if (level > 0 && (0 == level%25) && (level/25 <GameData::MAX_LOCK)) {
		auto lock = btn->getParent()->getChildByName(TAG_LOCK);
		
		if ( !bUnlock && (level - GameData::getInstance()->getHisLevel() <= 25)) {
			lock->setVisible(true);
		} else {
			lock->setVisible(false);
		}
	}
	
	if (bUnlock && level <= hisLevel + 1 && hisLevelStar > 0
		|| (level == hisLevel && 0 == hisLevelStar)) {
		btn->setEnabled(true);
		btn->setBright(true);
		btn->setTouchEnabled(true);
		auto s1 = dynamic_cast<Sprite*>(btn->getChildByTag(TAG_1STAR));
		if (nullptr != s1) {
			if (star > 0) {
				s1->setVisible(true);
				auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName((star>0)?s_icon_star_yellow:s_icon_star_blue);
				s1->setSpriteFrame(frame);
			} else {
				s1->setVisible(false);
			}
		}

		auto s2 = dynamic_cast<Sprite*>(btn->getChildByTag(TAG_2STAR));
		if (nullptr != s2) {
			if (star > 0) {
				s2->setVisible(true);
				auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName((star>1)?s_icon_star_yellow:s_icon_star_blue);
				s2->setSpriteFrame(frame);
			} else {
				s2->setVisible(false);
			}
		}

		auto s3 = dynamic_cast<Sprite*>(btn->getChildByTag(TAG_3STAR));
		if (nullptr != s3) {
			if (star > 0) {
				s3->setVisible(true);
				auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName((star>2)?s_icon_star_yellow:s_icon_star_blue);
				s3->setSpriteFrame(frame);
			} else {
				s3->setVisible(false);
			}
		}
	} else {
		btn->setEnabled(false);
		btn->setBright(false);
		btn->setTouchEnabled(false);

		auto s1 = btn->getChildByTag(TAG_1STAR);
		if (nullptr != s1) s1->setVisible(false);

		auto s2 = btn->getChildByTag(TAG_2STAR);
		if (nullptr != s2) s2->setVisible(false);

		auto s3 = btn->getChildByTag(TAG_3STAR);
		if (nullptr != s3) s3->setVisible(false);
	}

	if (btn->isEnabled() 
		&& ((level == GameData::getInstance()->getHisLevel() + 1)
			|| (0 == level && 0 == star)
			|| (GameData::getInstance()->isLevelUnlock(level) && !(GameData::getInstance()->isLevelUnlock(level+1))))) {
		if (nullptr != m_arrowSprite) {
			m_arrowSprite->removeFromParentAndCleanup(true);
			m_arrowSprite = nullptr;
		}

		m_arrowSprite = Sprite::createWithSpriteFrameName(s_icon_arrow);
		btn->addChild(m_arrowSprite);
		m_arrowSprite->setGlobalZOrder(100);
		m_arrowSprite->setAnchorPoint(Vec2(0.5, 0));
		m_arrowSprite->setPosition(btn->getContentSize().width * 0.5, btn->getContentSize().height + 20);

		//playRingEffect(btn->getParent(), btn->getPosition(), btn->getZOrder()-1);
	}
}

void GameLevelLayerController::on1sTimer() {
	updateCountdown();

	if (nullptr != m_lucky && nullptr != m_luckyNode && m_luckyNode->isVisible()) {
		updateLuckyTime();
		if (LuckyTimeManager::getInstance()->getCountDown() <= 0) {
		if (!m_lucky->isEnabled()) {
				m_lucky->setEnabled(true);
				m_lucky->setBright(true);
				playGiftEffect(m_lucky, m_luckyPosY);
			}

			if (m_luckyCountdown->isVisible()) {
				m_luckyCountdown->setVisible(false);
			}	

			if ((UserData::getGuidePass(GameData::getInstance()->getLevel())) 
				&& isEnter()
				&& LuckyTimeManager::getInstance()->isUnLotteryed()
				&& nullptr != m_luckyLayer && !m_luckyLayer->isEnter()) {
				m_luckyLayer->enter();
			}
		} else {
			if (m_lucky->isEnabled()) {
				m_lucky->setEnabled(false);
				m_lucky->setBright(false);
				stopGiftEffect(m_lucky, m_luckyPosY);
			}
			
			if (!m_luckyCountdown->isVisible()) {
				m_luckyCountdown->setVisible(true);
			}		
		}
	}

}

void GameLevelLayerController::updateCountdown() {
	if (nullptr == m_countdown) {
		return;
	}

	if (GameData::getInstance()->getEnergy() >= s_energy_recover_limit) {
		m_countdown->setVisible(false);
		return;
	}

	m_countdown->setVisible(true);
	long sec = EnergyManager::getInstance()->getCountDown();
	if (sec < 0) { sec = 0; }
	if (sec > s_energy_count_down) { sec = s_energy_count_down; }

	m_countdown->setString(String::createWithFormat("%02d/%02d", sec/60, sec%60)->_string);
}

void GameLevelLayerController::onEnergyChange() {
	updateTopInfo();
}

void GameLevelLayerController::playArrowEffect() {
	if (nullptr == m_arrowSprite) {
		return;
	}

	m_arrowSprite->stopAllActions();
	m_arrowSprite->runAction(RepeatForever::create(
		Sequence::create(
			MoveBy::create(0.6, Vec2(0, -10)),
			MoveBy::create(0.6, Vec2(0,10)),
			NULL
		)
		));
}

void GameLevelLayerController::stopArrowEffect() {
	if (nullptr == m_arrowSprite) {
		return;
	}

	m_arrowSprite->stopAllActions();
}

void GameLevelLayerController::enterDemo() {
	if (UserData::getGuidePass(0) || GameData::getInstance()->getStar(0)>0) {
		return;
	}

	if (nullptr != m_levelDemoLayer && !UserData::getGuidePass(0)) {
		if (nullptr != m_demoButton) {
			if (nullptr != m_scroll) {
				m_scroll->setInertiaScrollEnabled(false);
			}

			m_levelDemoLayer->enter(m_demoButton->getPosition() - Point(0, m_demoButton->getContentSize().height * 0.5));
		}
	}
}

void GameLevelLayerController::exitDemo() {
	if (nullptr != m_scroll) {
		m_scroll->setInertiaScrollEnabled(true);
	}

	if (nullptr != m_levelDemoLayer && m_levelDemoLayer->isEnter()) {
		m_levelDemoLayer->exit();
	}
}

void GameLevelLayerController::playGiftEffect(Button * gift, float originY) {
	if (NULL != gift) {
		gift->setPositionY(originY);
		GameUtil::jellyJump(gift, 40, true, 1);
	}
}

void GameLevelLayerController::stopGiftEffect(Button * gift, float originY) {
	if (NULL != gift) {
		gift->stopAllActions();
		gift->setPositionY(originY);
	}
}

void GameLevelLayerController::updateLuckyTime() {
	if (nullptr == m_lucky || m_lucky->isEnabled()) {
		return;
	}

	if (nullptr != m_luckyCountdown) {
		const long long countdown = LuckyTimeManager::getInstance()->getCountDown();
		int h = countdown/3600;
		int m = (countdown%3600)/60;
		int s = countdown%60;
		m_luckyCountdown->setString(String::createWithFormat("%02d/%02d/%02d", h,m,s )->_string);
	}
}

void GameLevelLayerController::updateLuckyBtn() {
	if (nullptr == m_lucky) {
		return;
	}

	if (LuckyTimeManager::getInstance()->isUnLotteryed()) {
		m_lucky->setEnabled(true);
		m_lucky->setBright(true);
		playGiftEffect(m_lucky, m_luckyPosY);
	
		if (nullptr != m_luckyCountdown) {
			m_luckyCountdown->setVisible(false);
		}
	} else {
		m_lucky->setEnabled(false);
		m_lucky->setBright(false);
		stopGiftEffect(m_lucky, m_luckyPosY);

		if (nullptr != m_luckyCountdown) {
			m_luckyCountdown->setVisible(true);
		}
	}
}

void GameLevelLayerController::onLevelUnlock() {
	updateBg();
	stopArrowEffect();
	playArrowEffect();
}

void GameLevelLayerController::playRingEffect(Node *parent, const Point&pos, int zorder) {
	if (nullptr != m_ring) {
		m_ring->removeFromParentAndCleanup(true);
		m_ring = nullptr;
	}

	Texture2D * txt2d = TextureCache::getInstance()->addImage(s_ani_ring2);
	if (nullptr == txt2d) {
		return;
	}

	float w = txt2d->getContentSize().width/s_ani_ring2_n;
	float h = txt2d->getContentSize().height;

	Animation * aniRing = Animation::create();
	aniRing->setDelayPerUnit(0.1);
	for (int i=0; i<s_ani_ring2_n; i++) {
		aniRing->addSpriteFrameWithTexture(txt2d, Rect(i*w, 0, w, h));
	}

	m_ring = Sprite::createWithSpriteFrame(SpriteFrame::createWithTexture(txt2d, Rect(0,0,w,h)));
	parent->addChild(m_ring, zorder);
	m_ring->setPosition(pos);
	m_ring->runAction(RepeatForever::create(Animate::create(aniRing)));
}

void GameLevelLayerController::stopRingEffect() {
	if (nullptr != m_ring) {
		m_ring->stopAllActions();
	}
}

void GameLevelLayerController::playEnergyFlyEffect(Node * dstNode, const CallFunc *callback) {
	if (nullptr == dstNode 
		|| nullptr == m_layer
		|| nullptr == m_energyIcon) {
		return;
	}

	const Point src = m_energyIcon->getParent()->convertToWorldSpace(m_energyIcon->getPosition());
	const Point dst = dstNode->getParent()->convertToWorldSpace(dstNode->getPosition());

	ccBezierConfig config;
	config.endPosition = m_energyIcon->getPosition() + (dst - src);

	config.controlPoint_1 = m_energyIcon->getPosition();
	config.controlPoint_2 = config.controlPoint_1 + Vec2(200, -20);
	
	auto energy = Sprite::createWithSpriteFrameName(s_icon_energy_small);
	m_energyIcon->getParent()->addChild(energy);
	energy->setZOrder(1000);
	energy->setPosition(m_energyIcon->getPosition());
	energy->runAction(Sequence::create(
		FadeIn::create(0.2),
		EaseSineIn::create(BezierTo::create(0.8, config)),
		FadeOut::create(0.1),
		CallFunc::create([=](){
			energy->removeFromParentAndCleanup(true);
		}),
		callback,
		NULL
		));
}