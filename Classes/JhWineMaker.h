
/********************************************************************
*	 酿酒器
*********************************************************************/

#ifndef _WineMaker_H_
#define _WineMaker_H_
#include "JhBuilding.h"
#include "cocos2d.h"
USING_NS_CC;

class JhWineMaker :public JhBuilding
{
public:
	JhWineMaker();
	~JhWineMaker();

public:
	/****************************
	酿酒操作
	@param minute （进度条时间）
	@param exminute 消耗游戏时间
	*****************************/
	virtual void action(int minute, int exminute);

	virtual bool init();

	CREATE_FUNC(JhWineMaker);
};
#endif

