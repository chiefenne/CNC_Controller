#include "page_home.h" // already brings lvgl.h in
#include "fonts.h"
#include "cnc_state.h"
#include <cstdio>

extern CncState cnc;

lv_obj_t *lbl_x;
lv_obj_t *lbl_y;
lv_obj_t *lbl_z;
lv_obj_t *lbl_cs;

void page_home_create(lv_obj_t *parent)
{
    char buf[16];

    // --- X coordinate ---
    lbl_x = lv_label_create(parent);
    snprintf(buf, sizeof(buf), "X=%7.3f", cnc.machine_pos[0]);
    lv_label_set_text(lbl_x, buf);
    lv_obj_set_style_text_font(lbl_x, &lv_font_roboto_mono_40, 0);
    lv_obj_align(lbl_x, LV_ALIGN_TOP_MID, -24, 14);

    // --- Y coordinate ---
    lbl_y = lv_label_create(parent);
    snprintf(buf, sizeof(buf), "Y=%7.3f", cnc.machine_pos[1]);
    lv_label_set_text(lbl_y, buf);
    lv_obj_set_style_text_font(lbl_y, &lv_font_roboto_mono_40, 0);
    lv_obj_align_to(lbl_y, lbl_x, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);

    // --- Z coordinate ---
    lbl_z = lv_label_create(parent);
    snprintf(buf, sizeof(buf), "Z=%7.3f", cnc.machine_pos[2]);
    lv_label_set_text(lbl_z, buf);
    lv_obj_set_style_text_font(lbl_z, &lv_font_roboto_mono_40, 0);
    lv_obj_align_to(lbl_z, lbl_y, LV_ALIGN_OUT_BOTTOM_MID, 0, 5);

    // --- Coordinate-system label (smaller font) ---
    lbl_cs = lv_label_create(parent);
    lv_label_set_text_fmt(lbl_cs, "G5%d", 3 + cnc.active_cs);
    lv_obj_set_style_text_font(lbl_cs, &lv_font_montserrat_24, 0);
    lv_obj_align(lbl_cs, LV_ALIGN_TOP_RIGHT, -6, 14);
}

void page_home_update()
{
    char buf[16];

    snprintf(buf, sizeof(buf), "X=%7.3f", cnc.machine_pos[0]);
    lv_label_set_text(lbl_x, buf);

    snprintf(buf, sizeof(buf), "Y=%7.3f", cnc.machine_pos[1]);
    lv_label_set_text(lbl_y, buf);

    snprintf(buf, sizeof(buf), "Z=%7.3f", cnc.machine_pos[2]);
    lv_label_set_text(lbl_z, buf);

    snprintf(buf, sizeof(buf), "G%d", 53 + cnc.active_cs);
    lv_label_set_text(lbl_cs, buf);
}
