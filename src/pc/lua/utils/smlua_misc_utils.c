#include "sm64.h"
#include "types.h"

#include "data/dynos.c.h"
#include "game/bettercamera.h"
#include "game/camera.h"
#include "game/hardcoded.h"
#include "game/hud.h"
#include "pc/lua/smlua.h"
#include "smlua_misc_utils.h"
#include "pc/debuglog.h"
#include "pc/mods/mod.h"
#include "pc/mods/mods.h"
#include "pc/mods/mods_utils.h"
#include "pc/pc_main.h"
#include "game/object_list_processor.h"
#include "game/rendering_graph_node.h"
#include "game/level_update.h"
#include "pc/djui/djui_console.h"
#include "pc/djui/djui_hud_utils.h"
#include "game/skybox.h"
#include "pc/gfx/gfx_pc.h"
#include "include/course_table.h"
#include "game/level_geo.h"
#include "game/first_person_cam.h"

static struct DateTime sDateTime;

///

u32 get_network_area_timer(void) {
    return gNetworkAreaTimer;
}

///

s32* get_temp_s32_pointer(s32 initialValue) {
    static s32 value = 0;
    value = initialValue;
    return &value;
}

s32 deref_s32_pointer(s32* pointer) {
    if (pointer == NULL) {
        LOG_LUA_LINE("Tried to dereference null pointer!");
        return 0;
    }
    return *pointer;
}

///

void djui_popup_create_global(const char* message, int lines) {
    djui_popup_create(message, lines);
    network_send_global_popup(message, lines);
}

struct AllowDjuiPopupOverride {
    bool value;
    bool override;
};

struct AllowDjuiPopupOverride sAllowDjuiPopupOverride;

bool djui_is_popup_disabled(void) {
    // return override if there is one, otherwise return the value in the config.
    return sAllowDjuiPopupOverride.override ? sAllowDjuiPopupOverride.value : configDisablePopups;
}

void djui_set_popup_disabled_override(bool value) {
    // enable override
    sAllowDjuiPopupOverride.override = true;
    // set override to value specified in lua mod
    sAllowDjuiPopupOverride.value = value;
}

void djui_reset_popup_disabled_override(void) {
    // disable override
    sAllowDjuiPopupOverride.override = false;
}

///

extern s8 gDialogBoxState;
s8 get_dialog_box_state(void) {
    return gDialogBoxState;
}

///

extern u8 gLastCollectedStarOrKey;
s32 get_last_star_or_key(void) {
    return gLastCollectedStarOrKey;
}

void set_last_star_or_key(u8 value) {
    gLastCollectedStarOrKey = value;
}

extern u8 gLastCompletedCourseNum;
u8 get_last_completed_course_num(void) {
    return gLastCompletedCourseNum;
}

void set_last_completed_course_num(u8 courseNum) {
    gLastCompletedCourseNum = courseNum;
}

extern u8 gLastCompletedStarNum;
u8 get_last_completed_star_num(void) {
    return gLastCompletedStarNum;
}

void set_last_completed_star_num(u8 starNum) {
    gLastCompletedStarNum = starNum;
}

extern u8 gGotFileCoinHiScore;
bool get_got_file_coin_hi_score(void) {
    return gGotFileCoinHiScore;
}

void set_got_file_coin_hi_score(bool value) {
    gGotFileCoinHiScore = value ? TRUE : FALSE;
}

extern s8 gSaveFileModified;
bool get_save_file_modified(void) {
    return gSaveFileModified;
}

void set_save_file_modified(bool value) {
    gSaveFileModified = value ? TRUE : FALSE;
}

///

void hud_hide(void) {
    gOverrideHideHud = TRUE;
}

void hud_show(void) {
    gOverrideHideHud = FALSE;
}

bool hud_is_hidden(void) {
    return gOverrideHideHud;
}

s32 hud_get_value(enum HudDisplayValue type) {
    switch (type) {
        case HUD_DISPLAY_LIVES:         return gHudDisplay.lives;
        case HUD_DISPLAY_COINS:         return gHudDisplay.coins;
        case HUD_DISPLAY_STARS:         return gHudDisplay.stars;
        case HUD_DISPLAY_WEDGES:        return gHudDisplay.wedges;
        case HUD_DISPLAY_KEYS:          return gHudDisplay.keys;
        case HUD_DISPLAY_FLAGS:         return gHudDisplay.flags;
        case HUD_DISPLAY_TIMER:         return gHudDisplay.timer;
        case HUD_DISPLAY_CAMERA_STATUS: return get_hud_camera_status();
    }
    return 0;
}

