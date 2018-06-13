
#ifndef __BOOK_DETAILS_LAYER__
#define __BOOK_DETAILS_LAYER__

#include "cocos2d.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "JhBookShelfLayer.h"
USING_NS_CC;

class JhBookDetailsLayer : public Layer
{
public:
	JhBookDetailsLayer();
	~JhBookDetailsLayer();
	bool init(BookData* bookdata);
	static JhBookDetailsLayer* create(BookData* bookdata);
	void removSelf();

private:
	void onOk(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);


private:
	cocos2d::ui::Button* m_okbtn;
	Node* m_csbnode;
};

#endif
