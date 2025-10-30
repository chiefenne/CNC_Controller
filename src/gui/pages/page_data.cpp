#include "page_data.h" // already brings lvgl.h in
#include "comm/espnow.h"

void page_data_create(lv_obj_t *parent)
{
    lv_obj_t *label = lv_label_create(parent);
    lv_label_set_text(label, "DATA PAGE");
    lv_obj_set_style_text_color(label, lv_color_black(), LV_PART_MAIN);
    lv_obj_align(label, LV_ALIGN_TOP_LEFT, 10, 10);

    lv_obj_t *temp = lv_label_create(parent);
    lv_label_set_text(temp, "Temp: 24.3°C\nVolt: 3.31V\nRSSI: -52dBm");
    lv_obj_set_style_text_color(temp, lv_color_black(), LV_PART_MAIN);
    lv_obj_align(temp, LV_ALIGN_TOP_LEFT, 10, 40);

        uint8_t payload[] = {0xA5, 0x01, 0x5A};  // small ID packet
        // ESPNOW::send(payload, sizeof(payload));
        Serial.println("📡 (Simulated) ESP-NOW payload prepared");

}
