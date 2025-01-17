#pragma once
#include "../cartridge/cartridge.hpp"

namespace nes_emu {

struct Bus {
  bool nmi{};
  bool irq{};
  [[nodiscard]] uint8_t read(uint16_t addr) noexcept;
  void write(uint16_t addr, uint8_t data) noexcept;

  void load_cartridge(std::string_view path) noexcept;

  void clock() noexcept;

private:
  std::array<uint8_t, 2048> memory{};
  std::shared_ptr<Cartridge> cart;
};

} // namespace nes_emu
