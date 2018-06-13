/********************************************************************
* 书架
*********************************************************************/

#ifndef _BOOKSHELF_H_
#define _BOOKSHELF_H_
#include "JhBuilding.h"
#include "cocos2d.h"
USING_NS_CC;

class JhBookShelf :public JhBuilding
{
public:
	JhBookShelf();
	~JhBookShelf();

public:

	virtual void action(int minute, int exminute);

	virtual bool init();

	CREATE_FUNC(JhBookShelf);
};
#endif

