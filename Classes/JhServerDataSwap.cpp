#include "JhServerDataSwap.h"
#include "JhGlobalData.h"
#include "JhHttpUtil.h"
#include "json.h"
#include "JhGameDataSave.h"
#include "JhCommonFuncs.h"
#include "JhHero.h"
#include "JhGameScene.h"

#define HTTPURL "https://www.stormnet.cn/api/"

bool JhServerDataSwap::isdoing = false;

JhServerDataSwap::JhServerDataSwap()
{
	m_pDelegateProtocol = NULL;
}

JhServerDataSwap::~JhServerDataSwap()
{
	m_pDelegateProtocol = NULL;
	isdoing = false;
}
JhServerDataSwap::JhServerDataSwap(ServerDataDelegateProtocol *pDelegateProtocol)
{
	m_pDelegateProtocol = pDelegateProtocol;
	isdoing = true;
}
bool JhServerDataSwap::isGetingData()
{
	return isdoing;
}

JhServerDataSwap* JhServerDataSwap::init(ServerDataDelegateProtocol* delegateProtocol)
{
	JhServerDataSwap *serverDataSwap = new  JhServerDataSwap(delegateProtocol);

	return serverDataSwap;
}

void JhServerDataSwap::setDelegate(ServerDataDelegateProtocol *delegateProtocol)
{
	m_pDelegateProtocol = delegateProtocol;
}

void JhServerDataSwap::release()
{
	delete this;
}

void JhServerDataSwap::postAllData()
{
	std::vector<std::string> vec_ids = JhGlobalData::getSaveListId();

	std::vector<std::string> vec_userid;
	for (unsigned int i = 0; i < vec_ids.size(); i++)
	{
		std::string saveuid = vec_ids[i];
		if (saveuid.length() > 0)
		{
			vec_userid.push_back(saveuid);
		}
	}

	int tag = 0;
	for (unsigned int i = 0; i < vec_userid.size(); i++)
	{
		if (i == vec_userid.size() - 1)
			tag = 1;
		postOneData(vec_userid[i], tag);
	}
}

void JhServerDataSwap::postOneData(std::string userid, int tag)
{
    isdoing = false;
	std::string postdata;

	rapidjson::Document writedoc;
	writedoc.SetObject();
	rapidjson::Document::AllocatorType& allocator = writedoc.GetAllocator();

	writedoc.AddMember("playerid", rapidjson::Value(JhGlobalData::UUID().c_str(), allocator), allocator);
	writedoc.AddMember("localid", rapidjson::Value(userid.c_str(), allocator), allocator);
	writedoc.AddMember("nickname", rapidjson::Value(JhGlobalData::getMyNickName().c_str(), allocator), allocator);

	int herounlock = 1;
	std::string str = JhGameDataSave::getInstance()->getHeroUnlockData();
	std::vector<std::string> tmp;
	JhCommonFuncs::split(str, tmp, "-");

	for (unsigned int i = 1; i < tmp.size(); i++)
	{
		int val = atoi(tmp[i].c_str());
		herounlock += val << i;
	}

	writedoc.AddMember("hunlock", herounlock, allocator);
	int coin = JhGameDataSave::getInstance()->getGoldCount();
	int usecoin = JhGameDataSave::getInstance()->getUseGold();
	writedoc.AddMember("coin", coin, allocator);
	writedoc.AddMember("costcoin", usecoin, allocator);
	JhGameDataSave::getInstance()->setUserId(userid);

	int type = JhGameDataSave::getInstance()->getHeroId();
	int lv = JhGameDataSave::getInstance()->getHeroLV();
	int exp = JhGameDataSave::getInstance()->getHeroExp();
	for (int i = 0; i <= lv; i++)
	{
		exp += JhGlobalData::map_heroAtr[type].vec_exp[i];
	}
	writedoc.AddMember("type", type, allocator);
	writedoc.AddMember("exp", exp, allocator);
	int hungry = JhGameDataSave::getInstance()->getHeroHunger();
	int innerhurt = JhGameDataSave::getInstance()->getHeroInnerinjury();
	int outerhurt = JhGameDataSave::getInstance()->getHeroOutinjury();
	int life = JhGameDataSave::getInstance()->getHeroLife();
	int days = JhGameDataSave::getInstance()->getLiveDays();
	int sprite = JhGameDataSave::getInstance()->getHeroSpirit();
	int plotindex = JhGameDataSave::getInstance()->getPlotMissionIndex();
	std::string bpotstr = JhGameDataSave::getInstance()->getBranchPlotMissionStatus();
	int unlock = JhGameDataSave::getInstance()->getPlotUnlockChapter();
	int sex = JhGameDataSave::getInstance()->getHeroSex();

	if (sex < 0)
	{
		if (type != 4)
			sex = S_MAN;
		else
			sex = S_WOMEN;
	}

	writedoc.AddMember("hungry", hungry, allocator);
	writedoc.AddMember("innerhurt", innerhurt, allocator);
	writedoc.AddMember("outerhurt", outerhurt, allocator);
	writedoc.AddMember("life", life, allocator);
	writedoc.AddMember("days", days, allocator);
	writedoc.AddMember("mood", sprite, allocator);
	writedoc.AddMember("task", plotindex, allocator);
	writedoc.AddMember("newbtask", rapidjson::Value(bpotstr.c_str(), allocator), allocator);
	writedoc.AddMember("unlock", unlock, allocator);
	writedoc.AddMember("sex", sex, allocator);

	std::string friendshipstr = JhGameDataSave::getInstance()->getFriendly();
	writedoc.AddMember("friendship", rapidjson::Value(friendshipstr.c_str(), allocator), allocator);

	std::string mixgfstr = JhGameDataSave::getInstance()->getMixGF();
	writedoc.AddMember("mixgf", rapidjson::Value(mixgfstr.c_str(), allocator), allocator);

	int cheat = JhGlobalData::dataIsModified?1:0;
	writedoc.AddMember("cheat", cheat, allocator);
	std::string achivestr = JhGameDataSave::getInstance()->getAchiveData();
	writedoc.AddMember("achievement", rapidjson::Value(achivestr.c_str(), allocator), allocator);
	JhGlobalData::dataIsModified = false;

	int fightingpower = 0;

	if (g_hero != NULL && !g_hero->getIsWDChallenge())
	{
		fightingpower = g_hero->getMaxLifeValue() + g_hero->getTotalDf() * 20 + g_hero->getTotalAtck() * 10 + g_hero->getCritRate() * 100 + g_hero->getdodgeRate() * 100;
	}
	if (fightingpower > 0)
	{
		if (fightingpower >= 1000000)
			fightingpower = 30000;
		writedoc.AddMember("fightingpower", fightingpower, allocator);
	}

	rapidjson::Document doc = ReadJsonFile("data/buildings.json");
	rapidjson::Value& allBuilds = doc["b"];
	for (unsigned int i = 0; i < allBuilds.Size(); i++)
	{
		rapidjson::Value& oneBuild = allBuilds[i];
		rapidjson::Value& oneitem = oneBuild["name"];
		std::string buildname = oneitem.GetString();
		int blv = JhGameDataSave::getInstance()->getBuildLV(buildname);
        std::string blvstr = StringUtils::format("%d", blv);
		writedoc.AddMember(rapidjson::Value(buildname.c_str(), allocator), rapidjson::Value(blvstr.c_str(), allocator), allocator);
	}
	
	rapidjson::Value dataArray(rapidjson::kArrayType);

	std::string strval[] = { JhGameDataSave::getInstance()->getStorageData(), JhGameDataSave::getInstance()->getPackage(), JhGameDataSave::getInstance()->getHeroProperData() };

	for (int m = 0; m < sizeof(strval) / sizeof(strval[0]); m++)
	{
		rapidjson::Value object(rapidjson::kObjectType);
		object.AddMember("flag", m + 1, allocator);

		std::vector<std::string> tmp;
		JhCommonFuncs::split(strval[m], tmp, ";");

		for (unsigned int i = 0; i < tmp.size(); i++)
		{
			std::vector<std::string> tmp2;
			JhCommonFuncs::split(tmp[i], tmp2, "-");

			std::string strid = tmp2[0];
			if (strid.length() <= 0)
				continue;

			int type = atoi(tmp2[1].c_str());
			int count = atoi(tmp2[2].c_str());
			int lv = atoi(tmp2[4].c_str());
			int goodvalue = atoi(tmp2[6].c_str());

			int slv = 0;
			if (tmp2.size() >= 9)
			{
				slv = atoi(tmp2[7].c_str());
			}

			if (strid.length() > 0 && count > 0)
			{
				int val = 0;
				std::string jsonkey;

				if (atoi(strid.c_str()) != 0)
				{
					val = count;
					jsonkey = StringUtils::format("r%s", strid.c_str());
				}
				else
				{
					jsonkey = strid;
					if (type == WEAPON || type == PROTECT_EQU)
					{
						val = slv * 1000 + goodvalue + 1;
					}
					else
					{
						val = lv + 1;
					}
				}
                std::string valstr = StringUtils::format("%d", val);
				object.AddMember(rapidjson::Value(jsonkey.c_str(), allocator), rapidjson::Value(valstr.c_str(), allocator), allocator);
			}
		}
		dataArray.PushBack(object, allocator);
	}

	writedoc.AddMember("data", dataArray, allocator);
	postdata = JsonWriter(writedoc);

	std::string url;
	url.append(HTTPURL);
	url.append("wx_savealldata");
	std::string tagstr = StringUtils::format("%d", tag);
	JhHttpUtil::getInstance()->doData(url, httputil_calback(JhServerDataSwap::httpPostOneDataCB, this), "", POST, postdata, tagstr);
}

void JhServerDataSwap::getAllData()
{
	std::string url;
	url.append(HTTPURL);
	url.append("wx_getalldata?");
	url.append("playerid=");
	url.append(JhGlobalData::UUID());
	JhHttpUtil::getInstance()->doData(url, httputil_calback(JhServerDataSwap::httpGetAllDataCB, this));
}

void JhServerDataSwap::propadjust()
{
	std::string url;
	url.append(HTTPURL);
	url.append("wx_propadjust?");
	url.append("playerid=");
	url.append(JhGlobalData::UUID());
	JhHttpUtil::getInstance()->doData(url, httputil_calback(JhServerDataSwap::httpPropadJustDataCB, this));
	
}

void JhServerDataSwap::modifyNickName(std::string nickname)
{
	std::string url;
	url.append(HTTPURL);
	url.append("wx_updateusername?");
	url.append("playerid=");
	url.append(JhGlobalData::UUID());
	url.append("&name=");
	url.append(nickname);
	JhHttpUtil::getInstance()->doData(url, httputil_calback(JhServerDataSwap::httpModifyNickNameCB, this));
}

void JhServerDataSwap::vipSuccNotice(std::string gid)
{
	std::string url;
	url.append(HTTPURL);
	url.append("wx_buynotify?");
	url.append("playerid=");
	url.append(JhGlobalData::UUID());
	url.append("&goodsid=");
	url.append(gid);
	JhHttpUtil::getInstance()->doData(url, httputil_calback(JhServerDataSwap::httpBlankCB, this));
}

void JhServerDataSwap::vipIsOn(int heroid)
{
	std::string url;
	url.append(HTTPURL);
	url.append("wx_takemonthlycard?");
	url.append("playerid=");
	url.append(JhGlobalData::UUID());
	url.append("&type=");
	std::string herostr = StringUtils::format("%d", heroid);
	url.append(herostr);
	url.append("&vercode=");
	url.append(JhGlobalData::getVersion());
	JhHttpUtil::getInstance()->doData(url, httputil_calback(JhServerDataSwap::httpVipIsOnCB, this));
}

