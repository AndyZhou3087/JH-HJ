#include "JhWinlayer.h"
#include "JhGlobalData.h"
#include "JhCommonFuncs.h"
#include "JhMyPackage.h"
#include "JhConst.h"
#include "JhGameDataSave.h"
#include "JhGameScene.h"
#include "JhStorageRoom.h"
#include "JhMapLayer.h"
#include "JhSoundManager.h"
#include "JhNpcLayer.h"
#include "JhHomeHill.h"
#include "JhNewerGuideLayer.h"
#include "JhAnalyticUtil.h"
#include "JhFightLayer.h"

JhWinlayer::JhWinlayer()
{
}


JhWinlayer::~JhWinlayer()
{
	if (g_maplayer != NULL)
		g_maplayer->heroResumeMoving();
}


JhWinlayer* JhWinlayer::create(std::string addrid, std::string npcid)
{
	JhWinlayer *pRet = new JhWinlayer();
	if (pRet && pRet->init(addrid, npcid))
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

bool JhWinlayer::init(std::string addrid, std::string npcid)
{
	Node* csbnode = CSLoader::createNode("jhwinLayer.csb");
	this->addChild(csbnode);

	m_addrid = addrid;
	m_npcid = npcid;

	m_backbtn = (cocos2d::ui::Button*)csbnode->getChildByName("backbtn");
	m_backbtn->addTouchEventListener(CC_CALLBACK_2(JhWinlayer::onBack, this));

	m_getallbtn = (cocos2d::ui::Button*)csbnode->getChildByName("allgetbtn");
	m_getallbtn->addTouchEventListener(CC_CALLBACK_2(JhWinlayer::onAllGet, this));

	cocos2d::ui::Button * continuebtn = (cocos2d::ui::Button*)csbnode->getChildByName("continuebtn");
	continuebtn->addTouchEventListener(CC_CALLBACK_2(JhWinlayer::onContinue, this));

	cocos2d::ui::Text* addrname = (cocos2d::ui::Text*)csbnode->getChildByName("title");
	addrname->setString(JhGlobalData::map_maps[m_addrid].cname);

	cocos2d::ui::ImageView* heroimg = (cocos2d::ui::ImageView*)csbnode->getChildByName("heroimg");
	std::string heroidstr = StringUtils::format("ui/tophero%d.png", g_hero->getHeadID());
	heroimg->loadTexture(heroidstr, cocos2d::ui::TextureResType::PLIST);

	cocos2d::ui::Text* m_heroname = (cocos2d::ui::Text*)csbnode->getChildByName("heroname");
	std::string str = heroname[g_hero->getHeadID() - 1];
	m_heroname->setString(str);

	loadFightCount();
	cocos2d::ui::TextBMFont* herobigatk = (cocos2d::ui::TextBMFont*)csbnode->getChildByName("herobigatk");
	herobigatk->setString(StringUtils::format("%d", bFightCount));
	cocos2d::ui::TextBMFont* heroatk = (cocos2d::ui::TextBMFont*)csbnode->getChildByName("heroatk");
	heroatk->setString(StringUtils::format("%d", sFightCount));
	if (g_hero->getTotalAtck() < 10)
	{
		heroatk->setVisible(false);
	}

	cocos2d::ui::ImageView* npcimg = (cocos2d::ui::ImageView*)csbnode->getChildByName("npcimg");
	std::string npcstr = StringUtils::format("ui/%s.png", m_npcid.c_str());
	npcimg->loadTexture(npcstr, cocos2d::ui::TextureResType::PLIST);

	cocos2d::ui::Text* npcname = (cocos2d::ui::Text*)csbnode->getChildByName("npcname");
	npcname->setString(JhGlobalData::map_npcs[m_npcid].name);

	loadNpcFightCount();
	cocos2d::ui::TextBMFont* npcbigatk = (cocos2d::ui::TextBMFont*)csbnode->getChildByName("npcbigatk");
	npcbigatk->setString(StringUtils::format("%d", bnpcFightCount));
	cocos2d::ui::TextBMFont* npcatk = (cocos2d::ui::TextBMFont*)csbnode->getChildByName("npcatk");
	npcatk->setString(StringUtils::format("%d", snpcFightCount));
	if (JhGlobalData::map_npcs[m_npcid].atk < 10)
	{
		npcatk->setVisible(false);
	}
	

	if (m_npcid.compare("n001") == 0)//在路上碰到山贼
		addrname->setString(JhCommonFuncs::gbk2utf("路上"));

	explbl = (cocos2d::ui::Text*)csbnode->getChildByName("explbl");
	std::string lblstr = StringUtils::format("+%d", addHeroExp());
	explbl->setString(lblstr);
	gfexplbl = (cocos2d::ui::Text*)csbnode->getChildByName("gfexplbl");
	if (g_hero->getAtrByType(H_WG)->count > 0 || g_hero->getAtrByType(H_NG)->count > 0)
	{

		lblstr = StringUtils::format("+%d", addGfExp());
		gfexplbl->setString(lblstr);
	}
	else
	{
		gfexplbl->setVisible(false);
		csbnode->getChildByName("gftext")->setVisible(false);
	}

	loadTempData();

	if (m_addrid.compare("m1-6") == 0)
		JhGlobalData::map_maps["m1-6"].npcs.clear();

	std::vector<std::string> winres = JhGlobalData::map_npcs[npcid].winres;
	std::vector<int> winresrnd = JhGlobalData::map_npcs[npcid].winresrnd;

	int curplot = JhGlobalData::getPlotMissionIndex();
	PlotMissionData * plotdata = NULL;
	int plottype = 0;

	std::string fnpc = m_npcid;
	if (JhGlobalData::map_maps[m_addrid].npcs.size() >= 10)
		fnpc = JhGlobalData::map_maps[m_addrid].npcs[0];

	if (m_addrid.compare("m13-1") != 0)
	{
		if (JhGlobalData::vec_PlotMissionData[curplot].dnpc.compare(fnpc) == 0 && JhGlobalData::vec_PlotMissionData[curplot].type == 1 && JhGlobalData::vec_PlotMissionData[curplot].status == M_DOING)
		{
			plotdata = &JhGlobalData::vec_PlotMissionData[curplot];
			plottype = 0;
		}
		else
		{
			std::string mid = JhGlobalData::getCurBranchPlotMissison();
			if (mid.length() > 0)
			{
				int subindex = JhGlobalData::map_BranchPlotMissionItem[mid].subindex;
				if (JhGlobalData::map_BranchPlotMissionData[mid][subindex].dnpc.compare(m_npcid) == 0 && JhGlobalData::map_BranchPlotMissionData[mid][subindex].type == 1 && JhGlobalData::map_BranchPlotMissionItem[mid].count > 0)
					plotdata = &JhGlobalData::map_BranchPlotMissionData[mid][subindex];
				plottype = 1;
			}
			//else
			//{
			//	std::map<std::string, std::vector<PlotMissionData>>::iterator it;
			//	for (it = JhGlobalData::map_BranchPlotMissionData.begin(); it != JhGlobalData::map_BranchPlotMissionData.end(); it++)
			//	{
			//		if (JhGlobalData::map_BranchPlotMissionData[it->first].size() > 0)
			//		{
			//			PlotMissionData* pmdata = &JhGlobalData::map_BranchPlotMissionData[it->first][0];
			//			if (pmdata->snpc.compare(npcid) == 0 && pmdata->unlockchapter <= JhGlobalData::getUnlockChapter() && JhGlobalData::map_BranchPlotMissionItem[pmdata->id].count > 0)
			//			{
			//				plotdata = pmdata;
			//				plottype = 1;
			//				break;
			//			}
			//		}
			//	}
			//}
		}
	}

	if (plotdata != NULL)
	{
		plotdata->status = M_DONE;

		winres = plotdata->rewords;
		winresrnd.clear();
		for (unsigned int i = 0; i < winres.size(); i++)
		{
			winresrnd.push_back(100);
		}
		int unlockchapter = 0;
		if (plottype == 0)
		{
			unlockchapter = plotdata->unlockchapter;
			JhGlobalData::setUnlockChapter(unlockchapter);
			JhGlobalData::setPlotMissionIndex(curplot + 1);
			JhGlobalData::savePlotMissionStatus();

			for (unsigned int i = 0; i < JhGlobalData::vec_achiveData.size(); i++)
			{
				if (JhGlobalData::vec_achiveData[i].type == A_6)
				{
					if (plotdata->id.compare(JhGlobalData::vec_achiveData[i].vec_para[0]) == 0 && JhGlobalData::vec_achiveData[i].finish != -1)
					{
						JhGlobalData::vec_achiveData[i].finish = 1;
						JhGlobalData::saveAchiveData();
					}
				}
			}
		}
		else
		{
			int subindex = JhGlobalData::map_BranchPlotMissionItem[plotdata->id].subindex;
			JhGlobalData::map_BranchPlotMissionData[plotdata->id][subindex].status = M_NONE;
			if (subindex + 1 >= JhGlobalData::map_BranchPlotMissionData[plotdata->id].size())
			{
				JhGlobalData::map_BranchPlotMissionItem[plotdata->id].subindex = 0;
				JhGlobalData::map_BranchPlotMissionItem[plotdata->id].count--;
				JhGlobalData::map_BranchPlotMissionItem[plotdata->id].time = JhGlobalData::map_BranchPlotMissionItem[plotdata->id].maxtime;
				JhGlobalData::saveBranchPlotMissionStatus("", 0);
			}
			else
			{
				JhGlobalData::map_BranchPlotMissionItem[plotdata->id].subindex++;
				JhGlobalData::saveBranchPlotMissionStatus(plotdata->id, M_NONE);
			}

			showMissionAnim(this, "任务完成", winres);
		}

		if (g_gameLayer != NULL)
		{
			JhNpcLayer * npclayer = (JhNpcLayer*)g_gameLayer->getChildByName("npclayer");
			if (npclayer != NULL && plottype == 0)
			{
				npclayer->updatePlotUI(plottype);
			}
		}

		if (g_maplayer != NULL && plottype == 0)
		{
			g_maplayer->updataPlotMissionIcon(plottype);
			if (unlockchapter > 0  && unlockchapter <= MAXCHAPTER)
				g_maplayer->scheduleOnce(schedule_selector(JhMapLayer::showUnlockLayer), 1.0f);

			if (plotdata->dnpc.compare("n089") == 0)
			{
				g_maplayer->scheduleOnce(schedule_selector(JhMapLayer::showEndAnim), 1.5f);
			}
		}
	}

	bool iswd = false;
	if (m_addrid.compare("m13-1") == 0)
	{
		m_backbtn->setTitleText(JhCommonFuncs::gbk2utf("退出挑战"));
		m_backbtn->setTitleFontSize(30);
		int npcsize = JhGlobalData::map_maps[m_addrid].npcs.size();
		if (npcid.compare(JhGlobalData::map_maps[m_addrid].npcs[npcsize - 1]) == 0)
		{
			continuebtn->setVisible(false);
			m_backbtn->setTitleText(JhCommonFuncs::gbk2utf("完成"));
		}

		winres = JhGlobalData::map_challengeReward[npcid].vec_winres;
		winresrnd.clear();
		for (unsigned int i = 0; i < winres.size(); i++)
		{
			int intrnd = JhGlobalData::map_challengeReward[npcid].vec_winrnd[i] * 10;
			winresrnd.push_back(intrnd);
		}
		iswd = true;
	}
	else
	{
		if (m_addrid.compare("m1-6") == 0)
			continuebtn->setVisible(false);
		else if (m_addrid.compare("m1-2") == 0)
		{
			for (unsigned int i = 0; i < JhGlobalData::vec_resData.size(); i++)
			{
				ResData *data = &JhGlobalData::vec_resData[i];
				if ((m_npcid.compare("n002") == 0 && data->strid.compare("67") == 0 && data->count <= 0) || (m_npcid.compare("n003") == 0 && data->strid.compare("68") == 0 && data->count <= 0))
				{
					continuebtn->setEnabled(false);
					break;
				}
			}
		}
		else
		{
			for (unsigned int i = 0; i < JhGlobalData::vec_achiveData.size(); i++)
			{
				if (JhGlobalData::vec_achiveData[i].type == A_4)
				{
					if (JhGlobalData::vec_achiveData[i].vec_para[0].compare(m_npcid) == 0)
					{
						if (JhGlobalData::vec_achiveData[i].finish != -1)
						{
							JhGlobalData::vec_achiveData[i].finish = 1;
							JhGlobalData::saveAchiveData();
						}
					}
				}
			}
			
		}
	}

	int winressize = winres.size();
	if (winressize <= 0)
		m_getallbtn->setEnabled(false);

	for (int i = 0; i < winressize; i++)
	{
		int res = atoi(winres[i].c_str());

		int r = 0;
		int winrnd = winresrnd[i];

		if (!iswd)
		{
			if (winrnd < 10)
			{
				if (JhGlobalData::map_npcs[npcid].winrescount[i] < 0)
				{
					r = JhGlobalData::createRandomNum(100) + 1;
					JhGlobalData::map_npcs[npcid].winrescount[i] = 1;
				}
				else
				{
					JhGlobalData::map_npcs[npcid].winrescount[i]++;
					if (JhGlobalData::map_npcs[npcid].winrescount[i] < 3)
						r = 200;
					else
					{
						r = JhGlobalData::createRandomNum(100) + 1;
						JhGlobalData::map_npcs[npcid].winrescount[i] = 1;
					}
				}
			}
			else
			{
				r = JhGlobalData::createRandomNum(100) + 1;
			}
		}
		else
		{
			r = JhGlobalData::createRandomNum(1000) + 1;
		}

		if (r <= winrnd)
		{			
			if (res != 0)
			{
				PackageData data;
				bool isfind = false;
				std::string strid = StringUtils::format("%d", res / 1000);
				data.strid = strid;
				data.count = res % 1000;

				for (unsigned int i = 0; i < JhGlobalData::vec_resData.size(); i++)
				{
					ResData rdata = JhGlobalData::vec_resData[i];
					if (atoi(rdata.strid.c_str()) == res / 1000)
					{
						isfind = true;
						data.type = rdata.type - 1;
						break;
					}
				}

				if (!isfind)
				{
					std::map<std::string, std::vector<BuildActionData>>::iterator it;
					for (it = JhGlobalData::map_buidACData.begin(); it != JhGlobalData::map_buidACData.end(); ++it)
					{
						std::vector<BuildActionData> vec_bactData = JhGlobalData::map_buidACData[it->first];

						for (unsigned int m = 0; m < vec_bactData.size(); m++)
						{
							BuildActionData bdata = vec_bactData[m];
							if (atoi(bdata.icon) == res / 1000)
							{
								isfind = true;
								data.strid = bdata.icon;
								data.count = res % 1000;
								data.type = bdata.type - 1;
								data.extype = bdata.extype;
								break;
							}
						}
						if (isfind)
							break;
					}
				}
				getRewardData.push_back(data);
			}
			else
			{
				PackageData data;
				std::string strid = winres[i];
				data.strid = strid;
				data.count = 1;

				bool isfind = false;
				std::map<std::string, WG_NGData>::iterator it;
				for (it = JhGlobalData::map_wgngs.begin(); it != JhGlobalData::map_wgngs.end(); ++it)
				{
					WG_NGData gfdata = JhGlobalData::map_wgngs[it->first];
					if (winres[i].compare(gfdata.id) == 0 && !g_hero->checkifHasGF_Equip(winres[i]) && JhGlobalData::tempHasGf_Equip(winres[i]).length() <= 0)
					{
						isfind = true;
						data.strid = gfdata.id;
						data.count = 1;
						data.lv = 0;
						data.type = gfdata.type - 1;
						data.extype = gfdata.extype;
						getRewardData.push_back(data);
						break;
					}
				}

				if (!isfind)
				{
					std::map<std::string, EquipData>::iterator ite;
					for (ite = JhGlobalData::map_equips.begin(); ite != JhGlobalData::map_equips.end(); ++ite)
					{
						EquipData edata = JhGlobalData::map_equips[ite->first];
						if (winres[i].compare(edata.id) == 0 && !g_hero->checkifHasGF_Equip(winres[i]) && JhGlobalData::tempHasGf_Equip(winres[i]).length() <= 0)
						{
							data.strid = edata.id;
							data.count = 1;
							data.type = edata.type - 1;
							data.goodvalue = 100;
							data.extype = edata.extype;
							getRewardData.push_back(data);
							break;
						}
					}
				}
			}
		}
	}

	updata();
	updataLV();

	if (JhGlobalData::isFightMaster)
	{
		JhGlobalData::map_myfriendly[m_npcid].relation = F_MASTEROUT;
		JhGlobalData::saveFriendly();
		JhGlobalData::isFightMaster = false;
		JhNpcLayer* npclayer = (JhNpcLayer*)g_gameLayer->getChildByName("npclayer");
		if (npclayer != NULL)
			npclayer->reFreshRelationUI();
		std::string desc = StringUtils::format("%s%s%s", JhGlobalData::map_npcs[m_npcid].name, JhCommonFuncs::gbk2utf("：青出于蓝而胜于蓝。").c_str(), JhCommonFuncs::gbk2utf("为师没什么能教你了！").c_str());
		g_uiScroll->addEventText(desc, 25, Color3B(204, 4, 4));
	}
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [=](Touch *touch, Event *event)
	{
		return true;
	};

	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	this->scheduleOnce(schedule_selector(JhWinlayer::delayShowNewerGuide), 0.2f);

	if (JhGlobalData::getUnlockChapter() >= MAXCHAPTER)
	{
		JhGlobalData::doAchive(A_5, JhGlobalData::getUnlockChapter());
	}

	for (unsigned int i = 0; i < JhGlobalData::vec_achiveData.size(); i++)
	{
		if (JhGlobalData::vec_achiveData[i].type == A_7)
		{
			std::string astr = JhGlobalData::vec_achiveData[i].vec_para[0];
			PackageData* mePackageData = g_hero->getMeHas(astr);
			if (mePackageData != NULL && JhGlobalData::vec_achiveData[i].finish != -1)
			{
				JhGlobalData::vec_achiveData[i].finish = mePackageData->lv + 1;
				JhGlobalData::saveAchiveData();
			}
		}

		if (JhGlobalData::vec_achiveData[i].type == A_10)
		{
			if (JhGlobalData::vec_achiveData[i].finish != -1)
			{
				int nlv = atoi(JhGlobalData::vec_achiveData[i].vec_para[0].c_str());
				JhGlobalData::vec_achiveData[i].finish = g_hero->getGfCountByLv(nlv);
				JhGlobalData::saveAchiveData();
			}
		}
	}
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	if (JhGlobalData::isOnline)
		JhServerDataSwap::init(NULL)->postOneData(JhGlobalData::getUId());
#endif
#ifdef ANALYTICS
	if (m_npcid.compare("n089") == 0)
		JhAnalyticUtil::onEvent("allpass");
#endif
	//n044

	if (npcid.compare("n044") == 0 && JhGlobalData::isExchangeGift && m_addrid.compare("m13-1") != 0 && JhGlobalData::isOnline)
		JhServerDataSwap::init(this)->getKajuanAction();

	return true;
}

void JhWinlayer::onEnterTransitionDidFinish()
{
	Layer::onEnterTransitionDidFinish();
}

void JhWinlayer::loadNpcFightCount()
{
	int curack = JhGlobalData::map_npcs[m_npcid].atk;
	for (int i = 6; i >= 0; i--)
	{
		int pf = pow(10, i);
		if (curack / pf > 0)
		{
			bnpcFightCount = curack / pf;
			snpcFightCount = curack % pf;
			break;
		}
	}
}

void JhWinlayer::loadFightCount()
{
	int curack = g_hero->getTotalAtck();
	for (int i = 6; i >= 0; i--)
	{
		int pf = pow(10, i);
		if (curack / pf > 0)
		{
			bFightCount = curack / pf;
			sFightCount = curack % pf;
			break;
		}
	}
}

void JhWinlayer::updataLV()
{

	g_hero->setExpValue(g_hero->getExpValue() + addHeroExp());
	int curlv = g_hero->getLVValue();
	unsigned int i = 0;
	int lv = 0;
	std::vector<int> vec_heroExp = JhGlobalData::map_heroAtr[g_hero->getHeadID()].vec_exp;
	for (i = curlv; i < vec_heroExp.size(); i++)
	{
		if (g_hero->getExpValue() >= vec_heroExp[i])
		{
			lv = i + 1;
			g_hero->setExpValue(g_hero->getExpValue() - vec_heroExp[i]);
		}
	}
	if (lv > curlv)
	{
		int heromaxlv = vec_heroExp.size();
		if (lv >= heromaxlv)
		{
			g_hero->setExpValue(vec_heroExp[heromaxlv - 1]);
			lv = heromaxlv - 1;
			g_hero->setLVValue(lv);
		}
		else
		{
			g_hero->setLVValue(lv);
			g_hero->setLifeValue(g_hero->getMaxLifeValue());
			vec_lvup.push_back("herolvup");
		}
		JhGlobalData::doAchive(A_2, lv + 1);
	}
	else
	{
		if (g_hero->getLVValue() == vec_heroExp.size() - 1)
		{
			JhGlobalData::doAchive(A_2, g_hero->getLVValue() + 1);
		}
	}
	for (int m = H_WG; m <= H_NG; m++)
	{
		lv = 0;
		PackageData* gfData = g_hero->getAtrByType((HeroAtrType)m);
		if (gfData->count > 0)
		{
			std::string gfname = gfData->strid;
			std::vector<int> vec_gfExp = JhGlobalData::map_wgngs[gfname].vec_exp;
			curlv = gfData->lv;

			gfData->exp += addGfExp();
			for (i = curlv; i < vec_gfExp.size(); i++)
			{
				if (gfData->exp >= vec_gfExp[i])
				{
					lv = i + 1;
					gfData->exp = gfData->exp - vec_gfExp[i];
				}
			}
			int gfmaxlv = JhGlobalData::map_wgngs[gfname].maxlv;
			if (lv > curlv)
			{
				if (lv >= gfmaxlv)
					gfData->lv = gfmaxlv - 1;
				else
				{
					gfData->lv = lv;
					if (m == H_WG)
						vec_lvup.push_back("wglvup");
					else
						vec_lvup.push_back("nglvup");
				}
			}
		}
	}

	if (vec_lvup.size() > 0)
		showLvUpAnim(0);

}

void JhWinlayer::onRewardItem(cocos2d::Ref* pSender)
{
	JhSoundManager::getInstance()->playSound(JhSoundManager::SOUND_ID_BUTTON);
	Node* node = (Node*)pSender;
	PackageData* data = (PackageData*)node->getUserData();
	int size = getRewardData.size();

	int count = data->count - 1;
	if (count <= 0)
	{
		std::vector<PackageData>::iterator it;
		for (it = getRewardData.begin(); it != getRewardData.end(); ++it)
		{
			if (it->strid.compare(data->strid) == 0)
			{
				PackageData pdata = *data;
				pdata.count = 1;
				if (JhMyPackage::add(pdata) == 0)
				{
					data->count--;
					getRewardData.erase(it);
				}
				break;
			}
		}
	}
	else
	{
		PackageData pdata = *data;
		pdata.count = 1;
		if (JhMyPackage::add(pdata) == 0)
		{
			data->count--;
		}
	}
	for (int i = 0; i < size; i++)
	{
		std::string name = StringUtils::format("resitem%d", i);
		this->removeChildByName(name);
	}
	saveTempData();
	updata();
	showNewerGuide(35);
}

void JhWinlayer::onPackageItem(cocos2d::Ref* pSender)
{
	JhSoundManager::getInstance()->playSound(JhSoundManager::SOUND_ID_BUTTON);
	Node* node = (Node*)pSender;
	int index = node->getTag();
	PackageData data = JhMyPackage::vec_packages[index];
	data.count = 1;
	unsigned int i = 0;
	for (i = 0; i < getRewardData.size(); i++)
	{
		if (data.strid.compare(getRewardData[i].strid) == 0 && (getRewardData[i].type == FOOD || getRewardData[i].type == MEDICINAL || getRewardData[i].type == RES_1 || getRewardData[i].type == RES_2))
		{
			getRewardData[i].count++;
			break;
		}
	}

	if (i == getRewardData.size())
	{
		getRewardData.push_back(data);
	}
	saveTempData();

	JhMyPackage::cutone(data);

	for (unsigned int i = 0; i < getRewardData.size(); i++)
	{
		std::string name = StringUtils::format("resitem%d", i);
		this->removeChildByName(name);
	}

	updata();
}

void JhWinlayer::onBack(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		JhHomeHill* homehill = (JhHomeHill*)g_gameLayer->getChildByName("homehill");
		if (homehill != NULL)
		{
			if (JhNewerGuideLayer::checkifNewerGuide(36))
				homehill->showNewerGuide(36);
			else if (JhNewerGuideLayer::checkifNewerGuide(39))
				homehill->showNewerGuide(39);
		}
		JhFightLayer* fightlayer = (JhFightLayer*)g_gameLayer->getChildByName("fightlayer");
		if (fightlayer != NULL)
			fightlayer->removeFromParentAndCleanup(true);

		JhNpcLayer * npclayer = (JhNpcLayer*)g_gameLayer->getChildByName("npclayer");
		if (npclayer != NULL)
		{
			npclayer->showTalkGuider();
		}

		this->removeFromParentAndCleanup(true);
	}
}

