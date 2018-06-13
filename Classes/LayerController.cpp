#include "LayerController.h"
#include "global.h"
#include "GameUtil.h"
#include "Resource.h"


LayerController::LayerController():
m_layer(NULL),
m_nextLayer(NULL),
m_exitLayer(NULL),
m_bEnter(false),
m_bgLayer(NULL),
m_effectNode1(NULL),
m_effectNode2(NULL),
m_effectNode3(NULL),
m_effectNode4(NULL),
m_effectNode5(NULL),
m_effectNode6(NULL)
{

}

LayerController::~LayerController() {

}

void LayerController::enter() {
	if (NULL == m_layer) {
		return;
	}

	if (m_bEnter) {
		return;
	}

	m_layer->ignoreAnchorPointForPosition(false);
	m_layer->setAnchorPoint(Vec2(0.5,0));
	m_layer->setPosition(Vec2(WIN_WIDTH * 0.5,0));
	m_bEnter = true;

	playEffectNode();
}

void LayerController::exit() {
	if (NULL == m_layer) {
		return;
	}

	if (!m_bEnter) {
		return;
	}

	m_layer->setPosition(Vec2(2 * WIN_WIDTH,0));
	m_bEnter = false;

	stopEffectNode();
}

void LayerController::regLayer(Node *l) {
	m_layer = l;
	initial();
}

void LayerController::enterBg() {
	if (nullptr != m_bgLayer) {
		m_bgLayer->enter();
	}
}

void LayerController::exitBg() {
	if (nullptr != m_bgLayer) {
		m_bgLayer->exit();
	}
}

void LayerController::playEffectNode() {
	if (nullptr != m_effectNode1) {
		m_effectNode1->setScale(1,1);
		m_effectNode1->stopAllActions();
		GameUtil::jelly(m_effectNode1, true, 1.5, true);
	}

	if (nullptr != m_effectNode2) {
		m_effectNode2->setScale(1,1);
		m_effectNode2->stopAllActions();
		GameUtil::jelly(m_effectNode2, true, 1.8, true);
	}

	if (nullptr != m_effectNode3) {
		m_effectNode3->setScale(1,1);
		m_effectNode3->stopAllActions();
		GameUtil::jelly(m_effectNode3, true, 2, true);
	}

	if (nullptr != m_effectNode4) {
		m_effectNode4->setScale(1,1);
		m_effectNode4->stopAllActions();
		GameUtil::jelly(m_effectNode4, true, 2.2, true);
	}

	if (nullptr != m_effectNode5) {
		m_effectNode5->setScale(1,1);
		m_effectNode5->stopAllActions();
		GameUtil::jelly(m_effectNode5, true, 1.6, true);
	}

	if (nullptr != m_effectNode6) {
		m_effectNode6->setScale(1,1);
		m_effectNode6->stopAllActions();
		GameUtil::jelly(m_effectNode6, true, 0.5, true);
	}
}

void LayerController::stopEffectNode() {
	if (nullptr != m_effectNode1) {	
		m_effectNode1->stopAllActions();
		m_effectNode1->setScale(1,1);
	}

	if (nullptr != m_effectNode2) {
		m_effectNode2->stopAllActions();
		m_effectNode2->setScale(1,1);
	}

	if (nullptr != m_effectNode3) {
		m_effectNode3->stopAllActions();
		m_effectNode3->setScale(1,1);
	}

	if (nullptr != m_effectNode4) {
		m_effectNode4->stopAllActions();
		m_effectNode4->setScale(1,1);
	}

	if (nullptr != m_effectNode5) {
		m_effectNode5->stopAllActions();
		m_effectNode5->setScale(1,1);
	}

	if (nullptr != m_effectNode6) {
		m_effectNode6->stopAllActions();
		m_effectNode6->setScale(1,1);
	}
}


	//m_emitterBlue	= initParticleSystem(Color_Blue);
	//m_emitterGreen	= initParticleSystem(Color_Green);
	//m_emitterRed	= initParticleSystem(Color_Red);
	//m_emitterYellow = initParticleSystem(Color_Yellow);















void BottomLayerController::enter() {
	if (NULL == m_layer) {
		return;
	}

	enterBg();
	const Size size = Director::getInstance()->getVisibleSize();
	m_layer->ignoreAnchorPointForPosition(false);
	m_layer->setAnchorPoint(Vec2(0.5,0));
	m_layer->setPosition(Vec2(size.width * 0.5, 0));

	m_bEnter = true;

	playEffectNode();
}

void BottomLayerController::exit() {
	if (NULL == m_layer) {
		return;
	}

	exitBg();
	const Size size = Director::getInstance()->getVisibleSize();
	m_layer->ignoreAnchorPointForPosition(false);
	m_layer->setAnchorPoint(Vec2(0.5, 0));
	m_layer->setPositionX(size.width * 2);
	m_bEnter = false;

	stopEffectNode();
}

void CenterLayerController::enter() {
	if (NULL == m_layer) {
		return;
	}

	enterBg();
	const Size size = Director::getInstance()->getVisibleSize();
	m_layer->ignoreAnchorPointForPosition(false);
	m_layer->setAnchorPoint(Vec2(0.5,0.5));
	m_layer->setPosition(Vec2(size.width * 0.5, size.height * 0.5));

	m_bEnter = true;
	playEffectNode();
}

void CenterLayerController::enterEaseSineOut() {
	if (NULL == m_layer) {
		return;
	}
	enterBg();
	const Size size = Director::getInstance()->getVisibleSize();
	m_layer->ignoreAnchorPointForPosition(false);
	m_layer->setAnchorPoint(Vec2(0.5,0.5));
	m_layer->setPosition(Vec2(size.width * 0.5, size.height * 2));

	m_bEnter = true;

	ActionInterval * action = EaseSineOut::create(MoveTo::create(0.5, Vec2(size.width * 0.5, size.height * 0.5)));
	m_layer->runAction(action);
}

void CenterLayerController::exit() {
	if (NULL == m_layer) {
		return;
	}

	exitBg();
	const Size size = Director::getInstance()->getVisibleSize();
	m_layer->ignoreAnchorPointForPosition(false);
	m_layer->setAnchorPoint(Vec2(0.5, 0));
	m_layer->setPositionX(size.width * 2);
	m_bEnter = false;

	stopEffectNode();
}