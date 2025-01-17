#pragma once
#include "cartridge/cartridge.hpp"
#include "nes_cpu.hpp"
#include "nes_ppu.hpp"

namespace nes_emu {

struct nes_bus {
  nes_cpu cpu;
  nes_ppu ppu;
  std::array<uint8_t, 2> controllers{};
  std::unique_ptr<nes_cartridge> cartridge;

  std::array<uint8_t, 2048> memory{};

  nes_bus();

  void insert_cartridge(const std::string &nes_cart);

  void reset();
  void clock();

  void write(uint16_t addr, uint8_t data) noexcept;
  [[nodiscard]] uint8_t read(uint16_t addr) noexcept;

private:
  uint32_t m_system_counter = 0;
  std::array<uint8_t, 2> m_controller_state{};
};

} // namespace nes_emu
