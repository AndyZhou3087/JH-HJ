#ifndef __Loading_SCENE_H__
#define __Loading_SCENE_H__

#include "cocos2d.h"

class LoadingScene : public cocos2d::LayerColor
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  
    
    // implement the "static create()" method manually
    CREATE_FUNC(LoadingScene);

private:
	LoadingScene();
	void onTimer(float dt);
	void onInitGame(float dt);
	bool initGame();
	void updatePot(int n);
private:
	cocos2d::Sprite *m_pot1;
	cocos2d::Sprite *m_pot2;
	cocos2d::Sprite *m_pot3;
	int m_timeCount;
	bool m_loadFinished;
	int m_initPhrase;
};

#endif // __HELLOWORLD_SCENE_H__
