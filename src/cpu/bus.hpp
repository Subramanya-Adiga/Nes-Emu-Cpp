#pragma once
#include "../cartridge/cartridge.hpp"
#include "../ppu/ppu.hpp"

namespace nes_emu {

struct Bus {
  bool nmi{};
  bool irq{};

  PPU *ppu = nullptr;
  Cartridge *cartridge = nullptr;
  std::array<uint8_t, 2> controllers{};

  [[nodiscard]] uint8_t read(uint16_t addr) noexcept;
  void write(uint16_t addr, uint8_t data) noexcept;

  std::array<uint8_t, 2048> memory{};

private:
  std::array<uint8_t, 2> m_controller_state{};
};

} // namespace nes_emu
