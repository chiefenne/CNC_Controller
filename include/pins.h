#pragma once

// --- GPIO assignments (ESP32-S3 DevKitC-1) ---
// defined in TFT_eSPI/User_Setups/Setup70b_ESP32_S3_ILI9341.h
// TFT_MOSI 11
// TFT_MISO 13
// TFT_SCLK 12
// TFT_CS   10
// TFT_DC   8
// TFT_RST  9
// TOUCH_CS 14
// TOUCH_IRQ 7

// --- Encoder pins ---
constexpr int NUM_ENCODER_PINS = 2;
constexpr int encoder_pins[] = {41, 42};

// --- Selector pins ---
constexpr int NUM_SELECTOR_PINS = 3;
constexpr int selector_pins[] = {38, 39, 40};

// --- Button pins ---
constexpr int NUM_BUTTON_PINS = 5;
constexpr int button_pins[NUM_BUTTON_PINS] = {19, 20, 21, 39, 40};
