#ifndef __game_energytip_layer_controller_h__
#define __game_energytip_layer_controller_h__

#include "LayerController.h"
#include "prefix.h"

enum EnumEnergyTipType {
	EnergyTip_this,
	EnergyTip_next,
};

class GameEnergyTipLayerController:public LayerController {
public:
	GameEnergyTipLayerController();
	~GameEnergyTipLayerController();

	virtual void initial();
	virtual void enter();
	virtual void exit();
	virtual void enter(EnumEnergyTipType type);

	virtual void onPayAddEnergyComplete(bool success) override;

private:
	EnumEnergyTipType m_type;
};

#endif