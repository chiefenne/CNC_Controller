#include "fonts.h"
#include "page_home.h" // already brings lvgl.h in

lv_obj_t *lbl_x;
lv_obj_t *lbl_y;
lv_obj_t *lbl_z;
lv_obj_t *lbl_cs;

void page_home_create(lv_obj_t *parent)
{
    // --- X coordinate ---
    lbl_x = lv_label_create(parent);
    lv_label_set_text(lbl_x, "X=+000.000");
    lv_obj_set_style_text_font(lbl_x, &lv_font_roboto_mono_40, 0);
    lv_obj_align(lbl_x, LV_ALIGN_TOP_MID, -24, 14);

    // --- Y coordinate ---
    lbl_y = lv_label_create(parent);
    lv_label_set_text(lbl_y, "Y=+000.000");
    lv_obj_set_style_text_font(lbl_y, &lv_font_roboto_mono_40, 0);
    lv_obj_align_to(lbl_y, lbl_x, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);

    // --- Z coordinate ---
    lbl_z = lv_label_create(parent);
    lv_label_set_text(lbl_z, "Z=+000.000");
    lv_obj_set_style_text_font(lbl_z, &lv_font_roboto_mono_40, 0);
    lv_obj_align_to(lbl_z, lbl_y, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);

    // --- Coordinate-system label (smaller font) ---
    lbl_cs = lv_label_create(parent);
    lv_label_set_text(lbl_cs, "G53");
    lv_obj_set_style_text_font(lbl_cs, &lv_font_montserrat_24, 0);
    lv_obj_align(lbl_cs, LV_ALIGN_TOP_RIGHT, -6, 14);
}
