#ifndef __loadgame_layer_controller_h__
#define __loadgame_layer_controller_h__

#include "cocos2d.h"
#include "LayerController.h"
#include "prefix.h"

USING_NS_CC;

class ULoadGameLayerController :public CenterLayerController{
public:
	ULoadGameLayerController();
	~ULoadGameLayerController();

	virtual void initial();
	virtual void enter() override;
	virtual void exit() override;
	void initPet();
	void updatePot(int n);

private:
	cocos2d::Sprite *m_loading;
	cocos2d::Sprite *m_pot1;
	cocos2d::Sprite *m_pot2;
	cocos2d::Sprite *m_pot3;

	int m_timeCount;
};

#endif