void hud_set_value(enum HudDisplayValue type, s32 value) {
    switch (type) {
        case HUD_DISPLAY_LIVES:         gHudDisplay.lives  = value;   break;
        case HUD_DISPLAY_COINS:         gHudDisplay.coins  = value;   break;
        case HUD_DISPLAY_STARS:         gHudDisplay.stars  = value;   break;
        case HUD_DISPLAY_WEDGES:        gHudDisplay.wedges = value;   break;
        case HUD_DISPLAY_KEYS:          gHudDisplay.keys   = value;   break;
        case HUD_DISPLAY_FLAGS:         gHudDisplay.flags  = value;   break;
        case HUD_DISPLAY_TIMER:         gHudDisplay.timer  = value;   break;
        case HUD_DISPLAY_CAMERA_STATUS: set_hud_camera_status(value); break;
    }
}

extern const u8 texture_power_meter_left_side[];
extern const u8 texture_power_meter_right_side[];
extern const u8 texture_power_meter_full[];
extern const u8 texture_power_meter_seven_segments[];
extern const u8 texture_power_meter_six_segments[];
extern const u8 texture_power_meter_five_segments[];
extern const u8 texture_power_meter_four_segments[];
extern const u8 texture_power_meter_three_segments[];
extern const u8 texture_power_meter_two_segments[];
extern const u8 texture_power_meter_one_segments[];

void hud_render_power_meter(s32 health, f32 x, f32 y, f32 width, f32 height) {
    static struct TextureInfo sPowerMeterTexturesInfo[] = {
        { (u8*)texture_power_meter_left_side,      8, 32, 64, "texture_power_meter_left_side"      },
        { (u8*)texture_power_meter_right_side,     8, 32, 64, "texture_power_meter_right_side"     },
        { (u8*)texture_power_meter_one_segments,   8, 32, 32, "texture_power_meter_one_segments"   },
        { (u8*)texture_power_meter_two_segments,   8, 32, 32, "texture_power_meter_two_segments"   },
        { (u8*)texture_power_meter_three_segments, 8, 32, 32, "texture_power_meter_three_segments" },
        { (u8*)texture_power_meter_four_segments,  8, 32, 32, "texture_power_meter_four_segments"  },
        { (u8*)texture_power_meter_five_segments,  8, 32, 32, "texture_power_meter_five_segments"  },
        { (u8*)texture_power_meter_six_segments,   8, 32, 32, "texture_power_meter_six_segments"   },
        { (u8*)texture_power_meter_seven_segments, 8, 32, 32, "texture_power_meter_seven_segments" },
        { (u8*)texture_power_meter_full,           8, 32, 32, "texture_power_meter_full"           },
    };
    djui_hud_render_texture(&sPowerMeterTexturesInfo[0], x, y, width / 64, height / 64);
    djui_hud_render_texture(&sPowerMeterTexturesInfo[1], x + (width - 2) / 2, y, width / 64, height / 64);
    s32 numWedges = MIN(MAX(health >> 8, 0), 8);
    if (numWedges != 0) {
        djui_hud_render_texture(&sPowerMeterTexturesInfo[numWedges + 1], x + (width - 4) / 4, y + height / 4, width / 64,  height / 64);
    }
}

