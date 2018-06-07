#include "MenuController.h"
#include "global.h"
#include "GameUtil.h"

MenuController::MenuController():
m_Menu(NULL),
m_bEnter(false),
m_homeMenu(NULL),
m_effectNode1(NULL),
m_effectNode2(NULL),
m_effectNode3(NULL),
m_effectNode4(NULL)
{

}

MenuController::~MenuController() {

}

void MenuController::enter() {
	if (NULL == m_Menu) {
		return;
	}

	m_Menu->ignoreAnchorPointForPosition(false);
	m_Menu->setAnchorPoint(Vec2(0.5,0));
	m_Menu->setPosition(Vec2(WIN_WIDTH * 0.5,0));
	m_bEnter = true;

	playEffectNode();
}

void MenuController::exit() {
	if (NULL == m_Menu) {
		return;
	}

	m_Menu->setPosition(Vec2(2 * WIN_WIDTH,0));
	m_bEnter = false;

	stopEffectNode();
}

void MenuController::regMenu(Node *m) {
	m_Menu = m;
	initial();
}

void MenuController::initial() {
	if (NULL == m_Menu) {
		return;
	}

	static const Size size = Director::getInstance()->getVisibleSize();
	m_Menu->ignoreAnchorPointForPosition(true);
	m_Menu->setAnchorPoint(Vec2(0.5, 0.5));
	m_Menu->setPosition(size.width * 0.5, size.height * 0.5);
}

void MenuController::playEffectNode() {
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
}

void MenuController::stopEffectNode() {
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
}