void JhServerDataSwap::isGetVip(std::vector<std::string> vipids)
{
	std::string url;
	url.append(HTTPURL);
	url.append("wx_donemonthlycard?");
	url.append("playerid=");
	url.append(JhGlobalData::UUID());
	std::string strvip;
	for (unsigned int i = 0; i < vipids.size(); i++)
	{
		std::string str = StringUtils::format("&%s=1", vipids[i].c_str());
		strvip.append(str);
	}
	url.append(strvip);
	JhHttpUtil::getInstance()->doData(url, httputil_calback(JhServerDataSwap::httpIsGetVipCB, this));
}

void JhServerDataSwap::updateFreeReviveCount()
{
	std::string url;
	url.append(HTTPURL);
	url.append("wx_usefreelife?");
	url.append("playerid=");
	url.append(JhGlobalData::UUID());
	JhHttpUtil::getInstance()->doData(url, httputil_calback(JhServerDataSwap::httpBlankCB, this));
}

void JhServerDataSwap::getServerTime()
{
	std::string url;
	url.append(HTTPURL);
	url.append("wx_getservertime");
	JhHttpUtil::getInstance()->doData(url, httputil_calback(JhServerDataSwap::httpGetServerTimeCB, this));
}

void JhServerDataSwap::getRankData(std::string orderby)
{
	std::string url;
	url.append(HTTPURL);
	url.append("wx_ranklist?");
	url.append("playerid=");
	url.append(JhGlobalData::UUID());

	url.append("&type=");
	std::string typestr = StringUtils::format("%d", g_hero->getHeadID());
	url.append(typestr);

	url.append("&");
	url.append(orderby);
	JhHttpUtil::getInstance()->doData(url, httputil_calback(JhServerDataSwap::httpGetRankDataCB, this));
}

void JhServerDataSwap::getannouncement()
{
	std::string url;
	url.append(HTTPURL);
	url.append("wx_getannouncement?");
	url.append("playerid=");
	url.append(JhGlobalData::UUID());
	url.append("&vercode=");
	url.append(JhGlobalData::getVersion());
	JhHttpUtil::getInstance()->doData(url, httputil_calback(JhServerDataSwap::httpGetAnnouncementCB, this));
}

void JhServerDataSwap::createFaciton(std::string name, int lvlimit, int sexlimit, std::string desc)
{
	std::string url;
	url.append(HTTPURL);
	url.append("wx_buildfaction?");
	url.append("playerid=");
	url.append(JhGlobalData::UUID());
	url.append("&nickname=");
	url.append(name);

	url.append("&type=");
	std::string typestr = StringUtils::format("%d", g_hero->getHeadID());
	url.append(typestr);

	url.append("&levellower=");
	std::string str = StringUtils::format("%d",lvlimit);
	url.append(str);
	url.append("&sex=");
	str = StringUtils::format("%d", sexlimit);
	url.append(str);
	url.append("&remark=");
	url.append(desc);

	JhHttpUtil::getInstance()->doData(url, httputil_calback(JhServerDataSwap::httpCreateFactionCB, this));
}

void JhServerDataSwap::modifyFaciton(int factionid, std::string name, int lvlimit, int sexlimit, std::string desc)
{
	std::string url;
	url.append(HTTPURL);
	url.append("wx_updatefaction?");
	url.append("playerid=");
	url.append(JhGlobalData::UUID());
	url.append("&nickname=");
	url.append(name);

	std::string fidstr = StringUtils::format("%d", factionid);
	url.append("&factionid=");
	url.append(fidstr);

	url.append("&levellower=");
	std::string str = StringUtils::format("%d", lvlimit);
	url.append(str);
	url.append("&sex=");
	str = StringUtils::format("%d", sexlimit);
	url.append(str);
	url.append("&remark=");
	url.append(desc);

	JhHttpUtil::getInstance()->doData(url, httputil_calback(JhServerDataSwap::httpModifyFactionCB, this));
}

void JhServerDataSwap::getFactionList()
{
	std::string url;
	url.append(HTTPURL);
	url.append("wx_factionlist?");
	url.append("playerid=");
	url.append(JhGlobalData::UUID());
	url.append("&type=");
	std::string typestr = StringUtils::format("%d", g_hero->getHeadID());
	url.append(typestr);

	JhHttpUtil::getInstance()->doData(url, httputil_calback(JhServerDataSwap::httpGetFactionListCB, this));
}

void JhServerDataSwap::requestFaction(int factionid)
{
	std::string url;
	url.append(HTTPURL);
	url.append("wx_requestfaction?");
	url.append("playerid=");
	url.append(JhGlobalData::UUID());
	url.append("&factionid=");
	std::string factionidstr = StringUtils::format("%d", factionid);
	url.append(factionidstr);

	url.append("&type=");
	std::string typestr = StringUtils::format("%d", g_hero->getHeadID());
	url.append(typestr);

	JhHttpUtil::getInstance()->doData(url, httputil_calback(JhServerDataSwap::httpRequestFactionListCB, this));
}

void JhServerDataSwap::cancelFaction(int factionid)
{
	std::string url;
	url.append(HTTPURL);
	url.append("wx_cancelfaction?");
	url.append("playerid=");
	url.append(JhGlobalData::UUID());
	url.append("&factionid=");
	std::string factionidstr = StringUtils::format("%d", factionid);
	url.append(factionidstr);

	url.append("&type=");
	std::string typestr = StringUtils::format("%d", g_hero->getHeadID());
	url.append(typestr);

	JhHttpUtil::getInstance()->doData(url, httputil_calback(JhServerDataSwap::httpCancelFactionCB, this));
}

void JhServerDataSwap::getFactionMembers(int factionid)
{
	std::string url;
	url.append(HTTPURL);
	url.append("wx_factionmemberlist?");
	url.append("playerid=");
	url.append(JhGlobalData::UUID());
	url.append("&factionid=");
	std::string factionidstr = StringUtils::format("%d", factionid);
	url.append(factionidstr);

	url.append("&type=");
	std::string herotypestr = StringUtils::format("%d", g_hero->getHeadID());
	url.append(herotypestr);

	JhHttpUtil::getInstance()->doData(url, httputil_calback(JhServerDataSwap::httpGetFactionMemberCB, this));
}

void JhServerDataSwap::joinFaction(int factionid, int requesterId, int requestertype)
{
	std::string url;
	url.append(HTTPURL);
	url.append("wx_joinfaction?");
	url.append("playerid=");
	url.append(JhGlobalData::UUID());
	url.append("&factionid=");
	std::string factionidstr = StringUtils::format("%d", factionid);
	url.append(factionidstr);

	url.append("&requester=");
	std::string requesterstr = StringUtils::format("%d", requesterId);
	url.append(requesterstr);

	url.append("&type=");
	requesterstr = StringUtils::format("%d", requestertype);
	url.append(requesterstr);

	JhHttpUtil::getInstance()->doData(url, httputil_calback(JhServerDataSwap::httpJionFactionCB, this));
}

void JhServerDataSwap::kickFaction(int factionid, int requesterId, int requestertype)
{
	std::string url;
	url.append(HTTPURL);
	url.append("wx_kickfaction?");
	url.append("playerid=");
	url.append(JhGlobalData::UUID());
	url.append("&factionid=");
	std::string factionidstr = StringUtils::format("%d", factionid);
	url.append(factionidstr);

	url.append("&requester=");
	std::string requesterstr = StringUtils::format("%d", requesterId);
	url.append(requesterstr);

	url.append("&type=");
	requesterstr = StringUtils::format("%d", requestertype);
	url.append(requesterstr);

	JhHttpUtil::getInstance()->doData(url, httputil_calback(JhServerDataSwap::httpKickFactionCB, this));
}

void JhServerDataSwap::promotionFaction(int factionid, int requesterId, int requestertype, int position)
{
	std::string url;
	url.append(HTTPURL);
	url.append("wx_promotionfaction?");
	url.append("playerid=");
	url.append(JhGlobalData::UUID());
	url.append("&factionid=");
	std::string factionidstr = StringUtils::format("%d", factionid);
	url.append(factionidstr);

	url.append("&requester=");
	std::string requesterstr = StringUtils::format("%d", requesterId);
	url.append(requesterstr);

	url.append("&type=");
	requesterstr = StringUtils::format("%d", requestertype);
	url.append(requesterstr);

	url.append("&title=");
	requesterstr = StringUtils::format("%d", position);
	url.append(requesterstr);

	JhHttpUtil::getInstance()->doData(url, httputil_calback(JhServerDataSwap::httpPromotionFactionCB, this));
}

void JhServerDataSwap::leaveFaction(int actiontype, int factionid, int herotype)
{
	std::string url;
	url.append(HTTPURL);
	if (actiontype == 0)
		url.append("wx_leavefaction?");
	else
		url.append("wx_releasefaction?");
	url.append("playerid=");
	url.append(JhGlobalData::UUID());

	url.append("&factionid=");
	std::string factionidstr = StringUtils::format("%d", factionid);
	url.append(factionidstr);

	url.append("&type=");
	std::string requesterstr = StringUtils::format("%d", herotype);
	url.append(requesterstr);

	JhHttpUtil::getInstance()->doData(url, httputil_calback(JhServerDataSwap::httpLeaveFactionCB, this));
}

void JhServerDataSwap::contributionFaction(int factionid, int contribution, int herotype)
{
	std::string url;
	url.append(HTTPURL);

	url.append("wx_contributionfaction?");
	url.append("playerid=");
	url.append(JhGlobalData::UUID());

	url.append("&factionid=");
	std::string factionidstr = StringUtils::format("%d", factionid);
	url.append(factionidstr);

	url.append("&contribution=");
	std::string contributionstr = StringUtils::format("%d", contribution);
	url.append(contributionstr);

	url.append("&type=");
	std::string requesterstr = StringUtils::format("%d", herotype);
	url.append(requesterstr);

	JhHttpUtil::getInstance()->doData(url, httputil_calback(JhServerDataSwap::httpContributionFactionCB, this));
}

void JhServerDataSwap::refuseFaction(int factionid, int requesterId, int requestertype)
{
	std::string url;
	url.append(HTTPURL);
	url.append("wx_refusefaction?");
	url.append("playerid=");
	url.append(JhGlobalData::UUID());
	url.append("&factionid=");
	std::string factionidstr = StringUtils::format("%d", factionid);
	url.append(factionidstr);

	url.append("&requester=");
	std::string requesterstr = StringUtils::format("%d", requesterId);
	url.append(requesterstr);

	url.append("&type=");
	requesterstr = StringUtils::format("%d", requestertype);
	url.append(requesterstr);

	JhHttpUtil::getInstance()->doData(url, httputil_calback(JhServerDataSwap::httpRefuseFactionCB, this));
}

void JhServerDataSwap::getlotteryData(int actiontype)
{
	std::string url;
	url.append(HTTPURL);
	url.append("wx_lottery?");
	url.append("playerid=");
	url.append(JhGlobalData::UUID());

	url.append("&type=");
	std::string str = StringUtils::format("%d", actiontype);
	url.append(str);

	JhHttpUtil::getInstance()->doData(url, httputil_calback(JhServerDataSwap::httpLotteryCB, this));
}

void JhServerDataSwap::getCoinpoolData()
{
	std::string url;
	url.append(HTTPURL);
	url.append("wx_getcoinpool?");
	url.append("playerid=");
	url.append(JhGlobalData::UUID());
	JhHttpUtil::getInstance()->doData(url, httputil_calback(JhServerDataSwap::httpGetCoinpoolCB, this));
}

void JhServerDataSwap::playCoinpoolData()
{
	std::string url;
	url.append(HTTPURL);
	url.append("wx_playcoinpool?");
	url.append("playerid=");
	url.append(JhGlobalData::UUID());
	JhHttpUtil::getInstance()->doData(url, httputil_calback(JhServerDataSwap::httpPlayCoinpoolCB, this));
}

