#ifndef __lottery_layer_controller_h__
#define __lottery_layer_controller_h__

#include "LayerController.h"
#include "prefix.h"

class LotteryLayerController:public LayerController {
public:
	LotteryLayerController();
	~LotteryLayerController();

	virtual void initial();
	virtual void enter();
	virtual void exit();
};

#endif