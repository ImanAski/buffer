#include "protocol.h"

#include <Arduino.h>

namespace {
constexpr uint8_t kSof = 0xAA;
constexpr uint8_t kMaxPayload = 32;

enum class ParseState : uint8_t {
  WaitSof,
  ReadType,
  ReadLen,
  ReadPayload,
  ReadChecksum
};

ParseState s_state = ParseState::WaitSof;
Packet s_packet = {};
uint8_t s_index = 0;
uint8_t s_checksum = 0;
bool s_parse_error = false;

void reset_parser() {
  s_state = ParseState::WaitSof;
  s_packet = {};
  s_index = 0;
  s_checksum = 0;
}
}

bool Protocol::parseByte(uint8_t b, Packet &out) {
  switch (s_state) {
    case ParseState::WaitSof:
      if (b == kSof) {
        s_state = ParseState::ReadType;
      }
      break;

    case ParseState::ReadType:
      s_packet.type = b;
      s_checksum = b;
      s_state = ParseState::ReadLen;
      break;

    case ParseState::ReadLen:
      s_packet.len = b;
      s_checksum ^= b;
      s_index = 0;

      if (s_packet.len > kMaxPayload) {
        s_parse_error = true;
        reset_parser();
      } else if (s_packet.len == 0) {
        s_state = ParseState::ReadChecksum;
      } else {
        s_state = ParseState::ReadPayload;
      }
      break;

    case ParseState::ReadPayload:
      s_packet.payload[s_index++] = b;
      s_checksum ^= b;

      if (s_index >= s_packet.len) {
        s_state = ParseState::ReadChecksum;
      }
      break;

    case ParseState::ReadChecksum:
      if (b == s_checksum) {
        out = s_packet;
        reset_parser();
        return true;
      }

      s_parse_error = true;
      reset_parser();
      break;
  }

  return false;
}

bool Protocol::takeParseError() {
  if (!s_parse_error) {
    return false;
  }

  s_parse_error = false;
  return true;
}

void Protocol::sendPacket(const uint8_t *data, uint8_t payloadLen) {
  uint8_t checksum = 0;

  Serial.write(kSof);
  Serial.write(data[0]);
  checksum ^= data[0];

  const uint8_t len = (payloadLen > 0) ? static_cast<uint8_t>(payloadLen - 1) : 0;
  Serial.write(len);
  checksum ^= len;

  for (uint8_t i = 1; i < payloadLen; ++i) {
    Serial.write(data[i]);
    checksum ^= data[i];
  }

  Serial.write(checksum);
}