void JhWinlayer::onContinue(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		JhFightLayer* fightlayer = (JhFightLayer*)g_gameLayer->getChildByName("fightlayer");
		if (fightlayer != NULL)
		{
			if (m_addrid.compare("m13-1") == 0)
				fightlayer->continueChallenge();
			else
			{
				for (unsigned int i = 0; i < JhGlobalData::vec_resData.size(); i++)
				{
					ResData *data = &JhGlobalData::vec_resData[i];
					if ((m_npcid.compare("n002") == 0 && data->strid.compare("67") == 0 && data->count > 0) || (m_npcid.compare("n003") == 0 && data->strid.compare("68") == 0 && data->count > 0))
					{
						data->count--;
						break;
					}
				}
				fightlayer->restartFightNpc(m_npcid);
			}
		}

		this->removeFromParentAndCleanup(true);
	}
}

void JhWinlayer::onAllGet(cocos2d::Ref *pSender, cocos2d::ui::Widget::TouchEventType type)
{
	JhCommonFuncs::BtnAction(pSender, type);
	if (type == ui::Widget::TouchEventType::ENDED)
	{
		for (unsigned int i = 0; i < getRewardData.size(); i++)
		{
			std::string name = StringUtils::format("resitem%d", i);
			this->removeChildByName(name);
		}
		std::vector<PackageData>::iterator it;
		bool isfull = false;
		for (it = getRewardData.begin(); it != getRewardData.end();)
		{
			bool isOver = false;
			int count = it->count;
			for (int m = 0; m < count; m++)
			{
				PackageData data = *it;
				data.count = 1;
				if (JhMyPackage::add(data) == 0)
				{
					if (--it->count <= 0)
					{
						it = getRewardData.erase(it);
						isOver = true;
						break;
					}
				}
				else
				{
					isfull = true;
					break;
				}
			}
			if (!isOver)
				it++;
			if (isfull)
				break;
		}
		saveTempData();
		updata();
		showNewerGuide(38);
	}
}


