#include <Arduino.h>
#include <lvgl.h>
#include <TFT_eSPI.h>
#include <XPT2046_Touchscreen.h>
#include <WiFi.h>
#include "ui.h" 
#include "time.h"
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <device_manager.hpp>
#include <value_manager.hpp>

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
lv_obj_t * kb = NULL; 

// Biến trạng thái toàn cục
bool is_wifi_connected = false;
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 7 * 3600; 
const int daylightOffset_sec = 0;   

String g_sn = ""; 
String g_status_text = "Not running"; 
bool g_is_mqtt_connected = false;

WiFiClient espClient;
PubSubClient client(espClient);
const char* mqtt_server = "devices.koisolutions.vn";
String device_sn = ""; 

// Mutex để bảo vệ tài nguyên dùng chung của LVGL khi ghi dữ liệu từ Task khác
SemaphoreHandle_t lvgl_mutex;
SemaphoreHandle_t mqtt_mutex;

/* --- KHAI BÁO CÁC HÀM TASK FREERTOS --- */
void TaskLVGL(void *pvParameters);
void TaskNetwork(void *pvParameters);
void TaskClock(void *pvParameters);

/* --- CALLBACK EVENT HANDLERS (LVGL) --- */
extern "C" {
    static void ta_event_cb(lv_event_t * e) {
        lv_event_code_t code = lv_event_get_code(e);
        lv_obj_t * ta = lv_event_get_target(e); 

        if(code == LV_EVENT_FOCUSED) {
            if(kb == NULL) {
                kb = lv_keyboard_create(lv_scr_act());
            } else {
                lv_obj_set_parent(kb, lv_scr_act()); 
            }
            lv_keyboard_set_textarea(kb, ta);
            lv_obj_clear_flag(kb, LV_OBJ_FLAG_HIDDEN);
            lv_obj_move_foreground(kb); 
        }

        if(code == LV_EVENT_DEFOCUSED) {
            if(kb != NULL) {
                lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
            }
        }
        if(code == LV_EVENT_READY) {
            lv_obj_add_flag(kb, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_state(ta, LV_STATE_FOCUSED); 
        }
    }
}

extern "C" {
    void action_connect_wifi(lv_event_t * e) {
        lv_event_code_t code = lv_event_get_code(e);
        if(code == LV_EVENT_CLICKED) {
            const char * ssid = lv_textarea_get_text(objects.wifi_id_text);
            const char * password = lv_textarea_get_text(objects.password_text);

            Serial.print("Dang ket noi WiFi: ");
            Serial.println(ssid);

            lv_textarea_set_text(objects.status_text, "Connecting...");
            
            // Ra lệnh kết nối WiFi (Hàm này bản chất là non-blocking nên gọi trực tiếp được)
            WiFi.begin(ssid, password);
        }
    }
}

void my_next_to_wifi_cb(lv_event_t * e) {
    if(lv_event_get_code(e) == LV_EVENT_CLICKED) {
        if(objects.wifi_setting_screen) {
            lv_scr_load_anim(objects.wifi_setting_screen, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 300, 0, false);
        }
    }
}

void my_next_to_monitor_cb(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    
    if(code == LV_EVENT_CLICKED) {
        // SỬA ĐÚNG: Gọi đúng biến đại diện cho MÀN HÌNH MONITOR
        if(objects.monitor_screen) { // <- Sửa tên biến ở đây
            lv_scr_load_anim(objects.monitor_screen, // <- Và ở đây
                             LV_SCR_LOAD_ANIM_MOVE_RIGHT, 
                             300, 0, false);
            Serial.println("Đã chuyển sang màn hình Monitor an toàn!");
        } else {
            Serial.println("Lỗi: Không tìm thấy đối tượng màn hình Monitor!");
        }
    }
}

void go_back_cb(lv_event_t * e) {
    if(lv_event_get_code(e) == LV_EVENT_CLICKED) {
        lv_scr_load_anim(objects.main, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 300, 0, false);
    }
}

void ui_btn_send_control(lv_event_t * e) {
    lv_event_code_t code = lv_event_get_code(e);
    
    if(code == LV_EVENT_CLICKED) {
        // LƯU Ý: Đổi 'objects.device_dropdown' và 'objects.value_dropdown' 
        // thành tên biến dropdown thực tế trong file ui.h của bạn
        uint16_t dev_idx = lv_dropdown_get_selected(objects.serial_number_list_dropdowm); 
        uint16_t val_idx = lv_dropdown_get_selected(objects.dropdown_value); 

        // Tra cứu mã chuẩn từ 2 module manager
        const char* target_sn = device_get_serial_by_index(dev_idx);
        const char* target_val = value_get_payload_by_index(val_idx);

        if (strcmp(target_sn, "UNKNOWN_SERIAL") == 0 || strcmp(target_val, "UNKNOWN_VALUE") == 0) {
            Serial.println("Lỗi: Index vượt quá giới hạn mảng!");
            return;
        }

        // Tạo Topic và Payload
        String topic = String("Kdev/") + target_sn + "/control";
        
        StaticJsonDocument<128> doc;
        doc["t"] = "m";
        doc["v"] = target_val;
        
        String payload;
        serializeJson(doc, payload);

        // Gửi MQTT an toàn qua Mutex
        if (WiFi.status() == WL_CONNECTED && client.connected()) {
            if (xSemaphoreTake(mqtt_mutex, pdMS_TO_TICKS(100)) == pdTRUE) {
                client.publish(topic.c_str(), payload.c_str());
                xSemaphoreGive(mqtt_mutex);
                
                Serial.print("MQTT Sent -> Topic: ");
                Serial.print(topic);
                Serial.print(" | Payload: ");
                Serial.println(payload);
            }
        } else {
            Serial.println("Lỗi: Không thể gửi, mất kết nối MQTT hoặc WiFi!");
        }
    }
}

// Biến toàn cục lưu cấu hình hiện tại để chạy các tác vụ Modbus/MQTT
const char* active_serial_number = "UNKNOWN_SERIAL";

// Hàm callback khi Dropdown thay đổi
void on_dropdown_changed(lv_event_t * e) {
    lv_obj_t * serial_number_list_dropdowm = lv_event_get_target(e);
    
    // Lấy index trực tiếp từ UI
    uint16_t selected_index = lv_dropdown_get_selected(serial_number_list_dropdowm); 
    
    // Chỉ cần gọi đúng 1 dòng lệnh từ file manager để gán giá trị cụ thể
    active_serial_number = device_get_serial_by_index(selected_index);
    Serial.print("Đã chọn thiết bị với Serial: ");
    Serial.println(active_serial_number);
    
}

/* --- DRIVERS GRAPHIC & TOUCH --- */
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


/* --- ARDUINO SETUP --- */

void reconnect_mqtt() {
    if (!client.connected()) {
        Serial.print("Attempting MQTT connection...");
        // Tạo một Client ID ngẫu nhiên để tránh bị trùng lặp trên Server
        String clientId = "ESP32Client-" + String(random(0, 0xffff), HEX);
        
        // Thử kết nối
        if (client.connect(clientId.c_str())) {
            Serial.println("connected");
        } else {
            Serial.print("failed, rc=");
            Serial.println(client.state());
        }
    }
}

void setup() {
    Serial.begin(115200);
    
    // Khởi tạo Mutex bảo vệ LVGL
    lvgl_mutex = xSemaphoreCreateMutex();
    mqtt_mutex = xSemaphoreCreateMutex();

    // 1. Hardware Init
    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH); 

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

    // 3. UI Init & Đăng ký sự kiện
    ui_init(); 
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    lv_obj_set_scrollbar_mode(lv_scr_act(), LV_SCROLLBAR_MODE_AUTO);

    if(objects.wifi_id_text) lv_obj_add_event_cb(objects.wifi_id_text, ta_event_cb, LV_EVENT_ALL, NULL);
    if(objects.password_text) lv_obj_add_event_cb(objects.password_text, ta_event_cb, LV_EVENT_ALL, NULL);
    if(objects.status_text) lv_textarea_set_text(objects.status_text, "Not connected");
    if(objects.connect_button) lv_obj_add_event_cb(objects.connect_button, action_connect_wifi, LV_EVENT_CLICKED, NULL);
    if(objects.wifi_setting_button) lv_obj_add_event_cb(objects.wifi_setting_button, my_next_to_wifi_cb, LV_EVENT_CLICKED, NULL);
    if(objects.monitor_button) lv_obj_add_event_cb(objects.monitor_button, my_next_to_monitor_cb, LV_EVENT_CLICKED, NULL);
    if(objects.send_button) {
        lv_obj_add_event_cb(objects.send_button, ui_btn_send_control, LV_EVENT_CLICKED, NULL);
    }
    lv_obj_add_event_cb(objects.back_wifi_button, go_back_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(objects.back_monitor_button, go_back_cb, LV_EVENT_CLICKED, NULL);

    lv_scr_load(objects.main);

    // 4. KHỞI TẠO CÁC TASK FREERTOS
    // Task xử lý UI đồ họa (Ưu tiên cao nhất, chạy Core 1)
    xTaskCreatePinnedToCore(TaskLVGL, "TaskLVGL", 4096, NULL, 3, NULL, 1);

    // Task xử lý Mạng WiFi + MQTT (Ưu tiên trung bình, chạy Core 0 tách biệt)
    xTaskCreatePinnedToCore(TaskNetwork, "TaskNetwork", 4096, NULL, 2, NULL, 0);

    // Task cập nhật Đồng hồ (Ưu tiên thấp, chạy Core 1 ké với đồ họa)
    xTaskCreatePinnedToCore(TaskClock, "TaskClock", 2048, NULL, 1, NULL, 1);
}

/* --- ARDUINO LOOP (BỎ TRỐNG VÌ ĐÃ CHUYỂN HẾT VÀO TASK) --- */
void loop() {
    vTaskDelete(NULL); // Tự hủy hàm loop mặc định để giải phóng tài nguyên
}

/* ================= THỰC THI CÁC TÁC VỤ (TASK FUNCTIONS) ================= */

// --- TASK 1: XỬ LÝ ĐỒ HỌA LVGL (CORE 1) ---
void TaskLVGL(void *pvParameters) {
    (void) pvParameters;
    TickType_t xLastWakeTime = xTaskGetTickCount();
    
    for (;;) {
        // Khóa Mutex trước khi cho phép LVGL xử lý giao diện nhằm tránh xung đột dữ liệu
        if (xSemaphoreTake(lvgl_mutex, pdMS_TO_TICKS(20)) == pdTRUE) {
            lv_timer_handler();
            xSemaphoreGive(lvgl_mutex);
        }
        // Cho Task ngủ chính xác 5ms để nhường CPU cho luồng khác
        vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(5));
    }
}

