#include "pins.h"
#include "input_events.h"
#include "input_manager.h"
#include <Arduino.h>

#define EMULATE_ENCODER 0 // 1 = simulate encoder, 0 = use real hardware

namespace InputManager
{
    // ---------------------------------------------------------------------
    // Configuration
    // ---------------------------------------------------------------------
    constexpr uint32_t DEBOUNCE_US = 700; // µs debounce threshold
    volatile int8_t step_dir = 0;
    volatile uint32_t last_us = 0;

    static InputEventCallback _callback = nullptr;

    // ---------------------------------------------------------------------
    // ISR: one pulse per detent, direction from phase relation
    // ---------------------------------------------------------------------
    void IRAM_ATTR handle_encoder_change()
    {
        uint32_t now = micros();
        if (now - last_us < DEBOUNCE_US)
            return;
        last_us = now;

        bool a = digitalRead(encoder_pins[0]);
        bool b = digitalRead(encoder_pins[1]);

        // determine direction: if A==B → clockwise; else counter-clockwise
        step_dir = (a == b) ? +1 : -1;
    }

    // ---------------------------------------------------------------------
    // Initialize encoder hardware
    // ---------------------------------------------------------------------
    void begin_encoder(InputEventCallback cb)
    {
        _callback = cb;

        for (int i = 0; i < NUM_ENCODER_PINS; ++i)
            pinMode(encoder_pins[i], INPUT_PULLUP);

        // attach ISR only to channel A
        attachInterrupt(digitalPinToInterrupt(encoder_pins[0]), handle_encoder_change, CHANGE);

        Serial.println("✅ Encoder initialized (1 pulse per detent)");
    }

    // ---------------------------------------------------------------------
    // Polling: emit events to application
    // ---------------------------------------------------------------------
    void poll_encoder()
    {
        if (EMULATE_ENCODER)
        {
            static int dir = 1;
            static uint32_t last_emul = 0;

            if (_callback && millis() - last_emul > 5000)
            {
                last_emul = millis();
                InputEvent e;
                e.type = InputType::Encoder;
                e.id = 0;
                e.event = (dir > 0 ? EventType::Increment : EventType::Decrement);
                e.value = dir;
                e.timestamp = millis();
                _callback(e);
                dir = -dir;
            }
            return;
        }

        int8_t dir = step_dir; // consume direction set by ISR
        if (dir != 0)
        {
            step_dir = 0; // reset flag

            if (_callback)
            {
                InputEvent e;
                e.type = InputType::Encoder;
                e.id = 0;
                e.event = (dir > 0 ? EventType::Increment : EventType::Decrement);
                e.value = dir;
                e.timestamp = millis();
                _callback(e);
            }
        }
    }

} // namespace InputManager
