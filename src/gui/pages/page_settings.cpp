#include "page_settings.h" // already brings lvgl.h in

void page_settings_create(lv_obj_t *parent)
{
    lv_obj_t *label = lv_label_create(parent);
    lv_label_set_text(label, "SETTINGS PAGE\n(No settings yet)");
    lv_obj_set_style_text_color(label, lv_color_black(), LV_PART_MAIN);
    lv_obj_align(label, LV_ALIGN_TOP_LEFT, 10, 10);
}
