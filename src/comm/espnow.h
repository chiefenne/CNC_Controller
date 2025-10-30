#pragma once
#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>
#include "payload.h"   // <- include your struct definition

namespace ESPNOW
{
    void init();
    bool send(const uint8_t *data, size_t len);
    bool send(const String &msg);
    bool send(const Payload &payload);   // 👈 new overload
    void printMac();
}
