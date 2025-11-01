#include "input_manager.h"
#include "pins.h"

namespace
{
    // --- Configuration ---
    constexpr bool EMULATE_ENCODER = false; // set true for simulated steps
    constexpr uint32_t DEBOUNCE_US = 1500;  // ignore faster transitions (µs)

    // --- State ---
    volatile uint8_t last_state = 0;
    volatile int8_t step_dir = 0;
    volatile uint32_t last_step_time = 0;
    InputEventCallback _callback = nullptr;

    // --- Quadrature lookup table ---
    // index = old_state<<2 | new_state  →  +1 / -1 / 0
    const int8_t quad_table[16] = {
        0, -1, +1, 0,
        +1, 0, 0, -1,
        -1, 0, 0, +1,
        0, +1, -1, 0};

    volatile int8_t pending_dir = 0;

    // --- Interrupt handler ---
    // IRAM_ATTR: put code in Instruction RAM for faster execution, e.g., ISRs
    void IRAM_ATTR handle_encoder_change()
    {
        uint8_t a = digitalRead(encoder_pins[0]);
        uint8_t b = digitalRead(encoder_pins[1]);
        uint8_t new_state = (a << 1) | b;

        uint8_t index = (last_state << 2) | new_state;
        int8_t delta = quad_table[index]; // ✅ define delta here
        last_state = new_state;

        uint32_t now = micros(); // ✅ define now here
        if (delta != 0 && (now - last_step_time) > DEBOUNCE_US)
        {
            last_step_time = now;
            step_dir = delta; // just store direction
        }
    }

    // --- Emulation state ---
    uint32_t last_emul = 0;
}

namespace InputManager
{

    void begin_encoder(InputEventCallback cb)
    {
        _callback = cb;

        for (int i = 0; i < NUM_ENCODER_PINS; ++i)
            pinMode(encoder_pins[i], INPUT_PULLUP);

        last_state = (digitalRead(encoder_pins[0]) << 1) | digitalRead(encoder_pins[1]);
        attachInterrupt(digitalPinToInterrupt(encoder_pins[0]), handle_encoder_change, CHANGE);
        attachInterrupt(digitalPinToInterrupt(encoder_pins[1]), handle_encoder_change, CHANGE);
        Serial.println("✅ Encoder initialized with debounce");
    }

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
                dir = -dir; // alternate direction
            }
            return;
        }

        // --- real encoder: consume step_dir set by ISR ---
        static int8_t last_dir = 0;
        int8_t dir = step_dir; // copy volatile variable locally
        if (dir != 0 && dir != last_dir)
        {
            last_dir = dir;

            // build event outside interrupt
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

            step_dir = 0; // reset after handling
        }
    }

} // namespace InputManager
