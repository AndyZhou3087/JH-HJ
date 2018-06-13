#include "JhGameScene.h"
#include "JhTopBar.h"
#include "JhUIScroll.h"
#include "JhGameDataSave.h"
#include "JhStorageRoom.h"
#include "JhGlobalData.h"
#include "JhMapLayer.h"
#include "JhCommonFuncs.h"
#include "JhReviveLayer.h"
#include "JhNewerGuideLayer.h"
#include "JhActivitScene.h"
#include "JhConst.h"
#include "JhHeroProperNode.h"
#include "JhMyParticle.h"
#include "JhGetVipRewardLayer.h"
#include "MD5.h"
#include "JhStartScene.h"
#include "JhHintBox.h"
#include "JhWaitingProgress.h"
#include "JhLoginRewardLayer.h"
#include "JhAchiveDoneAnimLayer.h"
#include "JhFrozenLayer.h"
#include "JhCannotTouchLayer.h"

USING_NS_CC;

JhNature* g_nature;
JhHero* g_hero;

JhGameScene* g_gameLayer = NULL;

Sprite* scrollbg;
JhUIScroll* g_uiScroll;
JhGameScene::JhGameScene()
{
	issavedata = true;
	isAnewGetData = false;
	homeLayer = NULL;
}
JhGameScene::~JhGameScene()
{
	//退出保存数据
	if (issavedata)
		saveAllData();

	JhGameDataSave::purgeGameSave();
	g_gameLayer = NULL;
	g_nature = NULL;
	g_hero = NULL;
	issavedata = true;
	isAnewGetData = false;
}

Scene* JhGameScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
	g_gameLayer = JhGameScene::create();

    // add layer as a child to scene
	scene->addChild(g_gameLayer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool JhGameScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }

	JhGlobalData::dataIsModified = false;

	//读取后山资源列表配置文件
	JhGlobalData::loadHillResJsonData();

	//读取地图配置文件
	JhGlobalData::loadMapJsonData();

	//读取NPC配置文件
	JhGlobalData::loadNpcJsonData();

	//读取武功招式配置文件
	JhGlobalData::loadGftrickData();

	//读取技能配置文件
	JhGlobalData::loadGfskillData();

	//读取商城数据
	JhGlobalData::loadShopData();

	//加载武道大会挑战奖励数据
	JhGlobalData::loadChallengeRewardData();

	JhGlobalData::loadTempGF_EquipData();
	JhGlobalData::loadNpcFriendJsonData();
	JhGlobalData::loadNpcMasterJsonData();
	JhGlobalData::loadFriendly();

	//读取成就配置文件
	JhGlobalData::loadAchiveJsonData();

	JhGlobalData::getAchiveData();
	JhGlobalData::getAchiveAnimData();

    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	JhGlobalData::g_gameStatus = GAMESTART;
	//天气，季节数据
	g_nature = JhNature::create();
	this->addChild(g_nature);

	loadSaveNatureData();
	//角色数据
	g_hero = JhHero::create();
	this->addChild(g_hero);

	loadSaveHeroData();

	//读取剧情配置文件
	JhGlobalData::loadPlotMissionJsonData(g_hero->getHeadID());

	//读取支线剧情配置文件
	JhGlobalData::loadBranchPlotMissionJsonData();


	checkRestBranchMisson();

	//滚动文字的背景
	scrollbg = Sprite::create("images/topeventwordbox.png");
	scrollbg->setPosition(Vec2(visibleSize.width / 2, 910));
	this->addChild(scrollbg, 3);

	//滚动文字
	g_uiScroll = JhUIScroll::create(580.0F, 100.0f);
	g_uiScroll->setPosition(Vec2(visibleSize.width / 2 + 5, 910));
	addChild(g_uiScroll, 3);

	//角色目前在哪个地点，第一次进入家
	std::string addrstr = JhGameDataSave::getInstance()->getHeroAddr();
	if (addrstr.compare("m1-1") == 0)//家
	{
		homeLayer = JhHomeLayer::create();
		addChild(homeLayer, 1, "homelayer");
	}
	else//其他地图界面
	{
		g_maplayer = JhMapLayer::create();
		addChild(g_maplayer, 1, "maplayer");
	}

	//任务属性和天气
	topBar = JhTopBar::create();
	topBar->setPosition(Vec2(visibleSize.width/2, 1085));
	addChild(topBar, 2, "topbar");
	
	this->schedule(schedule_selector(JhGameScene::updata), 1.0f);
	this->schedule(schedule_selector(JhGameScene::timerSaveData), 5.0f);
	this->schedule(schedule_selector(JhGameScene::checkiflive), 0.1f);
	this->schedule(schedule_selector(JhGameScene::checkAchiveIsDone), 1.0f);
	
	//处理制暖
	std::string warmConfigStr = JhGlobalData::getMakeWarmConfig();
	if (warmConfigStr.length() > 0)
	{
		std::vector<std::string> vec_retstr;
		JhCommonFuncs::split(warmConfigStr, vec_retstr, "-");
		int startwarmtime = atoi(vec_retstr[0].c_str());
		int warmtime = atoi(vec_retstr[1].c_str());
		int curtimemin = g_nature->getPastDays() * 24 * 60 + g_nature->getTime();
		if (curtimemin - startwarmtime < warmtime)
		{
			g_nature->setIsMaKeWarm(true);
			float wpasttime = curtimemin - startwarmtime;
			g_nature->setWarmtime(warmtime);
			g_nature->setWarmpasttime(wpasttime);
		}
		else
		{
			g_nature->setTemperature(g_nature->getTemperature() - 15);
			JhGlobalData::setMakeWarmConfig("");
		}
	}

	JhGlobalData::doAchive(A_3, JhStorageRoom::getCountById("80"));
	int goldcount = JhGlobalData::getMyGoldCount();
	JhGlobalData::doAchive(A_1, goldcount);

	if (JhGlobalData::isOnline)
	{
		JhServerDataSwap::init(this)->vipIsOn(g_hero->getHeadID());
		JhServerDataSwap::init(NULL)->getFactionList();
	}
    return true;
}

