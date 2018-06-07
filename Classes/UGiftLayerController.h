#ifndef __gift_layer_controller_h__
#define __gift_layer_controller_h__

#include "cocos2d.h"
#include "LayerController.h"
#include "prefix.h"

USING_NS_CC;

class UGiftLayerController :public CenterLayerController{
public:
	UGiftLayerController();
	~UGiftLayerController();

	virtual void initial();
	virtual void enter();
	virtual void exit();

	virtual void onPayZhiZunComplete(bool success) override;

private:
	void initialFrameless();
	void initialFrame();

	void enterFree();
	void enterDisfree();

private:
	TextAtlas *m_tipNum;
	Button *m_closeBtn;
	cocos2d::Node *m_freeTip;
	cocos2d::Node *m_disfreeTip;
	cocos2d::Node *m_disfreePayTip;

	bool m_bFree;
};

#endif