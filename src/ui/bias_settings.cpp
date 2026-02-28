#include "bias_settings.h"

#include <cstdio>

#include <lvgl.h>

namespace {
lv_obj_t *s_value_labels[static_cast<uint8_t>(BiasFieldId::Count)] = {nullptr};

void set_bias_value_text(BiasFieldId id, const char *text) {
  const uint8_t idx = static_cast<uint8_t>(id);
  if (idx >= static_cast<uint8_t>(BiasFieldId::Count) || s_value_labels[idx] == nullptr) {
    return;
  }
  lv_label_set_text(s_value_labels[idx], text);
}

lv_obj_t *create_value_label(int x, int y, lv_color_t color = lv_color_hex(0xffffff)) {
  lv_obj_t *label = lv_label_create(lv_screen_active());
  lv_obj_set_style_text_color(label, color, LV_PART_MAIN);
  lv_obj_set_pos(label, x, y);
  lv_label_set_text(label, "--");
  return label;
}

lv_obj_t *create_caption_label(const char *text, int x, int y, lv_color_t color = lv_color_hex(0xffffff)) {
  lv_obj_t *label = lv_label_create(lv_screen_active());
  lv_obj_set_style_text_color(label, color, LV_PART_MAIN);
  lv_obj_set_pos(label, x, y);
  lv_label_set_text(label, text);
  return label;
}

void create_row(int y, const char *title, BiasFieldId value_id) {
  lv_obj_t *left = lv_obj_create(lv_screen_active());
  lv_obj_set_pos(left, 0, y);
  lv_obj_set_size(left, 119, 20);
  lv_obj_set_style_bg_color(left, lv_color_hex(0x4d4d4d), LV_PART_MAIN);
  lv_obj_set_style_radius(left, 0, LV_PART_MAIN);

  lv_obj_t *right = lv_obj_create(lv_screen_active());
  lv_obj_set_pos(right, 121, y);
  lv_obj_set_size(right, 119, 20);
  lv_obj_set_style_bg_color(right, lv_color_hex(0x4d4d4d), LV_PART_MAIN);
  lv_obj_set_style_radius(right, 0, LV_PART_MAIN);

  create_caption_label(title, 2, y + 2);
  s_value_labels[static_cast<uint8_t>(value_id)] = create_value_label(140, y + 2);
}
}

void bias_settings_create() {
  for (uint8_t i = 0; i < static_cast<uint8_t>(BiasFieldId::Count); ++i) {
    s_value_labels[i] = nullptr;
  }

  lv_obj_t *scr = lv_screen_active();
  lv_obj_clean(scr);

  lv_obj_t *screen_bg = lv_obj_create(scr);
  lv_obj_set_pos(screen_bg, 0, 0);
  lv_obj_set_size(screen_bg, 240, 240);
  lv_obj_set_style_bg_color(screen_bg, lv_color_hex(0x000000), LV_PART_MAIN);
  lv_obj_set_style_radius(screen_bg, 0, LV_PART_MAIN);

  lv_obj_t *header_panel = lv_obj_create(lv_screen_active());
  lv_obj_set_pos(header_panel, 0, 0);
  lv_obj_set_size(header_panel, 240, 14);
  lv_obj_set_style_bg_color(header_panel, lv_color_hex(0x0f446c), LV_PART_MAIN);
  lv_obj_set_style_radius(header_panel, 0, LV_PART_MAIN);

  create_caption_label("BIAS SETTINGS", 0, -1);

  create_row(16, "MODE", BiasFieldId::Mode);
  create_row(38, "DITHER FREQ", BiasFieldId::DitherFreq);
  create_row(60, "DITHER AMP", BiasFieldId::DitherAmp);
  create_row(82, "DITHER", BiasFieldId::DitherEnable);
  create_row(104, "SLOPE", BiasFieldId::Slope);

  create_caption_label("BIAS VOLTAGE:", 2, 131, lv_color_hex(0xf44336));
  s_value_labels[static_cast<uint8_t>(BiasFieldId::BiasVoltage)] =
      create_value_label(140, 130, lv_color_hex(0xf44336));

  create_caption_label("OPTICAL OUTPUT:", 2, 155, lv_color_hex(0xf44336));
  s_value_labels[static_cast<uint8_t>(BiasFieldId::OpticalOutput)] =
      create_value_label(140, 155, lv_color_hex(0xf44336));
}

void bias_settings_apply_state(const AppState &state) {
  char buf[24];

  std::snprintf(buf, sizeof(buf), "%u", app_state_get_bias_field(state, BiasFieldId::Mode));
  set_bias_value_text(BiasFieldId::Mode, buf);

  std::snprintf(buf, sizeof(buf), "%u", app_state_get_bias_field(state, BiasFieldId::DitherFreq));
  set_bias_value_text(BiasFieldId::DitherFreq, buf);

  std::snprintf(buf, sizeof(buf), "%u", app_state_get_bias_field(state, BiasFieldId::DitherAmp));
  set_bias_value_text(BiasFieldId::DitherAmp, buf);

  set_bias_value_text(BiasFieldId::DitherEnable,
                      app_state_get_bias_field(state, BiasFieldId::DitherEnable) ? "ON" : "OFF");

  std::snprintf(buf, sizeof(buf), "%u", app_state_get_bias_field(state, BiasFieldId::Slope));
  set_bias_value_text(BiasFieldId::Slope, buf);

  std::snprintf(buf, sizeof(buf), "%u", app_state_get_bias_field(state, BiasFieldId::BiasVoltage));
  set_bias_value_text(BiasFieldId::BiasVoltage, buf);

  std::snprintf(buf, sizeof(buf), "%u", app_state_get_bias_field(state, BiasFieldId::OpticalOutput));
  set_bias_value_text(BiasFieldId::OpticalOutput, buf);
}
