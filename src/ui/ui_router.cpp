#include "ui_router.h"

#include "about_screen.h"
#include "home_screen.h"
#include "settings_screen.h"
#include "themes_screen.h"

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
      case ScreenId::Settings:
        settings_screen_create();
        break;
      case ScreenId::Themes:
        themes_screen_create();
        break;
      case ScreenId::About:
        about_screen_create();
        break;
      case ScreenId::Count:
        home_screen_create();
        break;
    }
    s_last_screen = state.active_screen;
  }

  switch (state.active_screen) {
    case ScreenId::Home:
      home_screen_apply_state(state);
      break;
    case ScreenId::Settings:
      settings_screen_apply_state(state);
      break;
    case ScreenId::Themes:
      themes_screen_apply_state(state);
      break;
    case ScreenId::About:
      about_screen_apply_state(state);
      break;
    case ScreenId::Count:
      break;
  }
}
