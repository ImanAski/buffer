#pragma once

//* Display
#define TFT_WIDTH 240
#define TFT_HEIGHT 240

//* Display pins
#define TFT_SCL 2 /* Could be SCLK */
#define TFT_SDA 3 /* Usually reffered as MOSI */
#define TFT_RES 6 /* Could be RST or RESET */
#define TFT_DC 4
#define TFT_CS 5 /* Chip Select*/
#define TFT_BL 7 /* Backlight (controlled with PWM)*/
#define TFT_MISO -1

//* UART pins
#define UART_BAUDRATE 115200
#define UART_TX 0
#define UART_RX 1


//* SPLASH SCREEN
#define SPLASH_TEXT "LAB EQUIP"