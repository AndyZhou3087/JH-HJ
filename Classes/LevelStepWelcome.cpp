#include "LevelStepWelcome.h"

LevelStepWelcome::LevelStepWelcome(LevelController * lev):
LevelStep(lev)
{

}

LevelStepWelcome::~LevelStepWelcome() {

}

void LevelStepWelcome::init() {

}

void LevelStepWelcome::onEnter() {
	showBgGrey(true);
	if (nullptr != m_levelController) {
		m_levelController->showWelcome(true);
	}
}

void LevelStepWelcome::onExit() {
	showBgGrey(false);
	if (nullptr != m_levelController) {
		m_levelController->showWelcome(false);
		m_levelController->guide();
	}
}

LevelStepWelcomeEnd::LevelStepWelcomeEnd(LevelController * lev):
	LevelStep(lev)
{

}

LevelStepWelcomeEnd::~LevelStepWelcomeEnd() {

}

void LevelStepWelcomeEnd::init() {

}

void LevelStepWelcomeEnd::onEnter() {
	showBgGrey(true);
	if (nullptr != m_levelController) {
		m_levelController->showWelcomeEnd();
	}
}

void LevelStepWelcomeEnd::onExit() {
	showBgGrey(false);
}