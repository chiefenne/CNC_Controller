#pragma once

// --- GPIO assignments (ESP32-S3 DevKitC-1) ---

// --- Encoder pins ---
constexpr int NUM_ENCODER_PINS = 2;
constexpr int encoder_pins[] = {41, 42};

// --- Selector pins ---
constexpr int NUM_SELECTOR_PINS = 3;
constexpr int selector_pins[] = {38, 39, 40};

// --- Button pins ---
constexpr int NUM_BUTTON_PINS = 5;                   // adjust count
constexpr int button_pins[NUM_BUTTON_PINS] = {1, 2, 3, 4, 5};  // example GPIOs
´