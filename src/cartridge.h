#pragma once
#include "mapper.h"

namespace nes_emu {

enum MIRROR {
  HORIZONTAL,
  VERTICAL,
  ONESCREEN_LO,
  ONESCREEN_HI,
};

class nes_cartridge {
  static constexpr uint32_t NES_NUMERIC_HEADER = 441664846;

public:
  nes_cartridge(const std::string &file_name);

  [[nodiscard]] bool image_valid() const noexcept;

  [[nodiscard]] bool cpu_read(uint16_t addr, uint8_t &data);
  [[nodiscard]] bool cpu_write(uint16_t addr, uint8_t data);

  [[nodiscard]] bool ppu_read(uint16_t addr, uint8_t &data);
  [[nodiscard]] bool ppu_write(uint16_t addr, uint8_t data);

  void reset() noexcept;
  MIRROR mirror = HORIZONTAL;

private:
  bool m_valid_image = false;
  uint8_t m_mapper_id{};
  uint8_t m_program_banks{};
  uint8_t m_character_banks{};
  std::vector<uint8_t> m_program_mem;
  std::vector<uint8_t> m_character_mem;
  std::unique_ptr<mapper> m_mapper;
};
} // namespace nes_emu