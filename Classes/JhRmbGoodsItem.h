
/****************************************************************
商城人民币购买

****************************************************************/
#ifndef _RMBGOODSITIME_H_
#define _RMBGOODSITIME_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "JhGlobalData.h"
USING_NS_CC;

class JhRmbGoodsItem :public Sprite
{
public:
	JhRmbGoodsItem();
	~JhRmbGoodsItem();

	virtual bool init(GoodsData* gdata);
	static JhRmbGoodsItem* create(GoodsData* gdata);
private:
	cocos2d::ui::ImageView* icon;
	cocos2d::ui::Text* nameTxt;
	bool isDraging;
	Vec2 startPos;
private:
	/****************************
	点击购买按钮回调
	*****************************/
	void onBuyBtn(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onItem(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
};
#endif

