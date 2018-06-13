#ifndef __tipwin_layer_controller_h__
#define __tipwin_layer_controller_h__

#include "cocos2d.h"
#include "LayerController.h"
#include "prefix.h"

USING_NS_CC;

class UTipWinLayerController :public CenterLayerController{
public:
	UTipWinLayerController();
	~UTipWinLayerController();

	virtual void initial();
	virtual void enter();
	virtual void exit();

	void playWinEffect();

private:
	cocos2d::Node *m_light;
	cocos2d::Node *m_label;
	float m_bgOriginY;
	float m_labelOriginY;

};

#endif