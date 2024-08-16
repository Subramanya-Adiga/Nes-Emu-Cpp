#include "nes_cpu.h"

namespace nes_emu {

void nes_cpu::clock() noexcept {}

void nes_cpu::reset() noexcept {}

std::vector<std::pair<uint16_t, std::string_view>>
nes_cpu::disassemble() const noexcept {
  return std::vector<std::pair<uint16_t, std::string_view>>();
}

uint8_t nes_cpu::m_fetch_data() noexcept { return 0; }

uint8_t nes_cpu::nop() noexcept { return 0; }

uint8_t nes_cpu::adc() noexcept { return 0; }

uint8_t nes_cpu::and_() noexcept { return 0; }

uint8_t nes_cpu::asl() noexcept { return 0; }

uint8_t nes_cpu::bcc() noexcept { return 0; }

uint8_t nes_cpu::bcs() noexcept { return 0; }

uint8_t nes_cpu::beq() noexcept { return 0; }

uint8_t nes_cpu::bit() noexcept { return 0; }

uint8_t nes_cpu::bmi() noexcept { return 0; }

uint8_t nes_cpu::bne() noexcept { return 0; }

uint8_t nes_cpu::bpl() noexcept { return 0; }

uint8_t nes_cpu::brk() noexcept { return 0; }

uint8_t nes_cpu::bvc() noexcept { return 0; }

uint8_t nes_cpu::bvs() noexcept { return 0; }

uint8_t nes_cpu::clc() noexcept { return 0; }

uint8_t nes_cpu::cld() noexcept { return 0; }

uint8_t nes_cpu::cli() noexcept { return 0; }

uint8_t nes_cpu::clv() noexcept { return 0; }

uint8_t nes_cpu::cmp() noexcept { return 0; }

uint8_t nes_cpu::cpx() noexcept { return 0; }

uint8_t nes_cpu::cpy() noexcept { return 0; }

uint8_t nes_cpu::dec() noexcept { return 0; }

uint8_t nes_cpu::dex() noexcept { return 0; }

uint8_t nes_cpu::dey() noexcept { return 0; }

uint8_t nes_cpu::eor() noexcept { return 0; }

uint8_t nes_cpu::inc() noexcept { return 0; }

uint8_t nes_cpu::inx() noexcept { return 0; }

uint8_t nes_cpu::iny() noexcept { return 0; }

uint8_t nes_cpu::jmp() noexcept { return 0; }

uint8_t nes_cpu::jsr() noexcept { return 0; }

uint8_t nes_cpu::lda() noexcept { return 0; }

uint8_t nes_cpu::ldx() noexcept { return 0; }

uint8_t nes_cpu::ldy() noexcept { return 0; }

uint8_t nes_cpu::lsr() noexcept { return 0; }

uint8_t nes_cpu::ora() noexcept { return 0; }

uint8_t nes_cpu::pha() noexcept { return 0; }

uint8_t nes_cpu::php() noexcept { return 0; }

uint8_t nes_cpu::pla() noexcept { return 0; }

uint8_t nes_cpu::plp() noexcept { return 0; }

uint8_t nes_cpu::rol() noexcept { return 0; }

uint8_t nes_cpu::ror() noexcept { return 0; }

uint8_t nes_cpu::rti() noexcept { return 0; }

uint8_t nes_cpu::rts() noexcept { return 0; }

uint8_t nes_cpu::sbc() noexcept { return 0; }

uint8_t nes_cpu::sec() noexcept { return 0; }

uint8_t nes_cpu::sed() noexcept { return 0; }

uint8_t nes_cpu::sei() noexcept { return 0; }

uint8_t nes_cpu::sta() noexcept { return 0; }

uint8_t nes_cpu::stx() noexcept { return 0; }

uint8_t nes_cpu::sty() noexcept { return 0; }

uint8_t nes_cpu::tax() noexcept { return 0; }

uint8_t nes_cpu::tay() noexcept { return 0; }

uint8_t nes_cpu::tsx() noexcept { return 0; }

uint8_t nes_cpu::txa() noexcept { return 0; }

uint8_t nes_cpu::txs() noexcept { return 0; }

uint8_t nes_cpu::tya() noexcept { return 0; }

uint8_t nes_cpu::imm() noexcept { return 0; }

uint8_t nes_cpu::imp() noexcept { return 0; }

uint8_t nes_cpu::zp0() noexcept { return 0; }

uint8_t nes_cpu::zpx() noexcept { return 0; }

uint8_t nes_cpu::zpy() noexcept { return 0; }

uint8_t nes_cpu::abs() noexcept { return 0; }

uint8_t nes_cpu::azx() noexcept { return 0; }

uint8_t nes_cpu::azy() noexcept { return 0; }

uint8_t nes_cpu::ind() noexcept { return 0; }

uint8_t nes_cpu::izx() noexcept { return 0; }

uint8_t nes_cpu::izy() noexcept { return 0; }

uint8_t nes_cpu::rel() noexcept { return 0; }

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
