#include "Analytics.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#include "../proj.android/jni/hellocpp/JniInterface.h"
#endif

USING_NS_CC;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//#include "TalkingData.h"
#endif

void Analytics::setAccount() {

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	//talkingdata Í³¼Æ
	//TDCCAccount* account = TDCCAccount::setAccount(TDCCTalkingDataGA::getDeviceId()); 
	//account->setAccountType(TDCCAccount::kAccountAnonymous);
#endif

}

void Analytics::onBegin(int level) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	//const int SIZE = 20;
	//char buffer[SIZE] = {0};
	//snprintf(buffer,SIZE,"begin level:%d",level);
	//TDCCMission::onBegin(buffer);
#endif
}

void Analytics::onComplete(int level) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	//const int SIZE = 20;
	//char buffer[SIZE] = {0};
	//snprintf(buffer,SIZE,"complete level:%d",level);
	//TDCCMission::onCompleted(buffer);
#endif
}

void Analytics::onFailed(int level) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	//const int SIZE = 20;
	//char buffer[SIZE] = {0};
	//snprintf(buffer,SIZE,"Game Over.level:%d",level);
	//TDCCMission::onFailed(buffer,"score");
#endif
}


void UMengAnalytics::onBegin(int level) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//	startLevel(level);
#endif
}

void UMengAnalytics::onComplete(int level, int sec, int targetSec) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//	finishLevel(level, sec, targetSec);
#endif
}

void UMengAnalytics::onFailed(int level, int score, int targetScore) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//	failLevel(level, score, targetScore);
#endif
}

void UMengAnalytics::onFailedBeforeSuccess(int level, int score, int targetScore) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//	failBeforeSuccess(level, score, targetScore);
#endif
}


