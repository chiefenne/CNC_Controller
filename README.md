# CNC Controller GUI

ESP32-S3 powered touchscreen interface for the CNC wireless keypad project. The firmware is built with PlatformIO, the Arduino framework, the TFT_eSPI driver, and LVGL for UI composition.
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

## License

This project is licensed under the [MIT License](LICENSE).
