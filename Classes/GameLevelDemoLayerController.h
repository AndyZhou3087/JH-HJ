#ifndef __game_level_demo_layer_controller_h__
#define __game_level_demo_layer_controller_h__

#include "LayerController.h"
#include "prefix.h"
#include "DemoPetData.h"

class DemoPetMatrix;
class BgLayerController;

class GameLevelDemoLayer:public cocos2d::Layer {
public:
	GameLevelDemoLayer();
	~GameLevelDemoLayer();
	CREATE_FUNC(GameLevelDemoLayer);

	virtual bool init() override;
};

class GameLevelDemoLayerController:public BottomLayerController {
public:
	GameLevelDemoLayerController();
	~GameLevelDemoLayerController();

	virtual void initial();
	virtual void enter();
	virtual void exit();
	
	void enter(const Point&maskPoint);


private:
	cocos2d::Sprite * createMaskedSprite(const char* srcFile, const char* maskFile, const Point&maskPos) ;
	void playArrowEffect();
	void stopArrowEffect();

private:
	cocos2d::EventListenerTouchOneByOne *m_eventLis;
	cocos2d::Sprite *m_mask;
	cocos2d::Point m_maskPoint;
	cocos2d::Sprite *m_arrow;

};

#endif