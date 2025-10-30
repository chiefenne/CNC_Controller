#pragma once
#include <lvgl.h>

enum GuiPage {
  PAGE_HOME = 0,
  PAGE_DATA,
  PAGE_SETTINGS,
  PAGE_COUNT
};

void gui_init();
void gui_set_page(GuiPage page);
