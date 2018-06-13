#ifndef __Load_game_SCENE_H__
#define __Load_game_SCENE_H__


#include "cocos2d.h"
class ULoadGameLayerController;

class LoadGameScene : public cocos2d::LayerColor
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    
    // implement the "static create()" method manually
    CREATE_FUNC(LoadGameScene);

	LoadGameScene();
	~LoadGameScene();

	virtual void onEnter() override;
	virtual void onExit() override;

	void onTimer(float dt);
	void onLoadUi(float dt);

private:

	ULoadGameLayerController * m_loadLayerController;
	int m_timeCount;
	bool m_loadUiFinished;

};

#endif // __HELLOWORLD_SCENE_H__
