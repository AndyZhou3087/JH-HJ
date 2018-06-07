#include "ResourceUtil.h"
#include "GameUtil.h"
USING_NS_CC;

void ResourceUtil::initLocal() {
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	ccLanguageType currentLanguageType = CCApplication::sharedApplication()->getCurrentLanguage();
	
	switch (currentLanguageType)  
	{    
	case kLanguageChinese: 
		CCFileUtils::sharedFileUtils()->addSearchPath("zh-Hans.lproj", true); 
		break;  
	case kLanguageEnglish:
	default:
		CCFileUtils::sharedFileUtils()->addSearchPath("en.lproj", true);
		break;  
	} 

	CCFileUtils::sharedFileUtils()->addSearchPath("home", false);
	CCFileUtils::sharedFileUtils()->addSearchPath("public", false);
	CCFileUtils::sharedFileUtils()->addSearchPath("game", false);
	CCFileUtils::sharedFileUtils()->addSearchPath("ui", false);
	CCFileUtils::sharedFileUtils()->addSearchPath("sound", false);

#endif
	
}

