#ifndef __pet_pay_h__
#define __pet_pay_h__

#include "cocos2d.h"
#include "SettingManager.h"

class URookieLayerController;
class ULuckyGiftLayerController;
class GameEnergyTipLayerController;
class UDiscountLayerController;
class GameBuyTimeLayerController;
class GameFailLayerController;
class UBigAwardLayerController;
class GameLotteryLayerController;
class USupriseLayerController;
class UGiftLayerController;
class GamePurchaseLayerController;
class UUnlockLayerController;
class GameLotteryLayerController;
class UExitLayerController;
class UToolConfirmLayerController;
class UGiftMoreLayerController;
class UExitGiftLayerController;
class UGuideTipLayerController;
 
class Pay {

public:
	static void onPayRookieComplete(bool success);
	static void onPayHappyComplete(bool success);
	static void onPayLuckyComplete(bool success);
	static void onPayAddEnergyComplete(bool success);
	static void onPayDiscountComplete(bool success);
	static void onPayAddTimeComplete(bool success);
	static void onPayPassGameComplete(bool success);
	static void onPayZhiZunComplete(bool success);
	static void onPayAllCardOpenComplete(bool success);
	static void onPaySupriseBoxComplete(bool success);
	static void onPayZunXiangComplete(bool success);
	static void onPayBuyTimeComplete(bool success);
	static void onPayBuyTipComplete(bool success);
	static void onPayBuyBombComplete(bool success);
	static void onPayBuyCrossComplete(bool success);
	static void onPayUnlockComplete(bool success);
	static void onPayFetchAllComplete(bool success);
	static void onPayHaoliComplete(bool success);
	static void onPayBuyEnergyComplete(bool success);
	static void onPayToolConfirmComplete(bool success);
	static void onPayGiftMoreComplete(bool success);
	static void onPayGuideTipComplete(bool success);

	static void onPayComplete(int payId, bool success);

	static void regRookieLis(URookieLayerController *lis) { m_rookieLis = lis;}
	static void unregRookieLis(URookieLayerController *lis) { if (lis == m_rookieLis) m_rookieLis = nullptr; }

	static void regHappyLis(URookieLayerController *lis) { m_happyLis = lis;}
	static void unregHappyLis(URookieLayerController *lis) { if (lis == m_happyLis) m_happyLis = nullptr; }

	static void regLuckyLis(ULuckyGiftLayerController *lis) { m_luckyLis = lis;}
	static void unregLuckyLis(ULuckyGiftLayerController *lis) { if (lis == m_luckyLis) m_luckyLis = nullptr; }

	static void regEnergytipLis(GameEnergyTipLayerController *lis) { m_addenergyLis = lis;}
	static void unregEnergytipLis(GameEnergyTipLayerController *lis) { if (lis == m_addenergyLis) m_addenergyLis = nullptr; }

	static void regDiscountLis(UDiscountLayerController *lis) { m_discountLis = lis;}
	static void unregDiscountLis(UDiscountLayerController *lis) { if (lis == m_discountLis) m_discountLis = nullptr; }

	static void regAddtimeLis(GameBuyTimeLayerController *lis) { m_addtimeLis = lis;}
	static void unregAddtimeLis(GameBuyTimeLayerController *lis) { if (lis == m_addtimeLis) m_addtimeLis = nullptr; }

	static void regPassgameLis(GameFailLayerController *lis) { m_passgameLis = lis;}
	static void unregPassgameLis(GameFailLayerController *lis) { if (lis == m_passgameLis) m_passgameLis = nullptr; }

	static void regZhizunLis(UGiftLayerController *lis) { m_zhizunLis = lis;}
	static void unregZhizunLis(UGiftLayerController *lis) { if (lis == m_zhizunLis) m_zhizunLis = nullptr; }

	static void regAllopenLis(GameLotteryLayerController *lis) { m_allopenLis = lis;}
	static void unregAllopenLis(GameLotteryLayerController *lis) { if (lis == m_allopenLis) m_allopenLis = nullptr; }

	static void regSupriseLis(USupriseLayerController *lis) { m_supriseLis = lis;}
	static void unregSupriseLis(USupriseLayerController *lis) { if (lis == m_supriseLis) m_supriseLis = nullptr; }

	static void regZunxiangLis(UBigAwardLayerController *lis) { m_zunxiangLis = lis;}
	static void unregZunxiangLis(UBigAwardLayerController *lis) { if (lis == m_zunxiangLis) m_zunxiangLis = nullptr; }

