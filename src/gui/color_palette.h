#pragma once

#include <stdint.h>

// Shades and tints around the primary hue 0x2986CC, kept for future styling tweaks.
static const uint32_t GUI_COLOR_SHADES[] = {
    0x2986CC,
    0x2478B7,
    0x206BA3,
    0x1C5D8E,
    0x18507A,
    0x144366,
    0x103551,
    0x0C283D,
    0x081A28,
    0x040D14,
    0x000000
};

static const uint32_t GUI_COLOR_TINTS[] = {
    0x2986CC,
    0x3E92D1,
    0x539ED6,
    0x69AADB,
    0x7EB6E0,
    0x94C2E5,
    0xA9CEEA,
    0xBEDAEF,
    0xD4E6F4,
    0xE9F2F9,
    0xFFFFFF
};

// Grayscale equivalents for neutral UI elements.
static const uint32_t GUI_COLOR_GRAYS[] = {
    0x808080,
    0x8A8A8A,
    0x949494,
    0x9E9E9E,
    0xA8A8A8,
    0xB2B2B2,
    0xBCBCBC,
    0xC6C6C6,
    0xD0D0D0,
    0xDADADA,
    0xFFFFFF
};

// Monochromatic color picks currently in use for structural elements.
static const uint32_t GUI_COLOR_MONO[] = {
    0x1C5C8C, // header / strong accent
    0x2986CC, // primary hue
    0x3892D7, // shadow accent
    0x4D9EDB, // inactive tab
    0x62AADF  // surface background
};

// Triadic accents used for interaction states.
static const uint32_t GUI_COLOR_TRIAD[] = {
    0xCC2986, // pressed tab feedback
    0x86CC29  // active tab highlight
};
