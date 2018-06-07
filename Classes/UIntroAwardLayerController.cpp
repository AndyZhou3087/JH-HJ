#include "UIntroAwardLayerController.h"
#include "GameData.h"
#include "GameManager.h"
#include "LuckyTimeManager.h"
#include "GameLevelLayerController.h"
#include "cocos2d.h"
USING_NS_CC;

UIntroAwardLayerController::UIntroAwardLayerController()
{

}

UIntroAwardLayerController::~UIntroAwardLayerController() {

}

void UIntroAwardLayerController::initial() {
	if (NULL == m_layer) {
		return;
	}

	auto yesBtn = dynamic_cast<Button*>(m_layer->getChildByName("bg_mid")
												->getChildByName("bg_bottom")
												->getChildByName("Btn_yes"));
	if (nullptr != yesBtn) {
		yesBtn->addClickEventListener([=](Ref*ref){
			Audio::getInstance()->playClick();
			GameData::getInstance()->addToolCross(2);
			GameData::getInstance()->addToolBomb(2);
			exit();

			if (nullptr != m_levelLayer) {
				m_levelLayer->enter();
			}
		});

		m_effectNode1 = yesBtn;
	}
}