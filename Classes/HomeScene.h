#ifndef __home_scene_h__
#define __home_scene_h__
#include <cocos2d.h>

class HomeMenuController;
class URookieLayerController;
class GameIntroLayerController;
class BgLayerController;
class GreyLayerController;
class UExitLayerController;
class UExitGiftLayerController;

class HomeScene :public cocos2d::CCLayer
{
public:
	HomeScene(void);
	~HomeScene(void);
	CREATE_FUNC(HomeScene);
	static cocos2d::Scene * createScene();
	virtual bool init();
	virtual void onEnter();
	virtual void onExit();

	void onExitCancel(cocos2d::EventCustom*);
	void onCloseIntro(cocos2d::EventCustom*);

private:
	void initBg();
	void initUi();
	void initJFBg();

	void initController();
	void initJFController();

	void regGameEvent();
	void unregGameEvent();
	void regSysEvent();
	void unregSysEvent();

private:
	cocos2d::Sprite *m_bg;
	cocos2d::Node *m_bggreyLayer;
	cocos2d::Node *m_homeMenuLayer;
	cocos2d::Node *m_uiRookieLayer;
	cocos2d::Node *m_introLayer;
	cocos2d::Node *m_bgLayer;
	cocos2d::Node *m_uiExitLayer;
	cocos2d::Node *m_uiExitGiftLayer;

	HomeMenuController	*m_homeMenuContrl;
	URookieLayerController	*m_uiRookieLayerController;
	GameIntroLayerController *m_introLayerController;
	BgLayerController	*m_bgLayerController;
	GreyLayerController *m_greyLayerController;
	UExitLayerController *m_uiExitLayerController;
	UExitGiftLayerController	*m_uiExitGiftLayerController;

	cocos2d::EventListener *m_ExitCancelLis;
	cocos2d::EventListener *m_CloseIntroLis;

	cocos2d::EventListenerKeyboard *m_keyboradEventListener;
	bool m_backTouched;

};

#endif