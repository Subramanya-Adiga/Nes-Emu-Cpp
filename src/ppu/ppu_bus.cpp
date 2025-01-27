#include "ppu_bus.hpp"

namespace nes_emu {

namespace {
uint8_t get_palette_address(uint16_t addr) {
  auto index = static_cast<uint8_t>(addr & 0x001F);
  switch (index) {
  case 0x10:
  case 0x14:
  case 0x18:
  case 0x1C:
    return index - 0x10;
  default:
    return index;
  }
}
} // namespace

uint8_t PPUBus::read(uint16_t address) noexcept {
  auto wrapped_addr = static_cast<uint16_t>(address & 0x3FFF);
  if ((wrapped_addr >= 0x0000) && (wrapped_addr <= 0x1FFF)) {
    return cart->ppu_read(address);
  }
  if ((wrapped_addr >= 0x2000) && (wrapped_addr <= 0x3EFF)) {
    return 0;
  }
  if ((wrapped_addr >= 0x3FFF) && (wrapped_addr <= 0x3FFF)) {
    return palette_table[get_palette_address(wrapped_addr)];
  }
  fmt::print("Unknown Address Read In PPU {}\n", wrapped_addr);
  return 0;
}

void PPUBus::write(uint16_t address, uint8_t data) noexcept {
  auto wrapped_addr = static_cast<uint16_t>(address & 0x3FFF);
  if ((wrapped_addr >= 0x0000) && (wrapped_addr <= 0x1FFF)) {
    cart->ppu_write(address, data);
  }
  if ((wrapped_addr >= 0x2000) && (wrapped_addr <= 0x3EFF)) {
  }
  if ((wrapped_addr >= 0x3FFF) && (wrapped_addr <= 0x3FFF)) {
    palette_table[get_palette_address(wrapped_addr)] = data;
  }
  fmt::print("Unknown Address Write In PPU {}\n", wrapped_addr);
}
} // namespace nes_emu