void JhGameScene::loadSaveNatureData()
{
	//设置保存的季节
	g_nature->setReason((EReason)JhGameDataSave::getInstance()->getNatureReason());
	//设置保存的天气
	g_nature->setWeather((EWeather)JhGameDataSave::getInstance()->getNatureWeather());
	
	int cdata = JhGameDataSave::getInstance()->getNatureWeatherChangeCount();
	//设置保存的天气改变的次数
	g_nature->changeWeatherCount = cdata / 10000;
	
	//设置天气改变的随机小时数
	g_nature->changeWeatherRandow = cdata % 10000;

	//设置保存的时间
	g_nature->setTime(JhGameDataSave::getInstance()->getNatureTime());
	//温度
	g_nature->setTemperature(JhGameDataSave::getInstance()->getNatureTemperature());
	//天数
	int livedays = JhGameDataSave::getInstance()->getLiveDays();
	g_nature->setPastDays(livedays);
	JhGlobalData::setMD5LiveDays(md5(livedays));
	
	//设置黑夜或白天

	if (g_nature->getTime() < 60 * 6 - 1 || g_nature->getTime() >= 18 * 60)
	{
		g_nature->setDayOrNight(Night);
	}
	else
	{
		g_nature->setDayOrNight(Day);
	}
}

void JhGameScene::loadSaveHeroData()
{
	//外伤
	g_hero->setOutinjuryValue(JhGameDataSave::getInstance()->getHeroOutinjury());
	//内伤
	g_hero->setInnerinjuryValue(JhGameDataSave::getInstance()->getHeroInnerinjury());
	//饱食度
	g_hero->setHungerValue(JhGameDataSave::getInstance()->getHeroHunger());
	//精神
	g_hero->setSpiritValue(JhGameDataSave::getInstance()->getHeroSpirit());
	//角色ID
	int heroid = JhGameDataSave::getInstance()->getHeroId();
	g_hero->setHeadID(heroid);
	//角色名字
	g_hero->setMyName(JhGlobalData::map_heroAtr[heroid].name);
	//角色等级
	int lv = JhGameDataSave::getInstance()->getHeroLV();
	g_hero->setLVValue(lv);
	//角色经验值
	int exp = JhGameDataSave::getInstance()->getHeroExp();
	g_hero->setExpValue(exp);
	//角色是否在家
	g_hero->setIsOut(JhGameDataSave::getInstance()->getHeroIsOut());

	//角色生命值
	float hlife = JhGameDataSave::getInstance()->getHeroLife();
	if (hlife > -1)//-100 新的角色第一次开始玩
		g_hero->setLifeValue(hlife);
	else
	{
		g_hero->setLifeValue(g_hero->getMaxLifeValue());
	}

	int sex = JhGameDataSave::getInstance()->getHeroSex();
	if (sex < 0)
	{
		if (heroid != 4)
			g_hero->setSex(S_MAN);
		else
			g_hero->setSex(S_WOMEN);
	}
	else
		g_hero->setSex((H_SEX)sex);

	//读取保存的仓库数据
	JhStorageRoom::loadStorageData();
	//读取保存的背包数据
	JhMyPackage::load();
	//读取保存的资源数据
	JhGlobalData::loadResData();
	//读取保存的角色属性数据
	loadSavedHeroPropData();

	getNpcRandMap();
}

