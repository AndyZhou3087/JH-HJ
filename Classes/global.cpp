#include "cocos2d.h"
#include "global.h"

//test
const bool s_game_test_flag = false;

//ani
const int s_ani_clear_n		= 6;
const int s_ani_clear2_n	= 4;
const int s_ani_ice_n		= 10;
const int s_ani_change_n	= 16;
const int s_ani_cross_n		= 4;
const int s_ani_ring_n		= 3;
const int s_ani_ring2_n		= 3;
const int s_ani_touch_n		= 2;
const int s_ani_frame_n		= 6;
const int s_ani_bomb_w_n	= 5;
const int s_ani_bomb_h_n	= 4;
const int s_ani_bossbomb_n	= 5;
const int s_ani_cross2_n	= 6;
const int s_ani_starbomb_n	= 5;

const int s_time_add = 15;
const int s_score_pet = 5;
const int s_1star_energy_add	= 1;
const int s_2star_energy_add	= 3;
const int s_3star_energy_add	= 5;

const int s_energy_use				= 5;//每次游戏 需要消耗的点数
const int s_energy_recover			= 5;//间隔恢复点数
const int s_energy_count_down		= 300;//体力恢复间隔秒数
const int s_energy_recover_limit	= 25;//体力恢复上限

const int s_time_count_down_tip	= 5;//5秒倒计时提示


//自定义事件标识
const char *s_event_level_update	= "EVENT_FLOOR_DOWN";
const char *s_event_exit_cancel		= "EVENT_FLOOR_UP";
const char *s_event_game_start		= "EVENT_GAME_START";
const char *s_event_game_restart	= "EVENT_GAME_RESTART";
const char *s_event_game_over		= "EVENT_GAME_OVER";
const char *s_event_game_pause		= "EVENT_GAME_PAUSE";
const char *s_event_game_ready		= "EVENT_GAME_READY";
const char *s_event_game_resume		= "EVENT_GAME_RESUME";
const char *s_event_game_relive		= "EVENT_GAME_RELIVE";
const char *s_event_bomb_change		= "EVENT_BOMB_CHANGE";
const char *s_event_score_change	= "EVENT_SCORE_CHANGE";
const char *s_event_tool_change		= "EVENT_TOOL_CHANGE";
const char *s_event_realease_cross	= "EVENT_REALEASE_CROSS";
const char *s_event_energy_change	= "EVENT_ENERGY_CHANGE";
const char *s_event_close_intro		= "EVENT_CLOSE_INTRO";
const char *s_event_level_unlock	= "EVENT_LEVEL_UNLOCK";
const char *s_event_time_change		= "EVENT_TIME_CHANGE";
const char *s_event_game_win		= "EVENT_GAME_WIN";
const char *s_event_attrack_boss	= "EVENT_ATTRACK_BOSS";
const char *s_event_game_finish		= "EVENT_GAME_FINISH";