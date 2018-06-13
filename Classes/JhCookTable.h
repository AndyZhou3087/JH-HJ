
/********************************************************************
*	 灶台
*********************************************************************/

#ifndef _COOKTABLE_H_
#define _COOKTABLE_H_
#include "JhBuilding.h"
#include "cocos2d.h"
USING_NS_CC;

class JhCookTable :public JhBuilding
{
public:
	JhCookTable();
	~JhCookTable();

public:

	/****************************
	制作食物操作
	@param minute （进度条时间）
	@param exminute 消耗游戏时间
	*****************************/
	virtual void action(int minute, int exminute);

	virtual bool init();

	CREATE_FUNC(JhCookTable);
};
#endif

