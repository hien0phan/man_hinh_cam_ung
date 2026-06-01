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
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.obj0 = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, 320, 33);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff2d75eb), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.obj1 = obj;
            lv_obj_set_pos(obj, 88, 9);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xfffafcff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "KOI WASH SYSTEMS");
        }
        {
            // date_timer
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.date_timer = obj;
            lv_obj_set_pos(obj, 9, 43);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xff212121), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "Date:                                         ");
        }
        {
            // time_timer
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.time_timer = obj;
            lv_obj_set_pos(obj, 9, 70);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xff212121), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "Time:                                     ");
        }
        {
            // wifi_setting_button
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.wifi_setting_button = obj;
            lv_obj_set_pos(obj, 30, 146);
            lv_obj_set_size(obj, 100, 50);
            // lv_obj_add_event_cb(obj, action_ui_btn_wifi_clicked, LV_EVENT_PRESSED, (void *)0);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff2d75eb), LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "Wifi setting");
                }
            }
        }
        {
            // monitor_button
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.monitor_button = obj;
            lv_obj_set_pos(obj, 196, 146);
            lv_obj_set_size(obj, 100, 50);
            // lv_obj_add_event_cb(obj, action_ui_btn_minitor_clicked, LV_EVENT_PRESSED, (void *)0);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff2d75eb), LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "Monitors");
                }
            }
        }
    }
    
    tick_screen_main();
}

void tick_screen_main() {
}

void create_screen_wifi_setting_screen() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.wifi_setting_screen = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 320, 240);
    {
        lv_obj_t *parent_obj = obj;
        {
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.obj2 = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, 320, 33);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff2d75eb), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.obj3 = obj;
            lv_obj_set_pos(obj, 112, 9);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xfffafcff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "WIFI SETTING");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.obj4 = obj;
            lv_obj_set_pos(obj, 16, 52);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xff040102), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "Wifi ID: ");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.obj5 = obj;
            lv_obj_set_pos(obj, 16, 92);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xff040102), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "Password:");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.obj6 = obj;
            lv_obj_set_pos(obj, 16, 132);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xff212121), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "Status:");
        }
        {
            // connect_button
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.connect_button = obj;
            lv_obj_set_pos(obj, 24, 180);
            lv_obj_set_size(obj, 100, 50);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "Connect");
                }
            }
        }
        {
            // back_wifi_button
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.back_wifi_button = obj;
            lv_obj_set_pos(obj, 193, 180);
            lv_obj_set_size(obj, 100, 50);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "Back");
                }
            }
        }
        {
            // wifi_id_text
            lv_obj_t *obj = lv_textarea_create(parent_obj);
            objects.wifi_id_text = obj;
            lv_obj_set_pos(obj, 93, 42);
            lv_obj_set_size(obj, 200, 36);
            lv_textarea_set_max_length(obj, 128);
            lv_textarea_set_one_line(obj, false);
            lv_textarea_set_password_mode(obj, false);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_color(obj, lv_color_hex(0xff212121), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // password_text
            lv_obj_t *obj = lv_textarea_create(parent_obj);
            objects.password_text = obj;
            lv_obj_set_pos(obj, 93, 82);
            lv_obj_set_size(obj, 200, 36);
            lv_textarea_set_max_length(obj, 128);
            lv_textarea_set_one_line(obj, false);
            lv_textarea_set_password_mode(obj, false);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_color(obj, lv_color_hex(0xff212121), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // status_text
            lv_obj_t *obj = lv_textarea_create(parent_obj);
            objects.status_text = obj;
            lv_obj_set_pos(obj, 93, 122);
            lv_obj_set_size(obj, 200, 36);
            lv_textarea_set_max_length(obj, 128);
            lv_textarea_set_one_line(obj, false);
            lv_textarea_set_password_mode(obj, false);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_color(obj, lv_color_hex(0xff212121), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
    }
    
    tick_screen_wifi_setting_screen();
}

void tick_screen_wifi_setting_screen() {
}

void create_screen_monitor_screen() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.monitor_screen = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 320, 240);
    {
        lv_obj_t *parent_obj = obj;
        {
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.obj7 = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, 320, 33);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff2d75eb), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.obj8 = obj;
            lv_obj_set_pos(obj, 125, 9);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_color(obj, lv_color_hex(0xfffafcff), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "MONITOR");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 6, 48);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "Serial Number:");
        }
        {
            // serial_number_list_dropdowm
            lv_obj_t *obj = lv_dropdown_create(parent_obj);
            objects.serial_number_list_dropdowm = obj;
            lv_obj_set_pos(obj, 125, 40);
            lv_obj_set_size(obj, 180, LV_SIZE_CONTENT);
            lv_dropdown_set_options(obj, "HG-W1\nHG-D1\nHG-W2\nHG-D2\nHG-W3\nHG-D3\nHG-W4\nHG-D4\nHV-W1\nHV-D1\nHV-W2\nHV-D2\nHV-W3\nHV-D3\nHV-W4\nHV-D4");
            lv_dropdown_set_selected(obj, 0);
            lv_obj_set_style_border_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 71, 95);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text(obj, "Value:");
        }
        {
            // send_button
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.send_button = obj;
            lv_obj_set_pos(obj, 25, 164);
            lv_obj_set_size(obj, 100, 50);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff2d75eb), LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "Send");
                }
            }
        }
        {
            // back_monitor_button
            lv_obj_t *obj = lv_btn_create(parent_obj);
            objects.back_monitor_button = obj;
            lv_obj_set_pos(obj, 198, 164);
            lv_obj_set_size(obj, 100, 50);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0xff2d75eb), LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "Back");
                }
            }
        }
        {
            // dropdown_value
            lv_obj_t *obj = lv_dropdown_create(parent_obj);
            objects.dropdown_value = obj;
            lv_obj_set_pos(obj, 125, 87);
            lv_obj_set_size(obj, 180, LV_SIZE_CONTENT);
            lv_dropdown_set_options(obj, "1\n2\n3\n4\n5\n6\n7\n8\n9");
            lv_dropdown_set_selected(obj, 0);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_color(obj, lv_color_hex(0xff000000), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
    }
    
    tick_screen_monitor_screen();
}

void tick_screen_monitor_screen() {
}



typedef void (*tick_screen_func_t)();
tick_screen_func_t tick_screen_funcs[] = {
    tick_screen_main,
    tick_screen_wifi_setting_screen,
    tick_screen_monitor_screen,
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
    create_screen_wifi_setting_screen();
    create_screen_monitor_screen();
}
