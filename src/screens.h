#ifndef EEZ_LVGL_UI_SCREENS_H
#define EEZ_LVGL_UI_SCREENS_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _objects_t {
    lv_obj_t *main;
    lv_obj_t *wifi_setting_screen;
    lv_obj_t *monitor_screen;
    lv_obj_t *obj0;
    lv_obj_t *obj1;
    lv_obj_t *date_timer;
    lv_obj_t *time_timer;
    lv_obj_t *wifi_setting_button;
    lv_obj_t *monitor_button;
    lv_obj_t *obj2;
    lv_obj_t *obj3;
    lv_obj_t *obj4;
    lv_obj_t *obj5;
    lv_obj_t *obj6;
    lv_obj_t *connect_button;
    lv_obj_t *back_wifi_button;
    lv_obj_t *wifi_id_text;
    lv_obj_t *password_text;
    lv_obj_t *status_text;
    lv_obj_t *obj7;
    lv_obj_t *obj8;
    lv_obj_t *serial_number_list_dropdowm;
    lv_obj_t *send_button;
    lv_obj_t *back_monitor_button;
    lv_obj_t *dropdown_value;
} objects_t;

extern objects_t objects;

enum ScreensEnum {
    SCREEN_ID_MAIN = 1,
    SCREEN_ID_WIFI_SETTING_SCREEN = 2,
    SCREEN_ID_MONITOR_SCREEN = 3,
};

void create_screen_main();
void tick_screen_main();

void create_screen_wifi_setting_screen();
void tick_screen_wifi_setting_screen();

void create_screen_monitor_screen();
void tick_screen_monitor_screen();

void tick_screen_by_id(enum ScreensEnum screenId);
void tick_screen(int screen_index);

void create_screens();


#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_SCREENS_H*/