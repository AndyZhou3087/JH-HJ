#include "JhGlobalData.h"
#include "JhBuilding.h"
#include "JhStorageRoom.h"
#include "JhMyPackage.h"
#include "JhGameDataSave.h"
#include "JhCommonFuncs.h"
#include "json.h"
#include "JhConst.h"
#include "MD5.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "iosfunc.h"
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"
#endif

std::map<std::string, std::vector<BuildActionData>> JhGlobalData::map_buidACData;

std::map<std::string, AllResource> JhGlobalData::map_allResource;

std::vector<ResData> JhGlobalData::vec_resData;

std::vector<std::string> JhGlobalData::vec_hillResid;

std::map<std::string, MapData> JhGlobalData::map_maps;

std::map<std::string, NpcData> JhGlobalData::map_npcs;

std::map<int, HeroAtrData> JhGlobalData::map_heroAtr;

std::map<std::string, WG_NGData> JhGlobalData::map_wgngs;

std::map<std::string, EquipData> JhGlobalData::map_equips;

std::vector<PlotMissionData> JhGlobalData::vec_PlotMissionData;

std::map<std::string, std::vector<PlotMissionData>> JhGlobalData::map_BranchPlotMissionData;
std::map<std::string, BranchPlotMissionItem> JhGlobalData::map_BranchPlotMissionItem;

std::map<std::string, GFTrickData> JhGlobalData::map_gftricks;

std::map<int, GFSkillData> JhGlobalData::map_gfskills;

std::vector<GoodsData> JhGlobalData::vec_goods;

std::vector<std::string> JhGlobalData::vec_buyVipIds;

std::map<std::string, int> JhGlobalData::map_buyVipDays;


std::map<std::string, ChallengeRewardData> JhGlobalData::map_challengeReward;
std::map<std::string, FriendlyData> JhGlobalData::map_myfriendly;

std::map<std::string, NPCFriendData> JhGlobalData::map_NPCFriendData;

std::map<std::string, NPCMasterData> JhGlobalData::map_NPCMasterData;

std::map<std::string, MixGfData> JhGlobalData::map_MixGfData;

std::vector<RankData> JhGlobalData::vec_rankData;

std::vector<FactionListData> JhGlobalData::vec_factionListData;

std::vector<FactionMemberData> JhGlobalData::vec_factionMemberData;

int JhGlobalData::factionExp = 0;

std::vector<AchiveData> JhGlobalData::vec_achiveData;
std::vector<std::string> JhGlobalData::vec_showAchiveNames;

bool JhGlobalData::unlockhero[4] = {true, false, false, false};

std::string JhGlobalData::uid = "";

std::vector<std::string> JhGlobalData::vec_saveids;

bool JhGlobalData::hasBuy = false;
int JhGlobalData::adsinterval = 10;
long JhGlobalData::adschoose = 11111111;

bool JhGlobalData::isPopingScene = false;

int JhGlobalData::myGlodCount = 0;

GameStatus JhGlobalData::g_gameStatus = GAMEOVER;
int JhGlobalData::wxbmapos = 0;
int JhGlobalData::dgqbmapos = 0;

int JhGlobalData::timegiftlefttime = 0;

bool JhGlobalData::isBuyTimeGift = false;

int JhGlobalData::freeReviveCount = 0;

std::map<std::string, std::vector<std::string>> JhGlobalData::map_tempGf_Equip;

std::string JhGlobalData::MD5MyGoldCount;
std::string JhGlobalData::MD5CostGlodCount;
std::string JhGlobalData::MD5FreeReviveCount;
std::string JhGlobalData::MD5HeroLv;
std::string JhGlobalData::MD5LiveDays;
bool JhGlobalData::dataIsModified = false;
bool JhGlobalData::ispunishment = false;
bool JhGlobalData::isFightMaster = false;
int JhGlobalData::servertime = 0;

int JhGlobalData::myrank = 0;
std::string JhGlobalData::noticecontent;

int JhGlobalData::myFaction = 0;
int JhGlobalData::mytitle = 0;

int JhGlobalData::myFactionlv = 0;

MyLotteryData JhGlobalData::myLotteryData;

RaffleData JhGlobalData::myRaffleData;

int JhGlobalData::continueLoginDays = 0;

bool JhGlobalData::isFrozen = false;

int JhGlobalData::myFihgtCount = 0;
int JhGlobalData::myTotalFihgtCount = 0;
int JhGlobalData::myFihgtexp = 0;

std::map<std::string, int> JhGlobalData::map_fighterPlayerData;

std::map<std::string, FriendlyData> JhGlobalData::map_fightPlayerfriendly;

std::string JhGlobalData::fightPlayerMixgf;

int JhGlobalData::fightPlayerFactionLv = 0;
bool JhGlobalData::isPlayerChallenging = false;

bool JhGlobalData::isExchangeGift = false;

int JhGlobalData::myLastHuafeiRank = 0;

std::string JhGlobalData::couponinfo;

MyMatchInfoData JhGlobalData::myMatchInfo;

MatchPlayerInfo JhGlobalData::matchPlayerInfo;
std::vector<MatchPlayerData> JhGlobalData::vec_matchPlayerData;

std::vector<HSLJRankData> JhGlobalData::vec_hsljRankData;

std::string JhGlobalData::updateDownLoadURL;

bool JhGlobalData::isPopUpdate = false;

std::string JhGlobalData::curBranchMissionID;

std::vector<std::string> JhGlobalData::vec_qq;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
bool JhGlobalData::isOnline = true;
#else
bool JhGlobalData::isOnline = false;
#endif


RechargeData JhGlobalData::recharageData;

bool JhGlobalData::isRecharge = false;

JhGlobalData::JhGlobalData()
{

}


JhGlobalData::~JhGlobalData()
{

}

void JhGlobalData::init()
{
	myGlodCount = JhGameDataSave::getInstance()->getGoldCount();
	MD5MyGoldCount = md5(myGlodCount);
	isBuyTimeGift = JhGameDataSave::getInstance()->getIsBuyTimeGift();
	myFaction = 0;
	mytitle = 0;
	myFactionlv = 0;
}

void JhGlobalData::loadBuildActionJSon()
{
	map_buidACData.clear();

	rapidjson::Document doc = ReadJsonFile("data/buildings.json");
	rapidjson::Value& allBuilds = doc["b"];
	for (unsigned int i = 0; i < allBuilds.Size(); i++)
	{
		rapidjson::Value& oneBuild = allBuilds[i];
		rapidjson::Value& oneitem = oneBuild["name"];
		std::string buildname = oneitem.GetString();

		std::string jsonfilename = StringUtils::format("data/%s.json", buildname.c_str());
		rapidjson::Document doc = ReadJsonFile(jsonfilename);
		rapidjson::Value& bc = doc["bc"];
		for (unsigned int i = 0; i < bc.Size(); i++)//建筑物数组
		{
			BuildActionData data;
			rapidjson::Value& jsonvalue = bc[i];
			if (jsonvalue.IsObject())
			{
				rapidjson::Value& value = jsonvalue["id"];
				strcpy(data.icon, value.GetString());

				value = jsonvalue["blv"];
				data.blv = atoi(value.GetString());

				value = jsonvalue["time"];
				data.actime = atoi(value.GetString());

				if (jsonvalue.HasMember("extime"))
				{
					value = jsonvalue["extime"];
					data.extime = atoi(value.GetString());
				}
				else
					data.extime = 0;

				if (jsonvalue.HasMember("type"))
				{
					value = jsonvalue["type"];
					data.type = atoi(value.GetString());
				}
				else
					data.type = -1;

				value = jsonvalue["actext"];
				strcpy(data.actext, value.GetString());

				if (jsonvalue.HasMember("ep"))
				{
					value = jsonvalue["ep"];

					for (unsigned int m = 0; m < value.Size(); m++)
					{
						data.ep.push_back(value[m].GetInt());
					}
				}

				if (jsonvalue.HasMember("extype"))
				{
					value = jsonvalue["extype"];
					data.extype = atoi(value.GetString());
				}
				else
					data.extype = 0;

				value = jsonvalue["res"];
				for (unsigned int i = 0; i < value.Size(); i++)
				{
					int tmp = value[i].GetInt();
					if (tmp > 0)
						data.res.push_back(value[i].GetInt());
				}
				if (jsonvalue.HasMember("name"))
				{
					value = jsonvalue["name"];
					data.cname = value.GetString();
				}
				else
				{
					data.cname = "";
				}
				if (jsonvalue.HasMember("desc"))
				{
					value = jsonvalue["desc"];
					data.desc = value.GetString();
				}
				else
				{
					data.desc = "";
				}

				map_buidACData[buildname].push_back(data);
			}
		}
	}
}

