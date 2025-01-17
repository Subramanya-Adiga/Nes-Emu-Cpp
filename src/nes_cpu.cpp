#include "nes_cpu.hpp"
#include "nes_bus.hpp"

namespace nes_emu {

void nes_cpu::clock() noexcept {
  if (cpu_cycles == 0) {

    m_opcode = m_get_instruction(read(pc));
    m_set_flags(CpuFlags::Unused, true);

    pc++;
    cpu_cycles = m_opcode.cycles;

    auto clk1 = (this->*m_opcode.address_mode)();
    auto clk2 = (this->*m_opcode.function)();

    cpu_cycles += (clk1 & clk2);
    m_set_flags(CpuFlags::Unused, true);
  }

  cpu_cycles--;
}

void nes_cpu::reset() noexcept {
  m_addr_abs = 0xFFFC;

  uint16_t lo = read(m_addr_abs + 0);
  uint16_t hi = read(m_addr_abs + 1);

  pc = (hi << 8) | lo;

  a = 0;
  x = 0;
  y = 0;

  stack_p = 0xFD;
  status = 0x00 | CpuFlags::Unused;

  m_addr_abs = 0;
  m_addr_rel = 0;
  fetched = 0;

  cpu_cycles = 8;
}

void nes_cpu::irq() noexcept {
  if (m_get_flags(CpuFlags::IntruptDisable) == 0) {

    write(0x0100 + stack_p, (pc >> 8) & 0x00FF);
    stack_p--;
    write(0x0100 + stack_p, pc & 0x00FF);
    stack_p--;

    // Then Push the status register to the stack
    m_set_flags(CpuFlags::Break, false);
    m_set_flags(CpuFlags::Unused, true);
    m_set_flags(CpuFlags::IntruptDisable, true);
    write(0x0100 + stack_p, status);
    stack_p--;

    m_addr_abs = 0xFFFE;
    uint16_t lo = read(m_addr_abs + 0);
    uint16_t hi = read(m_addr_abs + 1);
    pc = (hi << 8) | lo;

    cpu_cycles = 7;
  }
}

void nes_cpu::nmi() noexcept {
  write(0x0100 + stack_p, (pc >> 8) & 0x00FF);
  stack_p--;
  write(0x0100 + stack_p, pc & 0x00FF);
  stack_p--;

  // Then Push the status register to the stack
  m_set_flags(CpuFlags::Break, false);
  m_set_flags(CpuFlags::Unused, true);
  m_set_flags(CpuFlags::IntruptDisable, true);
  write(0x0100 + stack_p, status);
  stack_p--;

  m_addr_abs = 0xFFFA;
  uint16_t lo = read(m_addr_abs + 0);
  uint16_t hi = read(m_addr_abs + 1);
  pc = (hi << 8) | lo;

  cpu_cycles = 8;
}

void nes_cpu::connect_to_bus(nes_bus *bus) noexcept { m_bus = bus; }

bool nes_cpu::complete() const noexcept { return (cpu_cycles == 0); }

std::map<uint16_t, std::string>
nes_cpu::disassemble(uint16_t addr_start, uint16_t addr_stop) const noexcept {
  uint32_t addr = addr_start;
  uint8_t value = 0x00;
  uint8_t lo = 0x00;
  uint8_t hi = 0x00;
  std::map<uint16_t, std::string> mapLines;
  uint16_t line_addr = 0;

  while (addr <= (uint32_t)addr_stop) {
    line_addr = static_cast<uint16_t>(addr);

    auto opcode = m_get_instruction(m_bus->read(static_cast<uint16_t>(addr)));
    addr++;
    auto sInst = std::format("${:X}: {} ", addr, opcode.name);

    if (opcode.address_mode == &nes_cpu::imp) {
      sInst += std::format(" {}", "{IMP}");
    } else if (opcode.address_mode == &nes_cpu::imm) {
      value = m_bus->read(static_cast<uint16_t>(addr));
      addr++;
      sInst += std::format("#$ {:02X} {{IMM}}", value);
    } else if (opcode.address_mode == &nes_cpu::zp0) {
      lo = m_bus->read(static_cast<uint16_t>(addr));
      addr++;
      hi = 0x00;
      sInst += std::format("#$ {:02X} {{ZP0}}", lo);
    } else if (opcode.address_mode == &nes_cpu::zpx) {
      lo = m_bus->read(static_cast<uint16_t>(addr));
      addr++;
      hi = 0x00;
      sInst += std::format("#$ {:02X} {{ZPX}}", lo);
    } else if (opcode.address_mode == &nes_cpu::zpy) {
      lo = m_bus->read(static_cast<uint16_t>(addr));
      addr++;
      hi = 0x00;
      sInst += std::format("#$ {:02X} ,Y {{ZPY}}", lo);
    } else if (opcode.address_mode == &nes_cpu::izx) {
      lo = m_bus->read(static_cast<uint16_t>(addr));
      addr++;
      hi = 0x00;
      sInst += std::format("($ {:02X} ,X) {{IZX}}", lo);
    } else if (opcode.address_mode == &nes_cpu::izy) {
      lo = m_bus->read(static_cast<uint16_t>(addr));
      addr++;
      hi = 0x00;
      sInst += std::format("($ {:02X} ,Y) {{IZY}}", lo);
    } else if (opcode.address_mode == &nes_cpu::abs) {
      lo = m_bus->read(static_cast<uint16_t>(addr));
      addr++;
      hi = m_bus->read(static_cast<uint16_t>(addr));
      addr++;
      sInst += std::format("${:02X} {{ABS}}", (hi << 8) | lo);
    } else if (opcode.address_mode == &nes_cpu::azx) {
      lo = m_bus->read(static_cast<uint16_t>(addr));
      addr++;
      hi = m_bus->read(static_cast<uint16_t>(addr));
      addr++;
      sInst += std::format("${:02X} ,X) {{ABX}}", (hi << 8) | lo);
    } else if (opcode.address_mode == &nes_cpu::azy) {
      lo = m_bus->read(static_cast<uint16_t>(addr));
      addr++;
      hi = m_bus->read(static_cast<uint16_t>(addr));
      addr++;
      sInst += std::format("${:02X} ,Y {{ABY}}", (hi << 8) | lo);
    } else if (opcode.address_mode == &nes_cpu::ind) {
      lo = m_bus->read(static_cast<uint16_t>(addr));
      addr++;
      hi = m_bus->read(static_cast<uint16_t>(addr));
      addr++;
      sInst += std::format("${:02X}) {{IND}}", (hi << 8) | lo);
    } else if (opcode.address_mode == &nes_cpu::rel) {
      value = m_bus->read(static_cast<uint16_t>(addr));
      addr++;
      sInst += std::format("$ {:02X} [${:02X}] {{REL}}", value,
                           static_cast<int>(addr + value));
    }

    mapLines[line_addr] = sInst;
  }

  return mapLines;
}

uint8_t nes_cpu::m_fetch_data() noexcept {
  if (!(m_opcode.address_mode == &nes_cpu::imp)) {
    fetched = read(m_addr_abs);
    return fetched;
  }
  return 0;
}

uint8_t nes_cpu::read(uint16_t addr) noexcept { return m_bus->read(addr); }

void nes_cpu::write(uint16_t addr, uint8_t data) noexcept {
  m_bus->write(addr, data);
}

void nes_cpu::m_set_flags(CpuFlags flags, bool cond) noexcept {
  if (cond) {
    status |= flags;
  } else {
    status &= ~flags;
  }
}

uint8_t nes_cpu::m_get_flags(CpuFlags flags) const noexcept {
  return ((status & flags) > 0) ? 1 : 0;
}

uint8_t nes_cpu::nop() noexcept { return 0; }

uint8_t nes_cpu::adc() noexcept {
  (void)m_fetch_data();

  uint16_t tmp = static_cast<uint16_t>(a) + static_cast<uint16_t>(fetched) +
                 static_cast<uint16_t>(m_get_flags(Carry));
  m_set_flags(Carry, tmp > 255);

  m_set_flags(Zero, (tmp & 0x00FF) == 0);

  m_set_flags(Overflow, (~((uint16_t)a ^ (uint16_t)fetched) &
                         ((uint16_t)a ^ (uint16_t)tmp)) &
                            0x0080);

  m_set_flags(Negative, (tmp & 0x80) != 0);

  a = tmp & 0x00FF;

  return 1;
}

uint8_t nes_cpu::and_() noexcept {
  (void)m_fetch_data();
  a = a & fetched;
  m_set_flags(Negative, (a & 0x80) != 0);
  m_set_flags(Zero, a == 0x00);
  return 1;
}

uint8_t nes_cpu::asl() noexcept {
  (void)m_fetch_data();
  uint16_t tmp = static_cast<uint16_t>(fetched) << 1;
  m_set_flags(Carry, (tmp & 0xFF00) > 0);
  m_set_flags(Zero, (tmp & 0x00FF) == 0x00);
  m_set_flags(Negative, (tmp & 0x80) != 0);
  if (m_opcode.address_mode == &nes_cpu::imp) {
    a = tmp & 0x00FF;
  } else {
    write(m_addr_abs, tmp & 0x00FF);
  }
  return 0;
}

uint8_t nes_cpu::bcc() noexcept {
  if (m_get_flags(Carry) == 0) {
    cpu_cycles++;
    m_addr_abs = pc + m_addr_rel;
    if ((m_addr_abs & 0xFF00) != (pc & 0xFF00)) {

      cpu_cycles++;
    }
    pc = m_addr_abs;
  }
  return 0;
}

uint8_t nes_cpu::bcs() noexcept {
  if (m_get_flags(Carry) == 1) {
    cpu_cycles++;
    m_addr_abs = pc + m_addr_rel;
    if ((m_addr_abs & 0xFF00) != (pc & 0xFF00)) {

      cpu_cycles++;
    }
    pc = m_addr_abs;
  }
  return 0;
}

uint8_t nes_cpu::beq() noexcept {
  if (m_get_flags(Zero) == 1) {
    cpu_cycles++;
    m_addr_abs = pc + m_addr_rel;
    if ((m_addr_abs & 0xFF00) != (pc & 0xFF00)) {

      cpu_cycles++;
    }
    pc = m_addr_abs;
  }
  return 0;
}

uint8_t nes_cpu::bit() noexcept {
  (void)m_fetch_data();
  uint16_t tmp = a & fetched;
  m_set_flags(Zero, (tmp & 0x00FF) == 0x00);
  m_set_flags(CpuFlags::Negative, (fetched & (1 << 7)) != 0);
  m_set_flags(CpuFlags::Overflow, (fetched & (1 << 6)) != 0);
  return 0;
}

uint8_t nes_cpu::bmi() noexcept {
  if (m_get_flags(CpuFlags::Negative) == 1) {
    cpu_cycles++;
    m_addr_abs = pc + m_addr_rel;
    if ((m_addr_abs & 0xFF00) != (pc & 0xFF00)) {

      cpu_cycles++;
    }
    pc = m_addr_abs;
  }
  return 0;
}

uint8_t nes_cpu::bne() noexcept {
  if (m_get_flags(CpuFlags::Zero) == 0) {
    cpu_cycles++;
    m_addr_abs = pc + m_addr_rel;
    if ((m_addr_abs & 0xFF00) != (pc & 0xFF00)) {

      cpu_cycles++;
    }
    pc = m_addr_abs;
  }
  return 0;
}

uint8_t nes_cpu::bpl() noexcept {
  if (m_get_flags(CpuFlags::Negative) == 0) {
    cpu_cycles++;
    m_addr_abs = pc + m_addr_rel;
    if ((m_addr_abs & 0xFF00) != (pc & 0xFF00)) {

      cpu_cycles++;
    }
    pc = m_addr_abs;
  }
  return 0;
}

uint8_t nes_cpu::brk() noexcept {
  pc++;

  m_set_flags(CpuFlags::IntruptDisable, true);
  write(0x0100 + stack_p, (pc >> 8) & 0x00FF);
  stack_p--;
  write(0x0100 + stack_p, pc & 0x00FF);
  stack_p--;

  m_set_flags(CpuFlags::Break, true);
  write(0x0100 + stack_p, status);
  stack_p--;
  m_set_flags(CpuFlags::Break, false);

  pc = static_cast<uint16_t>(read(0xFFFE)) |
       (static_cast<uint16_t>(read(0xFFFF)) << 8);
  return 0;
}

uint8_t nes_cpu::bvc() noexcept {
  if (m_get_flags(CpuFlags::Overflow) == 0) {
    cpu_cycles++;
    m_addr_abs = pc + m_addr_rel;
    if ((m_addr_abs & 0xFF00) != (pc & 0xFF00)) {

      cpu_cycles++;
    }
    pc = m_addr_abs;
  }
  return 0;
}

uint8_t nes_cpu::bvs() noexcept {
  if (m_get_flags(CpuFlags::Overflow) == 1) {
    cpu_cycles++;
    m_addr_abs = pc + m_addr_rel;
    if ((m_addr_abs & 0xFF00) != (pc & 0xFF00)) {
      cpu_cycles++;
    }
    pc = m_addr_abs;
  }
  return 0;
}

uint8_t nes_cpu::clc() noexcept {
  m_set_flags(CpuFlags::Carry, false);
  return 0;
}

uint8_t nes_cpu::cld() noexcept {
  m_set_flags(CpuFlags::Decimal, false);
  return 0;
}

uint8_t nes_cpu::cli() noexcept {
  m_set_flags(CpuFlags::IntruptDisable, false);
  return 0;
}

uint8_t nes_cpu::clv() noexcept {
  m_set_flags(CpuFlags::Overflow, false);
  return 0;
}

uint8_t nes_cpu::cmp() noexcept {
  (void)m_fetch_data();
  uint16_t tmp = static_cast<uint16_t>(a) - fetched;
  m_set_flags(Carry, a >= fetched);
  m_set_flags(Zero, (tmp & 0x00FF) == 0x0000);
  m_set_flags(Negative, (tmp & 0x0080) != 0);
  return 1;
}

uint8_t nes_cpu::cpx() noexcept {
  (void)m_fetch_data();
  uint16_t tmp = static_cast<uint16_t>(x) - fetched;
  m_set_flags(Carry, x >= fetched);
  m_set_flags(Zero, (tmp & 0x00FF) == 0x0000);
  m_set_flags(Negative, (tmp & 0x0080) != 0);
  return 0;
}

uint8_t nes_cpu::cpy() noexcept {
  (void)m_fetch_data();
  uint16_t tmp = static_cast<uint16_t>(y) - fetched;
  m_set_flags(Carry, y >= fetched);
  m_set_flags(Zero, (tmp & 0x00FF) == 0x0000);
  m_set_flags(Negative, (tmp & 0x0080) != 0);
  return 0;
}

uint8_t nes_cpu::dec() noexcept {
  (void)m_fetch_data();
  uint16_t tmp = fetched - 1;
  write(m_addr_abs, tmp & 0x00FF);
  m_set_flags(Negative, (tmp & 0x0080) != 0);
  m_set_flags(Zero, (tmp & 0x00FF) == 0x0000);
  return 0;
}

uint8_t nes_cpu::dex() noexcept {
  x--;
  m_set_flags(Zero, x == 0x00);
  m_set_flags(Negative, (x & 0x80) != 0);
  return 0;
}

uint8_t nes_cpu::dey() noexcept {
  y--;
  m_set_flags(Negative, (y & 0x80) != 0);
  m_set_flags(Zero, y == 0x00);
  return 0;
}

uint8_t nes_cpu::eor() noexcept {
  (void)m_fetch_data();
  a = a ^ fetched;
  m_set_flags(Negative, (a & 0x80) != 0);
  m_set_flags(Zero, a == 0x00);
  return 0;
}

uint8_t nes_cpu::inc() noexcept {
  (void)m_fetch_data();
  uint16_t tmp = fetched + 1;
  write(m_addr_abs, tmp & 0x00FF);
  m_set_flags(Negative, (tmp & 0x0080) != 0);
  m_set_flags(Zero, (tmp & 0x00FF) == 0x0000);
  return 0;
}

uint8_t nes_cpu::inx() noexcept {
  x++;
  m_set_flags(Zero, x == 0x00);
  m_set_flags(Negative, (x & 0x80) != 0);
  return 0;
}

uint8_t nes_cpu::iny() noexcept {
  y++;
  m_set_flags(Negative, (y & 0x80) != 0);
  m_set_flags(Zero, y == 0x00);
  return 0;
}

uint8_t nes_cpu::jmp() noexcept {
  pc = m_addr_abs;
  return 0;
}

uint8_t nes_cpu::jsr() noexcept {
  pc--;
  write(0x100 + stack_p, (pc >> 8) & 0x00FF);
  stack_p--;
  write(0x100 + stack_p, pc & 0x00FF);
  stack_p--;
  pc = m_addr_abs;
  return 0;
}

uint8_t nes_cpu::lda() noexcept {
  (void)m_fetch_data();
  a = fetched;
  m_set_flags(Negative, (a & 0x80) != 0);
  m_set_flags(Zero, a == 0x00);
  return 1;
}

uint8_t nes_cpu::ldx() noexcept {
  (void)m_fetch_data();
  x = fetched;
  m_set_flags(Negative, (x & 0x80) != 0);
  m_set_flags(Zero, x == 0x00);
  return 1;
}

uint8_t nes_cpu::ldy() noexcept {
  (void)m_fetch_data();
  y = fetched;
  m_set_flags(Negative, (y & 0x80) != 0);
  m_set_flags(Zero, y == 0x00);
  return 1;
}

uint8_t nes_cpu::lsr() noexcept {
  (void)m_fetch_data();
  m_set_flags(Carry, fetched & 0x0001);
  uint16_t tmp = fetched >> 1;
  m_set_flags(Zero, (tmp & 0x00FF) == 0x0000);
  m_set_flags(Negative, (tmp & 0x0080) != 0);
  if (m_opcode.address_mode == &nes_cpu::imp) {
    a = tmp & 0x00FF;
  } else {
    write(m_addr_abs, tmp & 0x00FF);
  }
  return 0;
}

uint8_t nes_cpu::ora() noexcept {
  (void)m_fetch_data();
  a = a | fetched;
  m_set_flags(Zero, a == 0x00);
  m_set_flags(Negative, (a & 0x80) != 0);
  return 1;
}

uint8_t nes_cpu::pha() noexcept {
  write(0x0100 + stack_p, a);
  stack_p--;
  return 0;
}

uint8_t nes_cpu::php() noexcept {
  write(0x0100 + stack_p, status | Break | Unused);
  m_set_flags(Break, false);
  m_set_flags(Unused, false);
  stack_p--;
  return 0;
}

uint8_t nes_cpu::pla() noexcept {
  stack_p++;
  a = read(0x0100 + stack_p);
  m_set_flags(Zero, a == 0x00);
  m_set_flags(Negative, (a & 0x80) != 0);
  return 0;
}

uint8_t nes_cpu::plp() noexcept {
  stack_p++;
  status = read(0x0100 + stack_p);
  m_set_flags(Unused, true);
  return 0;
}

uint8_t nes_cpu::rol() noexcept {
  (void)m_fetch_data();
  uint16_t tmp = static_cast<uint16_t>(fetched << 1) | m_get_flags(Carry);
  m_set_flags(Carry, (tmp & 0xFF00) != 0);
  m_set_flags(Zero, (tmp & 0x00FF) == 0x0000);
  m_set_flags(Negative, (tmp & 0x0080) != 0);
  if (m_opcode.address_mode == &nes_cpu::imp) {
    a = tmp & 0x00FF;
  } else {
    write(m_addr_abs, tmp & 0x00FF);
  }
  return 0;
}

uint8_t nes_cpu::ror() noexcept {
  (void)m_fetch_data();
  uint16_t tmp =
      static_cast<uint16_t>(m_get_flags(Carry) << 7) | (fetched >> 1);
  m_set_flags(Carry, (fetched & 0x01) != 0);
  m_set_flags(Zero, (tmp & 0x00FF) == 0x00);
  m_set_flags(Negative, (tmp & 0x0080) != 0);
  if (m_opcode.address_mode == &nes_cpu::imp) {
    a = tmp & 0x00FF;
  } else {
    write(m_addr_abs, tmp & 0x00FF);
  }
  return 0;
}

uint8_t nes_cpu::rti() noexcept {
  stack_p++;
  status = read(0x0100 + stack_p);
  status &= ~Break;
  status &= ~Unused;

  stack_p++;
  pc = static_cast<uint16_t>(read(0x0100 + stack_p));
  stack_p++;
  pc |= static_cast<uint16_t>(read(0x0100 + stack_p)) << 8;
  return 0;
}

uint8_t nes_cpu::rts() noexcept {
  stack_p++;
  pc = static_cast<uint16_t>(read(0x0100 + stack_p));
  stack_p++;
  pc |= static_cast<uint16_t>(read(0x0100 + stack_p)) << 8;

  pc++;
  return 0;
}

uint8_t nes_cpu::sbc() noexcept {
  (void)m_fetch_data();

  auto value = (static_cast<uint16_t>(fetched)) ^ 0x00FF;

  auto tmp = static_cast<uint16_t>(a) + value +
             static_cast<uint16_t>(m_get_flags(Carry));
  m_set_flags(Carry, (tmp & 0xFF00) != 0);
  m_set_flags(Zero, ((tmp & 0x00FF) == 0));
  m_set_flags(Overflow,
              (tmp ^ static_cast<uint16_t>(a)) & (tmp ^ value) & 0x0080);
  m_set_flags(Negative, (tmp & 0x0080) != 0);
  a = tmp & 0x00FF;
  return 1;
}

uint8_t nes_cpu::sec() noexcept {
  m_set_flags(Carry, true);
  return 0;
}

uint8_t nes_cpu::sed() noexcept {
  m_set_flags(Decimal, true);
  return 0;
}

uint8_t nes_cpu::sei() noexcept {
  m_set_flags(IntruptDisable, true);
  return 0;
}

uint8_t nes_cpu::sta() noexcept {
  write(m_addr_abs, a);
  return 0;
}

uint8_t nes_cpu::stx() noexcept {
  write(m_addr_abs, x);
  return 0;
}

uint8_t nes_cpu::sty() noexcept {
  write(m_addr_abs, y);
  return 0;
}

uint8_t nes_cpu::tax() noexcept {
  x = a;
  m_set_flags(Negative, (x & 0x80) != 0);
  m_set_flags(Zero, x == 0x00);
  return 0;
}

uint8_t nes_cpu::tay() noexcept {
  y = a;
  m_set_flags(Negative, (y & 0x80) != 0);
  m_set_flags(Zero, y == 0x00);
  return 0;
}

uint8_t nes_cpu::tsx() noexcept {
  x = stack_p;
  m_set_flags(Negative, (x & 0x80) != 0);
  m_set_flags(Zero, x == 0x00);
  return 0;
}

uint8_t nes_cpu::txa() noexcept {
  a = x;
  m_set_flags(Negative, (a & 0x80) != 0);
  m_set_flags(Zero, a == 0x00);
  return 0;
}

uint8_t nes_cpu::txs() noexcept {
  stack_p = x;
  return 0;
}

uint8_t nes_cpu::tya() noexcept {
  a = y;
  m_set_flags(Negative, (a & 0x80) != 0);
  m_set_flags(Zero, a == 0x00);
  return 0;
}

// Addressing Modes
uint8_t nes_cpu::imm() noexcept {
  m_addr_abs = pc++;
  return 0;
}

uint8_t nes_cpu::imp() noexcept {
  fetched = a;
  return 0;
}

uint8_t nes_cpu::zp0() noexcept {
  m_addr_abs = read(pc);
  pc++;
  m_addr_abs &= 0x00FF;
  return 0;
}

uint8_t nes_cpu::zpx() noexcept {
  m_addr_abs = read(pc) + x;
  pc++;
  m_addr_abs &= 0x00FF;
  return 0;
}

uint8_t nes_cpu::zpy() noexcept {
  m_addr_abs = read(pc) + y;
  pc++;
  m_addr_abs &= 0x00FF;
  return 0;
}

uint8_t nes_cpu::abs() noexcept {
  uint16_t lo = read(pc);
  pc++;
  uint16_t hi = read(pc);
  pc++;
  m_addr_abs = (hi << 8) | lo;
  return 0;
}

uint8_t nes_cpu::azx() noexcept {
  uint16_t lo = read(pc);
  pc++;
  uint16_t hi = read(pc);
  pc++;

  m_addr_abs = (hi << 8) | lo;
  m_addr_abs += x;

  if ((m_addr_abs & 0xFF00) != (hi << 8)) {
    return 1;
  } else {
    return 0;
  }
}

uint8_t nes_cpu::azy() noexcept {
  uint16_t lo = read(pc);
  pc++;
  uint16_t hi = read(pc);
  pc++;

  m_addr_abs = (hi << 8) | lo;
  m_addr_abs += y;

  if ((m_addr_abs & 0xFF00) != (hi << 8)) {
    return 1;
  } else {
    return 0;
  }
}

uint8_t nes_cpu::ind() noexcept {
  uint16_t lo = read(pc);
  pc++;
  uint16_t hi = read(pc);
  pc++;

  uint16_t ptr = (hi << 8) | lo;

  if (lo == 0x00FF) {
    m_addr_abs = (read(ptr & 0xFF00) << 8) | read(ptr + 0);
  } else {
    m_addr_abs = (read(ptr + 1) << 8) | read(ptr + 0);
  }

  return 0;
}

uint8_t nes_cpu::izx() noexcept {
  uint16_t data = read(pc);
  pc++;

  uint16_t lo = read((uint16_t)(data + (uint16_t)x) & 0x00FF);
  uint16_t hi = read((uint16_t)(data + (uint16_t)x + 1) & 0x00FF);

  m_addr_abs = (hi << 8) | lo;

  return 0;
}

uint8_t nes_cpu::izy() noexcept {
  uint16_t data = read(pc);
  pc++;

  uint16_t lo = read(data & 0x00FF);
  uint16_t hi = read((data + 1) & 0x00FF);

  m_addr_abs = (hi << 8) | lo;
  m_addr_abs += y;

  if ((m_addr_abs & 0xFF00) != (hi << 8)) {

    return 1;
  } else {

    return 0;
  }
}

uint8_t nes_cpu::rel() noexcept {
  m_addr_rel = read(pc);
  pc++;
  if (m_addr_rel & 0x80) {
    m_addr_rel |= 0xFF00;
  }
  return 0;
}

instructions nes_cpu::m_get_instruction(uint8_t opcode) noexcept {
  switch (opcode) {
  case 0xEA:
    return {"Nop", &nes_cpu::nop, &nes_cpu::imp, 2}; //::Implied,
  case 0x69:
    return {"Adc", &nes_cpu::adc, &nes_cpu::imm, 2}; //::Immediate,
  case 0x65:
    return {"Adc", &nes_cpu::adc, &nes_cpu::zp0, 3}; //::ZeroPage,
  case 0x75:
    return {"Adc", &nes_cpu::adc, &nes_cpu::zpx, 4}; // nes_cpu::zp0X,
  case 0x6D:
    return {"Adc", &nes_cpu::adc, &nes_cpu::abs, 4}; //::Absolute,
  case 0x7D:
    return {"Adc", &nes_cpu::adc, &nes_cpu::azx, 4}; //::AbsoluteX,
  case 0x79:
    return {"Adc", &nes_cpu::adc, &nes_cpu::azy, 4}; //::AbsoluteY,
  case 0x61:
    return {"Adc", &nes_cpu::adc, &nes_cpu::izx, 6}; //::IndriectX,
  case 0x71:
    return {"Adc", &nes_cpu::adc, &nes_cpu::izy, 5}; //::IndriectY,
  case 0x29:
    return {"And", &nes_cpu::and_, &nes_cpu::imm, 2}; //::Immediate,
  case 0x25:
    return {"And", &nes_cpu::and_, &nes_cpu::zp0, 3}; //::ZeroPage,
  case 0x35:
    return {"And", &nes_cpu::and_, &nes_cpu::zpx, 4}; // nes_cpu::zp0X,
  case 0x2D:
    return {"And", &nes_cpu::and_, &nes_cpu::abs, 4}; //::Absolute,
  case 0x3D:
    return {"And", &nes_cpu::and_, &nes_cpu::azx, 4}; //::AbsoluteX,
  case 0x39:
    return {"And", &nes_cpu::and_, &nes_cpu::azy, 4}; //::AbsoluteY,
  case 0x21:
    return {"And", &nes_cpu::and_, &nes_cpu::izx, 6}; //::IndriectX,
  case 0x31:
    return {"And", &nes_cpu::and_, &nes_cpu::izy, 5}; //::IndriectY,
  case 0x0A:
    return {"Asl", &nes_cpu::asl, &nes_cpu::imp, 2}; //::Implied,
  case 0x06:
    return {"Asl", &nes_cpu::asl, &nes_cpu::zp0, 5}; //::ZeroPage,
  case 0x16:
    return {"Asl", &nes_cpu::asl, &nes_cpu::zpx, 6}; // nes_cpu::zp0X,
  case 0x0E:
    return {"Asl", &nes_cpu::asl, &nes_cpu::abs, 6}; //::Absolute,
  case 0x1E:
    return {"Asl", &nes_cpu::asl, &nes_cpu::azx, 7}; //::AbsoluteX,
  case 0x90:
    return {"Bcc", &nes_cpu::bcc, &nes_cpu::rel, 2}; //::Relative,
  case 0xB0:
    return {"Bcs", &nes_cpu::bcs, &nes_cpu::rel, 2}; //::Relative,
  case 0xF0:
    return {"Beq", &nes_cpu::beq, &nes_cpu::rel, 2}; //::Relative,
  case 0x24:
    return {"Bit", &nes_cpu::bit, &nes_cpu::zp0, 3}; //::ZeroPage,
  case 0x2C:
    return {"Bit", &nes_cpu::bit, &nes_cpu::abs, 4}; //::Absolute,
  case 0x30:
    return {"Bmi", &nes_cpu::bmi, &nes_cpu::rel, 2}; //::Relative,
  case 0xD0:
    return {"Bne", &nes_cpu::bne, &nes_cpu::rel, 2}; //::Relative,
  case 0x10:
    return {"Bpl", &nes_cpu::bpl, &nes_cpu::rel, 2}; //::Relative,
  case 0x00:
    return {"Brk", &nes_cpu::brk, &nes_cpu::imp, 7}; //::Implied,
  case 0x50:
    return {"Bvc", &nes_cpu::bvc, &nes_cpu::rel, 2}; //::Relative,
  case 0x70:
    return {"Bvs", &nes_cpu::bvs, &nes_cpu::rel, 2}; //::Relative,
  case 0x18:
    return {"Clc", &nes_cpu::clc, &nes_cpu::imp, 2}; //::Implied,
  case 0xD8:
    return {"Cld", &nes_cpu::cld, &nes_cpu::imp, 2}; //::Implied,
  case 0x58:
    return {"Cli", &nes_cpu::cli, &nes_cpu::imp, 2}; //::Implied,
  case 0xB8:
    return {"Clv", &nes_cpu::clv, &nes_cpu::imp, 2}; //::Implied,
  case 0xC9:
    return {"Cmp", &nes_cpu::cmp, &nes_cpu::imm, 2}; //::Immediate,
  case 0xC5:
    return {"Cmp", &nes_cpu::cmp, &nes_cpu::zp0, 3}; //::ZeroPage,
  case 0xD5:
    return {"Cmp", &nes_cpu::cmp, &nes_cpu::zpx, 4}; // nes_cpu::zp0X,
  case 0xCD:
    return {"Cmp", &nes_cpu::cmp, &nes_cpu::abs, 4}; //::Absolute,
  case 0xDD:
    return {"Cmp", &nes_cpu::cmp, &nes_cpu::azx, 4}; //::AbsoluteX,
  case 0xD9:
    return {"Cmp", &nes_cpu::cmp, &nes_cpu::azy, 4}; //::AbsoluteY,
  case 0xC1:
    return {"Cmp", &nes_cpu::cmp, &nes_cpu::izx, 6}; //::IndriectX,
  case 0xD1:
    return {"Cmp", &nes_cpu::cmp, &nes_cpu::izy, 5}; //::IndriectY,
  case 0xE0:
    return {"Cpx", &nes_cpu::cpx, &nes_cpu::imm, 2}; //::Immediate,
  case 0xE4:
    return {"Cpx", &nes_cpu::cpx, &nes_cpu::zp0, 3}; //::ZeroPage,
  case 0xEC:
    return {"Cpx", &nes_cpu::cpx, &nes_cpu::abs, 4}; //::Absolute,
  case 0xC0:
    return {"Cpy", &nes_cpu::cpy, &nes_cpu::imm, 2}; //::Immediate,
  case 0xC4:
    return {"Cpy", &nes_cpu::cpy, &nes_cpu::zp0, 3}; //::ZeroPage,
  case 0xCC:
    return {"Cpy", &nes_cpu::cpy, &nes_cpu::abs, 4}; //::Absolute,
  case 0xC6:
    return {"Dec", &nes_cpu::dec, &nes_cpu::zp0, 5}; //::ZeroPage,
  case 0xD6:
    return {"Dec", &nes_cpu::dec, &nes_cpu::zpx, 6}; // nes_cpu::zp0X,
  case 0xCE:
    return {"Dec", &nes_cpu::dec, &nes_cpu::abs, 6}; //::Absolute,
  case 0xDE:
    return {"Dec", &nes_cpu::dec, &nes_cpu::azx, 7}; //::AbsoluteX,
  case 0xCA:
    return {"Dex", &nes_cpu::dex, &nes_cpu::imp, 2}; //::Implied,
  case 0x88:
    return {"Dey", &nes_cpu::dey, &nes_cpu::imp, 2}; //::Implied,
  case 0x49:
    return {"Eor", &nes_cpu::eor, &nes_cpu::imm, 2}; //::Immediate,
  case 0x45:
    return {"Eor", &nes_cpu::eor, &nes_cpu::zp0, 3}; //::ZeroPage,
  case 0x55:
    return {"Eor", &nes_cpu::eor, &nes_cpu::zpx, 4}; // nes_cpu::zp0X,
  case 0x4D:
    return {"Eor", &nes_cpu::eor, &nes_cpu::abs, 4}; //::Absolute,
  case 0x5D:
    return {"Eor", &nes_cpu::eor, &nes_cpu::azx, 4}; //::AbsoluteX,
  case 0x59:
    return {"Eor", &nes_cpu::eor, &nes_cpu::azy, 4}; //::AbsoluteY,
  case 0x41:
    return {"Eor", &nes_cpu::eor, &nes_cpu::izx, 6}; //::IndriectX,
  case 0x51:
    return {"Eor", &nes_cpu::eor, &nes_cpu::izy, 5}; //::IndriectY,
  case 0xE6:
    return {"Inc", &nes_cpu::inc, &nes_cpu::zp0, 5}; //::ZeroPage,
  case 0xF6:
    return {"Inc", &nes_cpu::inc, &nes_cpu::zpx, 6}; // nes_cpu::zp0X,
  case 0xEE:
    return {"Inc", &nes_cpu::inc, &nes_cpu::abs, 6}; //::Absolute,
  case 0xFE:
    return {"Inc", &nes_cpu::inc, &nes_cpu::azx, 7}; //::AbsoluteX,
  case 0xE8:
    return {"Inx", &nes_cpu::inx, &nes_cpu::imp, 2}; //::Implied,
  case 0xC8:
    return {"Iny", &nes_cpu::iny, &nes_cpu::imp, 2}; //::Implied,
  case 0x4C:
    return {"Jmp", &nes_cpu::jmp, &nes_cpu::abs, 3}; //::Absolute,
  case 0x6C:
    return {"Jmp", &nes_cpu::jmp, &nes_cpu::ind, 5}; //::Indriect,
  case 0x20:
    return {"Jsr", &nes_cpu::jsr, &nes_cpu::abs, 6}; //::Absolute,
  case 0xA9:
    return {"Lda", &nes_cpu::lda, &nes_cpu::imm, 2}; //::Immediate,
  case 0xA5:
    return {"Lda", &nes_cpu::lda, &nes_cpu::zp0, 3}; //::ZeroPage,
  case 0xB5:
    return {"Lda", &nes_cpu::lda, &nes_cpu::zpx, 4}; // nes_cpu::zp0X,
  case 0xAD:
    return {"Lda", &nes_cpu::lda, &nes_cpu::abs, 4}; //::Absolute,
  case 0xBD:
    return {"Lda", &nes_cpu::lda, &nes_cpu::azx, 4}; //::AbsoluteX,
  case 0xB9:
    return {"Lda", &nes_cpu::lda, &nes_cpu::azy, 4}; //::AbsoluteY,
  case 0xA1:
    return {"Lda", &nes_cpu::lda, &nes_cpu::izx, 6}; //::IndriectX,
  case 0xB1:
    return {"Lda", &nes_cpu::lda, &nes_cpu::izy, 5}; //::IndriectY,
  case 0xA2:
    return {"Ldx", &nes_cpu::ldx, &nes_cpu::imm, 2}; //::Immediate,
  case 0xA6:
    return {"Ldx", &nes_cpu::ldx, &nes_cpu::zp0, 3}; //::ZeroPage,
  case 0xB6:
    return {"Ldx", &nes_cpu::ldx, &nes_cpu::zpy, 4}; //::ZeroPageY,
  case 0xAE:
    return {"Ldx", &nes_cpu::ldx, &nes_cpu::abs, 4}; //::Absolute,
  case 0xBE:
    return {"Ldx", &nes_cpu::ldx, &nes_cpu::azy, 4}; //::AbsoluteY,
  case 0xA0:
    return {"Ldy", &nes_cpu::ldy, &nes_cpu::imm, 2}; //::Immediate,
  case 0xA4:
    return {"Ldy", &nes_cpu::ldy, &nes_cpu::zp0, 3}; //::ZeroPage,
  case 0xB4:
    return {"Ldy", &nes_cpu::ldy, &nes_cpu::zpx, 4}; // nes_cpu::zp0X,
  case 0xAC:
    return {"Ldy", &nes_cpu::ldy, &nes_cpu::abs, 4}; //::Absolute,
  case 0xBC:
    return {"Ldy", &nes_cpu::ldy, &nes_cpu::azx, 4}; //::AbsoluteX,
  case 0x4A:
    return {"Lsr", &nes_cpu::lsr, &nes_cpu::imp, 2}; //::Implied,
  case 0x46:
    return {"Lsr", &nes_cpu::lsr, &nes_cpu::zp0, 5}; //::ZeroPage,
  case 0x56:
    return {"Lsr", &nes_cpu::lsr, &nes_cpu::zpx, 6}; // nes_cpu::zp0X,
  case 0x4E:
    return {"Lsr", &nes_cpu::lsr, &nes_cpu::abs, 6}; //::Absolute,
  case 0x5E:
    return {"Lsr", &nes_cpu::lsr, &nes_cpu::azx, 7}; //::AbsoluteX,
  case 0x09:
    return {"Ora", &nes_cpu::ora, &nes_cpu::imm, 2}; //::Immediate,
  case 0x05:
    return {"Ora", &nes_cpu::ora, &nes_cpu::zp0, 3}; //::ZeroPage,
  case 0x15:
    return {"Ora", &nes_cpu::ora, &nes_cpu::zpx, 4}; // nes_cpu::zp0X,
  case 0x0D:
    return {"Ora", &nes_cpu::ora, &nes_cpu::abs, 4}; //::Absolute,
  case 0x1D:
    return {"Ora", &nes_cpu::ora, &nes_cpu::azx, 4}; //::AbsoluteX,
  case 0x19:
    return {"Ora", &nes_cpu::ora, &nes_cpu::azy, 4}; //::AbsoluteY,
  case 0x01:
    return {"Ora", &nes_cpu::ora, &nes_cpu::izx, 6}; //::IndriectX,
  case 0x11:
    return {"Ora", &nes_cpu::ora, &nes_cpu::izy, 5}; //::IndriectY,
  case 0x48:
    return {"Pha", &nes_cpu::pha, &nes_cpu::imp, 3}; //::Implied,
  case 0x08:
    return {"Php", &nes_cpu::php, &nes_cpu::imp, 3}; //::Implied,
  case 0x68:
    return {"Pla", &nes_cpu::pla, &nes_cpu::imp, 4}; //::Implied,
  case 0x28:
    return {"Plp", &nes_cpu::plp, &nes_cpu::imp, 4}; //::Implied,
  case 0x2A:
    return {"Rol", &nes_cpu::rol, &nes_cpu::imp, 2}; //::Implied,
  case 0x26:
    return {"Rol", &nes_cpu::rol, &nes_cpu::zp0, 5}; //::ZeroPage,
  case 0x36:
    return {"Rol", &nes_cpu::rol, &nes_cpu::zpx, 6}; // nes_cpu::zp0X,
  case 0x2E:
    return {"Rol", &nes_cpu::rol, &nes_cpu::abs, 6}; //::Absolute,
  case 0x3E:
    return {"Rol", &nes_cpu::rol, &nes_cpu::azx, 7}; //::AbsoluteX,
  case 0x6A:
    return {"Ror", &nes_cpu::ror, &nes_cpu::imp, 2}; //::Implied,
  case 0x66:
    return {"Ror", &nes_cpu::ror, &nes_cpu::zp0, 5}; //::ZeroPage,
  case 0x76:
    return {"Ror", &nes_cpu::ror, &nes_cpu::zpx, 6}; // nes_cpu::zp0X,
  case 0x6E:
    return {"Ror", &nes_cpu::ror, &nes_cpu::abs, 6}; //::Absolute,
  case 0x7E:
    return {"Ror", &nes_cpu::ror, &nes_cpu::azx, 7}; //::AbsoluteX,
  case 0x40:
    return {"Rti", &nes_cpu::rti, &nes_cpu::imp, 6}; //::Implied,
  case 0x60:
    return {"Rts", &nes_cpu::rts, &nes_cpu::imp, 6}; //::Implied,
  case 0xE9:
    return {"Sbc", &nes_cpu::sbc, &nes_cpu::imm, 2}; //::Immediate,
  case 0xE5:
    return {"Sbc", &nes_cpu::sbc, &nes_cpu::zp0, 3}; //::ZeroPage,
  case 0xF5:
    return {"Sbc", &nes_cpu::sbc, &nes_cpu::zpx, 4}; // nes_cpu::zp0X,
  case 0xED:
    return {"Sbc", &nes_cpu::sbc, &nes_cpu::abs, 4}; //::Absolute,
  case 0xFD:
    return {"Sbc", &nes_cpu::sbc, &nes_cpu::azx, 4}; //::AbsoluteX,
  case 0xF9:
    return {"Sbc", &nes_cpu::sbc, &nes_cpu::azy, 4}; //::AbsoluteY,
  case 0xE1:
    return {"Sbc", &nes_cpu::sbc, &nes_cpu::izx, 6}; //::IndriectX,
  case 0xF1:
    return {"Sbc", &nes_cpu::sbc, &nes_cpu::izy, 5}; //::IndriectY,
  case 0x38:
    return {"Sec", &nes_cpu::sec, &nes_cpu::imp, 2}; //::Implied,
  case 0xF8:
    return {"Sed", &nes_cpu::sed, &nes_cpu::imp, 2}; //::Implied,
  case 0x78:
    return {"Sei", &nes_cpu::sei, &nes_cpu::imp, 2}; //::Implied,
  case 0x85:
    return {"Sta", &nes_cpu::sta, &nes_cpu::zp0, 3}; //::ZeroPage,
  case 0x95:
    return {"Sta", &nes_cpu::sta, &nes_cpu::zpx, 4}; // nes_cpu::zp0X,
  case 0x8D:
    return {"Sta", &nes_cpu::sta, &nes_cpu::abs, 4}; //::Absolute,
  case 0x9D:
    return {"Sta", &nes_cpu::sta, &nes_cpu::azx, 5}; //::AbsoluteX,
  case 0x99:
    return {"Sta", &nes_cpu::sta, &nes_cpu::azy, 5}; //::AbsoluteY,
  case 0x81:
    return {"Sta", &nes_cpu::sta, &nes_cpu::izx, 6}; //::IndriectX,
  case 0x91:
    return {"Sta", &nes_cpu::sta, &nes_cpu::izy, 6}; //::IndriectY,
  case 0x86:
    return {"Stx", &nes_cpu::stx, &nes_cpu::zp0, 3}; //::ZeroPage,
  case 0x96:
    return {"Stx", &nes_cpu::stx, &nes_cpu::zpy, 4}; //::ZeroPageY,
  case 0x8E:
    return {"Stx", &nes_cpu::stx, &nes_cpu::abs, 4}; //::Absolute,
  case 0x84:
    return {"Sty", &nes_cpu::sty, &nes_cpu::zp0, 3}; //::ZeroPage,
  case 0x94:
    return {"Sty", &nes_cpu::sty, &nes_cpu::zpx, 4}; // nes_cpu::zp0X,
  case 0x8C:
    return {"Sty", &nes_cpu::sty, &nes_cpu::abs, 4}; //::Absolute,
  case 0xAA:
    return {"Tax", &nes_cpu::tax, &nes_cpu::imp, 2}; //::Implied,
  case 0xA8:
    return {"Tay", &nes_cpu::tay, &nes_cpu::imp, 2}; //::Implied,
  case 0xBA:
    return {"Tsx", &nes_cpu::tsx, &nes_cpu::imp, 2}; //::Implied,
  case 0x8A:
    return {"Txa", &nes_cpu::txa, &nes_cpu::imp, 2}; //::Implied,
  case 0x9A:
    return {"Txs", &nes_cpu::txs, &nes_cpu::imp, 2}; //::Implied,
  case 0x98:
    return {"Tya", &nes_cpu::tya, &nes_cpu::imp, 2}; //::Implied,
  default:
    return {"Nop", &nes_cpu::nop, &nes_cpu::imp, 2};
  }
}

} // namespace nes_emu
