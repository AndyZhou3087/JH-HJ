/********************************************************************
*���ȷ�Ͻ���
*********************************************************************/
#ifndef __FACTIONKICKCOMFIRMLAYER__
#define __FACTIONKICKCOMFIRMLAYER__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "JhServerDataSwap.h"
#include "JhGlobalData.h"
USING_NS_CC;

class JhFactionKickComfirmLayer : public Layer, public ServerDataDelegateProtocol
{
public:
	/****************************
	��ʼ��
	*****************************/
	bool init(Node* target, FactionMemberData *fmdata);

	static JhFactionKickComfirmLayer* create(Node* target, FactionMemberData *fmdata);

private:
	/****************************
	�����ȷ�ϡ���ť�ص�
	*****************************/
	void onOk(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	/****************************
	�����ȡ������ť�ص�
	*****************************/
	void onCancel(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	/****************************
	�Ƴ���ǰ��
	*****************************/
	void removSelf();

	void onSuccess();
	void onErr(int errcode);
private:
	FactionMemberData *m_data;
	Node* m_target;
};

#endif
