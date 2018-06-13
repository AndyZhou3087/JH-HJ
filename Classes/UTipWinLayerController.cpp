#include "UTipWinLayerController.h"
#include "GameData.h"
#include "GameManager.h"
#include "GameUtil.h"
#include "cocos2d.h"
#include "SettingManager.h"
#include "GameEvent.h"
USING_NS_CC;

UTipWinLayerController::UTipWinLayerController():
m_light(NULL),
m_label(NULL)
{

}

UTipWinLayerController::~UTipWinLayerController() {

}

void UTipWinLayerController::initial() {
	if (NULL == m_layer) {
		return;
	}

	m_light = m_layer->getChildByName("bg_win");
	m_bgOriginY = m_light->getPositionY();

	m_label = m_layer->getChildByName("label_win");
	m_labelOriginY = m_label->getPositionY();
}

void UTipWinLayerController::enter() {
	CenterLayerController::enter();
	playWinEffect();
}

void UTipWinLayerController::exit() {
	CenterLayerController::exit();
}

void UTipWinLayerController::playWinEffect() {
	if (nullptr == m_light || nullptr == m_label) {
		return;
	}

	m_light->setOpacity(0);
	m_light->setScale(0.1,0.1);
	m_label->setPositionY(m_labelOriginY + 500);
	m_label->runAction(Sequence::create(
		MoveBy::create(0.5, Vec2(0,-500)),
		CallFunc::create([=](){
			GameUtil::jelly(m_label);
			m_light->runAction(Spawn::create(
				FadeIn::create(0.5),
				ScaleTo::create(0.5, 1,1),
				RotateBy::create(1, Vec3(0, 0, 360)),
				NULL
			));
		}),

		NULL));
}