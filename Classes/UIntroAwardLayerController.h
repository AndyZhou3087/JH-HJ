#ifndef __ui_introaward_layer_controller_h__
#define __ui_introaward_layer_controller_h__

#include "cocos2d.h"
#include "LayerController.h"
#include "prefix.h"

USING_NS_CC;

class GameLevelLayerController;

class UIntroAwardLayerController :public CenterLayerController{
public:
	UIntroAwardLayerController();
	~UIntroAwardLayerController();

	virtual void initial();

	void regLevelLayer(GameLevelLayerController *l) { m_levelLayer = l;}

private:
	GameLevelLayerController *m_levelLayer;
};

#endif