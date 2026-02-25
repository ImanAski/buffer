#pragma once
// #define LGFX_USE_V1

#include "LovyanGFX.hpp"
#include "Config.h"


class LGFX : public lgfx::LGFX_Device {
    lgfx::Panel_GC9A01 _panel;
    lgfx::Bus_SPI _bus;
public:
    LGFX() {
        /* SPI-Bus Config */
        {
            auto cfg = _bus.config();
            cfg.spi_host = 0;
            cfg.spi_mode = 0;
            cfg.freq_write = 80000000;
            cfg.pin_dc = TFT_DC;
            cfg.pin_mosi = TFT_SDA;
            cfg.pin_sclk = TFT_SCL;
            cfg.pin_miso = TFT_MISO;

            _bus.config(cfg);
            _panel.setBus(&_bus);
        }

        /* Panel Config */
        {
            auto cfg = _panel.config();
            cfg.pin_cs = 5;
            cfg.pin_rst = 6;
            cfg.panel_width = 240;
            cfg.panel_height = 240;

            cfg.offset_x = 0;
            cfg.offset_y = 0;
            cfg.offset_rotation = 0;
            cfg.invert = true;
            cfg.rgb_order = false;

            _panel.config(cfg);
        }

        setPanel(&_panel);
    }
};
