#ifndef __ui_unlock_layer_controller_h__
#define __ui_unlock_layer_controller_h__

#include "cocos2d.h"
#include "LayerController.h"
#include "prefix.h"

USING_NS_CC;


class UUnlockLayerController :public CenterLayerController{
	enum EnumUnlockUi {
		Ui_NotEnough_Star,
		Ui_Enough_Star,
		Ui_Pay_Unlock,
	};

public:
	UUnlockLayerController();
	~UUnlockLayerController();

	virtual void initial();
	void enter(int unlockLevel);
	virtual void onPayUnlockComplete(bool success) override;

private:
	void enterUiNotEnoughStar();
	void enterUiEnoughStar();
	void enterUiPayUnlock();

private:
	Node * m_panel_free;
	TextAtlas *m_starNum;
	Node *m_panel_disfree;
	Node *m_pay_tip;

	Button *m_unlockButton;
	Button *m_yesButton;
	Button *m_closeBtn;

	int m_levelRequireUnlock;
	EnumUnlockUi m_uiType;
};

#endif