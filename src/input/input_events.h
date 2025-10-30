#pragma once
#include <Arduino.h>

enum class InputType : uint8_t {
    Encoder = 0,
    Selector,
    Button
};

enum class EventType : uint8_t {
    Pressed = 0,
    Released,
    Increment,
    Decrement,
    PositionChange
};

struct InputEvent {
    InputType type;
    uint8_t id;           // Which encoder, selector, or button
    EventType event;      // The logical action
    int16_t value;        // Optional: delta or absolute pos
    uint32_t timestamp;   // millis() when created
};

// Callback signature used by the input system
using InputEventCallback = void (*)(const InputEvent&);
