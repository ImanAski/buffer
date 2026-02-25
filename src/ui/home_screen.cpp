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

// void home_screen_create() {
//   /* lv_obj_t *scr = lv_screen_active();
//   lv_obj_clean(scr);

//   lv_obj_t *root = lv_obj_create(scr);
//   lv_obj_set_size(root, LV_PCT(100), LV_PCT(100));
//   lv_obj_set_style_pad_all(root, 12, 0);
//   lv_obj_set_flex_flow(root, LV_FLEX_FLOW_COLUMN);
//   lv_obj_set_scrollbar_mode(root, LV_SCROLLBAR_MODE_OFF);

//   s_title = lv_label_create(root);
//   lv_label_set_text(s_title, "Home");
//   lv_obj_set_style_pad_bottom(s_title, 8, 0);

//   lv_obj_t *btn_settings = lv_btn_create(root);
//   lv_obj_set_width(btn_settings, LV_PCT(100));
//   lv_obj_add_event_cb(btn_settings, nav_settings_cb, LV_EVENT_CLICKED, nullptr);
//   lv_obj_t *lbl_settings = lv_label_create(btn_settings);
//   lv_label_set_text(lbl_settings, "Open Settings");
//   lv_obj_center(lbl_settings);

//   lv_obj_t *btn_themes = lv_btn_create(root);
//   lv_obj_set_width(btn_themes, LV_PCT(100));
//   lv_obj_add_event_cb(btn_themes, nav_themes_cb, LV_EVENT_CLICKED, nullptr);
//   lv_obj_t *lbl_themes = lv_label_create(btn_themes);
//   lv_label_set_text(lbl_themes, "Themes");
//   lv_obj_center(lbl_themes);

//   lv_obj_t *btn_about = lv_btn_create(root);
//   lv_obj_set_width(btn_about, LV_PCT(100));
//   lv_obj_add_event_cb(btn_about, nav_about_cb, LV_EVENT_CLICKED, nullptr);
//   lv_obj_t *lbl_about = lv_label_create(btn_about);
//   lv_label_set_text(lbl_about, "About");
//   lv_obj_center(lbl_about); */

//   lv_obj_t *panel_Layer_1 = lv_obj_create(lv_screen_active());
//   lv_obj_set_pos(panel_Layer_1, 0, 0);
//   lv_obj_set_size(panel_Layer_1, 240, 420);
//   lv_obj_set_style_bg_color(panel_Layer_1, lv_color_hex(0x000000), LV_PART_MAIN);
//   lv_obj_set_style_radius(panel_Layer_1, 0, LV_PART_MAIN);

//   lv_obj_t *panel_Layer_2 = lv_obj_create(lv_screen_active());
//   lv_obj_set_pos(panel_Layer_2, 43, 10);
//   lv_obj_set_size(panel_Layer_2, 188, 30);
//   lv_obj_set_style_radius(panel_Layer_2, 0, LV_PART_MAIN);

//   lv_obj_t *button_Layer_3 = lv_button_create(lv_screen_active());
//   lv_obj_set_pos(button_Layer_3, 14, 10);
//   lv_obj_set_size(button_Layer_3, 29, 28);
//   lv_obj_set_style_radius(button_Layer_3, 0, LV_PART_MAIN);
//   lv_obj_set_style_bg_color(button_Layer_3, lv_color_hex(0x3f51b5), LV_PART_MAIN);
//   lv_obj_set_style_text_color(button_Layer_3, lv_color_hex(0xffffff), LV_PART_MAIN);
//   lv_obj_t *button_Layer_3_label = lv_label_create(button_Layer_3);
//   lv_label_set_text(button_Layer_3_label, "");
//   lv_obj_center(button_Layer_3_label);
// }

