#ifndef _SYSSMALL_BOX_H_
#define _SYSSMALL_BOX_H_
#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
USING_NS_CC;

typedef enum
{
	REASON,
	WEATHER,
	LIVEDAYS,
	TIME,
	TEMPERATURE,
	INNERINJURY,
	OUTERINJURY,
	HUNGER,
	SPIRIT,
	LIFE

}BoxType;
class JhSysSmallBox :public Layer
{
public:
	JhSysSmallBox();
	~JhSysSmallBox();

	virtual bool init(BoxType type, std::string imagepath, std::string title, std::string title1, std::string text);
	static JhSysSmallBox* create(BoxType type, std::string imagepath, std::string title, std::string title1, std::string text);
private:
	cocos2d::ui::ImageView* image;
	cocos2d::ui::Text* titleTxt;
	cocos2d::ui::Text* title1Txt;
	Label* textTxt;
	cocos2d::ui::Text* valueTxt;
	BoxType mType;

private:
	void updataUI(float dt);
	int lastvalue;
};
#endif

