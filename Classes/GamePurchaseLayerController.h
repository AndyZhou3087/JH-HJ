#ifndef __game_purchase_layer_controller_h__
#define __game_purchase_layer_controller_h__

#include "LayerController.h"
#include "prefix.h"
#include "global.h"

class UDiscountLayerController;

class GamePurchaseLayerController:public LayerController {
public:
	GamePurchaseLayerController();
	~GamePurchaseLayerController();

	virtual void initial();
	virtual void enter();
	virtual void exit();

	void purchase(EnumGameTool tool);

	void purchaseCross();
	void purchaseTime();
	void purchaseTip();
	void purchaseBomb();
	void purchaseEnergy();

	virtual void onPayBuyTimeComplete(bool success) override;
	virtual void onPayBuyTipComplete(bool success) override;
	virtual void onPayBuyBombComplete(bool success) override;
	virtual void onPayBuyCrossComplete(bool success) override;
	virtual void onPayBuyEnergyComplete(bool success) override;
	 

private:
	static const char *getToolImage(EnumGameTool tool);

private:
	cocos2d::Sprite *m_toolImage;
	EnumGameTool m_toolType;
	TextAtlas *m_toolNum;
};

#endif