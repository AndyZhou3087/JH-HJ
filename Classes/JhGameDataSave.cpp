#include "JhGameDataSave.h"
#include "cocos2d.h"
#include "Utility.h"
#include "JhHero.h"
#include "JhNature.h"
#include "JhGlobalData.h"
JhGameDataSave* JhGameDataSave::_Context = NULL;
std::string JhGameDataSave::userid = "";
JhGameDataSave* JhGameDataSave::getInstance() {
	if (_Context == NULL) {
		_Context = new JhGameDataSave;
	}
	return _Context;
}

void JhGameDataSave::purgeGameSave() {
	CC_SAFE_DELETE(_Context);
}

int JhGameDataSave::getLiveDays()
{
	return loadIntDataByKey(addUIDString("livedays"));
}

void JhGameDataSave::setLiveDays(int val)
{
	saveIntDataByKey(addUIDString("livedays"), val);
}

float JhGameDataSave::getHeroOutinjury()
{
	return loadFloatDataByKey(addUIDString("outinjury"), JhHero::MAXOutinjuryValue);
}
void JhGameDataSave::setHeroOutinjury(float val)
{
	saveFloatDataByKey(addUIDString("outinjury"), val);
}

float JhGameDataSave::getHeroInnerinjury()
{
	return loadFloatDataByKey(addUIDString("innerinjury"), JhHero::MAXInnerinjuryValue);
}
void JhGameDataSave::setHeroInnerinjury(float val)
{
	saveFloatDataByKey(addUIDString("innerinjury"), val);
}

float JhGameDataSave::getHeroHunger()
{
	return loadFloatDataByKey(addUIDString("hunger"), JhHero::MAXHungerValue);
}
void JhGameDataSave::setHeroHunger(float val)
{
	saveFloatDataByKey(addUIDString("hunger"), val);
}

float JhGameDataSave::getHeroSpirit()
{
	return loadFloatDataByKey(addUIDString("spirit"), JhHero::MAXSpiritValue);
}
void JhGameDataSave::setHeroSpirit(float val)
{
	saveFloatDataByKey(addUIDString("spirit"), val);
}

float JhGameDataSave::getHeroLife()
{
	return loadFloatDataByKey(addUIDString("life"), -100.0f);
}
void JhGameDataSave::setHeroLife(float val)
{
	saveFloatDataByKey(addUIDString("life"), val);
}

int JhGameDataSave::getNatureTime()
{
	return loadIntDataByKey(addUIDString("ndaytime"));
}
void JhGameDataSave::setNatureTime(int val)
{
	saveIntDataByKey(addUIDString("ndaytime"), val);
}

int JhGameDataSave::getNatureWeather()
{
	return loadIntDataByKey(addUIDString("nwhather"), EWeather::Suny);
}
void JhGameDataSave::setNatureWeather(int val)
{
	saveIntDataByKey(addUIDString("nwhather"), val);
}

int JhGameDataSave::getNatureWeatherChangeCount()
{
	return loadIntDataByKey(addUIDString("nwhatherchgcount"), 0);
}

void JhGameDataSave::setNatureWeatherChangeCount(int val)
{
	saveIntDataByKey(addUIDString("nwhatherchgcount"), val);
}

int JhGameDataSave::getNatureReason()
{
	return loadIntDataByKey(addUIDString("nreason"), EReason::Spring);
}
void JhGameDataSave::setNatureReason(int val)
{
	saveIntDataByKey(addUIDString("nreason"), val);
}

int JhGameDataSave::getNatureTemperature()
{
	//默认温度取随机
	int maxr = tempeRange[getNatureReason()][1] - tempeRange[getNatureReason()][0] + 1;
	int r = tempeRange[getNatureReason()][0] + JhGlobalData::createRandomNum(maxr);
	return loadIntDataByKey(addUIDString("ntempe"), r);
}
void JhGameDataSave::setNatureTemperature(int val)
{
	saveIntDataByKey(addUIDString("ntempe"), val);
}

