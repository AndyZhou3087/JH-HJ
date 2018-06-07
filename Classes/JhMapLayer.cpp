#include "JhMapLayer.h"
#include "json.h"
#include "JhHomeLayer.h"
#include "JhHomeHill.h"
#include "JhGoWhereLayer.h"
#include "JhGameDataSave.h"
#include "JhConst.h"
#include "JhGameScene.h"
#include "JhCommonFuncs.h"
#include "JhGlobalData.h"
#include "JhShopLayer.h"
#include "JhSoundManager.h"
#include "JhUnlockLayer.h"
#include "JhNewerGuideLayer.h"
#include "JhAnalyticUtil.h"
#include "JhFightLayer.h"
#include "JhStoryScene.h"
#include "JhVipShopLayer.h"
#include "JhTimeGiftLayer.h"
#include "JhRelationLayer.h"
#include "JhRankLayer.h"
#include "JhFactionMainLayer.h"
#include "JhNewerGuide2Layer.h"
#include "JhRaffleLayer.h"
#include "JhAchiveLayer.h"
#include "JhPrizeLayer.h"
#include "JhHSLJMainLayer.h"
#include "JhBranchMissionLayer.h"
#include "JhHintBox.h"
#include "JhHelpMainLayer.h"
#include "JhRechargeLayer.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "iosfunc.h"
#endif

JhMapLayer* g_maplayer = NULL;
JhMapLayer::JhMapLayer()
{
	m_lotteryimg = NULL;
	ismoving = false;
	if (g_hero != NULL)
		g_hero->setIsMoving(false);
	m_isDraging = false;
	m_distance = 0.0f;
}


JhMapLayer::~JhMapLayer()
{
	ismoving = false;
	if (g_hero != NULL)
		g_hero->setIsMoving(false);
	g_maplayer = NULL;
}

