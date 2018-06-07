#ifndef __layer_controller_h__
#define __layer_controller_h__

#include "cocos2d.h"
USING_NS_CC;

class LayerController {
public:
	LayerController();
	virtual ~LayerController();

	enum EnumFragmentColor {
		Color_Red,
		Color_Blue,
		Color_Green,
		Color_Yellow,
	};

	virtual void enter();
	virtual void exit();
	void regLayer(Node *l);
	virtual void regNextLayer(LayerController * l) { m_nextLayer = l; }
	virtual void regExitLayer(LayerController * l) { m_exitLayer = l; }
	virtual void regBgLayer(LayerController *l) { m_bgLayer = l; }

	inline bool isEnter() { return m_bEnter; }

	virtual void initial() = 0;

	virtual void onUpdate() {};
	virtual void onGameStart() {};
	virtual void onGameRestart() {};
	virtual void onGamePause() {};
	virtual void onGameResume() {};
	virtual void onGameRelive() {};
	virtual void onGameOver() {}
	virtual void onGameScoreChange() {};
	virtual void onGameReady() {};
	virtual void onGameFinish() {};

	virtual void enterBg();
	virtual void exitBg();

	virtual void playEffectNode();
	virtual void stopEffectNode();

	virtual bool onTouch(Touch *, Event *){ return false; }

	virtual void onPayRookieComplete(bool success) {}
	virtual void onPayHappyComplete(bool success){}
	virtual void onPayLuckyComplete(bool success){}
	virtual void onPayAddEnergyComplete(bool success){}
	virtual void onPayDiscountComplete(bool success){}
	virtual void onPayAddTimeComplete(bool success){}
	virtual void onPayPassGameComplete(bool success){}
	virtual void onPayZhiZunComplete(bool success){}
	virtual void onPayAllCardOpenComplete(bool success){}
	virtual void onPaySupriseBoxComplete(bool success){}
	virtual void onPayZunXiangComplete(bool success){}
	virtual void onPayBuyTimeComplete(bool success){}
	virtual void onPayBuyTipComplete(bool success){}
	virtual void onPayBuyBombComplete(bool success){}
	virtual void onPayBuyCrossComplete(bool success){}
	virtual void onPayUnlockComplete(bool success){}
	virtual void onPayFetchAllComplete(bool success){}
	virtual void onPayHaoliComplete(bool success){}
	virtual void onPayBuyEnergyComplete(bool success){}
	virtual void onPayToolConfirmComplete(bool success) {}
	virtual void onPayGiftMoreComplete(bool success) {}

	virtual void onPayGuideTipComplete(bool success) {}

protected:
	LayerController *m_nextLayer;
	LayerController *m_exitLayer;
	LayerController *m_bgLayer;


	Node *m_layer;
	bool m_bEnter;
	 
	Node *m_effectNode1;
	Node *m_effectNode2;
	Node *m_effectNode3;
	Node *m_effectNode4;
	Node *m_effectNode5;
	Node *m_effectNode6;
};

class BottomLayerController :public LayerController {
public:
	virtual void enter();
	virtual void exit();
};

class CenterLayerController :public LayerController {
public:
	virtual void enter();
	virtual void exit();
	virtual void enterEaseSineOut();
};

#endif