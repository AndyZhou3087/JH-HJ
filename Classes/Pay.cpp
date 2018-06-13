#include "Pay.h"
#include "SettingManager.h"

#include "URookieLayerController.h"
#include "URookieLayerController.h"
#include "ULuckyGiftLayerController.h"
#include "GameEnergyTipLayerController.h"
#include "UDiscountLayerController.h"
#include "GameBuyTimeLayerController.h"
#include "GameFailLayerController.h"
#include "UBigAwardLayerController.h"
#include "GameLotteryLayerController.h"
#include "USupriseLayerController.h"
#include "UGiftLayerController.h"
#include "GamePurchaseLayerController.h"
#include "UUnlockLayerController.h"
#include "GameLotteryLayerController.h"
#include "UExitLayerController.h"
#include "GamePurchaseLayerController.h"
#include "UToolConfirmLayerController.h"
#include "UGiftMoreLayerController.h"
#include "UExitGiftLayerController.h"
#include "UGuideTipLayerController.h"

URookieLayerController			*Pay::m_rookieLis = NULL;
URookieLayerController			*Pay::m_happyLis = NULL;
ULuckyGiftLayerController		*Pay::m_luckyLis = NULL;
GameEnergyTipLayerController	*Pay::m_addenergyLis = NULL;
UDiscountLayerController		*Pay::m_discountLis = NULL;
GameBuyTimeLayerController		*Pay::m_addtimeLis = NULL;
GameFailLayerController			*Pay::m_passgameLis = NULL;
UGiftLayerController			*Pay::m_zhizunLis = NULL;
GameLotteryLayerController		*Pay::m_allopenLis = NULL;
USupriseLayerController			*Pay::m_supriseLis = NULL;
UBigAwardLayerController		*Pay::m_zunxiangLis = NULL;
GamePurchaseLayerController		*Pay::m_buytimeLis = NULL;
GamePurchaseLayerController		*Pay::m_buytipLis = NULL;
GamePurchaseLayerController		*Pay::m_buybombLis = NULL;
GamePurchaseLayerController		*Pay::m_buycrossLis = NULL;
UUnlockLayerController			*Pay::m_unlockLis = NULL;
GameLotteryLayerController		*Pay::m_fetchallLis = NULL;
UExitLayerController			*Pay::m_haoliLis = NULL;
GamePurchaseLayerController		*Pay::m_buyenergyLis = NULL;
UToolConfirmLayerController		*Pay::m_toolconfirmLis = NULL;
UGiftMoreLayerController		*Pay::m_giftmoreLis = NULL;
UExitGiftLayerController		*Pay::m_haoli2Lis = NULL;
UGuideTipLayerController		*Pay::m_guidetipLis = NULL;

void Pay::onPayRookieComplete(bool success) {
	if (nullptr != m_rookieLis) {
		m_rookieLis->onPayRookieComplete(success);
	}
}

void Pay::onPayHappyComplete(bool success) {
	if (nullptr != m_happyLis) {
		m_happyLis->onPayHappyComplete(success);
	}
}

void Pay::onPayLuckyComplete(bool success) {
	if (nullptr != m_luckyLis) {
		m_luckyLis->onPayLuckyComplete(success);
	}
}

void Pay::onPayAddEnergyComplete(bool success) {
	if (nullptr != m_addenergyLis) {
		m_addenergyLis->onPayAddEnergyComplete(success);
	}
}

void Pay::onPayDiscountComplete(bool success) {
	if (nullptr != m_discountLis) {
		m_discountLis->onPayDiscountComplete(success);
	}
}

void Pay::onPayAddTimeComplete(bool success) {
	if (nullptr != m_addtimeLis) {
		m_addtimeLis->onPayAddTimeComplete(success);
	}
}

void Pay::onPayPassGameComplete(bool success) {
	if (nullptr != m_passgameLis) {
		m_passgameLis->onPayPassGameComplete(success);
	}
}

void Pay::onPayZhiZunComplete(bool success) {
	if (nullptr != m_zhizunLis) {
		m_zhizunLis->onPayZhiZunComplete(success);
	}
}

void Pay::onPayAllCardOpenComplete(bool success) {
	if (nullptr != m_allopenLis) {
		m_allopenLis->onPayAllCardOpenComplete(success);
	}
}

void Pay::onPaySupriseBoxComplete(bool success) {
	if (nullptr != m_supriseLis) {
		m_supriseLis->onPaySupriseBoxComplete(success);
	}
}

void Pay::onPayZunXiangComplete(bool success) {
	if (nullptr != m_zunxiangLis) {
		m_zunxiangLis->onPayZunXiangComplete(success);
	}
}

