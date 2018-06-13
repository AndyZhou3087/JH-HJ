#include "LevelStepTip.h"

LevelStepTipDetail::LevelStepTipDetail(LevelController * lev):
	LevelStep(lev)
{

}

LevelStepTipDetail::~LevelStepTipDetail() {

}

void LevelStepTipDetail::init() {

}

void LevelStepTipDetail::onEnter() {
	showBgGrey(true);
	if (nullptr != m_levelController) {
		m_levelController->showTipDetail(true);
	}
}

void LevelStepTipDetail::onExit() {
	showBgGrey(false);
}

//////////////////////////////////////////////////////////////////////////////////////////
LevelStepTipFocus::LevelStepTipFocus(LevelController * lev):
	LevelStep(lev)
{

}

LevelStepTipFocus::~LevelStepTipFocus() {

}

void LevelStepTipFocus::init() {

}

void LevelStepTipFocus::onEnter() {
	if (nullptr != m_levelController) {
		m_levelController->enterTipFocus();
	}
}

void LevelStepTipFocus::onExit() {
}

//////////////////////////////////////////////////////////////////////////////////////////
LevelStepTipGuide::LevelStepTipGuide(LevelController * lev):
	LevelStep(lev)
{

}

LevelStepTipGuide::~LevelStepTipGuide() {

}

void LevelStepTipGuide::init() {

}

void LevelStepTipGuide::onEnter() {
	if (nullptr != m_levelController) {
		m_levelController->enterTipGuide();
	}
}

void LevelStepTipGuide::onExit() {
}

//////////////////////////////////////////////////////////////////////////////////////////
LevelStepTipManual::LevelStepTipManual(LevelController * lev):
	LevelStep(lev)
{

}

LevelStepTipManual::~LevelStepTipManual() {

}

void LevelStepTipManual::init() {

}

void LevelStepTipManual::onEnter() {
	if (nullptr != m_levelController) {
		m_levelController->enterTipManual();
	}
}

void LevelStepTipManual::onExit() {
}