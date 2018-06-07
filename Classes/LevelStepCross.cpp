#include "LevelStepCross.h"

LevelStepCrossDetail::LevelStepCrossDetail(LevelController * lev):
	LevelStep(lev)
{

}

LevelStepCrossDetail::~LevelStepCrossDetail() {

}

void LevelStepCrossDetail::init() {

}

void LevelStepCrossDetail::onEnter() {
	showBgGrey(true);
	if (nullptr != m_levelController) {
		m_levelController->showCrossDetail(true);
	}
}

void LevelStepCrossDetail::onExit() {
	showBgGrey(false);
}

//////////////////////////////////////////////////////////////////////////////////////////
LevelStepCrossFocus::LevelStepCrossFocus(LevelController * lev):
	LevelStep(lev)
{

}

LevelStepCrossFocus::~LevelStepCrossFocus() {

}

void LevelStepCrossFocus::init() {

}

void LevelStepCrossFocus::onEnter() {
	if (nullptr != m_levelController) {
		m_levelController->enterCrossFocus();
	}
}

void LevelStepCrossFocus::onExit() {
}

//////////////////////////////////////////////////////////////////////////////////////////
LevelStepCrossGuide::LevelStepCrossGuide(LevelController * lev):
	LevelStep(lev)
{

}

LevelStepCrossGuide::~LevelStepCrossGuide() {

}

void LevelStepCrossGuide::init() {

}

void LevelStepCrossGuide::onEnter() {
	if (nullptr != m_levelController) {
		m_levelController->enterCrossGuide();
	}
}

void LevelStepCrossGuide::onExit() {
}

//////////////////////////////////////////////////////////////////////////////////////////
LevelStepCrossManual::LevelStepCrossManual(LevelController * lev):
	LevelStep(lev)
{

}

LevelStepCrossManual::~LevelStepCrossManual() {

}

void LevelStepCrossManual::init() {

}

void LevelStepCrossManual::onEnter() {
	if (nullptr != m_levelController) {
		m_levelController->enterCrossManual();
	}
}

void LevelStepCrossManual::onExit() {
}