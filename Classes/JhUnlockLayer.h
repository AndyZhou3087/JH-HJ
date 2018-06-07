/********************************************************************
* 章节解锁 UI界面
*********************************************************************/
#ifndef _UNLOCK_LAYER_H_
#define _UNLOCK_LAYER_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
USING_NS_CC;

class JhUnlockLayer :public Layer
{
public:
	JhUnlockLayer();
	~JhUnlockLayer();

	virtual bool init();
	static JhUnlockLayer* create();
private:
	void showText(float dt);
	void removeself(float dt);

private:
	cocos2d::ui::Widget* m_unlockbg;
	cocos2d::ui::Widget* m_title;
	cocos2d::ui::Text* m_desc;
	cocos2d::ui::Text* m_addrname;
};
#endif

