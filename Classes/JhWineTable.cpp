#include "JhWineTable.h"
#include "JhHero.h"

#include "JhNature.h"
#include "JhConst.h"
extern JhNature* g_nature;
extern JhHero* g_hero;
JhWineTable::JhWineTable()
{

}


JhWineTable::~JhWineTable()
{

}

bool JhWineTable::init()
{
	if (Sprite::initWithSpriteFrameName("ui/winetable.png"))
	{

		return true;
	}
	return false;
}

void JhWineTable::action(int minute, int exminute)
{
	g_hero->drinking();
	g_nature->setTimeInterval(minute * NORMAL_TIMEINTERVAL * 1.0f / (getActionBarTime() * TIMESCALE));
}