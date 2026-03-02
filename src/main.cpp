#include <Arduino.h>
#include <lvgl.h>
#include <TFT_eSPI.h>
#include <XPT2046_Touchscreen.h>
#include "ui.h" 

/* --- CẤU HÌNH PIN CYD --- */
#define TFT_BL 21 
#define XPT2046_IRQ 36
#define XPT2046_MOSI 32
#define XPT2046_MISO 39
#define XPT2046_CLK 25
#define XPT2046_CS 33

TFT_eSPI tft = TFT_eSPI(); 
SPIClass touchscreenSPI = SPIClass(VSPI);
XPT2046_Touchscreen touchscreen(XPT2046_CS, XPT2046_IRQ);
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[320 * 40]; 

/* --- DRIVERS --- */
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);
    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, w, h);
    tft.pushColors((uint16_t *)&color_p->full, w * h, true);
    tft.endWrite();
    lv_disp_flush_ready(disp);
}

void my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data) {
    if (touchscreen.touched()) {
        TS_Point p = touchscreen.getPoint();
        data->point.x = map(p.x, 200, 3700, 1, 320);
        data->point.y = map(p.y, 240, 3800, 1, 240);
        data->state = LV_INDEV_STATE_PR;
    } else {
        data->state = LV_INDEV_STATE_REL;
    }
}

/* --- HÀM TẠO ANIMATION ĐƠN GIẢN --- */
// Hiệu ứng này làm cho một vật thể di chuyển lên xuống nhẹ nhàng
void start_my_animation(lv_obj_t * target) {
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, target);
    lv_anim_set_values(&a, 100, 120);            // Di chuyển từ Y=100 đến Y=120
    lv_anim_set_time(&a, 1000);                 // Thời gian 1 giây
    lv_anim_set_playback_time(&a, 1000);        // Quay về vị trí cũ trong 1 giây
    lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE); // Lặp vô hạn
    lv_anim_set_path_cb(&a, lv_anim_path_ease_in_out);     // Chuyển động mượt
    lv_anim_set_exec_cb(&a, (lv_anim_exec_xcb_t)lv_obj_set_y); // Tác động vào trục Y
    lv_anim_start(&a);
}

void setup() {
    Serial.begin(115200);
    
    // 1. Hardware Init
    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH); // Bật đèn nền tối đa

    tft.init();
    tft.setRotation(1);
    touchscreenSPI.begin(XPT2046_CLK, XPT2046_MISO, XPT2046_MOSI, XPT2046_CS);
    touchscreen.begin(touchscreenSPI);
    touchscreen.setRotation(1);

    // 2. LVGL Init
    lv_init();
    lv_disp_draw_buf_init(&draw_buf, buf, NULL, 320 * 40);
    
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = 320;
    disp_drv.ver_res = 240;
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;
    lv_disp_drv_register(&disp_drv);

    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    indev_drv.read_cb = my_touchpad_read;
    lv_indev_drv_register(&indev_drv);

    // 3. UI & Animation
    ui_init(); 
    
    // Áp dụng animation cho một đối tượng bất kỳ từ EEZ Studio
    // Hãy kiểm tra tên object trong file ui.h (ví dụ: objects.main_button)
    if(objects.my_switch) { 
        start_my_animation(objects.my_switch); 
    }
}

void loop() {
    lv_timer_handler(); 
    delay(5); 
}