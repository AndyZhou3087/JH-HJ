#include "JhMedicineKit.h"
#include "JhNature.h"
#include "JhConst.h"
#include "JhGameScene.h"

JhMedicineKit::JhMedicineKit()
{

}


JhMedicineKit::~JhMedicineKit()
{

}

bool JhMedicineKit::init()
{
	if (Sprite::initWithSpriteFrameName("ui/medicinekit.png"))
	{

		return true;
	}
	return false;
}

void JhMedicineKit::action(int minute, int exminute)
{
	g_nature->setTimeInterval(minute * NORMAL_TIMEINTERVAL * 1.0f / (getActionBarTime() * TIMESCALE));
}