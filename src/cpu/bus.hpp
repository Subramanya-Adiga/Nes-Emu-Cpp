#pragma once
#include <cartridge/cartridge.hpp>
#include <ppu/ppu.hpp>

namespace nes_emu {

struct Bus {
  bool nmi{};
  bool irq{};
  bool dma_transfer = false;
  uint32_t clock_counter = 0;

  PPU *ppu = nullptr;
  Cartridge *cartridge = nullptr;
  std::array<uint8_t, 2> controllers{};

  [[nodiscard]] uint8_t read(uint16_t addr) noexcept;
  void write(uint16_t addr, uint8_t data) noexcept;

  std::array<uint8_t, 2048> memory{};

  void clock();
  void reset();

private:
  std::array<uint8_t, 2> m_controller_state{};
  bool dma_wait = true;
  uint8_t dma_data = 0;
  uint8_t dma_page = 0;
  uint8_t dma_addr = 0;
};

} // namespace nes_emu
