#include "app.h"

#include <Arduino.h>
#include "app_state.h"
#include "events/event_dispatcher.h"
#include "events/event_queue.h"
#include "events/hw_input.h"
#include "events/serial_input.h"
#include "storage/settings_store.h"
#include "system/display_port.h"
#include "ui/ui_router.h"

namespace {
AppState s_state;
}

void app_init() {
  event_queue_reset();
  app_state_init(s_state);
  settings_store_load(s_state);

  display_port_init();
  ui_router_init(s_state);

  serial_input_init();
  hw_input_init();

  event_dispatcher_init(&s_state);
}

void app_tick() {
  serial_input_poll();
  hw_input_poll();

  Event e{};
  while (event_queue_pop(e)) {
    event_dispatcher_dispatch(e);
  }

  display_port_task();
}