void JhWinlayer::loadTempData()
{
	tempResData.clear();
	std::string datastr = JhGameDataSave::getInstance()->getTempStorage(m_addrid);
	std::vector<std::string> vec_retstr;
	JhCommonFuncs::split(datastr, vec_retstr, ";");
	for (unsigned int i = 0; i < vec_retstr.size(); i++)
	{
		std::vector<std::string> tmp;
		JhCommonFuncs::split(vec_retstr[i], tmp, "-");
		PackageData data;
		data.strid = tmp[0];
		data.type = atoi(tmp[1].c_str());
		data.count = atoi(tmp[2].c_str());
		data.extype = atoi(tmp[3].c_str());
		data.lv = atoi(tmp[4].c_str());
		data.exp = atoi(tmp[5].c_str());
		data.goodvalue = atoi(tmp[6].c_str());
		if (tmp.size() >= 9)
		{
			data.slv = atoi(tmp[7].c_str());
			data.tqu = atoi(tmp[8].c_str());
		}
		tempResData.push_back(data);
	}
}

void JhWinlayer::saveTempData()
{
	if (m_npcid.compare("n001") == 0)//在路上碰到山贼不保存物品
		return;
	std::vector<PackageData> allResData = tempResData;

	for (unsigned int i = 0; i < getRewardData.size(); i++)
	{
		int tmpsize = tempResData.size();
		int j = 0;
		for (j = 0; j < tmpsize; j++)
		{
			if (getRewardData[i].strid.compare(tempResData[j].strid) == 0 && (getRewardData[i].type == FOOD || getRewardData[i].type == MEDICINAL || getRewardData[i].type == RES_1 || getRewardData[i].type == RES_2))
			{
				allResData[j].count += getRewardData[i].count;
				break;
			}
		}
		if (j == tmpsize)
		{
			allResData.push_back(getRewardData[i]);
		}
	}
	JhGlobalData::map_tempGf_Equip[m_addrid].clear();

	std::string str;
	for (unsigned int i = 0; i < allResData.size(); i++)
	{
		std::string onestr = StringUtils::format("%s-%d-%d-%d-%d-%d-%d-%d-%d;", allResData[i].strid.c_str(), allResData[i].type, allResData[i].count, allResData[i].extype, allResData[i].lv, allResData[i].exp, allResData[i].goodvalue, allResData[i].slv, allResData[i].tqu);
		str.append(onestr);

		int tmptype = allResData[i].type;
		std::string tmpstrid = allResData[i].strid;
		if (tmptype == W_GONG || tmptype == N_GONG || tmptype == WEAPON || tmptype == PROTECT_EQU)
		{
			JhGlobalData::map_tempGf_Equip[m_addrid].push_back(tmpstrid);
		}
	}
	JhGameDataSave::getInstance()->setTempStorage(m_addrid, str.substr(0, str.length() - 1));
}

