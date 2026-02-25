#include "display_port.h"

#include <Arduino.h>
#include <LovyanGFX.hpp>
#include <lvgl.h>

#include "Config.h"
#include "lgfx_rp2040_zero.h"

namespace {
LGFX s_lcd;

uint32_t my_tick_cb() {
  return millis();
}

void my_flush_cb(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map) {
  const uint32_t w = lv_area_get_width(area);
  const uint32_t h = lv_area_get_height(area);

  s_lcd.startWrite();
  s_lcd.setAddrWindow(area->x1, area->y1, w, h);
  s_lcd.pushPixels(reinterpret_cast<uint16_t *>(px_map), w * h, true);
  s_lcd.endWrite();

  lv_display_flush_ready(disp);
}
}

void display_port_init() {
  pinMode(7, OUTPUT);
  digitalWrite(7, HIGH);

  lv_init();
  lv_tick_set_cb(my_tick_cb);

  s_lcd.init();
  s_lcd.setRotation(0);
  s_lcd.setBrightness(255);

  lv_display_t *disp = lv_display_create(TFT_WIDTH, TFT_HEIGHT);
  uint8_t *buf = static_cast<uint8_t *>(lv_malloc(TFT_WIDTH * 100 * 2));

  lv_display_set_color_format(disp, LV_COLOR_FORMAT_RGB565);
  lv_display_set_flush_cb(disp, my_flush_cb);
  lv_display_set_buffers(disp, buf, nullptr, TFT_WIDTH * 100 * 2, LV_DISPLAY_RENDER_MODE_PARTIAL);
}

void display_port_task() {
  lv_timer_handler();
  delay(5);
}
