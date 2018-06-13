#include "JhHero.h"
#include "JhConst.h"
#include "JhNature.h"
#include "JhGameScene.h"
#include "JhNewerGuideLayer.h"
float JhHero::MAXInnerinjuryValue = 100.0f;
float JhHero::MAXOutinjuryValue = 100.0f;
float JhHero::MAXHungerValue = 100.0f;
float JhHero::MAXSpiritValue = 100.0f;

#define HungerSpeed 2//饱食度下降速度
#define InnerinjurySpeed 1//内伤下降速度
#define OutinjurySpeed 1//外伤下降速度
#define SpiritSpeed 1.5f//精神下降速度
#define LifeLostSpeed 2//生命下降速度（按百分比）

#define SeriousInnerinjury 40.0f//严重内伤界限
#define SeriousOutinjury 40.0f//严重外伤界限
#define SeriousHunger 40.0f//严重饥饿界限

JhHero::JhHero()
{
	maxlifepercent = 1.0f;
	liferecoverpercent = 1.0f;
	m_atkpercent = 1.0f;
	m_dfpercent = 1.0f;
	m_maxHunger = MAXHungerValue;
	m_maxInnerinjury = MAXInnerinjuryValue;
	m_maxOutinjury = MAXOutinjuryValue;
	m_maxSpirit = MAXSpiritValue;

	injuryrecoverpercent = 1.0f;
	outjuryrecoverpercent = 1.0f;
	hungerrecoverpercent = 1.0f;
	sleepLostPercent = 1.0f;

	m_pastmin = 0;

	m_totalAtkBonusPercent = 0.0f;
	m_totalDfBonusPercent = 0.0f;
	m_isWDChallenge = false;
	m_isMoving = false;
}


JhHero::~JhHero()
{

}

bool JhHero::init()
{
	this->schedule(schedule_selector(JhHero::updateData), 1.0f/*60.0f / TIMESCALE*/);
	this->schedule(schedule_selector(JhHero::checkMaxVaule), 1.0f);
	return true;
}

void JhHero::updateData(float dt)
{
	if (JhGlobalData::g_gameStatus != GAMESTART)
		return;

	if (m_isWDChallenge || JhGlobalData::isPlayerChallenging)
		return;

	//12s，（游戏时间1小时更新一次）
	if (g_nature != NULL)
		m_pastmin += g_nature->getTimeInterval();

	if (m_pastmin >= 60)
	{
		int hour = m_pastmin / 60;
		m_pastmin = m_pastmin % 60;

		//新手引导不扣属性消耗
		if (g_NewerGuideLayer != NULL)
			return;

		//闭关没有分身符不扣属性消耗， 有分身符要扣
		if (JhGlobalData::isExercising() && !JhGlobalData::isHasFSF())
			return;

		if (m_outinjury < SeriousOutinjury)//严重外伤，内伤3倍下降
		{
			if (m_innerinjury < MAXInnerinjuryValue)
				m_innerinjury -= hour * InnerinjurySpeed * 3.0f;
			m_life -= hour * LifeLostSpeed * 1.0f * JhGlobalData::map_heroAtr[getHeadID()].vec_maxhp[getLVValue()] / 100.0f;
		}
		if (m_hunger < SeriousHunger)//过度饥饿 2倍外伤，2倍内伤下降
		{
			if (m_outinjury < MAXOutinjuryValue)
				m_outinjury -= hour * OutinjurySpeed * 2.0f;
			if (m_innerinjury < MAXInnerinjuryValue)
				m_innerinjury -= hour * InnerinjurySpeed * 2.0f;
			m_life -= hour * LifeLostSpeed * 1.0f * JhGlobalData::map_heroAtr[getHeadID()].vec_maxhp[getLVValue()] / 100.0f;
		}
		if (m_innerinjury < SeriousInnerinjury)//严重内伤，外伤2倍下降
		{
			if (m_outinjury < MAXOutinjuryValue)
				m_outinjury -= hour * OutinjurySpeed * 3.0f;
			m_life -= hour * LifeLostSpeed * 1.0f * JhGlobalData::map_heroAtr[getHeadID()].vec_maxhp[getLVValue()] / 100.0f;
		}

		//接上严重界限的消耗
		m_hunger -= hour * HungerSpeed * 1.0f * sleepLostPercent;

		if (m_innerinjury < MAXInnerinjuryValue)
		{
			if (m_innerinjury > 90)
				m_innerinjury -= hour * InnerinjurySpeed * 0.3f;
			else if (m_innerinjury > 70)
				m_innerinjury -= hour * InnerinjurySpeed * 0.5f;
			else
				m_innerinjury -= hour * InnerinjurySpeed * 2.0f;
		}
		if (m_outinjury < MAXOutinjuryValue)
		{
			if (m_outinjury > 90)
				m_outinjury -= hour * OutinjurySpeed * 0.3f;
			else if (m_outinjury > 70)
				m_outinjury -= hour * OutinjurySpeed * 0.5f;
			else
				m_outinjury -= hour * OutinjurySpeed * 2.0f;

			if (g_nature->getTemperature() > 20)
			{
				m_outinjury -= hour * OutinjurySpeed * 1.0f;
			}
		}

		else if (g_nature->getTemperature() < -5)
		{
			m_life -= hour * LifeLostSpeed * 1.0f * JhGlobalData::map_heroAtr[getHeadID()].vec_maxhp[getLVValue()] / 100.0f;
		}

		m_spirit -= hour * SpiritSpeed * 1.0f * sleepLostPercent;

		if (m_innerinjury < 0.0f)
			m_innerinjury = 0.0f;
		if (m_outinjury < 0.0f)
			m_outinjury = 0.0f;
		if (m_hunger < 0.0f)
			m_hunger = 0.0f;
		if (m_life < 0.0f)
			m_life = 0.0f;
		if (m_spirit < 0.0f)
			m_spirit = 0.0f;
	}

}

