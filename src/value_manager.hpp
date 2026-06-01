#ifndef VALUE_MANAGER_H
#define VALUE_MANAGER_H

#include <stdint.h>

typedef struct {
    uint16_t ui_index;       // Chỉ số trả về từ Dropdown Value
    const char* ui_name;     // Tên hiển thị tương ứng trên giao diện UI
    const char* payload_val; // Chuỗi ký tự số gửi lên Server
} ValueMapping;

// Hàm lấy giá trị chuỗi payload truyền từ index của Dropdown giá trị
const char* value_get_payload_by_index(uint16_t index);

// Hàm lấy tổng số lượng giá trị cấu hình
uint16_t value_get_total_count(void);

#endif // VALUE_MANAGER_H