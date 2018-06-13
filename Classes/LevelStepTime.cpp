#include "LevelStepTime.h"

LevelStepTimeDetail::LevelStepTimeDetail(LevelController * lev):
	LevelStep(lev)
{

}

LevelStepTimeDetail::~LevelStepTimeDetail() {

}

void LevelStepTimeDetail::init() {

}

void LevelStepTimeDetail::onEnter() {
	showBgGrey(true);
	if (nullptr != m_levelController) {
		m_levelController->showTimeDetail(true);
	}
}

void LevelStepTimeDetail::onExit() {
	showBgGrey(false);
}

//////////////////////////////////////////////////////////////////////////////////////////
LevelStepTimeFocus::LevelStepTimeFocus(LevelController * lev):
	LevelStep(lev)
{

}

LevelStepTimeFocus::~LevelStepTimeFocus() {

}

void LevelStepTimeFocus::init() {

}

void LevelStepTimeFocus::onEnter() {
	if (nullptr != m_levelController) {
		m_levelController->enterTimeFocus();
	}
}

void LevelStepTimeFocus::onExit() {
}

//////////////////////////////////////////////////////////////////////////////////////////
LevelStepTimeGuide::LevelStepTimeGuide(LevelController * lev):
	LevelStep(lev)
{

}

LevelStepTimeGuide::~LevelStepTimeGuide() {

}

void LevelStepTimeGuide::init() {

}

void LevelStepTimeGuide::onEnter() {
	if (nullptr != m_levelController) {
		m_levelController->enterTimeGuide();
	}
}

void LevelStepTimeGuide::onExit() {
}

//////////////////////////////////////////////////////////////////////////////////////////
LevelStepTimeManual::LevelStepTimeManual(LevelController * lev):
	LevelStep(lev)
{

}

LevelStepTimeManual::~LevelStepTimeManual() {

}

void LevelStepTimeManual::init() {

}

void LevelStepTimeManual::onEnter() {
	if (nullptr != m_levelController) {
		m_levelController->enterTimeManual();
	}
}

void LevelStepTimeManual::onExit() {
}