#include "settings_screen.h"

#include <Arduino.h>
#include <lvgl.h>

#include "events/event_queue.h"

namespace {
struct RowWidgets {
  lv_obj_t *row;
  lv_obj_t *toggle;
};

RowWidgets s_rows[static_cast<uint8_t>(ToggleId::Count)] = {};
lv_obj_t *s_title = nullptr;
bool s_applying_state = false;

const char *kLabels[static_cast<uint8_t>(ToggleId::Count)] = {
  "WiFi",
  "Bluetooth",
  "Display",
  "System"
};

Event make_toggle_event(ToggleId id, bool value) {
  Event e{};
  e.type = EventType::UiToggleChanged;
  e.ts_ms = millis();
  e.data.toggle.toggle_id = id;
  e.data.toggle.value = value;
  return e;
}

Event make_row_event(ToggleId id) {
  Event e{};
  e.type = EventType::UiRowPressed;
  e.ts_ms = millis();
  e.data.row.row_id = id;
  return e;
}

Event make_nav_event(ScreenId screen) {
  Event e{};
  e.type = EventType::UiNavigate;
  e.ts_ms = millis();
  e.data.screen.screen_id = screen;
  return e;
}

lv_color_t background_for_theme(ThemeId theme) {
  switch (theme) {
    case ThemeId::Light:
      return lv_color_hex(0xEFEFEF);
    case ThemeId::Ocean:
      return lv_color_hex(0x0B1D2A);
    case ThemeId::Dark:
    default:
      return lv_color_hex(0x121212);
  }
}

lv_color_t title_for_theme(ThemeId theme) {
  switch (theme) {
    case ThemeId::Light:
      return lv_color_hex(0x202020);
    case ThemeId::Ocean:
      return lv_color_hex(0xA8DBFF);
    case ThemeId::Dark:
    default:
      return lv_color_hex(0xF0F0F0);
  }
}

void toggle_event_cb(lv_event_t *e) {
  if (s_applying_state) {
    return;
  }

  const auto id_raw = reinterpret_cast<uintptr_t>(lv_event_get_user_data(e));
  const ToggleId id = static_cast<ToggleId>(static_cast<uint8_t>(id_raw));
  lv_obj_t *toggle = static_cast<lv_obj_t *>(lv_event_get_target(e));
  const bool checked = lv_obj_has_state(toggle, LV_STATE_CHECKED);
  event_queue_push(make_toggle_event(id, checked));
}

void row_event_cb(lv_event_t *e) {
  if (s_applying_state) {
    return;
  }

  const auto id_raw = reinterpret_cast<uintptr_t>(lv_event_get_user_data(e));
  const ToggleId id = static_cast<ToggleId>(static_cast<uint8_t>(id_raw));
  event_queue_push(make_row_event(id));
}

void themes_nav_cb(lv_event_t *e) {
  LV_UNUSED(e);
  event_queue_push(make_nav_event(ScreenId::Themes));
}

void home_nav_cb(lv_event_t *e) {
  LV_UNUSED(e);
  event_queue_push(make_nav_event(ScreenId::Home));
}

lv_obj_t *create_settings_row(lv_obj_t *parent, ToggleId id, const char *title) {
  lv_obj_t *row = lv_obj_create(parent);
  lv_obj_set_width(row, LV_PCT(100));
  lv_obj_set_height(row, 70);

  lv_obj_set_style_pad_all(row, 10, 0);
  lv_obj_set_style_radius(row, 0, 0);
  lv_obj_set_flex_flow(row, LV_FLEX_FLOW_ROW);
  lv_obj_set_flex_align(row, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
  lv_obj_add_flag(row, LV_OBJ_FLAG_CLICKABLE);
  lv_obj_add_event_cb(row, row_event_cb, LV_EVENT_CLICKED, reinterpret_cast<void *>(static_cast<uintptr_t>(static_cast<uint8_t>(id))));

  lv_obj_t *toggle = lv_btn_create(row);
  lv_obj_set_size(toggle, 40, 40);
  lv_obj_add_flag(toggle, LV_OBJ_FLAG_CHECKABLE);
  lv_obj_set_style_bg_color(toggle, lv_palette_main(LV_PALETTE_GREY), 0);
  lv_obj_set_style_bg_color(toggle, lv_palette_main(LV_PALETTE_BLUE), LV_PART_MAIN | LV_STATE_CHECKED);
  lv_obj_add_event_cb(toggle, toggle_event_cb, LV_EVENT_CLICKED, reinterpret_cast<void *>(static_cast<uintptr_t>(static_cast<uint8_t>(id))));

  lv_obj_t *icon = lv_label_create(toggle);
  lv_label_set_text(icon, LV_SYMBOL_OK);
  lv_obj_center(icon);

  lv_obj_t *label = lv_label_create(row);
  lv_label_set_text(label, title);
  lv_obj_set_style_pad_left(label, 15, 0);

  s_rows[static_cast<uint8_t>(id)] = {row, toggle};
  return row;
}
}

void settings_screen_create() {
  lv_obj_t *scr = lv_screen_active();

  lv_obj_clean(scr);
  lv_obj_set_style_bg_color(scr, lv_color_hex(0x121212), 0);
  lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, 0);

