#ifndef EEZ_LVGL_UI_SCREENS_H
#define EEZ_LVGL_UI_SCREENS_H

#include <lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _objects_t {
    lv_obj_t *main;
    lv_obj_t *my_panel_header;
    lv_obj_t *obj0;
    lv_obj_t *my_panel_bar;
    lv_obj_t *my_vertical_bar;
    lv_obj_t *my_vertiacl_label_bar;
    lv_obj_t *my_horizon_bar;
    lv_obj_t *my_label_horizon_bar;
    lv_obj_t *my_arc_bar;
    lv_obj_t *my_label_arc_bar;
    lv_obj_t *obj1;
    lv_obj_t *my_label_button_test;
    lv_obj_t *obj2;
    lv_obj_t *my_toggle_button;
    lv_obj_t *my_switch;
    lv_obj_t *my_label_switch;
    lv_obj_t *my_panel_slider;
    lv_obj_t *my_horizon_slide;
    lv_obj_t *my_horizon_slider_label;
    lv_obj_t *my_vertical_slider;
    lv_obj_t *my_vertical_label;
    lv_obj_t *my_image_panel;
    lv_obj_t *my_image;
} objects_t;

extern objects_t objects;

enum ScreensEnum {
    SCREEN_ID_MAIN = 1,
};

void create_screen_main();
void tick_screen_main();

void tick_screen_by_id(enum ScreensEnum screenId);
void tick_screen(int screen_index);

void create_screens();


#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_SCREENS_H*/