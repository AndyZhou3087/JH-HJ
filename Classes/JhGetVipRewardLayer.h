#ifndef _GETVIPREWARD_LAYER_H_
#define _GETVIPREWARD_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "JhGlobalData.h"
#include "JhServerDataSwap.h"
USING_NS_CC;

class JhGetVipRewardLayer :public Layer, public ServerDataDelegateProtocol
{
public:
	JhGetVipRewardLayer();
	~JhGetVipRewardLayer();

	bool init();
	static JhGetVipRewardLayer* create();

private:
	int rgoldcount;
	std::vector<int> vec_rgoodsindex;
private:
	void onClose(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onSuccess();
	void onErr(int errcode);
	void getVipGoods();
};
#endif