std::string JhGameDataSave::getStorageData()
{
	return loadStringDataByKey(addUIDString("storage"), "");
}
void JhGameDataSave::setStorageData(std::string valstr)
{
	saveStringDataByKey(addUIDString("storage"), valstr);
}

int JhGameDataSave::getBuildLV(std::string buildname)
{
	std::string rname = StringUtils::format("%slv", buildname.c_str());
	return loadIntDataByKey(addUIDString(rname));
}
void JhGameDataSave::setBuildLV(std::string buildname, int val)
{
	std::string rname = StringUtils::format("%slv", buildname.c_str());
	saveIntDataByKey(addUIDString(rname), val);
}

std::string JhGameDataSave::getHeroAddr()
{
	return loadStringDataByKey(addUIDString("heroaddr"), "m1-1");
}

void JhGameDataSave::setHeroAddr(std::string addr)
{
	saveStringDataByKey(addUIDString("heroaddr"), addr);
}

std::string JhGameDataSave::getTempStorage(std::string addrname)
{
	std::string str = StringUtils::format("%s-temps", addrname.c_str());
	return loadStringDataByKey(addUIDString(str));
}

void JhGameDataSave::setTempStorage(std::string addrname, std::string vstr)
{
	std::string str = StringUtils::format("%s-temps", addrname.c_str());
	saveStringDataByKey(addUIDString(str), vstr);
}

std::string JhGameDataSave::getPackage()
{
	return loadStringDataByKey(addUIDString("packages"));
}

void JhGameDataSave::setPackage(std::string vstr)
{
	saveStringDataByKey(addUIDString("packages"), vstr);
}

std::string JhGameDataSave::getResData()
{
	return loadStringDataByKey(addUIDString("resdata"));
}
void JhGameDataSave::setResData(std::string vstr)
{
	saveStringDataByKey(addUIDString("resdata"), vstr);
}

int JhGameDataSave::getHeroId()
{
	return loadIntDataByKey(addUIDString("heroid"), 1);
}

int JhGameDataSave::getHeroIdByUid(std::string uid)
{
	std::string key = StringUtils::format("%sheroid", uid.c_str());
	return loadIntDataByKey(key, 1);
}

void JhGameDataSave::setHeroId(int id)
{
	saveIntDataByKey(addUIDString("heroid"), id);
}

int JhGameDataSave::getHeroLV()
{
	return loadIntDataByKey(addUIDString("herolv"), 0);
}

int JhGameDataSave::getHeroLV(std::string localid)
{
	std::string key = localid + "herolv";
	return loadIntDataByKey(key, 0);
}

void JhGameDataSave::setHeroLV(int lv)
{
	saveIntDataByKey(addUIDString("herolv"), lv);
}


int JhGameDataSave::getHeroExp()
{
	return loadIntDataByKey(addUIDString("heroexp"), 0);
}
void JhGameDataSave::setHeroExp(int val)
{
	saveIntDataByKey(addUIDString("heroexp"), val);
}

bool JhGameDataSave::getHeroIsOut()
{
	int isout = loadIntDataByKey(addUIDString("heroisout"), 0);
	return isout == 0?false:true;
}
void JhGameDataSave::setHeroIsOut(bool val)
{
	saveIntDataByKey(addUIDString("heroisout"), val == true ? 1 : 0);
}

std::string JhGameDataSave::getHeroProperData()
{
	return loadStringDataByKey(addUIDString("heroproper"));
}

std::string JhGameDataSave::getHeroProperData(std::string localid)
{
	std::string key = localid + "heroproper";
	return loadStringDataByKey(key);
}

void JhGameDataSave::setHeroProperData(std::string strval)
{
	saveStringDataByKey(addUIDString("heroproper"), strval);
}

std::string JhGameDataSave::getHeroUnlockData()
{
	return loadStringDataByKey("herounlock", "1-0-0-0");
}
void JhGameDataSave::setHeroUnlockData(std::string strval)
{
	saveStringDataByKey("herounlock", strval);
}

std::string JhGameDataSave::getUserId()
{
	return loadStringDataByKey("uid");
}