void JhHero::sleep(int losttime, int hour)
{
	//按次恢复生命
	float delay = 0.0f;
	if (losttime > TIMESCALE * ACTION_BAR_TIME)
		delay = losttime / TIMESCALE;
	else
		delay = ACTION_BAR_TIME;

	float pertime = delay / (TIMESCALE* ACTION_BAR_TIME);
	sleephour = hour;
	this->schedule(schedule_selector(JhHero::sleepbystep), pertime, TIMESCALE* ACTION_BAR_TIME - 1, 0.0f);
	sleepLostPercent = 0.5f;


	this->scheduleOnce(schedule_selector(JhHero::sleepDone), delay);
}

void JhHero::sleepbystep(float dt)
{
	//每次恢复的生命值
	m_life += getMaxLifeValue() * sleephour * 0.1f * liferecoverpercent / (TIMESCALE* ACTION_BAR_TIME);
	if (m_life > getRecoverLifeMaxValue())
	{
		m_life = getRecoverLifeMaxValue();
		this->unschedule(schedule_selector(JhHero::sleepbystep));
	}
}

void JhHero::sleepDone(float dt)
{
	sleepLostPercent = 1.0f;
}

void JhHero::drinking()
{
	//按次恢复精神值
	this->schedule(schedule_selector(JhHero::drinkbystep), 0.2f, TIMESCALE* ACTION_BAR_TIME - 1, 0.0f);
}

void JhHero::drinkbystep(float dt)
{
	//每次恢复精神值
	m_spirit += 2.0f;
	if (m_spirit > getMaxSpiritValue())
	{
		m_spirit = getMaxSpiritValue();
		this->unschedule(schedule_selector(JhHero::drinkbystep));
	}
}

int JhHero::getAtkValue()
{
	return JhGlobalData::map_heroAtr[getHeadID()].vec_atk[getLVValue()];
}

int JhHero::getDfValue()
{
	return JhGlobalData::map_heroAtr[getHeadID()].vec_df[getLVValue()];
}

