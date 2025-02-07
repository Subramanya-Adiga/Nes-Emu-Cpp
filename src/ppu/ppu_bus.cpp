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

uint8_t PPUBus::read(uint16_t address) const noexcept {

  auto wrapped_addr = static_cast<uint16_t>(address & 0x3FFF);
  if ((wrapped_addr >= 0x0000) && (wrapped_addr <= 0x1FFF)) {
    return cart->ppu_read(address);
  }
  if ((wrapped_addr >= 0x2000) && (wrapped_addr <= 0x3EFF)) {
    auto addr = wrapped_addr & 0x0FFF;
    uint8_t data{};

    if (cart->mirror == Mirror::VERTICAL) {
      // Vertical
      if (addr >= 0x0000 && addr <= 0x03FF)
        data = name_table[0][addr & 0x03FF];
      if (addr >= 0x0400 && addr <= 0x07FF)
        data = name_table[1][addr & 0x03FF];
      if (addr >= 0x0800 && addr <= 0x0BFF)
        data = name_table[0][addr & 0x03FF];
      if (addr >= 0x0C00 && addr <= 0x0FFF)
        data = name_table[1][addr & 0x03FF];
    } else if (cart->mirror == Mirror::HORIZONTAL) {
      // Horizontal
      if (addr >= 0x0000 && addr <= 0x03FF)
        data = name_table[0][addr & 0x03FF];
      if (addr >= 0x0400 && addr <= 0x07FF)
        data = name_table[0][addr & 0x03FF];
      if (addr >= 0x0800 && addr <= 0x0BFF)
        data = name_table[1][addr & 0x03FF];
      if (addr >= 0x0C00 && addr <= 0x0FFF)
        data = name_table[1][addr & 0x03FF];
    }
    return data;
  }
  if ((wrapped_addr >= 0x3F00) && (wrapped_addr <= 0x3FFF)) {
    return palette_table[get_palette_address(wrapped_addr)];
  }
  return 0;
}

void PPUBus::write(uint16_t address, uint8_t data) noexcept {
  auto wrapped_addr = static_cast<uint16_t>(address & 0x3FFF);
  if ((wrapped_addr >= 0x0000) && (wrapped_addr <= 0x1FFF)) {
    cart->ppu_write(wrapped_addr, data);
  }
  if ((wrapped_addr >= 0x2000) && (wrapped_addr <= 0x3EFF)) {
    auto addr = wrapped_addr & 0x0FFF;
    if (cart->mirror == Mirror::VERTICAL) {
      // Vertical
      if (addr >= 0x0000 && addr <= 0x03FF)
        name_table[0][addr & 0x03FF] = data;
      if (addr >= 0x0400 && addr <= 0x07FF)
        name_table[1][addr & 0x03FF] = data;
      if (addr >= 0x0800 && addr <= 0x0BFF)
        name_table[0][addr & 0x03FF] = data;
      if (addr >= 0x0C00 && addr <= 0x0FFF)
        name_table[1][addr & 0x03FF] = data;
    } else if (cart->mirror == Mirror::HORIZONTAL) {
      // Horizontal
      if (addr >= 0x0000 && addr <= 0x03FF)
        name_table[0][addr & 0x03FF] = data;
      if (addr >= 0x0400 && addr <= 0x07FF)
        name_table[0][addr & 0x03FF] = data;
      if (addr >= 0x0800 && addr <= 0x0BFF)
        name_table[1][addr & 0x03FF] = data;
      if (addr >= 0x0C00 && addr <= 0x0FFF)
        name_table[1][addr & 0x03FF] = data;
    }
  }
  if ((wrapped_addr >= 0x3F00) && (wrapped_addr <= 0x3FFF)) {
    palette_table[get_palette_address(wrapped_addr)] = data;
  }
}
} // namespace nes_emu
