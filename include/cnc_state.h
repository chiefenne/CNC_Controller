#pragma once

// Core control modes determined by selector
enum class ControlMode : uint8_t {
    MoveX = 0,
    MoveY,
    MoveZ,
    SelectCoordSystem,
    Reserved
};

// Holds CNC-relevant state for GUI and control
struct CncState {
    ControlMode mode = ControlMode::MoveX;   // default
    int active_cs = 0;                       // 0 = G53
    float machine_pos[3] = {0.0f, 0.0f, 0.0f};
    float offsets[6][3] = {};                // G54–G59 offsets
};
