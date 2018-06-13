#ifndef __ui_suprise_layer_controller_h__
#define __ui_suprise_layer_controller_h__

#include "cocos2d.h"
#include "LayerController.h"
#include "prefix.h"

USING_NS_CC;

class USupriseLayerController :public CenterLayerController{
public:
	USupriseLayerController();
	~USupriseLayerController();

	virtual void initial();
	virtual void onPaySupriseBoxComplete(bool success) override;
};

#endif