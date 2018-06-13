#ifndef __grey_layer_h__
#define __grey_layer_h__

#include "cocos2d.h"
#include "LayerController.h"

class GreyLayer:public cocos2d::Layer {
public:
	GreyLayer();
	~GreyLayer();
	CREATE_FUNC(GreyLayer);

	virtual bool init() override;
	void initBg();

	void swallowTouches(bool swallowed);

	inline Sprite *getBgSprite() { return m_bg; }

private:
	cocos2d::Sprite *m_bg;
	cocos2d::EventListenerTouchOneByOne *m_eventLis;
};

class GreyLayerController:public LayerController {
public:
	GreyLayerController();
	~GreyLayerController();

	virtual void initial();
	virtual void enter();
	virtual void exit();
	void setTouchable(bool enable);
};

#endif