bool JhMapLayer::init()
{
	Node* csbnode = CSLoader::createNode("jhmapLayer.csb");
	this->addChild(csbnode);

	m_mapscroll = (cocos2d::ui::ScrollView*)csbnode->getChildByName("ScrollView");
	m_mapscroll->setScrollBarEnabled(false);
	m_mapscroll->setSwallowTouches(false);

	m_mapbg = (cocos2d::ui::Widget*)m_mapscroll->getChildByName("mapbg");

	int mapnamecount = JhGlobalData::map_maps.size();
	int heroposindex = 0;
	std::string addr = JhGameDataSave::getInstance()->getHeroAddr();

	for (int i = 0; i < mapnamecount; i++)
	{
		cocos2d::ui::Widget* mapname = (cocos2d::ui::Widget*)m_mapbg->getChildren().at(i);
		mapname->addTouchEventListener(CC_CALLBACK_2(JhMapLayer::onclick, this));
		mapname->setSwallowTouches(false);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
		mapname->setVisible(false);
#else
		mapname->setVisible(false);
#endif
		if (mapname->getName().compare(addr) == 0)
		{
			heroposindex = i;
		}

		if (mapname->getName().compare("m1-9") == 0)
		{
			m_lotteryimg = mapname;
			m_lotteryimg->setScale(0.0f);
		}
		else if (mapname->getName().compare("m1-10") == 0)
		{
			achiveRedpoint = (cocos2d::ui::Widget*)mapname->getChildByName("redpoint");
			achiveRedpoint->setVisible(false);
		}
		else if (mapname->getName().compare("m13-1") == 0)
		{
			mapname->setVisible(true);
		}
	}

	Vec2 pos = m_mapbg->getChildren().at(heroposindex)->getPosition();

	mapMoveTo(pos);

	m_heroPos = m_mapbg->getChildByName(addr)->getPosition();

	std::string heroidstr = StringUtils::format("ui/herohead%d.png", g_hero->getHeadID());
	m_herohead = Sprite::createWithSpriteFrameName(heroidstr);
	m_herohead->setAnchorPoint(Vec2(0.5, 0));
	m_herohead->setPosition(m_heroPos);
	if (addr.compare("m1-9") == 0)
	{
		m_herohead->setVisible(false);
	}
	m_mapscroll->addChild(m_herohead);

	updateUnlockChapter();

	for (int i = 0; i < 2; i++)
	{
		std::string missionstr = StringUtils::format("ui/mapmission%d_0.png", i);
		m_smissionIcon[i] = Sprite::createWithSpriteFrameName(missionstr);
		//m_smissionIcon[i]->setAnchorPoint(Vec2(0.5, 0));
		m_smissionIcon[i]->setVisible(false);
		m_mapbg->addChild(m_smissionIcon[i]);

		missionstr = StringUtils::format("ui/mapmission%d_1.png", i);
		m_dmissionIcon[i] = Sprite::createWithSpriteFrameName(missionstr);
		//m_dmissionIcon[i]->setAnchorPoint(Vec2(0.5, 0));
		m_dmissionIcon[i]->setVisible(false);
		m_mapbg->addChild(m_dmissionIcon[i]);
		updataPlotMissionIcon(i);
	}

	cocos2d::ui::Widget* shopbtn = (cocos2d::ui::Widget*)csbnode->getChildByName("shopbtn");
	shopbtn->addTouchEventListener(CC_CALLBACK_2(JhMapLayer::onShop, this));


	cocos2d::ui::Widget* vipbtn = (cocos2d::ui::Widget*)csbnode->getChildByName("vipbtn");
	vipbtn->addTouchEventListener(CC_CALLBACK_2(JhMapLayer::onVipShop, this));

	brachmissionicon = (cocos2d::ui::Widget*)csbnode->getChildByName("branchmission");
	brachmissionicon->setVisible(false);
	brachmissionicon->addTouchEventListener(CC_CALLBACK_2(JhMapLayer::onBranchMisson, this));

	m_timegiftbtn = (cocos2d::ui::Widget*)csbnode->getChildByName("timegiftbtn");
	m_timegiftbtn->addTouchEventListener(CC_CALLBACK_2(JhMapLayer::onTimeGift, this));
	m_timegiftbtn->setVisible(false);

	m_rafflebtn = (cocos2d::ui::Widget*)csbnode->getChildByName("rafflebtn");
	m_rafflebtn->addTouchEventListener(CC_CALLBACK_2(JhMapLayer::onRaffle, this));
	m_rafflebtn->setVisible(false);

	m_helpbtn = (cocos2d::ui::Widget*)csbnode->getChildByName("helpbtn");
	m_helpbtn->addTouchEventListener(CC_CALLBACK_2(JhMapLayer::onHelp, this));

	m_rechargebtn = (cocos2d::ui::Widget*)csbnode->getChildByName("rechargebtn");
	m_rechargebtn->addTouchEventListener(CC_CALLBACK_2(JhMapLayer::onRecharge, this));
	m_rechargebtn->setVisible(false);

	m_prizebtn = (cocos2d::ui::Widget*)csbnode->getChildByName("prizebtn");
	m_prizebtn->addTouchEventListener(CC_CALLBACK_2(JhMapLayer::onPrize, this));
	m_prizebtn->setVisible(false);

	m_tgiftlefttimelbl = (cocos2d::ui::Text*)m_timegiftbtn->getChildByName("lefttimelbl");

	if (!JhGlobalData::isOnline)
	{
		vipbtn->setVisible(false);
		m_timegiftbtn->setVisible(true);
		m_timegiftbtn->setPosition(vipbtn->getPosition());
		m_tgiftlefttimelbl->setString(JhCommonFuncs::gbk2utf("限时礼包"));
	}
	checkTimeGift(0);
	this->schedule(schedule_selector(JhMapLayer::checkTimeGift), 1.0f);
	this->schedule(schedule_selector(JhMapLayer::checkAchive), 2.0f);

	m_vulture = (cocos2d::ui::Widget*)m_mapbg->getChildByName("m1-1")->getChildByName("vulture");
	m_vulture->setVisible(false);

	if (JhGlobalData::isHasVulture())
		vultureAnim();


	g_uiScroll->setVisible(true);
	scrollbg->setVisible(true);

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		m_isDraging = false;
		m_startClickX = touch->getLocation().x;
		m_startClickY = touch->getLocation().y;
		return true;
	};

	listener->onTouchMoved = [=](Touch *touch, Event *event)
	{
		if (fabsf(m_startClickX - touch->getLocation().x) > 20 || fabsf(m_startClickY - touch->getLocation().y) > 20)
			m_isDraging = true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	if (g_nature != NULL)
	{
		if (g_nature->getWeather() == Rainy)
			rain();
		else if (g_nature->getWeather() == Snowy)
			snow();
		else
			removeWeatherPaticle();
	}
	JhSoundManager::getInstance()->playBackMusic(JhSoundManager::MUSIC_ID_MAP);

	return true;
}