float JhHero::getMaxLifeValue()
{
	float friendhppercent = 0.0f;
	std::map<std::string, FriendlyData>::iterator it;
	for (it = JhGlobalData::map_myfriendly.begin(); it != JhGlobalData::map_myfriendly.end(); ++it)
	{
		std::string nid = it->first;
		if (JhGlobalData::map_myfriendly[nid].relation == F_FRIEND)
		{
			friendhppercent += JhGlobalData::map_NPCFriendData[nid].hppercent / 100;
		}
		else if (JhGlobalData::map_myfriendly[nid].relation == F_MASTER)
		{
			friendhppercent += JhGlobalData::map_NPCMasterData[nid].hppercent / 100;
		}
	}

	float mixfglifepercent = 0.0f;

	std::string mymixgf = JhGlobalData::getMixGF();
	if (mymixgf.length() > 0)
	{
		MixGfData mdata = JhGlobalData::map_MixGfData[mymixgf];
		if ((g_hero->getAtrByType(H_WG)->count > 0 && g_hero->getAtrByType(H_WG)->strid.compare(mdata.mastergf) == 0) || (g_hero->getAtrByType(H_NG)->count > 0 && g_hero->getAtrByType(H_NG)->strid.compare(mdata.mastergf) == 0))
			mixfglifepercent += mdata.hppercent / 100;
	}

	float flife = JhGlobalData::map_heroAtr[getHeadID()].vec_maxhp[getLVValue()] * 1.0f;
	flife = flife + flife*friendhppercent + flife * mixfglifepercent;

	if (JhGlobalData::myFactionlv == 3)
		flife += flife*0.02f;
	return flife;
}

float JhHero::getRecoverLifeMaxValue()
{
	float flife = maxlifepercent * getMaxLifeValue();
	return flife;
}

void JhHero::setAtrByType(HeroAtrType type, PackageData pData)
{
	map_heroAtr[type] = pData;
	saveProperData();
}

PackageData* JhHero::getAtrByType(HeroAtrType type)
{
	return &map_heroAtr[type];
}

void JhHero::saveProperData()
{
	//保存装备栏数据
	std::string str;
	for (int i = H_WEAPON; i < H_MAX; i++)
	{
		PackageData* sdata = g_hero->getAtrByType((HeroAtrType)i);

		std::string idstr = StringUtils::format("%s-%d-%d-%d-%d-%d-%d-%d-%d;", sdata->strid.c_str(), sdata->type, sdata->count, sdata->extype, sdata->lv, sdata->exp, sdata->goodvalue, sdata->slv, sdata->tqu);
		str.append(idstr);
	}
	JhGlobalData::setHeroProperData(str.substr(0, str.length() - 1));
}

void JhHero::revive()
{
	//复活-满状态复活

	maxlifepercent = 1.0f;
	liferecoverpercent = 1.0f;
	m_atkpercent = 1.0f;
	m_dfpercent = 1.0f;

	setOutinjuryValue(MAXOutinjuryValue);
	setMaxOutinjuryValue(MAXOutinjuryValue);

	setInnerinjuryValue(MAXInnerinjuryValue);
	setMaxInnerinjuryValue(MAXInnerinjuryValue);

	setLifeValue(getMaxLifeValue());
	
	setHungerValue(MAXHungerValue);
	setMaxHungerValue(MAXHungerValue);

	setSpiritValue(MAXSpiritValue);
	setMaxSpiritValue(MAXSpiritValue);
}