void JhWinlayer::updata()
{
	//更新奖励栏
	updataRewardUI();
	//更新背包栏
	updataMyPackageUI();
}

void JhWinlayer::updataMyPackageUI()
{
	for (int i = 0; i < JhMyPackage::getMax(); i++)
	{
		std::string name = StringUtils::format("pitem%d", i);
		this->removeChildByName(name);
	}

	for (int i = 0; i < JhMyPackage::getSize(); i++)
	{
		std::string boxstr = "ui/winbox.png";
		/*PackageData tmpdata = JhMyPackage::vec_packages[i];
		if (tmpdata.type == WEAPON || tmpdata.type == PROTECT_EQU)
		{
			boxstr = StringUtils::format("ui/qubox%d.png", JhGlobalData::map_equips[tmpdata.strid].qu);
		}
		else if (tmpdata.type == N_GONG || tmpdata.type == W_GONG)
		{
			boxstr = StringUtils::format("ui/qubox%d.png", JhGlobalData::map_wgngs[tmpdata.strid].qu);
		}*/

		Sprite * box = Sprite::createWithSpriteFrameName(boxstr);

		MenuItemSprite* boxItem = MenuItemSprite::create(
			box,
			box,
			box,
			CC_CALLBACK_1(JhWinlayer::onPackageItem, this));
		boxItem->setTag(i);
		boxItem->setPosition(Vec2(110 + i * 125, 200));
		Menu* menu = Menu::create();
		menu->addChild(boxItem);
		menu->setPosition(Vec2(0, 0));
		std::string name = StringUtils::format("pitem%d", i);
		this->addChild(menu, 0, name);

		std::string str = StringUtils::format("ui/%s.png", JhMyPackage::vec_packages[i].strid.c_str());
		Sprite * res = Sprite::createWithSpriteFrameName(str);
		res->setPosition(Vec2(box->getContentSize().width / 2, box->getContentSize().height / 2));
		box->addChild(res);
		str = StringUtils::format("%d", JhMyPackage::vec_packages[i].count);
		Label * reslbl = Label::createWithTTF(str, "fonts/SIMHEI.TTF", 22);//Label::createWithSystemFont(str, "", 18);
		reslbl->setPosition(Vec2(box->getContentSize().width - 25, 5));
		box->addChild(reslbl);
	}
}

