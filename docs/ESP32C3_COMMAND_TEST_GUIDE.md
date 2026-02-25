# ESP32-C3 Super Mini Command Test Guide

This guide sends binary commands from an ESP32-C3 Super Mini to your RP2040 UI firmware.

## 1) Wiring (UART)

- ESP32-C3 `TX` -> RP2040 `RX` (your `UART_RX` pin in `Config.h`)
- ESP32-C3 `RX` -> RP2040 `TX` (your `UART_TX` pin in `Config.h`)
- ESP32-C3 `GND` -> RP2040 `GND`

Use the same baudrate on both boards: `115200`.

## 2) Packet format

Packet bytes:

`SOF(0xAA) | type | len | payload[len] | checksum`

Checksum is XOR of `type`, `len`, and all payload bytes.

Supported command types in current firmware:

- `0x01` Set toggle: payload `[toggle_id, value]`
- `0x02` Set screen: payload `[screen_id]`
- `0x03` Input event: payload `[input_id, value]`
- `0x04` Set theme: payload `[theme_id]`

## 3) IDs

Toggle IDs:

- `0` WiFi
- `1` Bluetooth
- `2` Display
- `3` System

Screen IDs:

- `0` Home
- `1` Settings
- `2` Themes
- `3` About

Theme IDs:

- `0` Dark
- `1` Light
- `2` Ocean

## 4) Example sender sketch (ESP32-C3)

Use this with Arduino framework on ESP32-C3. Update pins if your board uses different UART pins.

```cpp
#include <Arduino.h>

HardwareSerial Link(1);

static constexpr uint8_t SOF = 0xAA;

void sendPacket(uint8_t type, const uint8_t *payload, uint8_t len) {
  uint8_t checksum = type ^ len;
  Link.write(SOF);
  Link.write(type);
  Link.write(len);
  for (uint8_t i = 0; i < len; ++i) {
    Link.write(payload[i]);
    checksum ^= payload[i];
  }
  Link.write(checksum);
}

void cmdSetToggle(uint8_t id, bool on) {
  uint8_t p[2] = {id, static_cast<uint8_t>(on ? 1 : 0)};
  sendPacket(0x01, p, sizeof(p));
}

void cmdSetScreen(uint8_t screenId) {
  uint8_t p[1] = {screenId};
  sendPacket(0x02, p, sizeof(p));
}

void cmdInput(uint8_t inputId, uint8_t value) {
  uint8_t p[2] = {inputId, value};
  sendPacket(0x03, p, sizeof(p));
}

void cmdSetTheme(uint8_t themeId) {
  uint8_t p[1] = {themeId};
  sendPacket(0x04, p, sizeof(p));
}

void setup() {
  Serial.begin(115200);
  // Example pins; change to match your ESP32-C3 board routing
  Link.begin(115200, SERIAL_8N1, 6, 7); // RX, TX
  delay(1000);

  cmdSetScreen(2);   // Themes page
  delay(500);
  cmdSetTheme(2);    // Ocean
  delay(500);
  cmdSetScreen(1);   // Settings page
  delay(500);
  cmdSetToggle(0, true);  // WiFi ON
}

void loop() {
  static uint32_t last = 0;
  if (millis() - last > 3000) {
    last = millis();
    static bool on = false;
    on = !on;
    cmdSetToggle(1, on);  // Toggle Bluetooth every 3s
  }
}
```

## 5) Quick manual test sequence

1. Send `Set screen -> Themes`.
2. Send `Set theme -> Ocean`.
3. Send `Set screen -> Settings`.
4. Send `Set toggle -> WiFi ON`.
5. Reboot RP2040 and verify selected theme/toggles persist.