//功法每种只能有一本，不能重复
bool JhHero::checkifHasGF_Equip(std::string gfeid)
{
	HeroAtrType heroproptype[] = { H_WG, H_NG, H_ARMOR, H_WEAPON };

	//装备栏是否有
	for (int i = 0; i < sizeof(heroproptype) / sizeof(heroproptype[0]); i++)
	{
		if (getAtrByType(heroproptype[i])->count > 0 && getAtrByType(heroproptype[i])->strid.compare(gfeid) == 0)
			return true;
	}

	//背包中是否有
	for (int i = 0; i < JhMyPackage::getSize(); i++)
	{
		PackageData* pone = &JhMyPackage::vec_packages[i];
		if (pone->strid.compare(gfeid) == 0 && (pone->type == N_GONG || pone->type == W_GONG || pone->type == WEAPON || pone->type == PROTECT_EQU))
			return true;
	}
	//仓库中是否有
	StorageType storagetype[] = { N_GONG, W_GONG, WEAPON, PROTECT_EQU };

	for (int m = 0; m < sizeof(storagetype) / sizeof(storagetype[0]); m++)
	{
		StorageType stype = storagetype[m];
		for (unsigned i = 0; i < JhStorageRoom::map_storageData[stype].size(); i++)
		{
			if (JhStorageRoom::map_storageData[stype][i].strid.compare(gfeid) == 0)
				return true;
		}
	}

	std::string mymixgf = JhGlobalData::getMixGF();
	if (mymixgf.length() > 0)
	{
		MixGfData mdata = JhGlobalData::map_MixGfData[mymixgf];
		for (unsigned int n = 0; n < mdata.vec_secgf.size(); n++)
		{
			if (gfeid.compare(mdata.vec_secgf[n]) == 0)
				return true;
		}
	}

	return false;
}

PackageData* JhHero::getGF_Equip(std::string gfeid)
{
	HeroAtrType heroproptype[] = { H_WG, H_NG, H_ARMOR, H_WEAPON };

	for (int i = 0; i < sizeof(heroproptype) / sizeof(heroproptype[0]); i++)
	{
		if (getAtrByType(heroproptype[i])->count > 0 && getAtrByType(heroproptype[i])->strid.compare(gfeid) == 0)
			return getAtrByType(heroproptype[i]);
	}

	//背包中是否有
	for (int i = 0; i < JhMyPackage::getSize(); i++)
	{
		PackageData* pone = &JhMyPackage::vec_packages[i];
		if (pone->strid.compare(gfeid) == 0 && (pone->type == N_GONG || pone->type == W_GONG || pone->type == WEAPON || pone->type == PROTECT_EQU))
			return &JhMyPackage::vec_packages[i];
	}
	//仓库中是否有
	StorageType storagetype[] = { N_GONG, W_GONG, WEAPON, PROTECT_EQU};

	for (int m = 0; m < sizeof(storagetype) / sizeof(storagetype[0]); m++)
	{
		StorageType stype = storagetype[m];
		for (unsigned i = 0; i < JhStorageRoom::map_storageData[stype].size(); i++)
		{
			if (JhStorageRoom::map_storageData[stype][i].strid.compare(gfeid) == 0)
				return &JhStorageRoom::map_storageData[stype][i];
		}
	}



	return NULL;
}

PackageData* JhHero::getMeHas(std::string strid)
{
	//装备栏是否有
	for (int i = H_WEAPON; i < H_MAX; i++)
	{
		if (getAtrByType((HeroAtrType)i)->count > 0 && getAtrByType((HeroAtrType)i)->strid.compare(strid) == 0)
		{
			return getAtrByType((HeroAtrType)i);
		}
	}
	//背包中是否有
	for (int i = 0; i < JhMyPackage::getSize(); i++)
	{
		if (JhMyPackage::vec_packages[i].strid.compare(strid) == 0)
			return &JhMyPackage::vec_packages[i];
	}
	//仓库中是否有

	std::map<int, std::vector<PackageData>>::iterator it;
	for (it = JhStorageRoom::map_storageData.begin(); it != JhStorageRoom::map_storageData.end(); ++it)
	{
		int size = JhStorageRoom::map_storageData[it->first].size();
		for (int j = 0; j < size; j++)
		{
			PackageData sdata = JhStorageRoom::map_storageData[it->first][j];
			if (sdata.strid.compare(strid) == 0)
				return &JhStorageRoom::map_storageData[it->first][j];
		}
	}
	return NULL;
}

