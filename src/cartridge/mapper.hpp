#pragma once

namespace nes_emu {

struct MapperReturn {
  uint32_t mapped_addr{};
  uint8_t data{};
};

class Mapper {
public:
  Mapper(uint8_t prgBanks, uint8_t chrBanks);
  Mapper(const Mapper &) = default;
  Mapper(Mapper &&) = default;
  Mapper &operator=(const Mapper &) = default;
  Mapper &operator=(Mapper &&) = default;
  virtual ~Mapper() = default;

  virtual MapperReturn cpu_map_read(uint16_t addr) = 0;
  virtual MapperReturn cpu_map_write(uint16_t addr, uint8_t data) = 0;

  virtual uint32_t ppu_map_read(uint16_t addr) = 0;
  virtual uint32_t ppu_map_write(uint16_t addr) = 0;

  virtual void reset() = 0;

protected:
  uint8_t m_program_banks = 0;
  uint8_t m_character_banks = 0;
};
} // namespace nes_emu