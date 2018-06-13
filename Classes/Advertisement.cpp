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
	//�жϵ�ǰ�Ƿ�ΪAndroidƽ̨
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) 
	//����Jni������Ϣ�ṹ��
	JniMethodInfo minfo;
	//����һ��boolֵ��ʾ�Ƿ��ҵ��˺���
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
    //�жϵ�ǰ�Ƿ�ΪAndroidƽ̨
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    //����Jni������Ϣ�ṹ��
    JniMethodInfo minfo;
    //����һ��boolֵ��ʾ�Ƿ��ҵ��˺���
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
	//�жϵ�ǰ�Ƿ�ΪAndroidƽ̨
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) 
	//����Jni������Ϣ�ṹ��
	JniMethodInfo minfo;
	//����һ��boolֵ��ʾ�Ƿ��ҵ��˺���
	bool isHave = JniHelper::getStaticMethodInfo(minfo,"lemo/game/popstar2015/AppActivity","clearInlineAd", "()V");

	if (isHave) {
		minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
	}
#endif
}

bool DomobAdvertisement::showIntersAd()
{
	//�жϵ�ǰ�Ƿ�ΪAndroidƽ̨
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID) 
	//����Jni������Ϣ�ṹ��
	JniMethodInfo minfo;
	//����һ��boolֵ��ʾ�Ƿ��ҵ��˺���
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
