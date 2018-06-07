#include "cocos2d.h"

//logo
const char * s_unit_logo		= "unit_logo.png";

//bg
const char * s_bg_exit		= "bg_grey.png";
const char * s_bg_home		= "bg_home.jpg";
const char * s_bg_game		= "bg_game.jpg";
const char * s_bg_grey		= "bg_grey.png";
const char * s_bg_ground	= "bg_ground.png";
const char * s_bg_tree		= "bg_tree.png";
const char * s_bg_panel_txt			= "public/bg_panel_txt.png";
const char * s_bg_panel_txt_arrow	= "public/bg_panel_txt_arrow.png";

//cache-list
const char * s_plist_all_image_public	= "all_image_public.plist";
const char * s_plist_ani_change			= "ani_change.plist";
const char * s_plist_ani_ice			= "ani_ice.plist";

//texture
const char * s_all_image_public = "all_image_public.png";

//particle
const char * s_particle_plist_bg		= "colorful.plist";
const char * s_particle_plist_boss_bomb	= "bossBomb.plist";
const char * s_particle_frag_green		= "public/fragment_green.png";
const char * s_particle_frag_red		= "public/fragment_red.png";
const char * s_particle_frag_blue		= "public/fragment_blue.png";
const char * s_particle_frag_yellow		= "public/fragment_yellow.png";
const char * s_particle_star			= "public/particle_star.png";

//file
const char * s_file_level_data		= "level.xml";
const char * s_file_setting_data	= "setting.json";

//font
const char * s_font_exit = "Arial";

//ani
const char * s_ani_clear	= "ani_clear.png";
const char * s_ani_clear2	= "ani_clear2.png";
const char * s_ani_ice		= "ani_ice.png";
const char * s_ani_change	= "ani_change.png";
const char * s_ani_cross	= "ani_cross.png";
const char * s_ani_ring		= "ani_ring.png";
const char * s_ani_ring2	= "ani_ring2.png";
const char * s_ani_touch	= "ani_touch.png";
const char * s_ani_bomb		= "ani_bomb.png";
const char * s_ani_frame	= "ani_frame.png";
const char * s_ani_bossbomb	= "ani_bossbomb.png";
const char * s_ani_cross2	= "ani_cross2.png";
const char * s_ani_star_bomb	= "ani_star_bomb.png";

//pet
const char * s_pet_white	= "public/pet_white.png";
const char * s_pet_green	= "public/pet_green.png";
const char * s_pet_yellow	= "public/pet_yellow.png";
const char * s_pet_red		= "public/pet_red.png";
const char * s_pet_purple	= "public/pet_purple.png";
const char * s_pet_blue		= "public/pet_blue.png";

const char * s_pet_white_s		= "public/pet_white_s.png";
const char * s_pet_green_s		= "public/pet_green_s.png";
const char * s_pet_yellow_s		= "public/pet_yellow_s.png";
const char * s_pet_red_s		= "public/pet_red_s.png";
const char * s_pet_purple_s		= "public/pet_purple_s.png";
const char * s_pet_blue_s		= "public/pet_blue_s.png";

const char * s_eye_white_o		= "public/pet_white_eye_o.png";
const char * s_eye_white_c		= "public/pet_white_eye_c.png";
const char * s_eye_green_o		= "public/pet_green_eye_o.png";
const char * s_eye_green_c		= "public/pet_green_eye_c.png";
const char * s_eye_yellow_o		= "public/pet_yellow_eye_o.png";
const char * s_eye_yellow_c		= "public/pet_yellow_eye_c.png";
const char * s_eye_red_o		= "public/pet_red_eye_o.png";
const char * s_eye_red_c		= "public/pet_red_eye_c.png";
const char * s_eye_purple_o		= "public/pet_purple_eye_o.png";
const char * s_eye_purple_c		= "public/pet_purple_eye_c.png";
const char * s_eye_blue_o		= "public/pet_blue_eye_o.png";
const char * s_eye_blue_c		= "public/pet_blue_eye_c.png";


const char * s_special_clear		= "public/special_clear.png";
const char * s_special_shade		= "public/special_shade.png";
const char * s_special_face			= "public/special_face.png";
const char * s_special_ice_closed	= "public/special_ice_closed.png";
const char * s_special_ice_closing	= "public/special_ice_closing.png";
const char * s_special_everyok		= "public/special_everyok.png";

const char * s_tool_cross	= "public/effect_cross_normal.png";
const char * s_tool_time	= "public/effect_time_normal.png";
const char * s_tool_tip		= "public/effect_tip_normal.png";
const char * s_tool_bomb	= "public/effect_bomb_normal.png";
const char * s_tool_energy	= "public/icon_energy.png";

