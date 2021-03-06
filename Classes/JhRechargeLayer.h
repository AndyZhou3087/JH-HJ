﻿#ifndef _RECHARGE_LAYER_H_
#define _RECHARGE_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "JhGlobalData.h"
#include "JhServerDataSwap.h"
USING_NS_CC;

class JhRechargeLayer :public Layer, public ServerDataDelegateProtocol
{
public:
	JhRechargeLayer();
	~JhRechargeLayer();

	bool init();
	static JhRechargeLayer* create();
	static int getRechargeMaxAmount();
private:
	
	cocos2d::ui::Button* getbtn;
	cocos2d::ui::Text* datelbl;
	cocos2d::ui::Text* progresstext;
	cocos2d::ui::LoadingBar* progressbar;
	cocos2d::ui::ScrollView* scrollView;
	std::map<int, std::vector<std::string>> map_recharge;
	int maxamount;
	Node* m_csbnode;

	std::vector<std::string> vec_myawrd;
	int canGetHightAmount;
	cocos2d::ui::Widget* descbg;
private:
	void showdata();
	void onClose(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onGet(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onBoxClick(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onBGClick(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onSuccess();
	void onErr(int errcode);

	void splitRechargeAward();

	void showDescAward(int amount,Node* node);
	
};
#endif

