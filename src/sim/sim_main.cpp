#include <Arduino.h>

#include "app/app_state.h"
#include "events/event_queue.h"
#include "events/event_types.h"
#include "system/display_port.h"
#include "ui/ui_router.h"

namespace {
void apply_event(AppState &state, const Event &event) {
  switch (event.type) {
    case EventType::UiTabSelected:
    case EventType::McuSetTab:
      app_state_set_tab(state, event.data.tab.tab_id);
      if (event.data.tab.tab_id == TabId::Bias) {
        app_state_set_screen(state, ScreenId::Bias);
      } else {
        app_state_set_screen(state, ScreenId::Home);
      }
      break;

    case EventType::UiNavigate:
    case EventType::McuSetScreen:
      app_state_set_screen(state, event.data.screen.screen_id);
      break;

    case EventType::UiThemeSelected:
    case EventType::McuSetTheme:
      app_state_set_theme(state, event.data.theme.theme_id);
      break;

    case EventType::McuSetToggle:
    case EventType::UiToggleChanged:
      app_state_set_toggle(state, event.data.toggle.toggle_id, event.data.toggle.value);
      break;

    case EventType::McuSetBiasField:
      app_state_set_bias_field(state, event.data.bias.field_id, event.data.bias.value);
      break;

    case EventType::HwInputChanged:
      if (event.data.input.input_id < static_cast<uint8_t>(ToggleId::Count)) {
        app_state_set_toggle(state, static_cast<ToggleId>(event.data.input.input_id),
                             event.data.input.value != 0);
      }
      break;

    case EventType::ProtocolError:
      break;

    case EventType::UiRowPressed:
    case EventType::TouchInput:
      break;
  }
}
}

int main() {
  AppState state{};
  event_queue_reset();
  app_state_init(state);

  display_port_init();
  ui_router_init(state);

  while (true) {
    Event event{};
    while (event_queue_pop(event)) {
      apply_event(state, event);
    }

    ui_router_render(state);
    display_port_task();
  }

  return 0;
}