void JhGlobalData::loadResJsonData()
{
	vec_resData.clear();
	rapidjson::Document doc = ReadJsonFile("data/res.json");
	rapidjson::Value& values = doc["r"];
	for (unsigned int i = 0; i < values.Size(); i++)//一级资源数组
	{
		ResData data;
		rapidjson::Value& item = values[i];
		rapidjson::Value& v = item["id"];
		data.strid = v.GetString();
		v = item["max"];
		data.max = atoi(v.GetString());

		data.count = data.max;
		data.pastmin = 0.0f;
		data.waittime = 0.0f;
		v = item["perhour"];

		for (unsigned int m = 0; m < v.Size(); m++)
		{
			data.speed.push_back(v[m].GetDouble() * 60);
		}

		v = item["type"];
		data.type = atoi(v.GetString());
		v = item["actype"];
		data.actype = atoi(v.GetString());
		v = item["ep"];

		for (unsigned int m = 0; m < v.Size(); m++)
		{
			data.ep.push_back(v[m].GetInt());
		}
		v = item["res"];
		for (unsigned int m = 0; m < v.Size(); m++)
		{
			data.res.push_back(v[m].GetInt());
		}

		v = item["unit"];
		data.unitname = v.GetString();
		vec_resData.push_back(data);

	}
}

void JhGlobalData::loadAllResourceJsonData()
{
	map_allResource.clear();
	rapidjson::Document doc = ReadJsonFile("data/allresouces.json");
	rapidjson::Value& values = doc["rd"];
	for (unsigned int i = 0; i < values.Size(); i++)//一级资源数组
	{
		AllResource data;
		rapidjson::Value& item = values[i];
		rapidjson::Value& v = item["id"];
		data.strid = v.GetString();

		v = item["cname"];
		data.cname = v.GetString();
		v = item["desc"];
		data.desc = v.GetString();

		v = item["val"];
		data.val = atoi(v.GetString());

		v = item["fval"];
		data.fval = atoi(v.GetString());
		
		v = item["npcs"];

		for (unsigned int m = 0; m < v.Size(); m++)
		{
			rapidjson::Value& npsv = v[m];
			std::string npcid = npsv.GetString();
			if (npcid.length() > 1)
				data.npc.push_back(npcid);
		}

		v = item["npcval"];

		for (unsigned int m = 0; m < v.Size(); m++)
		{
			rapidjson::Value& npsv = v[m];
			std::string npcval = npsv.GetString();
			if (npcval.length() > 0)
				data.npcval.push_back(atof(npcval.c_str()));
		}

		map_allResource[data.strid]= data;

	}
}

void JhGlobalData::loadHillResJsonData()
{
	vec_hillResid.clear();
	rapidjson::Document doc = ReadJsonFile("data/homehill.json");
	rapidjson::Value& values = doc["sh"];
	for (unsigned int i = 0; i < values.Size(); i++)//后山显示的资源列表
	{
		vec_hillResid.push_back(values[i].GetString());
	}
}

void JhGlobalData::loadMapJsonData()
{
	map_maps.clear();
	rapidjson::Document doc = ReadJsonFile("data/map.json");
	rapidjson::Value& values = doc["m"];
	for (unsigned int i = 0; i < values.Size(); i++)//地图地点数组
	{
		MapData data;
		rapidjson::Value& item = values[i];
		rapidjson::Value& v = item["id"];
		strcpy(data.strid, v.GetString());
		v = item["npc"];

		for (unsigned int m = 0; m < v.Size(); m++)
		{
			rapidjson::Value& npsv = v[m];
			std::string npcid = npsv.GetString();
			if (npcid.compare("0") != 0)
				data.npcs.push_back(npcid);
		}
		v = item["cname"];
		strcpy(data.cname, v.GetString());

		v = item["t"];
		strcpy(data.tpngname, v.GetString());

		v = item["desc"];
		data.desc = v.GetString();

		v = item["qy"];
		int iscliff = atoi(v.GetString());
		data.isCliff = iscliff == 1 ? true : false;

		map_maps[data.strid] = data;

	}
}

void JhGlobalData::loadNpcJsonData()
{
	map_npcs.clear();
	rapidjson::Document doc = ReadJsonFile("data/npc.json");
	rapidjson::Value& values = doc["n"];
	for (unsigned int i = 0; i < values.Size(); i++)//npc数组
	{
		NpcData data;
		rapidjson::Value& item = values[i];
		rapidjson::Value& v = item["id"];
		strcpy(data.id, v.GetString());
		v = item["name"];
		strcpy(data.name, v.GetString());
		v = item["type"];
		data.type = atoi(v.GetString());
		v = item["life"];
		data.life = atoi(v.GetString());
		v = item["atk"];
		data.atk = atoi(v.GetString());
		v = item["df"];
		data.df = atoi(v.GetString());
		v = item["exp"];
		data.exp = atoi(v.GetString());

		v = item["lv"];
		data.lv = atoi(v.GetString());

		v = item["es"];
		data.escapernd = atoi(v.GetString());
		

		v = item["winres"];
		for (unsigned int m = 0; m < v.Size(); m++)
		{
			std::string str = v[m].GetString();
			if (str.length() > 1)
				data.winres.push_back(str);
		}
		v = item["winresrnd"];
		for (unsigned int m = 0; m < v.Size(); m++)
		{
			std::string str = v[m].GetString();
			int rnd = atoi(str.c_str());
			if (rnd > 0)
			{
				data.winresrnd.push_back(rnd);
				data.winrescount.push_back(-1);
			}
		}
		v = item["exchg"];
		for (unsigned int m = 0; m < v.Size(); m++)
		{
			std::string str = v[m].GetString();
			if (str.length() > 1)
				data.exchgres.push_back(str);
		}
		v = item["word"];
		for (unsigned int m = 0; m < v.Size(); m++)
		{
			std::string str = v[m].GetString();
			if (str.length() > 0)
				data.words.push_back(str);
		}
		v = item["avoid"];
		data.dodge = atof(v.GetString());

		v = item["crit"];
		data.crit = atof(v.GetString());

		v = item["skrnd"];
		data.skillrnd = atof(v.GetString());

		v = item["skef"];
		data.skilleffect = atoi(v.GetString());

		v = item["sk"];
		data.skilltype = atoi(v.GetString());

		data.skillleftval = 0;

		map_npcs[data.id] = data;
	}
}

void JhGlobalData::saveResData()
{
	//保存资源数据每项分号（;）分割
	std::string str;
	for (unsigned int i = 0; i < vec_resData.size(); i++)
	{
		std::string onestr = StringUtils::format("%d-%.1f-%.1f;", vec_resData[i].count, vec_resData[i].pastmin, vec_resData[i].waittime);
		str.append(onestr);
	}
	JhGameDataSave::getInstance()->setResData(str.substr(0, str.length() - 1));
}

void JhGlobalData::loadResData()
{
	//解析保存的资源数据
	std::string datastr = JhGameDataSave::getInstance()->getResData();
	if (datastr.length() > 0)
	{
		std::vector<std::string> vec_retstr;
		JhCommonFuncs::split(datastr, vec_retstr, ";");
		for (unsigned int i = 0; i < vec_retstr.size(); i++)
		{
			std::vector<std::string> tmp;
			JhCommonFuncs::split(vec_retstr[i], tmp, "-");
			vec_resData[i].count = atoi(tmp[0].c_str());
			vec_resData[i].pastmin = atof(tmp[1].c_str());
			vec_resData[i].waittime = atof(tmp[2].c_str());
		}
	}
	else
	{
		for (unsigned int i = 0; i < vec_resData.size(); i++)
		{
			vec_resData[i].count = vec_resData[i].max;
			vec_resData[i].pastmin = 0.0f;
			vec_resData[i].waittime = 0.0f;
		}
	}
}

void JhGlobalData::loadHeroAtrJsonData()
{
	map_heroAtr.clear();
	rapidjson::Document doc = ReadJsonFile("data/heroatr.json");
	rapidjson::Value& values = doc["h"];
	for (unsigned int i = 0; i < values.Size(); i++)//角色数组4个角色
	{
		rapidjson::Value& vitem = values[i];
		HeroAtrData data;
		rapidjson::Value& v = vitem["id"];
		data.id = atoi(v.GetString());

		v = vitem["name"];
		strcpy(data.name, v.GetString());

		v = vitem["atk"];
		for (unsigned int j = 0; j < v.Size(); j++)
		{
			data.vec_atk.push_back(v[j].GetInt());
		}

		v = vitem["df"];
		for (unsigned int j = 0; j < v.Size(); j++)
		{
			data.vec_df.push_back(v[j].GetInt());
		}

		v = vitem["exp"];
		for (unsigned int j = 0; j < v.Size(); j++)
		{
			data.vec_exp.push_back(v[j].GetInt());
		}

		v = vitem["maxhp"];
		for (unsigned int j = 0; j < v.Size(); j++)
		{
			data.vec_maxhp.push_back(v[j].GetInt());
		}
		v = vitem["crit"];
		for (unsigned int j = 0; j < v.Size(); j++)
		{
			data.vec_crit.push_back(v[j].GetDouble());
		}
		v = vitem["avoid"];
		for (unsigned int j = 0; j < v.Size(); j++)
		{
			data.vec_dodge.push_back(v[j].GetDouble());
		}
		map_heroAtr[data.id] = data;
	}
}

