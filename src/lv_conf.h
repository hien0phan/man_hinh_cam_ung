#ifndef LV_CONF_H
#define LV_CONF_H

#include <stdint.h>

// 1. Kích hoạt
#if 1 
#define LV_MEM_SIZE (48U * 1024U) // Tăng từ 32 lên 64 hoặc 96 nếu cần

// 2. Cấu hình màu sắc (ILI9341 dùng 16 bit)
#define LV_COLOR_DEPTH 16
#define LV_COLOR_16_SWAP 1 // Đảo byte màu (quan trọng với TFT_eSPI)

// 3. Bộ nhớ (ESP32 có RAM lớn, cho phép dùng 48KB cho LVGL)
#define LV_MEM_CUSTOM 0

// 4. Tần suất cập nhật (Dùng millis của Arduino)
#define LV_TICK_CUSTOM 1
#define LV_TICK_CUSTOM_INCLUDE "Arduino.h"
#define LV_TICK_CUSTOM_SYS_TIME_EXPR (millis())

// 5. Các cài đặt khác (Font chữ, v.v...)
#define LV_SHADOW_CACHE_SIZE 0
#define LV_IMG_CACHE_DEF_SIZE 0
#define LV_USE_LOG 0           // Tắt log để tiết kiệm
#define LV_USE_ASSERT_NULL 1
#define LV_USE_ASSERT_MEM 1
#define LV_USE_USER_DATA 1
#define LV_USE_GIF 1

#define LV_INDEV_DEF_READ_PERIOD 10

// Font chữ mặc định
#define LV_FONT_DEFAULT &lv_font_montserrat_14
#define LV_FONT_MONTSERRAT_12  0
#define LV_FONT_MONTSERRAT_14  1  // Mặc định hay dùng
#define LV_FONT_MONTSERRAT_20  0
#define LV_FONT_MONTSERRAT_18  0
#define LV_FONT_MONTSERRAT_16  1

#endif /*LV_CONF_H*/
#endif /*End of "if 0"*/