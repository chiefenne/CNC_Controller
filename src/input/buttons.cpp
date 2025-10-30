#include "pins.h"
#include "input_manager.h"

namespace
{
    constexpr bool EMULATE_BUTTONS = true; // set false for real hardware
    constexpr uint32_t DEBOUNCE_MS = 50; // standard button debounce
    uint32_t last_press[NUM_BUTTON_PINS] = {0};
    bool last_state[NUM_BUTTON_PINS] = {true}; // true = released (pull-up active)
    InputEventCallback _callback = nullptr;
    uint32_t last_emul = 0;
}

namespace InputManager
{

    void begin_buttons(InputEventCallback cb)
    {
        _callback = cb;
        for (int i = 0; i < NUM_BUTTON_PINS; ++i)
        {
            pinMode(button_pins[i], INPUT_PULLUP);
        }
        Serial.println("✅ Buttons initialized");
    }

    void poll_buttons()
    {
        if (EMULATE_BUTTONS)
        {
            // --- emulate button presses every few s ---
            if (millis() - last_emul > 8000)
            {
                last_emul = millis();
                static int pos = 0;
                pos = (pos + 1) % NUM_BUTTON_PINS;

                if (_callback)
                {
                    InputEvent e;
                    e.type = InputType::Button;
                    e.id = 0;
                    e.event = EventType::PositionChange;
                    e.value = pos; // 0..NUM_BUTTON_PINS-1
                    e.timestamp = millis();
                    _callback(e);
                }
            }
            return;
        }

        for (int i = 0; i < NUM_BUTTON_PINS; ++i)
        {
            bool pressed = (digitalRead(button_pins[i]) == LOW);
            uint32_t now = millis();

            if (pressed != last_state[i] && (now - last_press[i]) > DEBOUNCE_MS)
            {
                last_press[i] = now;
                last_state[i] = pressed;

                if (_callback)
                {
                    InputEvent e;
                    e.type = InputType::Button;
                    e.id = i;
                    e.event = pressed ? EventType::Pressed : EventType::Released;
                    e.value = pressed ? 1 : 0;
                    e.timestamp = now;
                    _callback(e);
                }
            }
        }
    }

} // namespace InputManager
