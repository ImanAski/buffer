#include "serial_input.h"

#include <Arduino.h>
#include "Config.h"
#include "event_queue.h"
#include "protocol.h"

namespace {
constexpr uint32_t kProtocolErrorRateLimitMs = 200;
uint32_t s_last_error_ms = 0;
void push_protocol_error(uint8_t code);

void debug_print_hex_byte(uint8_t v) {
#if PROTOCOL_DEBUG
  const char *hex = "0123456789ABCDEF";
  Serial.print(hex[(v >> 4) & 0x0F]);
  Serial.print(hex[v & 0x0F]);
#else
  (void)v;
#endif
}

void debug_print_packet(const char *source, const Packet &packet) {
#if PROTOCOL_DEBUG
  Serial.print("[PKT ");
  Serial.print(source);
  Serial.print("] type=0x");
  debug_print_hex_byte(packet.type);
  Serial.print(" len=");
  Serial.print(packet.len);
  Serial.print(" payload=");
  for (uint8_t i = 0; i < packet.len; ++i) {
    debug_print_hex_byte(packet.payload[i]);
    if (i + 1 < packet.len) {
      Serial.print(' ');
    }
  }
  Serial.println();
#else
  (void)source;
  (void)packet;
#endif
}

void debug_print_error(const char *source, uint8_t code) {
#if PROTOCOL_DEBUG
  Serial.print("[PKT ");
  Serial.print(source);
  Serial.print("] parse/error code=");
  Serial.println(code);
#else
  (void)source;
  (void)code;
#endif
}

void process_stream(Stream &stream, const char *source) {
  while (stream.available() > 0) {
    const uint8_t b = static_cast<uint8_t>(stream.read());
#if PROTOCOL_DEBUG_RAW_BYTES
    Serial.print("[RAW ");
    Serial.print(source);
    Serial.print("] 0x");
    debug_print_hex_byte(b);
    Serial.println();
#endif

    Packet packet{};
    if (Protocol::parseByte(b, packet)) {
      debug_print_packet(source, packet);
      Event e{};
      e.ts_ms = millis();

      switch (packet.type) {
        case 0x01:
          if (packet.len >= 2) {
            e.type = EventType::McuSetToggle;
            e.data.toggle.toggle_id = static_cast<ToggleId>(packet.payload[0]);
            e.data.toggle.value = packet.payload[1] != 0;
            event_queue_push(e);
          } else {
            push_protocol_error(2);
          }
          break;

        case 0x02:
          if (packet.len >= 1) {
            e.type = EventType::McuSetScreen;
            e.data.screen.screen_id = static_cast<ScreenId>(packet.payload[0]);
            event_queue_push(e);
          } else {
            push_protocol_error(3);
          }
          break;

        case 0x03:
          if (packet.len >= 2) {
            e.type = EventType::HwInputChanged;
            e.data.input.input_id = packet.payload[0];
            e.data.input.value = packet.payload[1];
            event_queue_push(e);
          } else {
            push_protocol_error(4);
          }
          break;

        case 0x04:
          if (packet.len >= 1) {
            e.type = EventType::McuSetTheme;
            e.data.theme.theme_id = static_cast<ThemeId>(packet.payload[0]);
            event_queue_push(e);
          } else {
            push_protocol_error(6);
          }
          break;

        default:
          push_protocol_error(5);
          break;
      }
    }

    if (Protocol::takeParseError()) {
      push_protocol_error(1);
      debug_print_error(source, 1);
    }
  }
}

void push_protocol_error(uint8_t code) {
  const uint32_t now = millis();
  if ((now - s_last_error_ms) < kProtocolErrorRateLimitMs) {
    return;
  }

  s_last_error_ms = now;
  Event e{};
  e.type = EventType::ProtocolError;
  e.ts_ms = now;
  e.data.protocol_error.code = code;
  event_queue_push(e);
}
}

void serial_input_init() {
  s_last_error_ms = 0;
  Serial1.begin(UART_BAUDRATE);
}

void serial_input_poll() {
  process_stream(Serial1, "UART");
  process_stream(Serial, "USB");
}