void JhMapLayer::onclick(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		if (m_isDraging || ismoving)
		{
			return;
		}
		JhSoundManager::getInstance()->playSound(JhSoundManager::SOUND_ID_BUTTON);

		Node* node = (Node*)pSender;
		m_addrname = node->getName();

		if (m_addrname.compare("m1-6") == 0)
		{
			JhRelationLayer* relationlayer = JhRelationLayer::create();
			g_gameLayer->addChild(relationlayer, 5);
		}
		else if (m_addrname.compare("m1-7") == 0)
		{
			JhRankLayer* ranklayer = JhRankLayer::create();
			g_gameLayer->addChild(ranklayer, 5, "ranklayer");
		}
		else if (m_addrname.compare("m1-8") == 0)
		{
			JhFactionMainLayer* factionmainlayer = JhFactionMainLayer::create();
			g_gameLayer->addChild(factionmainlayer, 5, "factionmainlayer");
		}
		else if (m_addrname.compare("m1-10") == 0)
		{
			JhAchiveLayer* alayer = JhAchiveLayer::create();
			g_gameLayer->addChild(alayer, 5);
		}
		else if (m_addrname.compare("m1-11") == 0)
		{
			JhHSLJMainLayer* alayer = JhHSLJMainLayer::create();
			g_gameLayer->addChild(alayer, 5, "hsljmainlayer");
		}
		else
		{
			m_destPos = node->getPosition();
			m_distance = fabsf(m_heroPos.distance(m_destPos));
			WHERELAYER_TYPE type = ARRIVE;
			if (m_distance <= 1.0f)
				type = ARRIVE;
			else
				type = GOWHERE;

			if (g_gameLayer != NULL)
				g_gameLayer->addChild(JhGoWhereLayer::create(m_addrname, type, m_distance), 3, "gowherelayer");
		}

	}
}

void JhMapLayer::heroPauseMoving()
{
	if (g_nature != NULL)
	{
		g_nature->setTimeInterval(NORMAL_TIMEINTERVAL);
		m_herohead->pause();
	}
}

void JhMapLayer::heroResumeMoving()
{
	if (ismoving && g_nature != NULL)
	{
		g_nature->setTimeInterval(TIMESCALE * 8);
		m_herohead->resume();
	}
}

void JhMapLayer::showRobberFight(float dt)
{
	heroPauseMoving();
	g_gameLayer->addChild(JhFightLayer::create(m_addrname, "n001"), 5, "fightlayer");
}

void JhMapLayer::showJump(float dt)
{
	heroPauseMoving();

	g_gameLayer->addChild(JhGoWhereLayer::create(m_addrname, ONWAY_JUMP), 2, "gowherelayer");
}

void JhMapLayer::showMoveToDest()
{
	ismoving = true;
	g_hero->setIsMoving(true);
	g_nature->setTimeInterval(TIMESCALE * 8);

	float dt = moveToDestTime(m_distance);

	m_herohead->runAction(Sequence::create(MoveTo::create(dt / (TIMESCALE * 8.0f), m_destPos), CallFunc::create(CC_CALLBACK_0(JhMapLayer::Arrive, this)), NULL));

	//int sec = dt / (TIMESCALE * 8.0f);
	//if (sec >= 2)
	//{
	//	int r = JhGlobalData::createRandomNum(100);
	//	int rnd = g_nature->getDayOrNight() == Night ? 30 : 20;

	//	if (r < rnd)
	//	{
	//		int r1 = JhGlobalData::createRandomNum(sec - 1) + 1;

	//		this->scheduleOnce(schedule_selector(JhMapLayer::showRobberFight), r1);
	//	}
	//}

	if (JhGlobalData::map_maps[m_addrname].isCliff)
	{
		int sec = dt / (TIMESCALE * 8.0f);
		int r = JhGlobalData::createRandomNum(100);
		if (r < 5)
		{
			this->scheduleOnce(schedule_selector(JhMapLayer::showJump), sec - 0.5f);
		}
	}
}