void JhGameScene::loadSavedHeroPropData()
{
	//角色佩戴装备
	std::string strval = JhGlobalData::getHeroProperData();
	std::vector<std::string> tmp;
	JhCommonFuncs::split(strval, tmp, ";");
	//解析
	for (unsigned int i = 0; i < tmp.size(); i++)
	{
		std::vector<std::string> tmp2;
		JhCommonFuncs::split(tmp[i], tmp2, "-");

		PackageData sdata;
		sdata.strid = tmp2[0];
		sdata.type = atoi(tmp2[1].c_str());
		sdata.count = atoi(tmp2[2].c_str());
		sdata.extype = JhGlobalData::getResExType(sdata.strid);//atoi(tmp2[3].c_str());
		sdata.lv = atoi(tmp2[4].c_str());
		sdata.exp = atoi(tmp2[5].c_str());
		sdata.goodvalue = atoi(tmp2[6].c_str());//耐久度
		if (tmp2.size() >= 9)
		{
			sdata.slv = atoi(tmp2[7].c_str());//
			sdata.tqu = atoi(tmp2[8].c_str());//
		}

		if (sdata.type == WEAPON || sdata.type == PROTECT_EQU)
		{
			std::vector<PackageData>::iterator it;
			for (it = JhStorageRoom::map_storageData[sdata.type].begin(); it != JhStorageRoom::map_storageData[sdata.type].end();)
			{
				if (it->strid.compare(sdata.strid) == 0 && sdata.count > 0)
				{
					sdata.goodvalue += it->goodvalue;
					if (it->slv < sdata.slv)
						it->slv = sdata.slv;
					if (it->lv < sdata.lv)
						it->lv = sdata.lv;
					it = JhStorageRoom::map_storageData[sdata.type].erase(it);
				}
				else
					++it;
			}
		}
		g_hero->setAtrByType((HeroAtrType)i, sdata);
		//g_hero->set [sdata.type].push_back(sdata);
	}
}

void JhGameScene::saveAllData()
{
	JhGameDataSave::getInstance()->setNatureReason(g_nature->getReason());
	JhGameDataSave::getInstance()->setNatureWeather(g_nature->getWeather());
	JhGameDataSave::getInstance()->setNatureWeatherChangeCount(g_nature->changeWeatherCount * 10000 + g_nature->changeWeatherRandow);
	JhGameDataSave::getInstance()->setNatureTemperature(g_nature->getTemperature());
	JhGameDataSave::getInstance()->setNatureTime(g_nature->getTime());
	JhGameDataSave::getInstance()->setLiveDays(g_nature->getPastDays());
	JhGameDataSave::getInstance()->setHeroLife(g_hero->getLifeValue());
	JhGameDataSave::getInstance()->setHeroOutinjury(g_hero->getOutinjuryValue());
	JhGameDataSave::getInstance()->setHeroInnerinjury(g_hero->getInnerinjuryValue());
	JhGameDataSave::getInstance()->setHeroHunger(g_hero->getHungerValue());
	JhGameDataSave::getInstance()->setHeroSpirit(g_hero->getSpiritValue());
	JhGameDataSave::getInstance()->setHeroLV(g_hero->getLVValue());
	JhGameDataSave::getInstance()->setHeroId(g_hero->getHeadID());
	JhGameDataSave::getInstance()->setHeroExp(g_hero->getExpValue());
	JhGameDataSave::getInstance()->setHeroIsOut(g_hero->getIsOut());
	JhGameDataSave::getInstance()->setHeroSex(g_hero->getSex());

	//保存资源数据
	JhGlobalData::saveResData();

	//保存装备数据
	g_hero->saveProperData();

	std::string curmid = JhGlobalData::getCurBranchPlotMissison();

	if (curmid.length() > 0)
	{
		int subindex = JhGlobalData::map_BranchPlotMissionItem[curmid].subindex;
		PlotMissionData pd = JhGlobalData::map_BranchPlotMissionData[curmid][subindex];
		JhGlobalData::saveBranchPlotMissionStatus(curmid, pd.status);
	}
}