const char * s_icon_star			= "public/icon_star.png";
const char * s_icon_star_yellow		= "public/icon_star_yellow.png";
const char * s_icon_star_blue		= "public/icon_star_blue.png";
const char * s_icon_ball_red		= "public/ball_red.png";
const char * s_icon_ball_yellow		= "public/ball_yellow.png";
const char * s_mark_yes				= "public/mark_yes.png";
const char * s_mark_no				= "public/mark_no.png";
const char * s_icon_arrow			= "public/icon_arrow.png";
const char * s_icon_arrow2			= "public/icon_arrow2.png";
const char * s_icon_award_cross		= "public/icon_award_cross.png";
const char * s_icon_award_gift		= "public/icon_award_gift.png";
const char * s_icon_award_time1		= "public/icon_award_time1.png";
const char * s_icon_award_time3		= "public/icon_award_time3.png";
const char * s_icon_touch_error		= "public/icon_touch_error.png";
const char * s_icon_lock			= "public/icon_lock.png";
const char * s_icon_loading			= "public/loading.png";
const char * s_icon_loading_pot		= "public/bg_ball_blue.png";
const char * s_icon_bullet			= "public/icon_bullet.png";
const char * s_icon_girl2			= "public/girl2.png";
const char * s_icon_energy_small	= "public/icon_energy_small.png";

const char * s_label_go				= "public/label_go.png";
const char * s_label_ready			= "public/label_ready.png";
const char * s_btn_boss_normal		= "public/btn_boss_normal.png";
const char * s_btn_boss_press		= "public/btn_boss_press.png";
const char * s_btn_boss_disable		= "public/btn_boss_disable.png";

const char * s_tip_ring		= "public/tip_ring.png";
const char * s_tip_light	= "public/tip_light.png";
const char * s_tip_red		= "public/red_tip.png";
const char * s_tip_cross	= "public/label_cross.png";
const char * s_tip_time		= "public/label_time.png";
const char * s_tip_tip		= "public/label_tip.png";
const char * s_tip_bomb		= "public/label_bomb.png";
const char * s_tip_add_5	= "public/label_add_5.png";
const char * s_tip_add_6	= "public/label_add_6.png";
const char * s_tip_add_7	= "public/label_add_7.png";
const char * s_tip_add_8	= "public/label_add_8.png";
const char * s_tip_click	= "public/label_click.png";
const char * s_tip_minus_5	= "public/label_minus_5.png";


//mask
const char * s_mask_ground	= "public/mask_ground.png";


//music
const char * s_music_home_bg			= "bg_home.mp3";
const char * s_music_game_bg			= "bg_game.mp3";
const char * s_music_effect_win			= "win.ogg";
const char * s_music_effect_timeup		= "timeup.ogg";
const char * s_music_effect_ready		= "ready.mp3";
const char * s_music_effect_dida		= "dida.mp3";
const char * s_music_effect_toucherror	= "toucherror.mp3";
const char * s_music_effect_buy			= "buy.mp3";
const char * s_music_effect_click		= "click.mp3";

const char * s_music_effect_clear2		= "clear2.ogg";
const char * s_music_effect_clear3		= "clear3.ogg";
const char * s_music_effect_clear4		= "clear4.ogg";
const char * s_music_effect_combo1		= "combo1.mp3";
const char * s_music_effect_combo2		= "combo2.mp3";
const char * s_music_effect_combo3		= "combo3.mp3";
const char * s_music_effect_combo4		= "combo4.mp3";
const char * s_music_effect_combo5		= "combo5.mp3";

const char * s_music_effect_zan3		= "zan3.mp3";
const char * s_music_effect_zan4		= "zan4.mp3";
const char * s_music_effect_zan5		= "zan5.mp3";
const char * s_music_effect_zan6		= "zan6.mp3";

const char * s_music_effect_impact		= "impact.mp3";
const char * s_music_effect_cross		= "cross.ogg";
const char * s_music_effect_bomb		= "bomb.mp3";
const char * s_music_effect_fail		= "fail.ogg";
const char * s_music_effect_welcome		= "welcome.mp3";
const char * s_music_effect_star		= "star.ogg";
const char * s_music_effect_bigaward	= "bigaward.ogg";
const char * s_music_effect_break		= "break.mp3";
const char * s_music_effect_lucky		= "lucky.mp3";
const char * s_music_effect_fall		= "fall.mp3";
const char * s_music_effect_fall2		= "fall2.wav";
const char * s_music_effect_fall3		= "fall3.wav";

const char * s_music_effect_boss_bomb	= "boss_bomb.mp3";
const char * s_music_effect_boss_hurt	= "boss_hurt.mp3";
const char * s_music_effect_boss_show	= "boss_show.mp3";
const char * s_music_effect_bullet		= "bullet.mp3";