void JhMapLayer::Arrive()
{
	g_nature->setTimeInterval(NORMAL_TIMEINTERVAL);
	m_heroPos = m_destPos;
	JhGameDataSave::getInstance()->setHeroAddr(m_addrname);

	if (m_distance > 1.0f)
	{
		std::string npcnames;
		int npcsize = JhGlobalData::map_maps[m_addrname].npcs.size();
		if (npcsize > 0)
		{
			npcnames.append(JhCommonFuncs::gbk2utf("这里有"));
			if (m_addrname.compare("m13-1") == 0)
			{
				npcnames.append(JhCommonFuncs::gbk2utf("江湖各大高手。"));
			}
			else
			{
				for (int i = 0; i < npcsize; i++)
				{
					npcnames.append(JhGlobalData::map_npcs[JhGlobalData::map_maps[m_addrname].npcs[i]].name);
					if (i == npcsize - 1)
						npcnames.append(JhCommonFuncs::gbk2utf("。"));
					else
						npcnames.append(JhCommonFuncs::gbk2utf("，"));
				}
			}
		}

		std::string str;
		if (g_hero->getAtrByType(H_MOUNT)->count > 0 && g_hero->getAtrByType(H_MOUNT)->goodvalue> 0)
			str.append(JhCommonFuncs::gbk2utf("你骑着马儿，一溜烟的来到了")); 
		else
			str.append(JhCommonFuncs::gbk2utf("你跑得双腿发麻，来到了"));

		str.append(JhGlobalData::map_maps[m_addrname].cname);
		str.append(JhCommonFuncs::gbk2utf("，"));
		str.append(JhGlobalData::map_maps[m_addrname].desc);
		str.append(JhCommonFuncs::gbk2utf("。"));
		str.append(npcnames);
		g_uiScroll->addEventText(str.c_str());

		if (g_hero->getAtrByType(H_MOUNT)->count > 0 && g_hero->getAtrByType(H_MOUNT)->strid.compare("74") == 0)
		{
			g_hero->getAtrByType(H_MOUNT)->goodvalue--;

			if (g_hero->getAtrByType(H_MOUNT)->goodvalue < 0)
			{
				g_hero->getAtrByType(H_MOUNT)->goodvalue = 0;
			}
			else if (g_hero->getAtrByType(H_MOUNT)->goodvalue == 20 || g_hero->getAtrByType(H_MOUNT)->goodvalue == 10)
			{
				std::string strid = g_hero->getAtrByType(H_MOUNT)->strid;
				std::string descstr = StringUtils::format("%s%s%d", JhGlobalData::map_allResource[strid].cname.c_str(), JhCommonFuncs::gbk2utf("生命仅剩").c_str(), g_hero->getAtrByType(H_MOUNT)->goodvalue);
				g_uiScroll->addEventText(descstr, 25, Color3B(204, 4, 4));
			}

			if (g_hero->getAtrByType(H_MOUNT)->goodvalue <= 10)
			{
				showTalkGuide();
			}
		}
	}
	if (g_gameLayer != NULL)
		g_gameLayer->addChild(JhGoWhereLayer::create(m_addrname, ARRIVE), 3, "gowherelayer");

	this->scheduleOnce(schedule_selector(JhMapLayer::finishMove), 0.1f);
}

void JhMapLayer::finishMove(float dt)
{
	ismoving = false;
	g_hero->setIsMoving(false);
}

void JhMapLayer::onShop(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		JhSoundManager::getInstance()->playSound(JhSoundManager::SOUND_ID_BUTTON);
		JhShopLayer* shopLayer = JhShopLayer::create();
		g_gameLayer->addChild(shopLayer, 5);
	}
}

void JhMapLayer::onVipShop(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		JhSoundManager::getInstance()->playSound(JhSoundManager::SOUND_ID_BUTTON);
		JhVipShopLayer* shopLayer = JhVipShopLayer::create();
		g_gameLayer->addChild(shopLayer, 5, "vipshoplayer");
	}
}

void JhMapLayer::onTimeGift(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		JhSoundManager::getInstance()->playSound(JhSoundManager::SOUND_ID_BUTTON);
		JhTimeGiftLayer* giftLayer = JhTimeGiftLayer::create(TIMEGIFT);
		g_gameLayer->addChild(giftLayer, 5, "gift");

	}
}

void JhMapLayer::onRaffle(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		JhSoundManager::getInstance()->playSound(JhSoundManager::SOUND_ID_BUTTON);
		JhRaffleLayer* rlayer = JhRaffleLayer::create();
		g_gameLayer->addChild(rlayer, 5);
	}
}

void JhMapLayer::onPrize(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		JhSoundManager::getInstance()->playSound(JhSoundManager::SOUND_ID_BUTTON);
		JhPrizeLayer* pzlayer = JhPrizeLayer::create();
		g_gameLayer->addChild(pzlayer, 5);
	}
}

