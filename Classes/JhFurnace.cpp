#include "JhFurnace.h"
#include "JhNature.h"
#include "JhConst.h"
#include "JhHintBox.h"
#include "JhCommonFuncs.h"
extern JhNature* g_nature;

JhFurnace::JhFurnace()
{
}


JhFurnace::~JhFurnace()
{

}

bool JhFurnace::init()
{
	if (Sprite::initWithSpriteFrameName("ui/furnace.png"))
	{

		return true;
	}
	return false;
}

void JhFurnace::action(int minute, int exminute)
{
	//生火
	extime = exminute;
	g_nature->setTimeInterval(minute * NORMAL_TIMEINTERVAL * 1.0f / (TIMESCALE* getActionBarTime()));

	this->scheduleOnce(schedule_selector(JhFurnace::onfire), getActionBarTime());
}
void JhFurnace::onfire(float dt)
{
	//温度+15°
	g_nature->makewarm(extime);
	
}