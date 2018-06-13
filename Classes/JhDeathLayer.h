/********************************************************************
* Game over UI界面
*********************************************************************/
#ifndef _DEATH_LAYER_H_
#define _DEATH_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
USING_NS_CC;

class JhDeathLayer :public Layer
{
public:
	JhDeathLayer();
	~JhDeathLayer();

	virtual bool init();
	static JhDeathLayer* create();
private:


private:

};
#endif

