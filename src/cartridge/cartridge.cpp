#include "cartridge.hpp"
#include "../utils.hpp"
#include "mapper/mapper0.hpp"

namespace nes_emu {

Cartridge::Cartridge(const std::string &file_name) {
  file_handle nes_file(fopen(file_name.data(), "rb"),
                       [](FILE *file) { (void)fclose(file); });

  if (nes_file) {
    if (file_handle_helper::read_u32(nes_file) == NES_NUMERIC_HEADER) {

      m_program_banks = file_handle_helper::read_u8(nes_file);
      m_character_banks = file_handle_helper::read_u8(nes_file);

      auto mapper1 = file_handle_helper::read_u8(nes_file);
      auto mapper2 = file_handle_helper::read_u8(nes_file);

      [[maybe_unused]] auto program_rom_size =
          file_handle_helper::read_u8(nes_file);
      [[maybe_unused]] auto tv_system1 = file_handle_helper::read_u8(nes_file);
      [[maybe_unused]] auto tv_system2 = file_handle_helper::read_u8(nes_file);

      (void)fseek(nes_file.get(), 5, SEEK_CUR);

      if ((mapper1 & 0x04) != 0) {
        (void)fseek(nes_file.get(), 512, SEEK_CUR);
      }

      m_mapper_id = ((mapper2 >> 4) << 4) | (mapper1 >> 4);
      mirror = ((mapper1 & 0x01) != 0) ? VERTICAL : HORIZONTAL;

      m_program_mem.resize(static_cast<uint64_t>(m_program_banks) * 16384);
      m_program_mem = file_handle_helper::read_vec_u8(
          nes_file, static_cast<uint32_t>(m_program_mem.size()));
      m_character_mem.resize(static_cast<uint64_t>(m_character_banks) * 8192);
      m_character_mem = file_handle_helper::read_vec_u8(
          nes_file, static_cast<uint32_t>(m_character_mem.size()));

      switch (m_mapper_id) {
      case 0:
        m_mapper =
            std::make_unique<Mapper0>(m_program_banks, m_character_banks);
        break;
      }
      m_valid_image = true;
    }
  }
}

uint8_t Cartridge::cpu_read(uint16_t addr) {
  if (auto mapped_data = m_mapper->cpu_map_read(addr);
      mapped_data.mapped_addr == 0xFFFFFFFF) {
    return mapped_data.data;
  } else {
    return m_program_mem[mapped_data.mapped_addr];
  }
  return {};
}
void Cartridge::cpu_write(uint16_t addr, uint8_t data) {
  if (auto mapped_data = m_mapper->cpu_map_write(addr, data);
      mapped_data.mapped_addr != 0xFFFFFFFF) {
    m_program_mem[mapped_data.mapped_addr] = data;
  }
}

bool Cartridge::ppu_read(uint16_t addr, uint8_t &data) {
  uint32_t mapped_addr = 0;
  if (m_mapper->ppu_map_read(addr, mapped_addr)) {
    data = m_character_mem[mapped_addr];
    return true;
  } else {
    return false;
  }
}
bool Cartridge::ppu_write(uint16_t addr, uint8_t data) {
  uint32_t mapped_addr = 0;
  if (m_mapper->ppu_map_read(addr, mapped_addr)) {
    m_character_mem[mapped_addr] = data;
    return true;
  } else {
    return false;
  }
}

void Cartridge::reset() noexcept {
  if (m_mapper != nullptr) {
    m_mapper->reset();
  }
}

bool Cartridge::image_valid() const noexcept { return m_valid_image; }
} // namespace nes_emu

// namespace nes_emu
