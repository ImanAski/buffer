#include "event_dispatcher.h"

#include <Arduino.h>
#include "protocol.h"
#include "storage/settings_store.h"
#include "ui/ui_router.h"

namespace {
AppState *s_state = nullptr;

bool is_valid_toggle(ToggleId id) {
  return static_cast<uint8_t>(id) < static_cast<uint8_t>(ToggleId::Count);
}

bool is_valid_screen(ScreenId id) {
  return static_cast<uint8_t>(id) < static_cast<uint8_t>(ScreenId::Count);
}

bool is_valid_theme(ThemeId id) {
  return static_cast<uint8_t>(id) < static_cast<uint8_t>(ThemeId::Count);
}

void send_toggle_ack(ToggleId id, bool value) {
  uint8_t payload[3];
  payload[0] = 0x81;
  payload[1] = static_cast<uint8_t>(id);
  payload[2] = value ? 1 : 0;
  Protocol::sendPacket(payload, sizeof(payload));
}
}

void event_dispatcher_init(AppState *state) {
  s_state = state;
}

void event_dispatcher_dispatch(const Event &event) {
  if (s_state == nullptr) {
    return;
  }

  switch (event.type) {
    case EventType::UiToggleChanged:
      if (is_valid_toggle(event.data.toggle.toggle_id)) {
        app_state_set_toggle(*s_state, event.data.toggle.toggle_id, event.data.toggle.value);
        settings_store_save(*s_state);
        send_toggle_ack(event.data.toggle.toggle_id, event.data.toggle.value);
      }
      break;

    case EventType::McuSetToggle:
      if (is_valid_toggle(event.data.toggle.toggle_id)) {
        app_state_set_toggle(*s_state, event.data.toggle.toggle_id, event.data.toggle.value);
        s_state->comms_online = true;
        settings_store_save(*s_state);
      }
      break;

    case EventType::McuSetScreen:
      if (is_valid_screen(event.data.screen.screen_id)) {
        app_state_set_screen(*s_state, event.data.screen.screen_id);
        s_state->comms_online = true;
        settings_store_save(*s_state);
      }
      break;

    case EventType::UiNavigate:
      if (is_valid_screen(event.data.screen.screen_id)) {
        app_state_set_screen(*s_state, event.data.screen.screen_id);
      }
      break;

    case EventType::UiThemeSelected:
      if (is_valid_theme(event.data.theme.theme_id)) {
        app_state_set_theme(*s_state, event.data.theme.theme_id);
        settings_store_save(*s_state);
      }
      break;

    case EventType::McuSetTheme:
      if (is_valid_theme(event.data.theme.theme_id)) {
        app_state_set_theme(*s_state, event.data.theme.theme_id);
        s_state->comms_online = true;
        settings_store_save(*s_state);
      }
      break;

    case EventType::HwInputChanged:
      if (event.data.input.input_id < static_cast<uint8_t>(ToggleId::Count)) {
        ToggleId id = static_cast<ToggleId>(event.data.input.input_id);
        bool value = event.data.input.value != 0;
        app_state_set_toggle(*s_state, id, value);
        settings_store_save(*s_state);
      }
      break;

    case EventType::ProtocolError:
      s_state->last_protocol_error_ms = event.ts_ms;
      s_state->comms_online = false;
      break;

    case EventType::UiRowPressed:
    case EventType::TouchInput:
      break;
  }

  ui_router_render(*s_state);
}
