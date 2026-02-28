#pragma once

#include <cstdint>

enum class ToggleId : uint8_t {
  Wifi = 0,
  Bluetooth,
  Display,
  System,
  Count
};

enum class ScreenId : uint8_t {
  Home = 0,
  Bias,
  Count
};

enum class TabId : uint8_t {
  Home = 0,
  Bias,
  Load,
  Help,
  Count
};

enum class BiasFieldId : uint8_t {
  Mode = 0,
  DitherFreq,
  DitherAmp,
  DitherEnable,
  Slope,
  BiasVoltage,
  OpticalOutput,
  Count
};

enum class ThemeId : uint8_t {
  Dark = 0,
  Light,
  Ocean,
  Count
};

enum class EventType : uint8_t {
  UiToggleChanged,
  UiRowPressed,
  UiNavigate,
  UiTabSelected,
  UiThemeSelected,
  McuSetToggle,
  McuSetScreen,
  McuSetTab,
  McuSetBiasField,
  McuSetTheme,
  HwInputChanged,
  TouchInput,
  ProtocolError
};

struct Event {
  EventType type;
  uint32_t ts_ms;
  union {
    struct {
      ToggleId toggle_id;
      bool value;
    } toggle;
    struct {
      ToggleId row_id;
    } row;
    struct {
      ScreenId screen_id;
    } screen;
    struct {
      TabId tab_id;
    } tab;
    struct {
      BiasFieldId field_id;
      uint16_t value;
    } bias;
    struct {
      ThemeId theme_id;
    } theme;
    struct {
      uint8_t input_id;
      uint16_t value;
    } input;
    struct {
      uint8_t code;
    } protocol_error;
  } data;
};