void JhGlobalData::loadWG_NGJsonData()
{
	map_wgngs.clear();
	rapidjson::Document doc = ReadJsonFile("data/wg.json");
	rapidjson::Value& values = doc["w"];
	for (unsigned int i = 0; i < values.Size(); i++)//外功数组
	{
		rapidjson::Value& vitem = values[i];
		WG_NGData data;
		rapidjson::Value& v = vitem["id"];
		strcpy(data.id, v.GetString());

		v = vitem["maxlv"];
		data.maxlv = atoi(v.GetString());

		v = vitem["bns"];
		for (unsigned int j = 0; j < v.Size(); j++)
		{
			data.vec_bns.push_back(v[j].GetInt());
		}

		v = vitem["exp"];
		for (unsigned int j = 0; j < v.Size(); j++)
		{
			data.vec_exp.push_back(v[j].GetInt());
		}

		v = vitem["crit"];
		for (unsigned int j = 0; j < v.Size(); j++)
		{
			data.vec_cirt.push_back(v[j].GetDouble());
		}

		v = vitem["skrnd"];
		for (unsigned int j = 0; j < v.Size(); j++)
		{
			data.vec_skrnd.push_back(v[j].GetDouble());
		}

		v = vitem["sk"];
		data.skilltype = atoi(v.GetString());

		v = vitem["skef"];
		data.skilleffect = atoi(v.GetString());

		data.type = W_GONG + 1;
		v = vitem["qu"];
		data.qu = atoi(v.GetString());

		v = vitem["type"];
		data.extype = atoi(v.GetString());

		map_wgngs[data.id] = data;
	}

	doc = ReadJsonFile("data/ng.json");
	rapidjson::Value& nvalues = doc["n"];
	int a = nvalues.Size();
	for (unsigned int i = 0; i < nvalues.Size(); i++)//内功数组
	{
		rapidjson::Value& vitem = nvalues[i];
		WG_NGData data;
		rapidjson::Value& v = vitem["id"];
		strcpy(data.id, v.GetString());

		v = vitem["maxlv"];
		data.maxlv = atoi(v.GetString());

		v = vitem["bns"];
		for (unsigned int j = 0; j < v.Size(); j++)
		{
			data.vec_bns.push_back(v[j].GetInt());
		}

		v = vitem["exp"];
		for (unsigned int j = 0; j < v.Size(); j++)
		{
			data.vec_exp.push_back(v[j].GetInt());
		}
		v = vitem["avoid"];
		for (unsigned int j = 0; j < v.Size(); j++)
		{
			data.vec_dodge.push_back(v[j].GetDouble());
		}

		v = vitem["skrnd"];
		for (unsigned int j = 0; j < v.Size(); j++)
		{
			data.vec_skrnd.push_back(v[j].GetDouble());
		}

		v = vitem["sk"];
		data.skilltype = atoi(v.GetString());

		v = vitem["skef"];
		data.skilleffect = atoi(v.GetString());

		data.type = N_GONG + 1;
		data.extype = 0;
		v = vitem["qu"];
		data.qu = atoi(v.GetString());

		map_wgngs[data.id] = data;
	}
}

void JhGlobalData::loadEquipJsonData()
{
	map_equips.clear();
	rapidjson::Document doc = ReadJsonFile("data/equip.json");
	rapidjson::Value& values = doc["ae"];
	for (unsigned int i = 0; i < values.Size(); i++)//武器防具数组
	{
		EquipData data;
		rapidjson::Value& item = values[i];
		rapidjson::Value& v = item["id"];
		data.id = v.GetString();
		v = item["type"];
		data.type = atoi(v.GetString());
		v = item["atk"];
		data.atk = atoi(v.GetString());
		v = item["df"];
		data.df = atoi(v.GetString());
		v = item["extype"];
		data.extype = atoi(v.GetString());
		v = item["qu"];
		data.qu = atoi(v.GetString());

		v = item["res"];
		for (unsigned int m = 0; m < v.Size(); m++)
		{
			int val = v[m].GetInt();
			if (val > 0)
			{
				std::string str = StringUtils::format("%d", val);
				data.repair_res.push_back(str);
			}
		}
		map_equips[data.id] = data;
	}
}

void JhGlobalData::loadUnlockHeroData()
{
	//解析角色是否解锁"-"分割
	std::string str = JhGameDataSave::getInstance()->getHeroUnlockData();
	std::vector<std::string> tmp;
	JhCommonFuncs::split(str, tmp, "-");

	for (unsigned int i = 0; i < tmp.size(); i++)
	{
		unlockhero[i] = atoi(tmp[i].c_str()) == 1 ? true : false;	
	}
	unlockhero[0] = true;
}

void JhGlobalData::setUnlockHero(int index, bool val)
{
	//保存角色是否解锁"-"分割
	unlockhero[index] = val;
	std::string str;
	for (int i = 0; i < 4; i++)
	{
		std::string tmp = StringUtils::format("%d-", unlockhero[i]?1:0);
		str.append(tmp);
	}
	JhGameDataSave::getInstance()->setHeroUnlockData(str.substr(0, str.length() - 1));
}

bool JhGlobalData::getUnlocHero(int index)
{
	return unlockhero[index];
}

int JhGlobalData::getSysSecTime()
{
	time_t timep;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	time(&timep);
#else
	struct timeval tv;
	gettimeofday(&tv, NULL);
	timep = tv.tv_sec;
#endif
	return timep;
}

int JhGlobalData::getDayOfYear()
{
	struct tm *tm;
	time_t timep;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	time(&timep);
#else
	struct timeval tv;
	gettimeofday(&tv, NULL);
	timep = tv.tv_sec;
#endif
	tm = localtime(&timep);
	int day = tm->tm_yday;
	return day;
}

