#ifndef __ui_rookie_layer_controller_h__
#define __ui_rookie_layer_controller_h__

#include "cocos2d.h"
#include "LayerController.h"
#include "prefix.h"

USING_NS_CC;

enum EnumRookieType {
	Rookie_xinshou,
	Rookie_huanle,
};

class URookieLayerController :public CenterLayerController{
public:
	URookieLayerController();
	~URookieLayerController();

	virtual void initial();
	virtual void enter();
	virtual void exit();

	void enterRookie();
	void enterHappy();

	virtual void onPayRookieComplete(bool success) override;
	virtual void onPayHappyComplete(bool success) override;

private:
	cocos2d::Sprite *m_rookie;
	cocos2d::Sprite *m_happy;
	EnumRookieType m_type;
	cocos2d::Node *m_baofa;
	
};

#endif