void Pay::onPayBuyTimeComplete(bool success) {
	if (nullptr != m_buytimeLis) {
		m_buytimeLis->onPayBuyTimeComplete(success);
	}
}

void Pay::onPayBuyTipComplete(bool success) {
	if (nullptr != m_buytipLis) {
		m_buytipLis->onPayBuyTipComplete(success);
	}
}

void Pay::onPayBuyBombComplete(bool success) {
	if (nullptr != m_buybombLis) {
		m_buybombLis->onPayBuyBombComplete(success);
	}
}

void Pay::onPayBuyCrossComplete(bool success) {
	if (nullptr != m_buycrossLis) {
		m_buycrossLis->onPayBuyCrossComplete(success);
	}
}

void Pay::onPayUnlockComplete(bool success) {
	if (nullptr != m_unlockLis) {
		m_unlockLis->onPayUnlockComplete(success);
	}
}

void Pay::onPayFetchAllComplete(bool success) {
	if (nullptr != m_fetchallLis) {
		m_fetchallLis->onPayFetchAllComplete(success);
	}
}

void Pay::onPayHaoliComplete(bool success) {
	if (nullptr != m_haoliLis) {
		m_haoliLis->onPayHaoliComplete(success);
	}

	if (nullptr != m_haoli2Lis) {
		m_haoli2Lis->onPayHaoliComplete(success);
	}
}

void Pay::onPayBuyEnergyComplete(bool success) {
	if (nullptr != m_buyenergyLis) {
		m_buyenergyLis->onPayBuyEnergyComplete(success);
	}
}

void Pay::onPayToolConfirmComplete(bool success) {
	if (nullptr != m_toolconfirmLis) {
		m_toolconfirmLis->onPayToolConfirmComplete(success);
	}
}

void Pay::onPayGiftMoreComplete(bool success) {
	if (nullptr != m_giftmoreLis) {
		m_giftmoreLis->onPayGiftMoreComplete(success);
	}
}

void Pay::onPayGuideTipComplete(bool success) {
	if (nullptr != m_guidetipLis) {
		m_guidetipLis->onPayGuideTipComplete(success);
	}
}
void Pay::onPayComplete(int payId, bool success) {
	switch(payId) {
	case Setting_rookie:
		onPayRookieComplete(success);
		break;
	case Setting_happy:
		onPayHappyComplete(success);
		break;
	case Setting_luckylottery:
		onPayLuckyComplete(success);
		break;
	case Setting_addenergy:
		onPayAddEnergyComplete(success);
		break;
	case Setting_discount:
		onPayDiscountComplete(success);
		break;
	case Setting_addtime:
		onPayAddTimeComplete(success);
		break;
	case Setting_nextgame:
		onPayPassGameComplete(success);
		break;
	case Setting_biglihe:
		onPayZhiZunComplete(success);
		break;
	case Setting_allopen:
		onPayAllCardOpenComplete(success);
		break;
	case Setting_suprise:
		onPaySupriseBoxComplete(success);
		break;
	case Setting_biglibao:
		onPayZunXiangComplete(success);
		break;
	case Setting_buytime:
		onPayBuyTimeComplete(success);
		break;
	case Setting_buytip:
		onPayBuyTipComplete(success);
		break;
	case Setting_buybomb:
		onPayBuyBombComplete(success);
		break;
	case Setting_buycross:
		onPayBuyCrossComplete(success);
		break;
	case Setting_unlock:
		onPayUnlockComplete(success);
		break;
	case Setting_fetchall:
		onPayFetchAllComplete(success);
		break;
	case Setting_exit:
		onPayHaoliComplete(success);
		break;
	case Setting_buyenergy:
		onPayBuyEnergyComplete(success);
		break;
	case Setting_toolconfirm:
		onPayToolConfirmComplete(success);
		break;
	case Setting_giftmore:
		onPayGiftMoreComplete(success);
		break;
	case Setting_guidetip:
		onPayGuideTipComplete(success);
		break;
	}
}

void Pay::removeAllLis() {
	m_rookieLis = NULL;
	m_happyLis = NULL;
	m_luckyLis = NULL;
	m_addenergyLis = NULL;
	m_discountLis = NULL;
	m_addtimeLis = NULL;
	m_passgameLis = NULL;
	m_zhizunLis = NULL;
	m_allopenLis = NULL;
	m_supriseLis = NULL;
	m_zunxiangLis = NULL;
	m_buytimeLis = NULL;
	m_buytipLis = NULL;
	m_buybombLis = NULL;
	m_buycrossLis = NULL;
	m_unlockLis = NULL;
	m_fetchallLis = NULL;
	m_haoliLis = NULL;
	m_buyenergyLis = NULL;
	m_toolconfirmLis = NULL;
}