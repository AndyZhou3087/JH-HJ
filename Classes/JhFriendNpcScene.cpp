#include "JhFriendNpcScene.h"
#include "JhCommonFuncs.h"
#include "JhConst.h"
#include "JhGlobalData.h"
#include "JhSoundManager.h"
#include "JhGameScene.h"
#include "JhFightLayer.h"

JhFriendNpcScene::JhFriendNpcScene()
{

}

JhFriendNpcScene::~JhFriendNpcScene()
{
	JhGlobalData::isPopingScene = false;
}

Scene* JhFriendNpcScene::createScene()
{
	if (JhGlobalData::isPopingScene)
		return NULL;
	JhGlobalData::isPopingScene = true;
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	Layer* layer = JhFriendNpcScene::create();

	if (layer == NULL)
		return NULL;
	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

JhFriendNpcScene* JhFriendNpcScene::create()
{
	JhFriendNpcScene *pRet = new JhFriendNpcScene();
	if (pRet && pRet->init())
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

bool JhFriendNpcScene::init()
{
	std::vector<std::string> vec_friends;
	std::map<std::string, FriendlyData>::iterator mit;
	for (mit = JhGlobalData::map_myfriendly.begin(); mit != JhGlobalData::map_myfriendly.end(); ++mit)
	{
		if (JhGlobalData::map_myfriendly[mit->first].relation == F_FRIEND)
		{
			if (JhGlobalData::map_NPCFriendData[mit->first].vec_fightnpc.size() > 0)
			vec_friends.push_back(mit->first);
		}
	}

	if (vec_friends.size() <= 0)
		return false;

	Node* csbnode = CSLoader::createNode("jhfriendNpcChallengeLayer.csb");
	this->addChild(csbnode);

	int rnd = JhGlobalData::createRandomNum(vec_friends.size());
	std::string npcid = vec_friends[rnd];

	vec_fightnpc = JhGlobalData::map_NPCFriendData[npcid].vec_fightnpc;
	std::string enemynpcname;

	JhGlobalData::map_maps["m1-6"].npcs.clear();
	for (unsigned int i = 0; i < vec_fightnpc.size(); i++)
	{
		enemynpcname.append(JhGlobalData::map_npcs[vec_fightnpc[i]].name);
		enemynpcname.append(JhCommonFuncs::gbk2utf("、"));
		JhGlobalData::map_maps["m1-6"].npcs.push_back(vec_fightnpc[i]);
	}
	enemynpcname = enemynpcname.substr(0, enemynpcname.length() - 3);

	//描述
	cocos2d::ui::Text* desc = (cocos2d::ui::Text*)csbnode->getChildByName("desc");

	desc1 = (cocos2d::ui::Text*)csbnode->getChildByName("desc1");
	desc1->setVisible(false);

	std::string descstr = StringUtils::format("%s%s%s%s%s", JhCommonFuncs::gbk2utf("你与").c_str(), JhGlobalData::map_npcs[npcid].name, JhCommonFuncs::gbk2utf("成为好友，").c_str(), enemynpcname.c_str(), JhCommonFuncs::gbk2utf("向你发起挑战！").c_str());
	desc->setString(descstr);

	cocos2d::ui::ImageView* typepng = (cocos2d::ui::ImageView*)csbnode->getChildByName("image");
	typepng->loadTexture("images/cchallenge.jpg", cocos2d::ui::Widget::TextureResType::LOCAL);
	
	escapebtn = (cocos2d::ui::Button*)csbnode->getChildByName("escapebtn");
	escapebtn->addTouchEventListener(CC_CALLBACK_2(JhFriendNpcScene::onEscape, this));

	getbtn = (cocos2d::ui::Button*)csbnode->getChildByName("getbtn");
	getbtn->addTouchEventListener(CC_CALLBACK_2(JhFriendNpcScene::onGet, this));


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


void JhFriendNpcScene::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();
}

void JhFriendNpcScene::onEscape(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		int r = JhGlobalData::createRandomNum(100);
		if (r < 50)
		{
			desc1->setVisible(true);
			escapebtn->setEnabled(false);
			getbtn->setEnabled(false);
			this->scheduleOnce(schedule_selector(JhFriendNpcScene::delayFight), 1.5f);
		}
		else
		{
			Director::getInstance()->popScene();
		}
	}
}

void JhFriendNpcScene::onGet(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		escapebtn->setEnabled(false);
		getbtn->setEnabled(false);
		this->scheduleOnce(schedule_selector(JhFriendNpcScene::delayFight), 0.5f);
	}
}

void JhFriendNpcScene::delayFight(float dt)
{
	Director::getInstance()->popScene();

	if (g_gameLayer != NULL)
	{
		g_gameLayer->addChild(JhFightLayer::create("m1-6", vec_fightnpc[0]), 4, "fightlayer");
	}
}