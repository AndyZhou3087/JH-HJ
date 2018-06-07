#ifndef __ui_giftmore_layer_controller_h__
#define __ui_giftmore_layer_controller_h__

#include "cocos2d.h"
#include "LayerController.h"
#include "prefix.h"

USING_NS_CC;

typedef std::function<void()> GiftMoreLayerCallback;


class UGiftMoreLayerController :public CenterLayerController{
public:
	UGiftMoreLayerController();
	~UGiftMoreLayerController();

	virtual void initial();
	virtual void onPayGiftMoreComplete(bool success) override;
	void setExitCallback(const GiftMoreLayerCallback& callback) { m_callback = callback; }

private:
	GiftMoreLayerCallback m_callback; 

};

#endif