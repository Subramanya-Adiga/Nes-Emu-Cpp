#include "nes_bus.h"

nes_emu::nes_bus::nes_bus() { cpu.connect_to_bus(this); }

void nes_emu::nes_bus::write(uint16_t addr, uint8_t data) noexcept {
  if ((addr >= 0x0000) && (addr <= 0xFFFF)) {
    memory[addr] = data;
  }
}

uint8_t nes_emu::nes_bus::read(uint16_t addr) noexcept {
  if ((addr >= 0x0000) && (addr <= 0xFFFF)) {
    return memory[addr];
  }
  return 0;
}
