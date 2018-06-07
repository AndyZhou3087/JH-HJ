#ifndef __game_buytime_layer_controller_h__
#define __game_buytime_layer_controller_h__

#include "LayerController.h"
#include "prefix.h"

class GameBuyTimeLayerController:public LayerController {
public:
	GameBuyTimeLayerController();
	~GameBuyTimeLayerController();

	virtual void initial();
	virtual void enter();
	virtual void exit();

	virtual void onPayAddTimeComplete(bool success) override;
};

#endif