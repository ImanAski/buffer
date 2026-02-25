#include "about_screen.h"

#include <Arduino.h>
#include <lvgl.h>

#include "events/event_queue.h"

namespace {
lv_obj_t *s_title = nullptr;
lv_obj_t *s_body = nullptr;

Event make_nav_event(ScreenId screen) {
  Event e{};
  e.type = EventType::UiNavigate;
  e.ts_ms = millis();
  e.data.screen.screen_id = screen;
  return e;
}

void nav_home_cb(lv_event_t *e) {
  LV_UNUSED(e);
  event_queue_push(make_nav_event(ScreenId::Home));
}

lv_color_t background_for_theme(ThemeId theme) {
  switch (theme) {
    case ThemeId::Light:
      return lv_color_hex(0xFAFAFA);
    case ThemeId::Ocean:
      return lv_color_hex(0x091E2E);
    case ThemeId::Dark:
    default:
      return lv_color_hex(0x101010);
  }
}

lv_color_t text_for_theme(ThemeId theme) {
  switch (theme) {
    case ThemeId::Light:
      return lv_color_hex(0x222222);
    case ThemeId::Ocean:
      return lv_color_hex(0xC4F0FF);
    case ThemeId::Dark:
    default:
      return lv_color_hex(0xEAEAEA);
  }
}
}

void about_screen_create() {
  lv_obj_t *scr = lv_screen_active();
  lv_obj_clean(scr);

  lv_obj_t *root = lv_obj_create(scr);
  lv_obj_set_size(root, LV_PCT(100), LV_PCT(100));
  lv_obj_set_style_pad_all(root, 12, 0);
  lv_obj_set_flex_flow(root, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_scrollbar_mode(root, LV_SCROLLBAR_MODE_OFF);

  s_title = lv_label_create(root);
  lv_label_set_text(s_title, "About");

  s_body = lv_label_create(root);
  lv_label_set_text(s_body, "Buffer UI\\nRP2040 + LVGL\\nEvent-driven architecture");
  lv_obj_set_style_pad_top(s_body, 8, 0);

  lv_obj_t *back_btn = lv_btn_create(root);
  lv_obj_set_width(back_btn, LV_PCT(100));
  lv_obj_set_style_pad_top(back_btn, 12, 0);
  lv_obj_add_event_cb(back_btn, nav_home_cb, LV_EVENT_CLICKED, nullptr);
  lv_obj_t *back_lbl = lv_label_create(back_btn);
  lv_label_set_text(back_lbl, "Back");
  lv_obj_center(back_lbl);
}

void about_screen_apply_state(const AppState &state) {
  lv_obj_t *scr = lv_screen_active();
  lv_obj_set_style_bg_color(scr, background_for_theme(state.theme), 0);
  lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, 0);

  const lv_color_t c = text_for_theme(state.theme);
  if (s_title != nullptr) {
    lv_obj_set_style_text_color(s_title, c, 0);
  }
  if (s_body != nullptr) {
    lv_obj_set_style_text_color(s_body, c, 0);
  }
}
