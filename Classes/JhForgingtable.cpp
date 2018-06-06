#include "JhForgingtable.h"

#include "JhNature.h"
#include "JhConst.h"
extern JhNature* g_nature;

JhForgingtable::JhForgingtable()
{
}


JhForgingtable::~JhForgingtable()
{

}


bool JhForgingtable::init()
{
	if (Sprite::initWithSpriteFrameName("ui/forgingtable.png"))
	{

		return true;
	}
	return false;
}

void JhForgingtable::action(int minute, int exminute)
{
	//锻造
	g_nature->setTimeInterval(minute * NORMAL_TIMEINTERVAL * 1.0f / (getActionBarTime() * TIMESCALE));
}