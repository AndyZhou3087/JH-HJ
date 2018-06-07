#include "LevelStepBomb.h"

LevelStepBombDetail::LevelStepBombDetail(LevelController * lev):
	LevelStep(lev)
{

}

LevelStepBombDetail::~LevelStepBombDetail() {

}

void LevelStepBombDetail::init() {

}

void LevelStepBombDetail::onEnter() {
	showBgGrey(true);
	if (nullptr != m_levelController) {
		m_levelController->showBombDetail(true);
	}
}

void LevelStepBombDetail::onExit() {
	showBgGrey(false);
}

//////////////////////////////////////////////////////////////////////////////////////////
LevelStepBombFocus::LevelStepBombFocus(LevelController * lev):
	LevelStep(lev)
{

}

LevelStepBombFocus::~LevelStepBombFocus() {

}

void LevelStepBombFocus::init() {

}

void LevelStepBombFocus::onEnter() {
	if (nullptr != m_levelController) {
		m_levelController->enterBombFocus();
	}
}

void LevelStepBombFocus::onExit() {
}

//////////////////////////////////////////////////////////////////////////////////////////
LevelStepBombManual::LevelStepBombManual(LevelController * lev):
	LevelStep(lev)
{

}

LevelStepBombManual::~LevelStepBombManual() {

}

void LevelStepBombManual::init() {

}

void LevelStepBombManual::onEnter() {
	if (nullptr != m_levelController) {
		m_levelController->enterBombManual();
	}
}

void LevelStepBombManual::onExit() {
}