#include "PaymentInterface.h"
#include "cocos2d.h"
#include "global.h"
#include "Pay.h"
#include "GameUtil.h"
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "../proj.android/jni/hellocpp/JniInterface.h"
#endif

#define PAY_SUCCESS 1
#define PAY_FAIL 2
#define PAY_CANCEL 3

using namespace cocos2d;

void startPayment( int itemId)
{
	if (s_game_test_flag) {
		auto scheduleCallback = [=](float dt){
		payCallback(1, itemId);
		};
		auto _schedule = Director::sharedDirector()->getRunningScene()->getScheduler();
		_schedule->schedule(scheduleCallback, TestObj::getInstance(), 0.0f,0,0.2f, false, String::createWithFormat("justtest_%d",itemId)->_string);
		return;
	}
#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
 	startPay(itemId);
#endif
}


void payCallback(int status, int itemId)
{
	if (s_game_test_flag) {
		Pay::onPayComplete(itemId, true);
		return;
	}

#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
 	switch(status)
	{
	case PAY_SUCCESS:
		//CCDirector::sharedDirector()->end();
		Pay::onPayComplete(itemId, true);
		callbackPayment(PAY_SUCCESS);
		break;
		
	case PAY_FAIL:
		Pay::onPayComplete(itemId, false);
		callbackPayment(PAY_FAIL);
		break;

	case PAY_CANCEL:
		Pay::onPayComplete(itemId, false);
		callbackPayment(PAY_CANCEL);
		break;
		
	default:
		log("***pay back:%d,other", itemId);
		Pay::onPayComplete(itemId, false);
		callbackPayment(100);
		break;
	}
#endif

}
char * getUserID()
{
	return "000000"; 
}
