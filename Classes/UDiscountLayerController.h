#ifndef __ui_discount_layer_controller_h__
#define __ui_discount_layer_controller_h__

#include "cocos2d.h"
#include "LayerController.h"
#include "prefix.h"

USING_NS_CC;

class UDiscountLayerController :public CenterLayerController{
public:
	UDiscountLayerController();
	~UDiscountLayerController();

	virtual void initial();
	virtual void enter();
	virtual void exit();

	virtual void onPayDiscountComplete(bool success) override;

private:
	void playSecEffect();
	void playMillSecEffect();

private:
	TextAtlas *m_time1;
	TextAtlas *m_time2;
	int m_sec;
	int m_millSec;
};

#endif