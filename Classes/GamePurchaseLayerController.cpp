#include "GamePurchaseLayerController.h"
#include "Audio.h"
#include "GameData.h"
#include "GameManager.h"
#include "PaymentInterface.h"
#include "SettingManager.h"
#include "GameEvent.h"

GamePurchaseLayerController::GamePurchaseLayerController():
m_toolImage(NULL),
m_toolType(Tool_Cross),
m_toolNum(NULL)
{


}

GamePurchaseLayerController::~GamePurchaseLayerController() {

}


void GamePurchaseLayerController::initial() {
	if (NULL == m_layer) {
		return;
	}

	m_toolNum = dynamic_cast<TextAtlas*>(m_layer->getChildByName("bg_mid")
											->getChildByName("bg_top")
											->getChildByName("panel")
											->getChildByName("num_tool"));

	m_toolImage = dynamic_cast<Sprite*>(m_layer->getChildByName("bg_mid")
												->getChildByName("bg_top")
												->getChildByName("panel")
												->getChildByName("tool"));

	auto yesBtn = dynamic_cast<Button*>(m_layer->getChildByName("bg_mid")
												->getChildByName("bg_bottom")
												->getChildByName("Btn_yes"));
	if (nullptr != yesBtn) {
		yesBtn->addClickEventListener([=](Ref*ref){
			Audio::getInstance()->playClick();
			exit();
			switch (m_toolType) {
			case Tool_Cross:
				purchaseCross();
				break;
			case Tool_Time:
				purchaseTime();
				break;
			case Tool_Tip:
				purchaseTip();
				break;
			case Tool_Bomb:
				purchaseBomb();
				break;
			case Tool_Energy:
				purchaseEnergy();
				break;
			default:
				break;
			}	
		});

		m_effectNode1 = yesBtn;
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
			} else {
				if (GameManager::getInstance()->isStarted() && GameManager::getInstance()->isPaused()) {
					GameManager::getInstance()->resume();
				}
			}
		});
	}
}

void GamePurchaseLayerController::purchase(EnumGameTool tool) {
	m_toolType = tool;

	if (nullptr != m_toolNum) {
		const int num = (Tool_Energy == tool)?25:5;
		m_toolNum->setString(String::createWithFormat(".%d", num)->_string);
	}

	if (nullptr != m_toolImage) {
		m_toolImage->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(getToolImage(tool)));
	}

	enter();
}

void GamePurchaseLayerController::enter() {
	if (NULL == m_layer) {
		return;
	}
	enterBg();
	const Size size = Director::getInstance()->getVisibleSize();
	m_layer->ignoreAnchorPointForPosition(false);
	m_layer->setAnchorPoint(Vec2(0.5,0.5));
	m_layer->setPosition(Vec2(size.width * 0.5, size.height * 0.5));

	m_bEnter = true;

	if (GameManager::getInstance()->isStarted()) {
		GameManager::getInstance()->pause();
	}

	playEffectNode();
}

void GamePurchaseLayerController::exit() {
	if (NULL == m_layer) {
		return;
	}
	exitBg();
	const Size size = Director::getInstance()->getVisibleSize();
	m_layer->ignoreAnchorPointForPosition(false);
	m_layer->setAnchorPoint(Vec2(0.5,0.5));
	m_layer->setPositionY(size.height * 2);
	m_bEnter = false;

	stopEffectNode();
}

void GamePurchaseLayerController::purchaseCross() {
	startPayment(Setting_buycross);
}

void GamePurchaseLayerController::purchaseTime() {
	startPayment(Setting_buytime);
}

void GamePurchaseLayerController::purchaseTip() {
	startPayment(Setting_buytip);
}

void GamePurchaseLayerController::purchaseBomb() {
	startPayment(Setting_buybomb);
}

void GamePurchaseLayerController::purchaseEnergy() {
	startPayment(Setting_buyenergy);
}

const char *GamePurchaseLayerController::getToolImage(EnumGameTool tool) {
	switch (tool) {
	case Tool_Cross:
		return s_tool_cross;
	case Tool_Time:
		return s_tool_time;
	case Tool_Tip:
		return s_tool_tip;
	case Tool_Bomb:
		return s_tool_bomb;
	case Tool_Energy:
		return s_tool_energy;
	default:
		break;
	}

	return nullptr;
}

void GamePurchaseLayerController::onPayBuyTimeComplete(bool success) {
	if (success) {
		GameData::getInstance()->addToolTime(5);
		Audio::getInstance()->playBuy();

		if (GameManager::getInstance()->isStarted() && GameManager::getInstance()->isPaused()) {
			GameManager::getInstance()->resume();
		}

		return;
	}

	if (nullptr != m_nextLayer) {
		m_nextLayer->enter();
	} else {
		if (GameManager::getInstance()->isStarted() && GameManager::getInstance()->isPaused()) {
			GameManager::getInstance()->resume();
		}
	}
}

void GamePurchaseLayerController::onPayBuyTipComplete(bool success) {
	if (success) {
		GameData::getInstance()->addToolTip(5);
		Audio::getInstance()->playBuy();

		if (GameManager::getInstance()->isStarted() && GameManager::getInstance()->isPaused()) {
			GameManager::getInstance()->resume();
		}

		return;
	}
	
	if (nullptr != m_nextLayer) {
		m_nextLayer->enter();
	} else {
		if (GameManager::getInstance()->isStarted() && GameManager::getInstance()->isPaused()) {
			GameManager::getInstance()->resume();
		}
	}
}

void GamePurchaseLayerController::onPayBuyBombComplete(bool success) {
	if (success) {
		GameData::getInstance()->addToolBomb(5);
		Audio::getInstance()->playBuy();

		if (GameManager::getInstance()->isStarted() && GameManager::getInstance()->isPaused()) {
			GameManager::getInstance()->resume();
		}

		return;
	}
	
	if (nullptr != m_nextLayer) {
		m_nextLayer->enter();
	} else {
		if (GameManager::getInstance()->isStarted() && GameManager::getInstance()->isPaused()) {
			GameManager::getInstance()->resume();
		}
	}
}

void GamePurchaseLayerController::onPayBuyCrossComplete(bool success) {
	if (success) {
		GameData::getInstance()->addToolCross(5);
		Audio::getInstance()->playBuy();

		if (GameManager::getInstance()->isStarted() && GameManager::getInstance()->isPaused()) {
			GameManager::getInstance()->resume();
		}

		return;
	}
	
	if (nullptr != m_nextLayer) {
		m_nextLayer->enter();
	} else {
		if (GameManager::getInstance()->isStarted() && GameManager::getInstance()->isPaused()) {
			GameManager::getInstance()->resume();
		}
	}
}

void GamePurchaseLayerController::onPayBuyEnergyComplete(bool success) {
	if (success) {
		GameData::getInstance()->addEnergy(25);
		Audio::getInstance()->playBuy();

		if (GameManager::getInstance()->isStarted() && GameManager::getInstance()->isPaused()) {
			GameManager::getInstance()->resume();
		}

		return;
	}
	
	if (nullptr != m_nextLayer) {
		m_nextLayer->enter();
	} else {
		if (GameManager::getInstance()->isStarted() && GameManager::getInstance()->isPaused()) {
			GameManager::getInstance()->resume();
		}
	}
}