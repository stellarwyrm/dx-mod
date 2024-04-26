#ifndef SMLUA_MISC_UTILS_H
#define SMLUA_MISC_UTILS_H

#include "dialog_ids.h"
#include "game/camera.h"

enum HudDisplayValue {
    HUD_DISPLAY_LIVES,
    HUD_DISPLAY_COINS,
    HUD_DISPLAY_STARS,
    HUD_DISPLAY_WEDGES,
    HUD_DISPLAY_KEYS,
    HUD_DISPLAY_FLAGS,
    HUD_DISPLAY_TIMER,
    HUD_DISPLAY_CAMERA_STATUS
};

enum HudDisplayFlags {
    HUD_DISPLAY_FLAGS_NONE = 0x0000,
    HUD_DISPLAY_FLAGS_LIVES = 0x0001,
    HUD_DISPLAY_FLAGS_COIN_COUNT = 0x0002,
    HUD_DISPLAY_FLAGS_STAR_COUNT = 0x0004,
    HUD_DISPLAY_FLAGS_CAMERA_AND_POWER = 0x0008,
    HUD_DISPLAY_FLAGS_KEYS = 0x0010,
    HUD_DISPLAY_FLAGS_UNKNOWN_0020 = 0x0020,
    HUD_DISPLAY_FLAGS_TIMER = 0x0040,
    HUD_DISPLAY_FLAGS_CAMERA = 0x0080,
    HUD_DISPLAY_FLAGS_POWER = 0x0100,
    HUD_DISPLAY_FLAGS_EMPHASIZE_POWER = 0x8000
};

struct DateTime {
    s32 year;
    s32 month;
    s32 day;
    s32 hour;
    s32 minute;
    s32 second;
};

u32 get_network_area_timer(void);

s32* get_temp_s32_pointer(s32 initialValue);
s32 deref_s32_pointer(s32* pointer);

void djui_popup_create_global(const char* message, int lines);
bool djui_is_popup_disabled(void);
void djui_set_popup_disabled_override(bool value);
void djui_reset_popup_disabled_override(void);

s8 get_dialog_box_state(void);
s16 get_dialog_id(void);

s32 get_last_star_or_key(void);
void set_last_star_or_key(u8 value);
u8 get_last_completed_course_num(void);
void set_last_completed_course_num(u8 courseNum);
u8 get_last_completed_star_num(void);
void set_last_completed_star_num(u8 starNum);
bool get_got_file_coin_hi_score(void);
void set_got_file_coin_hi_score(bool value);
bool get_save_file_modified(void);
void set_save_file_modified(bool value);

void hud_hide(void);
void hud_show(void);
bool hud_is_hidden(void);
s32  hud_get_value(enum HudDisplayValue type);
void hud_set_value(enum HudDisplayValue type, s32 value);
void hud_render_power_meter(s32 health, f32 x, f32 y, f32 width, f32 height);
void hud_render_power_meter_interpolated(s32 health, f32 prevX, f32 prevY, f32 prevWidth, f32 prevHeight, f32 x, f32 y, f32 width, f32 height);
s8 hud_get_flash(void);
void hud_set_flash(s8 value);

void camera_reset_overrides(void);
void camera_freeze(void);
void camera_unfreeze(void);
bool camera_is_frozen(void);
void camera_set_romhack_override(enum RomhackCameraOverride rco);
void camera_romhack_allow_centering(u8 allow);
void camera_allow_toxic_gas_camera(u8 allow);
void camera_romhack_allow_dpad_usage(u8 allow);

bool camera_config_is_free_cam_enabled(void);
bool camera_config_is_analog_cam_enabled(void);
bool camera_config_is_mouse_look_enabled(void);
bool camera_config_is_x_inverted(void);
bool camera_config_is_y_inverted(void);
u32  camera_config_get_x_sensitivity(void);
u32  camera_config_get_y_sensitivity(void);
u32  camera_config_get_aggression(void);
u32  camera_config_get_pan_level(void);
u32  camera_config_get_deceleration(void);

void camera_config_enable_free_cam(bool enable);
void camera_config_enable_analog_cam(bool enable);
void camera_config_enable_mouse_look(bool enable);
void camera_config_invert_x(bool invert);
void camera_config_invert_y(bool invert);
void camera_config_set_x_sensitivity(u32 value);
void camera_config_set_y_sensitivity(u32 value);
void camera_config_set_aggression(u32 value);
void camera_config_set_pan_level(u32 value);
void camera_config_set_deceleration(u32 value);

bool is_game_paused(void);
bool is_transition_playing(void);

u32 allocate_mario_action(u32 actFlags);

f32 get_hand_foot_pos_x(struct MarioState* m, u8 index);
f32 get_hand_foot_pos_y(struct MarioState* m, u8 index);
f32 get_hand_foot_pos_z(struct MarioState* m, u8 index);

s16 get_current_save_file_num(void);
bool save_file_get_using_backup_slot(void);
void save_file_set_using_backup_slot(bool usingBackupSlot);

void movtexqc_register(const char* name, s16 level, s16 area, s16 type);
f32 get_environment_region(u8 index);
void set_environment_region(u8 index, s32 value);

void set_override_fov(f32 fov);
void set_override_near(f32 near);
void set_override_far(f32 far);

void add_scroll_target(u32 index, const char* name, u32 offset, u32 size);

f32 get_lighting_dir(u8 index);
void set_lighting_dir(u8 index, f32 value);

u8 get_lighting_color(u8 index);
void set_lighting_color(u8 index, u8 value);

u8 get_vertex_color(u8 index);
void set_vertex_color(u8 index, u8 value);

u8 get_fog_color(u8 index);
void set_fog_color(u8 index, u8 value);
f32 get_fog_intensity(void);
void set_fog_intensity(f32 intensity);

s8 get_skybox(void);
void set_override_skybox(s8 background);

void play_transition(s16 transType, s16 time, u8 red, u8 green, u8 blue);

bool course_is_main_course(u16 courseNum);

s16 get_ttc_speed_setting(void);
void set_ttc_speed_setting(s16 speed);

s64 get_time(void);
struct DateTime* get_date_and_time(void);

u16 get_envfx(void);
void set_override_envfx(s32 envfx);

u32 get_global_timer(void);

bool get_coop_compatibility_enabled(void);

void set_window_title(const char* title);
void reset_window_title(void);

const char* get_os_name(void);

#endif
