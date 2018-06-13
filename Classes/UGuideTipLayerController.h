#ifndef __ui_guidetip_layer_controller_h__
#define __ui_guidetip_layer_controller_h__

#include "cocos2d.h"
#include "LayerController.h"
#include "prefix.h"

USING_NS_CC;

typedef std::function<void()> GuideTipLayerCallback;

class UGuideTipLayerController :public CenterLayerController{
public:
	UGuideTipLayerController();
	~UGuideTipLayerController();

	virtual void initial();
	virtual void enter();
	virtual void exit();

	virtual void onPayGuideTipComplete(bool success) override;
	void setExitCallback(const GuideTipLayerCallback& callback) { m_callback = callback; }

private:
	GuideTipLayerCallback m_callback;
};

#endif