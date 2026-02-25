#include <Arduino.h>
#include <LovyanGFX.hpp>
#include "lgfx_rp2040_zero.h"
#include <lvgl.h>
#include "Config.h"

static LGFX lcd;
// static lv_display_t *disp;

static constexpr uint32_t DRAW_BUF_PIXELS = TFT_WIDTH * 30;
static lv_color_t buf1[DRAW_BUF_PIXELS];
static lv_color_t buf2[DRAW_BUF_PIXELS];

static uint32_t my_tick_cb() {
  return millis();
}

static void my_flush_cb(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map) {
  const uint32_t w = lv_area_get_width(area);
  const uint32_t h = lv_area_get_height(area);

  // lv_draw_sw_rgb565_swap(px_map, w * h);
  // lcd.pushImage(area->x1, area->y1, w, h, (const uint16_t *)px_map);
  
  lcd.startWrite();
  lcd.setAddrWindow(area->x1, area->y1, w, h);
  lcd.pushPixels((uint16_t *)px_map, w * h, true);
  lcd.endWrite();
  lv_display_flush_ready(disp);
}


static void show_list_cb(lv_timer_t *timer)
{
    // Clear current screen
    lv_obj_clean(lv_screen_active());

    // Create list
    lv_obj_t *list = lv_list_create(lv_screen_active());
    lv_obj_set_size(list, 200, 200);
    lv_obj_center(list);

    lv_list_add_text(list, "Menu");

    lv_list_add_btn(list, LV_SYMBOL_FILE, "File");
    lv_list_add_btn(list, LV_SYMBOL_DIRECTORY, "Folder");
    lv_list_add_btn(list, LV_SYMBOL_SAVE, "Save");
}

void splash_scren() {
  lv_obj_t *label = lv_label_create(lv_screen_active());
  lv_label_set_text(label, SPLASH_TEXT);
  lv_obj_center(label);
}

lv_obj_t* create_settings_row(lv_obj_t *parent, const char *title)
{
    // Main row container
    lv_obj_t *row = lv_obj_create(parent);
    lv_obj_set_width(row, LV_PCT(100));
    lv_obj_set_height(row, 70);

    lv_obj_set_style_pad_all(row, 10, 0);
    lv_obj_set_style_radius(row, 0, 0);

    lv_obj_set_flex_flow(row, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(row,
                          LV_FLEX_ALIGN_START,
                          LV_FLEX_ALIGN_CENTER,
                          LV_FLEX_ALIGN_CENTER);

    // Make row clickable
    lv_obj_add_flag(row, LV_OBJ_FLAG_CLICKABLE);
    // lv_obj_add_event_cb(row, row_event_cb, LV_EVENT_CLICKED, NULL);

    // === Square Toggle Button ===
    lv_obj_t *toggle = lv_btn_create(row);
    lv_obj_set_size(toggle, 40, 40);

    lv_obj_add_flag(toggle, LV_OBJ_FLAG_CHECKABLE);
    // lv_obj_add_event_cb(toggle, toggle_event_cb, LV_EVENT_CLICKED, NULL);

    // Default style
    lv_obj_set_style_bg_color(toggle, lv_palette_main(LV_PALETTE_GREY), 0);

    // Checked style
    lv_obj_set_style_bg_color(toggle,
                              lv_palette_main(LV_PALETTE_BLUE),
                              LV_PART_MAIN | LV_STATE_CHECKED);

    // Icon inside toggle
    lv_obj_t *icon = lv_label_create(toggle);
    lv_label_set_text(icon, LV_SYMBOL_OK);
    lv_obj_center(icon);

    // === Title Label ===
    lv_obj_t *label = lv_label_create(row);
    lv_label_set_text(label, title);
    lv_obj_set_style_pad_left(label, 15, 0);

    return row;
}

void build_main_page()
{
    lv_obj_t *scr = lv_screen_active();

    lv_obj_set_style_bg_color(scr, lv_color_hex(0x121212), 0);
    lv_obj_set_style_bg_opa(scr, LV_OPA_COVER, 0);

    // Scrollable container
    lv_obj_t *list = lv_obj_create(scr);
    lv_obj_set_size(list, LV_PCT(100), LV_PCT(100));

    lv_obj_set_scroll_dir(list, LV_DIR_VER);
    lv_obj_set_style_pad_all(list, 0, 0);

    lv_obj_set_flex_flow(list, LV_FLEX_FLOW_COLUMN);

    // Add rows
    create_settings_row(list, "WiFi");
    create_settings_row(list, "Bluetooth");
    create_settings_row(list, "Display");
    create_settings_row(list, "System");
}

void setup() {
  Serial.begin(115200);
  
  pinMode(7, OUTPUT);
  digitalWrite(7, HIGH);

  lv_init();
  lv_tick_set_cb(my_tick_cb);

  lcd.init();
  lcd.setRotation(0);
  lcd.setBrightness(255);

  // disp = lv_display_create(TFT_WIDTH, TFT_HEIGHT);
  lv_display_t *disp = lv_display_create(240, 240);
  uint8_t *buf = (uint8_t *)lv_malloc(240 * 100 * 2);
  lv_display_set_color_format(disp, LV_COLOR_FORMAT_RGB565);
  lv_display_set_flush_cb(disp, my_flush_cb);
  lv_display_set_buffers(disp, buf, NULL, 240 * 100 * 2, LV_DISPLAY_RENDER_MODE_PARTIAL);
  // lv_display_set_buffers(disp, buf1, buf2, sizeof(buf1), LV_DISPLAY_RENDER_MODE_PARTIAL);

  build_main_page();
  // lv_obj_t *label = lv_label_create(lv_screen_active());
  // lv_label_set_text(label, "Hello world");
  // lv_obj_center(label);

  // lv_timer_create(show_list_cb, 4000, NULL);  // 4000 ms = 4 sec
}

void loop() {
  lv_timer_handler();
  delay(5);
}
