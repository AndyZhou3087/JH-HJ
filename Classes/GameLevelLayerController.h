#ifndef __game_level_layer_controller_h__
#define __game_level_layer_controller_h__

#include "LayerController.h"
#include "prefix.h"

class GameTopLayerController;
class URookieLayerController;
class GameEnergyTipLayerController;
class GamePurchaseLayerController;
class GameLevelDemoLayerController;
class ULuckyLayerController;
class UUnlockLayerController;

class GameLevelLayerController:public LayerController {
public:
	GameLevelLayerController();
	~GameLevelLayerController();

	virtual void initial();
	virtual void enter();
	virtual void exit();

	virtual void onLevelUnlock();

	void on1sTimer();
	void onEnergyChange();
	void regHappyLayer(URookieLayerController *l) { m_rookieLayer = l; }
	void regLuckyLayer(ULuckyLayerController *l){ m_luckyLayer = l; }
	void regEnergyTipLayer(GameEnergyTipLayerController *l) { m_energyTipLayer = l; }
	void regPurchaseLayer(GamePurchaseLayerController *l) { m_purchaseLayer = l; }
	void regDemoLayer(GameLevelDemoLayerController *l) { m_levelDemoLayer = l; }
	void regUnlockLayer(UUnlockLayerController *l) { m_unlockLayer = l;}

private:
	void initBg();
	void updateBg();
	void updateCountdown();
	void updateTopInfo();
	void updateLuckyTime();
	void updateLuckyBtn();
	void updateLevelShow(cocos2d::Node *bg, int rate);
	void selectLevel(int level, Node * node);

	void initButton(int level, Button *btn);
	void updateButton(int level, Button *btn);

	void playArrowEffect();
	void stopArrowEffect();

	void playGiftEffect(Button *gift, float originY);
	void stopGiftEffect(Button *gift, float originY);

	void playRingEffect(Node *parent, const Point&pos, int zorder);
	void stopRingEffect();

	void playEnergyFlyEffect(Node * dstNode, const CallFunc *callback);

	void enterDemo();
	void exitDemo();

private:
	cocos2d::Node *m_luckyNode;
	cocos2d::Node *m_bgNode;
	cocos2d::Node *m_energyIcon;
	cocos2d::Sprite *m_bg1;
	cocos2d::Sprite *m_bg2;
	cocos2d::Sprite *m_bg3;
	cocos2d::Sprite *m_bg4;
	cocos2d::Sprite *m_bg5;
	cocos2d::Sprite *m_bg6;
	Button * m_demoButton;

	cocos2d::Sprite *m_arrowSprite;
	cocos2d::Sprite *m_ring;

	Sprite *m_topPanel;

	Button *m_happy;
	Button *m_lucky;

	float m_happyPosY;
	float m_luckyPosY;

	cocos2d::Sprite *m_energyBar;
	TextAtlas *m_numEnergy;

	cocos2d::Sprite *m_starsBar;
	TextAtlas *m_numStars;

	ScrollView *m_scroll;
	URookieLayerController *m_rookieLayer;
	GameEnergyTipLayerController *m_energyTipLayer;
	GamePurchaseLayerController *m_purchaseLayer;
	GameLevelDemoLayerController *m_levelDemoLayer;
	ULuckyLayerController *m_luckyLayer;
	UUnlockLayerController *m_unlockLayer;

	TextAtlas *m_countdown;
	TextAtlas *m_luckyCountdown;
};

#endif