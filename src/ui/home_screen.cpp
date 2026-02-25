#include "home_screen.h"

#include <Arduino.h>
#include <lvgl.h>

#include "events/event_queue.h"

namespace {
lv_obj_t *s_title = nullptr;

Event make_nav_event(ScreenId screen) {
  Event e{};
  e.type = EventType::UiNavigate;
  e.ts_ms = millis();
  e.data.screen.screen_id = screen;
  return e;
}

void nav_settings_cb(lv_event_t *e) {
  LV_UNUSED(e);
  event_queue_push(make_nav_event(ScreenId::Settings));
}

void nav_themes_cb(lv_event_t *e) {
  LV_UNUSED(e);
  event_queue_push(make_nav_event(ScreenId::Themes));
}

void nav_about_cb(lv_event_t *e) {
  LV_UNUSED(e);
  event_queue_push(make_nav_event(ScreenId::About));
}

lv_color_t background_for_theme(ThemeId theme) {
  switch (theme) {
    case ThemeId::Light:
      return lv_color_hex(0xF6F6F6);
    case ThemeId::Ocean:
      return lv_color_hex(0x041722);
    case ThemeId::Dark:
    default:
      return lv_color_hex(0x0E1116);
  }
}

lv_color_t title_for_theme(ThemeId theme) {
  switch (theme) {
    case ThemeId::Light:
      return lv_color_hex(0x1E1E1E);
    case ThemeId::Ocean:
      return lv_color_hex(0x8FE6FF);
    case ThemeId::Dark:
    default:
      return lv_color_hex(0xFFFFFF);
  }
}
}

void home_screen_create() {
  /* lv_obj_t *scr = lv_screen_active();
  lv_obj_clean(scr);

  lv_obj_t *root = lv_obj_create(scr);
  lv_obj_set_size(root, LV_PCT(100), LV_PCT(100));
  lv_obj_set_style_pad_all(root, 12, 0);
  lv_obj_set_flex_flow(root, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_scrollbar_mode(root, LV_SCROLLBAR_MODE_OFF);

  s_title = lv_label_create(root);
  lv_label_set_text(s_title, "Home");
  lv_obj_set_style_pad_bottom(s_title, 8, 0);

  lv_obj_t *btn_settings = lv_btn_create(root);
  lv_obj_set_width(btn_settings, LV_PCT(100));
  lv_obj_add_event_cb(btn_settings, nav_settings_cb, LV_EVENT_CLICKED, nullptr);
  lv_obj_t *lbl_settings = lv_label_create(btn_settings);
  lv_label_set_text(lbl_settings, "Open Settings");
  lv_obj_center(lbl_settings);

  lv_obj_t *btn_themes = lv_btn_create(root);
  lv_obj_set_width(btn_themes, LV_PCT(100));
  lv_obj_add_event_cb(btn_themes, nav_themes_cb, LV_EVENT_CLICKED, nullptr);
  lv_obj_t *lbl_themes = lv_label_create(btn_themes);
  lv_label_set_text(lbl_themes, "Themes");
  lv_obj_center(lbl_themes);

  lv_obj_t *btn_about = lv_btn_create(root);
  lv_obj_set_width(btn_about, LV_PCT(100));
  lv_obj_add_event_cb(btn_about, nav_about_cb, LV_EVENT_CLICKED, nullptr);
  lv_obj_t *lbl_about = lv_label_create(btn_about);
  lv_label_set_text(lbl_about, "About");
  lv_obj_center(lbl_about); */

  lv_obj_t *panel_Layer_1 = lv_obj_create(lv_screen_active());
  lv_obj_set_pos(panel_Layer_1, 2, 16);
  lv_obj_set_size(panel_Layer_1, 240, 420);
  lv_obj_set_style_bg_color(panel_Layer_1, lv_color_hex(0x424242), LV_PART_MAIN);

  lv_obj_t *panel_Layer_2 = lv_obj_create(lv_screen_active());
  lv_obj_set_pos(panel_Layer_2, 43, 26);
  lv_obj_set_size(panel_Layer_2, 188, 30);
  lv_obj_set_style_radius(panel_Layer_2, 0, LV_PART_MAIN);

  lv_obj_t *button_Layer_3 = lv_button_create(lv_screen_active());
  lv_obj_set_pos(button_Layer_3, 13, 26);
  lv_obj_set_size(button_Layer_3, 29, 28);
  lv_obj_set_style_radius(button_Layer_3, 0, LV_PART_MAIN);
  lv_obj_set_style_bg_color(button_Layer_3, lv_color_hex(0x000000), LV_PART_MAIN);
  lv_obj_set_style_text_color(button_Layer_3, lv_color_hex(0xffffff), LV_PART_MAIN);
  lv_obj_t *button_Layer_3_label = lv_label_create(button_Layer_3);
  lv_label_set_text(button_Layer_3_label, "");
  lv_obj_center(button_Layer_3_label);
}

void home_screen_apply_state(const AppState &state) {
  lv_obj_t *scr = lv_screen_active();
  lv_obj_set_style_bg_color(scr, background_for_theme(state.theme), 0);
  lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, 0);
  if (s_title != nullptr) {
    lv_obj_set_style_text_color(s_title, title_for_theme(state.theme), 0);
  }
}
