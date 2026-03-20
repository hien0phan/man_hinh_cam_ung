#include <Arduino.h>
#include <lvgl.h>
#include <TFT_eSPI.h>
#include <XPT2046_Touchscreen.h>
#include <WiFi.h>
#include "ui.h" 
#include "time.h"
#include <PubSubClient.h>
#include <ArduinoJson.h>

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
lv_obj_t * kb = NULL; // Bàn phím ảo, nếu cần
bool is_wifi_connected = false;
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 7 * 3600; // GMT+7
const int   daylightOffset_sec = 0;   // Không có giờ mùa hè

String g_sn = ""; 
String g_status_text = "Not running"; 
bool g_is_mqtt_connected = false;

WiFiClient espClient;
PubSubClient client(espClient);

const char* mqtt_server = "devices.koisolutions.vn";
String device_sn = ""; // Biến lưu Serial Number để dùng cho Topic

extern "C" {
    // Hàm xử lý sự kiện khi nhấn vào bất kỳ TextArea nào
    static void ta_event_cb(lv_event_t * e) {
        lv_event_code_t code = lv_event_get_code(e);
        lv_obj_t * ta = lv_event_get_target(e); // Lấy TextArea đang được tác động

      if(code == LV_EVENT_FOCUSED) {
        // Nếu đã có bàn phím, phải đưa nó về màn hình hiện tại
        if(kb == NULL) {
            kb = lv_keyboard_create(lv_scr_act());
        } else {
            lv_obj_set_parent(kb, lv_scr_act()); // QUAN TRỌNG: Đưa kb sang màn hình mới
        }
        
        lv_keyboard_set_textarea(kb, ta);
        lv_obj_clear_flag(kb, LV_OBJ_FLAG_HIDDEN);
        lv_obj_move_foreground(kb); // Đưa lên trên cùng
    }

        if(code == LV_EVENT_DEFOCUSED) {
            // Khi nhấn ra ngoài hoặc nhấn dấu tích hoàn tất, ẩn bàn phím
            if(kb != NULL) {
                lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
            }
        }
        if(code == LV_EVENT_READY) {
            lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_state(ta, LV_STATE_FOCUSED); // Bỏ chọn TextArea
        }
    }
}

extern "C" {
    void action_connect_wifi(lv_event_t * e) {
        lv_event_code_t code = lv_event_get_code(e);
        
        if(code == LV_EVENT_CLICKED) {
            // 1. Lấy nội dung từ TextArea 1 (SSID) và 2 (Password)
            const char * ssid = lv_textarea_get_text(objects.wifi_id_text);
            const char * password = lv_textarea_get_text(objects.wifi_password_text);

            Serial.print("Dang ket noi WiFi: ");
            Serial.println(ssid);

            // 2. Cập nhật trạng thái tạm thời lên obj 3
            lv_textarea_set_text(objects.wifi_status_text, "Connecting...");

            // 3. Bắt đầu kết nối (Non-blocking hoặc dùng Timeout nhẹ)
            WiFi.begin(ssid, password);
        }
    }
}

void my_next_to_wifi_cb(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    
    if(code == LV_EVENT_CLICKED) {
        // Kiểm tra đối tượng màn hình có tồn tại không
        if(objects.wifi_setting_screen) {
            // Hiệu ứng nảy màn hình (trượt từ dưới lên)
            lv_scr_load_anim(objects.wifi_setting_screen, 
                             LV_SCR_LOAD_ANIM_MOVE_TOP, 
                             300, 0, false);
            Serial.println("Nut bam da kich hoat chuyen trang!");
        }
    }
}

void my_next_to_devices_cb(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    
    if(code == LV_EVENT_CLICKED) {
        // Kiểm tra đối tượng màn hình có tồn tại không
        if(objects.devices) {
            // Hiệu ứng nảy màn hình (trượt từ dưới lên)
            lv_scr_load_anim(objects.devices, 
                             LV_SCR_LOAD_ANIM_MOVE_TOP, 
                             300, 0, false);
            Serial.println("Nut bam da kich hoat chuyen trang!");
        }
    }
}

void my_next_to_monitor_cb(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    
    if(code == LV_EVENT_CLICKED) {
        // Kiểm tra đối tượng màn hình có tồn tại không
        if(objects.monitoring) {
            // Hiệu ứng nảy màn hình (trượt từ dưới lên)
            lv_scr_load_anim(objects.monitoring, 
                             LV_SCR_LOAD_ANIM_MOVE_TOP, 
                             300, 0, false);
            Serial.println("Nut bam da kich hoat chuyen trang!");
        }
    }
}

