#pragma once
#include "cpu/cpu.hpp"
namespace nes_emu {
struct nes_cpu;
struct nes_bus;

struct instructions {
  const char *name;
  uint8_t (nes_cpu::*function)();
  uint8_t (nes_cpu::*address_mode)();
  uint8_t cycles;
};

// enum cpu_flags {
//   Carry = (1 << 0),
//   Zero = (1 << 1),
//   IntruptDisable = (1 << 2),
//   Decimal = (1 << 3),
//   Break = (1 << 4),
//   Unused = (1 << 5),
//   Overflow = (1 << 6),
//   Negative = (1 << 7),
// };

struct nes_cpu {
  uint16_t pc{};
  uint8_t a{};
  uint8_t x{};
  uint8_t y{};
  uint8_t stack_p{};
  uint8_t status{};

  void clock() noexcept;
  void reset() noexcept;
  void irq() noexcept;
  void nmi() noexcept;
  void connect_to_bus(nes_bus *bus) noexcept;
  [[nodiscard]] bool complete() const noexcept;

  [[nodiscard]] std::map<uint16_t, std::string>
  disassemble(uint16_t addr_start, uint16_t addr_stop) const noexcept;

private:
  uint8_t cpu_cycles{};
  uint8_t fetched{};
  uint16_t m_addr_abs{};
  uint16_t m_addr_rel{};
  nes_bus *m_bus = nullptr;
  instructions m_opcode{};

  uint8_t m_fetch_data() noexcept;
  uint8_t read(uint16_t addr) noexcept;
  void write(uint16_t addr, uint8_t data) noexcept;

  void m_set_flags(CpuFlags flags, bool cond) noexcept;
  [[nodiscard]] uint8_t m_get_flags(CpuFlags flags) const noexcept;

  // Instructions
  [[nodiscard]] uint8_t nop() noexcept;
  [[nodiscard]] uint8_t adc() noexcept;
  [[nodiscard]] uint8_t and_() noexcept;
  [[nodiscard]] uint8_t asl() noexcept;
  [[nodiscard]] uint8_t bcc() noexcept;
  [[nodiscard]] uint8_t bcs() noexcept;
  [[nodiscard]] uint8_t beq() noexcept;
  [[nodiscard]] uint8_t bit() noexcept;
  [[nodiscard]] uint8_t bmi() noexcept;
  [[nodiscard]] uint8_t bne() noexcept;
  [[nodiscard]] uint8_t bpl() noexcept;
  [[nodiscard]] uint8_t brk() noexcept;
  [[nodiscard]] uint8_t bvc() noexcept;
  [[nodiscard]] uint8_t bvs() noexcept;
  [[nodiscard]] uint8_t clc() noexcept;
  [[nodiscard]] uint8_t cld() noexcept;
  [[nodiscard]] uint8_t cli() noexcept;
  [[nodiscard]] uint8_t clv() noexcept;
  [[nodiscard]] uint8_t cmp() noexcept;
  [[nodiscard]] uint8_t cpx() noexcept;
  [[nodiscard]] uint8_t cpy() noexcept;
  [[nodiscard]] uint8_t dec() noexcept;
  [[nodiscard]] uint8_t dex() noexcept;
  [[nodiscard]] uint8_t dey() noexcept;
  [[nodiscard]] uint8_t eor() noexcept;
  [[nodiscard]] uint8_t inc() noexcept;
  [[nodiscard]] uint8_t inx() noexcept;
  [[nodiscard]] uint8_t iny() noexcept;
  [[nodiscard]] uint8_t jmp() noexcept;
  [[nodiscard]] uint8_t jsr() noexcept;
  [[nodiscard]] uint8_t lda() noexcept;
  [[nodiscard]] uint8_t ldx() noexcept;
  [[nodiscard]] uint8_t ldy() noexcept;
  [[nodiscard]] uint8_t lsr() noexcept;
  [[nodiscard]] uint8_t ora() noexcept;
  [[nodiscard]] uint8_t pha() noexcept;
  [[nodiscard]] uint8_t php() noexcept;
  [[nodiscard]] uint8_t pla() noexcept;
  [[nodiscard]] uint8_t plp() noexcept;
  [[nodiscard]] uint8_t rol() noexcept;
  [[nodiscard]] uint8_t ror() noexcept;
  [[nodiscard]] uint8_t rti() noexcept;
  [[nodiscard]] uint8_t rts() noexcept;
  [[nodiscard]] uint8_t sbc() noexcept;
  [[nodiscard]] uint8_t sec() noexcept;
  [[nodiscard]] uint8_t sed() noexcept;
  [[nodiscard]] uint8_t sei() noexcept;
  [[nodiscard]] uint8_t sta() noexcept;
  [[nodiscard]] uint8_t stx() noexcept;
  [[nodiscard]] uint8_t sty() noexcept;
  [[nodiscard]] uint8_t tax() noexcept;
  [[nodiscard]] uint8_t tay() noexcept;
  [[nodiscard]] uint8_t tsx() noexcept;
  [[nodiscard]] uint8_t txa() noexcept;
  [[nodiscard]] uint8_t txs() noexcept;
  [[nodiscard]] uint8_t tya() noexcept;
  // Addressing Mode
  [[nodiscard]] uint8_t imm() noexcept;
  [[nodiscard]] uint8_t imp() noexcept;
  [[nodiscard]] uint8_t zp0() noexcept;
  [[nodiscard]] uint8_t zpx() noexcept;
  [[nodiscard]] uint8_t zpy() noexcept;
  [[nodiscard]] uint8_t abs() noexcept;
  [[nodiscard]] uint8_t azx() noexcept;
  [[nodiscard]] uint8_t azy() noexcept;
  [[nodiscard]] uint8_t ind() noexcept;
  [[nodiscard]] uint8_t izx() noexcept;
  [[nodiscard]] uint8_t izy() noexcept;
  [[nodiscard]] uint8_t rel() noexcept;

  [[nodiscard]] static instructions m_get_instruction(uint8_t opcode) noexcept;
};
} // namespace nes_emu
