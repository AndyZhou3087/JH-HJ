#include "JhGoWhereLayer.h"
#include "JhCommonFuncs.h"
#include "JhConst.h"
#include "JhGlobalData.h"
#include "JhHomeHill.h"
#include "JhHomeLayer.h"
#include "JhMapLayer.h"
#include "JhGameScene.h"
#include "TempStorageLayer.h"
#include "JhNpcLayer.h"
#include "SoundManager.h"
#include "JhGameDataSave.h"
#include "JhNewerGuideLayer.h"
#include "JhActivitScene.h"
#include "SepcialNpcLayer.h"
#include "JhAnalyticUtil.h"
#include "JhRollDiceLayer.h"
#include "TopBar.h"
JhGoWhereLayer::JhGoWhereLayer()
{

}


JhGoWhereLayer::~JhGoWhereLayer()
{
}

JhGoWhereLayer* JhGoWhereLayer::create(std::string addrname, WHERELAYER_TYPE type, float distance)
{
	JhGoWhereLayer *pRet = new JhGoWhereLayer();
	if (pRet && pRet->init(addrname, type, distance))
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

bool JhGoWhereLayer::init(std::string addrid, WHERELAYER_TYPE type, float distance)
{
	Node* csbnode = CSLoader::createNode("jhgoWhereLayer.csb");
	this->addChild(csbnode);

	m_addrstr = addrid;
	m_type = type;

	MapData mdata = JhGlobalData::map_maps[addrid];

	//名称
	cocos2d::ui::Text* title = (cocos2d::ui::Text*)csbnode->getChildByName("title");
	title->setString(mdata.cname);

	//描述
	cocos2d::ui::Text* desc = (cocos2d::ui::Text*)csbnode->getChildByName("desc");
	desc->setString(mdata.desc);

	//距离 ，换算成时间
	cocos2d::ui::Text* fast = (cocos2d::ui::Text*)csbnode->getChildByName("fast");

    float fmin = 0.0f;
    if (g_maplayer != NULL)
		fmin = g_maplayer->moveToDestTime(distance);

	int minute = (int)fmin;
	std::string faststr;
	if (minute / 60 > 0)
	{
		faststr = StringUtils::format("%d小时%d分钟", minute / 60, minute % 60);
		if (minute % 60 == 0)
			faststr = StringUtils::format("%d小时", minute / 60);
	}
	else
		faststr = StringUtils::format("%d分钟", minute);
	
	fast->setString(JhCommonFuncs::gbk2utf(faststr.c_str()));

	cocos2d::ui::Text* fasttitle = (cocos2d::ui::Text*)csbnode->getChildByName("text1");
	
	//地点的类型
	cocos2d::ui::ImageView* typeimg = (cocos2d::ui::ImageView*)csbnode->getChildByName("typepng");
	std::string str = StringUtils::format("images/%s.png", mdata.tpngname);
	typeimg->loadTexture(str, cocos2d::ui::TextureResType::LOCAL);
	
	cocos2d::ui::Button* closebtn = (cocos2d::ui::Button*)csbnode->getChildByName("closebtn");
	closebtn->addTouchEventListener(CC_CALLBACK_2(JhGoWhereLayer::onClose, this));

	m_gobtn = (cocos2d::ui::Button*)csbnode->getChildByName("gobtn");
	m_gobtn->addTouchEventListener(CC_CALLBACK_2(JhGoWhereLayer::onGO, this));

	//物品存放
	m_stbtn = (cocos2d::ui::Button*)csbnode->getChildByName("stbtn");
	m_stbtn->addTouchEventListener(CC_CALLBACK_2(JhGoWhereLayer::onST, this));

	m_stredpoint = (cocos2d::ui::Widget*)m_stbtn->getChildByName("redpoint");
	checkRedPoint(0);

	m_enterbtn = (cocos2d::ui::Button*)csbnode->getChildByName("enterbtn");
	m_enterbtn->addTouchEventListener(CC_CALLBACK_2(JhGoWhereLayer::onComeIn, this));

	cocos2d::ui::Button* backbtn = (cocos2d::ui::Button*)csbnode->getChildByName("backbtn");
	backbtn->addTouchEventListener(CC_CALLBACK_2(JhGoWhereLayer::onClose, this));

	if (type == GOWHERE)
	{
		closebtn->setVisible(true);
		m_gobtn->setVisible(true);
		m_stbtn->setVisible(false);
		m_enterbtn->setVisible(false);
		fast->setVisible(true);
		fasttitle->setVisible(true);
	}
	else if (type == ARRIVE)
	{
		closebtn->setVisible(false);
		m_gobtn->setVisible(false);
		m_stbtn->setVisible(true);
		m_enterbtn->setVisible(true);
		fast->setVisible(false);
		fasttitle->setVisible(false);
		if (m_addrstr.compare("m1-1") == 0)//家，没有物品存放
		{
			m_stbtn->setVisible(false);
			m_enterbtn->setPositionX(360);
		}
	}
	else if (type == ONWAY_JUMP)
	{
		closebtn->setVisible(true);
		closebtn->setTitleText(JhCommonFuncs::gbk2utf("不跳"));
		m_gobtn->setVisible(false);
		m_stbtn->setVisible(false);
		m_enterbtn->setVisible(true);
		m_enterbtn->setTitleText(JhCommonFuncs::gbk2utf("跳入"));
		fast->setVisible(false);
		fasttitle->setVisible(false);
		title->setString(JhCommonFuncs::gbk2utf("奇遇"));
		desc->setString(JhCommonFuncs::gbk2utf("发现悬崖下面紫光闪耀，好像藏着什么好东西，你想下去一探究竟，少侠还请三思，崖下白骨露野，跳下去多半会一命呜呼！"));
		std::string str = StringUtils::format("images/%s.jpg", "t008");
		typeimg->loadTexture(str, cocos2d::ui::TextureResType::LOCAL);
#ifdef ANALYTICS
		JhAnalyticUtil::onEvent("showjump");
#endif
	}
	//////layer 点击事件，屏蔽下层事件
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	this->schedule(schedule_selector(JhGoWhereLayer::checkRedPoint), 1.0f);
	this->scheduleOnce(schedule_selector(JhGoWhereLayer::delayShowNewerGuide), 0.2f);
	return true;
}


void JhGoWhereLayer::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();
	if (m_type == ONWAY_JUMP && g_maplayer != NULL)
		g_maplayer->heroPauseMoving();
}