	static void regBuytimeLis(GamePurchaseLayerController *lis) { m_buytimeLis = lis;}
	static void unregBuytimeLis(GamePurchaseLayerController *lis) { if (lis == m_buytimeLis) m_buytimeLis = nullptr; }

	static void regBuytipLis(GamePurchaseLayerController *lis) { m_buytipLis = lis;}
	static void unregBuytipLis(GamePurchaseLayerController *lis) { if (lis == m_buytipLis) m_buytipLis = nullptr; }

	static void regBuybombLis(GamePurchaseLayerController *lis) { m_buybombLis = lis;}
	static void unregBuybombLis(GamePurchaseLayerController *lis) { if (lis == m_buybombLis) m_buybombLis = nullptr; }

	static void regBuycrossLis(GamePurchaseLayerController *lis) { m_buycrossLis = lis;}
	static void unregBuycrossLis(GamePurchaseLayerController *lis) { if (lis == m_buycrossLis) m_buycrossLis = nullptr; }

	static void regUnlockLis(UUnlockLayerController *lis) { m_unlockLis = lis;}
	static void unregUnlockLis(UUnlockLayerController *lis) { if (lis == m_unlockLis) m_unlockLis = nullptr; }

	static void regFetchallLis(GameLotteryLayerController *lis) { m_fetchallLis = lis;}
	static void unregFetchallLis(GameLotteryLayerController *lis) { if (lis == m_fetchallLis) m_fetchallLis = nullptr; }

	static void regHaoliLis(UExitLayerController *lis) { m_haoliLis = lis;}
	static void unregHaoliLis(UExitLayerController *lis) { if (lis == m_haoliLis) m_haoliLis = nullptr; }

	static void regHaoli2Lis(UExitGiftLayerController *lis) { m_haoli2Lis = lis;}
	static void unregHaoli2Lis(UExitGiftLayerController *lis) { if (lis == m_haoli2Lis) m_haoli2Lis = nullptr; }

	static void regBuyenergyLis(GamePurchaseLayerController *lis) { m_buyenergyLis = lis;}
	static void unregBuyenergyLis(GamePurchaseLayerController *lis) { if (lis == m_buyenergyLis) m_buyenergyLis = nullptr; }

	static void regToolconfirmLis(UToolConfirmLayerController *lis) { m_toolconfirmLis = lis;}
	static void unregToolconfirmLis(UToolConfirmLayerController *lis) { if (lis == m_toolconfirmLis) m_toolconfirmLis = nullptr; }

	static void regGiftMoreLis(UGiftMoreLayerController *lis) { m_giftmoreLis = lis;}
	static void unregGiftMoreLis(UGiftMoreLayerController *lis) { if (lis == m_giftmoreLis) m_giftmoreLis = nullptr; }

	static void regGuideTipLis(UGuideTipLayerController *lis) { m_guidetipLis = lis; }
	static void unregGuideTipLis(UGuideTipLayerController *lis) { if (lis == m_guidetipLis) m_guidetipLis = nullptr; }

	static void removeAllLis();

private:
	static URookieLayerController			*m_rookieLis;
	static URookieLayerController			*m_happyLis;
	static ULuckyGiftLayerController		*m_luckyLis;
	static GameEnergyTipLayerController		*m_addenergyLis;
	static UDiscountLayerController			*m_discountLis;
	static GameBuyTimeLayerController		*m_addtimeLis;
	static GameFailLayerController			*m_passgameLis;
	static UGiftLayerController				*m_zhizunLis;
	static GameLotteryLayerController		*m_allopenLis;
	static USupriseLayerController			*m_supriseLis;
	static UBigAwardLayerController			*m_zunxiangLis;
	static GamePurchaseLayerController		*m_buytimeLis;
	static GamePurchaseLayerController		*m_buytipLis;
	static GamePurchaseLayerController		*m_buybombLis;
	static GamePurchaseLayerController		*m_buycrossLis;
	static UUnlockLayerController			*m_unlockLis;
	static GameLotteryLayerController		*m_fetchallLis;
	static UExitLayerController				*m_haoliLis;
	static UExitGiftLayerController			*m_haoli2Lis;
	static GamePurchaseLayerController		*m_buyenergyLis;
	static UToolConfirmLayerController		*m_toolconfirmLis;
	static UGiftMoreLayerController			*m_giftmoreLis;
	static UGuideTipLayerController			*m_guidetipLis;

};

#endif