int JhGlobalData::getMonth_Days()
{
	struct tm *tm;
	time_t timep;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	time(&timep);
#else
	struct timeval tv;
	gettimeofday(&tv, NULL);
	timep = tv.tv_sec;
#endif
	tm = localtime(&timep);
	int month = tm->tm_mon;
	int days[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	int day = days[month];
	int year = tm->tm_year;
	if ((year % 4 == 0 && year % 100 != 0 || year % 400 == 0) && month == 2)
		day += 1;
	return day;
}

int JhGlobalData::getTomorrowZeroTimeDif()
{
	struct tm *tm;
	time_t timep;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	time(&timep);
#else
	struct timeval tv;
	gettimeofday(&tv, NULL);
	timep = tv.tv_sec;
#endif
	tm = localtime(&timep);
	int hour = tm->tm_hour;
	int min = tm->tm_min;
	int sec = tm->tm_sec;

	int dif = (23 - hour) * 3600 + (59 - min) * 60 + (60 - sec);
	return dif;
}

std::string JhGlobalData::getUId()
{
	uid = JhGameDataSave::getInstance()->getUserId();
	return uid;
}
void JhGlobalData::setUId(std::string struid)
{
	uid = struid;
	JhGameDataSave::getInstance()->saveUserId(struid);
}

std::string JhGlobalData::getDefaultStorage(int heroindex)
{
	//默认仓库的数据
	
	std::string defaultdata = JhGameDataSave::getInstance()->getModifyDefaultStorage(heroindex);
	if (defaultdata.length() > 0)
		return defaultdata;
	else
	{
		return getOriginLocalStorage(heroindex);
	}
	return "";
}

std::string JhGlobalData::getOriginLocalStorage(int heroindex)
{
	rapidjson::Document doc = ReadJsonFile("data/defaultstorage.json");
	rapidjson::Value& values = doc["ds"];

	int size = values.Size();
	if (size > 0 && heroindex <= size)
	{
		rapidjson::Value& item = values[heroindex - 1];
		rapidjson::Value& v = item["val"];
		return v.GetString();
	}
	return "";
}

void JhGlobalData::setPlotMissionIndex(int val)
{
	JhGameDataSave::getInstance()->setPlotMissionIndex(val);
}

int JhGlobalData::getPlotMissionIndex()
{
	//兼容上一版，已经到通关了，最大的INDEX溢出
	int plotindex = JhGameDataSave::getInstance()->getPlotMissionIndex();
	int max = vec_PlotMissionData.size();

	if (plotindex >= max)
		plotindex = max - 1;
	//迭代2个版本后删除 start
	if (vec_PlotMissionData[plotindex].dnpc.compare("n089") == 0 && vec_PlotMissionData[plotindex].status == M_DONE)
	{
		plotindex++;
	}
	if (plotindex >= max)
		plotindex = max - 1;
	//迭代2个版本后删除 end
	return plotindex;
}

void JhGlobalData::loadPlotMissionJsonData(int herotype)
{
	vec_PlotMissionData.clear();
	std::string plotfilename = StringUtils::format("data/plotmission%d.json", herotype);

	rapidjson::Document doc = ReadJsonFile(plotfilename);
	rapidjson::Value& values = doc["m"];
	for (unsigned int i = 0; i < values.Size(); i++)//剧情数组
	{
		rapidjson::Value& vitem = values[i];
		PlotMissionData data;
		rapidjson::Value& v = vitem["id"];
		data.id = v.GetString();

		v = vitem["snpc"];
		data.snpc = v.GetString();

		v = vitem["dnpc"];
		data.dnpc = v.GetString();

		if (vitem.HasMember("map"))
		{
			v = vitem["map"];
			data.mapid = v.GetString();
		}
		else
		{
			data.mapid = "";
		}

		v = vitem["unlock"];
		data.unlockchapter = atoi(v.GetString());

		if (data.dnpc.compare("n089") == 0)
		{
			data.unlockchapter = 13;
		}
		v = vitem["t"];
		data.type = atoi(v.GetString());

		data.status = M_NONE;

		v = vitem["word"];
		for (unsigned int j = 0; j < v.Size(); j++)
		{
			std::string str = v[j].GetString();
			if (str.length() > 0)
				data.words.push_back(str);
		}

		v = vitem["myword"];
		for (unsigned int j = 0; j < v.Size(); j++)
		{
			std::string str = v[j].GetString();
			if (str.length() > 0)
				data.mywords.push_back(str);
		}

		v = vitem["bossword"];
		for (unsigned int j = 0; j < v.Size(); j++)
		{
			std::string str = v[j].GetString();
			if (str.length() > 0)
				data.bossword.push_back(str);
		}
		v = vitem["rwds"];
		for (unsigned int j = 0; j < v.Size(); j++)
		{
			std::string str = v[j].GetString();
			if (str.length() > 1)
				data.rewords.push_back(str);
		}
		vec_PlotMissionData.push_back(data);
	}

	JhGlobalData::updatePlotMissionStatus();
}

void JhGlobalData::loadBranchPlotMissionJsonData()
{
	map_BranchPlotMissionData.clear();
	map_BranchPlotMissionItem.clear();
	std::string plotfilename = "data/branchplotmission.json";

	rapidjson::Document doc = ReadJsonFile(plotfilename);
	rapidjson::Value& values = doc["m"];
	for (unsigned int i = 0; i < values.Size(); i++)//剧情数组
	{
		rapidjson::Value& vitem = values[i];
		PlotMissionData data;
		rapidjson::Value& v = vitem["id"];
		std::string mid = v.GetString();
		data.id = mid;
		v = vitem["snpc"];
		data.snpc = v.GetString();

		v = vitem["dnpc"];
		data.dnpc = v.GetString();

		v = vitem["unlock"];
		data.unlockchapter = atoi(v.GetString());

		v = vitem["t"];
		data.type = atoi(v.GetString());

		data.status = M_NONE;

		v = vitem["word"];
		for (unsigned int j = 0; j < v.Size(); j++)
		{
			std::string str = v[j].GetString();
			if (str.length() > 0)
				data.words.push_back(str);
		}

		v = vitem["myword"];
		for (unsigned int j = 0; j < v.Size(); j++)
		{
			std::string str = v[j].GetString();
			if (str.length() > 0)
				data.mywords.push_back(str);
		}

		v = vitem["bossword"];
		for (unsigned int j = 0; j < v.Size(); j++)
		{
			std::string str = v[j].GetString();
			if (str.length() > 0)
				data.bossword.push_back(str);
		}
		v = vitem["rwds"];
		for (unsigned int j = 0; j < v.Size(); j++)
		{
			std::string str = v[j].GetString();
			if (str.length() > 1)
				data.rewords.push_back(str);
		}

		v = vitem["para"];
		for (unsigned int j = 0; j < v.Size(); j++)
		{
			std::string str = v[j].GetString();
			if (str.length() > 1)
				data.needgoods.push_back(str);
		}

		map_BranchPlotMissionData[mid].push_back(data);
		BranchPlotMissionItem bitme;
		int time = -1;
		int count = -1;
		v = vitem["time"];
		time = atoi(v.GetString());
		bitme.time = time*60;
		bitme.maxtime = time*60;
		v = vitem["count"];
		count = atoi(v.GetString());
		bitme.count = count<0?99:count;
		bitme.maxcount = bitme.count;
		bitme.subindex = 0;
		map_BranchPlotMissionItem[mid] = bitme;
	}

	JhGlobalData::updateBranchPlotMissionStatus();
}

void JhGlobalData::savePlotMissionStatus()
{
	//保存剧情状态"-"分割
	std::string str;
	for (unsigned int i = 0; i < JhGlobalData::vec_PlotMissionData.size(); i++)
	{
		std::string tmpstr = StringUtils::format("%d-", JhGlobalData::vec_PlotMissionData[i].status);
		str.append(tmpstr);
	}
	JhGameDataSave::getInstance()->setPlotMissionStatus(str.substr(0, str.length() - 1));
}

void JhGlobalData::updatePlotMissionStatus()
{
	//解析剧情状态"-"分割
	std::string str = JhGameDataSave::getInstance()->getPlotMissionStatus();
	if (str.length() > 0)
	{
		std::vector<std::string> tmp;
		JhCommonFuncs::split(str, tmp, "-");

		int msize = JhGlobalData::vec_PlotMissionData.size();
		unsigned int i = 0;
		for (i = 0; i < tmp.size(); i++)
		{
			JhGlobalData::vec_PlotMissionData[i].status = atoi(tmp[i].c_str());
		}
		int localsavesize = tmp.size();
		if (msize > localsavesize)
		{
			int addcount = msize - tmp.size();
			for (int m = 0; m < addcount; m++)
			{
				JhGlobalData::vec_PlotMissionData[i+m].status = M_NONE;
			}
		}

	}
}

void JhGlobalData::saveBranchPlotMissionStatus(std::string mid, int status)
{
	//保存支线剧情状态"-"分割
	std::string str;
	std::map<std::string, BranchPlotMissionItem>::iterator it;

	int count = 0;

	if (mid.length() > 0)
	{
		std::string curstr = StringUtils::format("%s,%d,%d,%d", mid.c_str(), JhGlobalData::map_BranchPlotMissionItem[mid].subindex, status, JhGlobalData::map_BranchPlotMissionItem[mid].time);
		str.append(curstr);
	}
	str.append(";");

	for (it = JhGlobalData::map_BranchPlotMissionItem.begin(); it != JhGlobalData::map_BranchPlotMissionItem.end(); it++)
	{
		std::string countstr = StringUtils::format("%d,", JhGlobalData::map_BranchPlotMissionItem[it->first].count);
		str.append(countstr);
	}

	curBranchMissionID = mid;
	JhGameDataSave::getInstance()->setBranchPlotMissionStatus(str.substr(0, str.length() - 1));
}

void JhGlobalData::updateBranchPlotMissionStatus()
{
	//解析支线剧情状态"-"分割
	std::string str = JhGameDataSave::getInstance()->getBranchPlotMissionStatus();
	if (str.length() > 0)
	{
		std::vector<std::string> savestr;
		JhCommonFuncs::split(str, savestr, ";");
		if (savestr.size() >= 2)
		{
			std::vector<std::string> tmp;
			JhCommonFuncs::split(savestr[0], tmp, ",");
			if (tmp.size() >= 4)
			{
				std::string mid = tmp[0];
				curBranchMissionID = mid;
				int subindex = atoi(tmp[1].c_str());
				int status = atoi(tmp[2].c_str());
				int time = atoi(tmp[3].c_str());

				JhGlobalData::map_BranchPlotMissionData[mid][subindex].status = status;
				JhGlobalData::map_BranchPlotMissionItem[mid].subindex = subindex;
				JhGlobalData::map_BranchPlotMissionItem[mid].time = time;
			}
			std::vector<std::string> tmp1;
			JhCommonFuncs::split(savestr[1], tmp1, ",");
			int i = 0;
			if (tmp1.size() >= JhGlobalData::map_BranchPlotMissionItem.size())
			{
				std::map<std::string, BranchPlotMissionItem>::iterator it;
				for (it = JhGlobalData::map_BranchPlotMissionItem.begin(); it != JhGlobalData::map_BranchPlotMissionItem.end(); it++)
				{
					JhGlobalData::map_BranchPlotMissionItem[it->first].count = atoi(tmp1[i].c_str());
					i++;
				}

			}
		}
	}
}

std::string JhGlobalData::getCurBranchPlotMissison()
{
	//std::string str = JhGameDataSave::getInstance()->getBranchPlotMissionStatus();
	//if (str.length() > 0)
	//{
	//	std::vector<std::string> tmp;
	//	JhCommonFuncs::split(str, tmp, ",");
	//	if (tmp.size() >= 3)
	//	{
	//		std::string mid = tmp[0];
	//		return mid;
	//	}
	//}
	//return "";
	return curBranchMissionID;
}

bool JhGlobalData::isDoingBranchPlotMisson()
{
	std::map<std::string, std::vector<PlotMissionData>>::iterator it;
	for (it = map_BranchPlotMissionData.begin(); it != map_BranchPlotMissionData.end(); it++)
	{
		std::vector<PlotMissionData>::iterator sit;
		for (sit = map_BranchPlotMissionData[it->first].begin(); sit != map_BranchPlotMissionData[it->first].end(); sit++)
		{
			if (sit->status == M_DOING)
				return true;
		}
	}

	std::string mid = getCurBranchPlotMissison();

	if (mid.length() > 0)
	{
		int subindex = JhGlobalData::map_BranchPlotMissionItem[mid].subindex;

		if (subindex < JhGlobalData::map_BranchPlotMissionData[mid].size() - 1 && JhGlobalData::map_BranchPlotMissionData[mid][subindex].status == M_DONE)
		{
			return true;
		}
	}
	return false;
}

int JhGlobalData::getUnlockChapter()
{
	int c = JhGameDataSave::getInstance()->getPlotUnlockChapter();

	int plotindex = JhGameDataSave::getInstance()->getPlotMissionIndex();
	int max = vec_PlotMissionData.size();

	if (plotindex >= max)
		plotindex = max - 1;

    if (plotindex >= max - 1)
    {
        c = MAXCHAPTER;
    }
	if (vec_PlotMissionData[plotindex].dnpc.compare("n089") == 0 && vec_PlotMissionData[plotindex].status == M_DONE)
	{
		c = MAXCHAPTER;
	}

	if (c > MAXCHAPTER)
		c = MAXCHAPTER;
	return c;
}

void JhGlobalData::setUnlockChapter(int val)
{
	int lastChapter = JhGlobalData::getUnlockChapter();

	if (val >= lastChapter)
		JhGameDataSave::getInstance()->setPlotUnlockChapter(val);
}

void JhGlobalData::loadShopData()
{
	vec_goods.clear();
	rapidjson::Document doc = ReadJsonFile("data/shop.json");
	rapidjson::Value& values = doc["gs"];
	for (unsigned int i = 0; i < values.Size(); i++)
	{
		GoodsData data;
		rapidjson::Value& item = values[i];
		rapidjson::Value& v = item["icon"];
		data.icon = v.GetString();

		v = item["res"];
		for (unsigned int m = 0; m < v.Size(); m++)
		{
			data.vec_res.push_back(v[m].GetString());
		}
		v = item["name"];
		data.name = v.GetString();

		v = item["type"];
		data.type = atoi(v.GetString());

		v = item["desc"];
		data.desc = v.GetString();

		v = item["price"];
		data.price = atoi(v.GetString());

		v = item["img"];
		data.img = v.GetString();
		vec_goods.push_back(data);
	}
}

void JhGlobalData::loadAchiveJsonData()
{
	vec_achiveData.clear();
	rapidjson::Document doc = ReadJsonFile("data/achive.json");
	rapidjson::Value& values = doc["a"];
	for (unsigned int i = 0; i < values.Size(); i++)//武器防具数组
	{
		AchiveData data;
		rapidjson::Value& item = values[i];
		rapidjson::Value& v = item["id"];
		data.id = v.GetString();
		v = item["type"];
		data.type = atoi(v.GetString());
		v = item["name"];
		data.name = v.GetString();
		v = item["desc"];
		data.desc = v.GetString();
		v = item["para1"];
		data.vec_para.push_back(v.GetString());
		v = item["para2"];
		std::string para2str = v.GetString();
		if (para2str.length() > 0 && para2str.compare("0") != 0)
			data.vec_para.push_back(v.GetString());

		v = item["rwd"];
		for (unsigned int m = 0; m < v.Size(); m++)
		{
			std::string val = v[m].GetString();
			if (val.length() > 0 && val.compare("0") != 0)
			{
				data.vec_rwd.push_back(val);
			}
		}
		data.finish = 0;
		data.isshowanim = 0;
		vec_achiveData.push_back(data);
	}
}
int JhGlobalData::createRandomNum(int val)
{
	int syssec = JhGlobalData::getSysSecTime();
	int static randNum = 0;
	randNum += 3600 * 24;
	syssec += randNum;
	if (syssec < 0 || randNum < 0)
	{
		syssec = 0;
		randNum = 0;
	}
	srand(syssec);
	int r = rand() % val;
	return r;
}

std::vector<std::string> JhGlobalData::getSaveListId()
{
	std::string str = JhGameDataSave::getInstance()->getSaveListId();

	std::vector<std::string> tmp;
	JhCommonFuncs::split(str, tmp, ";");
	vec_saveids.clear();
	for (unsigned int i = 0; i < tmp.size(); i++)
	{
		vec_saveids.push_back(tmp[i]);
	}

	return vec_saveids;
}

void JhGlobalData::setSaveListId(std::vector<std::string> vec_val)
{
	std::string str;
	for (unsigned int i = 0; i < vec_val.size(); i++)
	{
		str.append(vec_val[i]);
		str.append(";");
	}

	JhGameDataSave::getInstance()->setSaveListId(str.substr(0, str.length() - 1));
}

void JhGlobalData::setCurHeroIdToSaveList()
{
	std::string uid = JhGameDataSave::getInstance()->getUserId();

	int heroid = JhGameDataSave::getInstance()->getHeroIdByUid(uid);
	std::vector<std::string> vec_ids = JhGlobalData::getSaveListId();

	vec_ids[heroid - 1] = uid;

	JhGlobalData::setSaveListId(vec_ids);
}

bool JhGlobalData::isExercising()
{
	std::string str = JhGameDataSave::getInstance()->getExersiceCfg();
	return str.length() > 0 ? true : false;
}

bool JhGlobalData::isHasFSF()
{
	for (unsigned int m = 0; m < JhStorageRoom::map_storageData[RES_2].size(); m++)
	{
		PackageData* pData = &JhStorageRoom::map_storageData[RES_2][m];
		if (pData->strid.compare("72") == 0 && pData->count > 0)
		{
			return true;
		}
	}

	for (unsigned int m = 0; m < JhMyPackage::vec_packages.size(); m++)
	{
		PackageData* pData = &JhMyPackage::vec_packages[m];
		if (pData->strid.compare("72") == 0 && pData->count > 0)
		{
			return true;
		}
	}
	return false;
}

bool JhGlobalData::isHasVulture()
{
	for (unsigned int m = 0; m < JhStorageRoom::map_storageData[RES_2].size(); m++)
	{
		PackageData* pData = &JhStorageRoom::map_storageData[RES_2][m];
		if (pData->strid.compare("83") == 0 && pData->count > 0)
		{
			return true;
		}
	}
	
	return false;
}

void JhGlobalData::loadGftrickData()
{
	map_gftricks.clear();
	rapidjson::Document doc = ReadJsonFile("data/gftrick.json");
	rapidjson::Value& values = doc["s"];
	for (unsigned int i = 0; i < values.Size(); i++)
	{
		rapidjson::Value& vitem = values[i];
		GFTrickData data;
		rapidjson::Value& v = vitem["id"];
		data.id = v.GetString();

		v = vitem["name"];
		for (unsigned int j = 0; j < v.Size(); j++)
		{
			std::string str = v[j].GetString();
			data.snames.push_back(str);
		}

		map_gftricks[data.id] = data;
	}
}

void JhGlobalData::loadGfskillData()
{
	map_gfskills.clear();
	rapidjson::Document doc = ReadJsonFile("data/skill.json");
	rapidjson::Value& values = doc["sk"];
	for (unsigned int i = 0; i < values.Size(); i++)
	{
		rapidjson::Value& vitem = values[i];
		GFSkillData data;

		rapidjson::Value& v = vitem["name"];
		data.name = v.GetString();
		v = vitem["id"];
		data.id = v.GetString();

		v = vitem["desc"];
		data.desc = v.GetString();

		v = vitem["desc1"];
		data.desc1 = v.GetString();

		data.leftval = 0;
		int intid = atoi(data.id.c_str());
		map_gfskills[intid] = data;
	}
}

std::string JhGlobalData::tempHasGf_Equip(std::string strid)
{
	//std::map<std::string, MapData>::iterator it;

	//for (it = JhGlobalData::map_maps.begin(); it != JhGlobalData::map_maps.end(); ++it)
	//{
	//	std::string mapid = JhGlobalData::map_maps[it->first].strid;

	//	std::string datastr = JhGameDataSave::getInstance()->getTempStorage(mapid);
	//	if (datastr.length() > 0)
	//	{
	//		std::vector<std::string> vec_retstr;
	//		JhCommonFuncs::split(datastr, vec_retstr, ";");
	//		for (unsigned int i = 0; i < vec_retstr.size(); i++)
	//		{
	//			std::vector<std::string> tmp;
	//			JhCommonFuncs::split(vec_retstr[i], tmp, "-");
	//			std::string tmpstrid = tmp[0];
	//			int tmptype = atoi(tmp[1].c_str());
	//			if ((tmptype == W_GONG || tmptype == N_GONG || tmptype == WEAPON || tmptype == PROTECT_EQU) && strid.compare(tmpstrid) == 0)
	//				return true;
	//		}
	//	}
	//}

	std::map<std::string, std::vector<std::string>>::iterator it;

	for (it = JhGlobalData::map_tempGf_Equip.begin(); it != JhGlobalData::map_tempGf_Equip.end(); ++it)
	{
		for (unsigned int i = 0; i < JhGlobalData::map_tempGf_Equip[it->first].size(); i++)
		{
			if (JhGlobalData::map_tempGf_Equip[it->first][i].compare(strid) == 0)
				return it->first;
		}
	}

	return "";
}

int JhGlobalData::getResType(std::string strid)
{
	std::map<std::string, std::vector<BuildActionData>>::iterator it;
	for (it = JhGlobalData::map_buidACData.begin(); it != JhGlobalData::map_buidACData.end(); ++it)
	{
		std::vector<BuildActionData> vec_bactData = JhGlobalData::map_buidACData[it->first];

		for (unsigned int m = 0; m < vec_bactData.size(); m++)
		{
			BuildActionData bdata = vec_bactData[m];
			if (strid.compare(bdata.icon) == 0)
			{
				return bdata.type - 1;
			}
		}
	}

	for (unsigned int n = 0; n < JhGlobalData::vec_resData.size(); n++)
	{
		ResData rdata = JhGlobalData::vec_resData[n];
		if (strid.compare(rdata.strid) == 0)
		{
			return rdata.type - 1;
		}
	}

	std::map<std::string, WG_NGData>::iterator itf;
	for (itf = JhGlobalData::map_wgngs.begin(); itf != JhGlobalData::map_wgngs.end(); ++itf)
	{
		WG_NGData gfdata = JhGlobalData::map_wgngs[itf->first];
		if (strid.compare(gfdata.id) == 0)
		{
			return gfdata.type - 1;
		}
	}

	std::map<std::string, EquipData>::iterator ite;
	for (ite = JhGlobalData::map_equips.begin(); ite != JhGlobalData::map_equips.end(); ++ite)
	{
		EquipData edata = JhGlobalData::map_equips[ite->first];
		if (strid.compare(edata.id) == 0)
		{
			return edata.type - 1;
		}
	}

	return 0;
}

int JhGlobalData::getResExType(std::string strid)
{
	std::map<std::string, std::vector<BuildActionData>>::iterator it;
	for (it = JhGlobalData::map_buidACData.begin(); it != JhGlobalData::map_buidACData.end(); ++it)
	{
		std::vector<BuildActionData> vec_bactData = JhGlobalData::map_buidACData[it->first];

		for (unsigned int m = 0; m < vec_bactData.size(); m++)
		{
			BuildActionData bdata = vec_bactData[m];
			if (strid.compare(bdata.icon) == 0)
			{
				return bdata.extype;
			}
		}
	}

	for (unsigned int n = 0; n < JhGlobalData::vec_resData.size(); n++)
	{
		ResData rdata = JhGlobalData::vec_resData[n];
		if (strid.compare(rdata.strid) == 0)
		{
			return rdata.actype;
		}
	}

	std::map<std::string, WG_NGData>::iterator itf;
	for (itf = JhGlobalData::map_wgngs.begin(); itf != JhGlobalData::map_wgngs.end(); ++itf)
	{
		WG_NGData gfdata = JhGlobalData::map_wgngs[itf->first];
		if (strid.compare(gfdata.id) == 0)
		{
			return gfdata.extype;
		}
	}

	std::map<std::string, EquipData>::iterator ite;
	for (ite = JhGlobalData::map_equips.begin(); ite != JhGlobalData::map_equips.end(); ++ite)
	{
		EquipData edata = JhGlobalData::map_equips[ite->first];
		if (strid.compare(edata.id) == 0)
		{
			return edata.extype;
		}
	}

	return 0;
}

void JhGlobalData::loadChallengeRewardData()
{
	map_challengeReward.clear();
	rapidjson::Document doc = ReadJsonFile("data/challengereward.json");
	rapidjson::Value& values = doc["rs"];
	for (unsigned int i = 0; i < values.Size(); i++)//npc数组
	{
		ChallengeRewardData data;
		rapidjson::Value& item = values[i];
		rapidjson::Value& v = item["npc"];
		std::string npcid = v.GetString();

		v = item["winres"];
		for (unsigned int m = 0; m < v.Size(); m++)
		{
			std::string str = v[m].GetString();
			if (str.length() > 1)
				data.vec_winres.push_back(str);
		}
		v = item["winresrnd"];
		for (unsigned int m = 0; m < v.Size(); m++)
		{
			std::string str = v[m].GetString();
			float rnd = atof(str.c_str());
			if (str.length() > 0 && rnd > 0.0f)
			{
				data.vec_winrnd.push_back(rnd);
			}
		}
		map_challengeReward[npcid] = data;
	}
}

void JhGlobalData::loadTempGF_EquipData()
{
	map_tempGf_Equip.clear();
	std::map<std::string, MapData>::iterator it;

	for (it = JhGlobalData::map_maps.begin(); it != JhGlobalData::map_maps.end(); ++it)
	{
		std::string mapid = JhGlobalData::map_maps[it->first].strid;

		std::string datastr = JhGameDataSave::getInstance()->getTempStorage(mapid);
		if (datastr.length() > 0)
		{
			std::vector<std::string> vec_retstr;
			JhCommonFuncs::split(datastr, vec_retstr, ";");
			for (unsigned int i = 0; i < vec_retstr.size(); i++)
			{
				std::vector<std::string> tmp;
				JhCommonFuncs::split(vec_retstr[i], tmp, "-");
				std::string tmpstrid = tmp[0];
				int tmptype = atoi(tmp[1].c_str());
				if (tmptype == W_GONG || tmptype == N_GONG || tmptype == WEAPON || tmptype == PROTECT_EQU)
				{
					map_tempGf_Equip[mapid].push_back(tmpstrid);
				}
			}
		}
	}
}

int JhGlobalData::getShareDay()
{
	return JhGameDataSave::getInstance()->getShareDayOfYear();
}

void JhGlobalData::setShareDay(int day)
{
	JhGameDataSave::getInstance()->setShareDayOfYear(day);
}


std::string JhGlobalData::getMakeWarmConfig()
{
	return JhGameDataSave::getInstance()->getWarmConfig();
}

void JhGlobalData::setMakeWarmConfig(std::string strval)
{
	JhGameDataSave::getInstance()->setWarmConfig(strval);
}

std::string JhGlobalData::getExgCfgData()
{
	return JhGameDataSave::getInstance()->getExgCfgData();
}

void JhGlobalData::setExgCfgData(std::string strval)
{
	JhGameDataSave::getInstance()->setExgCfgData(strval);
}

void JhGlobalData::setWxbMapPos(int pos)
{
	wxbmapos = pos;
	JhGameDataSave::getInstance()->setWxbMapPos(pos);
}


int JhGlobalData::getWxbMapPos()
{
	return wxbmapos;
}

void JhGlobalData::setDgqbMapPos(int pos)
{
	dgqbmapos = pos;
	JhGameDataSave::getInstance()->setDgqbMapPos(pos);
}

int JhGlobalData::getDgqbMapPos()
{
	return dgqbmapos;
}

void JhGlobalData::setReviveCount(int val)
{
	JhGameDataSave::getInstance()->setReviveCount(val);
}

int JhGlobalData::getReviveCount()
{
	return JhGameDataSave::getInstance()->getReviveCount();
}

int JhGlobalData::getMyGoldCount()
{
	return myGlodCount;
}

void JhGlobalData::setMyGoldCount(int count)
{
	myGlodCount = count;
	JhGlobalData::setMD5MyGoldCount(md5(myGlodCount));
	JhGameDataSave::getInstance()->setGoldCount(myGlodCount);
	JhGlobalData::doAchive(A_1, count);
}

std::string JhGlobalData::getMyID()
{
	return JhGameDataSave::getInstance()->getMyID();
}

void JhGlobalData::setMyID(std::string str)
{
	JhGameDataSave::getInstance()->setMyID(str);
}

std::string JhGlobalData::getMyNickName()
{
	return JhGameDataSave::getInstance()->getMyNickName();
}

void JhGlobalData::setMyNickName(std::string str)
{
	JhGameDataSave::getInstance()->setMyNickName(str);
}

int JhGlobalData::getTimeGiftLeftTime()
{
	return timegiftlefttime;
}

void JhGlobalData::setTimeGiftLeftTime(int val)
{
	timegiftlefttime = val;
}


void JhGlobalData::setIsBuyTimeGift(bool val)
{
	isBuyTimeGift = val;
	JhGameDataSave::getInstance()->setIsBuyTimeGift(val);
	
}

bool JhGlobalData::getIsBuyTimeGift()
{
	return isBuyTimeGift;
}

void JhGlobalData::setFreeReviveCount(int val)
{
	freeReviveCount = val;
}

int JhGlobalData::getFreeReviveCount()
{
	return freeReviveCount;
}

void JhGlobalData::setUseGold(int val)
{
	JhGameDataSave::getInstance()->setUseGold(val);
	JhGlobalData::doAchive(A_0, val);
}

int JhGlobalData::getUseGold()
{
	return JhGameDataSave::getInstance()->getUseGold();
}

void JhGlobalData::loadFriendly()
{
	map_myfriendly.clear();
	std::string datastr = JhGameDataSave::getInstance()->getFriendly();

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
				map_myfriendly[tmp[0]].friendly = friendly;
				map_myfriendly[tmp[0]].relation = atoi(tmp[2].c_str());
			}
		}
	}
}