void go_back_cb(lv_event_t * e) {
    if(lv_event_get_code(e) == LV_EVENT_CLICKED) {
        // Luôn nảy về màn hình chính
        lv_scr_load_anim(objects.main_screen, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 300, 0, false);
    }
}

void update_clock_ui() {
    // Chỉ cập nhật nếu đang ở màn hình Main để tránh lỗi tràn bộ nhớ (DRAM)
    if (lv_scr_act() != objects.main_screen) return;

    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        // Nếu chưa lấy được giờ, hiển thị trạng thái chờ
        if(objects.time_main_label) lv_label_set_text(objects.time_main_label, "--:--");
        return;
    }

    // 1. Cập nhật GIỜ (Ví dụ: 14:30)
    char buf_time[9];
    strftime(buf_time, sizeof(buf_time), "%H:%M", &timeinfo);
    if(objects.time_main_label) {
        lv_label_set_text(objects.time_main_label, buf_time);
    }

    // 2. Cập nhật NGÀY (Ví dụ: 09/03/2026)
    char buf_date[20];
    strftime(buf_date, sizeof(buf_date), "%d/%m/%Y", &timeinfo);
    if(objects.date_main_label) {
        lv_label_set_text(objects.date_main_label, buf_date);
    }
}

int last_s_value = -1; // Biến toàn cục lưu trạng thái cũ

void mqtt_callback(char* topic, byte* payload, unsigned int length) {
    // 1. Tạo buffer tạm để chứa nội dung bản tin (tránh lỗi bộ nhớ)
    StaticJsonDocument<256> doc;
    DeserializationError error = deserializeJson(doc, payload, length);

    // 2. Kiểm tra nếu giải mã JSON lỗi thì THOÁT NGAY, không làm gì cả
    if (error) {
        Serial.print("JSON Error: ");
        Serial.println(error.c_str());
        return; 
    }

    // 3. Kiểm tra xem Key "s" có tồn tại hay không
    // Nếu không có key "s", ArduinoJson sẽ tự gán s = 0 (gây ra lỗi bạn đang gặp)
    if (doc.containsKey("s")) {
        int current_s = doc["s"];

        // 4. Chỉ xử lý nếu giá trị s nhận được khác với trạng thái hiện tại
        if (current_s != last_s_value) {
            last_s_value = current_s; // Cập nhật trạng thái mới vào bộ nhớ đệm

            if (current_s == 1) {
                g_status_text = "Running";
                if (objects.mqtt_status_text) {
                    lv_textarea_set_text(objects.mqtt_status_text, "Running");
                    lv_obj_set_style_text_color(objects.mqtt_status_text, lv_palette_main(LV_PALETTE_GREEN), 0);
                }
            } 
            else if (current_s == 0) { // Dùng else if rõ ràng để tránh nhận giá trị rác
                g_status_text = "Not running";
                if (objects.mqtt_status_text) {
                    lv_textarea_set_text(objects.mqtt_status_text, "Not running");
                    lv_obj_set_style_text_color(objects.mqtt_status_text, lv_palette_main(LV_PALETTE_RED), 0);
                }
            }
            
            Serial.printf("State updated to: %s\n", g_status_text.c_str());
        }
    } else {
        // Nếu bản tin không có key "s", chúng ta bỏ qua hoàn toàn, 
        // không nhảy về "Not running" nữa.
        Serial.println("Warning: Received JSON without key 's'");
    }
}
// void ui_event_btn_connect(lv_event_t * e) {
//     if (lv_event_get_code(e) == LV_EVENT_CLICKED) {
//         const char * input_sn = lv_textarea_get_text(objects.text_serial_number_devices);
//         if (strlen(input_sn) == 0) return;

//         g_sn = String(input_sn);
        
//         // Cấu hình MQTT
//         client.setServer("devices.koisolutions.vn", 7183);
//         client.setCallback(mqtt_callback);

//         String clientId = "ESP32-Client-" + String(random(0, 1000));
//         if (client.connect(clientId.c_str())) {
//             String topic = "Kdev/" + g_sn + "/info";
//             client.subscribe(topic.c_str());
            
//             if(objects.mqtt_status_text) {
//                 lv_textarea_set_text(objects.mqtt_status_text, "Connected & Waiting...");
//             }
//         } else {
//             if(objects.mqtt_status_text) {
//                 lv_textarea_set_text(objects.mqtt_status_text, "Connect Failed!");
//             }
//         }
//     }
// }

