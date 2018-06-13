#include "UAwardLayerController.h"
#include "GameData.h"
#include "GameManager.h"
#include "LuckyTimeManager.h"
#include "cocos2d.h"
USING_NS_CC;

UAwardLayerController::UAwardLayerController():
m_iconAward(NULL),
m_awardType(Award_Undefine)
{

}

UAwardLayerController::~UAwardLayerController() {

}

void UAwardLayerController::initial() {
	if (NULL == m_layer) {
		return;
	}

	m_iconAward = dynamic_cast<Sprite*>(m_layer->getChildByName("bg_mid")
												->getChildByName("bg_top")
												->getChildByName("panel")
												->getChildByName("icon_award"));

	auto yesBtn = dynamic_cast<Button*>(m_layer->getChildByName("bg_mid")
												->getChildByName("bg_bottom")
												->getChildByName("Btn_yes"));
	if (nullptr != yesBtn) {
		yesBtn->addClickEventListener([=](Ref*ref){
			Audio::getInstance()->playClick();
			exit();

			if (Award_Energy == m_awardType) {
				GameData::getInstance()->addEnergy(1);
			} else if (Award_Time == m_awardType) {
				GameData::getInstance()->addToolTime(1);
			}
		});

		m_effectNode1 = yesBtn;
	}
}

void UAwardLayerController::enterEnergy() {
	if (nullptr != m_iconAward) {
		m_iconAward->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(s_tool_energy));
	}

	m_awardType = Award_Energy;
	enterEaseSineOut();
}

void UAwardLayerController::enterTime() {
	if (nullptr != m_iconAward) {
		m_iconAward->setSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName(s_tool_time));
	}

	m_awardType = Award_Time;
	enterEaseSineOut();
}