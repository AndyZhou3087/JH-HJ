#ifndef _HSLJREWARD_LAYER_H_
#define _HSLJREWARD_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "JhGlobalData.h"
#include "JhServerDataSwap.h"
USING_NS_CC;
class JhHSLJRewardLayer :public Layer, public ServerDataDelegateProtocol
{
public:
	JhHSLJRewardLayer();
	~JhHSLJRewardLayer();

	bool init(int goldcount);
	static JhHSLJRewardLayer* create(int goldcount);

private:
	void onGet(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onSuccess();
	void onErr(int errcode);
	int m_goldcount;
private:
};
#endif