void JhGlobalData::saveFriendly()
{
	std::string str;
	std::map<std::string, FriendlyData>::iterator it;
	for (it = JhGlobalData::map_myfriendly.begin(); it != JhGlobalData::map_myfriendly.end(); ++it)
	{
		int friendly = JhGlobalData::map_myfriendly[it->first].friendly;
		int relation = JhGlobalData::map_myfriendly[it->first].relation;
		bool issave = true;
		if (friendly == 0 && relation == 0)
			issave = false;

		if (issave)
		{
			std::string onestr = StringUtils::format("%s,%d,%d", it->first.c_str(), friendly, relation);
			str.append(onestr);
			str.append(";");
		}
	}
	JhGameDataSave::getInstance()->setFriendly(str.substr(0, str.length() - 1));
}

void JhGlobalData::loadNpcFriendJsonData()
{
	map_NPCFriendData.clear();
	rapidjson::Document doc = ReadJsonFile("data/friend.json");
	rapidjson::Value& values = doc["f"];
	for (unsigned int i = 0; i < values.Size(); i++)//一级资源数组
	{
		NPCFriendData data;
		rapidjson::Value& item = values[i];
		rapidjson::Value& v = item["npcid"];
		std::string npcid = v.GetString();

		v = item["max"];
		data.maxfriendly = atoi(v.GetString());

		v = item["need"];
		data.needfriendly = atoi(v.GetString());

		v = item["atk"];
		data.atkpercent = atof(v.GetString());

		v = item["df"];
		data.dfpercent = atof(v.GetString());

		v = item["avoid"];
		data.dodgepercent = atof(v.GetString());

		v = item["crit"];
		data.critpercent = atof(v.GetString());

		v = item["hp"];
		data.hppercent = atof(v.GetString());

		v = item["enemynpc"];

		for (unsigned int m = 0; m < v.Size(); m++)
		{
			rapidjson::Value& npsv = v[m];
			std::string npcid = npsv.GetString();
			if (npcid.length() > 1)
				data.vec_enemynpc.push_back(npcid);
		}

		v = item["fightnpc"];

		for (unsigned int m = 0; m < v.Size(); m++)
		{
			rapidjson::Value& npsv = v[m];
			std::string npcid = npsv.GetString();
			if (npcid.length() > 1)
				data.vec_fightnpc.push_back(npcid);
		}

		v = item["res"];

		for (unsigned int m = 0; m < v.Size(); m++)
		{
			rapidjson::Value& resv = v[m];
			std::string resval = resv.GetString();
			if (resval.length() > 1)
				data.vec_giveres.push_back(atoi(resval.c_str()));
		}

		v = item["nres"];

		for (unsigned int m = 0; m < v.Size(); m++)
		{
			rapidjson::Value& resv = v[m];
			std::string resval = resv.GetString();
			if (resval.length() > 1)
				data.vec_askres.push_back(atoi(resval.c_str()));
		}

		map_NPCFriendData[npcid] = data;

	}
}
void JhGlobalData::loadNpcMasterJsonData()
{
	map_NPCMasterData.clear();
	rapidjson::Document doc = ReadJsonFile("data/master.json");
	rapidjson::Value& values = doc["ms"];
	for (unsigned int i = 0; i < values.Size(); i++)//一级资源数组
	{
		NPCMasterData data;
		rapidjson::Value& item = values[i];
		rapidjson::Value& v = item["npcid"];
		std::string npcid = v.GetString();

		v = item["need"];
		data.needfriendly = atoi(v.GetString());

		v = item["atk"];
		data.atkpercent = atof(v.GetString());

		v = item["df"];
		data.dfpercent = atof(v.GetString());

		v = item["avoid"];
		data.dodgepercent = atof(v.GetString());

		v = item["crit"];
		data.critpercent = atof(v.GetString());

		v = item["hp"];
		data.hppercent = atof(v.GetString());

		v = item["gfid"];

		for (unsigned int m = 0; m < v.Size(); m++)
		{
			rapidjson::Value& resv = v[m];
			std::string resval = resv.GetString();
			if (resval.length() > 1)
				data.vec_gfid.push_back(resval);
		}

		v = item["bonus"];

		for (unsigned int m = 0; m < v.Size(); m++)
		{
			rapidjson::Value& resv = v[m];
			std::string resval = resv.GetString();
			if (resval.length() > 0 && resval.compare("0") != 0)
				data.vec_gfbonus.push_back(atof(resval.c_str()));
		}

		map_NPCMasterData[npcid] = data;

	}
}

