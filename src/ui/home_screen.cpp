#include "home_screen.h"

#include <cstdint>

#include <Arduino.h>
#include <lvgl.h>

#include "events/event_queue.h"

namespace {
constexpr lv_coord_t kScreenW = 240;
constexpr lv_coord_t kScreenH = 240;
constexpr lv_coord_t kTopBarH = 18;
constexpr lv_coord_t kStrapW = 44;
constexpr lv_coord_t kStrapItemH = 50;
constexpr lv_coord_t kStrapGap = 6;
constexpr lv_coord_t kStrapY = 28;

struct StrapItem {
  TabId id;
  const char *title;
  const char *icon;
};

constexpr StrapItem kStrapItems[] = {
    {TabId::Home, "HOME", LV_SYMBOL_HOME},
    {TabId::Bias, "BIAS", LV_SYMBOL_SETTINGS},
    {TabId::Load, "LOAD", LV_SYMBOL_DOWNLOAD},
    {TabId::Help, "HELP", LV_SYMBOL_WARNING},
};

lv_obj_t *s_title = nullptr;
lv_obj_t *s_strap_indicator = nullptr;

Event make_tab_event(TabId tab) {
  Event e{};
  e.type = EventType::UiTabSelected;
  e.ts_ms = millis();
  e.data.tab.tab_id = tab;
  return e;
}

int strap_index_for_tab(TabId tab) {
  for (int i = 0; i < static_cast<int>(sizeof(kStrapItems) / sizeof(kStrapItems[0])); ++i) {
    if (kStrapItems[i].id == tab) {
      return i;
    }
  }
  return 0;
}

const char *title_for_tab(TabId tab) {
  return kStrapItems[strap_index_for_tab(tab)].title;
}

lv_coord_t indicator_y_for_index(int index) {
  return kStrapY + (index * (kStrapItemH + kStrapGap));
}

void strap_btn_cb(lv_event_t *e) {
  uintptr_t idx = reinterpret_cast<uintptr_t>(lv_event_get_user_data(e));
  if (idx >= (sizeof(kStrapItems) / sizeof(kStrapItems[0]))) {
    return;
  }
  event_queue_push(make_tab_event(kStrapItems[idx].id));
}

lv_color_t background_for_theme(ThemeId theme) {
  switch (theme) {
    case ThemeId::Light:
      return lv_color_hex(0xF6F6F6);
    case ThemeId::Ocean:
      return lv_color_hex(0x041722);
    case ThemeId::Dark:
    default:
      return lv_color_hex(0x000000);
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
  lv_obj_t *scr = lv_screen_active();
  lv_obj_clean(scr);

  lv_obj_t *bg = lv_obj_create(scr);
  lv_obj_set_pos(bg, 0, 0);
  lv_obj_set_size(bg, kScreenW, kScreenH);
  lv_obj_set_style_radius(bg, 0, 0);
  lv_obj_set_style_border_width(bg, 0, 0);
  lv_obj_set_style_pad_all(bg, 0, 0);
  lv_obj_set_scrollbar_mode(bg, LV_SCROLLBAR_MODE_OFF);

  lv_obj_t *top = lv_obj_create(bg);
  lv_obj_set_pos(top, 0, 0);
  lv_obj_set_size(top, kScreenW, kTopBarH);
  lv_obj_set_style_radius(top, 0, 0);
  lv_obj_set_style_border_width(top, 0, 0);
  lv_obj_set_style_bg_color(top, lv_color_hex(0x0F446C), 0);
  lv_obj_set_style_pad_all(top, 0, 0);

  s_title = lv_label_create(top);
  lv_label_set_text(s_title, "HOME");
  lv_obj_set_pos(s_title, 4, 1);

  lv_obj_t *content = lv_obj_create(bg);
  lv_obj_set_pos(content, 0, kTopBarH);
  lv_obj_set_size(content, kScreenW - kStrapW, kScreenH - kTopBarH);
  lv_obj_set_style_radius(content, 0, 0);
  lv_obj_set_style_border_width(content, 0, 0);
  lv_obj_set_style_bg_opa(content, LV_OPA_TRANSP, 0);
  lv_obj_set_style_pad_all(content, 6, 0);

  lv_obj_t *hint = lv_label_create(content);
  lv_label_set_text(hint, "Use right strap\nfor tab select");
  lv_obj_set_style_text_color(hint, lv_color_hex(0xB0B0B0), 0);
  lv_obj_center(hint);

  lv_obj_t *strap = lv_obj_create(bg);
  lv_obj_set_pos(strap, kScreenW - kStrapW, kTopBarH);
  lv_obj_set_size(strap, kStrapW, kScreenH - kTopBarH);
  lv_obj_set_style_radius(strap, 0, 0);
  lv_obj_set_style_border_width(strap, 0, 0);
  lv_obj_set_style_bg_color(strap, lv_color_hex(0x000000), 0);
  lv_obj_set_style_pad_all(strap, 0, 0);

  s_strap_indicator = lv_obj_create(bg);
  lv_obj_set_size(s_strap_indicator, 4, kStrapItemH);
  lv_obj_set_pos(s_strap_indicator, kScreenW - kStrapW, kStrapY);
  lv_obj_set_style_radius(s_strap_indicator, 0, 0);
  lv_obj_set_style_border_width(s_strap_indicator, 0, 0);
  lv_obj_set_style_bg_color(s_strap_indicator, lv_color_hex(0x20C997), 0);

  for (uint32_t i = 0; i < (sizeof(kStrapItems) / sizeof(kStrapItems[0])); ++i) {
    const lv_coord_t target_y = indicator_y_for_index(static_cast<int>(i));

    lv_obj_t *btn = lv_button_create(bg);
    lv_obj_set_pos(btn, kScreenW - kStrapW + 4, target_y + 8);
    lv_obj_set_size(btn, kStrapW - 8, kStrapItemH);
    lv_obj_set_style_radius(btn, 6, 0);
    lv_obj_set_style_border_width(btn, 0, 0);
    lv_obj_set_style_bg_color(btn, lv_color_hex(0x111111), 0);
    lv_obj_set_style_bg_opa(btn, LV_OPA_60, 0);
    lv_obj_add_event_cb(btn, strap_btn_cb, LV_EVENT_CLICKED,
                        reinterpret_cast<void *>(static_cast<uintptr_t>(i)));

    lv_obj_t *icon = lv_label_create(btn);
    lv_label_set_text(icon, kStrapItems[i].icon);
    lv_obj_set_style_text_color(icon, lv_color_hex(0xFFFFFF), 0);
    lv_obj_center(icon);

    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, btn);
    lv_anim_set_exec_cb(&a, reinterpret_cast<lv_anim_exec_xcb_t>(lv_obj_set_y));
    lv_anim_set_values(&a, target_y + 8, target_y);
    lv_anim_set_delay(&a, static_cast<int32_t>(i) * 45);
    lv_anim_set_duration(&a, 170);
    lv_anim_set_path_cb(&a, lv_anim_path_ease_out);
    lv_anim_start(&a);
  }
}

void home_screen_apply_state(const AppState &state) {
  lv_obj_t *scr = lv_screen_active();
  lv_obj_set_style_bg_color(scr, background_for_theme(state.theme), 0);
  lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, 0);

  if (s_title != nullptr) {
    lv_label_set_text(s_title, title_for_tab(state.active_tab));
    lv_obj_set_style_text_color(s_title, title_for_theme(state.theme), 0);
  }

  if (s_strap_indicator != nullptr) {
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, s_strap_indicator);
    lv_anim_set_exec_cb(&a, reinterpret_cast<lv_anim_exec_xcb_t>(lv_obj_set_y));
    lv_anim_set_values(&a, lv_obj_get_y(s_strap_indicator),
                       indicator_y_for_index(strap_index_for_tab(state.active_tab)));
    lv_anim_set_duration(&a, 200);
    lv_anim_set_path_cb(&a, lv_anim_path_ease_out);
    lv_anim_start(&a);
  }
}
