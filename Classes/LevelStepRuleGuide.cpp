#include "LevelStepRuleGuide.h"

LevelStepRuleGuide::LevelStepRuleGuide(LevelController * lev):
	LevelStep(lev)
{

}

LevelStepRuleGuide::~LevelStepRuleGuide() {

}

void LevelStepRuleGuide::init() {

}

void LevelStepRuleGuide::onEnter() {
	if (nullptr != m_levelController) {
		m_levelController->showRule(true);
	}
}

void LevelStepRuleGuide::onExit() {
	if (nullptr != m_levelController) {
		m_levelController->showRule(false);
	}
}