void ui_event_btn_control(lv_event_t * e) {
    if (lv_event_get_code(e) == LV_EVENT_CLICKED) {
        // 1. Gán Serial Number sang màn hình tiếp theo
        if (objects.text_serial_number_control) {
            lv_textarea_set_text(objects.text_serial_number_control, g_sn.c_str());
        }

        // 2. Gán Status hiện tại sang màn hình tiếp theo
        if (objects.text_status_control) {
            lv_textarea_set_text(objects.text_status_control, g_status_text.c_str());
        }

        // 3. Chuyển màn hình
        lv_scr_load_anim(objects.monitoring, LV_SCR_LOAD_ANIM_FADE_ON, 300, 0, false);
    }
}


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
     
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    lv_obj_set_scrollbar_mode(lv_scr_act(), LV_SCROLLBAR_MODE_AUTO);

    if(objects.wifi_id_text) {
        lv_obj_add_event_cb(objects.wifi_id_text, ta_event_cb, LV_EVENT_ALL, NULL);
    }
    
    if(objects.wifi_password_text) {
        lv_obj_add_event_cb(objects.wifi_password_text, ta_event_cb, LV_EVENT_ALL, NULL);
    }

    // if(objects.text_serial_number_devices) {
    //     lv_obj_add_event_cb(objects.text_serial_number_devices, ta_event_cb, LV_EVENT_ALL, NULL);
    // }

    if(objects.wifi_status_text) {
        lv_textarea_set_text(objects.wifi_status_text, "Not connected");
    }

    // Gán hàm kết nối cho Button (nếu bạn chưa gán trong EEZ Studio)
    if(objects.connect_button_wifi) {
        lv_obj_add_event_cb(objects.connect_button_wifi, action_connect_wifi, LV_EVENT_CLICKED, NULL);
    }

    if(objects.button_main_wifi_setting) {
        lv_obj_add_event_cb(objects.button_main_wifi_setting, my_next_to_wifi_cb, LV_EVENT_CLICKED, NULL);
    }

    if(objects.button_main_devices_setting) {
        lv_obj_add_event_cb(objects.button_main_devices_setting, my_next_to_devices_cb, LV_EVENT_CLICKED, NULL);
    }

    if(objects.button_main_monitor) {
        lv_obj_add_event_cb(objects.button_main_monitor, my_next_to_monitor_cb, LV_EVENT_CLICKED, NULL);
    }

    if(objects.button_main_control) {
        lv_obj_add_event_cb(objects.button_main_control, my_next_to_monitor_cb, LV_EVENT_CLICKED, NULL);
    }

    // if(objects.connect_button_devices) {
    //     lv_obj_add_event_cb(objects.connect_button_devices, ui_event_btn_connect, LV_EVENT_CLICKED, NULL);
    // }

    if(objects.control_button_devices) {
        lv_obj_add_event_cb(objects.control_button_devices, ui_event_btn_control, LV_EVENT_CLICKED, NULL);
    }
    // if(objects.control_button_devices) {
    //     lv_obj_add_event_cb(objects.control_button_devices, ui_event_btn_control, LV_EVENT_CLICKED, NULL);
    // }


    lv_obj_add_event_cb(objects.back_button_wifi, go_back_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(objects.back_button_devices, go_back_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(objects.back_button_control, go_back_cb, LV_EVENT_CLICKED, NULL);

    // Luôn load màn hình chính lên đầu tiên
    lv_scr_load(objects.main_screen);

}

unsigned long last_check_time = 0;
unsigned long last_clock_tick = 0; // Khai báo biến lưu thời gian đếm

void loop() {
    lv_timer_handler();
    delay(5);

    if (millis() - last_clock_tick > 1000) {
        update_clock_ui();
        last_clock_tick = millis();
    }
    // Duy trì MQTT
    if (WiFi.status() == WL_CONNECTED) {
        client.loop();
    }
    // Cứ mỗi 500ms kiểm tra trạng thái WiFi một lần
    if (millis() - last_check_time > 500) {
        last_check_time = millis();
        
        if (WiFi.status() == WL_CONNECTED) {
            if (!is_wifi_connected) { // Chỉ cập nhật khi trạng thái thay đổi
                lv_textarea_set_text(objects.wifi_status_text, "Connected");
                // Có thể đổi màu chữ sang xanh nếu muốn
                lv_obj_set_style_text_color(objects.wifi_status_text, lv_palette_main(LV_PALETTE_GREEN), 0);
                is_wifi_connected = true;
                Serial.println("WiFi Connected!");
            }
        } else {
            if (is_wifi_connected) {
                lv_textarea_set_text(objects.wifi_status_text, "Not connected");
                lv_obj_set_style_text_color(objects.wifi_status_text, lv_palette_main(LV_PALETTE_RED), 0);
                is_wifi_connected = false;
            }
        }
    }
}