#ifndef __lucky_layer_controller_h__
#define __lucky_layer_controller_h__

#include "LayerController.h"
#include "prefix.h"

class UAwardLayerController;
class ULuckyGiftLayerController;

enum EnumLuckyAward {
	Award_Lottery_x3 = 0,
	Award_Gift,
	Award_Energy_x1,
	Award_Time_x1,
	Award_Tip_x10,
	Award_Energy_x10,
	Award_Lotter_x1,
	Award_Cross_x10,
};


class ULuckyLayerController:public CenterLayerController {
public:
	ULuckyLayerController();
	~ULuckyLayerController();

	virtual void initial();
	virtual void enter();
	virtual void exit();

	void regAwardLayer(UAwardLayerController *l) { m_awardLayer = l; }
	void regGiftLayer(ULuckyGiftLayerController *l) { m_giftLayer = l; }


private:
	void startLottery();

	EnumLuckyAward getRandomAwardItem();
	void startAwardTime();
	void startAwardEnergy();
	void startAwardLotteryOne();
	void startAwardGift();

	void playLabelStartEffect();
	void stopLabelStartEffect();

private:
	cocos2d::Sprite *m_dish;
	cocos2d::Sprite *m_labelStart;
	Button *m_spinBtn;
	UAwardLayerController *m_awardLayer;
	ULuckyGiftLayerController *m_giftLayer;
};

#endif