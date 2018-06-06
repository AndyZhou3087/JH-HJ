#include "JhBookShelf.h"

#include "JhConst.h"


JhBookShelf::JhBookShelf()
{
}


JhBookShelf::~JhBookShelf()
{

}

bool JhBookShelf::init()
{
	if (Sprite::initWithSpriteFrameName("ui/bookshelf.png"))
	{

		return true;
	}
	return false;
}

void JhBookShelf::action(int minute, int exminute)
{

}