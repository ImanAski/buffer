#include "hw_input.h"

#include <Arduino.h>
#include "Config.h"
#include "event_queue.h"

namespace {
int s_last_level = HIGH;

bool hw_input_enabled() {
  return HW_INPUT_PIN >= 0;
}
}

void hw_input_init() {
  if (!hw_input_enabled()) {
    return;
  }

  pinMode(HW_INPUT_PIN, INPUT_PULLUP);
  s_last_level = digitalRead(HW_INPUT_PIN);
}

void hw_input_poll() {
  if (!hw_input_enabled()) {
    return;
  }

  const int current = digitalRead(HW_INPUT_PIN);
  if (current == s_last_level) {
    return;
  }

  s_last_level = current;

  Event e{};
  e.type = EventType::HwInputChanged;
  e.ts_ms = millis();
  e.data.input.input_id = 0;
#if HW_INPUT_ACTIVE_LOW
  e.data.input.value = (current == LOW) ? 1 : 0;
#else
  e.data.input.value = (current == HIGH) ? 1 : 0;
#endif

  event_queue_push(e);
}
