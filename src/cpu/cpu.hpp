#pragma once
#include "bus.hpp"
#include "instruction.hpp"

namespace nes_emu {

enum CpuFlags : uint8_t {
  Carry = (1 << 0),
  Zero = (1 << 1),
  IntruptDisable = (1 << 2),
  Decimal = (1 << 3),
  Break = (1 << 4),
  Unused = (1 << 5),
  Overflow = (1 << 6),
  Negative = (1 << 7),
};

struct Cpu {
  uint8_t a{};
  uint8_t x{};
  uint8_t y{};
  uint8_t stack_p{};
  uint8_t status{};
  uint16_t pc{};

  Bus *bus = nullptr;

  void clock() noexcept;
  void reset() noexcept;
  [[nodiscard]] bool complete() const noexcept;

private:
  Instruction instruction{};
  uint8_t step_cycle{};
  uint8_t wait_cycle{};
  uint16_t op_address{};
  uint16_t prev_pc{};
  uint16_t num_bytes{};
  uint32_t total_cycle{};
  uint32_t cycles{};
  std::array<uint8_t, 3> op_bytes{};

  void nmi() noexcept;
  void irq() noexcept;

  uint8_t load_address(AddressingMode mode) noexcept;
  uint8_t execute(Mnemonic mnemonic) noexcept;

  [[nodiscard]] uint8_t read(uint16_t addr) const noexcept;
  void write(uint16_t addr, uint8_t data) const noexcept;

  void set_flag(CpuFlags flag, bool cond) noexcept;
  [[nodiscard]] uint8_t get_flag(CpuFlags flag) const noexcept;

  void branch(uint16_t addr) noexcept;
  void stack_push(uint8_t data) noexcept;
  uint8_t stack_pop() noexcept;
  void set_nz(uint8_t data) noexcept;
};
} // namespace nes_emu
