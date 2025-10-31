#include <Arduino.h> // in case serial is needed later
#include "gui.h"
#include "fonts.h"
#include "pages/page_home.h"
#include "pages/page_data.h"
#include "pages/page_settings.h"
#include "style_utils.h"
#include "color_palette.h"

static lv_obj_t *header;
static lv_obj_t *footer;
static lv_obj_t *content;
static lv_obj_t *btn_tabs[PAGE_COUNT];
static GuiPage active_page = PAGE_HOME;

// --- Layout constants ---
static const lv_coord_t FOOTER_HEIGHT = 36; // easily adjustable later
static const lv_coord_t HEADER_HEIGHT = 36; // optional consistency

// --- Color palette (monochromatic + triadic around 0x2986CC) ---
static const lv_color_t COLOR_HEADER = lv_color_hex(0x1C5C8C);
static const lv_color_t COLOR_SURFACE = lv_color_hex(0xE9F2F9);
static const lv_color_t COLOR_TAB_INACTIVE = lv_color_hex(0xD0D0D0);
static const lv_color_t COLOR_TAB_SHADOW = lv_color_hex(0x3892D7);
static const lv_color_t COLOR_TAB_PRESSED = lv_color_hex(0xCC2986);   // triadic accent
static const lv_color_t COLOR_TAB_ACTIVE = lv_color_hex(0x86CC29);    // triadic accent

// Forward declarations
static void create_header();
static void create_footer();
static void show_page(GuiPage p);
static void tab_event_cb(lv_event_t *e);

void gui_init()
{
    lv_obj_t *scr = lv_screen_active();
    lv_obj_set_style_bg_color(scr, COLOR_SURFACE, 0);

    create_header();
    create_footer();

    content = lv_obj_create(scr);
    lv_obj_set_size(content, SCREEN_WIDTH, SCREEN_HEIGHT - HEADER_HEIGHT - FOOTER_HEIGHT);
    lv_obj_set_pos(content, 0, HEADER_HEIGHT);
    lv_obj_set_style_bg_color(content, COLOR_SURFACE, 0);
    lv_obj_set_scrollbar_mode(content, LV_SCROLLBAR_MODE_OFF);
    gui_set_style_flat(content);

    show_page(PAGE_HOME);
}

void gui_set_page(GuiPage p)
{
    if (p == active_page)
        return;
    active_page = p;
    show_page(p);
}

// -----------------------------------------------------------------------------
// Header
// -----------------------------------------------------------------------------
static void create_header()
{
    header = lv_obj_create(lv_screen_active());
    lv_obj_set_size(header, TFT_HEIGHT, HEADER_HEIGHT);
    lv_obj_align(header, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_set_style_bg_color(header, COLOR_HEADER, 0);
    gui_set_style_flat(header);
    lv_obj_clear_flag(header, LV_OBJ_FLAG_SCROLLABLE);

    // --- Title ---
    lv_obj_t *title = lv_label_create(header);
    lv_label_set_text(title, "CNC CONTROLLER");
    lv_obj_set_style_text_color(title, lv_color_white(), 0);
    lv_obj_set_style_text_font(title, FONT_HEADER, 0);
    lv_obj_align(title, LV_ALIGN_CENTER, 6, 0);

    // --- Status text ---
    lv_obj_t *status = lv_label_create(header);
    static const char *version = "v1.0";
    lv_label_set_text_fmt(status, "%s", version);
    lv_obj_set_style_text_color(status, lv_color_white(), 0);
    lv_obj_set_style_text_font(status, &lv_font_montserrat_14, 0);
    lv_obj_align(status, LV_ALIGN_RIGHT_MID, -6, 0);
}

// -----------------------------------------------------------------------------
// Footer / navigation tabs
// -----------------------------------------------------------------------------
static void create_footer()
{
    footer = lv_obj_create(lv_screen_active());
    lv_obj_set_size(footer, LV_PCT(100), FOOTER_HEIGHT);
    lv_obj_align(footer, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_set_style_bg_color(footer, COLOR_SURFACE, 0);
    gui_set_style_flat(footer);
    lv_obj_clear_flag(footer, LV_OBJ_FLAG_SCROLLABLE);

    lv_obj_set_flex_flow(footer, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(footer,
                          LV_FLEX_ALIGN_SPACE_EVENLY,
                          LV_FLEX_ALIGN_CENTER,
                          LV_FLEX_ALIGN_CENTER);

    const char *names[PAGE_COUNT] = {"Home", "Data", "Settings"};

    lv_coord_t footer_h = FOOTER_HEIGHT;
    lv_coord_t btn_h = footer_h * 90 / 100;
    lv_coord_t margin_v = (footer_h - btn_h) / 2;

    // use our configured footer font
    const lv_font_t *font = FONT_FOOTER;

    for (int i = 0; i < PAGE_COUNT; ++i)
    {
        const char *txt = names[i];
        lv_point_t txt_size;
        lv_txt_get_size(&txt_size, txt, font, 0, 0, LV_COORD_MAX, LV_TEXT_FLAG_NONE);

        lv_coord_t btn_w = txt_size.x + (btn_h * 0.6);

        btn_tabs[i] = lv_button_create(footer);
        lv_obj_set_size(btn_tabs[i], btn_w, btn_h);
        lv_obj_set_style_margin_top(btn_tabs[i], margin_v, 0);
        lv_obj_set_style_margin_bottom(btn_tabs[i], margin_v, 0);
        lv_obj_set_style_radius(btn_tabs[i], btn_h / 3, 0);
        lv_obj_add_event_cb(btn_tabs[i], tab_event_cb, LV_EVENT_CLICKED, (void *)(uintptr_t)i);
        lv_obj_set_style_bg_color(btn_tabs[i], COLOR_TAB_INACTIVE, 0);
        lv_obj_set_style_bg_color(btn_tabs[i], COLOR_TAB_PRESSED, LV_STATE_PRESSED);
    lv_obj_set_style_text_color(btn_tabs[i], lv_color_black(), 0);

        lv_obj_t *label = lv_label_create(btn_tabs[i]);
        lv_label_set_text(label, txt);
        lv_obj_set_style_text_font(label, font, 0);
        lv_obj_center(label);
    }
}

// -----------------------------------------------------------------------------
// Page switching
// -----------------------------------------------------------------------------
static void show_page(GuiPage p)
{
    lv_obj_clean(content);

    switch (p)
    {
    case PAGE_HOME:
        page_home_create(content);
        break;
    case PAGE_DATA:
        page_data_create(content);
        break;
    case PAGE_SETTINGS:
        page_settings_create(content);
        break;
    default:
        break;
    }

    // Update tab highlight
    for (int i = 0; i < PAGE_COUNT; ++i)
    {
        bool is_active = (i == p);
    lv_obj_set_style_bg_color(btn_tabs[i], is_active ? COLOR_TAB_ACTIVE : COLOR_TAB_INACTIVE, 0);
    lv_obj_set_style_text_color(btn_tabs[i], lv_color_black(), 0);
        lv_obj_set_style_shadow_width(btn_tabs[i], is_active ? 6 : 0, 0);
        lv_obj_set_style_shadow_color(btn_tabs[i], is_active ? COLOR_TAB_SHADOW : COLOR_TAB_INACTIVE, 0);
    }
}

// -----------------------------------------------------------------------------
// Tab button callback
// -----------------------------------------------------------------------------
static void tab_event_cb(lv_event_t *e)
{
    int id = (int)(uintptr_t)lv_event_get_user_data(e);
    gui_set_page((GuiPage)id);
}