void JhServerDataSwap::getCoupons(std::string codestr)
{
	std::string url;
	url.append(HTTPURL);
	url.append("wx_coupons?");
	url.append("playerid=");
	url.append(JhGlobalData::UUID());
	url.append("&code=");
	url.append(codestr);
	JhHttpUtil::getInstance()->doData(url, httputil_calback(JhServerDataSwap::httpGetCouponsCB, this));
}

void JhServerDataSwap::getChallengeranklist()
{
	std::string url;
	url.append(HTTPURL);
	url.append("wx_challengeranklist?");
	url.append("playerid=");
	url.append(JhGlobalData::UUID());
	url.append("&type=");
	std::string str = StringUtils::format("%d", g_hero->getHeadID());
	url.append(str);
	JhHttpUtil::getInstance()->doData(url, httputil_calback(JhServerDataSwap::httpGetChallengeranklistCB, this));
}

void JhServerDataSwap::getMyFihgterData(std::string fightplayerid, int fightplayertype)
{
	std::string url;
	url.append(HTTPURL);
	url.append("wx_challengeinfo?");
	url.append("playerid=");
	url.append(fightplayerid);
	url.append("&type=");
	std::string str = StringUtils::format("%d", fightplayertype);
	url.append(str);
	JhHttpUtil::getInstance()->doData(url, httputil_calback(JhServerDataSwap::httpGetMyFihgterDataCB, this));
}

void JhServerDataSwap::getFightCount(int matchtype, int count)
{
	std::string url;
	url.append(HTTPURL);
	url.append("wx_addchallengecount?");
	url.append("playerid=");
	url.append(JhGlobalData::UUID());
	url.append("&type=");
	std::string str = StringUtils::format("%d", g_hero->getHeadID());
	url.append(str);
	url.append("&count=");
	str = StringUtils::format("%d", count);
	url.append(str);
	url.append("&matchtype=");
	str = StringUtils::format("%d", matchtype);
	url.append(str);
	JhHttpUtil::getInstance()->doData(url, httputil_calback(JhServerDataSwap::httpGetFightCountCB, this));
}

void JhServerDataSwap::getChallengeResult(int myrank, std::string fightplayerid, int fightplayertype, int fightrank, int win)
{
	std::string url;
	url.append(HTTPURL);
	url.append("wx_challengeresult?");
	url.append("playerid=");
	url.append(JhGlobalData::UUID());
	url.append("&type=");
	std::string str = StringUtils::format("%d", g_hero->getHeadID());
	url.append(str);
	url.append("&rank=");
	str = StringUtils::format("%d", myrank);
	url.append(str);

	url.append("&accepterplayerid=");
	url.append(fightplayerid);

	url.append("&acceptertype=");
	str = StringUtils::format("%d", fightplayertype);
	url.append(str);

	url.append("&accepterrank=");
	str = StringUtils::format("%d", fightrank);
	url.append(str);

	url.append("&win=");
	str = StringUtils::format("%d", win);
	url.append(str);

	JhHttpUtil::getInstance()->doData(url, httputil_calback(JhServerDataSwap::httpGetChallengeResultCB, this));
}

void JhServerDataSwap::getKajuanAction()
{
	std::string url;
	url.append(HTTPURL);
	url.append("wx_kajuanevent?");
	url.append("playerid=");
	url.append(JhGlobalData::UUID());
	url.append("&type=");
	std::string str = StringUtils::format("%d", g_hero->getHeadID());
	url.append(str);
	JhHttpUtil::getInstance()->doData(url, httputil_calback(JhServerDataSwap::httpGetKajuanActionCB, this));
}

void JhServerDataSwap::getKajuanAwardList()
{
	std::string url;
	url.append(HTTPURL);
	url.append("wx_kajuaneventawardlist?");
	url.append("playerid=");
	url.append(JhGlobalData::UUID());
	url.append("&type=");
	std::string str = StringUtils::format("%d", g_hero->getHeadID());
	url.append(str);
	JhHttpUtil::getInstance()->doData(url, httputil_calback(JhServerDataSwap::httpGetKajuanAwardListCB, this));
}

void JhServerDataSwap::getMyMatchInfo()
{
	std::string url;
	url.append(HTTPURL);
	url.append("wx_matchmatchselfinfo?");
	url.append("playerid=");
	url.append(JhGlobalData::UUID());
	JhHttpUtil::getInstance()->doData(url, httputil_calback(JhServerDataSwap::httpGetMyMatchInfoCB, this));
}

void JhServerDataSwap::getMatchFight()
{
	std::string url;
	url.append(HTTPURL);
	url.append("wx_matchmatchinfo?");
	url.append("playerid=");
	url.append(JhGlobalData::UUID());
	JhHttpUtil::getInstance()->doData(url, httputil_calback(JhServerDataSwap::httpGetMatchFightCB, this));
}

void JhServerDataSwap::getMatchFightResult(std::string fightplayerid, int score)
{
	std::string url;
	url.append(HTTPURL);
	url.append("wx_matchmatchresult?");
	url.append("playerid=");
	url.append(JhGlobalData::UUID());

	url.append("&matchplayerid=");
	url.append(fightplayerid);

	url.append("&score=");
	std::string str = StringUtils::format("%d", score);
	url.append(str);
	JhHttpUtil::getInstance()->doData(url, httputil_calback(JhServerDataSwap::httpGetMatchFightResultCB, this));
}

void JhServerDataSwap::getHSLJRankData()
{
	std::string url;
	url.append(HTTPURL);
	url.append("wx_matchmatchranklist?");
	url.append("playerid=");
	url.append(JhGlobalData::UUID());
	JhHttpUtil::getInstance()->doData(url, httputil_calback(JhServerDataSwap::httpGetHSLJRankDataCB, this));
}

void JhServerDataSwap::getHSLJRewardData()
{
	std::string url;
	url.append(HTTPURL);
	url.append("wx_matchmatchgetaward?");
	url.append("playerid=");
	url.append(JhGlobalData::UUID());
	JhHttpUtil::getInstance()->doData(url, httputil_calback(JhServerDataSwap::httpGetHSLJRewardDataCB, this));
	
}

void JhServerDataSwap::getCommonData()
{
	std::string url;
	url.append(HTTPURL);
	url.append("wx_qq?");
	url.append("pkg=");
	url.append(JhGlobalData::getPackageName());
	url.append("&vercode=");
	url.append(JhGlobalData::getVersion());
	url.append("&playerid=");
	url.append(JhGlobalData::UUID());
	url.append("&cid=");
	url.append(JhGlobalData::getChannelId());
	JhHttpUtil::getInstance()->doData(url, httputil_calback(JhServerDataSwap::httpGetCommonDataCB, this));
}

void JhServerDataSwap::getRechargeData()
{
	std::string url;
	url.append(HTTPURL);
	url.append("wx_topupevent?");
	url.append("playerid=");
	url.append(JhGlobalData::UUID());
	JhHttpUtil::getInstance()->doData(url, httputil_calback(JhServerDataSwap::httpGetRechargeDataCB, this));
}

void JhServerDataSwap::postMyRecharge(int amount, int type)
{
	std::string url;
	url.append(HTTPURL);
	url.append("wx_topup?");
	url.append("playerid=");
	url.append(JhGlobalData::UUID());
	url.append("&amount=");
	std::string str = StringUtils::format("%d", amount);
	url.append(str);
	url.append("&type=");
	str = StringUtils::format("%d", type);
	url.append(str);
	JhHttpUtil::getInstance()->doData(url, httputil_calback(JhServerDataSwap::httpPostMyRechargeCB, this));
}

void JhServerDataSwap::httpBlankCB(std::string retdata, int code, std::string tag)
{
	release();
}

void JhServerDataSwap::httpPostOneDataCB(std::string retdata, int code, std::string tag)
{
	if (code == 0)
	{
		if (tag.compare("1") == 0)
		{
			if (m_pDelegateProtocol != NULL)
			{
				m_pDelegateProtocol->onSuccess();
			}
		}
		rapidjson::Document doc;
		if (JsonReader(retdata, doc))
		{
			if (doc.HasMember("ret"))
			{
				rapidjson::Value& retval = doc["ret"];
				if (retval.GetInt() == 0)
				{
					rapidjson::Value& myidv = doc["id"];
					std::string myidstr = myidv.GetString();
					JhGameDataSave::getInstance()->setMyID(myidstr);

					rapidjson::Value& mynamev = doc["nickname"];
					std::string mynamestr = mynamev.GetString();
					JhGameDataSave::getInstance()->setMyNickName(mynamestr);
				}
			}

		}
	}
	else
	{
		if (m_pDelegateProtocol != NULL)
		{
			m_pDelegateProtocol->onErr(-1);
		}
	}
	release();
}