void JhGoWhereLayer::onClose(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		if (m_type == ONWAY_JUMP && g_maplayer != NULL)
		{
			g_maplayer->heroResumeMoving();
		}
		this->removeFromParentAndCleanup(true);
	}
}

void JhGoWhereLayer::onGO(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		this->removeFromParentAndCleanup(true);
		if (g_maplayer != NULL)//地图上移动角色头像
			g_maplayer->showMoveToDest();
	}
}

void JhGoWhereLayer::onST(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		TempStorageLayer* layer = TempStorageLayer::create(m_addrstr);

		if (g_gameLayer != NULL)
			g_gameLayer->addChild(layer, 3, "TempStorageLayer");
	}
}

void JhGoWhereLayer::onComeIn(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		if (m_addrstr.compare("m1-1") == 0)//进入家
		{
			JhHomeLayer* homelayer = JhHomeLayer::create();
			g_gameLayer->addChild(homelayer, 1, "homelayer");
			g_maplayer->removeFromParentAndCleanup(true);
			g_maplayer = NULL;
		}
		else if (m_addrstr.compare("m1-2") == 0)//进入后山
		{
			if (g_gameLayer != NULL)
				g_gameLayer->addChild(JhHomeHill::create(), 3, "homehill");
		}
		else if (m_addrstr.compare("m1-9") == 0)
		{
			JhRollDiceLayer* rlayer = JhRollDiceLayer::create();
			g_gameLayer->addChild(rlayer, 5);
		}
		else
		{
			if (m_type == ONWAY_JUMP)
			{
				Scene* activityScene = JhActivitScene::createScene("images/findtreasure.jpg", JhCommonFuncs::gbk2utf(""));
				if (activityScene != NULL)
				{
					Director::getInstance()->pushScene(activityScene);
					this->removeFromParentAndCleanup(true);
				}
				return;
			}
			else//进入NPC
			{
				if (m_addrstr.compare("m13-1") == 0)
				{
					if (g_gameLayer != NULL)
						g_gameLayer->addChild(SepcialNpcLayer::create(m_addrstr), 3, "sepcialnpclayer");
				}
				else
				{
					if (g_gameLayer != NULL)
						g_gameLayer->addChild(JhNpcLayer::create(m_addrstr), 3, "npclayer");
					std::string mapname = JhGlobalData::map_maps[m_addrstr].cname;
					if (mapname.find(JhCommonFuncs::gbk2utf("客栈")) != std::string::npos)
					{
						g_uiScroll->addEventText(JhCommonFuncs::gbk2utf("小二：客官里面请，吃饭一两银子，睡觉二两银子，喝酒二两银子"), 25, Color3B(204, 4, 4));
					}
				}
			}
		}
		this->removeFromParentAndCleanup(true);
	}
}



void JhGoWhereLayer::checkRedPoint(float dt)
{
	if (JhGameDataSave::getInstance()->getTempStorage(m_addrstr).length() > 0)
		m_stredpoint->setVisible(true);
	else
		m_stredpoint->setVisible(false);
}

void JhGoWhereLayer::showNewerGuide(int step)
{
	std::vector<Node*> nodes;
	if ((step == 21 || step == 41) && m_type == GOWHERE)
		nodes.push_back(m_gobtn);
	else if ((step == 22 || step == 42) && m_type == ARRIVE)
		nodes.push_back(m_enterbtn);	
	else if (step == 51 && m_type == ARRIVE)
		nodes.push_back(m_stbtn);
	if (nodes.size() > 0)
		g_gameLayer->showNewerGuide(step, nodes);
}

void JhGoWhereLayer::delayShowNewerGuide(float dt)
{
	if (JhNewerGuideLayer::checkifNewerGuide(21))
		showNewerGuide(21);
	else if (JhNewerGuideLayer::checkifNewerGuide(22))
		showNewerGuide(22);
	else if (JhNewerGuideLayer::checkifNewerGuide(41))
		showNewerGuide(41);
	else if (JhNewerGuideLayer::checkifNewerGuide(42))
		showNewerGuide(42);
	else if (JhNewerGuideLayer::checkifNewerGuide(51))
	{
		if (m_type == ARRIVE)
		{
			std::string datastr = JhGameDataSave::getInstance()->getTempStorage(m_addrstr);
			std::vector<std::string> vec_retstr;
			JhCommonFuncs::split(datastr, vec_retstr, ";");
			if (vec_retstr.size() > 0)
				showNewerGuide(51);
		}
	}
}
