#ifndef __ui_luckygift_layer_controller_h__
#define __ui_luckygift_layer_controller_h__

#include "cocos2d.h"
#include "LayerController.h"
#include "prefix.h"

USING_NS_CC;

class ULuckyGiftLayerController :public CenterLayerController{
public:
	ULuckyGiftLayerController();
	~ULuckyGiftLayerController();

	virtual void enter() override;
	virtual void exit() override;
	virtual void initial();
	virtual void onPayLuckyComplete(bool success) override;
};

#endif