void JhServerDataSwap::httpGetAllDataCB(std::string retdata, int code, std::string tag)
{
	if (code == 0)
	{
		rapidjson::Document doc;
		if (JsonReader(retdata, doc))
		{
			bool isok = false;
			if (doc.HasMember("ret"))
			{
				rapidjson::Value& retval = doc["ret"];
				if (retval.GetInt() == 0)
					isok = true;
			}


			if (isok)
			{
				std::vector<std::string> vec_saveid;
				for (int i = 0; i < 4; i++)
				{
					vec_saveid.push_back("");
				}
				rapidjson::Value& myidv = doc["id"];
				std::string myidstr = myidv.GetString();
				JhGameDataSave::getInstance()->setMyID(myidstr);

				rapidjson::Value& mynamev = doc["nickname"];
				std::string mynamestr = mynamev.GetString();
				JhGameDataSave::getInstance()->setMyNickName(mynamestr);

				if (doc.HasMember("coin"))
				{
					rapidjson::Value& coindata = doc["coin"];
					JhGameDataSave::getInstance()->setGoldCount(atoi(coindata.GetString()));
				}
				if (doc.HasMember("costcoin"))
				{
					rapidjson::Value& coindata = doc["costcoin"];
					JhGameDataSave::getInstance()->setUseGold(atoi(coindata.GetString()));
				}
				int hunlock = 1;
				if (doc.HasMember("hunlock"))
				{
					rapidjson::Value& hunlockdata = doc["hunlock"];
					hunlock = atoi(hunlockdata.GetString());

					for (int k = 0; k < 4; k++)
					{
						int val = hunlock & (1 << k);
						val = val >> k;
						JhGlobalData::setUnlockHero(k, val == 1 ? true : false);
					}
				}

				if (doc.HasMember("data"))
				{
					rapidjson::Value& dataArray = doc["data"];
					for (unsigned int m = 0; m < dataArray.Size(); m++)
					{
						rapidjson::Value& item = dataArray[m];

						rapidjson::Value& v = item["localid"];
						std::string localuid = v.GetString();

						JhGameDataSave::getInstance()->setUserId(localuid);

						v = item["type"];
						int type = atoi(v.GetString());
						JhGameDataSave::getInstance()->setHeroId(type);

						vec_saveid[type - 1] = localuid;

						v = item["exp"];
						int exp = atoi(v.GetString());
						int lv = 0;
						int size = JhGlobalData::map_heroAtr[type].vec_exp.size();
						for (int i = 0; i < size; i++)
						{
							if (exp > JhGlobalData::map_heroAtr[type].vec_exp[i])
							{
								lv = i;
								exp = exp - JhGlobalData::map_heroAtr[type].vec_exp[i];
							}
							else
							{
								break;
							}
						}
						if (lv >= size)
						{
							lv = size - 1;
							exp = JhGlobalData::map_heroAtr[type].vec_exp[lv];
						}

						JhGameDataSave::getInstance()->setHeroLV(lv);
						JhGameDataSave::getInstance()->setHeroExp(exp);

						v = item["sex"];
						int sex = atoi(v.GetString());
						if (type == 4)
							sex = S_WOMEN;
						JhGameDataSave::getInstance()->setHeroSex(sex);

						v = item["hungry"];
						int hungry = atoi(v.GetString());
						JhGameDataSave::getInstance()->setHeroHunger(hungry);

						v = item["innerhurt"];
						int innerhurt = atoi(v.GetString());
						JhGameDataSave::getInstance()->setHeroInnerinjury(innerhurt);

						v = item["outerhurt"];
						int outerhurt = atoi(v.GetString());
						JhGameDataSave::getInstance()->setHeroOutinjury(outerhurt);

						v = item["life"];
						int life = atoi(v.GetString());
						JhGameDataSave::getInstance()->setHeroLife(life);

						v = item["days"];
						int days = atoi(v.GetString());
						JhGameDataSave::getInstance()->setLiveDays(days);

						v = item["mood"];
						int spirit = atoi(v.GetString());
						JhGameDataSave::getInstance()->setHeroSpirit(spirit);

						v = item["task"];
						int task = atoi(v.GetString());
						JhGameDataSave::getInstance()->setPlotMissionIndex(task);

						JhGlobalData::loadPlotMissionJsonData(type);

						std::string str;
						int pdatasize = JhGlobalData::vec_PlotMissionData.size();
						for (int i = 0; i < pdatasize; i++)
						{
							std::string tmpstr;
							if (i < task)
								tmpstr = "2-";
							else
								tmpstr = "0-";
							str.append(tmpstr);
						}
						JhGameDataSave::getInstance()->setPlotMissionStatus(str.substr(0, str.length() - 1));

						v = item["newbtask"];
						std::string btask = v.GetString();

						JhGameDataSave::getInstance()->setBranchPlotMissionStatus(btask);

						v = item["unlock"];
						int unlock = atoi(v.GetString());
						JhGameDataSave::getInstance()->setPlotUnlockChapter(unlock);

						rapidjson::Document doc = ReadJsonFile("data/buildings.json");
						rapidjson::Value& allBuilds = doc["b"];
						for (unsigned int i = 0; i < allBuilds.Size(); i++)
						{
							rapidjson::Value& oneBuild = allBuilds[i];
							rapidjson::Value& oneitem = oneBuild["name"];
							std::string buildname = oneitem.GetString();
							v = item[buildname.c_str()];
							int blv = atoi(v.GetString());
							JhGameDataSave::getInstance()->setBuildLV(buildname, blv);
						}

						if (item.HasMember("friendship"))
						{
							v = item["friendship"];
							std::string friendshipstr = v.GetString();
							JhGameDataSave::getInstance()->setFriendly(friendshipstr);
						}
						if (item.HasMember("achievement"))
						{
							v = item["achievement"];
							std::string achivestr = v.GetString();
							JhGameDataSave::getInstance()->setAchiveData(achivestr);
						}


						if (item.HasMember("mixgf"))
						{
							v = item["mixgf"];
							JhGameDataSave::getInstance()->setMixGF(v.GetString());
						}

						v = item["holding"];

						for (unsigned int n = 0; n < v.Size(); n++)
						{
							rapidjson::Value& hv = v[n];
							rapidjson::Value& resv = hv["flag"];
							int flag = atoi(resv.GetString());
							std::string str;
							for (rapidjson::Value::ConstMemberIterator iter = hv.MemberBegin(); iter != hv.MemberEnd(); ++iter)
							{
								std::string strid = iter->name.GetString();

								if (strid.compare("flag") != 0)
								{
									int val = atoi(iter->value.GetString());
									int goodvalue = 100;
									int lv = 0;
									int tqu = 1;
									int slv = 0;
									int count = 0;
									if (strid.compare(0, 1, "r") == 0)
									{
										strid = strid.substr(1);
										count = val;
									}
									else
									{
										if (strid.compare(0, 1, "a") == 0 || strid.compare(0, 1, "e") == 0)
										{
											goodvalue = val % 1000 - 1;
											if (goodvalue < 0)
												goodvalue = 0;
											slv = val / 1000;
										}
										else
										{
											lv = val - 1;
										}
										count = 1;
									}
									std::string tempstr = StringUtils::format("%s-%d-%d-%d-%d-%d-%d-%d-%d;", strid.c_str(), JhGlobalData::getResType(strid), count, JhGlobalData::getResExType(strid), lv, 0, goodvalue, slv, tqu);
									str.append(tempstr);
								}
							}
							if (str.length() > 1)
							{
								str = str.substr(0, str.length() - 1);
								if (flag == 1)
									JhGameDataSave::getInstance()->setStorageData(str);
								else if (flag == 2)
								{
									JhGameDataSave::getInstance()->setPackage(str);
								}
								else if (flag == 3)
								{
									const std::string prestr[] = { "a", "24", "25", "26", "w", "x", "e", "7" };
									std::vector<std::string> tmp;
									JhCommonFuncs::split(str, tmp, ";");
									str.clear();
									for (int c = 0; c < 8; c++)
									{
										int len = prestr[c].length();
										bool isfind = false;
										for (unsigned int k = 0; k < tmp.size(); k++)
										{
											if (prestr[c].compare(0, len, tmp[k], 0, len) == 0)
											{
												isfind = true;
												str.append(tmp[k]);
												str.append(";");
												break;
											}
										}
										if (!isfind)
										{
											str.append("-0-0-0-0-0-100-0-1;");
										}
									}
									JhGameDataSave::getInstance()->setHeroProperData(str.substr(0, str.length() - 1));
								}
							}
						}

					}
				}

				JhGlobalData::setSaveListId(vec_saveid);
				if (vec_saveid.size() > 0)
					JhGlobalData::setUId(vec_saveid[0]);

				if (m_pDelegateProtocol != NULL)
				{
					m_pDelegateProtocol->onSuccess();
					JhGlobalData::init();
				}
			}
		}
	}

	if (m_pDelegateProtocol != NULL)
	{
		m_pDelegateProtocol->onErr(-1);
	}
	release();
}

void JhServerDataSwap::httpPropadJustDataCB(std::string retdata, int code, std::string tag)
{
	int retult = -1;
	if (code == 0)
	{
		rapidjson::Document doc;
		if (JsonReader(retdata, doc))
		{
			if (doc.HasMember("data"))
			{
				rapidjson::Value& dataArray = doc["data"];

				for (unsigned int m = 0; m < dataArray.Size(); m++)
				{
					rapidjson::Value& item = dataArray[m];

					rapidjson::Value& v = item["propid"];
					std::string strid = v.GetString();

					v = item["amount"];
					int val = atoi(v.GetString());

					if (strid.compare("coin") == 0)
					{
						int curcount = JhGameDataSave::getInstance()->getGoldCount();
						JhGameDataSave::getInstance()->setGoldCount(curcount + val);
						JhGlobalData::init();
						continue;
					}

					if (strid.compare("hunlock") == 0)
					{
						int hunlock = val;

						for (int k = 0; k < 4; k++)
						{
							int val = hunlock & (1 << k);
							val = val >> k;
							JhGlobalData::setUnlockHero(k, val == 1 ? true : false);
						}
						continue;
					}

					v = item["type"];
					int heroid = atoi(v.GetString());
					if (heroid > 0)
					{
						std::vector<std::string> vec_ids = JhGlobalData::getSaveListId();
						std::string uid = vec_ids[heroid - 1];
						if (uid.length() > 0)
						{
							JhGameDataSave::getInstance()->setUserId(uid);

							std::string strval = JhGameDataSave::getInstance()->getStorageData();
							std::vector<std::string> tmp;
							JhCommonFuncs::split(strval, tmp, ";");
							std::string retstr;
							bool isfind = false;
							for (unsigned int i = 0; i < tmp.size(); i++)
							{
								std::vector<std::string> tmp2;
								JhCommonFuncs::split(tmp[i], tmp2, "-");

								std::string id = tmp2[0];
								int type = atoi(tmp2[1].c_str());

								int count = atoi(tmp2[2].c_str());
								int extype = JhGlobalData::getResExType(id);
								int lv = atoi(tmp2[4].c_str());

								int exp = atoi(tmp2[5].c_str());
								int goodvalue = atoi(tmp2[6].c_str());
								int slv = 0;
								int tqu = 1;
								if (tmp2.size() >= 9)
								{
									slv = atoi(tmp2[7].c_str());
									tqu = atoi(tmp2[8].c_str());
								}
								if (strid.compare(id) == 0)
								{
									isfind = true;
									if (atoi(strid.c_str()) > 0)
										count += val;
									else
									{
										if (strid.compare(0, 1, "a") == 0 || strid.compare(0, 1, "e") == 0)
										{
											goodvalue = val % 1000;
											slv = val / 1000;
										}
										else
										{
											lv = val - 1;
										}
										count = 1;
									}
								}
								std::string idstr = StringUtils::format("%s-%d-%d-%d-%d-%d-%d-%d-%d;", id.c_str(), type, count, extype, lv, exp, goodvalue, slv, tqu);
								retstr.append(idstr);

							}
							if (!isfind)
							{
								int count = 0;
								int goodvalue = 100;
								int lv = 0;
								int slv = 0;
								if (atoi(strid.c_str()) > 0)
									count = val;
								else
								{
									if (strid.compare(0, 1, "a") == 0 || strid.compare(0, 1, "e") == 0)
									{
										goodvalue = val % 1000;
										slv = val / 1000;
									}
									else
									{
										lv = val - 1;
									}
									count = 1;
								}
								std::string idstr = StringUtils::format("%s-%d-%d-%d-%d-%d-%d-%d-%d;", strid.c_str(), JhGlobalData::getResType(strid), count, JhGlobalData::getResExType(strid), lv, 0, goodvalue, slv, 1);
								retstr.append(idstr);
							}

							if (retstr.length() > 0)
								JhGameDataSave::getInstance()->setStorageData(retstr.substr(0, retstr.length() - 1));
						}
					}
				}
				retult = 0;
			}
			else
				retult = -2;
		}
	}
	if (retult == 0)
	{
		if (m_pDelegateProtocol != NULL)
		{
			m_pDelegateProtocol->onSuccess();
		}
	}
	else
	{
		if (m_pDelegateProtocol != NULL)
		{
			m_pDelegateProtocol->onErr(retult);
		}
	}
	release();
}

void JhServerDataSwap::httpModifyNickNameCB(std::string retdata, int code, std::string tag)
{
	if (code == 0)
	{
		rapidjson::Document doc;
		if (JsonReader(retdata, doc))
		{
			rapidjson::Value& reval = doc["ret"];
			int ret = reval.GetInt();
			if (ret == 0)
			{
				if (m_pDelegateProtocol != NULL)
					m_pDelegateProtocol->onSuccess();
			}
			else
			{
				if (m_pDelegateProtocol != NULL)
					m_pDelegateProtocol->onErr(ret);
			}
		}
        else
        {
            if (m_pDelegateProtocol != NULL)
            {
                m_pDelegateProtocol->onErr(-1);
            }
        }
        
	}
	else
	{
		if (m_pDelegateProtocol != NULL)
		{
			m_pDelegateProtocol->onErr(-1);
		}
	}
	release();
}

