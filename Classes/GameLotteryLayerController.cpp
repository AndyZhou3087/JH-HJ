#include "GameLotteryLayerController.h"
#include "UBigAwardLayerController.h"
#include "GameData.h"
#include "GameUtil.h"
#include "cocos2d.h"
#include "SettingManager.h"
USING_NS_CC;

#define TAG_AWARD	100

GameLotteryLayerController::GameLotteryLayerController():
m_card1_front(NULL),
m_card1_back(NULL),
m_card2_front(NULL),
m_card2_back(NULL),
m_card3_front(NULL),
m_card3_back(NULL),
m_card4_front(NULL),
m_card4_back(NULL),
m_bIsCard1ActionDone(true),
m_bIsCard2ActionDone(true),
m_bIsCard3ActionDone(true),
m_bIsCard4ActionDone(true),
m_selected(false),
m_close(NULL),
m_get(NULL),
m_openall(NULL),
m_card1Selected(false),
m_card2Selected(false),
m_card3Selected(false),
m_card4Selected(false),
m_state(State_Free),
m_bigAwardLayer(NULL),
m_disfreeTip(NULL)
{
	m_award.push_back(Card_Cross);
	m_award.push_back(Card_Gift);
	m_award.push_back(Card_time1);
	m_award.push_back(Card_time3);
}

GameLotteryLayerController::~GameLotteryLayerController() {

}

void GameLotteryLayerController::initial() {
	if (NULL == m_layer) {
		return;
	}

	m_disfreeTip = m_layer->getChildByName("bg")->getChildByName("tip");
	m_close = dynamic_cast<Button*>(m_layer->getChildByName("bg")->getChildByName("Btn_close"));
	if (nullptr != m_close) {
		m_close->addClickEventListener([this](Ref*ref){
			Audio::getInstance()->playClick();
			if (State_Openall == m_state) {
				openAll();
				enterGetall();
			} else if (State_GetAll == m_state || State_BuyedAll == m_state) {
				exit();
				if (nullptr != m_nextLayer) {
					m_nextLayer->enter();
				}
			}		
		});
	}

	m_get = dynamic_cast<Button*>(m_layer->getChildByName("bg")->getChildByName("Btn_yes"));
	if (nullptr != m_get) {
		m_get->addClickEventListener([this](Ref*ref){
			Audio::getInstance()->playClick();
			if (m_state == State_Free) {
				if (!getAward()) {
					enterOpenall();
				} else {
					exit();
					if (nullptr != m_bigAwardLayer) {
						Audio::getInstance()->playBigAward();
						m_bigAwardLayer->enter();
					}
				}		
			} else if (m_state == State_GetAll) {
				//领取全部
				exit();
				startPayment(Setting_fetchall);
				//if (nullptr != m_nextLayer) {
				//	m_nextLayer->enter();
				//}
			}		
		});

		m_effectNode1 = m_get;
	}

	m_openall = dynamic_cast<Button*>(m_layer->getChildByName("bg")->getChildByName("Btn_openall"));
	if (nullptr != m_openall) {
		m_openall->addClickEventListener([this](Ref*ref){
			Audio::getInstance()->playClick();
			//openAll();
			startPayment(Setting_allopen);
		});

		m_effectNode2 = m_openall;
	}

	m_card1_front = dynamic_cast<Sprite*>(m_layer->getChildByName("bg")->getChildByName("card_1_front"));
	m_card1_back = dynamic_cast<Button*>(m_layer->getChildByName("bg")->getChildByName("card_1_back"));
	if (nullptr != m_card1_back) {
		m_card1_back->addClickEventListener([this](Ref*ref){
			if (m_selected) {
				return;
			}
			Audio::getInstance()->playClick();
			bool isGift = GameUtil::getRandomBoolean(0.65);
			startOrbitAction(0, isGift, true);
		});
	}

	m_card2_front = dynamic_cast<Sprite*>(m_layer->getChildByName("bg")->getChildByName("card_2_front"));
	m_card2_back = dynamic_cast<Button*>(m_layer->getChildByName("bg")->getChildByName("card_2_back"));
	if (nullptr != m_card2_back) {
		m_card2_back->addClickEventListener([this](Ref*ref){
			if (m_selected) {
				return;
			}
			Audio::getInstance()->playClick();
			bool isGift = GameUtil::getRandomBoolean(0.65);
			startOrbitAction(1, isGift, true);
		});
	}

	m_card3_front = dynamic_cast<Sprite*>(m_layer->getChildByName("bg")->getChildByName("card_3_front"));
	m_card3_back = dynamic_cast<Button*>(m_layer->getChildByName("bg")->getChildByName("card_3_back"));
	if (nullptr != m_card3_back) {
		m_card3_back->addClickEventListener([this](Ref*ref){
			if (m_selected) {
				return;
			}
			Audio::getInstance()->playClick();
			bool isGift = GameUtil::getRandomBoolean(0.65);
			startOrbitAction(2, isGift, true);
		});
	}

	m_card4_front = dynamic_cast<Sprite*>(m_layer->getChildByName("bg")->getChildByName("card_4_front"));
	m_card4_back = dynamic_cast<Button*>(m_layer->getChildByName("bg")->getChildByName("card_4_back"));
	if (nullptr != m_card4_back) {
		m_card4_back->addClickEventListener([this](Ref*ref){
			if (m_selected) {
				return;
			}
			Audio::getInstance()->playClick();
			bool isGift = GameUtil::getRandomBoolean(0.65);
			startOrbitAction(3, isGift, true);
		});
	}

	initAllCard();
}

