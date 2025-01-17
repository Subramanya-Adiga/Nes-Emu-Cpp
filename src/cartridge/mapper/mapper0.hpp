#pragma once
#include "../mapper.hpp"

namespace nes_emu {
class Mapper0 : public Mapper {
public:
  Mapper0(uint8_t prg_bnks, uint8_t chr_bnks);
  virtual ~Mapper0() override = default;

  MapperReturn cpu_map_read(uint16_t addr) override;
  MapperReturn cpu_map_write(uint16_t addr, uint8_t data) override;

  bool ppu_map_read(uint16_t addr, uint32_t &mapped_addr) override;
  bool ppu_map_write(uint16_t addr, uint32_t &mapped_addr) override;

  void reset() override;
};
} // namespace nes_emu
