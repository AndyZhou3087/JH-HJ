
#ifndef __BUILDING_DETAILS_LAYER__
#define __BUILDING_DETAILS_LAYER__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "JhBuilding.h"
USING_NS_CC;

class JhBuildingDetailsLayer : public Layer
{
public:
	JhBuildingDetailsLayer();
	~JhBuildingDetailsLayer();
	bool init(JhBuilding* pdata);
	static JhBuildingDetailsLayer* create(JhBuilding* pdata);
	void removSelf();

private:
	void onOk(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
private:
};

#endif