int JhHero::getGfCountByLv(int lv)
{
	int count = 0;
	//装备栏是否有
	for (int i = H_WG; i <= H_NG; i++)
	{
		if (getAtrByType((HeroAtrType)i)->count > 0 && getAtrByType((HeroAtrType)i)->lv >= lv-1)
		{
			count++;
		}
	}
	//背包中是否有
	for (int i = 0; i < JhMyPackage::getSize(); i++)
	{
		if ((JhMyPackage::vec_packages[i].type == W_GONG || JhMyPackage::vec_packages[i].type == N_GONG) && JhMyPackage::vec_packages[i].lv >= lv-1)
			count++;
	}
	//仓库中是否有
	for (int i = N_GONG; i <= W_GONG; i++)
	{
		int size = JhStorageRoom::map_storageData[i].size();
		for (int j = 0; j < size; j++)
		{
			PackageData sdata = JhStorageRoom::map_storageData[i][j];
			if (sdata.lv >= lv - 1)
				count++;
		}
	}
	return count;
}

void JhHero::checkMaxVaule(float dt)
{
	float mlife = 1.0f;

	float matk = 1.0f;
	float mdf = 1.0f;
	if (m_spirit <= 10.0f)
	{
		liferecoverpercent = 0.3f;
		injuryrecoverpercent = 0.3f;
		outjuryrecoverpercent = 0.3f;
		hungerrecoverpercent = 0.3f;
		matk = 0.7f;
		mdf = 0.7f;
	}
	else if (m_spirit <= 20.0f)
	{
		liferecoverpercent = 0.4f;
		injuryrecoverpercent = 0.4f;
		outjuryrecoverpercent = 0.4f;
		hungerrecoverpercent = 0.4f;
		matk = 0.9f;
		mdf = 0.9f;
	}

	else if (m_spirit <= 30.0f)
	{
		liferecoverpercent = 0.7f;
		injuryrecoverpercent = 0.7f;
		outjuryrecoverpercent = 0.7f;
		hungerrecoverpercent = 0.7f;
		matk =  1.0f;
		mdf = 1.0f;
	}

	else if (m_spirit <= 40.0f)
	{
		liferecoverpercent = 0.9f;
		injuryrecoverpercent = 0.9f;
		outjuryrecoverpercent = 0.9f;
		hungerrecoverpercent = 0.9f;
		matk = 1.0f;
		mdf = 1.0f;
	}
	else
	{
		liferecoverpercent = 1.0f;
		injuryrecoverpercent = 1.0f;
		outjuryrecoverpercent = 1.0f;
		hungerrecoverpercent = 1.0f;
		matk = 1.0f;
		mdf = 1.0f;
	}

	if (m_hunger <= 10.0f)
	{
		mlife = MIN(mlife, 0.3f);
		matk = MIN(matk, 0.3f);
		mdf = MIN(mdf, 0.3);
	}
	else if (m_hunger <= 20.0f)
	{
		mlife = MIN(mlife, 0.4f);
		matk = MIN(matk, 0.4f);
		mdf = MIN(mdf, 0.4);
	}
	else if (m_hunger <= 30.0f)
	{
		mlife = MIN(mlife, 1.0f);
		matk = MIN(matk, 0.7f);
		mdf = MIN(mdf, 0.7);
	}
	else if (m_hunger <= 40.0f)
	{
		mlife = MIN(mlife, 1.0f);
		matk = MIN(matk, 0.9f);
		mdf = MIN(mdf, 0.9);
	}
	else
	{
		mlife = MIN(mlife, 1.0f);
		matk = MIN(matk, 1.0f);
		mdf = MIN(mdf, 1.0);
	}

	if (m_innerinjury <= 10.0f)
	{
		mlife = MIN(mlife, 0.3f);
	}
	else if (m_innerinjury <= 20.0f)
	{
		mlife = MIN(mlife, 0.4f);
	}
	else if (m_innerinjury <= 30.0f)
	{
		mlife = MIN(mlife, 0.7f);
	}
	else if (m_innerinjury <= 40.0f)
	{
		mlife = MIN(mlife, 0.9f);
	}
	else
	{
		mlife = MIN(mlife, 1.0f);
	}

	if (m_outinjury <= 10.0f)
	{
		mlife = MIN(mlife, 0.3f);
	}
	else if (m_outinjury <= 20.0f)
	{
		mlife = MIN(mlife, 0.4f);
	}
	else if (m_outinjury <= 30.0f)
	{
		mlife = MIN(mlife, 0.7f);
	}
	else if (m_outinjury <= 40.0f)
	{
		mlife = MIN(mlife, 0.9f);
	}
	else
	{
		mlife = MIN(mlife, 1.0f);
	}

	maxlifepercent = mlife;

	if (m_life > getRecoverLifeMaxValue())
		setLifeValue(getRecoverLifeMaxValue());

	setAtkPercent(matk);
	setDfPercent(mdf);
}

