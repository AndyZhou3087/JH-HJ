#include "JhHomeHill.h"
#include "json.h"
#include "JhGlobalData.h"
#include "JhCommonFuncs.h"
#include "JhActionGetLayer.h"
#include "JhConst.h"
#include "JhGameScene.h"
#include "JhCommonFuncs.h"
#include "JhFightLayer.h"
#include "JhSoundManager.h"
#include "JhMapLayer.h"
#include "JhNewerGuideLayer.h"
JhHomeHill::JhHomeHill()
{
}


JhHomeHill::~JhHomeHill()
{
	JhSoundManager::getInstance()->playBackMusic(JhSoundManager::MUSIC_ID_MAP);
}

bool JhHomeHill::init()
{
	Node* csbnode = CSLoader::createNode("jhhomeHillLayer.csb");
	this->addChild(csbnode);

	//返回按钮
	m_backbtn = (cocos2d::ui::Widget*)csbnode->getChildByName("backbtn");
	m_backbtn->addTouchEventListener(CC_CALLBACK_2(JhHomeHill::onBack, this));
	
	int ressize = JhGlobalData::vec_hillResid.size();
	scrollView = (cocos2d::ui::ScrollView*)csbnode->getChildByName("ScrollView");
	scrollView->setScrollBarEnabled(false);
	scrollView->setBounceEnabled(true);

	int itemheight = 160;
	int innerheight = itemheight * ressize;
	int contentheight = scrollView->getContentSize().height;
	if (innerheight < contentheight)
		innerheight = contentheight;
	scrollView->setInnerContainerSize(Size(650, innerheight));

	for (int i = 0; i < ressize; i++)
	{
		//产出资源node
		Node* resnode = CSLoader::createNode("jhresNode.csb");
		std::string namestr = StringUtils::format("node%d", i);
		scrollView->addChild(resnode,0, namestr);
		resnode->setPosition(Vec2(scrollView->getContentSize().width/2, innerheight - i * itemheight - itemheight / 2));

		//资源id
		cocos2d::ui::ImageView* iconimg = (cocos2d::ui::ImageView*)resnode->getChildByName("icon");
		//资源名称
		cocos2d::ui::Text* name = (cocos2d::ui::Text*)resnode->getChildByName("name");
		//产出个数
		cocos2d::ui::Text* count = (cocos2d::ui::Text*)resnode->getChildByName("count");
		//产出速度
		cocos2d::ui::Text* speed = (cocos2d::ui::Text*)resnode->getChildByName("speed");
		//等待时间，采完之后显示，恢复满
		cocos2d::ui::Text* waittime = (cocos2d::ui::Text*)resnode->getChildByName("waittime");
		//资源描述
		cocos2d::ui::Text* desc = (cocos2d::ui::Text*)resnode->getChildByName("desc");
		cocos2d::ui::Text* waittext = (cocos2d::ui::Text*)resnode->getChildByName("waittext");
		
		//操作按钮
		cocos2d::ui::Button* actionbtn = (cocos2d::ui::Button*)resnode->getChildByName("actionbtn");
		actionbtn->addTouchEventListener(CC_CALLBACK_2(JhHomeHill::onclick, this));

		for (unsigned int m = 0; m < JhGlobalData::vec_resData.size(); m++)
		{
			ResData data = JhGlobalData::vec_resData[m];
			if (JhGlobalData::vec_hillResid[i].compare(data.strid) == 0)
			{
				actionbtn->setUserData((void*)JhGlobalData::vec_resData[m].strid.c_str());
				std::string str = StringUtils::format("ui/%s.png", data.strid.c_str());
				iconimg->loadTexture(str, cocos2d::ui::TextureResType::PLIST);
				iconimg->setContentSize(Sprite::createWithSpriteFrameName(str)->getContentSize());

				name->setString(JhGlobalData::map_allResource[data.strid].cname);
				desc->setString(JhGlobalData::map_allResource[data.strid].desc);

				str = StringUtils::format("%d", data.count);
				count->setString(str);

				float fs = (data.speed[g_nature->getReason()]) / 60.0f;
				str = StringUtils::format("%.1fh", fs);
				speed->setString(str);

				float wfs = fs * data.max;
				str = StringUtils::format("%.1fh", wfs);
				waittime->setString(str);

				if (data.count <= 0)
				{
					waittext->setVisible(true);
					waittime->setVisible(true);
					actionbtn->setEnabled(false);
				}
				else
				{
					waittext->setVisible(false);
					waittime->setVisible(false);
					actionbtn->setEnabled(true);
				}

				actionbtn->setTitleText(JhCommonFuncs::gbk2utf(acname[data.actype].c_str()));
				
			}
		}

	}
	this->schedule(schedule_selector(JhHomeHill::updateUI),0.2f);


	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	JhSoundManager::getInstance()->playBackMusic(JhSoundManager::MUSIC_ID_ENTER_MAPADDR);

	return true;
}

