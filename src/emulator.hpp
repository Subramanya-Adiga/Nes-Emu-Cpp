#pragma once

#include <cartridge/cartridge.hpp>
#include <controller/controller.hpp>
#include <cpu/bus.hpp>
#include <cpu/cpu.hpp>
#include <ppu/ppu.hpp>

namespace nes_emu {

struct Emulator {
  Cpu cpu;
  Bus bus;
  PPU ppu;
  Controller controller{};
  std::unique_ptr<Cartridge> cart;
  uint32_t system_counter{};

  Emulator();

  void run() noexcept;
  void reset() noexcept;

  void set_controller_one_status(Buttons status);
  void set_controller_two_status(Buttons status);

  void load_cartridge(std::string_view path) noexcept;
};
} // namespace nes_emu