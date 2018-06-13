#include "GameEvent.h"
#include "global.h"

USING_NS_CC;

EventListener * GameEvent::regEvent(EnumGameEvent event, std::function<void(EventCustom *)> callback) {

	EventListenerCustom* customEvt = NULL;
	switch (event) {
	case Event_Game_Start:
		customEvt = EventListenerCustom::create(s_event_game_start, callback);
		break;
	case Event_Level_Update:
		customEvt = EventListenerCustom::create(s_event_level_update, callback);
		break;
	case Event_Exit_Cancel:
		customEvt = EventListenerCustom::create(s_event_exit_cancel, callback);
		break;
	case Event_Game_Restart:
		customEvt = EventListenerCustom::create(s_event_game_restart, callback);
		break;
	case Event_Game_Over:
		customEvt = EventListenerCustom::create(s_event_game_over, callback);
		break;
	case Event_Game_Pause:
		customEvt = EventListenerCustom::create(s_event_game_pause, callback);
		break;
	case Event_Game_Ready:
		customEvt = EventListenerCustom::create(s_event_game_ready, callback);
		break;
	case Event_Game_Relive:
		customEvt = EventListenerCustom::create(s_event_game_relive, callback);
		break;
	case Event_Bomb_Change:
		customEvt = EventListenerCustom::create(s_event_bomb_change, callback);
		break;
	case Event_Score_Change:
		customEvt = EventListenerCustom::create(s_event_score_change, callback);
		break;
	case Event_Tool_Change:
		customEvt = EventListenerCustom::create(s_event_tool_change, callback);
		break;
	case Event_Release_Cross:
		customEvt = EventListenerCustom::create(s_event_realease_cross, callback);
		break;
	case Event_Energy_Change:
		customEvt = EventListenerCustom::create(s_event_energy_change, callback);
		break;
	case Event_Close_Intro:
		customEvt = EventListenerCustom::create(s_event_close_intro, callback);
		break;
	case Event_Level_Unlock:
		customEvt = EventListenerCustom::create(s_event_level_unlock, callback);
		break;
	case Event_Time_Change:
		customEvt = EventListenerCustom::create(s_event_time_change, callback);
		break;
	case Event_Game_Win:
		customEvt = EventListenerCustom::create(s_event_game_win, callback);
		break;
	case Event_Attrack_Boss:
		customEvt = EventListenerCustom::create(s_event_attrack_boss, callback);
		break;
	case Event_Game_Finish:
		customEvt = EventListenerCustom::create(s_event_game_finish, callback);
		break;
	default:
		return NULL;
		break;
	}

	//注册自定义事件（处理优先级为12）  
	Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(customEvt,12);
	return customEvt;
}

void GameEvent::unregEvent(EventListener * event) {
	Director::getInstance()->getEventDispatcher()->removeEventListener(event);
}

void GameEvent::throwEvent(EnumGameEvent event, void* userdata) {
	switch (event) {
	case Event_Game_Start:
		Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(s_event_game_start, userdata);
		break;
	case Event_Level_Update:
		Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(s_event_level_update, userdata);
		break;
	case Event_Exit_Cancel:
		Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(s_event_exit_cancel, userdata);
		break;
	case Event_Game_Restart:
		Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(s_event_game_restart, userdata);
		break;
	case Event_Game_Over:
		Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(s_event_game_over, userdata);
		break;
	case Event_Game_Pause:
		Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(s_event_game_pause, userdata);
		break;
	case Event_Game_Ready:
		Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(s_event_game_ready, userdata);
		break;
	case Event_Game_Relive:
		Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(s_event_game_relive, userdata);
		break;
	case Event_Bomb_Change:
		Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(s_event_bomb_change, userdata);
		break;
	case Event_Score_Change:
		Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(s_event_score_change, userdata);
		break;
	case Event_Tool_Change:
		Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(s_event_tool_change, userdata);
		break;
	case Event_Release_Cross:
		Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(s_event_realease_cross, userdata);
		break;
	case Event_Energy_Change:
		Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(s_event_energy_change, userdata);
		break;
	case Event_Close_Intro:
		Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(s_event_close_intro, userdata);
		break;
	case Event_Level_Unlock:
		Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(s_event_level_unlock, userdata);
		break;
	case Event_Time_Change:
		Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(s_event_time_change, userdata);
		break;
	case Event_Game_Win:
		Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(s_event_game_win, userdata);
		break;
	case Event_Attrack_Boss:
		Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(s_event_attrack_boss, userdata);
		break;
	case Event_Game_Finish:
		Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(s_event_game_finish, userdata);
		break;
	default:
		break;
	}
}