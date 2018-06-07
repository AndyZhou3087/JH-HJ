#ifndef __ui_exit_layer_controller_h__
#define __ui_exit_layer_controller_h__

#include "cocos2d.h"
#include "LayerController.h"
#include "prefix.h"

USING_NS_CC;

class UExitLayerController :public CenterLayerController{
public:
	UExitLayerController();
	~UExitLayerController();

	virtual void initial();
	virtual void enter();
	virtual void exit();

	virtual void onPayHaoliComplete(bool success) override;
};

#endif