void GameLotteryLayerController::initAllCard() {
	randomCard();

	Button * backCard[]		= { m_card1_back, m_card2_back, m_card3_back, m_card4_back};
	Sprite * frontCard[]	= { m_card1_front, m_card2_front, m_card3_front, m_card4_front};
	bool selected[]			= { m_card1Selected, m_card2Selected, m_card3Selected, m_card4Selected};

	for (int i=0; i<sizeof(backCard)/sizeof(backCard[0]); i++) {
		if (nullptr != backCard[i]) {
			backCard[i]->setVisible(true);
			if (selected[i]) {
				recoverBackCard(backCard[i]);
			}
		}
	}

	for (int i=0; i<sizeof(frontCard)/sizeof(frontCard[0]); i++) {
		if (nullptr != frontCard[i]) {
			frontCard[i]->setVisible(false);
			initCard(frontCard[i], m_award.at(i));
			if (selected[i]) {
				recoverFrontCard(frontCard[i]);
			}
		}
	}

	m_selected = false;
	m_bIsCard1ActionDone = true;
	m_bIsCard2ActionDone = true;
	m_bIsCard3ActionDone = true;
	m_bIsCard4ActionDone = true;

	m_card1Selected = false;
	m_card2Selected = false;
	m_card3Selected = false;
	m_card4Selected = false;

}

void GameLotteryLayerController::initFreeUi() {
	if (nullptr != m_close) {
		m_close->setVisible(false);
	}

	if (nullptr != m_get) {
		m_get->setVisible(false);
	}

	if (nullptr != m_openall) {
		m_openall->setVisible(false);
	}

	if (nullptr != m_disfreeTip) {
		m_disfreeTip->setVisible(false);
	}
}

void GameLotteryLayerController::initOpenallUi() {
	if (nullptr != m_close) {
		m_close->setVisible(true);
	}

	if (nullptr != m_get) {
		m_get->setVisible(false);
	}

	if (nullptr != m_openall) {
		m_openall->setVisible(true);
	}

	if (nullptr != m_disfreeTip) {
		m_disfreeTip->setVisible(true);
	}
}

void GameLotteryLayerController::initGetallUi() {
	if (nullptr != m_close) {
		m_close->setVisible(true);
	}

	if (nullptr != m_get) {
		m_get->setVisible(true);
		m_get->setEnabled(true);
		m_get->setBright(true);
	}

	if (nullptr != m_openall) {
		m_openall->setVisible(false);
	}

	if (nullptr != m_disfreeTip) {
		m_disfreeTip->setVisible(true);
	}
}

void GameLotteryLayerController::initBuyedallUi() {
	if (nullptr != m_close) {
		m_close->setVisible(true);
	}

	if (nullptr != m_get) {
		m_get->setVisible(false);
	}

	if (nullptr != m_openall) {
		m_openall->setVisible(false);
	}

	if (nullptr != m_disfreeTip) {
		m_disfreeTip->setVisible(false);
	}
}

void GameLotteryLayerController::enterFree() {
	initFreeUi();
	m_state = State_Free;
}

void GameLotteryLayerController::enterOpenall() {
	initOpenallUi();
	m_state = State_Openall;
}

void GameLotteryLayerController::enterGetall() {
	initGetallUi();
	m_state = State_GetAll;
}

void GameLotteryLayerController::enterBuyedAll() {
	initBuyedallUi();
	m_state = State_BuyedAll;
}



void GameLotteryLayerController::initCard(Sprite *card, EnumCardAward award) {
	if (nullptr == card) {
		return;
	}

	card->removeChildByTag(TAG_AWARD);
	Sprite *icon = Sprite::createWithSpriteFrameName(getIconByAward(award));
	if (nullptr != icon) {
		icon->setTag(TAG_AWARD);
		card->addChild(icon);
		icon->setPosition(card->getContentSize().width * 0.5, card->getContentSize().height * 0.5);
	}
}

