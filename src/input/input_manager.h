#pragma once
#include "input_events.h"


namespace InputManager {

// --- Encoder interface ---
void begin_encoder(InputEventCallback cb);                       // Init encoder hardware (or emulated)
void poll_encoder();                        // Called each loop iteration

// --- Selector interface ---
void begin_selector(InputEventCallback cb);  // Init selector switch
void poll_selector();                         // Poll selector state

}  // namespace InputManager
