#include "LevelStepTouchErr.h"

LevelStepTouchErr::LevelStepTouchErr(LevelController * lev):
	LevelStep(lev)
{

}

LevelStepTouchErr::~LevelStepTouchErr() {

}

void LevelStepTouchErr::init() {

}

void LevelStepTouchErr::onEnter() {
	showBgGrey(true);
	if (nullptr != m_levelController) {
		m_levelController->enterTouchErr();
	}
}

void LevelStepTouchErr::onExit() {
	showBgGrey(false);
}