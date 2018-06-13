#ifndef __game_timeout_layer_controller_h__
#define __game_timeout_layer_controller_h__

#include "LayerController.h"
#include "prefix.h"

class GamePurchaseLayerController;
class GameEnergyTipLayerController;

class GameTimeoutLayerController:public LayerController {
public:
	GameTimeoutLayerController();
	~GameTimeoutLayerController();

	virtual void initial();
	virtual void enter();
	virtual void exit();

	void regPurchaseLayer(GamePurchaseLayerController *l) { m_purchaseLayer = l; } 
	void regEnergytipLayer(GameEnergyTipLayerController *l) { m_energytipLayer = l; }

private:
	TextAtlas *m_timeNum;
	GamePurchaseLayerController *m_purchaseLayer;
	GameEnergyTipLayerController *m_energytipLayer;
};

#endif