void JhServerDataSwap::httpVipIsOnCB(std::string retdata, int code, std::string tag)
{
	JhGlobalData::isExchangeGift = false;
	JhGlobalData::isRecharge = false;
	JhGlobalData::couponinfo = "";
	if (code == 0)
	{
		rapidjson::Document doc;
		if (JsonReader(retdata, doc))
		{
			JhGlobalData::vec_buyVipIds.clear();
			JhGlobalData::map_buyVipDays.clear();
			for (rapidjson::Value::ConstMemberIterator iter = doc.MemberBegin(); iter != doc.MemberEnd(); ++iter)
			{
				std::string strid = iter->name.GetString();

				if (strid.compare(0, 3, "vip") == 0)
				{
					int val = iter->value.GetInt();
					if (val > 0)
					{
						JhGlobalData::vec_buyVipIds.push_back(strid);
						
					}
				}
				else
				{
					std::size_t pos = strid.find("vip");
					if (pos != std::string::npos && pos > 0)
					{
						int val = iter->value.GetInt();
						JhGlobalData::map_buyVipDays[strid.substr(pos)] = val;
					}
				}
			}

			if (doc.HasMember("timegift"))
			{
				rapidjson::Value& retval = doc["timegift"];
				JhGlobalData::setTimeGiftLeftTime(retval.GetInt());
			}

			if (doc.HasMember("freelife"))
			{
				rapidjson::Value& retval = doc["freelife"];
				JhGlobalData::setFreeReviveCount(retval.GetInt());
			}

			if (doc.HasMember("punishment"))
			{
				rapidjson::Value& retval = doc["punishment"];
				JhGlobalData::ispunishment = retval.GetInt() ==0?false:true;
			}


			if (doc.HasMember("coinpool"))
			{
				rapidjson::Value& retval = doc["coinpool"];
				JhGlobalData::myRaffleData.isshow = retval.GetInt() == 0 ? false : true;
			}

			if (doc.HasMember("lottery"))
			{
				rapidjson::Value& retval = doc["lottery"];
				JhGlobalData::myLotteryData.isshow = retval.GetInt() == 0 ? false : true;
			}

			if (doc.HasMember("login_days"))
			{
				rapidjson::Value& retval = doc["login_days"];
				int days = retval.GetInt();
				if (days > 0)
					JhGlobalData::continueLoginDays = retval.GetInt();
			}

			if (doc.HasMember("hei"))
			{
				rapidjson::Value& retval = doc["hei"];
				int vhei = retval.GetInt();
				JhGlobalData::isFrozen = vhei == 1 ? true : false;
			}

			if (doc.HasMember("opencoupon"))
			{
				rapidjson::Value& retval = doc["opencoupon"];
				int v = retval.GetInt();
				JhGlobalData::isExchangeGift = true; // = v == 1 ? true : false;
			}

			if (doc.HasMember("couponinfo"))
			{
				rapidjson::Value& retval = doc["couponinfo"];
				JhGlobalData::couponinfo = retval.GetString();
			}

			if (doc.HasMember("durl"))
			{
				rapidjson::Value& retval = doc["durl"];
				JhGlobalData::updateDownLoadURL = retval.GetString();
			}

			if (doc.HasMember("opentopup"))
			{
				rapidjson::Value& retval = doc["opentopup"];
				int v = retval.GetInt();
				JhGlobalData::isRecharge = v == 1 ? true : false;
			}
		
			if (m_pDelegateProtocol != NULL)
				m_pDelegateProtocol->onSuccess();
		}
	}
	else
	{
		if (m_pDelegateProtocol != NULL)
		{
			m_pDelegateProtocol->onErr(-1);
		}
	}
	release();
}

void JhServerDataSwap::httpGetServerTimeCB(std::string retdata, int code, std::string tag)
{
	bool isok = false;
	if (code == 0)
	{
		rapidjson::Document doc;
		if (JsonReader(retdata, doc))
		{
			rapidjson::Value& timev = doc["time"];

			JhGlobalData::servertime = timev.GetInt();
			isok = true;
		}
	}
	if (isok)
	{
		if (m_pDelegateProtocol != NULL)
		{
			m_pDelegateProtocol->onSuccess();
		}
	}
	else
	{
		if (m_pDelegateProtocol != NULL)
		{
			m_pDelegateProtocol->onErr(-1);
		}
	}
	release();
}

void JhServerDataSwap::httpGetRankDataCB(std::string retdata, int code, std::string tag)
{
	bool isok = false;
	if (code == 0)
	{
		rapidjson::Document doc;
		if (JsonReader(retdata, doc))
		{
			JhGlobalData::vec_rankData.clear();
			JhGlobalData::myrank = 0;
			if (doc.HasMember("rank"))
			{
				rapidjson::Value& v = doc["rank"];
				JhGlobalData::myrank = atoi(v.GetString());
			}
			if (doc.HasMember("data"))
			{
				rapidjson::Value& dataArray = doc["data"];

				for (unsigned int m = 0; m < dataArray.Size(); m++)
				{
					RankData rdata;
					rdata.rank = m + 1;
					rapidjson::Value& item = dataArray[m];
					rapidjson::Value& v = item["nickname"];
					rdata.nickname = v.GetString();
					v = item["sex"];
					rdata.herosex = atoi(v.GetString());

					v = item["type"];
					rdata.herotype = atoi(v.GetString());

					v = item["exp"];
					int exp = atoi(v.GetString());
					int lv = 0;
					int size = JhGlobalData::map_heroAtr[rdata.herotype].vec_exp.size();
					for (int i = 0; i < size; i++)
					{
						if (exp > JhGlobalData::map_heroAtr[rdata.herotype].vec_exp[i])
						{
							lv = i;
							exp = exp - JhGlobalData::map_heroAtr[rdata.herotype].vec_exp[i];
						}
						else
						{
							break;
						}
					}
					rdata.herolv = lv;

					int heroval = 0;
					if (item.HasMember("days"))
					{
						v = item["days"];
						heroval = atoi(v.GetString());
					}
					else if (item.HasMember("fightingpower"))
					{
						v = item["fightingpower"];
						heroval = atoi(v.GetString());
					}
					else
						heroval = 0;

					rdata.heroval = heroval;
					JhGlobalData::vec_rankData.push_back(rdata);
				}
				isok = true;
			}

		}
	}

	if (isok)
	{
		if (m_pDelegateProtocol != NULL)
		{
			m_pDelegateProtocol->onSuccess();
		}
	}
	else
	{
		if (m_pDelegateProtocol != NULL)
		{
			m_pDelegateProtocol->onErr(-1);
		}
	}
	release();
}

void JhServerDataSwap::httpGetAnnouncementCB(std::string retdata, int code, std::string tag)
{
	bool isok = false;
	if (code == 0)
	{
		rapidjson::Document doc;
		if (JsonReader(retdata, doc))
		{
			if (doc.HasMember("data"))
			{
				JhGlobalData::noticecontent.clear();
				rapidjson::Value& dataArray = doc["data"];

				for (unsigned int m = 0; m < dataArray.Size(); m++)
				{
					rapidjson::Value& v = dataArray[m]["content"];
					JhGlobalData::noticecontent.append(v.GetString());
					JhGlobalData::noticecontent.append("\r\n");
				}
				
				isok = true;
			}
			else
			{
				isok = false;
			}
		}
		if (JhGlobalData::noticecontent.length() <= 0)
		{
			isok = false;
		}
	}

	if (isok)
	{
		if (m_pDelegateProtocol != NULL)
		{
			m_pDelegateProtocol->onSuccess();
		}
	}
	else
	{
		if (m_pDelegateProtocol != NULL)
		{
			m_pDelegateProtocol->onErr(-1);
		}
	}
	release();
}

void JhServerDataSwap::httpIsGetVipCB(std::string retdata, int code, std::string tag)
{
	if (m_pDelegateProtocol != NULL)
	{
		if (code >= -1)
			m_pDelegateProtocol->onSuccess();
	}
	release();
}

void JhServerDataSwap::httpCreateFactionCB(std::string retdata, int code, std::string tag)
{
	int ret = code;
	if (m_pDelegateProtocol != NULL)
	{
		if (code == 0)
		{

			rapidjson::Document doc;
			if (JsonReader(retdata, doc))
			{
				if (doc.HasMember("ret"))
				{
					rapidjson::Value& v = doc["ret"];
					ret = v.GetInt();
				}
			}
			if (ret == 0)
				m_pDelegateProtocol->onSuccess();
			else
				m_pDelegateProtocol->onErr(-ret);
		}
		else
			m_pDelegateProtocol->onErr(ret);
	}
	release();
}

void JhServerDataSwap::httpModifyFactionCB(std::string retdata, int code, std::string tag)
{
	int ret = code;
	if (m_pDelegateProtocol != NULL)
	{
		if (code == 0)
		{

			rapidjson::Document doc;
			if (JsonReader(retdata, doc))
			{
				if (doc.HasMember("ret"))
				{
					rapidjson::Value& v = doc["ret"];
					ret = v.GetInt();
				}
			}
			if (ret == 0)
				m_pDelegateProtocol->onSuccess();
			else
				m_pDelegateProtocol->onErr(-ret);
		}
		else
			m_pDelegateProtocol->onErr(ret);
	}
	release();
}

void JhServerDataSwap::httpGetFactionListCB(std::string retdata, int code, std::string tag)
{
	bool isok = false;
	if (code == 0)
	{
		rapidjson::Document doc;
		if (JsonReader(retdata, doc))
		{
			JhGlobalData::vec_factionListData.clear();
			if (doc.HasMember("belongto"))
			{
				rapidjson::Value& v = doc["belongto"];
				JhGlobalData::myFaction = v.GetInt();
			}
			if (doc.HasMember("title"))
			{
				rapidjson::Value& v = doc["title"];
				JhGlobalData::mytitle = v.GetInt();
			}

			if (JhGlobalData::myFaction > 0 && JhGlobalData::mytitle > 0)
			{
				if (doc.HasMember("level"))
				{
					rapidjson::Value& v = doc["level"];
					JhGlobalData::myFactionlv = v.GetInt();
				}
			}

			if (doc.HasMember("data"))
			{
				rapidjson::Value& dataArray = doc["data"];

				for (unsigned int m = 0; m < dataArray.Size(); m++)
				{
					FactionListData fdata;
					fdata.rank = m;

					rapidjson::Value& v = dataArray[m]["id"];
					fdata.id = atoi(v.GetString());

					v = dataArray[m]["nickname"];
					fdata.factionname = v.GetString();

					v = dataArray[m]["levellower"];
					fdata.lvlimit = atoi(v.GetString());

					v = dataArray[m]["sex"];
					fdata.sexlimit = atoi(v.GetString());

					v = dataArray[m]["buildername"];
					fdata.owner = v.GetString();

					v = dataArray[m]["peoplenumber"];
					fdata.membercount = atoi(v.GetString());

					v = dataArray[m]["peopleupper"];
					fdata.maxcount = atoi(v.GetString());

					v = dataArray[m]["exp"];
					fdata.exp = atoi(v.GetString());

					fdata.lv = 0;

					v = dataArray[m]["remark"];
					fdata.desc = v.GetString();
					JhGlobalData::vec_factionListData.push_back(fdata);
				}
			}
			isok = true;
		}
	}

	if (isok)
	{
		if (m_pDelegateProtocol != NULL)
		{
			m_pDelegateProtocol->onSuccess();
		}
	}
	else
	{
		if (m_pDelegateProtocol != NULL)
		{
			m_pDelegateProtocol->onErr(-1);
		}
	}
	release();
}

void JhServerDataSwap::httpRequestFactionListCB(std::string retdata, int code, std::string tag)
{
	int ret = code;
	if (m_pDelegateProtocol != NULL)
	{
		if (code == 0)
		{
			rapidjson::Document doc;
			if (JsonReader(retdata, doc))
			{
				if (doc.HasMember("ret"))
				{
					rapidjson::Value& v = doc["ret"];
					ret = v.GetInt();
				}
			}
			if (ret == 0)
				m_pDelegateProtocol->onSuccess();
			else
				m_pDelegateProtocol->onErr(ret);
		}
		else
			m_pDelegateProtocol->onErr(ret);
	}
	release();
}