void JhHero::recoverOutjury(int val)
{
	float fval = val * outjuryrecoverpercent;
	m_outinjury += fval;
}

void JhHero::recoverInjury(int val)
{
	float fval = val * injuryrecoverpercent;
	m_innerinjury += fval;
}

void JhHero::recoverHunger(int val)
{
	float fval = val* hungerrecoverpercent;
	m_hunger += fval;
}

int JhHero::getTotalDf()
{
	int adf = 0;
	int ngdf = 0;
	float slvdf = 0.0f;//强化等级加防

	float frienddfpercent = 0.0f;
	std::string masternpc;
	std::map<std::string, FriendlyData>::iterator it;
	for (it = JhGlobalData::map_myfriendly.begin(); it != JhGlobalData::map_myfriendly.end(); ++it)
	{
		std::string nid = it->first;
		if (JhGlobalData::map_myfriendly[nid].relation == F_FRIEND)
		{
			frienddfpercent += JhGlobalData::map_NPCFriendData[nid].dfpercent / 100;
		}
		else if (JhGlobalData::map_myfriendly[nid].relation == F_MASTER)
		{
			frienddfpercent += JhGlobalData::map_NPCMasterData[nid].dfpercent / 100;
			masternpc = nid;
		}
	}

	if (g_hero->getAtrByType(H_NG)->count > 0)
	{
		std::string gfname = g_hero->getAtrByType(H_NG)->strid;
		int gflv = g_hero->getAtrByType(H_NG)->lv;

		if (gflv > JhGlobalData::map_wgngs[gfname].maxlv - 1)
			gflv = JhGlobalData::map_wgngs[gfname].maxlv - 1;
		ngdf = JhGlobalData::map_wgngs[gfname].vec_bns[gflv];

		int masterdf = 0;
		if (masternpc.length() > 0)
		{
			for (unsigned int i = 0; i < JhGlobalData::map_NPCMasterData[masternpc].vec_gfid.size(); i++)
			{
				if (gfname.compare(JhGlobalData::map_NPCMasterData[masternpc].vec_gfid[i]) == 0)
				{
					int msbdf = ngdf * JhGlobalData::map_NPCMasterData[masternpc].vec_gfbonus[i] / 100;
					masterdf += msbdf;

					break;
				}
			}
		}

		int mixdf = 0;
		std::string mymixgf = JhGlobalData::getMixGF();
		if (mymixgf.length() > 0)
		{
			MixGfData mdata = JhGlobalData::map_MixGfData[mymixgf];
			if ((g_hero->getAtrByType(H_WG)->count > 0 && g_hero->getAtrByType(H_WG)->strid.compare(mdata.mastergf) == 0) || (g_hero->getAtrByType(H_NG)->count > 0 && g_hero->getAtrByType(H_NG)->strid.compare(mdata.mastergf) == 0))
				mixdf += ngdf * mdata.dfpercent / 100;
		}
		ngdf = ngdf + masterdf + mixdf;
	}

	if (g_hero->getAtrByType(H_ARMOR)->count > 0 && g_hero->getAtrByType(H_ARMOR)->goodvalue > 0)
	{
		std::string aname = g_hero->getAtrByType(H_ARMOR)->strid;
		adf = JhGlobalData::map_equips[aname].df;
		int slv = g_hero->getAtrByType(H_ARMOR)->slv;
		slvdf = slv * (slv + adf / 10);
	}
	//防御属性
	float fdf = g_hero->getDfPercent() *(g_hero->getDfValue() + ngdf + adf);
	fdf += slvdf;

	fdf += fdf*m_totalDfBonusPercent;


	fdf += fdf*frienddfpercent;

	if (JhGlobalData::myFactionlv == 4)
		fdf += fdf*0.02f;

	adf = int(fdf + 0.5f);
	return adf;
}