void JhMapLayer::updateUnlockChapter()
{
	std::map<std::string, MapData>::iterator it;

	for (it = JhGlobalData::map_maps.begin(); it != JhGlobalData::map_maps.end(); ++it)
	{
		std::string mapid = JhGlobalData::map_maps[it->first].strid;
		if (mapid.length() > 0)
		{
			cocos2d::ui::Widget* mapNamImage = (cocos2d::ui::Widget*)m_mapbg->getChildByName(mapid);

			std::vector<std::string> tmp;

			JhCommonFuncs::split(mapid, tmp, "-");
			int mapchapter = atoi(tmp[0].substr(1, tmp[0].size() - 1).c_str());
			if (mapchapter <= JhGlobalData::getUnlockChapter())
			{
				if (mapNamImage != NULL)
				{
					if (JhGlobalData::isOnline)
						mapNamImage->setVisible(true);
					else
					{
						if (mapid.compare("m1-7") == 0 )
							mapNamImage->setVisible(false);
						else if (mapid.compare("m1-8") == 0)
							mapNamImage->setVisible(false);
						else if (mapid.compare("m1-11") == 0)
							mapNamImage->setVisible(false);
						else
							mapNamImage->setVisible(true);
					}
				}
			}
		}
	}
}

void JhMapLayer::showUnlockLayer(float dt)
{

	Director::getInstance()->getRunningScene()->addChild(JhUnlockLayer::create(), 10);
	updateUnlockChapter();
#ifdef ANALYTICS
	std::string unlockstr = StringUtils::format("u%d", JhGlobalData::getUnlockChapter());
	JhAnalyticUtil::onEvent(unlockstr.c_str());
#endif
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	this->scheduleOnce(schedule_selector(JhMapLayer::delayShowComments), 2.0f);
#endif
	JhGlobalData::doAchive(A_5, JhGlobalData::getUnlockChapter());
}

void JhMapLayer::delayShowComments(float dt)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	if (!JhGlobalData::getNoComments())
		alterView();
#endif
}

