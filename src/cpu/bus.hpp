#pragma once
#include <cartridge/cartridge.hpp>
#include <controller/controller.hpp>
#include <ppu/ppu.hpp>

namespace nes_emu {

struct Bus {
  bool nmi{};
  bool irq{};
  bool clock_cpu{};
  bool dma_transfer = false;
  uint32_t clock_counter = 0;

  PPU *ppu = nullptr;
  Cartridge *cartridge = nullptr;
  Controller *controller = nullptr;

  [[nodiscard]] uint8_t read(uint16_t addr) noexcept;
  void write(uint16_t addr, uint8_t data) noexcept;

  std::array<uint8_t, 2048> memory{};

  void clock();
  void reset();

private:
  bool dma_wait = true;
  uint8_t dma_data = 0;
  uint8_t dma_page = 0;
  uint8_t dma_addr = 0;
};

} // namespace nes_emu