int JhHero::getTotalAtck()
{
	int weaponAtk = 0;
	int wgAtk = 0;
	float slvAtk = 0.0f;//强化等级加攻
	if (g_hero->getAtrByType(H_WEAPON)->count > 0 && g_hero->getAtrByType(H_WEAPON)->goodvalue > 0)
	{
		std::string strid = g_hero->getAtrByType(H_WEAPON)->strid;
		weaponAtk = JhGlobalData::map_equips[strid].atk;
		int slv = g_hero->getAtrByType(H_WEAPON)->slv;
		slvAtk = slv * (slv + weaponAtk / 10);
	}

	float friendatkpercent = 0.0f;
	std::string masternpc;
	std::map<std::string, FriendlyData>::iterator it;
	for (it = JhGlobalData::map_myfriendly.begin(); it != JhGlobalData::map_myfriendly.end(); ++it)
	{
		std::string nid = it->first;
		if (JhGlobalData::map_myfriendly[nid].relation == F_FRIEND)
		{
			friendatkpercent += JhGlobalData::map_NPCFriendData[nid].atkpercent / 100;
		}
		else if (JhGlobalData::map_myfriendly[nid].relation == F_MASTER)
		{
			friendatkpercent += JhGlobalData::map_NPCMasterData[nid].atkpercent / 100;
			masternpc = nid;
		}
	}

	if (g_hero->getAtrByType(H_WG)->count > 0)
	{
		std::string strid = g_hero->getAtrByType(H_WG)->strid;
		int gflv = g_hero->getAtrByType(H_WG)->lv;
		if (gflv > JhGlobalData::map_wgngs[strid].maxlv - 1)
			gflv = JhGlobalData::map_wgngs[strid].maxlv - 1;
		wgAtk = JhGlobalData::map_wgngs[strid].vec_bns[gflv];

		int masteratk = 0;
		if (masternpc.length() > 0)
		{
			for (unsigned int i = 0; i < JhGlobalData::map_NPCMasterData[masternpc].vec_gfid.size(); i++)
			{
				if (strid.compare(JhGlobalData::map_NPCMasterData[masternpc].vec_gfid[i]) == 0)
				{
					int msbatk = wgAtk * JhGlobalData::map_NPCMasterData[masternpc].vec_gfbonus[i] / 100;
					masteratk += msbatk;
					break;
				}
			}
		}

		int mixatk = 0;
		std::string mymixgf = JhGlobalData::getMixGF();
		if (mymixgf.length() > 0)
		{
			MixGfData mdata = JhGlobalData::map_MixGfData[mymixgf];
			if ((g_hero->getAtrByType(H_WG)->count > 0 && g_hero->getAtrByType(H_WG)->strid.compare(mdata.mastergf) == 0) || (g_hero->getAtrByType(H_NG)->count > 0 && g_hero->getAtrByType(H_NG)->strid.compare(mdata.mastergf) == 0))
				mixatk += wgAtk * mdata.atkpercent / 100;
		}
		wgAtk = wgAtk + masteratk + mixatk;
	}

	//攻击属性
	float fack = g_hero->getAtkPercent() * (g_hero->getAtkValue() + weaponAtk + wgAtk);

	if (g_hero->getAtrByType(H_WG)->count > 0 && g_hero->getAtrByType(H_WEAPON)->count > 0 && g_hero->getAtrByType(H_WEAPON)->goodvalue > 0)
	{
		if (JhGlobalData::map_wgngs[g_hero->getAtrByType(H_WG)->strid].extype == JhGlobalData::map_equips[g_hero->getAtrByType(H_WEAPON)->strid].extype)
		{
			float back = fack * 0.05f;
			fack += back;
		}
	}
	fack += slvAtk;

	fack += fack* m_totalAtkBonusPercent;

	fack += fack* friendatkpercent;

	if (JhGlobalData::myFactionlv == 5)
		fack += fack*0.02f;

	int tatk = int(fack + 0.5f);
	return tatk;
}

