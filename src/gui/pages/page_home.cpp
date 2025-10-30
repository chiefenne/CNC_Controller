#include "page_home.h" // already brings lvgl.h in

void page_home_create(lv_obj_t *parent)
{
    lv_obj_t *label = lv_label_create(parent);
    lv_label_set_text(label, "HOME\nTap tabs below to switch pages");
    lv_obj_set_style_text_color(label, lv_color_black(), LV_PART_MAIN);
    lv_obj_align(label, LV_ALIGN_TOP_LEFT, 10, 10);
}
