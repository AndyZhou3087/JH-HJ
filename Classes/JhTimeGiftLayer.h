#ifndef _TIMEGIFT_LAYER_H_
#define _TIMEGIFT_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "JhGlobalData.h"
USING_NS_CC;

class JhTimeGiftLayer :public Layer
{
public:
	JhTimeGiftLayer();
	~JhTimeGiftLayer();

	bool init(int goodsId);
	static JhTimeGiftLayer* create(int goodsId);

private:
	cocos2d::ui::Text* m_timelbl;
private:
	void onbuy(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onClose(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void checkTimeGift(float dt);
};
#endif

