#pragma once

#include <cstdint>
#include "events/event_types.h"

struct AppState {
  bool toggles[static_cast<uint8_t>(ToggleId::Count)];
  ScreenId active_screen;
  ThemeId theme;
  bool comms_online;
  uint32_t last_protocol_error_ms;
};

void app_state_init(AppState &state);
void app_state_set_toggle(AppState &state, ToggleId id, bool value);
bool app_state_get_toggle(const AppState &state, ToggleId id);
void app_state_set_screen(AppState &state, ScreenId id);
void app_state_set_theme(AppState &state, ThemeId id);