void JhGlobalData::loadMixGfJsonData()
{
	map_MixGfData.clear();
	rapidjson::Document doc = ReadJsonFile("data/zhgf.json");
	rapidjson::Value& values = doc["m"];
	for (unsigned int i = 0; i < values.Size(); i++)//一级资源数组
	{
		MixGfData data;
		rapidjson::Value& item = values[i];
		rapidjson::Value& v = item["id"];
		data.id = v.GetString();

		v = item["name"];
		data.name = v.GetString();

		v = item["lv"];
		data.lv = atoi(v.GetString());

		v = item["atk"];
		data.atkpercent = atof(v.GetString());

		v = item["df"];
		data.dfpercent = atof(v.GetString());

		v = item["avoid"];
		data.dodgepercent = atof(v.GetString());

		v = item["crit"];
		data.critpercent = atof(v.GetString());

		v = item["hp"];
		data.hppercent = atof(v.GetString());

		v = item["sex"];
		data.sex = atoi(v.GetString());

		v = item["mgf"];
		data.mastergf = v.GetString();

		v = item["cgf"];
		for (unsigned int m = 0; m < v.Size(); m++)
		{
			rapidjson::Value& resv = v[m];
			std::string resval = resv.GetString();
			if (resval.length() > 1)
				data.vec_secgf.push_back(resval);
		}

		v = item["egf"];

		for (unsigned int m = 0; m < v.Size(); m++)
		{
			rapidjson::Value& resv = v[m];
			std::string resval = resv.GetString();
			if (resval.length() > 1)
				data.vec_mutexgf.push_back(resval);
		}

		map_MixGfData[data.id] = data;

	}
}

