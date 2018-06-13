#include "JhCookTable.h"
#include "JhNature.h"
#include "JhConst.h"
extern JhNature* g_nature;
JhCookTable::JhCookTable()
{

}


JhCookTable::~JhCookTable()
{

}

bool JhCookTable::init()
{
	if (Sprite::initWithSpriteFrameName("ui/cooktable.png"))
	{

		return true;
	}
	return false;
}

void JhCookTable::action(int minute, int exminute)
{
	g_nature->setTimeInterval(minute * NORMAL_TIMEINTERVAL * 1.0f / (TIMESCALE* getActionBarTime()));
}