#ifndef __homemenu_controller_h__
#define __homemenu_controller_h__

#include "cocos2d.h"
#include "MenuController.h"
#include "prefix.h"

USING_NS_CC;

class LayerController;
class URookieLayerController;
class GameIntroLayerController;

class HomeMenuController :public MenuController{
public:
	HomeMenuController();
	~HomeMenuController();

	virtual void initial();
	virtual void enter();
	virtual void exit();

	void regRookieLayer(URookieLayerController *l) { m_uiRookieLayer = l; }
	void regIntroLayer(GameIntroLayerController *l) { m_introLayer = l; }

private:
	inline float getGiftPosY() { return m_giftPosY; }

	void playGiftEffect();
	void stopGiftEffect();

private:
	Button *m_gift;
	Button *m_sound_on;
	Button *m_sound_off;
	cocos2d::Node *m_logo;
	float m_logoPosY;
	float m_giftPosY;

	URookieLayerController *m_uiRookieLayer;
	GameIntroLayerController *m_introLayer;
};

#endif