std::string JhGlobalData::getMixGF()
{
	return JhGameDataSave::getInstance()->getMixGF();
}

void JhGlobalData::setMixGF(std::string str)
{
	JhGameDataSave::getInstance()->setMixGF(str);
}

std::string JhGlobalData::getHeroProperData()
{
	return JhGameDataSave::getInstance()->getHeroProperData();
}
void JhGlobalData::setHeroProperData(std::string strval)
{
	JhGameDataSave::getInstance()->setHeroProperData(strval);
}

void JhGlobalData::getAchiveData()
{
	std::string datastr = JhGameDataSave::getInstance()->getAchiveData();
	if (datastr.length() > 0)
	{
		std::vector<std::string> vec_retstr;
		JhCommonFuncs::split(datastr, vec_retstr, ";");
		for (unsigned int i = 0; i < vec_retstr.size(); i++)
		{
			JhGlobalData::vec_achiveData[i].finish = atoi(vec_retstr[i].c_str());
		}
	}
}

void JhGlobalData::saveAchiveData()
{
	std::string str;
    std::vector<AchiveData> tmp_saveAchives;
    for (unsigned int i = 0; i < JhGlobalData::vec_achiveData.size(); i++)
    {
        tmp_saveAchives.push_back(JhGlobalData::vec_achiveData[i]);
    }
    
    int size = tmp_saveAchives.size();
    for(int i = 0; i < size; i++)
    {
        for(int j = 1; j < size - i; j++)
        {
            if(atoi(tmp_saveAchives[j].id.c_str())< atoi(tmp_saveAchives[j-1].id.c_str()))
            {
                AchiveData tempdata = tmp_saveAchives[j-1];
                tmp_saveAchives[j-1] = tmp_saveAchives[j];
                tmp_saveAchives[j] = tempdata;
            }
        }
    }

	for (unsigned int i = 0; i < tmp_saveAchives.size(); i++)
	{
		int finish = tmp_saveAchives[i].finish;
		int maxcount = JhGlobalData::getAchiveFinishCount(tmp_saveAchives[i]);
		if (finish > maxcount)
			finish = maxcount;
		std::string onestr = StringUtils::format("%d;", finish);
		str.append(onestr);
	}

	JhGameDataSave::getInstance()->setAchiveData(str.substr(0, str.length() - 1));
}

