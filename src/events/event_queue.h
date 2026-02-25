#pragma once

#include "event_types.h"

bool event_queue_push(const Event &e);
bool event_queue_pop(Event &out);
uint8_t event_queue_dropped_count();
void event_queue_reset();