void JhGameDataSave::setUserId(std::string val)
{
	userid = val;
}

void JhGameDataSave::saveUserId(std::string val)
{
	setUserId(val);
	saveStringDataByKey("uid", val);
}

std::string JhGameDataSave::addUIDString(std::string val)
{
	return userid + val;
}

void JhGameDataSave::setPlotMissionIndex(int val)
{
	saveIntDataByKey(addUIDString("plot"), val);
}

int JhGameDataSave::getPlotMissionIndex()
{
	return loadIntDataByKey(addUIDString("plot"), 0);
}

void JhGameDataSave::setPlotMissionStatus(std::string strval)
{
	saveStringDataByKey(addUIDString("plotstatus"), strval);
}

std::string JhGameDataSave::getPlotMissionStatus()
{
	return loadStringDataByKey(addUIDString("plotstatus"));
}

void JhGameDataSave::setPlotUnlockChapter(int val)
{
	saveIntDataByKey(addUIDString("unlockchapter"), val);
}

int JhGameDataSave::getPlotUnlockChapter()
{
	return loadIntDataByKey(addUIDString("unlockchapter"), 1);
}

void JhGameDataSave::setBranchPlotMissionStatus(std::string strval)
{
	saveStringDataByKey(addUIDString("newbplotstatus"), strval);
}

std::string JhGameDataSave::getBranchPlotMissionStatus()
{
	return loadStringDataByKey(addUIDString("newbplotstatus"));
}

std::string JhGameDataSave::getSaveListId()
{
	return loadStringDataByKey("savelistid", ";;;");
}

void JhGameDataSave::setBranchPlotMissionGiveGoods(std::string strval)
{
	saveStringDataByKey(addUIDString("bmggs"), strval);
}

std::string JhGameDataSave::getBranchPlotMissionGiveGoods()
{
	return loadStringDataByKey(addUIDString("bmggs"), "");
}

void JhGameDataSave::setSaveListId(std::string val)
{
	saveStringDataByKey("savelistid", val);
}

bool JhGameDataSave::getIsNewerGuide(int index)
{
	std::string key = StringUtils::format("guide%d", index);
	int val = loadIntDataByKey(key, 1);
	return val == 1 ? true : false;
}

void JhGameDataSave::setIsNewerGuide(int index, bool val)
{
	std::string key = StringUtils::format("guide%d", index);
	saveIntDataByKey(key, (int)val);
}


void JhGameDataSave::setExersiceCfg(std::string strval)
{
	saveStringDataByKey(addUIDString("exsercise"), strval);
}

std::string JhGameDataSave::getExersiceCfg()
{
	return loadStringDataByKey(addUIDString("exsercise"), "");
}


void JhGameDataSave::setModifyDefaultStorage(int heroindex, std::string strval)
{
	std::string key = StringUtils::format("hero%ddefstorge", heroindex);
	saveStringDataByKey(key, strval);
}

std::string JhGameDataSave::getModifyDefaultStorage(int heroindex)
{
	std::string key = StringUtils::format("hero%ddefstorge", heroindex);
	return loadStringDataByKey(key, "");
}

void JhGameDataSave::setShareDayOfYear(int day)
{
	saveIntDataByKey("shareday", day);
}

int JhGameDataSave::getShareDayOfYear()
{
	return loadIntDataByKey("shareday", -1);
}

void JhGameDataSave::setWarmConfig(std::string strval)
{
	saveStringDataByKey(addUIDString("warmconfig"), strval);
}


std::string JhGameDataSave::getWarmConfig()
{
	return loadStringDataByKey(addUIDString("warmconfig"), "");
}

void JhGameDataSave::setHeroSex(int val)
{
	saveIntDataByKey(addUIDString("sex"), val);
}

int JhGameDataSave::getHeroSex()
{
	return loadIntDataByKey(addUIDString("sex"), -1);
}

void JhGameDataSave::setHeroExpEndTime(int val)
{
	saveIntDataByKey(addUIDString("heroexpetime"), val);
}

