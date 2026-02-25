#pragma once
#include <cstdint>

class Protocol {
public:
    static void sendPacket(const char *data, uint8_t payloadLen);
};