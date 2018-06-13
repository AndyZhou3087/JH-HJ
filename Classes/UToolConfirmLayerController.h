#ifndef __ui_toolconfirm_layer_controller_h__
#define __ui_toolconfirm_layer_controller_h__

#include "cocos2d.h"
#include "LayerController.h"
#include "prefix.h"

USING_NS_CC;

typedef std::function<void()> ToolConfirmLayerCallback;

class UToolConfirmLayerController :public CenterLayerController{
	enum EnumToolConfirmType {
		ToolConfirm_Free,
		ToolConfirm_Disfree,
	};
public:
	UToolConfirmLayerController();
	~UToolConfirmLayerController();

	virtual void initial();
	virtual void enter() override;

	void enterFree();
	void enterDisfree();

	virtual void onPayToolConfirmComplete(bool success) override;

	void setExitCallback(const ToolConfirmLayerCallback& callback) { m_callback = callback; }

private:
	Button *m_close;
	Node *m_disfreeTip;
	Node *m_freeTip;
	Node *m_toolNum_1;
	Node *m_toolNum_2;

	EnumToolConfirmType m_type;
	ToolConfirmLayerCallback m_callback;
	
};

#endif