int JhGameDataSave::getHeroExpEndTime()
{
	return loadIntDataByKey(addUIDString("heroexpetime"), 0);
}

void JhGameDataSave::setGfEndTime(int val)
{
	saveIntDataByKey(addUIDString("gfexpetime"), val);
}

int JhGameDataSave::getGfEndTime()
{
	return loadIntDataByKey(addUIDString("gfexpetime"), 0);
}

std::string JhGameDataSave::getExgCfgData()
{
	return loadStringDataByKey(addUIDString("exgcfg"), "");
}

void JhGameDataSave::setExgCfgData(std::string strval)
{
	saveStringDataByKey(addUIDString("exgcfg"), strval);
}

void JhGameDataSave::setWxbMapPos(int val)
{
	saveIntDataByKey(addUIDString("wxbmap"), val);
}

int JhGameDataSave::getWxbMapPos()
{
	return loadIntDataByKey(addUIDString("wxbmap"), 0);
}

void JhGameDataSave::setDgqbMapPos(int val)
{
	saveIntDataByKey(addUIDString("dgqbmap"), val);
}

int JhGameDataSave::getDgqbMapPos()
{
	return loadIntDataByKey(addUIDString("dgqbmap"), 0);
}

void JhGameDataSave::setReviveCount(int val)
{
	saveIntDataByKey(addUIDString("revivecount"), val);
}

int JhGameDataSave::getReviveCount()
{
	return loadIntDataByKey(addUIDString("revivecount"), 1);
}

int JhGameDataSave::getGoldCount()
{
	return loadIntDataByKey("gold", 0);
}

void JhGameDataSave::setGoldCount(int count)
{
	saveIntDataByKey("gold", count);
}

bool JhGameDataSave::getIsPostAllData()
{
	int ret = loadIntDataByKey("postallok", 0);
	return ret == 1 ? true : false;
}

void JhGameDataSave::setIsPostAllData(bool val)
{
	saveIntDataByKey("postallok", val?1:0);
}

bool JhGameDataSave::getIsFirstInstall()
{
	int ret = loadIntDataByKey("firstintall", 1);
	return ret == 1 ? true : false;
}

void JhGameDataSave::setIsFirstInstall(bool val)
{
	saveIntDataByKey("firstintall", val ? 1 : 0);
}

std::string JhGameDataSave::getMyID()
{
	return loadStringDataByKey("id", "");
}

void JhGameDataSave::setMyID(std::string str)
{
	saveStringDataByKey("id", str);
}

std::string JhGameDataSave::getMyNickName()
{
	return loadStringDataByKey("nickname", "");
}

void JhGameDataSave::setMyNickName(std::string str)
{
	saveStringDataByKey("nickname", str);
}

bool JhGameDataSave::getIsBuyTimeGift()
{
	int ret = loadIntDataByKey("isbuytimegift", 0);
	return ret == 1 ? true : false;
}

void JhGameDataSave::setIsBuyTimeGift(bool val)
{
	saveIntDataByKey("isbuytimegift", val ? 1 : 0);
}

void JhGameDataSave::setUseGold(int val)
{
	saveIntDataByKey("costgold", val);
}

int JhGameDataSave::getUseGold()
{
	return loadIntDataByKey("costgold", 0);
}

std::string JhGameDataSave::getFriendly()
{
	return loadStringDataByKey(addUIDString("friendly"), "");
}

std::string JhGameDataSave::getFriendly(std::string localid)
{
	std::string key = localid + "friendly";
	return loadStringDataByKey(key, "");
}

void JhGameDataSave::setFriendly(std::string str)
{
	saveStringDataByKey(addUIDString("friendly"), str);
}

std::string JhGameDataSave::getMixGF()
{
	return loadStringDataByKey(addUIDString("mixgf"), "");
}

std::string JhGameDataSave::getMixGF(std::string localid)
{
	std::string key = localid + "mixgf";
	return loadStringDataByKey(key, "");
}

void JhGameDataSave::setMixGF(std::string str)
{
	saveStringDataByKey(addUIDString("mixgf"), str);
}

