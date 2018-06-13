/********************************************************************
* 功法组合结算界面
*********************************************************************/
#ifndef _MIXGFDONE_LAYER_H_
#define _MIXGFDONE_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
USING_NS_CC;

class JhMixGFDoneLayer :public Layer
{
public:
	JhMixGFDoneLayer();
	~JhMixGFDoneLayer();

	virtual bool init(int ret);
	static JhMixGFDoneLayer* create(int ret);
private:
	void removeself(float dt);
private:

};
#endif

