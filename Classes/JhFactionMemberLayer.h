#ifndef _FACTIONMEMBER_LAYER_H_
#define _FACTIONMEMBER_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "JhServerDataSwap.h"
#include "JhGlobalData.h"
USING_NS_CC;

class JhFactionMemberLayer :public Layer, public ServerDataDelegateProtocol
{
public:
	JhFactionMemberLayer();
	~JhFactionMemberLayer();

	bool init(FactionListData* fldata);
	virtual void onEnterTransitionDidFinish();
	static JhFactionMemberLayer* create(FactionListData* fldata);
	void getFactionMemberData();
	void updateUi();
	void delayShowData(float dt);
private:
	void onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onAction(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onModity(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onContribution(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onSuccess();
	void onErr(int errcode);

private:
	cocos2d::ui::ScrollView* srollView;
	FactionListData* m_fldata;
	cocos2d::ui::Text* countlbl;
	cocos2d::ui::Text* explbl;
	cocos2d::ui::Text* lvlbl;
	FactionAction f_action;
	int usetypecontribution;
};

class FactionMemberItem :public Sprite, public ServerDataDelegateProtocol
{
public:
	FactionMemberItem();
	~FactionMemberItem();

	bool init(FactionMemberData *data);
	static FactionMemberItem* create(FactionMemberData *data);
	void updatePosition(int position);
	void removeItem();
private:
	void onAction(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onModify(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onRefuse(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onSuccess();
	void onErr(int errcode);

private:
	FactionMemberData* m_data;
	cocos2d::ui::Button *actionbtn;
	cocos2d::ui::Button* modifybtn;
	cocos2d::ui::Text* postionlbl;
	cocos2d::ui::Text* contributionlbl;
	FactionAction f_action;
	cocos2d::ui::Button* refusebtn;
};
#endif