#ifndef __game_layer_controller_h__
#define __game_layer_controller_h__

#include "LayerController.h"
#include "LevelData.h"

class PetMatrix;
class GameBottomLayerController;
class USupriseLayerController;
class LevelController;

class GameLayerController:public LayerController
{
public:
	GameLayerController();
	virtual ~GameLayerController();

	virtual void initial();
	virtual void enter();
	virtual void exit();

	virtual void onGameStart() override;
	virtual void onGameReady() override;
	virtual void onGameOver() override;
	virtual void onGameFinish() override;

	void on1sTimer();
	void onTouch(const Point&pos);

	bool usingBomb();
	bool usingTip();
	bool usingCross();
	bool usingTime();

	void regSupriseLayer(USupriseLayerController *l) { m_supriseLayer = l; }
	void regBottomLayer(GameBottomLayerController *l) { m_bottomLayer = l; }

protected:
	void showSuprise();

protected:
	USupriseLayerController *m_supriseLayer;
	GameBottomLayerController *m_bottomLayer;
	bool m_bShowSuprise;
	bool m_bClearAlling;
	int m_timeCount;
	int m_tipTimeCount;
	LevelController *m_LevelController;
};
#endif