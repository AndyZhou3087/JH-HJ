#ifndef __game_pause_layer_controller_h__
#define __game_pause_layer_controller_h__

#include "LayerController.h"
#include "prefix.h"

class URookieLayerController;
class GameIntroLayerController;
class GameLevelLayerController;
class GameEnergyTipLayerController;

class GamePauseLayerController:public LayerController {
public:
	GamePauseLayerController();
	~GamePauseLayerController();

	virtual void initial();
	virtual void enter();
	virtual void exit();

	void regGiftLayer(URookieLayerController *l) { m_giftLayer = l; }
	void regIntroLayer(GameIntroLayerController *l) { m_introLayer = l; }
	void regLevelLayer(GameLevelLayerController *l) { m_levelLayer = l; }
	void regEnergyTipLayer(GameEnergyTipLayerController *l) { m_energyTipLayer = l; }


private:
	Button *m_sound_on;
	Button *m_sound_off;

	URookieLayerController *m_giftLayer;
	GameIntroLayerController *m_introLayer;
	GameLevelLayerController *m_levelLayer;
	GameEnergyTipLayerController *m_energyTipLayer;
};

#endif