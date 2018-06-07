#ifndef __ui_award_layer_controller_h__
#define __ui_award_layer_controller_h__

#include "cocos2d.h"
#include "LayerController.h"
#include "prefix.h"

USING_NS_CC;



class UAwardLayerController :public CenterLayerController{
	enum EnumAwardType {
		Award_Energy,
		Award_Time,
		Award_Undefine,
	};
public:
	UAwardLayerController();
	~UAwardLayerController();

	virtual void initial();

	void enterEnergy();
	void enterTime();

private:
	cocos2d::Sprite *m_iconAward;
	EnumAwardType m_awardType;
};

#endif