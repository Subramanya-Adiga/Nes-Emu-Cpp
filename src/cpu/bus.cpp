#include "bus.hpp"

namespace nes_emu {
uint8_t Bus::read(uint16_t addr) noexcept {
  if ((addr >= 0x0000) && (addr <= 0x1FFF)) {
    return memory[addr & 0x7FF];
  }
  if ((addr == 0x4016) || (addr == 0x4017)) {
    fmt::print("Controller Not Implemented");
  }
  if ((addr >= 0x2000) && (addr <= 0x3FFF)) {
    return ppu.cpu_read(addr & 0x0007);
  }
  if ((addr >= 0x4020) && (addr <= 0xFFFF)) {
    return cart->cpu_read(addr);
  }
  return 0;
}

void Bus::write(uint16_t addr, uint8_t data) noexcept {
  if ((addr >= 0x0000) && (addr <= 0x1FFF)) {
    memory[addr & 0x7FF] = data;
  }
  if ((addr >= 0x2000) && (addr <= 0x3FFF)) {
    ppu.cpu_write(addr & 0x0007, data);
  }
  if ((addr >= 0x4020) && addr <= 0xFFFF) {
    cart->cpu_write(addr, data);
  }
}

void Bus::load_cartridge(std::string_view path) noexcept {
  cart = std::make_unique<Cartridge>(path);
  ppu.connect_to_cartridge(&*cart);
}

uint32_t Bus::clock_counter() const noexcept { return counter; }

void Bus::clock() noexcept {
  ppu.clock();
  if (ppu.nmi) {
    ppu.nmi = false;
    nmi = true;
  }
  counter++;
}

void Bus::reset() noexcept {
  ppu.reset();
  counter = 0;
}
} // namespace nes_emu
