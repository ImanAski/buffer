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

//* Local input configuration
#ifndef HW_INPUT_PIN
#define HW_INPUT_PIN -1
#endif

#ifndef HW_INPUT_ACTIVE_LOW
#define HW_INPUT_ACTIVE_LOW 1
#endif

//* Protocol debug (USB serial logs)
#ifndef PROTOCOL_DEBUG
#define PROTOCOL_DEBUG 1
#endif

#ifndef PROTOCOL_DEBUG_RAW_BYTES
#define PROTOCOL_DEBUG_RAW_BYTES 0
#endif

//* SPLASH SCREEN
#define SPLASH_TEXT "LAB EQUIP"
