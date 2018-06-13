#include "LevelStepBomPet.h"

LevelStepBomPetGuide::LevelStepBomPetGuide(LevelController * lev):
	LevelStep(lev)
{

}

LevelStepBomPetGuide::~LevelStepBomPetGuide() {

}

void LevelStepBomPetGuide::init() {

}

void LevelStepBomPetGuide::onEnter() {
	if (nullptr != m_levelController) {
		m_levelController->enterBomPetGuide();
	}
}

void LevelStepBomPetGuide::onExit() {
}

//////////////////////////////////////////////////////////////////////////////////////////
LevelStepBomPetManual::LevelStepBomPetManual(LevelController * lev):
	LevelStep(lev)
{

}

LevelStepBomPetManual::~LevelStepBomPetManual() {

}

void LevelStepBomPetManual::init() {

}

void LevelStepBomPetManual::onEnter() {
	if (nullptr != m_levelController) {
		m_levelController->enterBomPetManual();
	}
}

void LevelStepBomPetManual::onExit() {
}