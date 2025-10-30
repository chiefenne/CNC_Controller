#include "input_manager.h"
#include "pins.h"

namespace
{
    // --- Configuration ---
    constexpr bool EMULATE_SELECTOR = true; // set false for real hardware
    constexpr uint32_t DEBOUNCE_MS = 100;   // stable for 100 ms before change

    // --- State ---
    int current_pos = -1;
    int stable_pos = -1;
    uint32_t last_change_time = 0;
    InputEventCallback _callback = nullptr;
    uint32_t last_emul = 0;
}

namespace InputManager
{

    void begin_selector(InputEventCallback cb)
    {
        _callback = cb;

        for (int i = 0; i < NUM_SELECTOR_PINS; ++i)
            pinMode(selector_pins[i], INPUT_PULLUP);

        // initialize current position
        for (int i = 0; i < NUM_SELECTOR_PINS; ++i)
            if (digitalRead(selector_pins[i]) == LOW)
                current_pos = stable_pos = i;

        Serial.println("✅ Selector initialized");
    }

    void poll_selector()
    {
        if (EMULATE_SELECTOR)
        {
            // --- emulate position cycling every 3 s ---
            if (millis() - last_emul > 10000)
            {
                last_emul = millis();
                static int pos = 0;
                pos = (pos + 1) % NUM_SELECTOR_PINS;

                if (_callback)
                {
                    InputEvent e;
                    e.type = InputType::Selector;
                    e.id = 0;
                    e.event = EventType::PositionChange;
                    e.value = pos; // 0..NUM_SELECTOR_PINS-1
                    e.timestamp = millis();
                    _callback(e);
                }
            }
            return;
        }

        // --- real hardware reading ---
        int read_pos = -1;
        for (int i = 0; i < NUM_SELECTOR_PINS; ++i)
        {
            if (digitalRead(selector_pins[i]) == LOW)
            {
                read_pos = i;
                break;
            }
        }

        if (read_pos != current_pos)
        {
            current_pos = read_pos;
            last_change_time = millis();
        }

        // debounce
        if (read_pos != -1 && (millis() - last_change_time) > DEBOUNCE_MS && read_pos != stable_pos)
        {
            stable_pos = read_pos;
            if (_callback)
            {
                InputEvent e;
                e.type = InputType::Selector;
                e.id = 0;
                e.event = EventType::PositionChange;
                e.value = stable_pos;
                e.timestamp = millis();
                _callback(e);
            }
        }
    }

} // namespace InputManager