void JhMapLayer::updataPlotMissionIcon(int type)
{

	m_smissionIcon[type]->stopAllActions();
	m_smissionIcon[type]->setVisible(false);
	m_dmissionIcon[type]->stopAllActions();
	m_dmissionIcon[type]->setVisible(false);

	int mapnamecount = JhGlobalData::map_maps.size();

	if (type == 0)
	{
		PlotMissionData* plotData = NULL;
		int plotindex = 0;
		plotindex = JhGlobalData::getPlotMissionIndex();
		plotData = &JhGlobalData::vec_PlotMissionData[plotindex];

		std::string snpc = plotData->snpc;
		std::string dnpc = plotData->dnpc;

		if (plotData->mapid.length() > 0)
		{
			for (unsigned int m = 0; m < JhGlobalData::map_maps[plotData->mapid].npcs.size(); m++)
			{
				if (snpc.compare(JhGlobalData::map_maps[plotData->mapid].npcs.at(m)) == 0)
				{
					if (plotData->status == M_NONE)
					{
						if (plotData->words.size() <= 0)
						{
							plotData->status = M_DOING;
						}
						else
						{
							m_smissionIcon[type]->setPosition(Vec2(m_mapbg->getChildByName(plotData->mapid)->getPosition().x, m_mapbg->getChildByName(plotData->mapid)->getPosition().y + m_smissionIcon[type]->getContentSize().height/2));
							m_smissionIcon[type]->setVisible(true);
							m_smissionIcon[type]->runAction(RepeatForever::create(Blink::create(2, 3)));
						}
					}
					else
					{
						m_smissionIcon[type]->stopAllActions();
						m_smissionIcon[type]->setVisible(false);
					}
				}
			}
		}
		else
		{
			for (int i = mapnamecount - 1; i >= 0; i--)
			{
				cocos2d::ui::Widget* mapname = (cocos2d::ui::Widget*)m_mapbg->getChildren().at(i);
				for (unsigned int m = 0; m < JhGlobalData::map_maps[mapname->getName()].npcs.size(); m++)
				{
					if (snpc.compare(JhGlobalData::map_maps[mapname->getName()].npcs.at(m)) == 0)
					{
						if (plotData->status == M_NONE)
						{
							if (plotData->words.size() <= 0)
							{
								plotData->status = M_DOING;
							}
							else
							{
								m_smissionIcon[type]->setPosition(Vec2(mapname->getPosition().x, mapname->getPosition().y + m_smissionIcon[type]->getContentSize().height/2));
								m_smissionIcon[type]->setVisible(true);
								m_smissionIcon[type]->runAction(RepeatForever::create(Blink::create(2, 3)));
							}
						}
						else
						{
							m_smissionIcon[type]->stopAllActions();
							m_smissionIcon[type]->setVisible(false);
						}
					}
				}
			}
		}

		if (plotData->mapid.length() > 0)
		{
			for (unsigned int m = 0; m < JhGlobalData::map_maps[plotData->mapid].npcs.size(); m++)
			{
				if (dnpc.compare(JhGlobalData::map_maps[plotData->mapid].npcs.at(m)) == 0)
				{
					if (plotData->status == M_DOING)
					{
						m_dmissionIcon[type]->setVisible(true);
						m_dmissionIcon[type]->runAction(RepeatForever::create(Blink::create(2, 3)));
						m_dmissionIcon[type]->setPosition(Vec2(m_mapbg->getChildByName(plotData->mapid)->getPosition().x, m_mapbg->getChildByName(plotData->mapid)->getPosition().y + m_dmissionIcon[type]->getContentSize().height / 2));
					}
					else
					{
						m_dmissionIcon[type]->stopAllActions();
						m_dmissionIcon[type]->setVisible(false);
					}

				}
			}
		}
		else
		{
			for (int i = mapnamecount - 1; i >= 0; i--)
			{
				cocos2d::ui::Widget* mapname = (cocos2d::ui::Widget*)m_mapbg->getChildren().at(i);
				for (unsigned int m = 0; m < JhGlobalData::map_maps[mapname->getName()].npcs.size(); m++)
				{
					if (dnpc.compare(JhGlobalData::map_maps[mapname->getName()].npcs.at(m)) == 0)
					{
						if (plotData->status == M_DOING)
						{
							m_dmissionIcon[type]->setVisible(true);
							m_dmissionIcon[type]->runAction(RepeatForever::create(Blink::create(2, 3)));
							m_dmissionIcon[type]->setPosition(Vec2(mapname->getPosition().x, mapname->getPosition().y + m_dmissionIcon[type]->getContentSize().height / 2));
						}
						else
						{
							m_dmissionIcon[type]->stopAllActions();
							m_dmissionIcon[type]->setVisible(false);
						}
					}
				}
			}
		}
	}
}

void JhMapLayer::mapMoveTo(Vec2 pos)
{
	float offsetx = 0.0f;
	float offsety = 0.0f;
	Size scollviewsize = m_mapscroll->getContentSize();
	if (pos.x > scollviewsize.width / 2)
		offsetx = pos.x - scollviewsize.width / 2;
	if (pos.y > scollviewsize.height / 2)
		offsety = pos.y - scollviewsize.height / 2;

	if (pos.x + scollviewsize.width / 2 > m_mapscroll->getInnerContainerSize().width)
		offsetx = m_mapscroll->getInnerContainerSize().width - scollviewsize.width;
	if (pos.y + scollviewsize.height / 2 > m_mapscroll->getInnerContainerSize().height)
		offsety = m_mapscroll->getInnerContainerSize().height - scollviewsize.height;

	m_mapscroll->setInnerContainerPosition(Vec2(-offsetx, -offsety));
}

void JhMapLayer::delayShowMapNewerGuide(float dt)
{
	if (JhNewerGuideLayer::checkifNewerGuide(20))
		showNewerGuide(20);
	else if (JhNewerGuideLayer::checkifNewerGuide(40))
		showNewerGuide(40);
	else if (JhNewerGuideLayer::checkifNewerGuide(48))
		showNewerGuide(48);
}

float JhMapLayer::moveToDestTime(float distance)
{
	float dt = distance * HERO_MOVE_SPEED;
	if (g_hero->getAtrByType(H_MOUNT)->count > 0 && g_hero->getAtrByType(H_MOUNT)->goodvalue > 0)
	{
		int index = -1;
		for (unsigned int i = 0; i < JhGlobalData::vec_resData.size(); i++)
		{
			ResData rdata = JhGlobalData::vec_resData[i];
			if (rdata.strid.compare(g_hero->getAtrByType(H_MOUNT)->strid) == 0)
			{
				index = i;
				break;
			}
		}
		if (index >= 0)
		{
			int ep = JhGlobalData::vec_resData[index].ep[0];
			dt *= 100.0f / (100.0f + ep);
		}
	}
	return dt;
}

