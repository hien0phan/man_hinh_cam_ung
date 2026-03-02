#include <string.h>

#include "screens.h"
#include "images.h"
#include "fonts.h"
#include "actions.h"
#include "vars.h"
#include "styles.h"
#include "ui.h"

#include <string.h>

objects_t objects;
lv_obj_t *tick_value_change_obj;
uint32_t active_theme_index = 0;

void create_screen_main() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.main = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 320, 240);
    {
        lv_obj_t *parent_obj = obj;
        {
            // my_panel_header
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.my_panel_header = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, 320, 33);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff00878f), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.obj0 = obj;
            lv_obj_set_pos(obj, 98, 9);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "Test screen LVGL");
        }
        {
            // my_panel_bar
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.my_panel_bar = obj;
            lv_obj_set_pos(obj, 4, 37);
            lv_obj_set_size(obj, 202, 95);
            lv_obj_set_style_border_color(obj, lv_color_hex(0xffb6b6b6), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // my_vertical_bar
            lv_obj_t *obj = lv_bar_create(parent_obj);
            objects.my_vertical_bar = obj;
            lv_obj_set_pos(obj, 11, 44);
            lv_obj_set_size(obj, 9, 80);
            lv_bar_set_value(obj, 50, LV_ANIM_OFF);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffb6b6b6), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff47663b), LV_PART_INDICATOR | LV_STATE_DEFAULT);
        }
        {
            // my_vertiacl_label_bar
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.my_vertiacl_label_bar = obj;
            lv_obj_set_pos(obj, 22, 44);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xff47663b), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "50");
        }
        {
            // my_horizon_bar
            lv_obj_t *obj = lv_bar_create(parent_obj);
            objects.my_horizon_bar = obj;
            lv_obj_set_pos(obj, 24, 118);
            lv_obj_set_size(obj, 80, 9);
            lv_bar_set_value(obj, 50, LV_ANIM_OFF);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff47663b), LV_PART_INDICATOR | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 255, LV_PART_INDICATOR | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffb6b6b6), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // my_label_horizon_bar
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.my_label_horizon_bar = obj;
            lv_obj_set_pos(obj, 26, 105);
            lv_obj_set_size(obj, 80, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xff47663b), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "50");
        }
        {
            // my_arc_bar
            lv_obj_t *obj = lv_arc_create(parent_obj);
            objects.my_arc_bar = obj;
            lv_obj_set_pos(obj, 120, 46);
            lv_obj_set_size(obj, 80, 80);
            lv_arc_set_value(obj, 25);
            lv_arc_set_bg_start_angle(obj, 130);
            lv_arc_set_bg_end_angle(obj, 50);
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_CLICKABLE);
            lv_obj_set_style_arc_width(obj, 9, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_arc_color(obj, lv_color_hex(0xffb6b6b6), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_arc_width(obj, 9, LV_PART_INDICATOR | LV_STATE_DEFAULT);
            lv_obj_set_style_arc_color(obj, lv_color_hex(0xff47663b), LV_PART_INDICATOR | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
        }
        {
            // my_label_arc_bar
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.my_label_arc_bar = obj;
            lv_obj_set_pos(obj, 151, 107);
            lv_obj_set_size(obj, 20, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xff47663b), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "50");
        }
        {
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.obj1 = obj;
            lv_obj_set_pos(obj, 79, 40);
            lv_obj_set_size(obj, 40, 24);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff47663b), LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // my_label_button_test
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.my_label_button_test = obj;
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "Test");
                }
            }
        }
        {
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.obj2 = obj;
            lv_obj_set_pos(obj, 212, 38);
            lv_obj_set_size(obj, 104, 95);
            lv_obj_set_style_border_color(obj, lv_color_hex(0xffb6b6b6), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // my_toggle_button
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.my_toggle_button = obj;
            lv_obj_set_pos(obj, 218, 43);
            lv_obj_set_size(obj, 90, 40);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_CHECKABLE);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffff7f50), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "Toggle Button");
                }
            }
        }
        {
            // my_switch
            lv_obj_t *obj = lv_switch_create(parent_obj);
            objects.my_switch = obj;
            lv_obj_set_pos(obj, 215, 100);
            lv_obj_set_size(obj, 50, 25);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffb6b6b6), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff2196f3), LV_PART_INDICATOR | LV_STATE_CHECKED);
            lv_obj_set_style_bg_opa(obj, 255, LV_PART_INDICATOR | LV_STATE_CHECKED);
        }
        {
            // my_label_switch
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.my_label_switch = obj;
            lv_obj_set_pos(obj, 271, 102);
            lv_obj_set_size(obj, 37, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xffb6b6b6), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "OFF");
        }
        {
            // my_panel_slider
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.my_panel_slider = obj;
            lv_obj_set_pos(obj, 4, 136);
            lv_obj_set_size(obj, 202, 95);
            lv_obj_set_style_border_color(obj, lv_color_hex(0xffb6b6b6), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // my_horizon_slide
            lv_obj_t *obj = lv_slider_create(parent_obj);
            objects.my_horizon_slide = obj;
            lv_obj_set_pos(obj, 30, 213);
            lv_obj_set_size(obj, 145, 12);
            lv_slider_set_value(obj, 50, LV_ANIM_OFF);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffb6b6b6), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff7209b7), LV_PART_INDICATOR | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 2, LV_PART_KNOB | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 2, LV_PART_KNOB | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_left(obj, 2, LV_PART_KNOB | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 2, LV_PART_KNOB | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff56088a), LV_PART_KNOB | LV_STATE_DEFAULT);
        }
        {
            // my_horizon_slider_label
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.my_horizon_slider_label = obj;
            lv_obj_set_pos(obj, 95, 194);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xff7209b7), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "50");
        }
        {
            // my_vertical_slider
            lv_obj_t *obj = lv_slider_create(parent_obj);
            objects.my_vertical_slider = obj;
            lv_obj_set_pos(obj, 188, 149);
            lv_obj_set_size(obj, 12, 70);
            lv_slider_set_value(obj, 50, LV_ANIM_OFF);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xffb6b6b6), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff7209b7), LV_PART_INDICATOR | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 2, LV_PART_KNOB | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 2, LV_PART_KNOB | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_left(obj, 2, LV_PART_KNOB | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 2, LV_PART_KNOB | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff56088a), LV_PART_KNOB | LV_STATE_DEFAULT);
        }
        {
            // my_vertical_label
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.my_vertical_label = obj;
            lv_obj_set_pos(obj, 160, 149);
            lv_obj_set_size(obj, 24, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xff7209b7), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "50");
        }
        {
            // my_image_panel
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.my_image_panel = obj;
            lv_obj_set_pos(obj, 211, 137);
            lv_obj_set_size(obj, 104, 95);
            lv_obj_set_style_border_color(obj, lv_color_hex(0xffb6b6b6), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // my_image
            lv_obj_t *obj = lv_img_create(parent_obj);
            objects.my_image = obj;
            lv_obj_set_pos(obj, 222, 144);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_img_set_src(obj, &img_image);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_CLICKABLE);
        }
    }
    
    tick_screen_main();
}

void tick_screen_main() {
}



typedef void (*tick_screen_func_t)();
tick_screen_func_t tick_screen_funcs[] = {
    tick_screen_main,
};
void tick_screen(int screen_index) {
    tick_screen_funcs[screen_index]();
}
void tick_screen_by_id(enum ScreensEnum screenId) {
    tick_screen_funcs[screenId - 1]();
}

void create_screens() {
    lv_disp_t *dispp = lv_disp_get_default();
    lv_theme_t *theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), false, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);
    
    create_screen_main();
}
