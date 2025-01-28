#pragma once
#include "../cartridge/cartridge.hpp"
#include "../ppu/ppu.hpp"
#include "cpu.hpp"

namespace nes_emu {

struct Bus {
  bool nmi{};
  bool irq{};

  PPU ppu;
  Cpu cpu;
  std::array<uint8_t, 2> controllers{};

  Bus();

  [[nodiscard]] uint8_t read(uint16_t addr) noexcept;
  void write(uint16_t addr, uint8_t data) noexcept;

  void load_cartridge(std::string_view path) noexcept;
  uint32_t clock_counter() const noexcept;

  void clock() noexcept;
  void reset() noexcept;

private:
  std::array<uint8_t, 2048> memory{};
  std::array<uint8_t, 2> m_controller_state{};
  std::unique_ptr<Cartridge> cart;
  uint32_t counter{};
};

} // namespace nes_emu
