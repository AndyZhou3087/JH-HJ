#ifndef __global_h__
#define __global_h__

#define WIN_WIDTH  (cocos2d::CCDirector::sharedDirector()->getWinSize().width)
#define WIN_HEIGHT  (cocos2d::CCDirector::sharedDirector()->getWinSize().height)

extern const bool s_game_test_flag ;//= true; 

//ani
extern const int s_ani_clear_n		;//= 6;
extern const int s_ani_clear2_n		;//= 4;
extern const int s_ani_ice_n		;//= 10;
extern const int s_ani_change_n		;//= 16;
extern const int s_ani_cross_n		;//= 4;
extern const int s_ani_ring_n		;//=5;
extern const int s_ani_ring2_n		;//=5;
extern const int s_ani_touch_n		;//= 2;
extern const int s_ani_frame_n		;//= 6;
extern const int s_ani_bomb_w_n		;//= 5;
extern const int s_ani_bomb_h_n		;//= 4;
extern const int s_ani_bossbomb_n	;//= 5;
extern const int s_ani_cross2_n		;//=5;
extern const int s_ani_starbomb_n;

extern const int s_time_add				;//= 15;
extern const int s_score_pet			;//5
extern const int s_1star_energy_add		;//= 1;
extern const int s_2star_energy_add		;//= 3;
extern const int s_3star_energy_add		;//= 5;
extern const int s_energy_use			;//= 5;
extern const int s_energy_recover		;//= 5;//间隔恢复点数
extern const int s_energy_count_down	;//= 300;
extern const int s_time_count_down_tip	;//= 5;//5秒倒计时提示
extern const int s_energy_recover_limit	;//= 25;//体力恢复上限

enum EnumGameEvent {
	Event_Level_Update,
	Event_Exit_Cancel,
	Event_Game_Start,
	Event_Game_Over,
	Event_Game_Pause,
	Event_Game_Resume,
	Event_Game_Restart,
	Event_Game_Ready,
	Event_Game_Relive,
	Event_Game_Finish,
	Event_Bomb_Change,
	Event_Score_Change,
	Event_Tool_Change,
	Event_Release_Cross,
	Event_Energy_Change,
	Event_Close_Intro,
	Event_Level_Unlock,
	Event_Time_Change,
	Event_Game_Win,
	Event_Attrack_Boss,
};

enum EnumGameTool {
	Tool_Cross,
	Tool_Time,
	Tool_Tip,
	Tool_Bomb,
	Tool_Energy,
};


//自定义事件标识
extern const char *s_event_level_update		;//= "EVENT_FLOOR_DOWN";
extern const char *s_event_exit_cancel		;//= "EVENT_FLOOR_UP";
extern const char *s_event_game_start		;//= "EVENT_GAME_START";
extern const char *s_event_game_restart		;//= "EVENT_GAME_RESTART";
extern const char *s_event_game_over		;//= "EVENT_GAME_OVER";
extern const char *s_event_game_pause		;//= "EVENT_GAME_PAUSE";
extern const char *s_event_game_resume		;//= "EVENT_GAME_PAUSE";
extern const char *s_event_game_ready		;
extern const char *s_event_game_relive		;
extern const char *s_event_bomb_change		;//= "EVENT_BOMB_CHANGE";
extern const char *s_event_score_change		;
extern const char *s_event_tool_change		;
extern const char *s_event_realease_cross	;//= "EVENT_REALEASE_CROSS";
extern const char *s_event_energy_change	;//= "EVENT_ENERGY_CHANGE";
extern const char *s_event_close_intro		;
extern const char *s_event_level_unlock		;
extern const char *s_event_time_change		;
extern const char *s_event_game_win			;
extern const char *s_event_attrack_boss		;//= "EVENT_ATTRACK_BOSS";
extern const char *s_event_game_finish		;

#endif