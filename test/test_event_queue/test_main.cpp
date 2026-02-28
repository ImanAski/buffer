#include <unity.h>

#include "events/event_queue.h"

void setUp() {
  event_queue_reset();
}

void tearDown() {}

void test_event_queue_push_and_pop() {
  Event in{};
  in.type = EventType::UiNavigate;
  in.ts_ms = 42;
  in.data.screen.screen_id = ScreenId::Bias;

  TEST_ASSERT_TRUE(event_queue_push(in));

  Event out{};
  TEST_ASSERT_TRUE(event_queue_pop(out));
  TEST_ASSERT_EQUAL_UINT32(static_cast<uint32_t>(EventType::UiNavigate),
                           static_cast<uint32_t>(out.type));
  TEST_ASSERT_EQUAL_UINT32(42, out.ts_ms);
  TEST_ASSERT_EQUAL_UINT8(static_cast<uint8_t>(ScreenId::Bias),
                          static_cast<uint8_t>(out.data.screen.screen_id));
}

void test_event_queue_reports_drop_when_full() {
  Event e{};
  e.type = EventType::UiNavigate;

  for (uint8_t i = 0; i < 32; ++i) {
    TEST_ASSERT_TRUE(event_queue_push(e));
  }

  TEST_ASSERT_FALSE(event_queue_push(e));
  TEST_ASSERT_EQUAL_UINT8(1, event_queue_dropped_count());
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_event_queue_push_and_pop);
  RUN_TEST(test_event_queue_reports_drop_when_full);
  return UNITY_END();
}
