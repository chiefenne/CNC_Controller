#include "espnow.h"
#include <esp_err.h>

namespace ESPNOW
{
    static bool initialized = false;
    static bool tx_enabled = false; // 🚦 controls whether to send via radio

    static uint8_t receiver_mac[6] = {0x24, 0x58, 0x7C, 0xE4, 0x0F, 0x60};
    constexpr size_t MAX_PAYLOAD = 250; // ESP-NOW limit
    static uint8_t tx_buffer[MAX_PAYLOAD];
    static size_t tx_len = 0;

    // ------------------------------------------------------------
    // Callback: send result
    // ------------------------------------------------------------
    static void onSend(const uint8_t *mac_addr, esp_now_send_status_t status)
    {
        Serial.printf("📤 ESP-NOW unicast %s → %02X:%02X:%02X:%02X:%02X:%02X\n",
                      (status == ESP_NOW_SEND_SUCCESS ? "OK" : "FAIL"),
                      mac_addr[0], mac_addr[1], mac_addr[2],
                      mac_addr[3], mac_addr[4], mac_addr[5]);
        if (tx_len > 0)
        {
            Serial.print("    Sent payload: ");
            for (size_t i = 0; i < tx_len; ++i)
            {
                Serial.printf("%02X", tx_buffer[i]);
                if (i + 1 < tx_len)
                    Serial.print(' ');
            }
            Serial.println();
            tx_len = 0;
        }
    }

    // ------------------------------------------------------------
    // Init ESP-NOW in unicast mode
    // ------------------------------------------------------------
    void init()
    {
        if (initialized)
            return;

        WiFi.mode(WIFI_STA);
        WiFi.disconnect();

        if (esp_now_init() != ESP_OK)
        {
            Serial.println("❌ ESP-NOW init failed");
            return;
        }

        esp_now_peer_info_t peerInfo = {};
        memcpy(peerInfo.peer_addr, receiver_mac, 6);
        peerInfo.channel = 0;
        peerInfo.encrypt = false;
        peerInfo.ifidx = WIFI_IF_STA;

        if (esp_now_add_peer(&peerInfo) != ESP_OK)
        {
            Serial.println("❌ Failed to add receiver peer");
            return;
        }

        esp_now_register_send_cb(onSend);
        initialized = true;

        Serial.println("✅ ESP-NOW unicast ready");
        printMac();
    }

    // ------------------------------------------------------------
    // Send bytes
    // ------------------------------------------------------------
    bool send(const uint8_t *data, size_t len)
    {
        if (!initialized)
        {
            Serial.println("⚠️ ESP-NOW send skipped: module not initialised");
            return false;
        }
        if (!data || len == 0)
        {
            Serial.println("⚠️ ESP-NOW send skipped: empty payload");
            return false;
        }
        if (len > MAX_PAYLOAD)
        {
            Serial.printf("❌ ESP-NOW payload too large (%u bytes, max %u)\n",
                          (unsigned)len, (unsigned)MAX_PAYLOAD);
            return false;
        }

        memcpy(tx_buffer, data, len);
        tx_len = len;

        Serial.print("📦 ESP-NOW payload: ");
        for (size_t i = 0; i < tx_len; ++i)
        {
            Serial.printf("%02X", tx_buffer[i]);
            if (i + 1 < tx_len)
                Serial.print(' ');
        }
        Serial.println();

        // debug: simulate send disabled
        if (!tx_enabled)
        {
            Serial.println("🚫 [Test Mode] ESP-NOW send skipped (radio disabled)");
            delay(5);
            return true;
        }

        esp_err_t result = esp_now_send(receiver_mac, tx_buffer, tx_len);
        if (result != ESP_OK)
        {
            Serial.printf("❌ esp_now_send failed: %s\n", esp_err_to_name(result));
            return false;
        }
        return true;
    }

    // ------------------------------------------------------------
    // Send string
    // ------------------------------------------------------------
    bool send(const String &msg)
    {
        return send((const uint8_t *)msg.c_str(), msg.length());
    }

    // ------------------------------------------------------------
    // Send structured payload (new overload)
    // ------------------------------------------------------------
    bool send(const Payload &payload)
    {
        return send(reinterpret_cast<const uint8_t *>(&payload), sizeof(payload));
    }

    // ------------------------------------------------------------
    // Print this device's MAC
    // ------------------------------------------------------------
    void printMac()
    {
        uint8_t mac[6];
        esp_read_mac(mac, ESP_MAC_WIFI_STA);
        Serial.printf("📡 My MAC: %02X:%02X:%02X:%02X:%02X:%02X\n",
                      mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    }
}
