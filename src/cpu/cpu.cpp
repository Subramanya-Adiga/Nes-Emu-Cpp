#include "cpu.hpp"
#include "bus.hpp"

namespace nes_emu {

void Cpu::clock() noexcept {
  if (wait_cycle > 0) {
    wait_cycle -= 1;
    return;
  }

  if (bus->irq && (get_flag(CpuFlags::IntruptDisable) == 0)) {
    bus->irq = false;
    irq();
  }

  if (bus->nmi) {
    bus->nmi = false;
    nmi();
  }

  step_cycle = 0;
  total_cycle = cycles;

  op_bytes[0] = read(pc);
  instruction = process_opcode(op_bytes[0]);
  set_flag(CpuFlags::Unused, true);

  prev_pc = pc;
  pc++;
  step_cycle = instruction.cycles;

  auto clk1 = load_address(instruction.addressing_mode);

  num_bytes |= pc - prev_pc;

  auto clk2 = execute(instruction.mnemonic);

  step_cycle += (clk1 & clk2);
  cycles += step_cycle;
  set_flag(CpuFlags::Unused, true);
  wait_cycle |= step_cycle - 1;
}

void Cpu::reset() noexcept {
  op_address = 0xFFFC;

  auto lo = read(op_address);
  auto hi = read(op_address + 1);
  pc = static_cast<uint16_t>((hi << 8) | lo);

  a = 0;
  x = 0;
  y = 0;

  stack_p = 0xFD;
  status = 0 | static_cast<uint8_t>(CpuFlags::Unused) |
           static_cast<uint8_t>(CpuFlags::IntruptDisable);

  op_address = 0;
  wait_cycle = 0;
  step_cycle = 0;
  cycles = 7;
}

bool Cpu::complete() const noexcept { return wait_cycle == 0; }

void Cpu::nmi() noexcept {
  stack_push(static_cast<uint8_t>(pc >> 8) & 0x00FF);
  stack_push(static_cast<uint8_t>(pc) & 0x00FF);

  set_flag(CpuFlags::Break, false);
  set_flag(CpuFlags::Unused, true);
  set_flag(CpuFlags::IntruptDisable, true);
  stack_push(status);

  op_address = 0xFFFA;

  auto lo = read(op_address);
  auto hi = read(op_address + 1);
  pc = static_cast<uint16_t>((hi << 8) | lo);
  cycles = 8;
}

void Cpu::irq() noexcept {
  if (get_flag(CpuFlags::IntruptDisable) == 0) {
    stack_push(static_cast<uint8_t>(pc >> 8) & 0x00FF);
    stack_push(static_cast<uint8_t>(pc) & 0x00FF);

    set_flag(CpuFlags::Break, false);
    set_flag(CpuFlags::Unused, true);
    set_flag(CpuFlags::IntruptDisable, true);
    stack_push(status);

    op_address = 0xFFFE;

    auto lo = read(op_address);
    auto hi = read(op_address + 1);
    pc = static_cast<uint16_t>((hi << 8) | lo);
    cycles = 7;
  }
}

std::map<uint16_t, std::string>
Cpu::disassemble(uint16_t addr_start, uint16_t addr_stop) const noexcept {
  uint32_t addr = addr_start;
  uint8_t value = 0x00;
  uint8_t lo = 0x00;
  uint8_t hi = 0x00;
  std::map<uint16_t, std::string> mapLines;
  uint16_t line_addr = 0;

  while (addr <= static_cast<uint32_t>(addr_stop)) {
    line_addr = static_cast<uint16_t>(addr);

    auto opcode = process_opcode(bus->read(static_cast<uint16_t>(addr)));
    addr++;
    auto sInst = std::format("${:X}: {} ", addr, opcode.name);

    if (opcode.addressing_mode == AddressingMode::Implied) {
      sInst += std::format(" {}", "{IMP}");
    } else if (opcode.addressing_mode == AddressingMode::Immediate) {
      value = bus->read(static_cast<uint16_t>(addr));
      addr++;
      sInst += std::format("#$ {:02X} {{IMM}}", value);
    } else if (opcode.addressing_mode == AddressingMode::ZeroPage) {
      lo = bus->read(static_cast<uint16_t>(addr));
      addr++;
      hi = 0x00;
      sInst += std::format("#$ {:02X} {{ZP0}}", lo);
    } else if (opcode.addressing_mode == AddressingMode::ZeroPageX) {
      lo = bus->read(static_cast<uint16_t>(addr));
      addr++;
      hi = 0x00;
      sInst += std::format("#$ {:02X} {{ZPX}}", lo);
    } else if (opcode.addressing_mode == AddressingMode::ZeroPageY) {
      lo = bus->read(static_cast<uint16_t>(addr));
      addr++;
      hi = 0x00;
      sInst += std::format("#$ {:02X} ,Y {{ZPY}}", lo);
    } else if (opcode.addressing_mode == AddressingMode::IndirectX) {
      lo = bus->read(static_cast<uint16_t>(addr));
      addr++;
      hi = 0x00;
      sInst += std::format("($ {:02X} ,X) {{IZX}}", lo);
    } else if (opcode.addressing_mode == AddressingMode::IndirectY) {
      lo = bus->read(static_cast<uint16_t>(addr));
      addr++;
      hi = 0x00;
      sInst += std::format("($ {:02X} ,Y) {{IZY}}", lo);
    } else if (opcode.addressing_mode == AddressingMode::Absolute) {
      lo = bus->read(static_cast<uint16_t>(addr));
      addr++;
      hi = bus->read(static_cast<uint16_t>(addr));
      addr++;
      sInst += std::format("${:02X} {{ABS}}", (hi << 8) | lo);
    } else if (opcode.addressing_mode == AddressingMode::AbsoluteX) {
      lo = bus->read(static_cast<uint16_t>(addr));
      addr++;
      hi = bus->read(static_cast<uint16_t>(addr));
      addr++;
      sInst += std::format("${:02X} ,X) {{ABX}}", (hi << 8) | lo);
    } else if (opcode.addressing_mode == AddressingMode::AbsoluteY) {
      lo = bus->read(static_cast<uint16_t>(addr));
      addr++;
      hi = bus->read(static_cast<uint16_t>(addr));
      addr++;
      sInst += std::format("${:02X} ,Y {{ABY}}", (hi << 8) | lo);
    } else if (opcode.addressing_mode == AddressingMode::Indirect) {
      lo = bus->read(static_cast<uint16_t>(addr));
      addr++;
      hi = bus->read(static_cast<uint16_t>(addr));
      addr++;
      sInst += std::format("${:02X}) {{IND}}", (hi << 8) | lo);
    } else if (opcode.addressing_mode == AddressingMode::Relative) {
      value = bus->read(static_cast<uint16_t>(addr));
      addr++;
      sInst += std::format("$ {:02X} [${:02X}] {{REL}}", value,
                           static_cast<int>(addr + value));
    }

    mapLines[line_addr] = sInst;
  }

  return mapLines;
}

uint8_t Cpu::read(uint16_t addr) const noexcept { return bus->read(addr); }

void Cpu::write(uint16_t addr, uint8_t data) const noexcept {
  bus->write(addr, data);
}

void Cpu::set_flag(CpuFlags flags, bool cond) noexcept {
  if (cond) {
    status |= flags;
  } else {
    status &= ~flags;
  }
}

uint8_t Cpu::get_flag(CpuFlags flags) const noexcept {
  return ((status & flags) > 0) ? 1 : 0;
}

void Cpu::stack_push(uint8_t data) noexcept {
  write(0x100 + stack_p, data);
  stack_p--;
}

uint8_t Cpu::stack_pop() noexcept {
  stack_p++;
  return read(0x100 + stack_p);
}

void Cpu::set_nz(uint8_t data) noexcept {
  set_flag(CpuFlags::Negative, (data >> 7) != 0);
  set_flag(CpuFlags::Zero, data == 0);
}

void Cpu::branch(uint16_t addr) noexcept {
  step_cycle++;
  op_address = pc + addr;
  if ((op_address & 0xFF00) != (pc & 0xFF00)) {
    step_cycle++;
  }
  pc = op_address;
}

uint8_t Cpu::load_address(AddressingMode mode) noexcept {
  switch (mode) {
  case AddressingMode::Accumilate:
  case AddressingMode::Implied:
    return 0;
  case AddressingMode::Immediate: {
    op_bytes[1] = read(pc);
    op_address = pc;
    pc++;
    return 0;
  }
  case AddressingMode::ZeroPage: {
    op_bytes[1] = read(pc);
    op_address = static_cast<uint16_t>(op_bytes[1]);
    pc++;
    op_address &= 0x00FF;
    return 0;
  }
  case AddressingMode::ZeroPageX: {
    op_bytes[1] = read(pc);
    op_address = static_cast<uint16_t>(op_bytes[1] + x);
    pc++;
    op_address &= 0x00FF;
    return 0;
  }
  case AddressingMode::ZeroPageY: {
    op_bytes[1] = read(pc);
    op_address = static_cast<uint16_t>(op_bytes[1] + y);
    pc++;
    op_address &= 0x00FF;
    return 0;
  }
  case AddressingMode::Absolute: {
    op_bytes[1] = read(pc);
    op_bytes[2] = read(pc + 1);
    pc += 2;

    auto lo = static_cast<uint16_t>(op_bytes[1]);
    auto hi = static_cast<uint16_t>(op_bytes[2]);

    op_address = static_cast<uint16_t>((hi << 8) | lo);
    return 0;
  }
  case AddressingMode::AbsoluteX: {
    op_bytes[1] = read(pc);
    op_bytes[2] = read(pc + 1);

    auto lo = static_cast<uint16_t>(op_bytes[1]);
    auto hi = static_cast<uint16_t>(op_bytes[2]);

    pc += 2;
    op_address = static_cast<uint16_t>(((hi << 8) | lo) + x);
    if ((op_address & 0xFF00) != (hi << 8)) {
      return 1;
    }
    return 0;
  }
  case AddressingMode::AbsoluteY: {
    op_bytes[1] = read(pc);
    op_bytes[2] = read(pc + 1);

    auto lo = static_cast<uint16_t>(op_bytes[1]);
    auto hi = static_cast<uint16_t>(op_bytes[2]);

    pc += 2;
    op_address = static_cast<uint16_t>(((hi << 8) | lo) + y);
    if ((op_address & 0xFF00) != (hi << 8)) {
      return 1;
    }
    return 0;
  }
  case AddressingMode::Indirect: {
    op_bytes[1] = read(pc);
    op_bytes[2] = read(pc + 1);

    auto lo = static_cast<uint16_t>(op_bytes[1]);
    auto hi = static_cast<uint16_t>(op_bytes[2]);
    pc += 2;

    auto itrm_data = static_cast<uint16_t>((hi << 8) | lo);

    if (lo == 0x00FF) {
      op_address = static_cast<uint16_t>((read(itrm_data & 0xFF00)) << 8 |
                                         read(itrm_data));
    } else {
      op_address =
          static_cast<uint16_t>((read(itrm_data + 1)) << 8 | read(itrm_data));
    }

    return 0;
  }
  case AddressingMode::IndirectX: {
    op_bytes[1] = read(pc);
    auto data = static_cast<uint16_t>(op_bytes[1]);
    pc++;

    auto lo =
        read(static_cast<uint16_t>(data + static_cast<uint16_t>(x)) & 0x00FF);
    auto hi = read(static_cast<uint16_t>(data + static_cast<uint16_t>(x) + 1) &
                   0x00FF);

    op_address = static_cast<uint16_t>((hi << 8) | lo);
    return 0;
  }
  case AddressingMode::IndirectY: {
    op_bytes[1] = read(pc);
    pc++;

    auto data = static_cast<uint16_t>(op_bytes[1]);

    auto lo = static_cast<uint16_t>(read(data & 0x00FF));
    auto hi = static_cast<uint16_t>(read((data + 1) & 0x00FF));

    op_address = static_cast<uint16_t>(((hi << 8) | lo) + y);
    if ((op_address & 0xFF00) != (hi << 8)) {
      return 1;
    }
    return 0;
  }
  case AddressingMode::Relative: {
    op_bytes[1] = read(pc);
    op_address = op_bytes[1];
    pc++;
    if ((op_address & 0x80) != 0) {
      op_address |= 0xFF00;
    }
    return 0;
  }
  }
  return 0;
}

uint8_t Cpu::execute(Mnemonic mnemonic) noexcept {
  switch (mnemonic) {
  case Mnemonic::Nop: {
    switch (op_bytes[0]) {
    case 0x1C:
    case 0x3C:
    case 0x5C:
    case 0x7C:
    case 0xDC:
    case 0xFC:
      return 1;
    default:
      return 0;
    }
  }
  case Mnemonic::Adc: {
    auto add_op = read(op_address);
    auto tmp = static_cast<uint16_t>(a) + static_cast<uint16_t>(add_op) +
               static_cast<uint16_t>(get_flag(CpuFlags::Carry));
    set_flag(CpuFlags::Carry, tmp > 255);
    set_flag(CpuFlags::Zero, (tmp & 0x00FF) == 0);
    auto cond = (~(a ^ add_op) & (a ^ tmp)) & 0x0080;
    set_flag(CpuFlags::Overflow, cond != 0);
    set_flag(CpuFlags::Negative, (tmp & 0x80) != 0);
    a = static_cast<uint8_t>(tmp & 0x00FF);
    return 1;
  }
  case Mnemonic::And: {
    a = a & read(op_address);
    set_nz(a);
    return 1;
  }
  case Mnemonic::Asl: {
    if (instruction.addressing_mode == AddressingMode::Accumilate) {
      set_flag(CpuFlags::Carry, (a >> 7) != 0);
      a <<= 1;
      set_nz(a);
    } else {
      auto op = read(op_address);
      auto res = static_cast<uint8_t>(op << 1);
      write(op_address, res);
      set_flag(CpuFlags::Carry, (op >> 7) != 0);
      set_nz(res);
    }
    return 0;
  }
  case Mnemonic::Bcc: {
    if (get_flag(CpuFlags::Carry) == 0) {
      branch(op_address);
    }
    return 0;
  }
  case Mnemonic::Bcs: {
    if (get_flag(CpuFlags::Carry) == 1) {
      branch(op_address);
    }
    return 0;
  }
  case Mnemonic::Beq: {
    if (get_flag(CpuFlags::Zero) == 1) {
      branch(op_address);
    }
    return 0;
  }
  case Mnemonic::Bit: {
    auto op = read(op_address);
    set_flag(CpuFlags::Zero, (a & op) == 0);
    set_flag(CpuFlags::Negative, (op & (1 << 7)) != 0);
    set_flag(CpuFlags::Overflow, (op & (1 << 6)) != 0);
    return 0;
  }
  case Mnemonic::Bmi: {
    if (get_flag(CpuFlags::Negative) == 1) {
      branch(op_address);
    }
    return 0;
  }
  case Mnemonic::Bne: {
    if (get_flag(CpuFlags::Zero) == 0) {
      branch(op_address);
    }
    return 0;
  }
  case Mnemonic::Bpl: {
    if (get_flag(CpuFlags::Negative) == 0) {
      branch(op_address);
    }
    return 0;
  }
  case Mnemonic::Brk: {
    pc++;

    set_flag(CpuFlags::IntruptDisable, true);
    stack_push(static_cast<uint8_t>((pc >> 8) & 0x00FF));
    stack_push(static_cast<uint8_t>(pc & 0x00FF));

    set_flag(CpuFlags::Break, true);
    stack_push(status);
    set_flag(CpuFlags::Break, false);

    pc = static_cast<uint16_t>((read(0xFFFE) | read(0xFFFF)) << 8);
    return 0;
  }
  case Mnemonic::Bvc: {
    if (get_flag(CpuFlags::Overflow) == 0) {
      branch(op_address);
    }
    return 0;
  }
  case Mnemonic::Bvs: {
    if (get_flag(CpuFlags::Overflow) == 1) {
      branch(op_address);
    }
    return 0;
  }
  case Mnemonic::Clc: {
    set_flag(CpuFlags::Carry, false);
    return 0;
  }
  case Mnemonic::Cld: {
    set_flag(CpuFlags::Decimal, false);
    return 0;
  }
  case Mnemonic::Cli: {
    set_flag(CpuFlags::IntruptDisable, false);
    return 0;
  }
  case Mnemonic::Clv: {
    set_flag(CpuFlags::Overflow, false);
    return 0;
  }
  case Mnemonic::Cmp: {
    auto op = read(op_address);
    auto res = a - op;
    set_flag(CpuFlags::Carry, a >= op);
    set_nz(static_cast<uint8_t>(res));
    return 1;
  }
  case Mnemonic::Cpx: {
    auto op = read(op_address);
    auto res = static_cast<uint8_t>(x - op);
    set_flag(CpuFlags::Carry, x >= op);
    set_nz(res);
    return 0;
  }
  case Mnemonic::Cpy: {
    auto op = read(op_address);
    auto res = static_cast<uint8_t>(y - op);
    set_flag(CpuFlags::Carry, y >= op);
    set_nz(res);
    return 0;
  }
  case Mnemonic::Dec: {
    auto op = static_cast<uint8_t>(read(op_address) - 1);
    write(op_address, op);
    set_nz(op);
    return 0;
  }
  case Mnemonic::Dex: {
    x--;
    set_nz(x);
    return 0;
  }
  case Mnemonic::Dey: {
    y--;
    set_nz(y);
    return 0;
  }
  case Mnemonic::Eor: {
    a = a ^ read(op_address);
    set_nz(a);
    return 1;
  }
  case Mnemonic::Inc: {
    auto tmp = static_cast<uint8_t>(read(op_address) + 1);
    write(op_address, tmp);
    set_nz(tmp);
    return 0;
  }
  case Mnemonic::Inx: {
    x++;
    set_nz(x);
    return 0;
  }
  case Mnemonic::Iny: {
    y++;
    set_nz(y);
    return 0;
  }
  case Mnemonic::Jmp: {
    pc = op_address;
    return 0;
  }
  case Mnemonic::Jsr: {
    pc--;
    stack_push(static_cast<uint8_t>((pc >> 8) & 0x00FF));
    stack_push(static_cast<uint8_t>(pc & 0x00FF));
    pc = op_address;
    return 0;
  }
  case Mnemonic::Lda: {
    a = read(op_address);
    set_nz(a);
    return 1;
  }
  case Mnemonic::Ldx: {
    x = read(op_address);
    set_nz(x);
    return 0;
  }
  case Mnemonic::Ldy: {
    y = read(op_address);
    set_nz(y);
    return 0;
  }
  case Mnemonic::Lsr: {
    if (instruction.addressing_mode == AddressingMode::Accumilate) {
      set_flag(CpuFlags::Carry, (a & 0x0001) != 0);
      a >>= 1;
      set_nz(a);
    } else {
      auto op = read(op_address);
      auto res = static_cast<uint8_t>(op >> 1);
      write(op_address, res);
      set_flag(CpuFlags::Carry, (op & 0x0001) != 0);
      set_nz(res);
    }
    return 0;
  }
  case Mnemonic::Ora: {
    a = a | read(op_address);
    set_nz(a);
    return 1;
  }
  case Mnemonic::Pha: {
    stack_push(a);
    return 0;
  }
  case Mnemonic::Php: {
    stack_push(status | static_cast<uint8_t>(CpuFlags::Break) |
               static_cast<uint8_t>(CpuFlags::Unused));
    set_flag(CpuFlags::Break, false);
    set_flag(CpuFlags::Unused, false);
    return 0;
  }
  case Mnemonic::Pla: {
    a = stack_pop();
    set_nz(a);
    return 0;
  }
  case Mnemonic::Plp: {
    status = stack_pop();
    set_flag(CpuFlags::Unused, true);
    set_flag(CpuFlags::Break, false);
    return 0;
  }
  case Mnemonic::Rol: {
    if (instruction.addressing_mode == AddressingMode::Accumilate) {
      auto res = static_cast<uint8_t>(a << 1 | get_flag(CpuFlags::Carry));
      set_flag(CpuFlags::Carry, (a >> 7) != 0);
      a = res;
      set_nz(a);
    } else {
      auto op = read(op_address);
      auto res = static_cast<uint8_t>(op << 1 | get_flag(CpuFlags::Carry));
      write(op_address, res);
      set_flag(CpuFlags::Carry, (op >> 7) != 0);
      set_nz(res);
    }
    return 0;
  }
  case Mnemonic::Ror: {
    if (instruction.addressing_mode == AddressingMode::Accumilate) {
      auto op = a;
      a = static_cast<uint8_t>((a >> 1) | (get_flag(CpuFlags::Carry) << 7));
      set_flag(CpuFlags::Carry, (op & 0x01) != 0);
      set_nz(a);
    } else {
      auto op = read(op_address);
      auto res =
          static_cast<uint8_t>((op >> 1) | (get_flag(CpuFlags::Carry) << 7));
      write(op_address, res);
      set_flag(CpuFlags::Carry, (op & 0x01) != 0);
      set_nz(res);
    }
    return 0;
  }
  case Mnemonic::Rti: {
    status = stack_pop();
    status &= ~static_cast<uint8_t>(CpuFlags::Break);
    status &= ~static_cast<uint8_t>(CpuFlags::Unused);
    pc = static_cast<uint16_t>(stack_pop());
    pc |= static_cast<uint16_t>(stack_pop() << 8);
    return 0;
  }
  case Mnemonic::Rts: {
    pc = static_cast<uint16_t>(stack_pop());
    pc |= static_cast<uint16_t>(stack_pop() << 8);
    pc++;
    return 0;
  }
  case Mnemonic::Sbc: {
    auto sub_op = read(op_address);
    auto val = static_cast<uint16_t>(sub_op ^ 0x00FF);
    auto tmp = static_cast<uint16_t>(a) + val +
               static_cast<uint16_t>(get_flag(CpuFlags::Carry));
    set_flag(CpuFlags::Carry, (tmp & 0xFF00) != 0);
    set_flag(CpuFlags::Zero, (tmp & 0x00FF) == 0);
    set_flag(CpuFlags::Overflow, (((tmp ^ a) & (tmp ^ val)) & 0x0080) != 0);
    set_flag(CpuFlags::Negative, (tmp & 0x0080) != 0);
    a = static_cast<uint8_t>(tmp & 0x00FF);

    return 1;
  }
  case Mnemonic::Sec: {
    set_flag(CpuFlags::Carry, true);
    return 0;
  }
  case Mnemonic::Sed: {
    set_flag(CpuFlags::Decimal, true);
    return 0;
  }
  case Mnemonic::Sei: {
    set_flag(CpuFlags::IntruptDisable, true);
    return 0;
  }
  case Mnemonic::Sta: {
    write(op_address, a);
    return 0;
  }
  case Mnemonic::Stx: {
    write(op_address, x);
    return 0;
  }
  case Mnemonic::Sty: {
    write(op_address, y);
    return 0;
  }
  case Mnemonic::Tax: {
    x = a;
    set_nz(x);
    return 0;
  }
  case Mnemonic::Tay: {
    y = a;
    set_nz(y);
    return 0;
  }
  case Mnemonic::Tsx: {
    x = stack_p;
    set_nz(x);
    return 0;
  }
  case Mnemonic::Txa: {
    a = x;
    set_nz(a);
    return 0;
  }
  case Mnemonic::Txs: {
    stack_p = x;
    return 0;
  }
  case Mnemonic::Tya: {
    a = y;
    set_nz(a);
    return 0;
  }
  // Illegal
  case Mnemonic::Lax: {
    if (instruction.addressing_mode == AddressingMode::Immediate) {
      (void)execute(Mnemonic::Lda);
      (void)execute(Mnemonic::Tax);
    } else {
      (void)execute(Mnemonic::Lda);
      (void)execute(Mnemonic::Ldx);
    }
    set_nz(a | x);
    if ((instruction.addressing_mode == AddressingMode::AbsoluteY) ||
        (instruction.addressing_mode == AddressingMode::IndirectY)) {
      return 1;
    }
    return 0;
  }
  case Mnemonic::Sax: {
    write(op_address, a & x);
    return 0;
  }
  case Mnemonic::Dcp: {
    (void)execute(Mnemonic::Dec);
    (void)execute(Mnemonic::Cmp);
    return 0;
  }
  case Mnemonic::Isb: {
    (void)execute(Mnemonic::Inc);
    (void)execute(Mnemonic::Sbc);
    return 0;
  }
  case Mnemonic::Slo: {
    (void)execute(Mnemonic::Asl);
    (void)execute(Mnemonic::Ora);
    return 0;
  }
  case Mnemonic::Rla: {
    (void)execute(Mnemonic::Rol);
    (void)execute(Mnemonic::And);
    return 0;
  }
  case Mnemonic::Sre: {
    (void)execute(Mnemonic::Lsr);
    (void)execute(Mnemonic::Eor);
    return 0;
  }
  case Mnemonic::Rra: {
    (void)execute(Mnemonic::Ror);
    (void)execute(Mnemonic::Adc);
    return 0;
  }
  default:
    break;
  }
  return 0;
}

} // namespace nes_emu