void JhServerDataSwap::httpGetFactionMemberCB(std::string retdata, int code, std::string tag)
{
	bool isok = false;
	if (code == 0)
	{
		rapidjson::Document doc;
		if (JsonReader(retdata, doc))
		{
			if (doc.HasMember("data"))
			{
				JhGlobalData::vec_factionMemberData.clear();
				rapidjson::Value& dataArray = doc["data"];

				for (unsigned int m = 0; m < dataArray.Size(); m++)
				{
					FactionMemberData fdata;
					fdata.id = m + 1;
					rapidjson::Value& v = dataArray[m]["id"];
					fdata.userid = atoi(v.GetString());

					v = dataArray[m]["type"];
					fdata.herotype = atoi(v.GetString());

					v = dataArray[m]["exp"];
					int exp = atoi(v.GetString());
					int lv = 0;
					int size = JhGlobalData::map_heroAtr[fdata.herotype].vec_exp.size();
					for (int i = 0; i < size; i++)
					{
						if (exp > JhGlobalData::map_heroAtr[fdata.herotype].vec_exp[i])
						{
							lv = i;
							exp = exp - JhGlobalData::map_heroAtr[fdata.herotype].vec_exp[i];
						}
						else
						{
							break;
						}
					}
					fdata.herolv = lv;

					v = dataArray[m]["nickname"];
					fdata.nickname = v.GetString();

					v = dataArray[m]["contribution"];
					fdata.contribution = atoi(v.GetString());

					v = dataArray[m]["factionid"];
					fdata.factionid = atoi(v.GetString());

					v = dataArray[m]["title"];
					fdata.position = atoi(v.GetString());
					if (fdata.position >= 0)
						JhGlobalData::vec_factionMemberData.push_back(fdata);
				}
			}
			isok = true;
		}
	}

	if (isok)
	{
		if (m_pDelegateProtocol != NULL)
		{
			m_pDelegateProtocol->onSuccess();
		}
	}
	else
	{
		if (m_pDelegateProtocol != NULL)
		{
			m_pDelegateProtocol->onErr(-1);
		}
	}
	release();
}

void JhServerDataSwap::httpJionFactionCB(std::string retdata, int code, std::string tag)
{
	int ret = code;
	if (m_pDelegateProtocol != NULL)
	{
		if (code == 0)
		{
			rapidjson::Document doc;
			if (JsonReader(retdata, doc))
			{
				if (doc.HasMember("ret"))
				{
					rapidjson::Value& v = doc["ret"];
					ret = v.GetInt();
				}
			}
			if (ret == 0)
				m_pDelegateProtocol->onSuccess();
			else
				m_pDelegateProtocol->onErr(-ret);
		}
		else
			m_pDelegateProtocol->onErr(ret);
	}
	release();
}

void JhServerDataSwap::httpKickFactionCB(std::string retdata, int code, std::string tag)
{
	int ret = code;
	if (m_pDelegateProtocol != NULL)
	{
		if (code == 0)
		{
			rapidjson::Document doc;
			if (JsonReader(retdata, doc))
			{
				if (doc.HasMember("ret"))
				{
					rapidjson::Value& v = doc["ret"];
					ret = v.GetInt();
				}
			}
			if (ret == 0)
				m_pDelegateProtocol->onSuccess();
			else
				m_pDelegateProtocol->onErr(-ret);
		}
		else
			m_pDelegateProtocol->onErr(ret);
	}
	release();
}

void JhServerDataSwap::httpPromotionFactionCB(std::string retdata, int code, std::string tag)
{
	int ret = code;
	if (m_pDelegateProtocol != NULL)
	{
		if (code == 0)
		{
			rapidjson::Document doc;
			if (JsonReader(retdata, doc))
			{
				if (doc.HasMember("ret"))
				{
					rapidjson::Value& v = doc["ret"];
					ret = v.GetInt();
				}
			}
			if (ret == 0)
				m_pDelegateProtocol->onSuccess();
			else
				m_pDelegateProtocol->onErr(-ret);
		}
		else
			m_pDelegateProtocol->onErr(ret);
	}
	release();
}

void JhServerDataSwap::httpLeaveFactionCB(std::string retdata, int code, std::string tag)
{
	int ret = code;
	if (m_pDelegateProtocol != NULL)
	{
		if (code == 0)
		{
			rapidjson::Document doc;
			if (JsonReader(retdata, doc))
			{
				if (doc.HasMember("ret"))
				{
					rapidjson::Value& v = doc["ret"];
					ret = v.GetInt();
				}
			}
			if (ret == 0)
				m_pDelegateProtocol->onSuccess();
			else
				m_pDelegateProtocol->onErr(-ret);
		}
		else
			m_pDelegateProtocol->onErr(ret);
	}
	release();
}

void JhServerDataSwap::httpContributionFactionCB(std::string retdata, int code, std::string tag)
{
	JhGlobalData::factionExp = 0;
	int ret = code;
	if (m_pDelegateProtocol != NULL)
	{
		if (code == 0)
		{
			rapidjson::Document doc;
			if (JsonReader(retdata, doc))
			{
				if (doc.HasMember("ret"))
				{
					rapidjson::Value& v = doc["ret"];
					ret = v.GetInt();
				}
			}
			if (ret == 0)
			{
				if (doc.HasMember("exp"))
				{
					rapidjson::Value& v = doc["exp"];
					JhGlobalData::factionExp = v.GetInt();
				}
				m_pDelegateProtocol->onSuccess();
			}
			else
				m_pDelegateProtocol->onErr(-ret);
		}
		else
			m_pDelegateProtocol->onErr(ret);
	}
	release();
}

void JhServerDataSwap::httpCancelFactionCB(std::string retdata, int code, std::string tag)
{
	int ret = code;
	if (m_pDelegateProtocol != NULL)
	{
		if (code == 0)
		{
			int title = 0;
			rapidjson::Document doc;
			if (JsonReader(retdata, doc))
			{
				if (doc.HasMember("ret"))
				{
					rapidjson::Value& v = doc["ret"];
					ret = v.GetInt();
				}
				if (doc.HasMember("title"))
				{
					rapidjson::Value& v = doc["title"];
					title = atoi(v.GetString());
				}
			}
			if (ret == 0)
			{
				m_pDelegateProtocol->onSuccess();
				if (title == 4)
					m_pDelegateProtocol->onErr(-title);
			}
			else
				m_pDelegateProtocol->onErr(-ret);
		}
		else
			m_pDelegateProtocol->onErr(ret);
	}
	release();
}

void JhServerDataSwap::httpRefuseFactionCB(std::string retdata, int code, std::string tag)
{
	int ret = code;
	if (m_pDelegateProtocol != NULL)
	{
		if (code == 0)
		{
			rapidjson::Document doc;
			if (JsonReader(retdata, doc))
			{
				if (doc.HasMember("ret"))
				{
					rapidjson::Value& v = doc["ret"];
					ret = v.GetInt();
				}
			}
			if (ret == 0)
				m_pDelegateProtocol->onSuccess();
			else
				m_pDelegateProtocol->onErr(-ret);
		}
		else
			m_pDelegateProtocol->onErr(ret);
	}
	release();
}

void JhServerDataSwap::httpLotteryCB(std::string retdata, int code, std::string tag)
{
	int ret = code;
	if (m_pDelegateProtocol != NULL)
	{
		if (code == 0)
		{
			rapidjson::Document doc;
			if (JsonReader(retdata, doc))
			{
				if (doc.HasMember("ret"))
				{
					rapidjson::Value& v = doc["ret"];
					ret = v.GetInt();
				}
				if (doc.HasMember("count"))
				{
					rapidjson::Value& v = doc["count"];
					JhGlobalData::myLotteryData.leftcount = v.GetInt();
				}
				if (doc.HasMember("cost"))
				{
					rapidjson::Value& v = doc["cost"];
					JhGlobalData::myLotteryData.nextcostgold = v.GetInt();
				}
				if (doc.HasMember("gain"))
				{
					rapidjson::Value& v = doc["gain"];
					JhGlobalData::myLotteryData.wingold = v.GetInt();
				}
			}
			if (ret == 0)
				m_pDelegateProtocol->onSuccess();
			else
				m_pDelegateProtocol->onErr(-ret);
		}
		else
			m_pDelegateProtocol->onErr(ret);
	}
	release();
}

void JhServerDataSwap::httpGetCoinpoolCB(std::string retdata, int code, std::string tag)
{
	int ret = code;
	if (m_pDelegateProtocol != NULL)
	{
		if (code == 0)
		{
			rapidjson::Document doc;
			if (JsonReader(retdata, doc))
			{
				if (doc.HasMember("ret"))
				{
					rapidjson::Value& v = doc["ret"];
					ret = v.GetInt();

					if (ret == 2)
					{
						JhGlobalData::myRaffleData.iscanplay = false;
					}
					else if (ret == 0)
						JhGlobalData::myRaffleData.iscanplay = true;
				}
				if (doc.HasMember("remain"))
				{
					rapidjson::Value& v = doc["remain"];
					JhGlobalData::myRaffleData.leftime = v.GetInt();
				}

				if (doc.HasMember("stage"))
				{
					rapidjson::Value& v = doc["stage"];
					JhGlobalData::myRaffleData.curstage = v.GetString();
				}

				if (doc.HasMember("pool"))
				{
					rapidjson::Value& v = doc["pool"];
					JhGlobalData::myRaffleData.poolgold = v.GetInt();
				}

				if (doc.HasMember("data"))
				{
					JhGlobalData::myRaffleData.vec_nicknames.clear();
					JhGlobalData::myRaffleData.vec_wingold.clear();
					rapidjson::Value& dataArray = doc["data"];

					for (unsigned int m = 0; m < dataArray.Size(); m++)
					{
						rapidjson::Value& v = dataArray[m]["nickname"];
						JhGlobalData::myRaffleData.vec_nicknames.push_back(v.GetString());
					}

					for (unsigned int m = 0; m < dataArray.Size(); m++)
					{
						rapidjson::Value& v = dataArray[m]["gain"];
						JhGlobalData::myRaffleData.vec_wingold.push_back(atoi(v.GetString()));
					}
				}

				if (doc.HasMember("my"))
				{
					rapidjson::Value& dataArray = doc["my"];

					if (dataArray.Size() >= 1)
					{
						rapidjson::Value& v = dataArray[0]["gain"];
						JhGlobalData::myRaffleData.mywingold = atoi(v.GetString());
						v = dataArray[0]["stage"];
						JhGlobalData::myRaffleData.mywinstage = v.GetString();
						v = dataArray[0]["rank"];
						JhGlobalData::myRaffleData.mywinrank = atoi(v.GetString());
					}
				}
			}
			if (ret == 0 || ret == 2)
				m_pDelegateProtocol->onSuccess();
			else
				m_pDelegateProtocol->onErr(-ret);
		}
		else
			m_pDelegateProtocol->onErr(ret);
	}
	release();
}

void JhServerDataSwap::httpPlayCoinpoolCB(std::string retdata, int code, std::string tag)
{
	int ret = code;
	if (m_pDelegateProtocol != NULL)
	{
		if (code == 0)
		{
			rapidjson::Document doc;
			if (JsonReader(retdata, doc))
			{
				if (doc.HasMember("ret"))
				{
					rapidjson::Value& v = doc["ret"];
					ret = v.GetInt();
				}

				if (doc.HasMember("pool"))
				{
					rapidjson::Value& v = doc["pool"];
					JhGlobalData::myRaffleData.poolgold = v.GetInt();
				}
			}
			if (ret == 0)
				m_pDelegateProtocol->onSuccess();
			else
				m_pDelegateProtocol->onErr(-ret);
		}
		else
			m_pDelegateProtocol->onErr(ret);
	}
	release();
}

void JhServerDataSwap::httpGetCouponsCB(std::string retdata, int code, std::string tag)
{
	int ret = code;
	if (m_pDelegateProtocol != NULL)
	{
		if (code == 0)
		{
			rapidjson::Document doc;
			if (JsonReader(retdata, doc))
			{
				if (doc.HasMember("ret"))
				{
					rapidjson::Value& v = doc["ret"];
					ret = v.GetInt();
				}
			}
			if (ret == 0)
				m_pDelegateProtocol->onSuccess();
			else
				m_pDelegateProtocol->onErr(-ret);
		}
		else
			m_pDelegateProtocol->onErr(ret);
	}
	release();
}

