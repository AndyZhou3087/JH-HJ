#include "JhSelectHeroScene.h"
#include "JhGlobalData.h"
#include "JhConst.h"
#include "JhHintBox.h"
#include "JhCommonFuncs.h"
#include "JhGameDataSave.h"
#include "JhBuyDetailsLayer.h"
#include "JhSoundManager.h"
#include "JhStoryScene.h"
#include "JhGameScene.h"
#include "JhStartScene.h"
#include "JhComfirmSaveLayer.h"
#include "JhGameDataSave.h"

USING_NS_CC;
using namespace ui;

JhSelectHeroScene* g_SelectHeroScene = NULL;
JhSelectHeroScene::JhSelectHeroScene()
{
	_lastSelect = 1;
	isMoving = false;
}
JhSelectHeroScene::~JhSelectHeroScene()
{
	g_SelectHeroScene = NULL;
}

Scene* JhSelectHeroScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
	g_SelectHeroScene = JhSelectHeroScene::create();

    // add layer as a child to scene
	scene->addChild(g_SelectHeroScene);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool JhSelectHeroScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	JhGlobalData::loadUnlockHeroData();

	Node* csbnode = CSLoader::createNode("jhselectHeroLayer.csb");
	this->addChild(csbnode);

	cocos2d::ui::Widget* startbtn = (cocos2d::ui::Widget*)csbnode->getChildByName("startbtn");
	startbtn->addTouchEventListener(CC_CALLBACK_2(JhSelectHeroScene::onStart, this));

	cocos2d::ui::Widget* backbtn = (cocos2d::ui::Widget*)csbnode->getChildByName("backbtn");
	backbtn->addTouchEventListener(CC_CALLBACK_2(JhSelectHeroScene::onBack, this));

	cocos2d::ui::PageView* m_pageview = (cocos2d::ui::PageView*)csbnode->getChildByName("PageView");
	m_pageview->addEventListener((ui::PageView::ccPageViewCallback)CC_CALLBACK_2(JhSelectHeroScene::JumpSceneCallback, this));

	sheros = (cocos2d::ui::ImageView*)csbnode->getChildByName("sheros");
	sheros_0 = (cocos2d::ui::ImageView*)csbnode->getChildByName("sheros_0");

	int defaultindex = m_pageview->getCurPageIndex();
	sheros->loadTexture("images/shero4_s.png", cocos2d::ui::TextureResType::LOCAL);
	sheros_0->loadTexture("images/shero2_s.png", cocos2d::ui::TextureResType::LOCAL);

	m_heroname = (cocos2d::ui::Text*)csbnode->getChildByName("heroname");
	m_heroname->setString(JhCommonFuncs::gbk2utf(heroname[defaultindex].c_str()));

	herosign = (cocos2d::ui::ImageView*)csbnode->getChildByName("herosign");
	std::string str = StringUtils::format("ui/herosign%d.png", defaultindex + 1);
	herosign->loadTexture(str, cocos2d::ui::TextureResType::PLIST);

	herodesc = (cocos2d::ui::ImageView*)csbnode->getChildByName("herodesc");
	str = StringUtils::format("images/herodesc%d.png", defaultindex + 1);
	herodesc->loadTexture(str, cocos2d::ui::TextureResType::LOCAL);

	for (int i = 0; i < 4; i++)
	{
		cocos2d::ui::Layout* panel = (cocos2d::ui::Layout*)m_pageview->getPage(i);
		panel->setTag(i + 1);
		panel->addTouchEventListener(CC_CALLBACK_2(JhSelectHeroScene::onSelect, this));

		str = StringUtils::format("lock_%d", i + 1);
		lock[i] = (cocos2d::ui::ImageView*)panel->getChildByName(str);
		lock[i]->setVisible(!JhGlobalData::getUnlocHero(i));
	}


    return true;
}

void JhSelectHeroScene::onExit()
{
	Layer::onExit();
}

