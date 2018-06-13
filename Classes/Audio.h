#ifndef _AUDIO_H_
#define _AUDIO_H_


class Audio{
public:
	~Audio();
	static Audio* getInstance();
	void init();
	void preLoad();
	void playGameBGM();
	void stopGameBGM();

	void playHomeBGM();
	void stopHomeBGM();

	void playWin();
	void playTimeup();
	void playReady();
	void playDida();
	void playToucherror();
	void playBuy();
	void playClick();

	void playClear(int num);
	void playClearPet(int hitCount);
	void playCombo(int index);
	void playZan(int hitCount);
	void playImpact();
	void playCross();
	void playBomb();
	void playFail();
	void playWelcome();
	void playStar();
	void playBreak();
	void playBigAward();
	void playLucky();
	void playFall();
	void playFall2();
	void playFall3();
	void stopLucky();

	void playBossBomb();
	void playBossHurt();
	void playBossShow();
	void playBullet();


	void setSoundSwitch(bool soundSwitch, bool bHome);
	inline bool isSoundOpened() { return m_soundSwitch; }

private:
	Audio();

private:
	static Audio* m_instance;
	bool m_soundSwitch;
	int m_effectLuckyId;
};
#endif