void JhServerDataSwap::httpGetChallengeranklistCB(std::string retdata, int code, std::string tag)
{
	int ret = code;
	if (code == 0)
	{
		rapidjson::Document doc;
		if (JsonReader(retdata, doc))
		{
			JhGlobalData::vec_rankData.clear();
			JhGlobalData::myrank = 0;
			JhGlobalData::myFihgtCount = 0;

			JhGlobalData::myTotalFihgtCount = 0;

			if (doc.HasMember("ret"))
			{
				rapidjson::Value& v = doc["ret"];
				ret = v.GetInt();
			}

			if (ret == 0)
			{
				if (doc.HasMember("my"))
				{
					rapidjson::Value& dataobject = doc["my"];
					if (dataobject.HasMember("fightrank"))
					{
						rapidjson::Value& v = dataobject["fightrank"];
						JhGlobalData::myrank = v.GetInt();
					}
					if (dataobject.HasMember("challengecount"))
					{
						rapidjson::Value& v = dataobject["challengecount"];
						JhGlobalData::myFihgtCount = atoi(v.GetString());
					}
					if (dataobject.HasMember("finishedcount"))
					{
						rapidjson::Value& v = dataobject["finishedcount"];
						JhGlobalData::myTotalFihgtCount = JhGlobalData::myFihgtCount + atoi(v.GetString());
					}

					if (dataobject.HasMember("challengescore"))
					{
						rapidjson::Value& v = dataobject["challengescore"];
						JhGlobalData::myFihgtexp = atoi(v.GetString());
					}

				}
				for (int i = 1; i <= 3; i++)
				{
					std::string datastr = StringUtils::format("data%d", i);
					if (doc.HasMember(datastr.c_str()))
					{
						rapidjson::Value& dataArray = doc[datastr.c_str()];

						for (unsigned int m = 0; m < dataArray.Size(); m++)
						{
							RankData rdata;
							rapidjson::Value& item = dataArray[m];

							rapidjson::Value& v = item["fightrank"];
							rdata.rank = atoi(v.GetString());

							v = item["nickname"];
							rdata.nickname = v.GetString();

							if (item.HasMember("playerid"))
							{
								v = item["playerid"];
								rdata.playerid = v.GetString();
							}

							v = item["sex"];
							rdata.herosex = atoi(v.GetString());

							v = item["type"];
							rdata.herotype = atoi(v.GetString());

							v = item["exp"];
							int exp = atoi(v.GetString());
							int lv = 0;
							int size = JhGlobalData::map_heroAtr[rdata.herotype].vec_exp.size();
							for (int i = 0; i < size; i++)
							{
								if (exp > JhGlobalData::map_heroAtr[rdata.herotype].vec_exp[i])
								{
									lv = i;
									exp = exp - JhGlobalData::map_heroAtr[rdata.herotype].vec_exp[i];
								}
								else
								{
									break;
								}
							}
							rdata.herolv = lv;

							int heroval = 0;

							if (item.HasMember("fightingpower"))
							{
								v = item["fightingpower"];
								heroval = atoi(v.GetString());
							}
							else
								heroval = 0;

							rdata.heroval = heroval;
							JhGlobalData::vec_rankData.push_back(rdata);
						}
					}
				}
				if (m_pDelegateProtocol != NULL)
				{
					m_pDelegateProtocol->onSuccess();
				}
			}
			else
			{
				if (m_pDelegateProtocol != NULL)
					m_pDelegateProtocol->onErr(-ret);
			}

		}
	}
	else
	{
		if (m_pDelegateProtocol != NULL)
			m_pDelegateProtocol->onErr(ret);
	}
	release();
}


void JhServerDataSwap::httpGetMyFihgterDataCB(std::string retdata, int code, std::string tag)
{
	JhGlobalData::map_fighterPlayerData.clear();

	JhGlobalData::map_fightPlayerfriendly.clear();

	JhGlobalData::fightPlayerMixgf = "";
	JhGlobalData::fightPlayerFactionLv = 0;
	int ret = code;
	if (m_pDelegateProtocol != NULL)
	{
		if (code == 0)
		{
			rapidjson::Document doc;
			if (JsonReader(retdata, doc))
			{
				if (doc.HasMember("ret"))
				{
					rapidjson::Value& v = doc["ret"];
					ret = v.GetInt();

					if (doc.HasMember("data"))
					{
						rapidjson::Value& dataArray = doc["data"];
						if (dataArray.Size() > 0)
						{
							rapidjson::Value& item = dataArray[0];
							for (rapidjson::Value::ConstMemberIterator iter = item.MemberBegin(); iter != item.MemberEnd(); ++iter)
							{
								std::string keyname = iter->name.GetString();
								int keyval = atoi(iter->value.GetString());
								JhGlobalData::map_fighterPlayerData[keyname] = keyval;
							}
						}
					}

					std::string datastr;
					if (doc.HasMember("friendship"))
					{
						rapidjson::Value& item = doc["friendship"];
						datastr = item.GetString();
					}

					if (datastr.length() > 0)
					{
						std::vector<std::string> vec_retstr;
						JhCommonFuncs::split(datastr, vec_retstr, ";");
						for (unsigned int i = 0; i < vec_retstr.size(); i++)
						{
							std::vector<std::string> tmp;
							JhCommonFuncs::split(vec_retstr[i], tmp, ",");
							if (tmp.size() >= 3)
							{
								int friendly = atoi(tmp[1].c_str());
								if (friendly < -100000 || friendly > 100000)
									friendly = 0;
								JhGlobalData::map_fightPlayerfriendly[tmp[0]].friendly = friendly;
								JhGlobalData::map_fightPlayerfriendly[tmp[0]].relation = atoi(tmp[2].c_str());
							}
						}
					}

					if (doc.HasMember("mixgf"))
					{
						rapidjson::Value& item = doc["mixgf"];
						JhGlobalData::fightPlayerMixgf = item.GetString();
					}

					if (doc.HasMember("level"))
					{
						rapidjson::Value& v = doc["level"];
						JhGlobalData::fightPlayerFactionLv = v.GetInt();
					}
				}
			}
			if (ret == 0)
				m_pDelegateProtocol->onSuccess();
			else
				m_pDelegateProtocol->onErr(-ret);
		}
		else
			m_pDelegateProtocol->onErr(ret);
	}
	release();
}

void JhServerDataSwap::httpGetFightCountCB(std::string retdata, int code, std::string tag)
{
	int ret = code;
	if (m_pDelegateProtocol != NULL)
	{
		if (code == 0)
		{
			rapidjson::Document doc;
			if (JsonReader(retdata, doc))
			{
				if (doc.HasMember("ret"))
				{
					rapidjson::Value& v = doc["ret"];
					ret = v.GetInt();
				}
			}
			if (ret == 0)
				m_pDelegateProtocol->onSuccess();
			else
				m_pDelegateProtocol->onErr(-ret);
		}
		else
			m_pDelegateProtocol->onErr(ret);
	}
	release();
}

void JhServerDataSwap::httpGetChallengeResultCB(std::string retdata, int code, std::string tag)
{
	int ret = code;
	if (m_pDelegateProtocol != NULL)
	{
		if (code == 0)
		{
			rapidjson::Document doc;
			if (JsonReader(retdata, doc))
			{
				if (doc.HasMember("ret"))
				{
					rapidjson::Value& v = doc["ret"];
					ret = v.GetInt();
				}
			}
			if (ret == 0)
				m_pDelegateProtocol->onSuccess();
			else
				m_pDelegateProtocol->onErr(-ret);
		}
		else
			m_pDelegateProtocol->onErr(ret);
	}
	release();
}

void JhServerDataSwap::httpGetKajuanActionCB(std::string retdata, int code, std::string tag)
{
	int ret = code;
	if (m_pDelegateProtocol != NULL)
	{
		if (code == 0)
		{
			rapidjson::Document doc;
			if (JsonReader(retdata, doc))
			{
				if (doc.HasMember("ret"))
				{
					rapidjson::Value& v = doc["ret"];
					ret = v.GetInt();
				}
			}
			if (ret == 0)
				m_pDelegateProtocol->onSuccess();
			else
				m_pDelegateProtocol->onErr(-ret);
		}
		else
			m_pDelegateProtocol->onErr(ret);
	}
	release();
}
void JhServerDataSwap::httpGetKajuanAwardListCB(std::string retdata, int code, std::string tag)
{
	bool isok = false;
	JhGlobalData::myLastHuafeiRank = 0;
	if (code == 0)
	{
		rapidjson::Document doc;
		if (JsonReader(retdata, doc))
		{
			isok = true;

			if (doc.HasMember("lastrank"))
			{
				rapidjson::Value& v = doc["lastrank"];
				JhGlobalData::myLastHuafeiRank = v.GetInt();
			}
		}
	}

	if (isok)
	{
		if (m_pDelegateProtocol != NULL)
		{
			m_pDelegateProtocol->onSuccess();
		}
	}
	else
	{
		if (m_pDelegateProtocol != NULL)
		{
			m_pDelegateProtocol->onErr(-1);
		}
	}
	release();
}

void JhServerDataSwap::httpGetMyMatchInfoCB(std::string retdata, int code, std::string tag)
{
	bool isok = false;
	JhGlobalData::myMatchInfo.vec_factionlv.clear();
	JhGlobalData::myMatchInfo.matchaward = 0;
	for (int i = 0; i < 4; i++)
	{
		JhGlobalData::myMatchInfo.vec_factionlv.push_back(0);
	}
	if (code == 0)
	{
		rapidjson::Document doc;
		if (JsonReader(retdata, doc))
		{
			int ret = -1;
			if (doc.HasMember("ret"))
			{
				rapidjson::Value& v = doc["ret"];
				ret = v.GetInt();
			}
			if (ret == 0)
			{
				isok = true;

				if (doc.HasMember("startday"))
				{
					rapidjson::Value& v = doc["startday"];
					JhGlobalData::myMatchInfo.starttime = v.GetString();
				}
				if (doc.HasMember("endday"))
				{
					rapidjson::Value& v = doc["endday"];
					JhGlobalData::myMatchInfo.endtime = v.GetString();
				}
				if (doc.HasMember("matchno"))
				{
					rapidjson::Value& v = doc["matchno"];
					JhGlobalData::myMatchInfo.matchno = atoi(v.GetString());
				}
				if (doc.HasMember("score"))
				{
					rapidjson::Value& v = doc["score"];
					JhGlobalData::myMatchInfo.myexp = atoi(v.GetString());
				}
				if (doc.HasMember("wincount"))
				{
					rapidjson::Value& v = doc["wincount"];
					JhGlobalData::myMatchInfo.mywincount = atoi(v.GetString());
				}
				if (doc.HasMember("lostcount"))
				{
					rapidjson::Value& v = doc["lostcount"];
					JhGlobalData::myMatchInfo.myfailcount = atoi(v.GetString());
				}
				if (doc.HasMember("matchfinishedcount"))
				{
					rapidjson::Value& v = doc["matchfinishedcount"];
					JhGlobalData::myMatchInfo.finishedcount = atoi(v.GetString());
				}
				if (doc.HasMember("matchcount"))
				{
					rapidjson::Value& v = doc["matchcount"];
					JhGlobalData::myMatchInfo.leftcount = atoi(v.GetString());
				}
				if (doc.HasMember("matchaward"))
				{
					rapidjson::Value& v = doc["matchaward"];
					JhGlobalData::myMatchInfo.matchaward = atoi(v.GetString());
				}

				if (doc.HasMember("level1"))
				{
					rapidjson::Value& v = doc["level1"];
					JhGlobalData::myMatchInfo.vec_factionlv[0] = v.GetInt();
				}
				if (doc.HasMember("level2"))
				{
					rapidjson::Value& v = doc["level2"];
					JhGlobalData::myMatchInfo.vec_factionlv[1] = v.GetInt();
				}
				if (doc.HasMember("level3"))
				{
					rapidjson::Value& v = doc["level3"];
					JhGlobalData::myMatchInfo.vec_factionlv[2] = v.GetInt();
				}
				if (doc.HasMember("level4"))
				{
					rapidjson::Value& v = doc["level4"];
					JhGlobalData::myMatchInfo.vec_factionlv[3] = v.GetInt();
				}
			}
		}
	}

	if (isok)
	{
		if (m_pDelegateProtocol != NULL)
		{
			m_pDelegateProtocol->onSuccess();
		}
	}
	else
	{
		if (m_pDelegateProtocol != NULL)
		{
			m_pDelegateProtocol->onErr(-1);
		}
	}
	release();
}

