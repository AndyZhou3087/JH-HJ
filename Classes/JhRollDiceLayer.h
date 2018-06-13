#ifndef _ROLLDICE_LAYER_H_
#define _ROLLDICE_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "JhGlobalData.h"
#include "JhServerDataSwap.h"
USING_NS_CC;

class JhRollDiceLayer :public Layer, public ServerDataDelegateProtocol
{
public:
	JhRollDiceLayer();
	~JhRollDiceLayer();

	bool init();
	static JhRollDiceLayer* create();

private:
	void onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onRoll(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onSuccess();
	void onErr(int errcode);

	void showResult(float dt);
	void openDice(float dt);
private:
	void refreshGoldCount(float dt);
	cocos2d::ui::Text* mygoldlbl;
	cocos2d::ui::Text* costgoldlbl;
	cocos2d::ui::Widget* costgold;
	cocos2d::ui::Text* counttext;
	cocos2d::ui::Text* countlbl;
	int actiontype;
	void setUiVisual(bool val);
	cocos2d::ui::Widget* rollbtn;
	cocos2d::ui::Button* backbtn;
	cocos2d::ui::Text* word;
	int useGold;
	cocostudio::timeline::ActionTimeline* anim_action;
	Node* animnode;
	int winbs;//倍数

};
#endif

