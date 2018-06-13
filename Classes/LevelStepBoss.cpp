#include "LevelStepBoss.h"

LevelStepBossEnter::LevelStepBossEnter(LevelController * lev):
	LevelStep(lev)
{

}

LevelStepBossEnter::~LevelStepBossEnter() {

}

void LevelStepBossEnter::init() {

}

void LevelStepBossEnter::onEnter() {
	if (nullptr != m_levelController) {
		m_levelController->showBoss();
	}
}

void LevelStepBossEnter::onExit() {
}

//////////////////////////////////////////////////////////////////////////////////////////
LevelStepBossTalk::LevelStepBossTalk(LevelController * lev):
	LevelStep(lev)
{

}

LevelStepBossTalk::~LevelStepBossTalk() {

}

void LevelStepBossTalk::init() {

}

void LevelStepBossTalk::onEnter() {
	showBgGrey(true);
	setBgGreyTouchable(false);
	if (nullptr != m_levelController) {
		m_levelController->showBossTalk(true);
	}
}

void LevelStepBossTalk::onExit() {
	setBgGreyTouchable(false);
	showBgGrey(false);
	if (nullptr != m_levelController) {
		m_levelController->showBossTalk(false);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
LevelStepGirlTalk::LevelStepGirlTalk(LevelController * lev):
	LevelStep(lev)
{

}

LevelStepGirlTalk::~LevelStepGirlTalk() {

}

void LevelStepGirlTalk::init() {

}

void LevelStepGirlTalk::onEnter() {
	showBgGrey(true);
	setBgGreyTouchable(false);
	if (nullptr != m_levelController) {
		m_levelController->showGirlTalk(true);
	}
}

void LevelStepGirlTalk::onExit() {
	showBgGrey(false);
	setBgGreyTouchable(false);
	if (nullptr != m_levelController) {
		m_levelController->showGirlTalk(false);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
LevelStepBossTip::LevelStepBossTip(LevelController * lev):
	LevelStep(lev)
{

}

LevelStepBossTip::~LevelStepBossTip() {

}

void LevelStepBossTip::init() {

}

void LevelStepBossTip::onEnter() {
	showBgGrey(true);
	setBgGreyTouchable(false);
	if (nullptr != m_levelController) {
		m_levelController->showBossTip(true);
	}
}

void LevelStepBossTip::onExit() {
	showBgGrey(false);
	setBgGreyTouchable(false);
	if (nullptr != m_levelController) {
		m_levelController->showBossTip(false);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////
LevelStepBossManual::LevelStepBossManual(LevelController * lev):
	LevelStep(lev)
{

}

LevelStepBossManual::~LevelStepBossManual() {

}

void LevelStepBossManual::init() {

}

void LevelStepBossManual::onEnter() {
	if (nullptr != m_levelController) {
		m_levelController->enterBossManual();
	}
}

void LevelStepBossManual::onExit() {
}