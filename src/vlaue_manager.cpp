#include "value_manager.hpp"

// Mảng tĩnh lưu cấu hình ánh xạ các mức giá trị dao động từ 1 đến 9
static const ValueMapping value_list[] = {
    {0, "1", "1"},
    {1, "2", "2"},
    {2, "3", "3"},
    {3, "4", "4"},
    {4, "5", "5"},
    {5, "6", "6"},
    {6, "7", "7"},
    {7, "8", "8"},
    {8, "9", "9"}
};

static const uint16_t total_values = sizeof(value_list) / sizeof(value_list[0]);

const char* value_get_payload_by_index(uint16_t index) {
    if (index < total_values) {
        if (value_list[index].ui_index == index) {
            return value_list[index].payload_val;
        }
    }
    return "UNKNOWN_VALUE";
}

uint16_t value_get_total_count(void) {
    return total_values;
}