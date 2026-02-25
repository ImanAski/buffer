#pragma once

#include "event_types.h"
#include "app/app_state.h"

void event_dispatcher_init(AppState *state);
void event_dispatcher_dispatch(const Event &event);