void JhGameScene::onExit()
{
	//saveAllData();
	Layer::onExit();
}

void JhGameScene::saveMyData()
{
	if (g_gameLayer != NULL)
	{
		g_gameLayer->saveAllData();
	}
}

void JhGameScene::changeGameStates(int status)
{
	if (g_gameLayer != NULL)
	{
		GameStatus gs = (GameStatus)status;
		if (gs == GAMEPAUSE)
		{
			if (JhGlobalData::g_gameStatus == GAMESTART)
			{
				JhGlobalData::g_gameStatus = gs;
			}
		}
		else if (gs == GAMESTART)
		{
			if (JhGlobalData::g_gameStatus == GAMEPAUSE)
			{
				JhGlobalData::g_gameStatus = gs;
			}
		}
	}	
}

void JhGameScene::updata(float dt)
{
	JhGlobalData::setTimeGiftLeftTime(JhGlobalData::getTimeGiftLeftTime() - 1);
	if (JhGlobalData::g_gameStatus != GAMESTART || JhGlobalData::isPlayerChallenging)
		return;

	if (g_hero != NULL && g_hero->getIsWDChallenge())
		return;

	//更新资源列表数据
	for (unsigned int i = 0; i < JhGlobalData::vec_hillResid.size(); i++)
	{
		for (unsigned int m = 0; m < JhGlobalData::vec_resData.size(); m++)
		{
			ResData* data = &JhGlobalData::vec_resData[m];
			if (data->strid.compare(JhGlobalData::vec_hillResid[i]) == 0)
			{
				if (data->count <= 0)//采完，等待
				{
					data->pastmin = 0.0f;
					data->waittime += g_nature->getTimeInterval();
					if (data->waittime >= data->speed[g_nature->getReason()] * data->max)
					{
						data->count = data->max;
						data->waittime = 0.0f;
					}
				}
				else
				{
					data->pastmin += g_nature->getTimeInterval();
					if (data->pastmin >= data->speed[g_nature->getReason()])
					{
						int pcount = data->pastmin / data->speed[g_nature->getReason()];
						data->count += pcount;

						float leftmin = data->pastmin - (data->speed[g_nature->getReason()] * pcount);
						data->pastmin = leftmin;
	
					}

					if (data->count >= data->max)//产出最大
					{
						data->pastmin = 0;
						data->count = data->max;
						if (JhGameDataSave::getInstance()->getApprenticeDay() > 0)
						{
							PackageData pdata;
							if (data->res.size() > 0)
							{
								int cres = data->res[0];
								if (cres > 0 && atoi(data->strid.c_str()) != 67 && atoi(data->strid.c_str()) != 68)
								{
									std::string strid = StringUtils::format("%d", cres);
									pdata.strid = strid;
									pdata.type = data->type - 1;
									pdata.count = data->max;
									JhStorageRoom::add(pdata);
									data->count = 0;
									data->waittime = 0.0f;
									std::string str = StringUtils::format("%s%s x%d", JhCommonFuncs::gbk2utf("你的弟子刚刚帮你收集了").c_str(), JhGlobalData::map_allResource[strid].cname.c_str(), data->max);
									g_uiScroll->addEventText(str, 25, Color3B(204, 4, 4));
								}
							}
						}
					}
				}
			}
		}
	}

	std::string curmid = JhGlobalData::getCurBranchPlotMissison();

	if (curmid.length() > 0 && JhGlobalData::map_BranchPlotMissionItem[curmid].count > 0)
	{
		int subindex = JhGlobalData::map_BranchPlotMissionItem[curmid].subindex;
		PlotMissionData *pd = &JhGlobalData::map_BranchPlotMissionData[curmid][subindex];

		if (JhGlobalData::map_BranchPlotMissionItem[curmid].time > 0)
		{
			JhGlobalData::map_BranchPlotMissionItem[curmid].time--;
			if (JhGlobalData::map_BranchPlotMissionItem[curmid].time <= 0)
			{
				JhGlobalData::map_BranchPlotMissionItem[curmid].count--;
				JhGlobalData::map_BranchPlotMissionItem[curmid].time = JhGlobalData::map_BranchPlotMissionItem[curmid].maxtime;
				pd->status = M_NONE;
				JhGlobalData::saveBranchPlotMissionStatus("", 0);
				JhGameDataSave::getInstance()->setBranchPlotMissionGiveGoods("");
			}
		}
	}
}