void JhWinlayer::updataRewardUI()
{
	for (unsigned int i = 0; i < getRewardData.size(); i++)
	{
		std::string boxstr = "ui/winbox.png";
		PackageData tmpdata = getRewardData[i];
		/*if (tmpdata.type == WEAPON || tmpdata.type == PROTECT_EQU)
		{
			boxstr = StringUtils::format("ui/qubox%d.png", JhGlobalData::map_equips[tmpdata.strid].qu);
		}
		else if (tmpdata.type == N_GONG || tmpdata.type == W_GONG)
		{
			boxstr = StringUtils::format("ui/qubox%d.png", JhGlobalData::map_wgngs[tmpdata.strid].qu);
		}*/

		Sprite * box = Sprite::createWithSpriteFrameName(boxstr);

		MenuItemSprite* boxItem = MenuItemSprite::create(
			box,
			box,
			box,
			CC_CALLBACK_1(JhWinlayer::onRewardItem, this));
		boxItem->setTag(i);
		boxItem->setUserData(&getRewardData[i]);
		boxItem->setPosition(Vec2(150 + i * 135, 400));
		Menu* menu = Menu::create();
		menu->addChild(boxItem);
		menu->setPosition(Vec2(0, 0));
		std::string name = StringUtils::format("resitem%d", i);
		this->addChild(menu, 0, name);

		std::string str = StringUtils::format("ui/%s.png", getRewardData[i].strid.c_str());
		Sprite * res = Sprite::createWithSpriteFrameName(str);
		res->setPosition(Vec2(box->getContentSize().width / 2, box->getContentSize().height / 2));
		box->addChild(res);

		str = StringUtils::format("%d", getRewardData[i].count);
		Label * reslbl = Label::createWithTTF(str, "fonts/SIMHEI.TTF", 22);//Label::createWithSystemFont(str, "", 18);
		reslbl->setPosition(Vec2(box->getContentSize().width - 25, 5));
		box->addChild(reslbl);
	}
}