  lv_obj_t *list = lv_obj_create(scr);
  lv_obj_set_size(list, LV_PCT(100), LV_PCT(100));
  lv_obj_set_scroll_dir(list, LV_DIR_VER);
  lv_obj_set_style_pad_all(list, 0, 0);
  lv_obj_set_flex_flow(list, LV_FLEX_FLOW_COLUMN);

  s_title = lv_label_create(list);
  lv_label_set_text(s_title, "Settings");
  lv_obj_set_style_pad_all(s_title, 12, 0);

  lv_obj_t *nav_row = lv_obj_create(list);
  lv_obj_set_width(nav_row, LV_PCT(100));
  lv_obj_set_height(nav_row, LV_SIZE_CONTENT);
  lv_obj_set_style_pad_all(nav_row, 8, 0);
  lv_obj_set_flex_flow(nav_row, LV_FLEX_FLOW_ROW);
  lv_obj_set_style_border_width(nav_row, 0, 0);

  lv_obj_t *home_btn = lv_btn_create(nav_row);
  lv_obj_add_event_cb(home_btn, home_nav_cb, LV_EVENT_CLICKED, nullptr);
  lv_obj_t *home_lbl = lv_label_create(home_btn);
  lv_label_set_text(home_lbl, "Home");
  lv_obj_center(home_lbl);

  lv_obj_t *theme_btn = lv_btn_create(nav_row);
  lv_obj_add_event_cb(theme_btn, themes_nav_cb, LV_EVENT_CLICKED, nullptr);
  lv_obj_t *theme_lbl = lv_label_create(theme_btn);
  lv_label_set_text(theme_lbl, "Themes");
  lv_obj_center(theme_lbl);

  for (uint8_t i = 0; i < static_cast<uint8_t>(ToggleId::Count); ++i) {
    create_settings_row(list, static_cast<ToggleId>(i), kLabels[i]);
  }
}

void settings_screen_apply_state(const AppState &state) {
  s_applying_state = true;
  lv_obj_t *scr = lv_screen_active();
  lv_obj_set_style_bg_color(scr, background_for_theme(state.theme), 0);
  lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, 0);
  if (s_title != nullptr) {
    lv_obj_set_style_text_color(s_title, title_for_theme(state.theme), 0);
  }

  for (uint8_t i = 0; i < static_cast<uint8_t>(ToggleId::Count); ++i) {
    lv_obj_t *toggle = s_rows[i].toggle;
    if (toggle == nullptr) {
      continue;
    }

    if (state.toggles[i]) {
      lv_obj_add_state(toggle, LV_STATE_CHECKED);
    } else {
      lv_obj_remove_state(toggle, LV_STATE_CHECKED);
    }
  }

  s_applying_state = false;
}
