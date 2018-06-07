#ifndef _ADDFIGHTCOUNT_LAYER_H_
#define _ADDFIGHTCOUNT_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "JhServerDataSwap.h"
#include "JhGlobalData.h"
USING_NS_CC;
class JhAddFightCountLayer :public Layer, public ServerDataDelegateProtocol
{
public:
	JhAddFightCountLayer();
	~JhAddFightCountLayer();

	bool init(int matchtype);
	static JhAddFightCountLayer* create(int matchtype);

private:
	void onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onAddCount(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onSuccess();
	void onErr(int errcode);
private:
	Node* m_csbnode;
	int buycount;
	int m_matchtype;
};
#endif