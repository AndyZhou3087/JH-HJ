#include "Audio.h"
#include "SimpleAudioEngine.h"
#include "Resource.h"
#include "UserData.h"
#include "GameUtil.h"

using namespace CocosDenshion;
USING_NS_CC;

Audio* Audio::m_instance = nullptr;
Audio* Audio::getInstance(){
	if(m_instance == nullptr){
		m_instance = new Audio();
	}
	return m_instance;
}

Audio::Audio():
m_soundSwitch(true),
m_effectLuckyId(-1)
{
	init();
}

Audio::~Audio() {

}

void Audio::init() {
	m_soundSwitch = UserData::getSoundSwitch();
}

void Audio::preLoad() {
	SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic(s_music_game_bg);
	SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic(s_music_home_bg);
	SimpleAudioEngine::sharedEngine()->preloadEffect(s_music_effect_win);
	SimpleAudioEngine::sharedEngine()->preloadEffect(s_music_effect_timeup);
	SimpleAudioEngine::sharedEngine()->preloadEffect(s_music_effect_ready);
	SimpleAudioEngine::sharedEngine()->preloadEffect(s_music_effect_dida);
	SimpleAudioEngine::sharedEngine()->preloadEffect(s_music_effect_toucherror);
	SimpleAudioEngine::sharedEngine()->preloadEffect(s_music_effect_buy);
	SimpleAudioEngine::sharedEngine()->preloadEffect(s_music_effect_click);

	SimpleAudioEngine::sharedEngine()->preloadEffect(s_music_effect_clear2);
	SimpleAudioEngine::sharedEngine()->preloadEffect(s_music_effect_clear3);
	SimpleAudioEngine::sharedEngine()->preloadEffect(s_music_effect_clear4);
	SimpleAudioEngine::sharedEngine()->preloadEffect(s_music_effect_combo1);
	SimpleAudioEngine::sharedEngine()->preloadEffect(s_music_effect_combo2);
	SimpleAudioEngine::sharedEngine()->preloadEffect(s_music_effect_combo3);
	SimpleAudioEngine::sharedEngine()->preloadEffect(s_music_effect_combo4);
	SimpleAudioEngine::sharedEngine()->preloadEffect(s_music_effect_combo5);
	SimpleAudioEngine::sharedEngine()->preloadEffect(s_music_effect_zan3);
	SimpleAudioEngine::sharedEngine()->preloadEffect(s_music_effect_zan4);
	SimpleAudioEngine::sharedEngine()->preloadEffect(s_music_effect_zan5);
	SimpleAudioEngine::sharedEngine()->preloadEffect(s_music_effect_zan6);


	SimpleAudioEngine::sharedEngine()->preloadEffect(s_music_effect_impact);
	SimpleAudioEngine::sharedEngine()->preloadEffect(s_music_effect_cross);
	SimpleAudioEngine::sharedEngine()->preloadEffect(s_music_effect_bomb);
	SimpleAudioEngine::sharedEngine()->preloadEffect(s_music_effect_fail);
	SimpleAudioEngine::sharedEngine()->preloadEffect(s_music_effect_welcome);
	SimpleAudioEngine::sharedEngine()->preloadEffect(s_music_effect_star);
	SimpleAudioEngine::sharedEngine()->preloadEffect(s_music_effect_break);
	SimpleAudioEngine::sharedEngine()->preloadEffect(s_music_effect_bigaward);
	SimpleAudioEngine::sharedEngine()->preloadEffect(s_music_effect_lucky);
	SimpleAudioEngine::sharedEngine()->preloadEffect(s_music_effect_fall);
	SimpleAudioEngine::sharedEngine()->preloadEffect(s_music_effect_fall2);
	SimpleAudioEngine::sharedEngine()->preloadEffect(s_music_effect_fall3);

	SimpleAudioEngine::sharedEngine()->preloadEffect(s_music_effect_boss_bomb);
	SimpleAudioEngine::sharedEngine()->preloadEffect(s_music_effect_boss_hurt);
	SimpleAudioEngine::sharedEngine()->preloadEffect(s_music_effect_boss_show);
	SimpleAudioEngine::sharedEngine()->preloadEffect(s_music_effect_bullet);

	//ÉèÖÃÒôÁ¿
	SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0.72);
	SimpleAudioEngine::sharedEngine()->setEffectsVolume(0.9);
}