void JhWinlayer::onExit()
{
	saveTempData();
	Layer::onExit();
}

void JhWinlayer::showLvUpAnim(float dt)
{
	this->removeChildByName("lvanim");
	if (vec_lvup.size() <= 0)
	{
		return;
	}
	//Sprite* lvUpSprite = Sprite::createWithSpriteFrameName("ui/herolvuptext.png");
	//lvUpSprite->setPosition(Vec2(360, 400));
	//this->addChild(lvUpSprite);
	//lvUpSprite->runAction(Spawn::create(MoveTo::create(3.0f, Vec2(360, 700)), FadeOut::create(3.0f), NULL));

	Node* csbnode = CSLoader::createNode("jhlvupanim.csb");
	csbnode->setPosition(Vec2(360, 640));
	this->addChild(csbnode, 0, "lvanim");
	cocos2d::ui::ImageView* textimg = (cocos2d::ui::ImageView*)csbnode->getChildByName("textimg");
	std::string imgstr = StringUtils::format("lvuppic/%s.png", vec_lvup[0].c_str());
	textimg->loadTexture(imgstr, cocos2d::ui::Widget::TextureResType::LOCAL);
	auto action = CSLoader::createTimeline("jhlvupanim.csb");
	csbnode->runAction(action);
	action->gotoFrameAndPlay(0, false);

	vec_lvup.erase(vec_lvup.begin());
	float delaytime = action->getEndFrame()*1.0f / (action->getTimeSpeed() * 60);
	this->schedule(schedule_selector(JhWinlayer::showLvUpAnim), delaytime + 1.5f, vec_lvup.size(), 0);
}

