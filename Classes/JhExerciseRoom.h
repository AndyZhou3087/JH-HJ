/********************************************************************
* 练功房
*********************************************************************/

#ifndef _EXERCISE_ROOM_H_
#define _EXERCISE_ROOM_H_
#include "JhBuilding.h"
#include "cocos2d.h"
USING_NS_CC;

class JhExerciseRoom :public JhBuilding
{
public:
	JhExerciseRoom();
	~JhExerciseRoom();

public:
	/****************************
	闭关操作
	@param minute 
	@param exminute 消耗真实时间
	*****************************/
	virtual void action(int minute, int exminute);

	virtual bool init();

	CREATE_FUNC(JhExerciseRoom);
};
#endif

