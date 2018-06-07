#ifndef __game_unlock_layer_controller_h__
#define __game_unlock_layer_controller_h__

#include "LayerController.h"
#include "prefix.h"

class GameUnlockLayerController:public LayerController {
public:
	GameUnlockLayerController();
	~GameUnlockLayerController();

	virtual void initial();
	virtual void enter();
	virtual void exit();

private:
	TextAtlas *m_unlockStar;
};
#endif