void JhGlobalData::doAchive(int atype, int count)
{
	bool issave = false;
	for (unsigned int i = 0; i < JhGlobalData::vec_achiveData.size(); i++)
	{
		if (JhGlobalData::vec_achiveData[i].type == atype && JhGlobalData::vec_achiveData[i].finish != -1)
		{
			issave = true;
			JhGlobalData::vec_achiveData[i].finish = count;
		}
	}
	if (issave)
		saveAchiveData();
}

int JhGlobalData::getAchiveFinishCount(AchiveData adata)
{
	int needcount = 1;

	if (adata.type == A_0 || adata.type == A_1 || adata.type == A_2 || adata.type == A_3 || adata.type == A_5 || adata.type == A_11)
	{
		needcount = atoi(adata.vec_para[0].c_str());
	}

	else if (adata.type == A_7 || adata.type == A_9)
	{
		needcount = atoi(adata.vec_para[1].c_str());

	}
	else if (adata.type == A_10)
	{
		needcount = atoi(adata.vec_para[1].c_str());
	}
	else if (adata.type == A_12)
	{
		needcount = atoi(adata.vec_para[1].c_str());
	}
	return needcount;
}

void JhGlobalData::getAchiveAnimData()
{
	std::string str = JhGameDataSave::getInstance()->getAchiveAnimData();
	if (str.length() > 0)
	{
		std::vector<std::string> vec_retstr;
		JhCommonFuncs::split(str, vec_retstr, ";");
		for (unsigned int i = 0; i < vec_retstr.size(); i++)
		{
			JhGlobalData::vec_achiveData[i].isshowanim = atoi(vec_retstr[i].c_str());
		}
	}
}

void JhGlobalData::saveAchiveAnimData()
{
	std::string str;
	std::vector<AchiveData> tmp_saveAchives;
	for (unsigned int i = 0; i < JhGlobalData::vec_achiveData.size(); i++)
	{
		tmp_saveAchives.push_back(JhGlobalData::vec_achiveData[i]);
	}

	int size = tmp_saveAchives.size();
	for (int i = 0; i < size; i++)
	{
		for (int j = 1; j < size - i; j++)
		{
			if (atoi(tmp_saveAchives[j].id.c_str())< atoi(tmp_saveAchives[j - 1].id.c_str()))
			{
				AchiveData tempdata = tmp_saveAchives[j - 1];
				tmp_saveAchives[j - 1] = tmp_saveAchives[j];
				tmp_saveAchives[j] = tempdata;
			}
		}
	}

	for (unsigned int i = 0; i < tmp_saveAchives.size(); i++)
	{
		int isshow = tmp_saveAchives[i].isshowanim;
		std::string onestr = StringUtils::format("%d;", isshow);
		str.append(onestr);
	}

	JhGameDataSave::getInstance()->setAchiveAnimData(str.substr(0, str.length() - 1));
}

std::string JhGlobalData::addUidString(std::string val)
{
    return uid + val;
}

std::string JhGlobalData::UUID()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	return getDeviceIDInKeychain();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	return "18EF88C6-D1E2-49F4-BA81-F7D0D324E8B2";
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	std::string ret;
	JniMethodInfo methodInfo;
	if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/jh/Utils", "UUID", "()Ljava/lang/String;"))
	{
		jstring jstr = (jstring)methodInfo.env->CallStaticObjectMethod(methodInfo.classID, methodInfo.methodID);
		ret = methodInfo.env->GetStringUTFChars(jstr, 0);
	}
	return ret;
#endif
}

std::string JhGlobalData::getVersion()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    return getvercode();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	return "1.0";
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	std::string ret;
	JniMethodInfo methodInfo;
	if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/jh/Utils", "getVersion", "()Ljava/lang/String;"))
	{
		jstring jstr = (jstring)methodInfo.env->CallStaticObjectMethod(methodInfo.classID, methodInfo.methodID);
		ret = methodInfo.env->GetStringUTFChars(jstr, 0);
	}
	return ret;
#endif
}

std::string JhGlobalData::getPackageName()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	return getbundleid();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	return "com.kuxx.jh";
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	std::string ret;
	JniMethodInfo methodInfo;
	if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/jh/Utils", "getPkgName", "()Ljava/lang/String;"))
	{
		jstring jstr = (jstring)methodInfo.env->CallStaticObjectMethod(methodInfo.classID, methodInfo.methodID);
		ret = methodInfo.env->GetStringUTFChars(jstr, 0);
	}
	return ret;
#endif
}

std::string JhGlobalData::getChannelId()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	return getbundleid();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	return "win32";
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	std::string ret;
	JniMethodInfo methodInfo;
	if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/jh/Utils", "getChannelID", "()Ljava/lang/String;"))
	{
		jstring jstr = (jstring)methodInfo.env->CallStaticObjectMethod(methodInfo.classID, methodInfo.methodID);
		ret = methodInfo.env->GetStringUTFChars(jstr, 0);
	}
	return ret;
#endif
}

void JhGlobalData::copyToClipBoard(std::string text)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	copytoclipboard((char*)text.c_str());
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniMethodInfo methodInfo;
	char p_str[256] = { 0 };
	sprintf(p_str, "%s", text.c_str());
	if (JniHelper::getStaticMethodInfo(methodInfo, "com/kuxx/jh/AppActivity", "copyToClipboard", "(Ljava/lang/String;)V"))
	{
		jstring para1 = methodInfo.env->NewStringUTF(p_str);
		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, para1);
}
#endif
}

std::string JhGlobalData::getMD5MyGoldCount()
{
	return MD5MyGoldCount;
}

void JhGlobalData::setMD5MyGoldCount(std::string val)
{
	MD5MyGoldCount = val;
}

std::string JhGlobalData::getMD5CostGlodCount()
{
	return MD5CostGlodCount;
}

void JhGlobalData::setMD5CostGlodCount(std::string val)
{
	MD5CostGlodCount = val;
}

std::string JhGlobalData::getMD5FreeReviveCount()
{
	return MD5FreeReviveCount;
}

void JhGlobalData::setMD5FreeReviveCount(std::string val)
{
	MD5FreeReviveCount = val;
}

std::string JhGlobalData::getMD5HeroLv()
{
	return MD5HeroLv;
}

void JhGlobalData::setMD5HeroLv(std::string val)
{
	MD5HeroLv = val;
}

std::string JhGlobalData::getMD5LiveDays()
{
	return MD5LiveDays;
}

void JhGlobalData::setMD5LiveDays(std::string val)
{
	MD5LiveDays = val;
}

int JhGlobalData::getNoPopNoticeDay()
{
	return JhGameDataSave::getInstance()->getNoPopNoticeDay();
}

void JhGlobalData::setNoPopNoticeDay(int day)
{
	JhGameDataSave::getInstance()->setNoPopNoticeDay(day);
}

void JhGlobalData::setNoAds(bool val)
{
    UserDefault::getInstance()->setBoolForKey(addUidString("noads").c_str(), val);
    UserDefault::getInstance()->flush();
}

bool JhGlobalData::getNoAds()
{
    return UserDefault::getInstance()->getBoolForKey(addUidString("noads").c_str(), false);
}

void JhGlobalData::setNoComments(bool val)
{
    UserDefault::getInstance()->setBoolForKey(addUidString("nocomments").c_str(), val);
    UserDefault::getInstance()->flush();
}

bool JhGlobalData::getNoComments()
{
    return UserDefault::getInstance()->getBoolForKey(addUidString("nocomments").c_str(), false);
}

void JhGlobalData::setDailyAdsCount(int val)
{
    UserDefault::getInstance()->setIntegerForKey(addUidString("dailyadscount").c_str(), val);
    UserDefault::getInstance()->flush();
}

int JhGlobalData::getDailyAdsCount()
{
    return UserDefault::getInstance()->getIntegerForKey(addUidString("dailyadscount").c_str(), 0);
}

void JhGlobalData::setDailyAdsDay(int val)
{
    UserDefault::getInstance()->setIntegerForKey(addUidString("dailyadsday").c_str(), val);
    UserDefault::getInstance()->flush();
}

int JhGlobalData::getDailyAdsDay()
{
    return UserDefault::getInstance()->getIntegerForKey(addUidString("dailyadsday").c_str(), 0);
}

void JhGlobalData::setAdsInterval(int val)
{
    adsinterval = val;
}

int JhGlobalData::getAdsInterval()
{
    return adsinterval;
}

void JhGlobalData::setAdsChoose(long val)
{
    adschoose = val;
}

int JhGlobalData::getAdsChoose(int position)
{
    /*long m = adschoose;
     int n =pow(10,position);
     float x = m/n;
     long i = (long)(m/n);
     int j = i % 10;
     return j;*/
    return (int)(adschoose/pow(10, position)) % 10;
}

void JhGlobalData::setHasBuy(bool val)
{
    hasBuy = val;
}

bool JhGlobalData::getHasBuy()
{
    return hasBuy;
}