void Audio::setSoundSwitch(bool soundSwitch, bool bHome) {
	m_soundSwitch = soundSwitch;
	UserData::setSoundSwitch(soundSwitch);
	if (m_soundSwitch) {
		if (bHome) {
			playHomeBGM();
		} else {
			playGameBGM();
		}
	} else {
		if (bHome) {
			stopHomeBGM();
		} else {
			stopGameBGM();
		}
	}
}

void Audio::playGameBGM(){
	if (isSoundOpened()) {
		CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(s_music_game_bg,true);
	}
}

void Audio::stopGameBGM(){
	CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
}
	

void Audio::playHomeBGM(){
	if (isSoundOpened()) {
		CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(s_music_home_bg,true);
	}
}

void Audio::stopHomeBGM(){
	CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
}

void Audio::playWin(){
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(s_music_effect_win,false);
}

void Audio::playTimeup(){
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(s_music_effect_timeup,false);
}

void Audio::playReady(){
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(s_music_effect_ready,false);
}

void Audio::playDida(){
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(s_music_effect_dida,false);
}

void Audio::playToucherror() {
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(s_music_effect_toucherror,false);
}

void Audio::playBuy() {
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(s_music_effect_buy,false);
}

void Audio::playClick() {
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(s_music_effect_click,false);
}

void Audio::playClear(int n) {
	switch (n) {
		case 4:
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(s_music_effect_clear4, false);
			break;
		case 3:
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(s_music_effect_clear3, false);
			break;
		case 2:
		case 1:
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(s_music_effect_clear2, false);
			break;

	}
}

void Audio::playClearPet(int hitCount) {
	if (hitCount < 1) {
		return;
	}

	switch (hitCount) {
	case 1:
		playClear(2);
		playCombo(0);
		break;
	case 2:
		playClear(3);
		playCombo(1);
		break;
	case 3:
		playClear(4);
		playCombo(2);
		playZan(3);
		break;
	case 4:
		playClear(4);
		playCombo(3);
		playZan(4);
		break;
	case 5:
		playClear(4);
		playCombo(4);
		playZan(5);
		break;
	case 6:
		playClear(4);
		playCombo(5);
		playZan(6);
		break;
	default:
		playClear(4);
		playCombo(6);
		playZan(6);
		break;
	}
}

void Audio::playCross() {
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(s_music_effect_cross,false);
}

void Audio::playBomb() {
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(s_music_effect_bomb,false);
}

void Audio::playFail() {
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(s_music_effect_fail,false);
}

void Audio::playWelcome() {
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(s_music_effect_welcome,false);
}

void Audio::playCombo(int index) {
	const char * combo[] = { s_music_effect_combo1, 
									s_music_effect_combo2, 
									s_music_effect_combo3, 
									s_music_effect_combo4, 
									s_music_effect_combo5 };
	const int size = sizeof(combo)/sizeof(combo[0]);
	if (index < 0) {
		index = 0;
	}

	if (index >= size) {
		index = size - 1;
	}

	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(combo[index],false);
}

void Audio::playZan(int hitCount) {
	const char * zan[] = { 
		nullptr,
		nullptr,
		nullptr,
		s_music_effect_zan3, 
		s_music_effect_zan4, 
		s_music_effect_zan5, 
		s_music_effect_zan6 };

	if (hitCount < 0) {
		return;
	}

	if (hitCount >= sizeof(zan)/sizeof(zan[0])) {
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(s_music_effect_zan6, false);
	} else if (zan[hitCount] != nullptr){
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(zan[hitCount],false);
	}
}

void Audio::playStar() {
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(s_music_effect_star,false);
}

void Audio::playImpact() {
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(s_music_effect_impact,false);
}

void Audio::playBreak() {
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(s_music_effect_break,false);
}

void Audio::playBigAward() {
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(s_music_effect_bigaward,false);
}

void Audio::playLucky() {
	m_effectLuckyId = CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(s_music_effect_lucky,false);
}

void Audio::playFall() {
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(s_music_effect_fall,false);
}

void Audio::playFall2() {
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(s_music_effect_fall2,false);
}

void Audio::playFall3() {
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(s_music_effect_fall3,false);
}

void Audio::stopLucky() {
	if (-1 != m_effectLuckyId) {
		CocosDenshion::SimpleAudioEngine::getInstance()->stopEffect(m_effectLuckyId);
		m_effectLuckyId = -1;
	}
	
}

void Audio::playBossBomb() {
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(s_music_effect_boss_bomb,false);
}

void Audio::playBossHurt() {
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(s_music_effect_boss_hurt,false);
}

void Audio::playBossShow() {
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(s_music_effect_boss_show,false);
}

void Audio::playBullet() {
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(s_music_effect_bullet,false);
}
