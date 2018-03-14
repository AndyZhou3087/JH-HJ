#include "SelectHeroScene.h"
#include "GlobalData.h"
#include "Const.h"
#include "HintBox.h"
#include "CommonFuncs.h"
#include "GameDataSave.h"
#include "BuyDetailsLayer.h"
#include "SoundManager.h"
#include "StoryScene.h"
#include "GameScene.h"
#include "StartScene.h"
#include "ComfirmSaveLayer.h"
#include "GameDataSave.h"

USING_NS_CC;

SelectHeroScene* g_SelectHeroScene = NULL;
SelectHeroScene::SelectHeroScene()
{
	_lastSelect = 1;
	isMoving = false;
}
SelectHeroScene::~SelectHeroScene()
{
	g_SelectHeroScene = NULL;
}

Scene* SelectHeroScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
	g_SelectHeroScene = SelectHeroScene::create();

    // add layer as a child to scene
	scene->addChild(g_SelectHeroScene);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool SelectHeroScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    /*Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	GlobalData::loadUnlockHeroData();

	Node* csbnode = CSLoader::createNode("selectHeroLayer.csb");
	this->addChild(csbnode);

	cocos2d::ui::Widget* startbtn = (cocos2d::ui::Widget*)csbnode->getChildByName("startbtn");
	startbtn->addTouchEventListener(CC_CALLBACK_2(SelectHeroScene::onStart, this));

	cocos2d::ui::Widget* backbtn = (cocos2d::ui::Widget*)csbnode->getChildByName("backbtn");
	backbtn->addTouchEventListener(CC_CALLBACK_2(SelectHeroScene::onBack, this));

	for (int i = 0; i < 4; i++)
	{
		std::string str = StringUtils::format("heroimg%d", i + 1);
		heroimg[i] = (cocos2d::ui::ImageView*)csbnode->getChildByName(str);
		heroimg[i]->setTag(i + 1);
		heroimg[i]->addTouchEventListener(CC_CALLBACK_2(SelectHeroScene::onSelect, this));
		heroimg[i]->setLocalZOrder(i);

		str = StringUtils::format("select_%d", i + 1);
		selectimg[i] = (cocos2d::ui::ImageView*)heroimg[i]->getChildByName(str);
		selectimg[i]->setPositionX(heroimg[i]->getContentSize().width / 2);
		heronameimg[i] = (cocos2d::ui::Widget*)selectimg[i]->getChildByName("sheroname");
		heronameimg[i]->setPositionX(selectimg[i]->getContentSize().width / 2);

		str = StringUtils::format("lock_%d", i + 1);
		lock[i] = (cocos2d::ui::ImageView*)heroimg[i]->getChildByName(str);
		lock[i]->setVisible(!GlobalData::getUnlocHero(i));
	}
	int defaultindex = 0;
	std::string str = StringUtils::format("images/shero%d.jpg", defaultindex + 1);
	heroimg[defaultindex]->loadTexture(str, cocos2d::ui::TextureResType::LOCAL);
	heroimg[defaultindex]->setContentSize(Sprite::create(str)->getContentSize());

	str = StringUtils::format("images/select.png");
	selectimg[defaultindex]->loadTexture(str, cocos2d::ui::TextureResType::LOCAL);
	selectimg[defaultindex]->setContentSize(Sprite::create(str)->getContentSize());
	selectimg[defaultindex]->getChildByName("sheroname")->setPositionX(selectimg[defaultindex]->getContentSize().width / 2);*/


	//首先创建scrollView  
	auto scroll_layer = Layer::create();//创建scrollView中的容器层  
	scroll_layer->setPosition(Point::ZERO);
	scroll_layer->setAnchorPoint(Point::ZERO);
	scroll_layer->setContentSize(Size(1750, 1280));//设置容器层大小为（600，300）  

	auto scrollView = ScrollView::create(Size(750, 1280), scroll_layer);//创建scrollView，显示窗口大小为(400,300)  
	scrollView->setDelegate(this);//设置委托  
	scrollView->setDirection(ScrollView::Direction::HORIZONTAL);//设置滚动方向为水平  
	scrollView->setPosition(Point(0, 0));
	this->addChild(scrollView, 2);

	for (int i = 0; i < 8; i++)
	{
		//auto str = StringUtils::format("images/shero%d.jpg", i + 1);
		auto boy = Sprite::create("images/shero1.jpg");//没错，主角又是我们熟悉的那仨。多么温馨。  
		boy->setPosition(Point( i*300, 640));
		scroll_layer->addChild(boy, 2);
		sp_vec.pushBack(boy);
	}

	//创建三个对象  
	//auto boy = Sprite::create("images/shero1.jpg");//没错，主角又是我们熟悉的那仨。多么温馨。  
	//boy->setPosition(Point(0, 100));
	//scroll_layer->addChild(boy, 2);

	//auto girl = Sprite::create("images/shero2.jpg");
	//girl->setPosition(Point(300, 100));
	//scroll_layer->addChild(girl, 2);

	//auto girl3 = Sprite::create("images/shero3.jpg");
	//girl3->setPosition(Point(600, 100));
	//scroll_layer->addChild(girl3, 2);

	//sp_vec.pushBack(boy);//将三个对象放入容器中  
	//sp_vec.pushBack(girl);
	//sp_vec.pushBack(girl3);

    return true;
}

