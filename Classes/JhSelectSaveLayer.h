/********************************************************************
*����ѡ�����
*********************************************************************/
#ifndef __SELECTSAVELAYER__
#define __SELECTSAVELAYER__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
USING_NS_CC;

class JhSelectSaveLayer : public Layer
{
public:
	/****************************
	��ʼ��
	*****************************/
	virtual bool init();

	CREATE_FUNC(JhSelectSaveLayer);

private:

	/****************************
	��������ء���ť�ص�
	*****************************/
	void onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	/****************************
	ѡ�д浵
	*****************************/
	void onSelect(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	/****************************
	�Ƴ���ǰ��
	*****************************/
	void removSelf();
private:
	Node* m_saveNode[4];//�����4����ɫ�ڵ�
};

#endif
