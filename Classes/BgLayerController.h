#ifndef __bg_layer_controller_h__
#define __bg_layer_controller_h__

#include "cocos2d.h"
#include "LayerController.h"

class BgLayer:public cocos2d::Layer {
public:
	BgLayer();
	~BgLayer();
	CREATE_FUNC(BgLayer);

	virtual bool init() override;
	void initBg();

private:
	cocos2d::Sprite *m_bg;
	cocos2d::Sprite *m_bgGround;
	cocos2d::Node *m_top;
	cocos2d::Node *m_bottom;
};

class BgLayerController:public BottomLayerController {
public:
	BgLayerController();
	~BgLayerController();

	virtual void initial();
};

#endif