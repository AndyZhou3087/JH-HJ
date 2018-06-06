#include "JhNature.h"
#include "JhConst.h"
#include "JhGameScene.h"
#include "JhCommonFuncs.h"
#include "MD5.h"

int JhNature::ReasonCDays = 30;

JhNature::JhNature()
{
	m_timeinterval = NORMAL_TIMEINTERVAL;
	m_daynight = Night;
	m_ismakewarm = false;
	m_isshowinsect = false;
	m_warmtime = 0;
	m_warmpasttime = 0.0f;
}

JhNature::~JhNature()
{

}

bool JhNature::init()
{
	this->schedule(schedule_selector(JhNature::updateData), NORMAL_TIMEINTERVAL * 1.0f / TIMESCALE);
	return true;
}


void JhNature::ChangeWeather()
{
	int r = JhGlobalData::createRandomNum(100);
	int i = 0;
	for (i = 0; i < 4; i++)
	{
		if (r < WeatherWeight[m_reason][i])
		{
			break;
		}
	}
	EWeather w = (EWeather)i;
	if (w != m_weather)
	{
		int c = w - m_weather;
		setWeather(w);
		setTemperature(m_temperature - c * 5);
		g_uiScroll->addEventText(JhCommonFuncs::gbk2utf(weatherEventText[w].c_str()));
	}


}

void JhNature::ChangeReason()
{
	int yudays = m_pastdays % (4 * ReasonCDays);
	EReason r = Spring;
	if (yudays < ReasonCDays)
	{
		r = Spring;
	}
	else if (yudays < ReasonCDays * 2)
	{
		r = Summer;
	}
	else if (yudays < ReasonCDays * 3)
	{
		r = Autumn;
	}
	else
	{
		r = Winter;
	}
	if (r != m_reason)
	{
		setReason(r);
		ChangeTemperature();

		g_uiScroll->addEventText(JhCommonFuncs::gbk2utf(reasonEventText[r].c_str()));
	}
}


void JhNature::makewarm(int extime)
{
	setIsMaKeWarm(true);
	setTemperature(m_temperature + 15);
	m_warmpasttime = 0.0f;
	m_warmtime = extime;
	//this->scheduleOnce(schedule_selector(JhNature::makewarmover), extime / TIMESCALE);

	std::string str = StringUtils::format("%d-%d", m_pastdays * 24 * 60 + (int)m_time, extime);
	JhGlobalData::setMakeWarmConfig(str);
}

void JhNature::makewarmover()
{
	//取暖时间到
	setIsMaKeWarm(false);
	m_warmpasttime = 0.0f;
	setTemperature(m_temperature - 15);
	JhGlobalData::setMakeWarmConfig("");
}


void JhNature::ChangeDayNight()
{
	if (m_time < 60 * 6 - 1 || m_time >= 18 * 60)
	{
		if (m_daynight == Day)
		{
			setDayOrNight(Night);
			setTemperature(m_temperature - 5);
		}
	}
	else
	{
		if (m_daynight == Night)
		{
			setDayOrNight(Day);
			//setTemperature(m_temperature + 5);
			ChangeTemperature();
		}
	}
}

void JhNature::ChangeTemperature()
{
	int maxr = tempeRange[m_reason][1] - tempeRange[m_reason][0] + 1;
	int  t = tempeRange[m_reason][0] + JhGlobalData::createRandomNum(maxr);
	if (m_ismakewarm)
		t += 15;
	setTemperature(t);
}

void JhNature::setTemperature(int val)
{
	m_temperature = val;

	if (m_temperature < -20)
		m_temperature = 20;
	else if (m_temperature > 25)
		m_temperature = 25;
}

int JhNature::getTemperature()
{
	return m_temperature;
}

void JhNature::updateData(float dt)
{
	if (JhGlobalData::g_gameStatus != GAMESTART || JhGlobalData::isPlayerChallenging)
		return;
	if (g_hero != NULL && g_hero->getIsWDChallenge())
		return;

	m_time += getTimeInterval();

	if (m_time >= 1440.0f)
	{
		if (JhGlobalData::getMD5LiveDays().compare(md5(m_pastdays)) != 0)
		{
			JhGlobalData::dataIsModified = true;
			m_pastdays = 0;
		}

		m_pastdays++;

		JhGlobalData::doAchive(A_11, m_pastdays);
		JhGlobalData::setMD5LiveDays(md5(m_pastdays));
		m_time -= 1440.0f;
		changeWeatherCount = 0;
		changeWeatherRandow = JhGlobalData::createRandomNum(24) + 1;
		ChangeReason();
	}
	if (m_ismakewarm)
	{
		//制暖时间
		m_warmpasttime += getTimeInterval();

		if (m_warmpasttime >= m_warmtime)//之前时间到结束
		{
			makewarmover();
		}
	}
	//产生随机数
	int inttime = (int)m_time;
	if (changeWeatherRandow <= 0)
		changeWeatherRandow = JhGlobalData::createRandomNum(23) + 1;
	if (changeWeatherRandow == inttime / 60)
	{
		changeWeatherCount++;
		if (changeWeatherCount <= 1)//每天变化一次
		{
			ChangeWeather();
		}
	}

	ChangeDayNight();
}