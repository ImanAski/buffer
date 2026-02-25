#include "themes_screen.h"

#include <Arduino.h>
#include <lvgl.h>

#include "events/event_queue.h"

namespace {
lv_obj_t *s_title = nullptr;
lv_obj_t *s_theme_btns[static_cast<uint8_t>(ThemeId::Count)] = {};

Event make_nav_event(ScreenId screen) {
  Event e{};
  e.type = EventType::UiNavigate;
  e.ts_ms = millis();
  e.data.screen.screen_id = screen;
  return e;
}

Event make_theme_event(ThemeId theme) {
  Event e{};
  e.type = EventType::UiThemeSelected;
  e.ts_ms = millis();
  e.data.theme.theme_id = theme;
  return e;
}

void nav_home_cb(lv_event_t *e) {
  LV_UNUSED(e);
  event_queue_push(make_nav_event(ScreenId::Home));
}

void theme_pick_cb(lv_event_t *e) {
  const auto raw = reinterpret_cast<uintptr_t>(lv_event_get_user_data(e));
  const ThemeId id = static_cast<ThemeId>(static_cast<uint8_t>(raw));
  event_queue_push(make_theme_event(id));
}

const char *theme_name(ThemeId id) {
  switch (id) {
    case ThemeId::Light:
      return "Light";
    case ThemeId::Ocean:
      return "Ocean";
    case ThemeId::Dark:
    default:
      return "Dark";
  }
}

lv_color_t background_for_theme(ThemeId theme) {
  switch (theme) {
    case ThemeId::Light:
      return lv_color_hex(0xFDFDFD);
    case ThemeId::Ocean:
      return lv_color_hex(0x062034);
    case ThemeId::Dark:
    default:
      return lv_color_hex(0x151515);
  }
}

lv_color_t title_for_theme(ThemeId theme) {
  switch (theme) {
    case ThemeId::Light:
      return lv_color_hex(0x111111);
    case ThemeId::Ocean:
      return lv_color_hex(0xA2EAFF);
    case ThemeId::Dark:
    default:
      return lv_color_hex(0xF8F8F8);
  }
}
}

void themes_screen_create() {
  lv_obj_t *scr = lv_screen_active();
  lv_obj_clean(scr);

  lv_obj_t *root = lv_obj_create(scr);
  lv_obj_set_size(root, LV_PCT(100), LV_PCT(100));
  lv_obj_set_style_pad_all(root, 12, 0);
  lv_obj_set_flex_flow(root, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_scrollbar_mode(root, LV_SCROLLBAR_MODE_OFF);

  s_title = lv_label_create(root);
  lv_label_set_text(s_title, "Themes");
  lv_obj_set_style_pad_bottom(s_title, 8, 0);

  for (uint8_t i = 0; i < static_cast<uint8_t>(ThemeId::Count); ++i) {
    const ThemeId id = static_cast<ThemeId>(i);
    lv_obj_t *btn = lv_btn_create(root);
    lv_obj_set_width(btn, LV_PCT(100));
    lv_obj_add_flag(btn, LV_OBJ_FLAG_CHECKABLE);
    lv_obj_add_event_cb(btn, theme_pick_cb, LV_EVENT_CLICKED,
                        reinterpret_cast<void *>(static_cast<uintptr_t>(i)));

    lv_obj_t *lbl = lv_label_create(btn);
    lv_label_set_text(lbl, theme_name(id));
    lv_obj_center(lbl);

    s_theme_btns[i] = btn;
  }

  lv_obj_t *back_btn = lv_btn_create(root);
  lv_obj_set_width(back_btn, LV_PCT(100));
  lv_obj_add_event_cb(back_btn, nav_home_cb, LV_EVENT_CLICKED, nullptr);
  lv_obj_t *back_lbl = lv_label_create(back_btn);
  lv_label_set_text(back_lbl, "Back");
  lv_obj_center(back_lbl);
}

void themes_screen_apply_state(const AppState &state) {
  lv_obj_t *scr = lv_screen_active();
  lv_obj_set_style_bg_color(scr, background_for_theme(state.theme), 0);
  lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, 0);
  if (s_title != nullptr) {
    lv_obj_set_style_text_color(s_title, title_for_theme(state.theme), 0);
  }

  const uint8_t selected = static_cast<uint8_t>(state.theme);
  for (uint8_t i = 0; i < static_cast<uint8_t>(ThemeId::Count); ++i) {
    lv_obj_t *btn = s_theme_btns[i];
    if (btn == nullptr) {
      continue;
    }

    if (i == selected) {
      lv_obj_add_state(btn, LV_STATE_CHECKED);
    } else {
      lv_obj_remove_state(btn, LV_STATE_CHECKED);
    }
  }
}