void GameLotteryLayerController::randomCard() {
	std::random_device rd;
	std::mt19937 mt(rd());

	// do random shuffle.
	std::shuffle(m_award.begin(), m_award.end(), mt);
}

void GameLotteryLayerController::enter() {
	if (NULL == m_layer) {
		return;
	}
	enterBg();
	const Size size = Director::getInstance()->getVisibleSize();
	m_layer->ignoreAnchorPointForPosition(false);
	m_layer->setAnchorPoint(Vec2(0.5,0.5));
	m_layer->setPosition(Vec2(size.width * 0.5, size.height * 0.5));
	m_bEnter = true;
	enterFree();
	initAllCard();
	Audio::getInstance()->playWelcome();
	playEffectNode();
}

void GameLotteryLayerController::exit() {
	if (NULL == m_layer) {
		return;
	}
	exitBg();
	const Size size = Director::getInstance()->getVisibleSize();
	m_layer->ignoreAnchorPointForPosition(false);
	m_layer->setAnchorPoint(Vec2(0.5,0.5));
	m_layer->setPositionX(size.width * 2);
	m_bEnter = false;
	Audio::getInstance()->stopGameBGM();
	stopEffectNode();
}

//翻牌动作  
void GameLotteryLayerController::startOrbitAction(int cardIndex, bool enableSwap, bool enableSwapTime3)  
{
	Button * backCard[]		= { m_card1_back, m_card2_back, m_card3_back, m_card4_back};
	Sprite * frontCard[]	= { m_card1_front, m_card2_front, m_card3_front, m_card4_front};
	bool * isDone[]			= {&m_bIsCard1ActionDone, &m_bIsCard2ActionDone, &m_bIsCard3ActionDone, &m_bIsCard4ActionDone};

	const int size = sizeof(backCard)/sizeof(backCard[0]);
	if (cardIndex < 0 || cardIndex >= size
		|| cardIndex >= sizeof(frontCard)/sizeof(frontCard[0])) {
			return;
	}

	Button * back = backCard[cardIndex];
	Sprite * front = frontCard[cardIndex];
	if (nullptr ==  back || nullptr == front) {
		return;
	}
	//换牌
	if (enableSwap) {
		swapGiftCard(Card_Gift, cardIndex);
	} else if (enableSwapTime3){
		checkCard(cardIndex);
	}
 
	float orbitTime = 0.5;  
	if (!front->isVisible() && (*isDone[cardIndex])) {  
		(*isDone[cardIndex]) = false; 
		m_selected = true;
		//第一个参数是旋转的时间，第二个参数是起始半径，第三个参数半径差，第四个参数是起始Z角，第五个参数是旋转Z角差，第六个参数是起始X角，最后一个参数旋转X角差，  
		CCOrbitCamera* orbitFront = CCOrbitCamera::create(orbitTime, 1, 0, 270, 90, 0, 0);  
		CCSequence* sequenceFront = CCSequence::createWithTwoActions(CCShow::create(), orbitFront);  
		CCTargetedAction* targetFront = CCTargetedAction::create(front, sequenceFront);  

		CCOrbitCamera* orbitBack = CCOrbitCamera::create(orbitTime, 1, 0, 0, 90, 0, 0);  
		back->runAction(CCSequence::create(orbitBack, 
											CCHide::create(), 
											targetFront, 
											CallFunc::create([=](){
												selectCard(cardIndex);
												if (m_state == State_Free) {
													if (!getAward()) {
														enterOpenall();
													} else {
														exit();
														if (nullptr != m_bigAwardLayer) {
															m_bigAwardLayer->enter();
														}
													}		
												} 

											}),NULL));  
	}
}  

void GameLotteryLayerController::selectCard(int cardIndex) {
	bool * isDone[]	= {&m_bIsCard1ActionDone, &m_bIsCard2ActionDone, &m_bIsCard3ActionDone, &m_bIsCard4ActionDone};
	(*isDone[cardIndex]) = true;

	bool * isSelected[]	= {&m_card1Selected, &m_card2Selected, &m_card3Selected, &m_card4Selected};
	(*isSelected[cardIndex]) = true;
}

const char * GameLotteryLayerController::getIconByAward(EnumCardAward award) {
	switch (award)
	{
	case Card_Cross:
		return s_icon_award_cross;
	case Card_Gift:
		return s_icon_award_gift;
	case Card_time1:
		return s_icon_award_time1;
	case Card_time3:
		return s_icon_award_time3;
	default:
		break;
	}

	return nullptr;
}

