#include "JhExerciseRoom.h"
#include "JhNature.h"
#include "JhHero.h"
#include "JhConst.h"

extern JhNature* g_nature;
extern JhHero* g_hero;

JhExerciseRoom::JhExerciseRoom()
{
}


JhExerciseRoom::~JhExerciseRoom()
{

}

bool JhExerciseRoom::init()
{
	if (Sprite::initWithSpriteFrameName("ui/exerciseroom.png"))
	{

		return true;
	}
	return false;
}

void JhExerciseRoom::action(int minute, int exminute)
{

}