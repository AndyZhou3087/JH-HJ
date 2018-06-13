
/********************************************************************
*	 锻造台
*********************************************************************/

#ifndef _FORGINGTABLE_H_
#define _FORGINGTABLE_H_
#include "JhBuilding.h"
#include "cocos2d.h"
USING_NS_CC;

class JhForgingtable :public JhBuilding
{
public:
	JhForgingtable();
	~JhForgingtable();

public:

	/****************************
	锻造操作
	@param minute （进度条时间）
	@param exminute 消耗游戏时间
	*****************************/
	virtual void action(int minute, int exminute);

	virtual bool init();

	CREATE_FUNC(JhForgingtable);
};
#endif

