#include "mapper0.hpp"

namespace nes_emu {
mapper0::mapper0(uint8_t prg_bnks, uint8_t chr_bnks)
    : mapper(prg_bnks, chr_bnks) {}

bool mapper0::cpu_map_read(uint16_t addr, uint32_t &mapped_addr) {
  if ((addr >= 0x8000) && (addr <= 0xFFFF)) {
    mapped_addr = addr & (m_program_banks > 1 ? 0x7FFF : 0x3FFF);
    return true;
  }
  return false;
}

bool mapper0::cpu_map_write(uint16_t addr, uint32_t &mapped_addr,
                            uint8_t data) {
  if ((addr >= 0x8000) && (addr <= 0xFFFF)) {
    mapped_addr = addr & (m_program_banks > 1 ? 0x7FFF : 0x3FFF);
    return true;
  }
  return false;
}
bool mapper0::ppu_map_read(uint16_t addr, uint32_t &mapped_addr) {
  if (addr >= 0x0000 && addr <= 0x1FFF) {
    mapped_addr = addr;
    return true;
  }
  return false;
}
bool mapper0::ppu_map_write(uint16_t addr, uint32_t &mapped_addr) {
  if (addr >= 0x0000 && addr <= 0x1FFF) {
    if (m_character_banks == 0) {
      mapped_addr = addr;
      return true;
    }
  }
  return false;
}
void mapper0::reset() {}
} // namespace nes_emu
