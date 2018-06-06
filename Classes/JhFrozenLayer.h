/********************************************************************
*∑‚∫≈≤„
*********************************************************************/
#ifndef __FROZENLAYER__
#define __FROZENLAYER__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
USING_NS_CC;

class JhFrozenLayer : public Layer
{
public:
	/****************************
	≥ı ºªØ
	*****************************/
    bool init();

	static JhFrozenLayer* create();
private:
	void onQQ(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
};

#endif
