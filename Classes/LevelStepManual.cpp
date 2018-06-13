#include "LevelStepManual.h"

LevelStepToolConfirm::LevelStepToolConfirm(LevelController * lev):
LevelStep(lev)
{

}

LevelStepToolConfirm::~LevelStepToolConfirm() {

}

void LevelStepToolConfirm::init() {

}

void LevelStepToolConfirm::onEnter() {
	showBgGrey(true);
	if (nullptr != m_levelController) {
		m_levelController->showToolConfirm();
	}
}

void LevelStepToolConfirm::onExit() {
	showBgGrey(false);
	if (nullptr != m_levelController) {
	}
}
//////////////////////////////////////////////////////////////////////
LevelStepManual::LevelStepManual(LevelController * lev):
	LevelStep(lev)
{

}

LevelStepManual::~LevelStepManual() {

}

void LevelStepManual::init() {

}

void LevelStepManual::onEnter() {
	if (nullptr != m_levelController) {
		m_levelController->enterManual();
	}
}

void LevelStepManual::onExit() {
	if (nullptr != m_levelController) {
	}
}
