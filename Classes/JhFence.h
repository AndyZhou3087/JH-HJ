/********************************************************************
* 围墙
*********************************************************************/

#ifndef _FENCE_H_
#define _FENCE_H_
#include "JhBuilding.h"
#include "cocos2d.h"
USING_NS_CC;

class JhFence :public JhBuilding
{
public:
	JhFence();
	~JhFence();

public:

	virtual bool init();

	CREATE_FUNC(JhFence);
};
#endif

