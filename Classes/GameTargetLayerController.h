#ifndef __game_target_layer_controller_h__
#define __game_target_layer_controller_h__

#include "LayerController.h"
#include "prefix.h"

class GameTargetLayerController:public LayerController {
public:
	GameTargetLayerController();
	~GameTargetLayerController();

	virtual void initial();
	virtual void enter();
	virtual void exit();

protected:
	virtual void updateInfo();
	void enterBar();
	void showReady();
	void showGo();

protected:
	TextAtlas *m_target;
	TextAtlas *m_time;
	TextAtlas *m_levelNum;
	Node *m_bar;
	Sprite *m_ready;
	Sprite *m_go;

	float m_timeOriginPosX;
};

/////////////////////////////////////////////////////////////////
class GameTargetBossLayerController:public GameTargetLayerController {
public:
	GameTargetBossLayerController();
	~GameTargetBossLayerController();

	virtual void initial();

protected:
	virtual void updateInfo() override {}
};
#endif