void JhSelectHeroScene::JumpSceneCallback(cocos2d::Ref* pScene, cocos2d::ui::PageView::EventType type)
{
	if (type == ui::PageView::EventType::TURNING)
	{
		PageView * pageView = dynamic_cast<PageView *>(pScene);
		int defaultindex = pageView->getCurPageIndex();
		CCLOG("--------------adadfgggggggggggg -- %d ", defaultindex);
		std::string str = StringUtils::format("images/herodesc%d.png", defaultindex + 1);
		herodesc->loadTexture(str, cocos2d::ui::TextureResType::LOCAL);
		str = StringUtils::format("ui/herosign%d.png", defaultindex + 1);
		herosign->loadTexture(str, cocos2d::ui::TextureResType::PLIST);
		m_heroname->setString(JhCommonFuncs::gbk2utf(heroname[defaultindex].c_str()));

		str = StringUtils::format("images/shero%d_s.png", defaultindex);
		if (defaultindex == 0)
		{
			str = "images/shero4_s.png";
		}
		sheros->loadTexture(str, cocos2d::ui::TextureResType::LOCAL);
		str = StringUtils::format("images/shero%d_s.png", defaultindex + 2);
		if (defaultindex == 3)
		{
			str = "images/shero1_s.png";
		}
		sheros_0->loadTexture(str, cocos2d::ui::TextureResType::LOCAL);
	}
}

void JhSelectHeroScene::onSelect(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		JhSoundManager::getInstance()->playSound(JhSoundManager::SOUND_ID_BUTTON);
		Node* btnnode = (Node*)pSender;
		int tag = btnnode->getTag();

		if (!JhGlobalData::getUnlocHero(tag - 1))
		{
			Layer* layer = JhBuyDetailsLayer::create(tag);
			layer->setTag(tag - 1);
			this->addChild(layer, 0, "buyherolayer");
			return;
		}
		if (_lastSelect == tag || isMoving)
			return;

		this->scheduleOnce(schedule_selector(JhSelectHeroScene::clickMoveFinish), 0.1f);
		_lastSelect = tag;
	}
}

void JhSelectHeroScene::onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		Director::getInstance()->replaceScene(JhStartScene::createScene());
	}
}

void JhSelectHeroScene::onStart(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		std::string uid = JhGlobalData::getUId();
		if (uid.size() <= 0)
		{
			enterNextScene();
			return;
		}

		bool isExit = false;
		std::vector<std::string> vec_ids = JhGlobalData::getSaveListId();
		
		for (unsigned int i = 0; i < vec_ids.size(); i++)
		{
			std::string saveuid = vec_ids[i];
			if (saveuid.length() > 0)
			{
				int heroid = JhGameDataSave::getInstance()->getHeroIdByUid(saveuid);
				if (heroid == _lastSelect)
				{
					isExit = true;
					break;
				}
			}
		}
		if (isExit)
		{
			this->addChild(JhComfirmSaveLayer::create());
		}
		else
		{
			enterNextScene();
		}
	}
}

void JhSelectHeroScene::enterNextScene()
{
	int systime = JhGlobalData::getSysSecTime();
	std::string uidstr = StringUtils::format("%d", systime);
	JhGlobalData::setUId(uidstr);
	JhGameDataSave::getInstance()->setHeroId(_lastSelect);

	JhGlobalData::setCurHeroIdToSaveList();

	std::string defaultStorageStr = JhGlobalData::getDefaultStorage(_lastSelect);
	JhGameDataSave::getInstance()->setStorageData(defaultStorageStr);

	Scene* scene = JhStoryScene::createScene(0);

	Director::getInstance()->replaceScene(scene);
}

void JhSelectHeroScene::unlockSucc(int index)
{
	this->removeChildByName("buyherolayer");
	lock[index]->setVisible(false);
	JhGlobalData::setUnlockHero(index, true);
}

void JhSelectHeroScene::clickMoveFinish(float dt)
{
	isMoving = false;
}
