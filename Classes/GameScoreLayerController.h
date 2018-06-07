#ifndef __game_score_layer_h__
#define __game_score_layer_h__

#include "LayerController.h"
#include "cocos2d.h"
#include "prefix.h"

class GameScoreLayerController:public LayerController {
public:
	GameScoreLayerController();
	~GameScoreLayerController();

	virtual void initial();
	virtual void enter();
	virtual void exit();

private:
	cocos2d::Sprite *m_star1;
	cocos2d::Sprite *m_star2;
	cocos2d::Sprite *m_star3;
	TextAtlas *m_target;
	TextAtlas *m_score;
	TextAtlas *m_time;
};

#endif