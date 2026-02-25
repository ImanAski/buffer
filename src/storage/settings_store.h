#pragma once

#include "app/app_state.h"

bool settings_store_load(AppState &state);
bool settings_store_save(const AppState &state);
