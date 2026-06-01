#include "device_manager.hpp"
#include <stddef.h> // Để dùng NULL

// Mảng tĩnh chứa danh sách thiết bị (chỉ file này nhìn thấy)
static const DeviceMapping device_list[] = {
{0, "HG-D1", "TEWD43824IS5"},
{1, "HG-W1", "TEWD43824IS5"},
{2, "HG-W2", "TEWD43888E0V"},      
{3, "HG-D2", "TEWD439047DO"},
{4, "HG-W3", "TEWD439521PK"},
{5, "HG-D3", "TEWD43792MCY"},
{6, "HG-W4", "TEWD43936POL"},
{7, "HG-D4", "TEWD43248HKB"},
{8, "HV-W1", "TEWD43168T2E"},
{9, "HV-D1", "TEWD432161J4"},
{10, "HV-W2", "TEWD43200TJ3"},
{11, "HV-D2", "TEWD43232S8O"},
{12, "HV-W3", "TEWD4302456P"},
{13, "HV-D3", "TEWD43008MIA"},
{14, "HV-W4", "TEWD43056711"},
{15, "HV-D4", "TEWD44000YZ8"}

};

// Tính toán số lượng phần tử trong mảng
static const uint16_t total_devices = sizeof(device_list) / sizeof(device_list[0]);

void device_manager_init(void) {
    // Thực hiện cấu hình ban đầu nếu cần (ví dụ: đọc từ Flash/EEPROM)
    // Hiện tại có thể để trống
}

uint16_t device_get_total_count(void) {
    return total_devices;
}

const char* device_get_serial_by_index(uint16_t index) {
    if (index < total_devices) {
        // Kiểm tra tính đồng bộ giữa Index UI và vị trí mảng
        if (device_list[index].ui_index == index) {
            return device_list[index].serial_num;
        }
    }
    return "UNKNOWN_SERIAL"; // Trả về mã lỗi nếu không tìm thấy
}

const char* device_get_name_by_index(uint16_t index) {
    if (index < total_devices) {
        return device_list[index].ui_name;
    }
    return "UNKNOWN_DEVICE";
}