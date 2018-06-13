#include "JhWineMaker.h"
#include "JhNature.h"
#include "JhConst.h"
extern JhNature* g_nature;

JhWineMaker::JhWineMaker()
{

}


JhWineMaker::~JhWineMaker()
{

}

bool JhWineMaker::init()
{
	if (Sprite::initWithSpriteFrameName("ui/winemaker.png"))
	{

		return true;
	}
	return false;
}

void JhWineMaker::action(int minute, int exminute)
{
	g_nature->setTimeInterval(minute * NORMAL_TIMEINTERVAL * 1.0f / (getActionBarTime() * TIMESCALE));
}