void GameLotteryLayerController::recoverFrontCard(cocos2d::Node *card) {
	if (nullptr == card) {
		return;
	}

	card->runAction(CCOrbitCamera::create(0, 1, 0, 0, 90, 0, 0));  
}

void GameLotteryLayerController::recoverBackCard(cocos2d::Node *card) {
	if (nullptr == card) {
		return;
	}

	card->runAction(CCOrbitCamera::create(0, 1, 0, 270, 90, 0, 0));  
}

void GameLotteryLayerController::openAll() {
	if (!m_card1Selected) {
		startOrbitAction(0);
	}

	if (!m_card2Selected) {
		startOrbitAction(1);
	}

	if (!m_card3Selected) {
		startOrbitAction(2);
	}

	if (!m_card4Selected) {
		startOrbitAction(3);
	}
}

bool GameLotteryLayerController::getAward() {
	bool exitGift = false;
	if (m_card1Selected) {
		exitGift = getAward(m_award.at(0)) || exitGift;
	}

	if (m_card2Selected) {
		exitGift = getAward(m_award.at(1)) || exitGift;
	}

	if (m_card3Selected) {
		exitGift = getAward(m_award.at(2)) || exitGift;
	}

	if (m_card4Selected) {
		exitGift = getAward(m_award.at(3)) || exitGift;
	}

	return exitGift;
}

bool GameLotteryLayerController::getAward(EnumCardAward award) {
	switch (award)
	{
	case Card_Cross:
		GameData::getInstance()->addToolCross(1);
		break;
	case Card_Gift:
		return true;
	case Card_time1:
		GameData::getInstance()->addToolTime(1);
		break;
	case Card_time3:
		GameData::getInstance()->addToolTime(3);
		break;
	default:
		break;
	}

	return false;
}

//换礼物卡到指定位置
void GameLotteryLayerController::swapGiftCard(EnumCardAward award, int index) {
	if (index < 0 || index >= m_award.size()) {
		return;
	}

	if (award == m_award[index]) {
		return;
	}

	int i=0;
	for (; i<m_award.size(); i++) {
		if (award == m_award[i]) {
			break;
		}
	}

	if (i == index) {
		return;
	}

	if (i<m_award.size()) {
		swapCard(i, index);
	}
}

void GameLotteryLayerController::checkCard(int index) {
	if (index < 0 || index >= m_award.size()) {
		return;
	}

	auto award = m_award[index];
	if (Card_time3 == award) {
		auto swapAward = (GameUtil::getRandomBoolean())?Card_Cross:Card_time1;
		swapGiftCard(swapAward, index);
	}
}

void GameLotteryLayerController::swapCard(int src, int dst) {
	if (src < 0 || src >= m_award.size()) {
		return;
	}

	if (dst < 0 || dst >= m_award.size()) {
		return;
	}

	EnumCardAward tmp = m_award[src];
	m_award[src] = m_award[dst];
	m_award[dst] = tmp;

	Sprite * frontCard[] = { m_card1_front, m_card2_front, m_card3_front, m_card4_front};
	initCard(frontCard[src], m_award[src]);
	initCard(frontCard[dst], m_award[dst]);
}

void GameLotteryLayerController::onPayAllCardOpenComplete(bool success) {
	if (success) {
		GameData::getInstance()->setLotteryFlag(true, GameData::getInstance()->getLevel());
		openAll();
		GameData::getInstance()->addToolCross(1);
		GameData::getInstance()->addToolTime(4);
		GameData::getInstance()->addToolCross(5);
		GameData::getInstance()->addToolBomb(3);
		GameData::getInstance()->addToolTime(5);
		GameData::getInstance()->addToolTip(3);
		GameData::getInstance()->addEnergy(10);
		Audio::getInstance()->playBuy();
		enterBuyedAll();
	} else {
		exit();
		if (nullptr != m_nextLayer) {
			m_nextLayer->enter();
		}
	}
}

void GameLotteryLayerController::onPayFetchAllComplete(bool success) {
	if (success) {
		GameData::getInstance()->setLotteryFlag(true, GameData::getInstance()->getLevel());
		GameData::getInstance()->addToolCross(1);
		GameData::getInstance()->addToolTime(4);
		GameData::getInstance()->addToolCross(5);
		GameData::getInstance()->addToolBomb(3);
		GameData::getInstance()->addToolTime(5);
		GameData::getInstance()->addToolTip(3);
		GameData::getInstance()->addEnergy(10);
		Audio::getInstance()->playBuy();
	}
		
	exit();
	if (nullptr != m_nextLayer) {
		m_nextLayer->enter();
	}
}