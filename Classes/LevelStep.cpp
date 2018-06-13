#include "LevelStep.h"
#include "GameUiManager.h"
#include "GreyLayer.h"

LevelStep::LevelStep(LevelController * lev):
m_next(NULL),
m_levelController(lev),
m_bEnter(false)
{

}

LevelStep::~LevelStep() {

}

void LevelStep::init() {

}

void LevelStep::enter() {
	m_bEnter = true;
	onEnter();
}

void LevelStep::exit() {
	if (!m_bEnter) {
		return;
	}

	onExit();
	m_bEnter = false;
	if (nullptr != m_next) {
		m_next->enter();
	}
}

void LevelStep::finish() {
	if (!m_bEnter) {
		return;
	}

	onExit();
	m_bEnter = false;
}

void LevelStep::showBgGrey(bool visible) {
	auto bg = GameUiManager::getInstance()->getBgGreyLayerController();
	if (nullptr != bg) {
		if (visible) {
			bg->enter();
		} else {
			bg->exit();
		}
	}
}

void LevelStep::setBgGreyTouchable(bool enable) {
	auto bg = GameUiManager::getInstance()->getBgGreyLayerController();
	if (nullptr != bg) {
		bg->setTouchable(enable);
	}
}