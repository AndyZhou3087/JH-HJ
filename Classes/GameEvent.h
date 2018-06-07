#ifndef __TOWEREVENT_H__
#define __TOWEREVENT_H__
#include <cocos2d.h>
#include "global.h"

class GameEvent {
public:
	static cocos2d::EventListener * regEvent(EnumGameEvent event, std::function<void(cocos2d::EventCustom *)> callback);
	static void unregEvent(cocos2d::EventListener *event);
	static void throwEvent(EnumGameEvent event, void* userdata);
};

#endif
