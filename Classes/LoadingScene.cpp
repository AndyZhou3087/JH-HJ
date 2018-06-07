#include "LoadingScene.h"
#include "global.h"
#include "ResourceUtil.h"
#include "HomeScene.h"
#include "Resource.h"
#include "Audio.h"
#include "GameUtil.h"
#include "Analytics.h"
#include "GameData.h"
#include "EnergyManager.h"
#include "SettingManager.h"

USING_NS_CC;

LoadingScene::LoadingScene():
m_pot1(NULL),
m_pot2(NULL),
m_pot3(NULL),
m_timeCount(0),
m_loadFinished(false),
m_initPhrase(0)
{
}
Scene* LoadingScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = LoadingScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool LoadingScene::init()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    if ( !CCLayerColor::initWithColor(Color4B::BLACK) )
    {
        return false;
    }
    
    //Sprite * logo = Sprite::create(s_unit_logo);
    //logo->setAnchorPoint(Vec2(0.5,0.5));
   // logo->setPosition(Vec2(WIN_WIDTH * 0.5, WIN_HEIGHT * 0.5));
   //addChild(logo);
	const auto size = Director::getInstance()->getVisibleSize();
	auto loading = Sprite::create(s_icon_loading);
	addChild(loading);
	loading->setPosition(size.width * 0.5 - 20, size.height * 0.5);

	m_pot1 = Sprite::create(s_icon_loading_pot);
	loading->addChild(m_pot1);
	m_pot1->setPosition(loading->getContentSize().width + 20, loading->getContentSize().height * 0.45);

	m_pot2 = Sprite::create(s_icon_loading_pot);
	loading->addChild(m_pot2);
	m_pot2->setPosition(m_pot1->getPositionX() + 20, loading->getContentSize().height * 0.45);

	m_pot3 = Sprite::create(s_icon_loading_pot);
	loading->addChild(m_pot3);
	m_pot3->setPosition(m_pot2->getPositionX() + 20, loading->getContentSize().height * 0.45);

	auto animal = Sprite::create("public/cat2.png");
	addChild(animal);
	animal->setAnchorPoint(Vec2(0.5,0));
	animal->setPosition(size.width * 0.5, loading->getPositionY() + 50);

#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    if ( !CCLayerColor::init() )
    {
        return false;
    }
    
#endif
	m_loadFinished = false;
	m_initPhrase = 0;
	schedule(schedule_selector(LoadingScene::onTimer), 0.5);
	schedule(schedule_selector(LoadingScene::onInitGame), 0.1);
    return true;
}

void LoadingScene::updatePot(int n) {
	if (nullptr != m_pot1) {
		m_pot1->setVisible(n>0);
	}
	if (nullptr != m_pot2) {
		m_pot2->setVisible(n>1);
	}
	if (nullptr != m_pot3) {
		m_pot3->setVisible(n>2);
	}
}
void LoadingScene::onTimer(float dt) {
	if (m_timeCount>3) {
		m_timeCount = 0;
	}
	updatePot(m_timeCount);
	m_timeCount++;
}
void LoadingScene::onInitGame(float dt) {
	if (!m_loadFinished && initGame()) {
		m_loadFinished = true;
		Director::sharedDirector()->replaceScene(HomeScene::createScene());
	}
}
bool LoadingScene::initGame() {
	if (0 == m_initPhrase) {
	GameUtil::initRandSeed();
	ResourceUtil::initLocal();
		m_initPhrase++;
		return false;
	}
	//加载精灵框帧缓存
	if (1 == m_initPhrase) {
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile(s_plist_all_image_public);
		m_initPhrase++;
		return false;
	}
	if (2 == m_initPhrase) {
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile(s_plist_ani_change);
		m_initPhrase++;
		return false;
	}
	if (3 == m_initPhrase) {
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile(s_plist_ani_ice);
		m_initPhrase++;
		return false;
	}
	if (4 == m_initPhrase) {
		TextureCache::getInstance()->addImage(s_ani_star_bomb);
		m_initPhrase++;
		return false;
	}

	//加载音效
	if (5 == m_initPhrase) {
	Audio::getInstance()->preLoad();
		m_initPhrase++;
		return false;
	}
	if (6 == m_initPhrase) {
	Analytics::setAccount();
		m_initPhrase++;
		return false;
	}
	//数据初始化
	if (7 == m_initPhrase) {
	GameData::getInstance();
		m_initPhrase++;
		return false;
	}
	if (8 == m_initPhrase) {
	EnergyManager::getInstance();
	SettingManager::getInstance();
		m_initPhrase++;
		return false;
	}
	return true;
}