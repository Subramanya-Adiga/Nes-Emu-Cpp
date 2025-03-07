#include "mapper.hpp"

namespace nes_emu {
Mapper::Mapper(uint8_t prgBanks, uint8_t chrBanks)
    : m_program_banks(prgBanks), m_character_banks(chrBanks) {}
} // namespace nes_emu