float JhHero::getCritRate()
{
	int critrnd = JhGlobalData::map_heroAtr[g_hero->getHeadID()].vec_crit[g_hero->getLVValue()];
	if (g_hero->getAtrByType(H_WG)->count > 0)
		critrnd += JhGlobalData::map_wgngs[g_hero->getAtrByType(H_WG)->strid].vec_cirt[g_hero->getAtrByType(H_WG)->lv];

	float friendcritrnd = 0.0f;
	std::map<std::string, FriendlyData>::iterator it;
	for (it = JhGlobalData::map_myfriendly.begin(); it != JhGlobalData::map_myfriendly.end(); ++it)
	{
		std::string nid = it->first;
		if (JhGlobalData::map_myfriendly[nid].relation == F_FRIEND)
		{
			friendcritrnd += JhGlobalData::map_NPCFriendData[nid].critpercent;
		}
		else if (JhGlobalData::map_myfriendly[nid].relation == F_MASTER)
		{
			friendcritrnd += JhGlobalData::map_NPCMasterData[nid].critpercent;
		}
	}
	critrnd += friendcritrnd;

	float mixcrit = 0.0f;
	std::string mymixgf = JhGlobalData::getMixGF();
	if (mymixgf.length() > 0)
	{
		MixGfData mdata = JhGlobalData::map_MixGfData[mymixgf];
		if ((g_hero->getAtrByType(H_WG)->count > 0 && g_hero->getAtrByType(H_WG)->strid.compare(mdata.mastergf) == 0) || (g_hero->getAtrByType(H_NG)->count > 0 && g_hero->getAtrByType(H_NG)->strid.compare(mdata.mastergf) == 0))
			mixcrit += mdata.critpercent;
	}
	critrnd += mixcrit;
	if (JhGlobalData::myFactionlv == 2)
		critrnd += 2;
	return critrnd;
}

float JhHero::getdodgeRate()
{
	int dodgernd = JhGlobalData::map_heroAtr[g_hero->getHeadID()].vec_dodge[g_hero->getLVValue()];

	if (g_hero->getAtrByType(H_NG)->count > 0)
		dodgernd += JhGlobalData::map_wgngs[g_hero->getAtrByType(H_NG)->strid].vec_dodge[g_hero->getAtrByType(H_NG)->lv];

	float frienddogdernd = 0.0f;
	std::map<std::string, FriendlyData>::iterator it;
	for (it = JhGlobalData::map_myfriendly.begin(); it != JhGlobalData::map_myfriendly.end(); ++it)
	{
		std::string nid = it->first;
		if (JhGlobalData::map_myfriendly[nid].relation == F_FRIEND)
		{
			frienddogdernd += JhGlobalData::map_NPCFriendData[nid].dodgepercent;
		}
		else if (JhGlobalData::map_myfriendly[nid].relation == F_MASTER)
		{
			frienddogdernd += JhGlobalData::map_NPCMasterData[nid].dodgepercent;
		}
	}
	dodgernd += frienddogdernd;

	float mixdodge = 0.0f;
	std::string mymixgf = JhGlobalData::getMixGF();
	if (mymixgf.length() > 0)
	{
		MixGfData mdata = JhGlobalData::map_MixGfData[mymixgf];
		if ((g_hero->getAtrByType(H_WG)->count > 0 && g_hero->getAtrByType(H_WG)->strid.compare(mdata.mastergf) == 0) || (g_hero->getAtrByType(H_NG)->count > 0 && g_hero->getAtrByType(H_NG)->strid.compare(mdata.mastergf) == 0))
			mixdodge += mdata.dodgepercent;
	}
	dodgernd += mixdodge;
	if (JhGlobalData::myFactionlv == 1)
		dodgernd += 2;

	return dodgernd;
}