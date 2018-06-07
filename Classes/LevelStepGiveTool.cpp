#include "LevelStepGiveTool.h"

LevelStepGiveTool::LevelStepGiveTool(LevelController * lev):
	LevelStep(lev)
{

}

LevelStepGiveTool::~LevelStepGiveTool() {

}

void LevelStepGiveTool::init() {

}

void LevelStepGiveTool::onEnter() {
	showBgGrey(true);
	if (nullptr != m_levelController) {
		m_levelController->showToolConfirm();
	}
}

void LevelStepGiveTool::onExit() {
	showBgGrey(false);
}