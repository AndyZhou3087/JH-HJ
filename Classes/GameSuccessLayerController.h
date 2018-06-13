#ifndef __game_success_layer_controller_h__
#define __game_success_layer_controller_h__

#include "LayerController.h"
#include "prefix.h"

class UUnlockLayerController;
class GameEnergyTipLayerController;

class GameSuccessLayerController:public LayerController {
public:
	GameSuccessLayerController();
	~GameSuccessLayerController();

	virtual void initial();
	virtual void enter();
	virtual void exit();

	void regUnlockLayer(UUnlockLayerController *l) { m_unlockLayer = l; }
	void regEnergyTipLayer(GameEnergyTipLayerController *l) { m_energyTipLayer = l; }

private:
	void initStar();
	void updateRankInfo();
	void playStar1Effect();
	void playStar2Effect();
	void playStar3Effect();
	void playStarBombEffect(cocos2d::Node *star, int offset);

private:
	cocos2d::Sprite *m_title;
	cocos2d::Sprite *m_star1;
	cocos2d::Sprite *m_star2;
	cocos2d::Sprite *m_star3;

	cocos2d::Point m_star1OriginPos;
	cocos2d::Point m_star2OriginPos;
	cocos2d::Point m_star3OriginPos;

	TextAtlas *m_rank;
	TextAtlas *m_time;
	TextAtlas *m_percent;
	TextAtlas *m_energyNum;
	UUnlockLayerController *m_unlockLayer;
	GameEnergyTipLayerController *m_energyTipLayer;
};

#endif