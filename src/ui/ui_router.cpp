#include "ui_router.h"

#include "bias_settings.h"
#include "home_screen.h"

namespace {
bool s_initialized = false;
ScreenId s_last_screen = ScreenId::Count;
}

void ui_router_init(const AppState &state) {
  s_initialized = true;
  s_last_screen = ScreenId::Count;
  ui_router_render(state);
}

void ui_router_render(const AppState &state) {
  if (!s_initialized) {
    return;
  }

  if (s_last_screen != state.active_screen) {
    switch (state.active_screen) {
      case ScreenId::Home:
        home_screen_create();
        break;
      case ScreenId::Bias:
        bias_settings_create();
        break;
      case ScreenId::Count:
        home_screen_create();
        break;
      default:
        home_screen_create();
        break;
    }
    s_last_screen = state.active_screen;
  }

  switch (state.active_screen) {
    case ScreenId::Home:
      home_screen_apply_state(state);
      break;
    case ScreenId::Bias:
      bias_settings_apply_state(state);
      break;
    case ScreenId::Count:
      break;
    default:
      break;
  }
}
