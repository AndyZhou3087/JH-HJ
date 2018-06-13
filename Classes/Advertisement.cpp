#include "Advertisement.h"
#include "cocos2d.h"

USING_NS_CC;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//#include "PluginHelper.h"
#endif

void DomobAdvertisement::showInlineAd()
{
	//判断当前是否为Android平台
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) 
	//定义Jni函数信息结构体
	JniMethodInfo minfo;
	//返回一个bool值表示是否找到此函数
	bool isHave = JniHelper::getStaticMethodInfo(minfo,"lemo/game/popstar2015/AppActivity","showInlineAd", "()V");

	if (isHave) {
		minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
	}
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//    plugin_showInlineAd();
#endif

}

void DomobAdvertisement::hideInlineAd()
{
    //判断当前是否为Android平台
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    //定义Jni函数信息结构体
    JniMethodInfo minfo;
    //返回一个bool值表示是否找到此函数
    bool isHave = JniHelper::getStaticMethodInfo(minfo,"lemo/game/popstar2015/AppActivity","hideInlineAd", "()V");
    
    if (isHave) {
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
    }
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//    plugin_hideInlineAd();
#endif
    
}

void DomobAdvertisement::clearInlineAd()
{
	//判断当前是否为Android平台
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) 
	//定义Jni函数信息结构体
	JniMethodInfo minfo;
	//返回一个bool值表示是否找到此函数
	bool isHave = JniHelper::getStaticMethodInfo(minfo,"lemo/game/popstar2015/AppActivity","clearInlineAd", "()V");

	if (isHave) {
		minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
	}
#endif
}

bool DomobAdvertisement::showIntersAd()
{
	//判断当前是否为Android平台
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) 
	//定义Jni函数信息结构体
	JniMethodInfo minfo;
	//返回一个bool值表示是否找到此函数
	bool isHave = JniHelper::getStaticMethodInfo(minfo,"lemo/game/popstar2015/AppActivity","showIntersAd", "()V");

	if (isHave) {
		minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
	}
    return true;
#endif
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
//    return plugin_showIntersAd();
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	return true;
#endif
}