void JhGameScene::timerSaveData(float dt)
{
	if (issavedata)
		saveAllData();
}

void JhGameScene::checkiflive(float dt)
{
	//生命为0，死掉，弹出复活界面
	if (g_hero != NULL && g_hero->getLifeValue() <= 0.0f && !JhGlobalData::isPopingScene)
	{
		this->unschedule(schedule_selector(JhGameScene::checkiflive));
		topBar->stopLoseAnim();
		JhGlobalData::g_gameStatus = GAMEPAUSE;
		
		if (g_hero->getIsOut() && g_maplayer != NULL)
		{
			g_maplayer->heroPauseMoving();
		}

		JhReviveLayer* layer = JhReviveLayer::create();
		g_gameLayer->addChild(layer, 100, "revivelayer");
	}
}

void JhGameScene::heroRevive()
{
	//复活成功
	g_hero->revive();
	topBar->stopLoseAnim();
	JhGlobalData::g_gameStatus = GAMESTART;
	if (g_maplayer != NULL && g_hero->getIsOut())
		g_maplayer->heroResumeMoving();
	this->schedule(schedule_selector(JhGameScene::checkiflive), 0.1f);
}

void JhGameScene::delayShowOutScence(float dt)
{
	Scene* scene = JhActivitScene::createScene("images/cout.jpg", JhCommonFuncs::gbk2utf("出门..."));
	if (scene != NULL)
	{
		auto transition = TransitionCrossFade::create(0.5f, scene);
		Director::getInstance()->pushScene(transition);
	}
	this->scheduleOnce(schedule_selector(JhGameScene::showGOOut), 0.1f);
}

void JhGameScene::showGOOut(float dt)
{
	g_maplayer = JhMapLayer::create();
	g_gameLayer->addChild(g_maplayer, 1, "maplayer");
	g_gameLayer->removeChildByName("homelayer");
	g_hero->setIsOut(true);
	if (g_maplayer != NULL)
		g_maplayer->scheduleOnce(schedule_selector(JhMapLayer::delayShowMapNewerGuide), 0.2f);
}

void JhGameScene::getNpcRandMap()
{
	int pos = JhGameDataSave::getInstance()->getWxbMapPos();
	JhGlobalData::setWxbMapPos(pos);
	JhGlobalData::map_maps[wxbinmap[pos]].npcs.push_back("n012");

	pos = JhGameDataSave::getInstance()->getDgqbMapPos();
	JhGlobalData::setDgqbMapPos(pos);
	JhGlobalData::map_maps[dgqbinmap[pos]].npcs.push_back("n091");
}

void JhGameScene::showNewerGuide(int step, std::vector<Node*> nodes)
{
	if (JhNewerGuideLayer::checkifNewerGuide(step))
	{
		m_newerStep = step;
		m_newerNode = nodes;
		this->scheduleOnce(schedule_selector(JhGameScene::delayShowNewerGuide),0.05f);
	}
}

