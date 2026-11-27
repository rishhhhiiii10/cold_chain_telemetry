#pragma once
#include "types.h"

class WiFiUplink {
public:
    bool begin(const char* ssid, const char* password);
    bool isConnected();

    bool postRealtime(const TelemetryPacket& packet, uint8_t seq_counter);
};
