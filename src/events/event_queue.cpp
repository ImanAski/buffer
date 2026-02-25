#include "event_queue.h"

namespace {
constexpr uint8_t kQueueSize = 32;
Event s_queue[kQueueSize];
uint8_t s_head = 0;
uint8_t s_tail = 0;
uint8_t s_count = 0;
uint8_t s_dropped = 0;
}

bool event_queue_push(const Event &e) {
  if (s_count >= kQueueSize) {
    if (s_dropped < 255) {
      ++s_dropped;
    }
    return false;
  }

  s_queue[s_head] = e;
  s_head = static_cast<uint8_t>((s_head + 1) % kQueueSize);
  ++s_count;
  return true;
}

bool event_queue_pop(Event &out) {
  if (s_count == 0) {
    return false;
  }

  out = s_queue[s_tail];
  s_tail = static_cast<uint8_t>((s_tail + 1) % kQueueSize);
  --s_count;
  return true;
}

uint8_t event_queue_dropped_count() {
  return s_dropped;
}

void event_queue_reset() {
  s_head = 0;
  s_tail = 0;
  s_count = 0;
  s_dropped = 0;
}
