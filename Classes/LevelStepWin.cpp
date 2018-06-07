#include "LevelStepWin.h"

LevelStepWin::LevelStepWin(LevelController * lev):
	LevelStep(lev)
{

}

LevelStepWin::~LevelStepWin() {

}

void LevelStepWin::init() {

}

void LevelStepWin::onEnter() {
	if (nullptr != m_levelController) {
		m_levelController->showWinTip();
	}
}

void LevelStepWin::onExit() {

}