void JhGameScene::delayShowNewerGuide(float dt)
{
	if (g_NewerGuideLayer == NULL)
	{
		g_NewerGuideLayer = JhNewerGuideLayer::create(m_newerStep, m_newerNode);
		if (g_gameLayer != NULL)
			g_gameLayer->addChild(g_NewerGuideLayer, 10);
	}
}

void JhGameScene::onSuccess()
{
	if (JhGlobalData::isFrozen)
	{
		if (JhGlobalData::g_gameStatus == GAMESTART)
			JhGlobalData::g_gameStatus = GAMEPAUSE;
		Director::getInstance()->getRunningScene()->addChild(JhFrozenLayer::create(), 10000);
		return;
	}
	if (isAnewGetData)
	{
		isAnewGetData = false;
		this->scheduleOnce(schedule_selector(JhGameScene::delayChangeStartScene), 0.5f);
		return;
	}

	if (JhGlobalData::ispunishment)
	{
		issavedata = false;
		JhGlobalData::ispunishment = false;
		isAnewGetData = true;
		JhWaitingProgress* waitbox = JhWaitingProgress::create("数据异常...");
		Director::getInstance()->getRunningScene()->addChild(waitbox, 1, "waitbox");
		JhServerDataSwap::init(this)->getAllData();
	}
	else
	{
		if (JhGlobalData::vec_buyVipIds.size() > 0)
		{
			JhGetVipRewardLayer* layer = JhGetVipRewardLayer::create();
			if (g_gameLayer != NULL)
				g_gameLayer->addChild(layer, 10, "viprewardlayer");
		}
		else
		{
			if (JhGlobalData::continueLoginDays > 0)
			{
				JhLoginRewardLayer* llayer = JhLoginRewardLayer::create();
				g_gameLayer->addChild(llayer, 100);
			}
			else
			{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
				if (JhGlobalData::updateDownLoadURL.length() > 0 && !JhGlobalData::isPopUpdate)
				{
					JniMethodInfo methodInfo;
					char p_str[256] = { 0 };
					sprintf(p_str, "%s", JhGlobalData::updateDownLoadURL.c_str());
					if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/jh/AppActivity", "updateApk", "(Ljava/lang/String;)V"))
					{
						JhGlobalData::isPopUpdate = true;
						jstring para1 = methodInfo.env->NewStringUTF(p_str);
						methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, para1);
					}
				}
#endif
			}
		}
	}
}

void JhGameScene::onErr(int errcode)
{
	Director::getInstance()->getRunningScene()->removeChildByName("waitbox");
	if (isAnewGetData)
	{
		isAnewGetData = false;
	}
}

void JhGameScene::delayChangeStartScene(float dt)
{

	Scene* scene = JhStartScene::createScene();

	Director::getInstance()->replaceScene(scene);
}

void JhGameScene::checkAchiveIsDone(float dt)
{
	JhGlobalData::vec_showAchiveNames.clear();
	for (unsigned i = 0; i < JhGlobalData::vec_achiveData.size(); i++)
	{
		AchiveData* data = &JhGlobalData::vec_achiveData[i];
		int needcount = JhGlobalData::getAchiveFinishCount(*data);
		if (data->finish >= needcount && data->isshowanim == 0)
		{
			data->isshowanim = 1;
			JhGlobalData::vec_showAchiveNames.push_back(data->name);
		}
	}

	if (JhGlobalData::vec_showAchiveNames.size() > 0)
	{
		this->unschedule(schedule_selector(JhGameScene::checkAchiveIsDone));
		JhAchiveDoneAnimLayer* aalayer = JhAchiveDoneAnimLayer::create();
		Director::getInstance()->getRunningScene()->addChild(aalayer, 100);
	}
}

void JhGameScene::checkRestBranchMisson()
{
	int days = JhGameDataSave::getInstance()->getEnterGameDaysOfYear();

	if (days != JhGlobalData::getDayOfYear())
	{
		JhGameDataSave::getInstance()->setBranchPlotMissionGiveGoods("");
		JhGameDataSave::getInstance()->setBranchPlotMissionStatus("");
		JhGlobalData::loadBranchPlotMissionJsonData();
		JhGameDataSave::getInstance()->setEnterGameDaysOfYear(JhGlobalData::getDayOfYear());
	}
}
