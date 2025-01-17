#include "bus.hpp"

namespace nes_emu {
uint8_t Bus::read(uint16_t addr) noexcept {
  if (addr >= 0x0000 && addr <= 0x1FFF) {
    return memory[addr & 0x7FF];
  }
  if ((addr == 0x4016) || (addr == 0x4017)) {
    fmt::print("Controller Not Implemented");
  }
  if (addr >= 0x2000 && addr <= 0x3FFF) {
    fmt::print("PPU Not Implemented");
  }
  if (addr >= 0x4020 && addr <= 0xFFFF) {
    fmt::print("Cartridge Not Implemented");
  }
  return 0;
}

void Bus::write(uint16_t addr, uint8_t data) noexcept {
  if (addr >= 0x0000 && addr <= 0x1FFF) {
    memory[addr & 0x7FF] = data;
  }
}

void Bus::load_cartridge(std::string_view path) noexcept {}

void Bus::clock() noexcept {}
} // namespace nes_emu
