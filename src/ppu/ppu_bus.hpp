#pragma once
#include "../cartridge/cartridge.hpp"

namespace nes_emu {

struct PPUBus {
  Cartridge *cart = nullptr;

  [[nodiscard]] uint8_t read(uint16_t address) const noexcept;
  void write(uint16_t address, uint8_t data) noexcept;

private:
  uint8_t name_table[2][1024]{};
  std::array<uint8_t, 32> palette_table{};
};
} // namespace nes_emu
