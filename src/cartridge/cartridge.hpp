#pragma once
#include <cartridge/mapper.hpp>

namespace nes_emu {

enum Mirror : uint8_t {
  HORIZONTAL,
  VERTICAL,
  ONESCREEN_LO,
  ONESCREEN_HI,
};

class Cartridge {
  static constexpr uint32_t NES_NUMERIC_HEADER = 441664846;

public:
  Cartridge(std::string_view file_name);

  [[nodiscard]] bool image_valid() const noexcept;

  [[nodiscard]] uint8_t cpu_read(uint16_t addr);
  void cpu_write(uint16_t addr, uint8_t data);

  [[nodiscard]] uint8_t ppu_read(uint16_t addr);
  void ppu_write(uint16_t addr, uint8_t data);

  void reset() noexcept;
  Mirror mirror = HORIZONTAL;

private:
  bool m_valid_image = false;
  uint8_t m_mapper_id{};
  uint8_t m_program_banks{};
  uint8_t m_character_banks{};
  std::vector<uint8_t> m_program_mem;
  std::vector<uint8_t> m_character_mem;
  std::unique_ptr<Mapper> m_mapper;
};
} // namespace nes_emu