void JhServerDataSwap::httpGetMatchFightCB(std::string retdata, int code, std::string tag)
{
	int ret = code;
	JhGlobalData::vec_matchPlayerData.clear();
	if (m_pDelegateProtocol != NULL)
	{
		if (code == 0)
		{
			rapidjson::Document doc;
			if (JsonReader(retdata, doc))
			{
				if (doc.HasMember("ret"))
				{
					rapidjson::Value& v = doc["ret"];
					ret = v.GetInt();

					if (ret == 0)
					{
						v = doc["matchscore"];
						JhGlobalData::matchPlayerInfo.exp = atoi(v.GetString());

						v = doc["matchwincount"];
						JhGlobalData::matchPlayerInfo.wincount = atoi(v.GetString());

						v = doc["matchlostcount"];
						JhGlobalData::matchPlayerInfo.failcount = atoi(v.GetString());

						v = doc["nickname"];
						JhGlobalData::matchPlayerInfo.nickname = v.GetString();

						v = doc["matchplayerid"];
						JhGlobalData::matchPlayerInfo.playerid = v.GetString();

						if (doc.HasMember("data"))
						{
							rapidjson::Value& playerArray = doc["data"];
							for (unsigned int i = 0; i < playerArray.Size(); i++)
							{
								rapidjson::Value& item = playerArray[i];
								MatchPlayerData mpdata;
								if (item.HasMember("holding"))
								{
									rapidjson::Value& dataArray = item["holding"];
									if (dataArray.Size() > 0)
									{
										rapidjson::Value& item = dataArray[0];
										for (rapidjson::Value::ConstMemberIterator iter = item.MemberBegin(); iter != item.MemberEnd(); ++iter)
										{
											std::string keyname = iter->name.GetString();
											int keyval = atoi(iter->value.GetString());
											mpdata.map_playerData[keyname] = keyval;
										}
									}
								}

								std::string datastr;
								if (item.HasMember("friendship"))
								{
									rapidjson::Value& v = item["friendship"];
									datastr = v.GetString();
								}

								if (datastr.length() > 0)
								{
									std::vector<std::string> vec_retstr;
									JhCommonFuncs::split(datastr, vec_retstr, ";");
									for (unsigned int i = 0; i < vec_retstr.size(); i++)
									{
										std::vector<std::string> tmp;
										JhCommonFuncs::split(vec_retstr[i], tmp, ",");
										if (tmp.size() >= 3)
										{
											int friendly = atoi(tmp[1].c_str());
											if (friendly < -100000 || friendly > 100000)
												friendly = 0;
											mpdata.map_playerfriendly[tmp[0]].friendly = friendly;
											mpdata.map_playerfriendly[tmp[0]].relation = atoi(tmp[2].c_str());
										}
									}
								}

								if (item.HasMember("mixgf"))
								{
									rapidjson::Value& v = item["mixgf"];
									mpdata.mixgf = v.GetString();
								}
								int herotype = 1;
								if (item.HasMember("type"))
								{
									rapidjson::Value& v = item["type"];
									mpdata.type = atoi(v.GetString());
									herotype = mpdata.type;
								}
								mpdata.factionlv = 0;
								if (item.HasMember("level"))
								{
									rapidjson::Value& v = item["level"];
									if (v.IsString())
										mpdata.factionlv = atoi(v.GetString());
									else if (v.IsInt())
										mpdata.factionlv = v.GetInt();

								}
								if (item.HasMember("exp"))
								{
									rapidjson::Value& v = item["exp"];
									int exp = atoi(v.GetString());
									int lv = 0;
									int size = JhGlobalData::map_heroAtr[herotype].vec_exp.size();
									for (int i = 0; i < size; i++)
									{
										if (exp > JhGlobalData::map_heroAtr[herotype].vec_exp[i])
										{
											lv = i;
											exp = exp - JhGlobalData::map_heroAtr[herotype].vec_exp[i];
										}
										else
										{
											break;
										}
									}
									if (lv >= size)
									{
										lv = size - 1;
									}
									mpdata.herolv = lv;
								}

								JhGlobalData::vec_matchPlayerData.push_back(mpdata);
							}
						}
					}
				}
			}
			if (ret == 0)
				m_pDelegateProtocol->onSuccess();
			else
				m_pDelegateProtocol->onErr(-ret);
		}
		else
			m_pDelegateProtocol->onErr(ret);
	}
	release();
}

void JhServerDataSwap::httpGetMatchFightResultCB(std::string retdata, int code, std::string tag)
{
	bool isok = false;
	if (code == 0)
	{
		rapidjson::Document doc;
		if (JsonReader(retdata, doc))
		{
			rapidjson::Value& v = doc["ret"];
			int ret = v.GetInt();

			if (ret == 0)
			{
				isok = true;

				rapidjson::Value& v = doc["before"];
				JhGlobalData::myMatchInfo.beforerank = v.GetInt();

				v = doc["after"];
				JhGlobalData::myMatchInfo.afterrank = v.GetInt();
			}
		}
	}

	if (isok)
	{
		if (m_pDelegateProtocol != NULL)
		{
			m_pDelegateProtocol->onSuccess();
		}
	}
	else
	{
		if (m_pDelegateProtocol != NULL)
		{
			m_pDelegateProtocol->onErr(-1);
		}
	}
	release();
}

void JhServerDataSwap::httpGetHSLJRankDataCB(std::string retdata, int code, std::string tag)
{
	bool isok = false;
	JhGlobalData::vec_hsljRankData.clear();
	if (code == 0)
	{
		rapidjson::Document doc;
		if (JsonReader(retdata, doc))
		{
			isok = true;
			if (doc.HasMember("data"))
			{
				rapidjson::Value& dataArray = doc["data"];
				for (unsigned int i = 0; i < dataArray.Size(); i++)
				{
					rapidjson::Value& item = dataArray[i];
					HSLJRankData data;
					data.rank = i + 1;
					rapidjson::Value& v = item["nickname"];
					data.nickname = v.GetString();

					v = item["matchscore"];
					data.exp = atoi(v.GetString());

					v = item["wincount"];
					data.wincount = atoi(v.GetString());
					v = item["lostcount"];
					int failcount = atoi(v.GetString());
					int totalcount = data.wincount + failcount;
					if (totalcount > 0)
					{
						data.totalcount = totalcount;
						JhGlobalData::vec_hsljRankData.push_back(data);
					}
				}
			}
		}
	}

	if (isok)
	{
		if (m_pDelegateProtocol != NULL)
		{
			m_pDelegateProtocol->onSuccess();
		}
	}
	else
	{
		if (m_pDelegateProtocol != NULL)
		{
			m_pDelegateProtocol->onErr(-1);
		}
	}
	release();
}

void JhServerDataSwap::httpGetHSLJRewardDataCB(std::string retdata, int code, std::string tag)
{
	bool isok = false;
	if (code == 0)
	{
		rapidjson::Document doc;
		if (JsonReader(retdata, doc))
		{
			if (doc.HasMember("ret"))
			{
				rapidjson::Value& v = doc["ret"];
				int ret = v.GetInt();
				if (ret == 0)
					isok = true;
			}
		}
	}

	if (isok)
	{
		if (m_pDelegateProtocol != NULL)
		{
			m_pDelegateProtocol->onSuccess();
		}
	}
	else
	{
		if (m_pDelegateProtocol != NULL)
		{
			m_pDelegateProtocol->onErr(-1);
		}
	}
	release();
}

void JhServerDataSwap::httpGetCommonDataCB(std::string retdata, int code, std::string tag)
{
	JhGlobalData::vec_qq.clear();
	bool isok = false;
	if (code == 0)
	{
		rapidjson::Document doc;
		if (JsonReader(retdata, doc))
		{
			if (doc.HasMember("ret"))
			{
				rapidjson::Value& v = doc["ret"];
				int ret = v.GetInt();
				if (ret == 0)
					isok = true;

			}
			if (doc.HasMember("qq"))
			{
				rapidjson::Value& v = doc["qq"];
				std::string qqstr = v.GetString();
				if (qqstr.length() > 0)
					JhCommonFuncs::split(v.GetString(), JhGlobalData::vec_qq);
			}
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#else
			if (doc.HasMember("online"))
			{
				rapidjson::Value& v = doc["online"];
				JhGlobalData::isOnline = v.GetInt() == 1?true : false;
			}
#endif
		}
	}

	if (isok)
	{
		if (m_pDelegateProtocol != NULL)
		{
			m_pDelegateProtocol->onSuccess();
		}
	}
	else
	{
		if (m_pDelegateProtocol != NULL)
		{
			m_pDelegateProtocol->onErr(-1);
		}
	}
	release();
}

void JhServerDataSwap::httpGetRechargeDataCB(std::string retdata, int code, std::string tag)
{
	bool isok = false;
	JhGlobalData::recharageData.rtime = "";
	JhGlobalData::recharageData.myrechage = 0;
	JhGlobalData::recharageData.rewardstr = "";
	if (code == 0)
	{
		rapidjson::Document doc;
		if (JsonReader(retdata, doc))
		{
			if (doc.HasMember("ret"))
			{
				rapidjson::Value& v = doc["ret"];
				int ret = v.GetInt();
				if (ret == 0)
					isok = true;

			}
			if (doc.HasMember("date"))
			{
				rapidjson::Value& v = doc["date"];
				JhGlobalData::recharageData.rtime = v.GetString();

			}

			if (doc.HasMember("award"))
			{
				rapidjson::Value& v = doc["award"];
				JhGlobalData::recharageData.rewardstr = v.GetString();
			}
			if (doc.HasMember("topup"))
			{
				rapidjson::Value& v = doc["topup"];
				JhGlobalData::recharageData.myrechage = atoi(v.GetString());
			}
			if (doc.HasMember("gotten"))
			{
				rapidjson::Value& v = doc["gotten"];
				JhGlobalData::recharageData.mygotton = atoi(v.GetString());
			}

			
		}
	}

	if (isok)
	{
		if (m_pDelegateProtocol != NULL)
		{
			m_pDelegateProtocol->onSuccess();
		}
	}
	else
	{
		if (m_pDelegateProtocol != NULL)
		{
			m_pDelegateProtocol->onErr(-1);
		}
	}
	release();
}

void JhServerDataSwap::httpPostMyRechargeCB(std::string retdata, int code, std::string tag)
{
	bool isok = false;
	if (code == 0)
	{
		rapidjson::Document doc;
		if (JsonReader(retdata, doc))
		{
			if (doc.HasMember("ret"))
			{
				rapidjson::Value& v = doc["ret"];
				int ret = v.GetInt();
				if (ret == 0)
					isok = true;

			}
		}
	}

	if (isok)
	{
		if (m_pDelegateProtocol != NULL)
		{
			m_pDelegateProtocol->onSuccess();
		}
	}
	else
	{
		if (m_pDelegateProtocol != NULL)
		{
			m_pDelegateProtocol->onErr(-1);
		}
	}
	release();
}