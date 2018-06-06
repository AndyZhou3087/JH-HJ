#include "JhApprenticeScene.h"
#include "JhCommonFuncs.h"
#include "JhConst.h"
#include "JhGlobalData.h"
#include "SoundManager.h"
#include "StorageRoom.h"
#include "JhHintBox.h"
#include "JhGameDataSave.h"
#include "MD5.h"
#include "JhGameScene.h"
#include "ShopLayer.h"

JhApprenticeScene::JhApprenticeScene()
{

}

JhApprenticeScene::~JhApprenticeScene()
{
	JhGlobalData::isPopingScene = false;
}

Scene* JhApprenticeScene::createScene(int type)
{
	if (JhGlobalData::isPopingScene)
		return NULL;
	JhGlobalData::isPopingScene = true;
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	Layer* layer = JhApprenticeScene::create(type);

	if (layer == NULL)
		return NULL;
	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

JhApprenticeScene* JhApprenticeScene::create(int type)
{
	JhApprenticeScene *pRet = new JhApprenticeScene();
	if (pRet && pRet->init(type))
	{
		pRet->autorelease();
	}
	else
	{
		delete pRet;
		pRet = NULL;
	}
	return pRet;
}

bool JhApprenticeScene::init(int type)
{
	Node* csbnode = CSLoader::createNode("jhapprenticeLayer.csb.csb");
	this->addChild(csbnode);

	m_type = type;

	std::string descstr = "有人慕名远道而来，想向你拜师，是否收他为徒？徒弟会帮你收集后山资源！";
	std::string typeimgstr = "images/apprentice0.jpg";
	std::string closebtntext = "不收";
	std::string getbtntext = "收徒";
	if (type == 1)
	{
		descstr = "你的徒儿为你累死累活收集资源，现在向你讨要生活费来了，是否赏赐生活费？太抠门徒弟可能离你而去！";
		typeimgstr = "images/apprentice1.jpg";
		closebtntext = "不给";
		getbtntext = "赏赐";
	}
	else
	{
		int aday = JhGameDataSave::getInstance()->getApprenticeDay();
		if (aday < 0)
		{
			descstr = "由于你抠门的名声已经传遍江湖，想收徒要先支付生活费！";
			csbnode->getChildByName("gold")->setVisible(true);
			csbnode->getChildByName("goldcount")->setVisible(true);
		}
	}

	//描述
	cocos2d::ui::Text* desc = (cocos2d::ui::Text*)csbnode->getChildByName("desc");
	desc->setString(JhCommonFuncs::gbk2utf(descstr.c_str()));


	cocos2d::ui::ImageView* typepng = (cocos2d::ui::ImageView*)csbnode->getChildByName("image");
	typepng->loadTexture(typeimgstr, cocos2d::ui::Widget::TextureResType::LOCAL);
	
	cocos2d::ui::Button* closebtn = (cocos2d::ui::Button*)csbnode->getChildByName("nobtn");
	closebtn->addTouchEventListener(CC_CALLBACK_2(JhApprenticeScene::onClose, this));
	closebtn->setTitleText(JhCommonFuncs::gbk2utf(closebtntext.c_str()));

	cocos2d::ui::Button* getbtn = (cocos2d::ui::Button*)csbnode->getChildByName("getbtn");
	getbtn->addTouchEventListener(CC_CALLBACK_2(JhApprenticeScene::onGet, this));
	getbtn->setTitleText(JhCommonFuncs::gbk2utf(getbtntext.c_str()));

	if (m_type ==  1)
	{
		std::string boxstr = "ui/buildsmall.png";

		Sprite * box = Sprite::createWithSpriteFrameName(boxstr);
		box->setPosition(Vec2(360, 220));
		this->addChild(box);

		std::string str = "ui/gd0.png";
		Sprite* res = Sprite::createWithSpriteFrameName(str);
		res->setPosition(Vec2(box->getContentSize().width / 2, box->getContentSize().width / 2));
		box->addChild(res);
		
		Label * namelbl = Label::createWithTTF(JhCommonFuncs::gbk2utf("金元宝"), "fonts/STXINGKA.TTF", 25);
		namelbl->setColor(Color3B(255, 255, 255));
		namelbl->setPosition(Vec2(box->getPositionX(), 140));
		this->addChild(namelbl);

		std::string strcount = StringUtils::format("x%d", 20);
		Label * coutlbl = Label::createWithTTF(strcount, "fonts/STXINGKA.TTF", 25);//Label::createWithSystemFont(strcount, "", 25);
		coutlbl->setAnchorPoint(Vec2(1, 0.5f));
		coutlbl->setColor(Color3B(255, 255, 255));
		coutlbl->setPosition(Vec2(box->getPositionX() + 50, 180));
		this->addChild(coutlbl);
	}


	//////layer 点击事件，屏蔽下层事件
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}


void JhApprenticeScene::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();
}

void JhApprenticeScene::onClose(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		if (m_type == 1)
		{
			JhGameDataSave::getInstance()->setApprenticeDay(-1);
			showHint(0);
		}
		Director::getInstance()->popScene();
	}
}

void JhApprenticeScene::onGet(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		if (m_type == 0)
		{
			int curgameday = g_nature->getPastDays();
			int aday = JhGameDataSave::getInstance()->getApprenticeDay();
			if (aday < 0)
			{
				giveGold();
			}
			else
			{
				JhGameDataSave::getInstance()->setApprenticeDay(curgameday);
				Director::getInstance()->popScene();
			}
		}
		else
		{
			giveGold();
		}
	}
}
void JhApprenticeScene::giveGold()
{
	int curgameday = g_nature->getPastDays();
	int mygold = JhGlobalData::getMyGoldCount();

	if (mygold >= 20)
	{
		if (JhGlobalData::getMD5MyGoldCount().compare(md5(mygold)) != 0)
		{
			JhGlobalData::dataIsModified = true;
			JhGlobalData::setMyGoldCount(0);
			JhHintBox* hint = JhHintBox::create(JhCommonFuncs::gbk2utf("发现有作弊行为，金元宝清零作为处罚！！"));
			this->addChild(hint);
			return;
		}
		JhGlobalData::setMyGoldCount(JhGlobalData::getMyGoldCount() - 20);
		JhGameDataSave::getInstance()->setApprenticeDay(curgameday);
		if (m_type == 1)
			showHint(1);

		Director::getInstance()->popScene();
	}
	else
	{
		JhHintBox* hbox = JhHintBox::create(JhCommonFuncs::gbk2utf("金元宝不足！"));
		this->addChild(hbox, 0, "hintbox");
		this->scheduleOnce(schedule_selector(JhApprenticeScene::delayShowShop), 0.8f);
	}
}

void JhApprenticeScene::showHint(int type)
{
	std::string str = "因为你太过小气，徒弟已经离你而去！";
	if (type == 1)
		str = "因为你慷慨大方，徒弟更加任劳任怨！";
	JhHintBox* hbox = JhHintBox::create(JhCommonFuncs::gbk2utf(str.c_str()));
	g_gameLayer->addChild(hbox, 10);
}

void JhApprenticeScene::delayShowShop(float dt)
{
	this->removeChildByName("hintbox");
	this->addChild(ShopLayer::create(), 1);
}