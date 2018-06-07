#ifndef _game_ui_manager_h_
#define _game_ui_manager_h_

#include "cocos2d.h"
USING_NS_CC;

#define REG_LAYER_CONTROLLER(layer,layerClass) \
public:\
	void reg##layer##LayerController(layerClass *l) { m_##layer##LayerController = l; } \
	layerClass *get##layer##LayerController() { return m_##layer##LayerController; } \
private: \
	layerClass * m_##layer##LayerController;

#define REG_LAYER(layer) \
public:\
	void reg##layer(Node *l) { m_##layer##Layer = l; showLayer(l,false); } \
	Node *get##layer##Layer() { return m_##layer##Layer; } \
private: \
	Node * m_##layer##Layer;

class GameTargetLayerController;
class GameTargetBossLayerController;
class GameBossLayerController;
class GameTopLayerController;
class GreyLayerController;

class UTipWinLayerController;
class UToolConfirmLayerController;
class UGiftMoreLayerController;
class UGuideTipLayerController;

enum EnumShow {
	Show_Bottom,
	Show_Center
};

class GameUiManager {
public:
	~GameUiManager();
	static GameUiManager* getInstance();
	void init();

	void unregAllUiController();
	void showLayer(Node * layer, bool visilbe, EnumShow show = Show_Center);

	REG_LAYER_CONTROLLER(Target,		GameTargetLayerController)
	REG_LAYER_CONTROLLER(Boss,			GameBossLayerController)
	REG_LAYER_CONTROLLER(TipWin,		UTipWinLayerController)
	REG_LAYER_CONTROLLER(ToolConfirm,	UToolConfirmLayerController)
	REG_LAYER_CONTROLLER(Top,			GameTopLayerController)
	REG_LAYER_CONTROLLER(BgGrey,		GreyLayerController)
	REG_LAYER_CONTROLLER(TargetBoss,	GameTargetBossLayerController)
	REG_LAYER_CONTROLLER(GiftMore,		UGiftMoreLayerController)
	REG_LAYER_CONTROLLER(TipGuide,		UGuideTipLayerController)
	REG_LAYER(StepWelcome)
	REG_LAYER(WelcomeEnd)
	REG_LAYER(CrossGuide)
	REG_LAYER(BombGuide)
	REG_LAYER(TimeGuide)
	REG_LAYER(ToolConfirm)
	REG_LAYER(SpecailPet)
	REG_LAYER(GirlTalk)
	REG_LAYER(BossTalk)
	REG_LAYER(BossTip)
	REG_LAYER(TouchErr)


private:
	void showLayerBottom(Node *layer);
	void showLayerCenter(Node *layer);

private:
	GameUiManager();

private:
	static GameUiManager* m_instance;
};
#endif