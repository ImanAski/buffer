#include "app_state.h"

void app_state_init(AppState &state) {
  for (uint8_t i = 0; i < static_cast<uint8_t>(ToggleId::Count); ++i) {
    state.toggles[i] = false;
  }
  state.active_screen = ScreenId::Home;
  state.theme = ThemeId::Dark;
  state.comms_online = true;
  state.last_protocol_error_ms = 0;
}

void app_state_set_toggle(AppState &state, ToggleId id, bool value) {
  const uint8_t idx = static_cast<uint8_t>(id);
  if (idx >= static_cast<uint8_t>(ToggleId::Count)) {
    return;
  }

  state.toggles[idx] = value;
}

bool app_state_get_toggle(const AppState &state, ToggleId id) {
  const uint8_t idx = static_cast<uint8_t>(id);
  if (idx >= static_cast<uint8_t>(ToggleId::Count)) {
    return false;
  }

  return state.toggles[idx];
}

void app_state_set_screen(AppState &state, ScreenId id) {
  const uint8_t idx = static_cast<uint8_t>(id);
  if (idx >= static_cast<uint8_t>(ScreenId::Count)) {
    return;
  }

  state.active_screen = id;
}

void app_state_set_theme(AppState &state, ThemeId id) {
  const uint8_t idx = static_cast<uint8_t>(id);
  if (idx >= static_cast<uint8_t>(ThemeId::Count)) {
    return;
  }

  state.theme = id;
}