void JhHomeHill::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();
	showNewerGuide(23);
}

void JhHomeHill::onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
        if (g_maplayer != NULL)
            g_maplayer->scheduleOnce(schedule_selector(JhMapLayer::delayShowMapNewerGuide), 0.2f);
		this->removeFromParentAndCleanup(true);
	}
}

void JhHomeHill::onclick(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		Node* node = (Node*)pSender;
		unsigned int i = 0;
		ResData* data = NULL;
		for (i = 0; i<JhGlobalData::vec_resData.size(); i++)
		{
			data = &JhGlobalData::vec_resData[i];
			std::string strid = (char*)node->getUserData();
			if (data->strid.compare(strid) == 0)
				break;
		}

		if (JhGlobalData::vec_resData[i].count > 0)
		{
			JhGlobalData::vec_resData[i].count--;

			if (data->strid.compare("67") == 0 || data->strid.compare("68") == 0)//兔子，狼战斗界面
			{
				std::string npcid = "n002";
				if (data->strid.compare("68") == 0)
					npcid = "n003";
				JhFightLayer* layer = JhFightLayer::create("m1-2", npcid);
				if (g_gameLayer != NULL)
					g_gameLayer->addChild(layer, 4, "fightlayer");
			}
			else
			{
				JhActionGetLayer* layer = JhActionGetLayer::create(i, data->res, data->type, data->actype);
				if (g_gameLayer != NULL)
					g_gameLayer->addChild(layer, 3, "JhActionGetLayer");
			}


		}
	}
}

void JhHomeHill::updateUI(float dt)
{
	for (unsigned int i = 0; i < JhGlobalData::vec_hillResid.size(); i++)
	{
		std::string namestr = StringUtils::format("node%d", i);
		Node* resnode = scrollView->getChildByName(namestr);

		cocos2d::ui::Text* count = (cocos2d::ui::Text*)resnode->getChildByName("count");
		cocos2d::ui::Text* speed = (cocos2d::ui::Text*)resnode->getChildByName("speed");
		cocos2d::ui::Text* waittime = (cocos2d::ui::Text*)resnode->getChildByName("waittime");
		cocos2d::ui::Text* waittext = (cocos2d::ui::Text*)resnode->getChildByName("waittext");
		cocos2d::ui::Button* actionbtn = (cocos2d::ui::Button*)resnode->getChildByName("actionbtn");
		for (unsigned int m = 0; m < JhGlobalData::vec_resData.size(); m++)
		{
			ResData* data = &JhGlobalData::vec_resData[m];
			if (JhGlobalData::vec_hillResid[i].compare(data->strid) == 0)
			{
				std::string str = StringUtils::format("%d", data->count);
				count->setString(str);
				str = StringUtils::format("%.1fh", data->speed[g_nature->getReason()] / 60.f);
				speed->setString(str);

				str = StringUtils::format("%.1fh", (data->speed[g_nature->getReason()] * data->max - data->waittime) / 60.f);
				waittime->setString(str);
	
				if (data->count <= 0)
				{
					waittext->setVisible(true);
					waittime->setVisible(true);
					count->setColor(Color3B::RED);
					actionbtn->setEnabled(false);
				}
				else
				{
					waittext->setVisible(false);
					waittime->setVisible(false);
					count->setColor(Color3B::BLACK);
					actionbtn->setEnabled(true);
				}
			}
		}
	}
}

void JhHomeHill::showNewerGuide(int step)
{
	std::vector<Node*> nodes;
	if (step == 23)
	{
		Node* resnode = scrollView->getChildByName("node0");

		nodes.push_back(resnode->getChildByName("actionbtn"));
	}
	else if (step == 28)
	{
		scrollView->jumpToPercentVertical(20);
		Node* resnode = scrollView->getChildByName("node4");

		nodes.push_back(resnode->getChildByName("actionbtn"));
	}

	else if (step == 33 || step == 36)
	{
		scrollView->jumpToPercentVertical(70);
		Node* resnode = scrollView->getChildByName("node7");

		nodes.push_back(resnode->getChildByName("actionbtn"));
	}
	else if (step == 39)
	{
		nodes.push_back(m_backbtn);

	}

	g_gameLayer->showNewerGuide(step, nodes);
}
