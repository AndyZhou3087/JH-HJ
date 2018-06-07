#ifndef __game_fail_layer_controller_h__
#define __game_fail_layer_controller_h__

#include "LayerController.h"
#include "prefix.h"

class GameEnergyTipLayerController;
class UUnlockLayerController;

class GameFailLayerController:public LayerController {
public:
	GameFailLayerController();
	~GameFailLayerController();

	virtual void initial();
	virtual void enter();
	virtual void exit();

	inline void regEnergyTipLayer(GameEnergyTipLayerController *l) { m_energytipLayer = l; }
	inline void regUnlockLayer(UUnlockLayerController *l) { m_unlockLayer = l; }
	virtual void onPayPassGameComplete(bool success) override;

private:
	GameEnergyTipLayerController *m_energytipLayer;
	UUnlockLayerController *m_unlockLayer;
};

#endif