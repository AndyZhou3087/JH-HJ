#ifndef __game_scene_h__
#define __game_scene_h__
#include <cocos2d.h>

class GameBgLayerController;
class GameTopLayerController;
class GameBottomLayerController;
class GameLayerController;
class GamePauseMenuLayerController;
class GameOverMenuLayerController;
class GameLevelLayerController;
class GameTargetLayerController;
class GameTargetBossLayerController;
class GameLotteryLayerController;
class GameSuccessLayerController;
class GameFailLayerController;
class GameBuyTimeLayerController;
class GameEnergyTipLayerController;
class GamePauseLayerController;
class GameTimeoutLayerController;
class GamePurchaseLayerController;
class GreyLayerController;
class GameIntroLayerController;
class BgLayerController;
class GameLevelDemoLayerController;
class GameBossLayerController;

class URookieLayerController;
class ULuckyLayerController;
class UAwardLayerController;
class UDiscountLayerController;
class USupriseLayerController;
class UGiftLayerController;
class UBigAwardLayerController;
class UExitLayerController;
class ULuckyGiftLayerController;
class UUnlockLayerController;
class UIntroAwardLayerController;
class UTipWinLayerController;
class UToolConfirmLayerController;
class UGiftMoreLayerController;
class UExitGiftLayerController;
class UGuideTipLayerController;

class GameScene :public cocos2d::CCLayer
{
public:
	GameScene(void);
	~GameScene(void);
	CREATE_FUNC(GameScene);
	static cocos2d::Scene * createScene();
	virtual bool init();
	virtual void onEnter();
	virtual void onExit();

	void onGameStart(cocos2d::EventCustom*);
	void onGameOver(cocos2d::EventCustom*);
	void onGameRestart(cocos2d::EventCustom*);
	void onGamePause(cocos2d::EventCustom*);
	void onGameResume(cocos2d::EventCustom*);
	void onGameReady(cocos2d::EventCustom*);
	void onGameRelive(cocos2d::EventCustom*);
	void onGameScoreChanged(cocos2d::EventCustom*);
	void onGameToolChanged(cocos2d::EventCustom*);
	void onReleaseCross(cocos2d::EventCustom*);
	void onEnergyChange(cocos2d::EventCustom*);
	void onExitCancel(cocos2d::EventCustom*);
	void onCloseIntro(cocos2d::EventCustom*);
	void onLevelUnlock(cocos2d::EventCustom*);
	void onTimeChange(cocos2d::EventCustom*);
	void onGameWin(cocos2d::EventCustom*);
	void onAttrackBoss(cocos2d::EventCustom*);
	void onGameFinish(cocos2d::EventCustom*);

	virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event  *event) override;
	virtual void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event  *event) override;
	virtual void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event  *event) override;
	virtual void onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event *event) override;

	void on1sTimer(float dt);
	static bool loadUi();

private:
	void initBg();
	void initUi();
	void initJFBg();
	void initIntroUi();
	void initGuideUi();

	void initController();
	void initJFController();
	void initIntroController();

	void regGameEvent();
	void unregGameEvent();
	void regSysEvent();
	void unregSysEvent();
    long getCurrentTime();

