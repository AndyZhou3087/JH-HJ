﻿#ifndef _PAYSELECT_LAYER_H_
#define _PAYSELECT_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "JhGlobalData.h"
USING_NS_CC;

class JhPaySelectLayer :public Layer
{
public:
	JhPaySelectLayer();
	~JhPaySelectLayer();

	bool init(int payindex);
	static JhPaySelectLayer* create(int payindex);

private:
	int m_payindex;
private:
	void onCancel(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onPay(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
};
#endif

