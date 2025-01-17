#include "nes_bus.hpp"

namespace nes_emu {
nes_bus::nes_bus() { cpu.connect_to_bus(this); }

void nes_bus::write(uint16_t addr, uint8_t data) noexcept {
  if ((addr >= 0x0000) && (addr <= 0x1FFF)) {
    // System Ram
    memory[addr & 0x07FF] = data;
  } else if ((addr >= 0x2000) && (addr <= 0x3FFF)) {
    // Pixel Processing Unit Write
    ppu.cpu_write(addr & 0x0007, data);
  } else if (addr >= 0x4016 && addr <= 0x4017) {
    //  // Controllers
    m_controller_state[addr & 0x0001] = controllers[addr & 0x0001];
  } else if (addr >= 0x4020 && addr <= 0xFFFF) {
    cartridge->cpu_write(addr, data);
  }
}

uint8_t nes_bus::read(uint16_t addr) noexcept {
  uint8_t data = 0;
  if ((addr >= 0x0000) && (addr <= 0x1FFF)) {
    // System Ram
    data = memory[addr & 0x07FF];
  } else if ((addr >= 0x2000) && (addr <= 0x3FFF)) {
    // Pixel Processing Unit
    data = ppu.cpu_read(addr & 0x0007);
  } else if (addr >= 0x4016 && addr <= 0x4017) {
    // Controllers
    data = (m_controller_state[addr & 0x0001] & 0x80) > 0;
    m_controller_state[addr & 0x0001] <<= 1;
  }
  if (addr >= 0x4020 && addr <= 0xFFFF) {
    data = cartridge->cpu_read(addr);
  }
  return data;
}

void nes_bus::insert_cartridge(const std::string &nes_cart) {
  cartridge = std::make_unique<Cartridge>(nes_cart);
  ppu.connect_to_cartridge(&*cartridge);
}

void nes_bus::reset() {
  cartridge->reset();
  cpu.reset();
  ppu.reset();
  m_system_counter = 0;
}

void nes_bus::clock() {
  ppu.clock();
  if ((m_system_counter % 3) == 0) {
    cpu.clock();
  }
  if (ppu.nmi) {
    ppu.nmi = false;
    cpu.nmi();
  }
  m_system_counter++;
}
} // namespace nes_emu