void hud_render_power_meter_interpolated(s32 health, f32 prevX, f32 prevY, f32 prevWidth, f32 prevHeight, f32 x, f32 y, f32 width, f32 height) {
    static struct TextureInfo sPowerMeterTexturesInfo[] = {
        { (u8*)texture_power_meter_left_side,      8, 32, 64, "texture_power_meter_left_side"      },
        { (u8*)texture_power_meter_right_side,     8, 32, 64, "texture_power_meter_right_side"     },
        { (u8*)texture_power_meter_one_segments,   8, 32, 32, "texture_power_meter_one_segments"   },
        { (u8*)texture_power_meter_two_segments,   8, 32, 32, "texture_power_meter_two_segments"   },
        { (u8*)texture_power_meter_three_segments, 8, 32, 32, "texture_power_meter_three_segments" },
        { (u8*)texture_power_meter_four_segments,  8, 32, 32, "texture_power_meter_four_segments"  },
        { (u8*)texture_power_meter_five_segments,  8, 32, 32, "texture_power_meter_five_segments"  },
        { (u8*)texture_power_meter_six_segments,   8, 32, 32, "texture_power_meter_six_segments"   },
        { (u8*)texture_power_meter_seven_segments, 8, 32, 32, "texture_power_meter_seven_segments" },
        { (u8*)texture_power_meter_full,           8, 32, 32, "texture_power_meter_full"           },
    };

    djui_hud_render_texture_interpolated(&sPowerMeterTexturesInfo[0],
        prevX, prevY, prevWidth / 64, prevHeight / 64,
        x,     y,     width     / 64, height     / 64);

    djui_hud_render_texture_interpolated(&sPowerMeterTexturesInfo[1],
        prevX + (prevWidth - 2) / 2, prevY, prevWidth / 64, prevHeight / 64,
        x     + (width - 2)     / 2, y,     width     / 64, height     / 64);

    s32 numWedges = MIN(MAX(health >> 8, 0), 8);
    if (numWedges != 0) {
        djui_hud_render_texture_interpolated(&sPowerMeterTexturesInfo[numWedges + 1],
            prevX + (prevWidth - 4) / 4, prevY + prevHeight / 4, prevWidth / 64, prevHeight / 64,
            x     + (width - 4)     / 4, y     + height     / 4, width     / 64, height     / 64);
    }
}

s8 hud_get_flash(void) {
    return gHudFlash;
}

void hud_set_flash(s8 value) {
    gHudFlash = value;
}

///

struct CameraOverride {
    unsigned int value;
    bool override;
};

struct CameraOverride sOverrideCameraXSens   = { 0 };
struct CameraOverride sOverrideCameraYSens   = { 0 };
struct CameraOverride sOverrideCameraAggr    = { 0 };
struct CameraOverride sOverrideCameraPan     = { 0 };
struct CameraOverride sOverrideCameraDegrade = { 0 };
struct CameraOverride sOverrideCameraInvertX = { 0 };
struct CameraOverride sOverrideCameraInvertY = { 0 };
struct CameraOverride sOverrideEnableCamera  = { 0 };
struct CameraOverride sOverrideCameraAnalog  = { 0 };
struct CameraOverride sOverrideCameraMouse   = { 0 };

void camera_reset_overrides(void) {
    sOverrideCameraXSens.override = false;
    sOverrideCameraYSens.override = false;
    sOverrideCameraAggr.override = false;
    sOverrideCameraPan.override = false;
    sOverrideCameraDegrade.override = false;
    sOverrideCameraInvertX.override = false;
    sOverrideCameraInvertY.override = false;
    sOverrideEnableCamera.override = false;
    sOverrideCameraAnalog.override = false;
    sOverrideCameraMouse.override = false;
}

void camera_freeze(void) {
    gOverrideFreezeCamera = TRUE;
}

void camera_unfreeze(void) {
    gOverrideFreezeCamera = FALSE;
}

bool camera_is_frozen(void) {
    return gOverrideFreezeCamera;
}

void camera_set_romhack_override(enum RomhackCameraOverride rco) {
    gOverrideRomhackCamera = rco;
}

void camera_romhack_allow_centering(u8 allow) {
    gRomhackCameraAllowCentering = allow;
}

void camera_allow_toxic_gas_camera(u8 allow) {
    gOverrideAllowToxicGasCamera = allow;
}

void camera_romhack_allow_dpad_usage(u8 allow) {
    gRomhackCameraAllowDpad = allow;
}

bool camera_config_is_free_cam_enabled(void) {
    return sOverrideEnableCamera.override ? sOverrideEnableCamera.value : configEnableCamera;
}

bool camera_config_is_analog_cam_enabled(void) {
    return sOverrideCameraAnalog.override ? sOverrideCameraAnalog.value : configCameraAnalog;
}

bool camera_config_is_mouse_look_enabled(void) {
    return sOverrideCameraMouse.override ? sOverrideCameraMouse.value : configCameraMouse;
}

bool camera_config_is_x_inverted(void) {
    return sOverrideCameraInvertX.override ? sOverrideCameraInvertX.value : configCameraInvertX;
}

bool camera_config_is_y_inverted(void) {
    return sOverrideCameraInvertY.override ? sOverrideCameraInvertY.value : configCameraInvertY;
}