void JhMapLayer::showEndAnim(float dt)
{
	Director::getInstance()->pushScene(JhStoryScene::createScene(1));
}

void JhMapLayer::showNewerGuide(int step)
{
	std::vector<Node*> nodes;
	if (step == 20)
	{
		nodes.push_back(m_mapbg->getChildByName("m1-2"));
	}
	else if (step == 40)
	{
		nodes.push_back(m_mapbg->getChildByName("m1-1"));
	}
	else if (step == 48)
	{
		nodes.push_back(m_mapbg->getChildByName("m1-4"));
	}
	g_gameLayer->showNewerGuide(step, nodes);
}

void JhMapLayer::rain()
{
	removeWeatherPaticle();
	ParticleRain *rain = ParticleRain::create();
	rain->setScale(5);
	rain->setTotalParticles(1500);
	rain->setPosition(Vec2(360, 1400));
	rain->setTextureWithRect(Director::getInstance()->getTextureCache()->addImage("particle/rain.png"), Rect(0, 0, 96, 96));
	this->addChild(rain, 1000, "rain");
}

void JhMapLayer::snow()
{
	removeWeatherPaticle();
	ParticleSnow *snow = ParticleSnow::create();
	snow->setSpeed(100);
	snow->setPosition(Vec2(360, 1400));
	snow->setTextureWithRect(Director::getInstance()->getTextureCache()->addImage("particle/snow.png"), Rect(0, 0, 32, 32));
	this->addChild(snow, 1000, "snow");
}

void JhMapLayer::removeWeatherPaticle()
{
	if (g_maplayer != NULL)
	{
		g_maplayer->removeChildByName("rain");
		g_maplayer->removeChildByName("snow");
	}
}

void JhMapLayer::vultureAnim()
{
	m_vulture->stopAllActions();
	m_vulture->setVisible(true);

	auto animation = Animation::create();
	for (int i = 1; i < 10; i++)
	{
		char szName[20] = { 0 };
		sprintf(szName, "ui/vulture%d.png", i);
		SpriteFrame* frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(szName);
		animation->addSpriteFrame(frame);
	}
	animation->setDelayPerUnit(0.1f);
	//animation->setRestoreOriginalFrame(true);

	auto action = Animate::create(animation);
	PointArray *arrayPoint = PointArray::create(5);
	arrayPoint->addControlPoint(Vec2(30, 110));
	arrayPoint->addControlPoint(Vec2(55, 90));
	arrayPoint->addControlPoint(Vec2(30, 70));
	arrayPoint->addControlPoint(Vec2(10,90));
	arrayPoint->addControlPoint(Vec2(30, 110));
	ActionInterval *forward = CCCardinalSplineTo::create(arrayPoint->count(), arrayPoint, 0);
	m_vulture->runAction(RepeatForever::create(forward));
	m_vulture->runAction(RepeatForever::create(action));
	//m_vulture->runAction(Sequence::create(action, action->reverse(), NULL));
}

void JhMapLayer::checkTimeGift(float dt)
{
	int lefttime = JhGlobalData::getTimeGiftLeftTime();
	if (lefttime > 0 && !JhGlobalData::getIsBuyTimeGift())
	{
		m_timegiftbtn->setVisible(true);
		int hour = lefttime / 3600;
		int min = lefttime % 3600 / 60;
		int sec = lefttime % 3600 % 60;
		std::string timestr = StringUtils::format("%02d:%02d:%02d", hour,min,sec);
		m_tgiftlefttimelbl->setString(timestr);
	}
	else
	{
		if (!JhGlobalData::isOnline)
			m_timegiftbtn->setVisible(true);
		else
			m_timegiftbtn->setVisible(false);
	}
	if (JhGlobalData::myRaffleData.isshow && JhGlobalData::isOnline)
	{
		m_rafflebtn->setVisible(true);
		if (m_timegiftbtn->isVisible())
		{
			m_rafflebtn->setPositionY(500);
		}
		else
		{
			m_rafflebtn->setPositionY(665);
		}
	}

	if (JhGlobalData::isRecharge && JhGlobalData::isOnline)
	{
		m_rechargebtn->setVisible(true);
		if (m_timegiftbtn->isVisible() && JhGlobalData::myRaffleData.isshow)
		{
			m_rechargebtn->setPositionY(305);
		}
		else if ((m_timegiftbtn->isVisible() && !JhGlobalData::myRaffleData.isshow) || (!m_timegiftbtn->isVisible() && JhGlobalData::myRaffleData.isshow))
		{
			m_rechargebtn->setPositionY(455);
		}
		else if (!m_timegiftbtn->isVisible() && !JhGlobalData::myRaffleData.isshow)
		{
			m_rechargebtn->setPositionY(620);
		}
	}
	if (JhGlobalData::myLotteryData.isshow && m_lotteryimg != NULL)
	{
		m_herohead->setVisible(true);
		m_lotteryimg->setScale(1.0f);
	}

	//m_prizebtn->setVisible(JhGlobalData::isExchangeGift);

	updateBranchMissionTime();
}

