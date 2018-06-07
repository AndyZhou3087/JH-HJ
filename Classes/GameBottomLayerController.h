#ifndef __game_bottom_layer_h__
#define __game_bottom_layer_h__

#include "LayerController.h"
#include "cocos2d.h"
#include "prefix.h"

class GamePurchaseLayerController;
class GameLayerController;

class GameBottomLayerController:public LayerController {
public:
	GameBottomLayerController();
	~GameBottomLayerController();

	virtual void initial();
	virtual void enter();
	virtual void exit();
	virtual void onGameReady() override;
	virtual void onGameStart() override;
	virtual void onGameRestart() override;
	virtual void onGameRelive() override;
	virtual void onGameOver() override;
	virtual void onGameToolChanged();

	inline void regPurchaseLayer(GamePurchaseLayerController *l) { m_purchaseLayer = l; }
	inline void regGameLayer(GameLayerController *l) { m_gameLayer = l; }

	void releaseCross();

	void tipBomb();
	void tipTip();

private:
	void updateInfo();
	void updateBeat(int beatCount);

	void onClickCross();
	void onClickTime();
	void onClickTip();
	void onClickBomb();

private:
	Button *m_cross;
	Button *m_time;
	Button *m_tip;
	Button *m_bomb;

	TextAtlas *m_cross_num;
	TextAtlas *m_time_num;
	TextAtlas *m_tip_num;
	TextAtlas *m_bomb_num;

	float m_crossPosY;
	bool m_bSelectCross;

	GamePurchaseLayerController *m_purchaseLayer;
	GameLayerController *m_gameLayer;
	
};

#endif