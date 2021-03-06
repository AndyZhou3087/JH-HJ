﻿
/****************************************************************
月卡Vip物品类

****************************************************************/
#ifndef _VIPGOODSITIME_H_
#define _VIPGOODSITIME_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "JhGlobalData.h"
USING_NS_CC;

class JhVipGoodsItem :public Sprite
{
public:
	JhVipGoodsItem();
	~JhVipGoodsItem();

	virtual bool init(GoodsData* gdata);
	static JhVipGoodsItem* create(GoodsData* gdata);
private:
	cocos2d::ui::ImageView* icon;
	cocos2d::ui::Text* nameTxt;
	cocos2d::ui::Text* descTxt;
	cocos2d::ui::Text* priceTxt;
	bool isDraging;
	Vec2 startPos;
	cocos2d::ui::Text* leftday;
	cocos2d::ui::Text* leftday_0;
	cocos2d::ui::Text* leftday_1;
	GoodsData* m_goodData;
	cocos2d::ui::Button* m_buybtn;
private:

	/****************************
	点击购买按钮回调
	*****************************/
	void onBuyBtn(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onItem(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void updateLeftDays(float dt);
};
#endif

