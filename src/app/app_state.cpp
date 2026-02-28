#include "app_state.h"

void app_state_init(AppState &state) {
  for (uint8_t i = 0; i < static_cast<uint8_t>(ToggleId::Count); ++i) {
    state.toggles[i] = false;
  }
  for (uint8_t i = 0; i < static_cast<uint8_t>(BiasFieldId::Count); ++i) {
    state.bias_values[i] = 0;
  }

  state.bias_values[static_cast<uint8_t>(BiasFieldId::Mode)] = 0;
  state.bias_values[static_cast<uint8_t>(BiasFieldId::DitherEnable)] = 0;
  state.bias_values[static_cast<uint8_t>(BiasFieldId::BiasVoltage)] = 115;
  state.bias_values[static_cast<uint8_t>(BiasFieldId::OpticalOutput)] = 115;

  state.active_screen = ScreenId::Home;
  state.active_tab = TabId::Home;
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

void app_state_set_tab(AppState &state, TabId id) {
  const uint8_t idx = static_cast<uint8_t>(id);
  if (idx >= static_cast<uint8_t>(TabId::Count)) {
    return;
  }

  state.active_tab = id;
}

void app_state_set_bias_field(AppState &state, BiasFieldId id, uint16_t value) {
  const uint8_t idx = static_cast<uint8_t>(id);
  if (idx >= static_cast<uint8_t>(BiasFieldId::Count)) {
    return;
  }

  state.bias_values[idx] = value;
}

uint16_t app_state_get_bias_field(const AppState &state, BiasFieldId id) {
  const uint8_t idx = static_cast<uint8_t>(id);
  if (idx >= static_cast<uint8_t>(BiasFieldId::Count)) {
    return 0;
  }

  return state.bias_values[idx];
}

void app_state_set_theme(AppState &state, ThemeId id) {
  const uint8_t idx = static_cast<uint8_t>(id);
  if (idx >= static_cast<uint8_t>(ThemeId::Count)) {
    return;
  }

  state.theme = id;
}
