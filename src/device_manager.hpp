#ifndef DEVICE_MANAGER_H
#define DEVICE_MANAGER_H

#include <stdint.h>

// Định nghĩa cấu trúc dữ liệu thiết bị
typedef struct {
    uint16_t ui_index;       // Chỉ số trả về từ Dropdown UI
    const char* ui_name;     // Tên viết tắt trên UI
    const char* serial_num;  // Mã Serial chuẩn
} DeviceMapping;

// Khai báo hàm khởi tạo hoặc các hàm tiện ích
void device_manager_init(void);

// Hàm tra cứu Serial Number từ index của Dropdown
const char* device_get_serial_by_index(uint16_t index);

// Hàm lấy tên viết tắt từ index (nếu cần hiển thị log)
const char* device_get_name_by_index(uint16_t index);

// Hàm lấy tổng số lượng thiết bị hiện tại
uint16_t device_get_total_count(void);

#endif // DEVICE_MANAGER_H