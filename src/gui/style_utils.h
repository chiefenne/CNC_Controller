#pragma once

#include <lvgl.h>

// Helper to flatten an LVGL container: no radius, border, or padding.
inline void gui_set_style_flat(lv_obj_t *obj, lv_style_selector_t selector = 0)
{
    lv_obj_set_style_radius(obj, 0, selector);
    lv_obj_set_style_border_width(obj, 0, selector);
    lv_obj_set_style_pad_all(obj, 0, selector);
}
