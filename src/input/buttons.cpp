#include "pins.h"
#include "input_manager.h"

namespace
{
    constexpr uint32_t DEBOUNCE_MS = 50; // standard button debounce
    uint32_t last_press[NUM_BUTTON_PINS] = {0};
    bool last_state[NUM_BUTTON_PINS] = {true}; // true = released (pull-up active)
    InputEventCallback _callback = nullptr;
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
