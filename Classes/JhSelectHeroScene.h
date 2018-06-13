#ifndef __SELECTHERO_SCENE_H__
#define __SELECTHERO_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "../extensions/cocos-ext.h"
USING_NS_CC;
USING_NS_CC_EXT;
class JhSelectHeroScene : public cocos2d::Layer
{
public:
	JhSelectHeroScene();
	~JhSelectHeroScene();
    static cocos2d::Scene* createScene();

	virtual void onExit();
    virtual bool init();
    
	CREATE_FUNC(JhSelectHeroScene);
	void unlockSucc(int index);
	void enterNextScene();

private:
	cocos2d::ui::ImageView* heroimg[4];
	cocos2d::ui::ImageView* selectimg[4];
	cocos2d::ui::Widget* heronameimg[4];
	cocos2d::ui::Widget* lock[4];
	int _lastSelect;
	bool isMoving;
	Vector<Sprite*> sp_vec;//声明一个容器 

	cocos2d::ui::Text* m_heroname;
	cocos2d::ui::ImageView* herosign;
	cocos2d::ui::ImageView* herodesc;

	cocos2d::ui::ImageView* sheros;
	cocos2d::ui::ImageView* sheros_0;
private:
	void onSelect(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onStart(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);
	void onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type);

	void JumpSceneCallback(cocos2d::Ref* pScene, cocos2d::ui::PageView::EventType type);

	void clickMoveFinish(float dt);

};
extern JhSelectHeroScene* g_SelectHeroScene;
#endif
