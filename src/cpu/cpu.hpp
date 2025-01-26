#pragma once
#include "instruction.hpp"
#include <fmt/ranges.h>
#include <fmt/std.h>

namespace nes_emu {

struct Bus;

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

  [[nodiscard]] std::map<uint16_t, std::string>
  disassemble(uint16_t addr_start, uint16_t addr_stop) const noexcept;

private:
  friend fmt::formatter<nes_emu::Cpu>;
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

template <>
struct fmt::formatter<nes_emu::Cpu> : fmt::formatter<std::string_view> {
  fmt::format_context::iterator format(nes_emu::Cpu cpu,
                                       fmt::format_context &ctx) const {
    std::string ret = "UNKNOWN";
    ret = fmt::format("{:4X} ", cpu.prev_pc);
    ret += fmt::format("{:2X} ", fmt::join(cpu.op_bytes, " "));
    ret += fmt::format(" {} ", cpu.instruction.name);

    switch (cpu.instruction.addressing_mode) {
    case nes_emu::AddressingMode::Accumilate: {
      ret += fmt::format("A{:>28}", " ");
      break;
    }
    case nes_emu::AddressingMode::Implied: {
      ret += fmt::format("{:>29}", " ");
      break;
    }
    case nes_emu::AddressingMode::Immediate: {
      ret += fmt::format("#${:X>25}", cpu.read(cpu.op_address));
      break;
    }
    case nes_emu::AddressingMode::ZeroPage: {
      ret += fmt::format("${:X} = {:X>21}", cpu.op_address,
                         static_cast<uint16_t>(cpu.op_bytes[1]));
      break;
    }
    case nes_emu::AddressingMode::ZeroPageX: {
      ret += fmt::format("${:X},X @ {:X} = {:X>14}", cpu.op_address + cpu.x,
                         cpu.op_address, cpu.read(cpu.op_address + cpu.x));
      break;
    }
    case nes_emu::AddressingMode::ZeroPageY: {
      ret += fmt::format("${:X},Y @ {:X} = {:X>14}", cpu.op_address + cpu.y,
                         cpu.op_address, cpu.read(cpu.op_address + cpu.y));
      break;
    }
    case nes_emu::AddressingMode::Absolute: {
      if ((cpu.instruction.mnemonic == nes_emu::Mnemonic::Jsr) ||
          (cpu.instruction.mnemonic == nes_emu::Mnemonic::Jmp)) {
        ret += fmt::format("${:4X}", cpu.op_address);
      } else {
        ret += fmt::format("${:4X} = {:2X}", cpu.op_address,
                           cpu.read(cpu.op_address));
      }
      break;
    }
    case nes_emu::AddressingMode::AbsoluteX: {
      ret += fmt::format("${:4X},X @ {:4X} = {:2X}", cpu.op_address - cpu.x,
                         cpu.op_address, cpu.read(cpu.op_address - cpu.x));
      break;
    }
    case nes_emu::AddressingMode::AbsoluteY: {
      ret += fmt::format("${:4X},Y @ {:4X} = {:2X}", cpu.op_address - cpu.y,
                         cpu.op_address, cpu.read(cpu.op_address - cpu.y));
      break;
    }
    case nes_emu::AddressingMode::Indirect: {
      ret +=
          fmt::format("(${:4X}) = {:4X}",
                      cpu.op_bytes[2] << 8 | cpu.op_bytes[1], cpu.op_address);
      break;
    }
    case nes_emu::AddressingMode::IndirectX: {
      ret += fmt::format("(${:2X}),X = {:4X} @ {:4X} = {:2X}  ",
                         cpu.op_bytes[1], cpu.op_address + cpu.x,
                         cpu.op_address, cpu.read(cpu.op_address + cpu.x));
      break;
    }
    case nes_emu::AddressingMode::IndirectY: {
      ret += fmt::format("(${:2X}),Y = {:4X} @ {:2X} = {:2X}  ",
                         cpu.op_bytes[1], cpu.op_address - cpu.y,
                         cpu.op_address, cpu.read(cpu.op_address - cpu.y));
      break;
    }
    case nes_emu::AddressingMode::Relative: {
      auto val = cpu.read(cpu.prev_pc + 1);
      auto addr = cpu.prev_pc + 2;
      ret += fmt::format("${:X>24}", addr + val);
      break;
    } break;
    }

    ret += fmt::format("A:{:X} X:{:X} Y:{:X} P:{:X} SP:{:X} "
                       "PPU:{:3},{:3} CYC:{:5}",
                       cpu.a, cpu.x, cpu.y, cpu.status, cpu.stack_p, 0, 0,
                       cpu.total_cycle);

    return fmt::formatter<string_view>::format(ret, ctx);
  }
};
