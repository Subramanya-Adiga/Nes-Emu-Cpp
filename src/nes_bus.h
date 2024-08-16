#pragma once
#include "nes_cpu.h"

namespace nes_emu {

struct nes_bus {
  nes_cpu cpu;

  std::array<uint8_t, 64 * 1024> memory{};

  nes_bus();

  void write(uint16_t addr, uint8_t data) noexcept;
  [[nodiscard]] uint8_t read(uint16_t addr) noexcept;
};

} // namespace nes_emu