u32 camera_config_get_x_sensitivity(void) {
    return sOverrideCameraXSens.override ? sOverrideCameraXSens.value : configCameraXSens;
}

u32 camera_config_get_y_sensitivity(void) {
    return sOverrideCameraYSens.override ? sOverrideCameraYSens.value : configCameraYSens;
}

u32 camera_config_get_aggression(void) {
    return sOverrideCameraAggr.override ? sOverrideCameraAggr.value : configCameraAggr;
}

u32 camera_config_get_pan_level(void) {
    return sOverrideCameraPan.override ? sOverrideCameraPan.value : configCameraPan;
}

u32 camera_config_get_deceleration(void) {
    return sOverrideCameraDegrade.override ? sOverrideCameraDegrade.value : configCameraDegrade;
}

void camera_config_enable_free_cam(bool enable) {
    sOverrideEnableCamera.value = enable;
    sOverrideEnableCamera.override = true;
    newcam_init_settings();
}

void camera_config_enable_analog_cam(bool enable) {
    sOverrideCameraAnalog.value = enable;
    sOverrideCameraAnalog.override = true;
    newcam_init_settings();
}

void camera_config_enable_mouse_look(bool enable) {
    sOverrideCameraMouse.value = enable;
    sOverrideCameraMouse.override = true;
    newcam_init_settings();
}

void camera_config_invert_x(bool invert) {
    sOverrideCameraInvertX.value = invert;
    sOverrideCameraInvertX.override = true;
    newcam_init_settings();
}

void camera_config_invert_y(bool invert) {
    sOverrideCameraInvertY.value = invert;
    sOverrideCameraInvertY.override = true;
    newcam_init_settings();
}

void camera_config_set_x_sensitivity(u32 value) {
    sOverrideCameraXSens.value = MIN(MAX(value, 1), 100);
    sOverrideCameraXSens.override = true;
    newcam_init_settings();
}

void camera_config_set_y_sensitivity(u32 value) {
    sOverrideCameraYSens.value = MIN(MAX(value, 1), 100);
    sOverrideCameraYSens.override = true;
    newcam_init_settings();
}

void camera_config_set_aggression(u32 value) {
    sOverrideCameraAggr.value = MIN(MAX(value, 0), 100);
    sOverrideCameraAggr.override = true;
    newcam_init_settings();
}

void camera_config_set_pan_level(u32 value) {
    sOverrideCameraPan.value = MIN(MAX(value, 0), 100);
    sOverrideCameraPan.override = true;
    newcam_init_settings();
}

void camera_config_set_deceleration(u32 value) {
    sOverrideCameraDegrade.value = MIN(MAX(value, 0), 100);
    sOverrideCameraDegrade.override = true;
    newcam_init_settings();
}

///

extern s16 gMenuMode;
bool is_game_paused(void) {
    return gMenuMode != -1;
}

///

bool is_transition_playing(void) {
    return sTransitionUpdate != NULL || gWarpTransition.isActive;
}

///

u32 allocate_mario_action(u32 actFlags) {
    u32 actGroup = ((actFlags & ACT_GROUP_MASK) >> 6);
    u32 actIndex = gLuaMarioActionIndex[actGroup]++;
    if (actIndex >= ACT_NUM_ACTIONS_PER_GROUP) {
        LOG_LUA("Cannot allocate more actions for group %u", actGroup);
        return 0;
    }
    return (actFlags & ~ACT_INDEX_MASK) | ACT_FLAG_CUSTOM_ACTION | actIndex;
}

///

f32 get_hand_foot_pos_x(struct MarioState* m, u8 index) {
    if (!m) { return 0; }
    if (index >= 4) { index = 0; }
    return m->marioBodyState->handFootPos[index][0];
}

f32 get_hand_foot_pos_y(struct MarioState* m, u8 index) {
    if (!m) { return 0; }
    if (index >= 4) { index = 0; }
    return m->marioBodyState->handFootPos[index][1];
}

f32 get_hand_foot_pos_z(struct MarioState* m, u8 index) {
    if (!m) { return 0; }
    if (index >= 4) { index = 0; }
    return m->marioBodyState->handFootPos[index][2];
}

///

s16 get_current_save_file_num(void) {
    extern s16 gCurrSaveFileNum;
    return gCurrSaveFileNum;
}

extern u8 gSaveFileUsingBackupSlot;
bool save_file_get_using_backup_slot(void) {
    return gSaveFileUsingBackupSlot;
}

