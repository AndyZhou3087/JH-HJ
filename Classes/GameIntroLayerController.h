#ifndef __game_intro_layer_controller_h__
#define __game_intro_layer_controller_h__

#include "LayerController.h"
#include "prefix.h"


class GameIntroLayerController:public CenterLayerController {
public:
	GameIntroLayerController();
	~GameIntroLayerController();

	virtual void initial();
	virtual void enter() override;
	virtual void exit() override;

private:
	cocos2d::Sprite *m_ball_red;
	cocos2d::Sprite *m_ball_yellow;

	float m_leftX;
	float m_rightX;
};

#endif