int Layer_4_w = 240;
void home_screen_create() {
    // panel: Layer 1
    lv_obj_t * panel_Layer_1 = lv_obj_create(lv_screen_active());
    lv_obj_set_pos(panel_Layer_1, 0, 0);
    lv_obj_set_size(panel_Layer_1, 240, 240);
    lv_obj_set_style_bg_color(panel_Layer_1, lv_color_hex(0x000000), LV_PART_MAIN);
    lv_obj_set_style_radius(panel_Layer_1, 0, LV_PART_MAIN);

    // panel: Layer 4
    lv_obj_t * panel_Layer_4 = lv_obj_create(lv_screen_active());
    lv_obj_set_pos(panel_Layer_4, 0, 0);
    lv_obj_set_size(panel_Layer_4, Layer_4_w, 14);
    lv_obj_set_style_bg_color(panel_Layer_4, lv_color_hex(0x0f446c), LV_PART_MAIN);
    lv_obj_set_style_radius(panel_Layer_4, 0, LV_PART_MAIN);

    // string: Layer 5
    lv_obj_t * label_Layer_5 = lv_label_create(lv_screen_active());
    lv_obj_set_style_text_color(label_Layer_5, lv_color_hex(0xffffff), LV_PART_MAIN);
    lv_obj_set_pos(label_Layer_5, 0, -1);
    lv_label_set_text(label_Layer_5, "Home");

    // panel: Layer 2 copy 1
    lv_obj_t * panel_Layer_2_copy_1 = lv_obj_create(lv_screen_active());
    lv_obj_set_pos(panel_Layer_2_copy_1, 49, 79);
    lv_obj_set_size(panel_Layer_2_copy_1, 146, 62);
    lv_obj_set_style_bg_color(panel_Layer_2_copy_1, lv_color_hex(0x333333), LV_PART_MAIN);
    lv_obj_set_style_radius(panel_Layer_2_copy_1, 0, LV_PART_MAIN);

    // button: Layer 3 copy 1
    lv_obj_t * button_Layer_3_copy_1 = lv_button_create(lv_screen_active());
    lv_obj_set_pos(button_Layer_3_copy_1, 0, 79);
    lv_obj_set_size(button_Layer_3_copy_1, 49, 60);
    lv_obj_set_style_radius(button_Layer_3_copy_1, 0, LV_PART_MAIN);
    lv_obj_set_style_bg_color(button_Layer_3_copy_1, lv_color_hex(0x4d4d4d), LV_PART_MAIN);
    lv_obj_set_style_text_color(button_Layer_3_copy_1, lv_color_hex(0xffffff), LV_PART_MAIN);
    lv_obj_t * button_Layer_3_copy_1_label = lv_label_create(button_Layer_3_copy_1);
    lv_label_set_text(button_Layer_3_copy_1_label, "Bias");
    lv_obj_center(button_Layer_3_copy_1_label);

    // button: Layer 6 copy 1
    lv_obj_t * button_Layer_6_copy_1 = lv_button_create(lv_screen_active());
    lv_obj_set_pos(button_Layer_6_copy_1, 195, 79);
    lv_obj_set_size(button_Layer_6_copy_1, 45, 60);
    lv_obj_set_style_radius(button_Layer_6_copy_1, 0, LV_PART_MAIN);
    lv_obj_set_style_text_color(button_Layer_6_copy_1, lv_color_hex(0xffffff), LV_PART_MAIN);
    lv_obj_t * button_Layer_6_copy_1_label = lv_label_create(button_Layer_6_copy_1);
    lv_label_set_text(button_Layer_6_copy_1_label, "Load");
    lv_obj_center(button_Layer_6_copy_1_label);

    // panel: Layer 2
    lv_obj_t * panel_Layer_2 = lv_obj_create(lv_screen_active());
    lv_obj_set_pos(panel_Layer_2, 49, 15);
    lv_obj_set_size(panel_Layer_2, 146, 62);
    lv_obj_set_style_bg_color(panel_Layer_2, lv_color_hex(0x333333), LV_PART_MAIN);
    lv_obj_set_style_radius(panel_Layer_2, 0, LV_PART_MAIN);

    // button: Layer 3
    lv_obj_t * button_Layer_3 = lv_button_create(lv_screen_active());
    lv_obj_set_pos(button_Layer_3, 0, 15);
    lv_obj_set_size(button_Layer_3, 49, 60);
    lv_obj_set_style_radius(button_Layer_3, 0, LV_PART_MAIN);
    lv_obj_set_style_bg_color(button_Layer_3, lv_color_hex(0x4d4d4d), LV_PART_MAIN);
    lv_obj_set_style_text_color(button_Layer_3, lv_color_hex(0xffffff), LV_PART_MAIN);
    lv_obj_t * button_Layer_3_label = lv_label_create(button_Layer_3);
    lv_label_set_text(button_Layer_3_label, "Bias");
    lv_obj_center(button_Layer_3_label);

    // button: Layer 6
    lv_obj_t * button_Layer_6 = lv_button_create(lv_screen_active());
    lv_obj_set_pos(button_Layer_6, 195, 15);
    lv_obj_set_size(button_Layer_6, 45, 60);
    lv_obj_set_style_radius(button_Layer_6, 0, LV_PART_MAIN);
    lv_obj_set_style_text_color(button_Layer_6, lv_color_hex(0xffffff), LV_PART_MAIN);
    lv_obj_t * button_Layer_6_label = lv_label_create(button_Layer_6);
    lv_label_set_text(button_Layer_6_label, "Load");
    lv_obj_center(button_Layer_6_label);

    // panel: Layer 2 copy 2
    lv_obj_t * panel_Layer_2_copy_2 = lv_obj_create(lv_screen_active());
    lv_obj_set_pos(panel_Layer_2_copy_2, 49, 143);
    lv_obj_set_size(panel_Layer_2_copy_2, 146, 62);
    lv_obj_set_style_bg_color(panel_Layer_2_copy_2, lv_color_hex(0x333333), LV_PART_MAIN);
    lv_obj_set_style_radius(panel_Layer_2_copy_2, 0, LV_PART_MAIN);

    // button: Layer 3 copy 2
    lv_obj_t * button_Layer_3_copy_2 = lv_button_create(lv_screen_active());
    lv_obj_set_pos(button_Layer_3_copy_2, 0, 143);
    lv_obj_set_size(button_Layer_3_copy_2, 49, 60);
    lv_obj_set_style_radius(button_Layer_3_copy_2, 0, LV_PART_MAIN);
    lv_obj_set_style_bg_color(button_Layer_3_copy_2, lv_color_hex(0x4d4d4d), LV_PART_MAIN);
    lv_obj_set_style_text_color(button_Layer_3_copy_2, lv_color_hex(0xffffff), LV_PART_MAIN);
    lv_obj_t * button_Layer_3_copy_2_label = lv_label_create(button_Layer_3_copy_2);
    lv_label_set_text(button_Layer_3_copy_2_label, "Bias");
    lv_obj_center(button_Layer_3_copy_2_label);

    // button: Layer 6 copy 2
    lv_obj_t * button_Layer_6_copy_2 = lv_button_create(lv_screen_active());
    lv_obj_set_pos(button_Layer_6_copy_2, 195, 143);
    lv_obj_set_size(button_Layer_6_copy_2, 45, 60);
    lv_obj_set_style_radius(button_Layer_6_copy_2, 0, LV_PART_MAIN);
    lv_obj_set_style_text_color(button_Layer_6_copy_2, lv_color_hex(0xffffff), LV_PART_MAIN);
    lv_obj_t * button_Layer_6_copy_2_label = lv_label_create(button_Layer_6_copy_2);
    lv_label_set_text(button_Layer_6_copy_2_label, "Load");
    lv_obj_center(button_Layer_6_copy_2_label);

}


void home_screen_apply_state(const AppState &state) {
  lv_obj_t *scr = lv_screen_active();
  lv_obj_set_style_bg_color(scr, background_for_theme(state.theme), 0);
  lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, 0);
  if (s_title != nullptr) {
    lv_obj_set_style_text_color(s_title, title_for_theme(state.theme), 0);
  }
}
