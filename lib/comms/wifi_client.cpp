#include "wifi_client.h"
#include "secrets.h"
#include <WiFi.h>
#include <esp_wifi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <math.h>

bool WiFiUplink::begin(const char* ssid, const char* password) {
    WiFi.mode(WIFI_STA);
#ifdef WIFI_COUNTRY_CODE
    const esp_err_t country_status = esp_wifi_set_country_code(WIFI_COUNTRY_CODE, false);
    if (country_status != ESP_OK) {
        Serial.printf("[WiFi] Country configuration failed: %s\n", esp_err_to_name(country_status));
    }
#endif
    WiFi.onEvent([](arduino_event_id_t event, arduino_event_info_t info) {
        if (event == ARDUINO_EVENT_WIFI_STA_DISCONNECTED) {
            Serial.printf("[WiFi] Disconnected, reason %u\n", info.wifi_sta_disconnected.reason);
        } else if (event == ARDUINO_EVENT_WIFI_STA_GOT_IP) {
            Serial.printf("[WiFi] Connected, IP %s\n", WiFi.localIP().toString().c_str());
        }
    });
    WiFi.setAutoReconnect(true);
    WiFi.begin(ssid, password);
    return true;
}

bool WiFiUplink::isConnected() {
    return WiFi.status() == WL_CONNECTED;
}

bool WiFiUplink::postRealtime(const TelemetryPacket& packet, uint8_t seq_counter) {
    if (!isConnected() || !isfinite(packet.metrics.temperature) ||
        !isfinite(packet.metrics.vibration_rms) || packet.metrics.vibration_rms < 0) {
        return false;
    }

    // The backend assigns the receipt time and resolves the registered node ID.
    JsonDocument doc;
    doc["node_id"] = packet.node_id;
    doc["seq_counter"] = seq_counter;
    doc["temperature_c"] = packet.metrics.temperature;
    doc["vibration_rms"] = packet.metrics.vibration_rms;

    String payload;
    serializeJson(doc, payload);

    HTTPClient http;
    http.setConnectTimeout(3000);
    http.setTimeout(3000);
    if (!http.begin(TELEMETRY_API_URL)) return false;

    http.addHeader("Content-Type", "application/json");
    const int status = http.POST(payload);
    http.end();
    Serial.printf("[Telemetry] POST status: %d\n", status);
    return status >= 200 && status < 300;
}
