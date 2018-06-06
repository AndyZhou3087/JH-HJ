/********************************************************************
*不可点击层
*********************************************************************/
#ifndef _CANNOT_TOUCH_LAYER_H_
#define _CANNOT_TOUCH_LAYER_H_
#include "cocos2d.h"
USING_NS_CC;

class JhCannotTouchLayer :public Layer
{
public:
	JhCannotTouchLayer();
	~JhCannotTouchLayer();

	virtual bool init();
	static JhCannotTouchLayer* create();
private:


};
#endif

