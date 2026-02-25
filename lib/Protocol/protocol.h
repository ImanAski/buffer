#pragma once
#include <cstdint>

struct Packet {
  uint8_t type;
  uint8_t len;
  uint8_t payload[32];
};

class Protocol {
 public:
  static bool parseByte(uint8_t b, Packet &out);
  static bool takeParseError();
  static void sendPacket(const uint8_t *data, uint8_t payloadLen);
};
