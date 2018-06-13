#ifndef _PRIZE_LAYER_H_
/********************************************************************
* 兑换码界面
* 
*********************************************************************/
#define _PRIZE_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "JhServerDataSwap.h"
USING_NS_CC;

class JhPrizeLayer :public Layer, public cocos2d::ui::EditBoxDelegate, public ServerDataDelegateProtocol
{
public:
	JhPrizeLayer();
	~JhPrizeLayer();

	virtual bool init();
	CREATE_FUNC(JhPrizeLayer);
private:

	void onOk(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	void onClose(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	void onHuafeiEnter(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	void editBoxEditingDidBegin(cocos2d::ui::EditBox* editBox);

	void editBoxEditingDidEnd(cocos2d::ui::EditBox* editBox);

	void editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string &text);

	void editBoxReturn(cocos2d::ui::EditBox *editBox);

	void onSuccess();
	void onErr(int errcode);

private:
	cocos2d::ui::EditBox* m_edit;
	cocos2d::ui::TextField* m_input;
	cocos2d::ui::Button* m_okbtn;
private:
	void textFieldEvent(Ref * pSender, cocos2d::ui::TextField::EventType type);
	void showRwd();
	void addRes();
};
#endif

