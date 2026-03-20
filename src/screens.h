#ifndef EEZ_LVGL_UI_SCREENS_H
#define EEZ_LVGL_UI_SCREENS_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _objects_t {
    lv_obj_t *main_screen;
    lv_obj_t *wifi_setting_screen;
    lv_obj_t *devices;
    lv_obj_t *monitoring;
    lv_obj_t *obj0;
    lv_obj_t *obj1;
    lv_obj_t *date_main_label;
    lv_obj_t *time_main_label;
    lv_obj_t *button_main_wifi_setting;
    lv_obj_t *button_main_devices_setting;
    lv_obj_t *button_main_monitor;
    lv_obj_t *button_main_control;
    lv_obj_t *obj2;
    lv_obj_t *wifi_id_text;
    lv_obj_t *connect_button_wifi;
    lv_obj_t *back_button_wifi;
    lv_obj_t *wifi_password_text;
    lv_obj_t *wifi_status_text;
    lv_obj_t *obj3;
    lv_obj_t *connect_button_devices;
    lv_obj_t *back_button_devices;
    lv_obj_t *control_button_devices;
    lv_obj_t *mqtt_status_text;
    lv_obj_t *list_serial_number;
    lv_obj_t *obj4;
    lv_obj_t *text_serial_number_control;
    lv_obj_t *text_status_control;
    lv_obj_t *back_button_control;
    lv_obj_t *text_command_control;
    lv_obj_t *send_button_control;
} objects_t;

extern objects_t objects;

enum ScreensEnum {
    SCREEN_ID_MAIN_SCREEN = 1,
    SCREEN_ID_WIFI_SETTING_SCREEN = 2,
    SCREEN_ID_DEVICES = 3,
    SCREEN_ID_MONITORING = 4,
};

void create_screen_main_screen();
void tick_screen_main_screen();

void create_screen_wifi_setting_screen();
void tick_screen_wifi_setting_screen();

void create_screen_devices();
void tick_screen_devices();

void create_screen_monitoring();
void tick_screen_monitoring();

void tick_screen_by_id(enum ScreensEnum screenId);
void tick_screen(int screen_index);

void create_screens();


#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_SCREENS_H*/