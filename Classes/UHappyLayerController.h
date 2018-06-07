#ifndef __ui_happy_layer_controller_h__
#define __ui_happy_layer_controller_h__

#include "cocos2d.h"
#include "LayerController.h"
#include "prefix.h"

USING_NS_CC;

class UHappyLayerController :public LayerController{
public:
	UHappyLayerController();
	~UHappyLayerController();

	virtual void initial();
	virtual void enter();
	virtual void exit();

	virtual void onPayHappyComplete(bool success) override;

};

#endif