#include "JhFence.h"
#include "JhConst.h"

JhFence::JhFence()
{
}


JhFence::~JhFence()
{

}

bool JhFence::init()
{
	if (Sprite::initWithSpriteFrameName("ui/fence.png"))
	{

		return true;
	}
	return false;
}