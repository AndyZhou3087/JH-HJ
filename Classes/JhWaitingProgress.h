

#ifndef __WAITINGPROGRESS__
#define __WAITINGPROGRESS__

#include "cocos2d.h"
USING_NS_CC;


class JhWaitingProgress : public Layer
{
public:
    bool init(std::string text);
	static JhWaitingProgress* create(std::string content);
	Sprite* boxbg;
private:
	std::string textstr;
	void delayShowText(float dt);
};

#endif