void JhMapLayer::checkAchive(float dt)
{
	bool ishas = false;
	for (unsigned int i = 0; i < JhGlobalData::vec_achiveData.size(); i++)
	{
		if (JhGlobalData::vec_achiveData[i].finish >= JhGlobalData::getAchiveFinishCount(JhGlobalData::vec_achiveData[i]))
		{
			ishas = true;
		}
	}
	if (ishas)
	{
		achiveRedpoint->setVisible(true);
	}
	else
	{
		achiveRedpoint->setVisible(false);
	}
}

void JhMapLayer::showTalkGuide()
{
	if (JhNewerGuide2Layer::checkifNewerGuide(103))
	{
		std::vector<Node*> vec_node;
		JhNewerGuide2Layer *layer = JhNewerGuide2Layer::create(103, vec_node);
		g_gameLayer->addChild(layer, NEWERLAYERZOER, "JhNewerGuide2Layer");
	}
}

void JhMapLayer::updateBranchMissionTime()
{
	std::string curmid = JhGlobalData::getCurBranchPlotMissison();

	if (curmid.length() > 0)
	{
		string str;
		brachmissionicon->setVisible(true);
		cocos2d::ui::Text* timetext = (cocos2d::ui::Text*)brachmissionicon->getChildByName("timetext");
		if (JhGlobalData::map_BranchPlotMissionItem[curmid].time > 0)
			str = StringUtils::format("%02d:%02d", JhGlobalData::map_BranchPlotMissionItem[curmid].time / 60, JhGlobalData::map_BranchPlotMissionItem[curmid].time % 60);
		else
			str = JhCommonFuncs::gbk2utf("不限时");
		timetext->setString(str);

		if (g_gameLayer != NULL && g_gameLayer->getChildByName("JhNewerGuide2Layer") == NULL && g_gameLayer->getChildByName("npclayer") == NULL)
		{
			if (JhNewerGuide2Layer::checkifNewerGuide(104))
			{
				std::vector<Node*> vec_node;
				brachmissionicon->setUserData((void*)"branchmissionbox");
				vec_node.push_back(brachmissionicon);
				JhNewerGuide2Layer *layer = JhNewerGuide2Layer::create(104, vec_node);
				g_gameLayer->addChild(layer, NEWERLAYERZOER, "JhNewerGuide2Layer");
			}
		}
	}
	else
	{
		brachmissionicon->setVisible(false);
		if (g_gameLayer != NULL)
			g_gameLayer->removeChildByName("bmissionlayer");
	}
}

void JhMapLayer::onBranchMisson(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		JhSoundManager::getInstance()->playSound(JhSoundManager::SOUND_ID_BUTTON);
		if (JhGlobalData::getCurBranchPlotMissison().length() > 0)
		{
			JhBranchMissionLayer* layer = JhBranchMissionLayer::create();
			g_gameLayer->addChild(layer, 5, "bmissionlayer");
		}

	}
}

void JhMapLayer::onHelp(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		JhSoundManager::getInstance()->playSound(JhSoundManager::SOUND_ID_BUTTON);
		JhHelpMainLayer* layer = JhHelpMainLayer::create();
		g_gameLayer->addChild(layer, 5);
	}
}

void JhMapLayer::onRecharge(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		JhSoundManager::getInstance()->playSound(JhSoundManager::SOUND_ID_BUTTON);
		JhRechargeLayer* layer = JhRechargeLayer::create();
		g_gameLayer->addChild(layer, 5);
	}
}
