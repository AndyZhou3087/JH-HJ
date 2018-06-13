#include "LevelStepTarget.h"

LevelStepTarget::LevelStepTarget(LevelController * lev):
	LevelStep(lev)
{

}

LevelStepTarget::~LevelStepTarget() {

}

void LevelStepTarget::init() {

}

void LevelStepTarget::onEnter() {
	showBgGrey(true);
	if (nullptr != m_levelController) {
		if (m_levelController->isBossLevel()) {
			m_levelController->showTargetBoss();
		} else {
			m_levelController->showTarget();
		}
 		
	}
}

void LevelStepTarget::onExit() {
	showBgGrey(false);
}
