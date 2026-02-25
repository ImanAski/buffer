#include "settings_store.h"

#include <cstring>
#include "drivers/FlashIAP.h"

namespace {
constexpr uint32_t kMagic = 0x42554646UL;  // "BUFF"
constexpr uint8_t kVersion = 1;
constexpr size_t kScratchMax = 512;

struct PersistedSettings {
  uint32_t magic;
  uint8_t version;
  uint8_t theme;
  uint8_t active_screen;
  uint8_t toggles[static_cast<uint8_t>(ToggleId::Count)];
  uint8_t crc;
};

uint8_t calc_crc(const PersistedSettings &cfg) {
  uint8_t crc = 0;
  const uint8_t *bytes = reinterpret_cast<const uint8_t *>(&cfg);
  for (size_t i = 0; i < sizeof(PersistedSettings) - 1; ++i) {
    crc ^= bytes[i];
  }
  return crc;
}

bool flash_region(uint32_t &addr, uint32_t &sector_size, uint32_t &program_size) {
  mbed::FlashIAP flash;
  if (flash.init() != 0) {
    return false;
  }

  const uint32_t flash_start = flash.get_flash_start();
  const uint32_t flash_size = flash.get_flash_size();
  const uint32_t flash_end = flash_start + flash_size;

  sector_size = flash.get_sector_size(flash_end - 1);
  program_size = flash.get_page_size();
  addr = flash_end - sector_size;

  flash.deinit();
  return true;
}

bool read_raw(PersistedSettings &cfg) {
  uint32_t addr = 0;
  uint32_t sector_size = 0;
  uint32_t program_size = 0;
  if (!flash_region(addr, sector_size, program_size)) {
    return false;
  }

  mbed::FlashIAP flash;
  if (flash.init() != 0) {
    return false;
  }

  int rc = flash.read(&cfg, addr, sizeof(PersistedSettings));
  flash.deinit();
  return rc == 0;
}

bool write_raw(const PersistedSettings &cfg) {
  uint32_t addr = 0;
  uint32_t sector_size = 0;
  uint32_t program_size = 0;
  if (!flash_region(addr, sector_size, program_size)) {
    return false;
  }

  const size_t padded_size =
      ((sizeof(PersistedSettings) + program_size - 1) / program_size) * program_size;
  if (padded_size > kScratchMax) {
    return false;
  }

  uint8_t scratch[kScratchMax];
  std::memset(scratch, 0xFF, padded_size);
  std::memcpy(scratch, &cfg, sizeof(PersistedSettings));

  mbed::FlashIAP flash;
  if (flash.init() != 0) {
    return false;
  }

  const int erase_rc = flash.erase(addr, sector_size);
  int program_rc = 0;
  if (erase_rc == 0) {
    program_rc = flash.program(scratch, addr, padded_size);
  }
  flash.deinit();
  return erase_rc == 0 && program_rc == 0;
}
}

bool settings_store_load(AppState &state) {
  PersistedSettings cfg{};
  if (!read_raw(cfg)) {
    return false;
  }

  if (cfg.magic != kMagic || cfg.version != kVersion || cfg.crc != calc_crc(cfg)) {
    return false;
  }

  if (cfg.theme < static_cast<uint8_t>(ThemeId::Count)) {
    state.theme = static_cast<ThemeId>(cfg.theme);
  }

  if (cfg.active_screen < static_cast<uint8_t>(ScreenId::Count)) {
    state.active_screen = static_cast<ScreenId>(cfg.active_screen);
  }

  for (uint8_t i = 0; i < static_cast<uint8_t>(ToggleId::Count); ++i) {
    state.toggles[i] = cfg.toggles[i] != 0;
  }

  return true;
}

bool settings_store_save(const AppState &state) {
  PersistedSettings cfg{};
  cfg.magic = kMagic;
  cfg.version = kVersion;
  cfg.theme = static_cast<uint8_t>(state.theme);
  cfg.active_screen = static_cast<uint8_t>(state.active_screen);
  for (uint8_t i = 0; i < static_cast<uint8_t>(ToggleId::Count); ++i) {
    cfg.toggles[i] = state.toggles[i] ? 1 : 0;
  }
  cfg.crc = calc_crc(cfg);

  return write_raw(cfg);
}
