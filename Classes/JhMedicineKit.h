
/********************************************************************
*	 药箱
*********************************************************************/

#ifndef _MEDICINEKIT_H_
#define _MEDICINEKIT_H_
#include "JhBuilding.h"
#include "cocos2d.h"
USING_NS_CC;

class JhMedicineKit :public JhBuilding
{
public:
	JhMedicineKit();
	~JhMedicineKit();

public:
	/****************************
	制药操作
	@param minute （进度条时间）
	@param exminute 消耗游戏时间
	*****************************/
	virtual void action(int minute, int exminute);

	virtual bool init();

	CREATE_FUNC(JhMedicineKit);
};
#endif

