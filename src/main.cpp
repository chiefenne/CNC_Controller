#include <Arduino.h>
#include <TFT_eSPI.h>
#include <lvgl.h>
#include "gui/gui.h"
#include "input/input_manager.h"
#include "comm/espnow.h"
#include "payload.h"
#include "cnc_state.h"
#include "gui/pages/page_home.h"

// TFT instance
TFT_eSPI tft = TFT_eSPI();

// Initialize global CNC state
#include "cnc_state.h"

CncState cnc;

// LVGL buffers
static lv_color_t buf1[SCREEN_WIDTH * 10];
static lv_draw_buf_t draw_buf;
static lv_display_t *display;
static lv_indev_t *indev;

// forward declarations
void my_disp_flush(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map);
void my_touch_read(lv_indev_t *drv, lv_indev_data_t *data);
void handle_input_event(const InputEvent &e);

static const char *input_type_to_str(InputType type)
{
    switch (type)
    {
    case InputType::Encoder:
        return "Encoder";
    case InputType::Selector:
        return "Selector";
    case InputType::Button:
        return "Button";
    default:
        return "Unknown";
    }
}


void setup()
{
    Serial.begin(115200);
    delay(50);

    tft.init();
    tft.setRotation(1);

    // Touch calibration from your working TFT version
    uint16_t calData[5] = {248, 3362, 355, 3442, 1};
    tft.setTouch(calData);

    lv_init();

    // --- LVGL 9.3 API ---
    static lv_color_t buf1[SCREEN_WIDTH * 10]; // 10-line draw buffer

    lv_display_t *disp = lv_display_create(SCREEN_WIDTH, SCREEN_HEIGHT);
    lv_display_set_flush_cb(disp, my_disp_flush);

    // Assign draw buffer (replaces old lv_draw_buf_init + set_draw_buffers)
    lv_display_set_buffers(disp, buf1, NULL, sizeof(buf1),
                           LV_DISPLAY_RENDER_MODE_PARTIAL);

    lv_display_set_default(disp);

    // Quick sanity label to prove rendering works
    lv_obj_t *label = lv_label_create(lv_screen_active());
    lv_label_set_text(label, "LVGL 9.3 OK");
    lv_obj_center(label);

    // Touch input
    indev = lv_indev_create();
    lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(indev, my_touch_read);

    // Initialize GUI
    gui_init();

    // Initialize ESP-NOW
    ESPNOW::init();

    // Initialize input devices
    InputManager::begin_encoder(handle_input_event);
    InputManager::begin_selector(handle_input_event);
    InputManager::begin_buttons(handle_input_event);

    // Set initial CNC mode fixed for testing
    Serial.print("Initial selector control mode: ");
    Serial.println(static_cast<int>(cnc.mode));

    Serial.println("Setup complete");
    Serial.println("Wireless CNC Controller starting...");
}

void loop()
{

    // Read inputs
    InputManager::poll_encoder();
    // InputManager::poll_selector();
    InputManager::poll_buttons();

    lv_tick_inc(5);      // tell LVGL how much time has passed
    lv_timer_handler();  // let the GUI do its work
    delay(5);            // let this time pass

}

// --- LVGL callbacks ---
void my_disp_flush(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map)
{
    uint32_t w = area->x2 - area->x1 + 1;
    uint32_t h = area->y2 - area->y1 + 1;
    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, w, h);
    tft.pushColors((uint16_t *)px_map, w * h, true);
    tft.endWrite();
    lv_display_flush_ready(disp);
}

void my_touch_read(lv_indev_t *drv, lv_indev_data_t *data)
{
    uint16_t x, y;
    bool touched = tft.getTouch(&x, &y);

    if (touched)
    {
        // Touch position debug
        // Serial.printf("Touch: %u, %u\n", x, y);
        data->point.x = x;
        data->point.y = y;
        data->state = LV_INDEV_STATE_PRESSED;
    }
    else
    {
        data->state = LV_INDEV_STATE_RELEASED;
    }
}

// Handle input events
void handle_input_event(const InputEvent &e)
{

    // --- Interpret selector position for CNC mode ---
    if (e.type == InputType::Selector && e.event == EventType::PositionChange)
    {
        switch (e.value)
        {
        case 0:
            cnc.mode = ControlMode::MoveX;
            break;
        case 1:
            cnc.mode = ControlMode::MoveY;
            break;
        case 2:
            cnc.mode = ControlMode::MoveZ;
            break;
        case 3:
            cnc.mode = ControlMode::SelectCoordSystem;
            break;
        case 4:
            cnc.mode = ControlMode::Reserved;
            break;
        default:
            break;
        }

        Serial.print("→ Control mode set to: ");
        Serial.println(static_cast<int>(cnc.mode));
    }

    // --- Handle encoder events based on current mode ---
    if (e.type == InputType::Encoder)
    {
        if (e.event == EventType::Increment || e.event == EventType::Decrement)
        {
            int dir = (e.event == EventType::Increment) ? +1 : -1;
            constexpr float JOG_STEP = 1.0f;

            switch (cnc.mode)
            {
                case ControlMode::MoveX:
                    cnc.machine_pos[0] += dir * JOG_STEP;
                    Serial.printf("Encoder → move X axis (%+d) | X: %.3f\n", dir, cnc.machine_pos[0]);
                    page_home_update();
                    break;

                    case ControlMode::MoveY:
                    cnc.machine_pos[1] += dir * JOG_STEP;
                    Serial.printf("Encoder → move Y axis (%+d) | Y: %.3f\n", dir, cnc.machine_pos[1]);
                    page_home_update();
                    break;

                    case ControlMode::MoveZ:
                    cnc.machine_pos[2] += dir * JOG_STEP;
                    Serial.printf("Encoder → move Z axis (%+d) | Z: %.3f\n", dir, cnc.machine_pos[2]);
                    page_home_update();
                    break;

                    case ControlMode::SelectCoordSystem:
                    cnc.active_cs = max(0, min(cnc.active_cs + dir, 5));
                    Serial.printf("Encoder → cycle coordinate system (%+d) | G5%d\n", dir, 3 + cnc.active_cs);
                    page_home_update();
                    break;

                case ControlMode::Reserved:
                    Serial.printf("Encoder → reserved action (%+d)\n", dir);
                    break;
            }
        }
    }

    // Send via ESP-NOW (enable or disable in espnow.cpp using tx_enabled flag)
    Payload p = make_payload_from_event(e);
    ESPNOW::send(p);
}