// --- TASK 2: XỬ LÝ MẠNG WIFI & MQTT (CORE 0) ---
// --- TASK 2: XỬ LÝ MẠNG WIFI & MQTT (CHẠY TRÊN CORE 0) ---
void TaskNetwork(void *pvParameters) {
    (void) pvParameters;
    
    for (;;) {
        // 1. KIỂM TRA TRẠNG THÁI KẾT NỐI WIFI
        if (WiFi.status() == WL_CONNECTED) {
            
            // Nếu vừa mới kết nối thành công (trạng thái chuyển từ Disconnected -> Connected)
            if (!is_wifi_connected) {
                is_wifi_connected = true;
                Serial.println("WiFi Connected successfully!");
                
                // Ép ESP32 gửi yêu cầu đồng bộ thời gian tới NTP Server ngay khi có Internet
                configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
                
                // Chiếm lvgl_mutex để cập nhật chữ "Connected" màu XANH lên UI an toàn
                if (xSemaphoreTake(lvgl_mutex, portMAX_DELAY) == pdTRUE) {
                    if (objects.status_text) {
                        lv_textarea_set_text(objects.status_text, "Connected");
                        lv_obj_set_style_text_color(objects.status_text, lv_palette_main(LV_PALETTE_GREEN), 0);
                    }
                    xSemaphoreGive(lvgl_mutex); // Nhả khóa ngay sau khi cập nhật xong
                }
            }
            
            // 2. DUY TRÌ VÒNG LẶP VÀ KẾT NỐI LẠI MQTT (CHỈ CHẠY KHI ĐÃ CÓ WIFI)
            // Chiếm mqtt_mutex để tránh xung đột với hàm bấm nút gửi dữ liệu ở Core 1
            if (xSemaphoreTake(mqtt_mutex, pdMS_TO_TICKS(50)) == pdTRUE) {
                if (!client.connected()) {
                    reconnect_mqtt(); // Gọi hàm kết nối lại Broker ngầm
                }
                client.loop(); // Duy trì và nhận bản tin MQTT từ Server
                xSemaphoreGive(mqtt_mutex);
            }
        } 
        else {
            // Nếu bị mất kết nối WiFi (trạng thái chuyển từ Connected -> Disconnected)
            if (is_wifi_connected) {
                is_wifi_connected = false;
                Serial.println("WiFi Link Down!");

                // Chiếm lvgl_mutex để cập nhật chữ "Not connected" màu ĐỎ lên UI an toàn
                if (xSemaphoreTake(lvgl_mutex, portMAX_DELAY) == pdTRUE) {
                    if (objects.status_text) {
                        lv_textarea_set_text(objects.status_text, "Not connected");
                        lv_obj_set_style_text_color(objects.status_text, lv_palette_main(LV_PALETTE_RED), 0);
                    }
                    xSemaphoreGive(lvgl_mutex);
                }
            }
        }
        
        // Chu kỳ quét mạng và xử lý MQTT là 200ms (giúp nhận tin MQTT nhanh, không gây nghẽn CPU)
        vTaskDelay(pdMS_TO_TICKS(200));
    }
}
// --- TASK 3: CẬP NHẬT ĐỒNG HỒ ĐỊNH KỲ (CORE 1) ---
void TaskClock(void *pvParameters) {
    (void) pvParameters;
    struct tm timeinfo;
    char buf_time[20];
    char buf_date[20];

    for (;;) {
        // Luôn ngủ 1 giây trước khi chạy chu kỳ tiếp theo
        vTaskDelay(pdMS_TO_TICKS(1000));

        // Kiểm tra xem hiện tại màn hình Main có đang được hiển thị hay không
        if (lv_scr_act() == objects.main) {
            if (getLocalTime(&timeinfo)) {
                strftime(buf_time, sizeof(buf_time), "Time:%H:%M", &timeinfo);
                strftime(buf_date, sizeof(buf_date), "Date:%d/%m/%Y", &timeinfo);

                // Chiếm quyền điều khiển UI để ghi chuỗi thời gian lên màn hình
                if (xSemaphoreTake(lvgl_mutex, pdMS_TO_TICKS(50)) == pdTRUE) {
                    if(objects.time_timer) lv_label_set_text(objects.time_timer, buf_time);
                    if(objects.date_timer) lv_label_set_text(objects.date_timer, buf_date);
                    xSemaphoreGive(lvgl_mutex);
                }
            } else {
                if (xSemaphoreTake(lvgl_mutex, pdMS_TO_TICKS(50)) == pdTRUE) {
                    if(objects.time_timer) lv_label_set_text(objects.time_timer, "Time:--:--");
                    xSemaphoreGive(lvgl_mutex);
                }
            }
        }
    }
}