void JhWinlayer::showNewerGuide(int step)
{
	std::vector<Node*> nodes;
	if (step == 34)
	{
		Node* resnode = this->getChildByName("resitem0");
		if (resnode != NULL)
			nodes.push_back(resnode->getChildren().at(0));
	}
	else if (step == 35 || step == 38)
	{
		nodes.push_back(m_backbtn);
	}
	else if (step == 37)
	{
		nodes.push_back(m_getallbtn);
	}
	if (nodes.size() > 0)
		g_gameLayer->showNewerGuide(step, nodes);
}


void JhWinlayer::delayShowNewerGuide(float dt)
{
	if (JhNewerGuideLayer::checkifNewerGuide(34))
		showNewerGuide(34);
	else if (JhNewerGuideLayer::checkifNewerGuide(37))
		showNewerGuide(37);
}

int JhWinlayer::addHeroExp()
{
	int winexp = JhGlobalData::map_npcs[m_npcid].exp;
	int herolv = g_hero->getLVValue() + 1;
	int bosslv = JhGlobalData::map_npcs[m_npcid].lv;
	int lvless = herolv - bosslv;
	if (lvless > 30)
		winexp = winexp * 0.1f;
	else if (lvless > 20)
		winexp = winexp * 0.3f;
	else if (lvless > 10)
		winexp = winexp * 0.5f;

	int heorexpetime = JhGameDataSave::getInstance()->getHeroExpEndTime();
	if (JhGlobalData::getSysSecTime() <= heorexpetime)
	{
		winexp *= 2;
	}

	return winexp;
}
int JhWinlayer::addGfExp()
{
	int winexp = JhGlobalData::map_npcs[m_npcid].exp;
	int herolv = g_hero->getLVValue() + 1;
	int bosslv = JhGlobalData::map_npcs[m_npcid].lv;
	int lvless = herolv - bosslv;

	float gfwinexp = JhGlobalData::map_npcs[m_npcid].exp * 3.0f / 2.0f;
	if (lvless > 10)
		gfwinexp = gfwinexp * 0.5f;

	int gfetime = JhGameDataSave::getInstance()->getGfEndTime();
	if (JhGlobalData::getSysSecTime() <= gfetime)
	{
		gfwinexp *= 2;
	}

	return (int)gfwinexp;
}

