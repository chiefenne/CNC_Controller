# CNC Controller GUI

ESP32-S3 powered touchscreen interface for my CNC wireless keypad project. The firmware is built with PlatformIO, the Arduino framework, the TFT_eSPI driver, and LVGL for UI composition.
Developed on ESP32-S3-DevKitC-1 with integrated USB and WiFi capabilities for seamless development and wireless communication.

## Features

- 320x240 ILI9341 display with LVGL-driven tabbed interface
- Responsive touch handling (XPT2046)
- Multiple input devices: rotary encoder, switch selector, and momentary buttons
- ESP-NOW wireless communication for real-time CNC data exchange
- Bidirectional command/status protocol with automatic device pairing
  - Receiver acts as a HID keybaord device (not in this repo)
- PlatformIO multi-environment setup (main firmware + touch calibration)

## Repository Layout

```text
CNC Controller/
├── include/          # Headers shared across components
├── lib/              # Optional PlatformIO libraries
├── src/              # Application sources (GUI, comms)
├── touch_calibration/# Calibration sketch used for TFT_eSPI touch setup
├── platformio.ini    # Environment configuration
└── README.md

## Hardware Gallery

| Breadboard Prototype | Touchscreen Close-up |
| --- | --- |
| _Add image: docs/images/breadboard_setup.jpg | _Add image: docs/images/touch_display_closeup.jpg |
```

## Development

1. Install [PlatformIO](https://platformio.org/install/ide?install=vscode) for VS Code.
2. Clone the repository and open the workspace folder `CNC Controller` in VS Code.
3. Use the **PlatformIO** sidebar to build or upload the `esp32-s3-devkitc-1` environment.
4. For touch calibration, run the `touch-calibration` environment by modifying `platformio.ini`:

    ```ini
    [platformio]
    ; default_envs = esp32-s3-devkitc-1
    ; src_dir = src
    default_envs = touch-calibration
    src_dir = touch_calibration
    ```

    Comment/uncomment the appropriate lines to switch between environments.

## Configuration

The project requires specific TFT_eSPI library modifications in the `.pio/libdeps` directory. There are several ways to do that. Here is how I did it (fits of course to the wireing I used between ESP32 and display):

### Display Driver Setup

Activate exactly one `#define` per the following files.

- **`esp32-s3-devkitc-1/TFT_eSPI/User_Setup.h`**
    ```c
    #define ILI9341_DRIVER
    ```

- **`esp32-s3-devkitc-1/TFT_eSPI/User_Setup_Select.h`**

    ```c
    #include <User_Setups/Setup70b_ESP32_S3_ILI9341.h>
    ```

### Hardware Pin Configuration

- **`esp32-s3-devkitc-1/TFT_eSPI/User_Setups/Setup70b_ESP32_S3_ILI9341.h`**

    Modified pin assignments for custom hardware:

    ```c
    #define TFT_MOSI 11
    #define TFT_MISO 13
    #define TFT_SCLK 12
    #define TFT_CS   10
    #define TFT_DC   8
    #define TFT_RST  9

    #define TOUCH_CS 14
    #define TOUCH_IRQ 7
    #define TOUCH_XPT2046
    ```


### ESP32-S3 Processor Fix

- **`esp32-s3-devkitc-1/TFT_eSPI/Processors/TFT_eSPI_ESP32_S3.h`**

    Add the following line before `#ifndef REG_SPI_BASE` to prevent execution corruption (that fix may not be permanently needed as the libs will be further developed):

    ```c
    #undef REG_SPI_BASE
    ```

> **Note:** These modifications are applied to the PlatformIO library dependencies and may need to be reapplied if libraries are updated.


## License

This project is licensed under the [MIT License](LICENSE).