void save_file_set_using_backup_slot(bool usingBackupSlot) {
    gSaveFileUsingBackupSlot = usingBackupSlot ? TRUE : FALSE;
}

///

void movtexqc_register(const char* name, s16 level, s16 area, s16 type) {
    dynos_movtexqc_register(name, level, area, type);
}

///

f32 get_environment_region(u8 index) {
    s32 idx = 6 * index;
    if (gEnvironmentRegions != NULL && index > 0 && index <= gEnvironmentRegions[0] && gEnvironmentRegionsLength > idx) {
        return gEnvironmentRegions[idx];
    }
    return gLevelValues.floorLowerLimit;
}

void set_environment_region(u8 index, s32 value) {
    s32 idx = 6 * index;
    if (gEnvironmentRegions != NULL && index > 0 && index <= gEnvironmentRegions[0] && gEnvironmentRegionsLength > idx) {
        gEnvironmentRegions[idx] = value;
    }
}

///

void set_override_fov(f32 fov) {
    gOverrideFOV = fov;
}

///

void set_override_near(f32 near) {
    gOverrideNear = near;
}

///

void set_override_far(f32 far) {
    gOverrideFar = far;
}

///

void add_scroll_target(u32 index, const char* name, u32 offset, u32 size) {
    dynos_add_scroll_target(index, name, offset, size);
}

///

f32 get_lighting_dir(u8 index) {
    if (index > 2) { return 0; }
    return gLightingDir[index];
}

void set_lighting_dir(u8 index, f32 value) {
    if (index > 2) { return; }
    gLightingDir[index] = value;
}

u8 get_lighting_color(u8 index) {
    if (index > 2) { return 0; }
    return gLightingColor[index];
}

void set_lighting_color(u8 index, u8 value) {
    if (index > 2) { return; }
    gLightingColor[index] = value;
}

///

u8 get_vertex_color(u8 index) {
    if (index > 2) { return 0; }
    return gVertexColor[index];
}

void set_vertex_color(u8 index, u8 value) {
    if (index > 2) { return; }
    gVertexColor[index] = value;
}

///

u8 get_fog_color(u8 index) {
    if (index > 2) { return 0; }
    return gFogColor[index];
}

void set_fog_color(u8 index, u8 value) {
    if (index > 2) { return; }
    gFogColor[index] = value;
}

f32 get_fog_intensity(void) {
    return gFogIntensity;
}

void set_fog_intensity(f32 intensity) {
    gFogIntensity = intensity;
}

///

s8 get_skybox(void) {
    return gReadOnlyBackground;
}

void set_override_skybox(s8 background) {
    gOverrideBackground = background;
}

///

bool course_is_main_course(u16 courseNum) {
    return COURSE_IS_MAIN_COURSE(courseNum);
}

///

s16 get_ttc_speed_setting(void) {
    return gTTCSpeedSetting;
}

void set_ttc_speed_setting(s16 speed) {
    gTTCSpeedSetting = speed;
}

///

s64 get_time(void) {
    return time(NULL);
}

struct DateTime* get_date_and_time(void) {
    time_t currentTime;
    time(&currentTime);
    struct tm *lt = localtime(&currentTime);

    sDateTime.year = lt->tm_year;
    sDateTime.month = lt->tm_mon;
    sDateTime.day = lt->tm_mday;
    sDateTime.hour = lt->tm_hour;
    sDateTime.minute = lt->tm_min;
    sDateTime.second = lt->tm_sec;
    return &sDateTime;
}

///

u16 get_envfx(void) {
    return gReadOnlyEnvFx;
}

void set_override_envfx(s32 envfx) {
    gOverrideEnvFx = envfx;
}

///

u32 get_global_timer(void) {
    return gGlobalTimer;
}

///

bool get_coop_compatibility_enabled(void) {
    return configCoopCompatibility;
}

///

void set_window_title(const char* title) {
    WAPI.set_window_title(title);
}

void reset_window_title(void) {
    WAPI.reset_window_title();
}

///

const char* get_os_name(void) {
#if defined(_WIN32) || defined(_WIN64)
    return "Windows";
#elif __APPLE__ || __MACH__
    return "Mac OSX"; // should be macOS
#elif __linux__
    return "Linux";
#elif __FreeBSD__
    return "FreeBSD";
#elif __unix || __unix__
    return "Unix";
#else
    return "Unknown";
#endif
}
