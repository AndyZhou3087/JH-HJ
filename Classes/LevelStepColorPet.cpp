#include "LevelStepColorPet.h"

LevelStepColorPetGuide::LevelStepColorPetGuide(LevelController * lev):
	LevelStep(lev)
{

}

LevelStepColorPetGuide::~LevelStepColorPetGuide() {

}

void LevelStepColorPetGuide::init() {

}

void LevelStepColorPetGuide::onEnter() {
	if (nullptr != m_levelController) {
		m_levelController->enterColorPetGuide();
	}
}

void LevelStepColorPetGuide::onExit() {
}

//////////////////////////////////////////////////////////////////////////////////////////
LevelStepColorPetManual::LevelStepColorPetManual(LevelController * lev):
	LevelStep(lev)
{

}

LevelStepColorPetManual::~LevelStepColorPetManual() {

}

void LevelStepColorPetManual::init() {

}

void LevelStepColorPetManual::onEnter() {
	if (nullptr != m_levelController) {
		m_levelController->enterColorPetManual();
	}
}

void LevelStepColorPetManual::onExit() {
}