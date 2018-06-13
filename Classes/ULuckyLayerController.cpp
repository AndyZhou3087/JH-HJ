#include "ULuckyLayerController.h"
#include "GameData.h"
#include "GameManager.h"
#include "GameUtil.h"
#include "cocos2d.h"
#include "UserData.h"
#include "UAwardLayerController.h"
#include "ULuckyGiftLayerController.h"
#include "LuckyTimeManager.h"
#include "SettingManager.h"
USING_NS_CC;

ULuckyLayerController::ULuckyLayerController():
m_dish(NULL),
m_spinBtn(NULL),
m_awardLayer(NULL),
m_giftLayer(NULL),
m_labelStart(NULL)
{

}

ULuckyLayerController::~ULuckyLayerController() {

}

void ULuckyLayerController::initial() {
	if (NULL == m_layer) {
		return;
	}

	m_dish = dynamic_cast<Sprite*>(m_layer->getChildByName("dish"));
	m_dish->setRotation(0);

	auto closeBtn = dynamic_cast<Button*>(m_layer->getChildByName("Btn_close"));
	if (nullptr != closeBtn) {
		closeBtn->addClickEventListener([=](Ref*ref){
			Audio::getInstance()->playClick();
			exit();
			if (nullptr != m_dish) {
				m_dish->stopAllActions();
			}
		});
	}

	m_spinBtn = dynamic_cast<Button*>(m_layer->getChildByName("reg")->getChildByName("Btn_spin"));
	if (nullptr != m_spinBtn) {
		m_spinBtn->addClickEventListener([=](Ref*ref){
			Audio::getInstance()->playClick();
			startLottery();
			Audio::getInstance()->playLucky();
			m_spinBtn->setEnabled(false);
			m_spinBtn->setBright(false);
			stopLabelStartEffect();
		});

		m_labelStart = dynamic_cast<Sprite*>(m_spinBtn->getChildByName("label_start"));
	}
}

void ULuckyLayerController::enter() {
	CenterLayerController::enter();
	LuckyTimeManager::getInstance()->saveTime();
	Audio::getInstance()->playLucky();
	if (nullptr != m_spinBtn) {
		m_spinBtn->setEnabled(false);
		m_spinBtn->setBright(false);
	}

	if (nullptr != m_dish) {
		m_dish->setRotation(0);
	}
	startLottery();
}

void ULuckyLayerController::exit() {
	CenterLayerController::exit();
	Audio::getInstance()->stopLucky();
	stopLabelStartEffect();
}

void ULuckyLayerController::startLottery() {
	if (UserData::getFirstLucky()) {
		if (GameUtil::getRandomBoolean()) {
			startAwardTime();
		} else {
			startAwardEnergy();
		}

		UserData::setFirstLucky(false);
	} else {
		EnumLuckyAward item = getRandomAwardItem();
		switch (item) {
			case Award_Gift:
				startAwardGift();
				break;
			case Award_Time_x1:
				startAwardTime();
				break;
			case Award_Lotter_x1:
				startAwardLotteryOne();
				break;
			case Award_Energy_x1:
				startAwardEnergy();
				break;
			default:
				break;
		}
	}
}

void ULuckyLayerController::startAwardTime() {
	if (nullptr == m_dish) {
		return;
	}

	float rotate =  360 - (int)(m_dish->getRotation())%360 + 1440 + GameUtil::getRandomNum(182, 223);
	m_dish->runAction(Sequence::create(
			EaseExponentialOut::create(RotateBy::create(6, rotate)),
			CallFunc::create([=](){
				exit();
				if (nullptr != m_awardLayer) {
					m_awardLayer->enterTime();
				}
			}),
			NULL));
}

void ULuckyLayerController::startAwardEnergy() {
	if (nullptr == m_dish) {
		return;
	}

	float rotate = 360 - (int)(m_dish->getRotation())%360 + 1440 + GameUtil::getRandomNum(227, 268);
	m_dish->runAction(Sequence::create(
			EaseExponentialOut::create(RotateBy::create(6, rotate)),
			CallFunc::create([=](){
				exit();
				if (nullptr != m_awardLayer) {				
					m_awardLayer->enterEnergy();
				}
		}),
			NULL));
}

void ULuckyLayerController::startAwardLotteryOne() {
	if (nullptr == m_dish) {
		return;
	}

	float rotate = 360 - (int)(m_dish->getRotation())%360 + 1440 + GameUtil::getRandomNum(47, 88);
	m_dish->runAction(Sequence::create(
			EaseExponentialOut::create(RotateBy::create(6, rotate)),
			CallFunc::create(
				[=](){
					if (nullptr != m_spinBtn) {
						m_spinBtn->setEnabled(true);
						m_spinBtn->setBright(true);
						playLabelStartEffect();
					}
				}
			),
			NULL));
}

void ULuckyLayerController::startAwardGift() {
	if (nullptr == m_dish) {
		return;
	}

	float rotate = 360 - (int)(m_dish->getRotation())%360 + 1440 + GameUtil::getRandomNum(272, 313);
	m_dish->runAction(Sequence::create(
			EaseExponentialOut::create(RotateBy::create(6, rotate)),
			CallFunc::create(
				[=](){
					exit();
					if (nullptr != m_giftLayer) {
						m_giftLayer->enter();
					}
				}
			),
			NULL));
}

EnumLuckyAward ULuckyLayerController::getRandomAwardItem() {
	std::random_device rd;
	std::mt19937 mt(rd());

	std::vector<EnumLuckyAward> vAwards;
	vAwards.push_back(Award_Energy_x1);//10%
	vAwards.push_back(Award_Time_x1);//10%
	vAwards.push_back(Award_Lotter_x1);//10%
	for (int i=0; i<7; i++) {
		vAwards.push_back(Award_Gift);
	}//70%
	// do random shuffle.
	std::shuffle(vAwards.begin(), vAwards.end(), mt);
	return vAwards.at(0);
}

void ULuckyLayerController::playLabelStartEffect() {
	if (nullptr == m_labelStart) {
		return;
	}

	m_labelStart->setScale(1);
	GameUtil::jump(m_labelStart, true, 1);
}

void ULuckyLayerController::stopLabelStartEffect() {
	if (nullptr == m_labelStart) {
		return;
	}

	m_labelStart->stopAllActions();
	m_labelStart->setScale(1);
}