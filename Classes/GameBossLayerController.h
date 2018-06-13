#ifndef __gameboss_layer_controller_h__
#define __gameboss_layer_controller_h__

#include "cocos2d.h"
#include "LayerController.h"
#include "prefix.h"

USING_NS_CC;

class UTipWinLayerController;

class GameBossLayerController :public CenterLayerController{
public:
	GameBossLayerController();
	~GameBossLayerController();

	enum EnumBoss {
		Boss_unknow = 0,
		Boss_Snow = 1,
		Boss_Bear = 2,
	};

	virtual void initial();
	virtual void enter() override;
	virtual void exit() override;

	void onGameStart() override;
	void onGameOver() override;
	void onScoreChange();
	void on1sTimer();
	void onTimeChange();
	void onAttrackBoss();

	void playBossShowEffect(CallFunc * callback = nullptr);
	void playBossDeathEffect();

	inline void stopTime() { m_bStop = true; }
	inline void startTime() { m_bStop = false; }

	void regTipWinLayer(UTipWinLayerController *l) { m_tipwinLayer = l; }

private:
	void reset();
	void initalBoss();
	void initTime();
	void updateBloodBar();
	void updateTime();
	void resetBoss();

	void initBossBombParticleSystem();
	void playAttrackEffect();
	void playBombEffect();
	void playBossActiveEffect();
	void stopBossActiveEffect();

	void setHurtBossVisible(bool isVisible);

private:
	Sprite *m_bloodBar;
	TextAtlas *m_time;
	bool m_bStop;
	Sprite *m_boss;

	ParticleSystem * m_emitterBomb;

	float m_bossOriginPosY;
	float m_bossOriginPosX;

	UTipWinLayerController * m_tipwinLayer;

	Sprite *m_normalBoss_head;
	Sprite *m_normallBoss_hand_left;
	Sprite *m_normallBoss_hand_right;

	Sprite *m_hurtBoss_head;
	Sprite *m_hurtBoss_hand_left;
	Sprite *m_hurtBoss_hand_right;
	Sprite *m_hurtBoss_body;
};

#endif