int JhGameDataSave::getFreeMixTime(int type)
{
	std::string typestr = StringUtils::format("freemixtime%d", type);
	return loadIntDataByKey(addUIDString(typestr), 0);
}

void JhGameDataSave::setFreeMixTime(int type, int val)
{
	std::string typestr = StringUtils::format("freemixtime%d", type);
	saveIntDataByKey(addUIDString(typestr), val);
}


int JhGameDataSave::getMixGfCountByType(int type)
{
	std::string typestr = StringUtils::format("mixcount%d", type);
	return loadIntDataByKey(addUIDString(typestr), 0);
}

void JhGameDataSave::setMixGfCountByType(int type, int val)
{
	std::string typestr = StringUtils::format("mixcount%d", type);
	saveIntDataByKey(addUIDString(typestr), val);
}

std::string JhGameDataSave::getSuggestMixGf()
{
	return loadStringDataByKey(addUIDString("suggestmixgf"), "");
}

void JhGameDataSave::setSuggestMixGf(std::string str)
{
	saveStringDataByKey(addUIDString("suggestmixgf"), str);
}

int JhGameDataSave::getNoPopNoticeDay()
{
	return loadIntDataByKey("nopopnoticeday", -1);
}

void JhGameDataSave::setNoPopNoticeDay(int day)
{
	saveIntDataByKey("nopopnoticeday", day);
}

int JhGameDataSave::getContributionDay()
{
	return loadIntDataByKey(addUIDString("contributeday"), -1);
}

void JhGameDataSave::setContributionDay(int day)
{
	saveIntDataByKey(addUIDString("contributeday"), day);
}


int JhGameDataSave::getSliverContribution()
{
	return loadIntDataByKey(addUIDString("slivercontribute"), 0);
}


void JhGameDataSave::setSliverContribution(int val)
{
	saveIntDataByKey(addUIDString("slivercontribute"), val);
}

int JhGameDataSave::getGoldContribution()
{
	return loadIntDataByKey(addUIDString("goldcontribute"), 0);
}

void JhGameDataSave::setGoldContribution(int val)
{
	saveIntDataByKey(addUIDString("goldcontribute"), val);
}

int JhGameDataSave::getApprenticeDay()
{
	return loadIntDataByKey(addUIDString("apprentice"), 0);
}
void JhGameDataSave::setApprenticeDay(int val)
{
	saveIntDataByKey(addUIDString("apprentice"), val);
}

std::string JhGameDataSave::getRaffleStage()
{
	return loadStringDataByKey("rafflestage", "");
}

void JhGameDataSave::setRaffleStage(std::string strval)
{
	saveStringDataByKey("rafflestage", strval);
}

std::string JhGameDataSave::getAchiveData()
{
	return loadStringDataByKey(addUIDString("achive"), "");
}

void JhGameDataSave::setAchiveData(std::string strval)
{
	saveStringDataByKey(addUIDString("achive"), strval);
}

std::string JhGameDataSave::getAchiveAnimData()
{
	return loadStringDataByKey(addUIDString("achiveanim"), "");
}

void JhGameDataSave::setAchiveAnimData(std::string strval)
{
	saveStringDataByKey(addUIDString("achiveanim"), strval);
}

void JhGameDataSave::setPlayerChallengeData(std::string strval)
{
	saveStringDataByKey(addUIDString("playerchallenge"), strval);
}

std::string JhGameDataSave::getPlayerChallengeData()
{
	return loadStringDataByKey(addUIDString("playerchallenge"), "");
}

void JhGameDataSave::setHsljMatchPlayer(std::string val)
{
	saveStringDataByKey("shljmatch", val);
}

std::string JhGameDataSave::getHsljMatchPlayer()
{
	return loadStringDataByKey("shljmatch", "");
}

int JhGameDataSave::getEnterGameDaysOfYear()
{
	return loadIntDataByKey(addUIDString("daysofyear"), -1);
}

void JhGameDataSave::setEnterGameDaysOfYear(int days)
{
	saveIntDataByKey(addUIDString("daysofyear"), days);
}