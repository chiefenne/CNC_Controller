#pragma once
#include <Arduino.h>
#include "../src/input/input_events.h"

enum class MsgType : uint8_t {
    Button = 0,
    Encoder,
    Selector
};

struct Payload {
    uint8_t src_id;     // e.g., board or sender ID
    MsgType msg_type;   // what kind of data
    uint8_t target;     // axis, feed, etc.
    int16_t value;      // delta or absolute
};

// Helper for creating a payload from an input event
inline Payload make_payload_from_event(const InputEvent& e, uint8_t src_id = 1) {
    Payload p;
    p.src_id = src_id;
    switch (e.type) {
        case InputType::Encoder:  p.msg_type = MsgType::Encoder;  break;
        case InputType::Selector: p.msg_type = MsgType::Selector; break;
        case InputType::Button:   p.msg_type = MsgType::Button;   break;
    }
    p.target = e.id;
    p.value  = e.value;
    return p;
}