private:
	cocos2d::Sprite *m_bg;
	cocos2d::Sprite *m_bgGround;
	cocos2d::Node *m_introBgLayer;
	cocos2d::Node *m_bggreyLayer;	
	
	static cocos2d::Node *m_gameLayer;
	static cocos2d::Node *m_topLayer;
	static cocos2d::Node *m_bottomLayer;
	static cocos2d::Node *m_levelLayer;
	static cocos2d::Node *m_giftLayer;
	static cocos2d::Node *m_targetLayer;
	static cocos2d::Node *m_targetBossLayer;
	static cocos2d::Node *m_lotteryLayer;
	static cocos2d::Node *m_successLayer;
	static cocos2d::Node *m_failLayer;
	static cocos2d::Node *m_buytimeLayer;
	static cocos2d::Node *m_timeoutLayer;
	static cocos2d::Node *m_energytipLayer;
	static cocos2d::Node *m_pauseLayer;
	static cocos2d::Node *m_gameBossLayer;
	static cocos2d::Node *m_purchaseLayer;
	static cocos2d::Node *m_introLayer;

	static cocos2d::Node *m_welcome;
	static cocos2d::Node *m_welcomeEnd;
	static cocos2d::Node *m_crossGuide;
	static cocos2d::Node *m_bombGuide;
	static cocos2d::Node *m_timeGuide;
	static cocos2d::Node *m_tipGuide;
	static cocos2d::Node *m_specialPetGuide;
	static cocos2d::Node *m_bossTalk;
	static cocos2d::Node *m_girlTalk;
	static cocos2d::Node *m_bossTip;
	static cocos2d::Node *m_touchErr;

	cocos2d::Node *m_levelDemoLayer;

	static cocos2d::Node *m_uiRookieLayer;
	static cocos2d::Node *m_uiLuckyLayer;
	static cocos2d::Node *m_uiAwardLayer;
	static cocos2d::Node *m_uiDiscountLayer;
	static cocos2d::Node *m_uiSupriseLayer;
	static cocos2d::Node *m_uiBigAwardLayer;
	static cocos2d::Node *m_uiExitLayer;
	static cocos2d::Node *m_uiLuckyGiftLayer;
	static cocos2d::Node *m_uiUnlockLayer;
	static cocos2d::Node *m_uiIntroAwardLayer;
	static cocos2d::Node *m_uiTipwinLayer;
	static cocos2d::Node *m_uiToolConfirmLayer;
	static cocos2d::Node *m_uiGiftMoreLayer;
	static cocos2d::Node *m_uiExitGiftLayer;
	
	GreyLayerController				*m_greyLayerController;
	GameTopLayerController			*m_topLayerController;
	GameBottomLayerController		*m_bottomLayerController;
	GameLayerController				*m_gameLayerController;
	GameLevelLayerController		*m_levelLayerController;
	GameTargetLayerController		*m_targetLayerController;
	GameTargetBossLayerController	*m_targetBossLayerController;
	GameLotteryLayerController		*m_lotteryLayerController;
	GameSuccessLayerController		*m_successLayerController;
	GameFailLayerController			*m_failLayerController;
	GameBuyTimeLayerController		*m_buytimeLayerController;
	GameTimeoutLayerController		*m_timeoutLayerController;
	GameEnergyTipLayerController	*m_energytipLayerController;
	GamePauseLayerController		*m_pauseLayerController;
	GamePurchaseLayerController		*m_purchaseLayerController;
	GameIntroLayerController		*m_introLayerController;
	BgLayerController				*m_introBgLayerController;
	GameLevelDemoLayerController	*m_levelDemoLayerController;
	GameBossLayerController			*m_gameBossLayerController;

	URookieLayerController		*m_uiRookieLayerController;
	ULuckyLayerController		*m_uiLuckyLayerController;
	UAwardLayerController		*m_uiAwardLayerController;
	UDiscountLayerController	*m_uiDiscountLayerController;
	USupriseLayerController		*m_uiSupriseLayerController;
	UGiftLayerController		*m_giftLayerController;
	UBigAwardLayerController	*m_uiBigAwardLayerController;
	UExitLayerController		*m_uiExitLayerController;
	ULuckyGiftLayerController	*m_uiLuckyGiftLayerController;
	UUnlockLayerController		*m_uiUnlockLayerController;
	UIntroAwardLayerController	*m_uiIntroAwardLayerController;
	UTipWinLayerController		*m_uiTipwinLayerController;
	UToolConfirmLayerController	*m_uiToolConfirmLayerController;
	UGiftMoreLayerController	*m_uiGiftMoreLayerController;
	UExitGiftLayerController	*m_uiExitGiftLayerController;
	UGuideTipLayerController	*m_uiGuideTipLayerController;

	cocos2d::EventListener *m_GameStartLis;
	cocos2d::EventListener *m_GameRestartLis;
	cocos2d::EventListener *m_GameOverLis;
	cocos2d::EventListener *m_GameResumeLis;
	cocos2d::EventListener *m_GamePauseLis;
	cocos2d::EventListener *m_GameReadyLis;
	cocos2d::EventListener *m_GameReliveLis;
	cocos2d::EventListener *m_ExitCancelLis;
	cocos2d::EventListener *m_GameScoreChangeLis;
	cocos2d::EventListener *m_GameToolChangeLis;
	cocos2d::EventListener *m_ReleaseCrossLis;
	cocos2d::EventListener *m_EnrgyChangeLis;
	cocos2d::EventListener *m_CloseIntroLis;
	cocos2d::EventListener *m_LevelUnlockLis;
	cocos2d::EventListener *m_TimeChangeLis;
	cocos2d::EventListener *m_GameWinLis;
	cocos2d::EventListener *m_AttrackBossLis;
	cocos2d::EventListener *m_GameFinishLis;

    long m_currentTime;

	cocos2d::EventListenerTouchOneByOne *m_touchEventListener;
	cocos2d::EventListenerKeyboard *m_keyboradEventListener;
	bool m_backTouched;
};

#endif