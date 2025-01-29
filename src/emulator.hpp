#pragma once

#include "cartridge/cartridge.hpp"
#include "cpu/bus.hpp"
#include "cpu/cpu.hpp"
#include "ppu/ppu.hpp"

namespace nes_emu {

struct Emulator {
  Cpu cpu;
  Bus bus;
  PPU ppu;
  std::unique_ptr<Cartridge> cart;
  uint32_t system_counter{};

  Emulator();

  void run() noexcept;
  void reset() noexcept;

  void load_cartridge(std::string_view path) noexcept;
};
} // namespace nes_emu