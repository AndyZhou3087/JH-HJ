#ifndef _BUYCOMFIRM_LAYER_H_
#define _BUYCOMFIRM_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "JhGlobalData.h"
USING_NS_CC;

class JhBuyComfirmLayer :public Layer
{
public:
	JhBuyComfirmLayer();
	~JhBuyComfirmLayer();

	bool init(GoodsData* gdata);
	static JhBuyComfirmLayer* create(GoodsData* gdata);

private:
	GoodsData* m_gdata;
private:
	void onCancel(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onBuy(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void changeSex();
};
#endif

