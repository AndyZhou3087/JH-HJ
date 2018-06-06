
#ifndef __RES_DETAILS_LAYER__
#define __RES_DETAILS_LAYER__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "JhMyPackage.h"
USING_NS_CC;

class JhResDetailsLayer : public Layer
{
public:
	JhResDetailsLayer();
	~JhResDetailsLayer();
	bool init(PackageData* pdata);
	static JhResDetailsLayer* create(PackageData* pdata);
	static JhResDetailsLayer* createByResId(std::string resid);
	void removSelf();

	static int whereClick;
private:
	void onOk(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onUse(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	void onAddOne(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onMinusOne(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	void recoveInjuryValue(int addwvalue, int addnvalue);
	void recoveHungerValue(int addvalue);

	void updateHorseData(int addvalue);

	void updataLeftTime(float dt);
	void updateUI();
private:
	PackageData* m_packageData;
	cocos2d::ui::Text* valuelbl;
	cocos2d::ui::Text* uselbl;
	cocos2d::ui::Text* lefttimelbl;
	cocos2d::ui::Text* selectCountlbl;
	cocos2d::ui::Slider* slider;
	cocos2d::ui::Button* m_okbtn;
	void sliderEvent(Ref * pSender, cocos2d::ui::Slider::EventType type);
	Node* m_csbnode;
	int m_expendtime;
};

#endif
