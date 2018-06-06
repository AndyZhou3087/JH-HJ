#ifndef _RAFFLE_RESULT_LAYER_H_
#define _RAFFLE_RESULT_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "JhGlobalData.h"
USING_NS_CC;

class JhRaffleResultLayer :public Layer
{
public:
	JhRaffleResultLayer();
	~JhRaffleResultLayer();

	bool init();
	static JhRaffleResultLayer* create();

private:
	void onOK(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
private:
	int wingold;
};
#endif