void SelectHeroScene::onExit()
{
	Layer::onExit();
}

void SelectHeroScene::onSelect(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		SoundManager::getInstance()->playSound(SoundManager::SOUND_ID_BUTTON);
		Node* btnnode = (Node*)pSender;
		int tag = btnnode->getTag();

		if (!GlobalData::getUnlocHero(tag - 1))
		{
			Layer* layer = BuyDetailsLayer::create(tag);
			layer->setTag(tag - 1);
			this->addChild(layer, 0, "buyherolayer");
			return;
		}
		if (_lastSelect == tag || isMoving)
			return;

		isMoving = true;
		std::string str = StringUtils::format("images/shero%d.jpg", tag);
		heroimg[tag - 1]->loadTexture(str, cocos2d::ui::TextureResType::LOCAL);
		heroimg[tag - 1]->setContentSize(Sprite::create(str)->getContentSize());

		str = StringUtils::format("images/select.png");
		selectimg[tag - 1]->loadTexture(str, cocos2d::ui::TextureResType::LOCAL);
		selectimg[tag - 1]->setContentSize(Sprite::create(str)->getContentSize());

		selectimg[tag - 1]->setPositionX(heroimg[tag - 1]->getContentSize().width / 2);

		heronameimg[tag - 1]->setPositionX(selectimg[tag - 1]->getContentSize().width / 2);

		str = StringUtils::format("images/shero%d_s.jpg", _lastSelect);
		heroimg[_lastSelect - 1]->loadTexture(str, cocos2d::ui::TextureResType::LOCAL);
		heroimg[_lastSelect - 1]->setContentSize(Sprite::create(str)->getContentSize());
		heroimg[_lastSelect - 1]->setLocalZOrder(_lastSelect - 1);

		str = StringUtils::format("images/unselect.png");
		selectimg[_lastSelect - 1]->loadTexture(str, cocos2d::ui::TextureResType::LOCAL);
		selectimg[_lastSelect - 1]->setContentSize(Sprite::create(str)->getContentSize());
		selectimg[_lastSelect - 1]->setPositionX(heroimg[_lastSelect - 1]->getContentSize().width / 2);
		heronameimg[_lastSelect - 1]->setPositionX(selectimg[_lastSelect - 1]->getContentSize().width / 2);
		
		int headsize = sizeof(heroimg) / sizeof(heroimg[0]);

		int leftperwidth = (720 - selectimg[tag - 1]->getContentSize().width) /(headsize - 1);
		for (int i = 0; i < headsize; i++)
		{
			int movex = 0;
			if (i < tag - 1)
			{
				movex = leftperwidth / 2 + i*leftperwidth;
			}
			else if (i == tag - 1)
			{
				movex = (tag - 1) * leftperwidth + heroimg[tag - 1]->getContentSize().width / 2;
			}
			else
			{
				movex = (tag - 1)* leftperwidth + heroimg[tag - 1]->getContentSize().width + leftperwidth / 2 + (i - tag) * leftperwidth;
			}
			heroimg[i]->runAction(MoveTo::create(0.1f, Vec2(movex, heroimg[i]->getPositionY())));
			
		}
		this->scheduleOnce(schedule_selector(SelectHeroScene::clickMoveFinish), 0.1f);
		_lastSelect = tag;
	}
}

