#ifndef __ui_bigaward_layer_controller_h__
#define __ui_bigaward_layer_controller_h__

#include "cocos2d.h"
#include "LayerController.h"
#include "prefix.h"

USING_NS_CC;

class UBigAwardLayerController :public CenterLayerController{
public:
	UBigAwardLayerController();
	~UBigAwardLayerController();

	virtual void initial();
	virtual void enter() override;
	virtual void exit() override;
	virtual void onPayZunXiangComplete(bool success) override;
};

#endif