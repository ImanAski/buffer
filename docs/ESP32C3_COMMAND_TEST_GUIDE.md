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
- `0x02` Set page: payload `[page_id]`
- `0x03` Input event: payload `[input_id, value]`
- `0x04` Set theme: payload `[theme_id]`
- `0x05` Set tab: payload `[tab_id]`
- `0x06` Set bias field: payload `[field_id, value_hi, value_lo]`

## 3) IDs

Toggle IDs:

- `0` WiFi
- `1` Bluetooth
- `2` Display
- `3` System

Page IDs (`0x02`):

- `0` Home
- `1` Bias

Tab IDs (`0x05`):

- `0` Home
- `1` Bias
- `2` Load
- `3` Help

Theme IDs:

- `0` Dark
- `1` Light
- `2` Ocean

Bias Field IDs (`0x06`):

- `0` Mode
- `1` Dither Freq
- `2` Dither Amp
- `3` Dither Enable (`0` OFF, non-zero ON)
- `4` Slope
- `5` Bias Voltage
- `6` Optical Output

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

void cmdSetPage(uint8_t pageId) {
  uint8_t p[1] = {pageId};
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

void cmdSetTab(uint8_t tabId) {
  uint8_t p[1] = {tabId};
  sendPacket(0x05, p, sizeof(p));
}

void cmdSetBiasField(uint8_t fieldId, uint16_t value) {
  uint8_t p[3] = {
    fieldId,
    static_cast<uint8_t>((value >> 8) & 0xFF),
    static_cast<uint8_t>(value & 0xFF)
  };
  sendPacket(0x06, p, sizeof(p));
}

void setup() {
  Serial.begin(115200);
  // Example pins; change to match your ESP32-C3 board routing
  Link.begin(115200, SERIAL_8N1, 6, 7); // RX, TX
  delay(1000);

  cmdSetPage(0);   // Home page
  delay(500);
  cmdSetTab(2);    // LOAD tab title on home screen
  delay(500);
  cmdSetTab(3);    // HELP tab title on home screen
  delay(500);
  cmdSetPage(1);   // Bias page
  delay(500);
  cmdSetBiasField(0, 2);    // Mode = 2
  cmdSetBiasField(1, 450);  // Dither Freq
  cmdSetBiasField(2, 17);   // Dither Amp
  cmdSetBiasField(3, 1);    // Dither ON
  cmdSetBiasField(4, 9);    // Slope
  cmdSetBiasField(5, 115);  // Bias Voltage
  cmdSetBiasField(6, 98);   // Optical Output
  delay(500);
  cmdSetTheme(2);  // Ocean
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

1. Send `Set page -> Home`.
2. Send `Set tab -> Load`, verify top title shows `LOAD`.
3. Send `Set tab -> Help`, verify top title shows `HELP`.
4. Send `Set page -> Bias`, verify bias page is shown.
5. Send several `Set bias field` commands and verify values update on Bias page.
6. Send `Set page -> Home` and `Set tab -> Home`, verify title returns to `HOME`.
