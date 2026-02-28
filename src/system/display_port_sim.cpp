#include "display_port.h"

#include <Arduino.h>
#include <lvgl.h>
#include <src/drivers/sdl/lv_sdl_keyboard.h>
#include <src/drivers/sdl/lv_sdl_mouse.h>
#include <src/drivers/sdl/lv_sdl_mousewheel.h>
#include <src/drivers/sdl/lv_sdl_window.h>

#include "Config.h"

namespace {
lv_display_t *s_display = nullptr;
}

void display_port_init() {
  lv_init();

  s_display = lv_sdl_window_create(TFT_WIDTH, TFT_HEIGHT);
  lv_sdl_window_set_title(s_display, "buffer - UI simulator");
  lv_sdl_window_set_zoom(s_display, 2.0f);
  lv_sdl_window_set_resizeable(s_display, false);

  lv_indev_t *mouse = lv_sdl_mouse_create();
  lv_indev_set_display(mouse, s_display);

  lv_indev_t *keyboard = lv_sdl_keyboard_create();
  lv_indev_set_display(keyboard, s_display);

#if LV_SDL_MOUSEWHEEL_MODE == LV_SDL_MOUSEWHEEL_MODE_ENCODER
  lv_indev_t *wheel = lv_sdl_mousewheel_create();
  lv_indev_set_display(wheel, s_display);
#endif
}

void display_port_task() {
  lv_timer_handler();
  delay(5);
}