void SelectHeroScene::onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		Director::getInstance()->replaceScene(StartScene::createScene());
	}
}

void SelectHeroScene::onStart(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	CommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		std::string uid = GlobalData::getUId();
		if (uid.size() <= 0)
		{
			enterNextScene();
			return;
		}

		bool isExit = false;
		std::vector<std::string> vec_ids = GlobalData::getSaveListId();
		
		for (unsigned int i = 0; i < vec_ids.size(); i++)
		{
			std::string saveuid = vec_ids[i];
			if (saveuid.length() > 0)
			{
				int heroid = GameDataSave::getInstance()->getHeroIdByUid(saveuid);
				if (heroid == _lastSelect)
				{
					isExit = true;
					break;
				}
			}
		}
		if (isExit)
		{
			this->addChild(ComfirmSaveLayer::create());
		}
		else
		{
			enterNextScene();
		}
	}
}

void SelectHeroScene::enterNextScene()
{
	int systime = GlobalData::getSysSecTime();
	std::string uidstr = StringUtils::format("%d", systime);
	GlobalData::setUId(uidstr);
	GameDataSave::getInstance()->setHeroId(_lastSelect);

	GlobalData::setCurHeroIdToSaveList();

	std::string defaultStorageStr = GlobalData::getDefaultStorage(_lastSelect);
	GameDataSave::getInstance()->setStorageData(defaultStorageStr);

	Scene* scene = StoryScene::createScene(0);

	Director::getInstance()->replaceScene(scene);
}

void SelectHeroScene::unlockSucc(int index)
{
	this->removeChildByName("buyherolayer");
	lock[index]->setVisible(false);
	GlobalData::setUnlockHero(index, true);
}

void SelectHeroScene::clickMoveFinish(float dt)
{
	isMoving = false;
}

void SelectHeroScene::scrollViewDidScroll(ScrollView* view)
{
	//在scrollView拖动时响应该函数  

	auto offsetPosX = (view->getContentOffset()).x;//获得偏移X坐标(向右移动，偏移量为正数，向左则为负数）  
	//  CCLOG("offset pos is %f , %f",offsetPos.x,offsetPos.y);  

	//for 循环遍历容器中的每个精灵  
	for (auto e : sp_vec)
	{
		auto pointX = e->getPositionX();//获得当前对象的X坐标（不管怎么滚动，这个坐标都是不变的）  
		float endPosX = pointX + offsetPosX;//将精灵的 X坐标 + 偏移X坐标  

		//当endPosX在 150~250 范围，则对象的大小从左向右递增  
		if (endPosX > 150 && endPosX < 250)
		{
			float x = endPosX / 150;//放大倍数为 endPosX / 150;  
			e->setScale(x);
			CCLOG("x = %f", x);
		}
		//当endPosX在 250~350 范围，则对象的大小从左向右递减  
		else if (endPosX > 250 && endPosX < 350)
		{
			//下面这个公式不好解释，我就这么说吧：  
			//假设 endPosX = 200，那么放大倍数应该是 200 / 150 = 1.33左右，那么当endPosX = 300时，出于对称的原理，我们以250为对称中心，那么  
			//300 的放大倍数也应该是 1.33。这就是下面的公式由来  
			float a = endPosX - 250;
			float b = 250 - a;

			float x = b / 150;
			e->setScale(x);
		}
		else
		{
			//不是在上面的范围，则设置为正常大小  
			e->setScale(1.0f);
		}
	}
}

void SelectHeroScene::scrollViewDidZoom(ScrollView* view)
{
	//do something  
}
void SelectHeroScene::scrollViewMoveOver(ScrollView* view)
{
	//do something  
}