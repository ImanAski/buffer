#include <Arduino.h>

#include "Config.h"
#include "app/app.h"

void setup() {
  Serial.begin(UART_BAUDRATE);
  app_init();
}

void loop() {
  app_tick();
}
