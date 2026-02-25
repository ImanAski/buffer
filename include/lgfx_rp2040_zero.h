#pragma once
// #define LGFX_USE_V1

#include "LovyanGFX.hpp"


class LGFX : public lgfx::LGFX_Device {
    lgfx::Panel_GC9A01 _panel;
    lgfx::Bus_SPI _bus;
public:
    LGFX() {
        {
            auto cfg = _bus.config();
            cfg.spi_host = 0;
            cfg.spi_mode = 0;
            cfg.freq_write = 40000000;
            // cfg.freq_read = 16000000;
            cfg.pin_dc = 4;
            cfg.pin_mosi = 3;
            cfg.pin_sclk = 2;
            cfg.pin_miso = -1;

            _bus.config(cfg);
            _panel.setBus(&_bus);
        }

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