void JhWinlayer::onSuccess()
{
	std::vector<std::string> res;

	showMissionAnim(this, "挑战扫地僧成功", res);
}

void JhWinlayer::onErr(int errcode)
{
}

void JhWinlayer::showMissionAnim(Node* _target, std::string text, std::vector<std::string> vec_res)
{
	if (_target != NULL)
	{
		LayerColor* color = LayerColor::create(Color4B(11, 32, 22, 150));
		_target->addChild(color, 10, "missionanimlayer");

		auto listener = EventListenerTouchOneByOne::create();
		listener->onTouchBegan = [=](Touch *touch, Event *event)
		{
			return true;
		};
		listener->setSwallowTouches(true);
		color->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, color);

		Node* csbnode = CSLoader::createNode("jhachiveNodeAnim.csb");
		csbnode->setPosition(Vec2(360, 800));
		csbnode->getChildByName("cjz_1")->setVisible(false);
		color->addChild(csbnode);
		cocos2d::ui::Text* textname = (cocos2d::ui::Text*)csbnode->getChildByName("name");
		textname->setString(JhCommonFuncs::gbk2utf(text.c_str()));
		auto action = CSLoader::createTimeline("jhachiveNodeAnim.csb");
		csbnode->runAction(action);
		action->gotoFrameAndPlay(0, false);
		csbnode->getChildByName("light")->runAction(RepeatForever::create(RotateTo::create(8, 720)));

		if (vec_res.size() > 0)
		{
			std::string resstr = JhCommonFuncs::gbk2utf("获得任务奖励：");
			for (unsigned int i = 0; i < vec_res.size(); i++)
			{
				std::string resid = vec_res[i];
				int intres = atoi(resid.c_str());
				int count = 1;
				if (intres != 0)
				{
					resid = StringUtils::format("%d", intres / 1000);
					count = intres % 1000;
				}

				resstr.append(StringUtils::format("%sx%d", JhGlobalData::map_allResource[resid].cname.c_str(), count));
				resstr.append(JhCommonFuncs::gbk2utf("，"));
			}
			Label * reslbl = Label::createWithTTF(resstr.substr(0, resstr.length()-3), "fonts/STXINGKA.TTF", 32);
			reslbl->setMaxLineWidth(650);
			reslbl->setPosition(Vec2(0, -200));
			reslbl->setColor(Color3B(255, 255, 255));
			csbnode->addChild(reslbl);
		}
		_target->runAction(Sequence::create(DelayTime::create(2.3f), CallFunc::create(CC_CALLBACK_0(JhWinlayer::removeMissionAnim, _target)), NULL));
	}
}

void JhWinlayer::removeMissionAnim(Ref* pSender)
{
	Node* _target = (Node*)pSender;
	if (_target != NULL)
		_target->removeChildByName("missionanimlayer");
}
