#include "instruction.hpp"
#include <cstdint>

namespace nes_emu {
Instruction process_opcode(uint8_t opcode) {
  switch (opcode) {
  case 0x69:
    return {.name = "Adc",
            .mnemonic = Mnemonic::Adc,
            .addressing_mode = AddressingMode::Immediate,
            .cycles = 2};
  case 0x65:
    return {.name = "Adc",
            .mnemonic = Mnemonic::Adc,
            .addressing_mode = AddressingMode::ZeroPage,
            .cycles = 3};
  case 0x75:
    return {.name = "Adc",
            .mnemonic = Mnemonic::Adc,
            .addressing_mode = AddressingMode::ZeroPageX,
            .cycles = 4};
  case 0x6D:
    return {.name = "Adc",
            .mnemonic = Mnemonic::Adc,
            .addressing_mode = AddressingMode::Absolute,
            .cycles = 4};
  case 0x7D:
    return {.name = "Adc",
            .mnemonic = Mnemonic::Adc,
            .addressing_mode = AddressingMode::AbsoluteX,
            .cycles = 4};
  case 0x79:
    return {.name = "Adc",
            .mnemonic = Mnemonic::Adc,
            .addressing_mode = AddressingMode::AbsoluteY,
            .cycles = 4};
  case 0x61:
    return {.name = "Adc",
            .mnemonic = Mnemonic::Adc,
            .addressing_mode = AddressingMode::IndirectX,
            .cycles = 6};
  case 0x71:
    return {.name = "Adc",
            .mnemonic = Mnemonic::Adc,
            .addressing_mode = AddressingMode::IndirectY,
            .cycles = 5};
  // And
  case 0x29:
    return {.name = "And",
            .mnemonic = Mnemonic::And,
            .addressing_mode = AddressingMode::Immediate,
            .cycles = 2};
  case 0x25:
    return {.name = "And",
            .mnemonic = Mnemonic::And,
            .addressing_mode = AddressingMode::ZeroPage,
            .cycles = 3};
  case 0x35:
    return {.name = "And",
            .mnemonic = Mnemonic::And,
            .addressing_mode = AddressingMode::ZeroPageX,
            .cycles = 4};
  case 0x2D:
    return {.name = "And",
            .mnemonic = Mnemonic::And,
            .addressing_mode = AddressingMode::Absolute,
            .cycles = 4};
  case 0x3D:
    return {.name = "And",
            .mnemonic = Mnemonic::And,
            .addressing_mode = AddressingMode::AbsoluteX,
            .cycles = 4};
  case 0x39:
    return {.name = "And",
            .mnemonic = Mnemonic::And,
            .addressing_mode = AddressingMode::AbsoluteY,
            .cycles = 4};
  case 0x21:
    return {.name = "And",
            .mnemonic = Mnemonic::And,
            .addressing_mode = AddressingMode::IndirectX,
            .cycles = 6};
  case 0x31:
    return {.name = "And",
            .mnemonic = Mnemonic::And,
            .addressing_mode = AddressingMode::IndirectY,
            .cycles = 5};
  // ASl
  case 0x0A:
    return {.name = "Asl",
            .mnemonic = Mnemonic::Asl,
            .addressing_mode = AddressingMode::Accumilate,
            .cycles = 2};
  case 0x06:
    return {.name = "Asl",
            .mnemonic = Mnemonic::Asl,
            .addressing_mode = AddressingMode::ZeroPage,
            .cycles = 5};
  case 0x16:
    return {.name = "Asl",
            .mnemonic = Mnemonic::Asl,
            .addressing_mode = AddressingMode::ZeroPageX,
            .cycles = 6};
  case 0x0E:
    return {.name = "Asl",
            .mnemonic = Mnemonic::Asl,
            .addressing_mode = AddressingMode::Absolute,
            .cycles = 6};
  case 0x1E:
    return {.name = "Asl",
            .mnemonic = Mnemonic::Asl,
            .addressing_mode = AddressingMode::AbsoluteX,
            .cycles = 7};
  // BCC
  case 0x90:
    return {.name = "Bcc",
            .mnemonic = Mnemonic::Bcc,
            .addressing_mode = AddressingMode::Relative,
            .cycles = 2};
  // Bcs
  case 0xB0:
    return {.name = "Bcs",
            .mnemonic = Mnemonic::Bcs,
            .addressing_mode = AddressingMode::Relative,
            .cycles = 2};
  // Beq
  case 0xF0:
    return {.name = "Beq",
            .mnemonic = Mnemonic::Beq,
            .addressing_mode = AddressingMode::Relative,
            .cycles = 2};
  // Bit
  case 0x24:
    return {.name = "Bit",
            .mnemonic = Mnemonic::Bit,
            .addressing_mode = AddressingMode::ZeroPage,
            .cycles = 3};
  case 0x2C:
    return {.name = "Bit",
            .mnemonic = Mnemonic::Bit,
            .addressing_mode = AddressingMode::Absolute,
            .cycles = 4};
  // Bmi
  case 0x30:
    return {.name = "Bmi",
            .mnemonic = Mnemonic::Bmi,
            .addressing_mode = AddressingMode::Relative,
            .cycles = 2};
  // Bne
  case 0xD0:
    return {.name = "Bne",
            .mnemonic = Mnemonic::Bne,
            .addressing_mode = AddressingMode::Relative,
            .cycles = 2};
  // Bpl
  case 0x10:
    return {.name = "Bpl",
            .mnemonic = Mnemonic::Bpl,
            .addressing_mode = AddressingMode::Relative,
            .cycles = 2};
  // Brk
  case 0x00:
    return {.name = "Brk",
            .mnemonic = Mnemonic::Brk,
            .addressing_mode = AddressingMode::Implied,
            .cycles = 7};
  // Bvc
  case 0x50:
    return {.name = "Bvc",
            .mnemonic = Mnemonic::Bvc,
            .addressing_mode = AddressingMode::Relative,
            .cycles = 2};
  // Bvs
  case 0x70:
    return {.name = "Bvs",
            .mnemonic = Mnemonic::Bvs,
            .addressing_mode = AddressingMode::Relative,
            .cycles = 2};
  // Clc
  case 0x18:
    return {.name = "Clc",
            .mnemonic = Mnemonic::Clc,
            .addressing_mode = AddressingMode::Implied,
            .cycles = 2};
  // Cld
  case 0xD8:
    return {.name = "Cld",
            .mnemonic = Mnemonic::Cld,
            .addressing_mode = AddressingMode::Implied,
            .cycles = 2};
  // Cli
  case 0x58:
    return {.name = "Cli",
            .mnemonic = Mnemonic::Cli,
            .addressing_mode = AddressingMode::Implied,
            .cycles = 2};
  // Clv
  case 0xB8:
    return {.name = "Clv",
            .mnemonic = Mnemonic::Clv,
            .addressing_mode = AddressingMode::Implied,
            .cycles = 2};
  // Cmp
  case 0xC9:
    return {.name = "Cmp",
            .mnemonic = Mnemonic::Cmp,
            .addressing_mode = AddressingMode::Immediate,
            .cycles = 2};
  case 0xC5:
    return {.name = "Cmp",
            .mnemonic = Mnemonic::Cmp,
            .addressing_mode = AddressingMode::ZeroPage,
            .cycles = 3};
  case 0xD5:
    return {.name = "Cmp",
            .mnemonic = Mnemonic::Cmp,
            .addressing_mode = AddressingMode::ZeroPageX,
            .cycles = 4};
  case 0xCD:
    return {.name = "Cmp",
            .mnemonic = Mnemonic::Cmp,
            .addressing_mode = AddressingMode::Absolute,
            .cycles = 4};
  case 0xDD:
    return {.name = "Cmp",
            .mnemonic = Mnemonic::Cmp,
            .addressing_mode = AddressingMode::AbsoluteX,
            .cycles = 4};
  case 0xD9:
    return {.name = "Cmp",
            .mnemonic = Mnemonic::Cmp,
            .addressing_mode = AddressingMode::AbsoluteY,
            .cycles = 4};
  case 0xC1:
    return {.name = "Cmp",
            .mnemonic = Mnemonic::Cmp,
            .addressing_mode = AddressingMode::IndirectX,
            .cycles = 6};
  case 0xD1:
    return {.name = "Cmp",
            .mnemonic = Mnemonic::Cmp,
            .addressing_mode = AddressingMode::IndirectY,
            .cycles = 5};
  // Cpx
  case 0xE0:
    return {.name = "Cpx",
            .mnemonic = Mnemonic::Cpx,
            .addressing_mode = AddressingMode::Immediate,
            .cycles = 2};
  case 0xE4:
    return {.name = "Cpx",
            .mnemonic = Mnemonic::Cpx,
            .addressing_mode = AddressingMode::ZeroPage,
            .cycles = 3};
  case 0xEC:
    return {.name = "Cpx",
            .mnemonic = Mnemonic::Cpx,
            .addressing_mode = AddressingMode::Absolute,
            .cycles = 4};
  // Cpy
  case 0xC0:
    return {.name = "Cpy",
            .mnemonic = Mnemonic::Cpy,
            .addressing_mode = AddressingMode::Immediate,
            .cycles = 2};
  case 0xC4:
    return {.name = "Cpy",
            .mnemonic = Mnemonic::Cpy,
            .addressing_mode = AddressingMode::ZeroPage,
            .cycles = 3};
  case 0xCC:
    return {.name = "Cpy",
            .mnemonic = Mnemonic::Cpy,
            .addressing_mode = AddressingMode::Absolute,
            .cycles = 4};
  // Dec
  case 0xC6:
    return {.name = "Dec",
            .mnemonic = Mnemonic::Dec,
            .addressing_mode = AddressingMode::ZeroPage,
            .cycles = 5};
  case 0xD6:
    return {.name = "Dec",
            .mnemonic = Mnemonic::Dec,
            .addressing_mode = AddressingMode::ZeroPageX,
            .cycles = 6};
  case 0xCE:
    return {.name = "Dec",
            .mnemonic = Mnemonic::Dec,
            .addressing_mode = AddressingMode::Absolute,
            .cycles = 6};
  case 0xDE:
    return {.name = "Dec",
            .mnemonic = Mnemonic::Dec,
            .addressing_mode = AddressingMode::AbsoluteX,
            .cycles = 7};
  // Dex
  case 0xCA:
    return {.name = "Dex",
            .mnemonic = Mnemonic::Dex,
            .addressing_mode = AddressingMode::Implied,
            .cycles = 2};
  // Dey
  case 0x88:
    return {.name = "Dey",
            .mnemonic = Mnemonic::Dey,
            .addressing_mode = AddressingMode::Implied,
            .cycles = 2};
  // Eor
  case 0x49:
    return {.name = "Eor",
            .mnemonic = Mnemonic::Eor,
            .addressing_mode = AddressingMode::Immediate,
            .cycles = 2};
  case 0x45:
    return {.name = "Eor",
            .mnemonic = Mnemonic::Eor,
            .addressing_mode = AddressingMode::ZeroPage,
            .cycles = 3};
  case 0x55:
    return {.name = "Eor",
            .mnemonic = Mnemonic::Eor,
            .addressing_mode = AddressingMode::ZeroPageX,
            .cycles = 4};
  case 0x4D:
    return {.name = "Eor",
            .mnemonic = Mnemonic::Eor,
            .addressing_mode = AddressingMode::Absolute,
            .cycles = 4};
  case 0x5D:
    return {.name = "Eor",
            .mnemonic = Mnemonic::Eor,
            .addressing_mode = AddressingMode::AbsoluteX,
            .cycles = 4};
  case 0x59:
    return {.name = "Eor",
            .mnemonic = Mnemonic::Eor,
            .addressing_mode = AddressingMode::AbsoluteY,
            .cycles = 4};
  case 0x41:
    return {.name = "Eor",
            .mnemonic = Mnemonic::Eor,
            .addressing_mode = AddressingMode::IndirectX,
            .cycles = 6};
  case 0x51:
    return {.name = "Eor",
            .mnemonic = Mnemonic::Eor,
            .addressing_mode = AddressingMode::IndirectY,
            .cycles = 5};
  // Inc
  case 0xE6:
    return {.name = "Inc",
            .mnemonic = Mnemonic::Inc,
            .addressing_mode = AddressingMode::ZeroPage,
            .cycles = 5};
  case 0xF6:
    return {.name = "Inc",
            .mnemonic = Mnemonic::Inc,
            .addressing_mode = AddressingMode::ZeroPageX,
            .cycles = 6};
  case 0xEE:
    return {.name = "Inc",
            .mnemonic = Mnemonic::Inc,
            .addressing_mode = AddressingMode::Absolute,
            .cycles = 6};
  case 0xFE:
    return {.name = "Inc",
            .mnemonic = Mnemonic::Inc,
            .addressing_mode = AddressingMode::AbsoluteX,
            .cycles = 7};
  // Inx
  case 0xE8:
    return {.name = "Inx",
            .mnemonic = Mnemonic::Inx,
            .addressing_mode = AddressingMode::Implied,
            .cycles = 2};
  // Iny
  case 0xC8:
    return {.name = "Iny",
            .mnemonic = Mnemonic::Iny,
            .addressing_mode = AddressingMode::Implied,
            .cycles = 2};
  // Jmp
  case 0x4C:
    return {.name = "Jmp",
            .mnemonic = Mnemonic::Jmp,
            .addressing_mode = AddressingMode::Absolute,
            .cycles = 3};
  case 0x6C:
    return {.name = "Jmp",
            .mnemonic = Mnemonic::Jmp,
            .addressing_mode = AddressingMode::Indirect,
            .cycles = 5};
  // Jsr
  case 0x20:
    return {.name = "Jsr",
            .mnemonic = Mnemonic::Jsr,
            .addressing_mode = AddressingMode::Absolute,
            .cycles = 6};
  // Lda
  case 0xA9:
    return {.name = "Lda",
            .mnemonic = Mnemonic::Lda,
            .addressing_mode = AddressingMode::Immediate,
            .cycles = 2};
  case 0xA5:
    return {.name = "Lda",
            .mnemonic = Mnemonic::Lda,
            .addressing_mode = AddressingMode::ZeroPage,
            .cycles = 3};
  case 0xB5:
    return {.name = "Lda",
            .mnemonic = Mnemonic::Lda,
            .addressing_mode = AddressingMode::ZeroPageX,
            .cycles = 4};
  case 0xAD:
    return {.name = "Lda",
            .mnemonic = Mnemonic::Lda,
            .addressing_mode = AddressingMode::Absolute,
            .cycles = 4};
  case 0xBD:
    return {.name = "Lda",
            .mnemonic = Mnemonic::Lda,
            .addressing_mode = AddressingMode::AbsoluteX,
            .cycles = 4};
  case 0xB9:
    return {.name = "Lda",
            .mnemonic = Mnemonic::Lda,
            .addressing_mode = AddressingMode::AbsoluteY,
            .cycles = 4};
  case 0xA1:
    return {.name = "Lda",
            .mnemonic = Mnemonic::Lda,
            .addressing_mode = AddressingMode::IndirectX,
            .cycles = 6};
  case 0xB1:
    return {.name = "Lda",
            .mnemonic = Mnemonic::Lda,
            .addressing_mode = AddressingMode::IndirectY,
            .cycles = 5};
  // Ldx
  case 0xA2:
    return {.name = "Ldx",
            .mnemonic = Mnemonic::Ldx,
            .addressing_mode = AddressingMode::Immediate,
            .cycles = 2};
  case 0xA6:
    return {.name = "Ldx",
            .mnemonic = Mnemonic::Ldx,
            .addressing_mode = AddressingMode::ZeroPage,
            .cycles = 3};
  case 0xB6:
    return {.name = "Ldx",
            .mnemonic = Mnemonic::Ldx,
            .addressing_mode = AddressingMode::ZeroPageY,
            .cycles = 4};
  case 0xAE:
    return {.name = "Ldx",
            .mnemonic = Mnemonic::Ldx,
            .addressing_mode = AddressingMode::Absolute,
            .cycles = 4};
  case 0xBE:
    return {.name = "Ldx",
            .mnemonic = Mnemonic::Ldx,
            .addressing_mode = AddressingMode::AbsoluteY,
            .cycles = 4};
  // Ldy
  case 0xA0:
    return {.name = "Ldy",
            .mnemonic = Mnemonic::Ldy,
            .addressing_mode = AddressingMode::Immediate,
            .cycles = 2};
  case 0xA4:
    return {.name = "Ldy",
            .mnemonic = Mnemonic::Ldy,
            .addressing_mode = AddressingMode::ZeroPage,
            .cycles = 3};
  case 0xB4:
    return {.name = "Ldy",
            .mnemonic = Mnemonic::Ldy,
            .addressing_mode = AddressingMode::ZeroPageX,
            .cycles = 4};
  case 0xAC:
    return {.name = "Ldy",
            .mnemonic = Mnemonic::Ldy,
            .addressing_mode = AddressingMode::Absolute,
            .cycles = 4};
  case 0xBC:
    return {.name = "Ldy",
            .mnemonic = Mnemonic::Ldy,
            .addressing_mode = AddressingMode::AbsoluteX,
            .cycles = 4};
  // Lsr
  case 0x4A:
    return {.name = "Lsr",
            .mnemonic = Mnemonic::Lsr,
            .addressing_mode = AddressingMode::Accumilate,
            .cycles = 2};
  case 0x46:
    return {.name = "Lsr",
            .mnemonic = Mnemonic::Lsr,
            .addressing_mode = AddressingMode::ZeroPage,
            .cycles = 5};
  case 0x56:
    return {.name = "Lsr",
            .mnemonic = Mnemonic::Lsr,
            .addressing_mode = AddressingMode::ZeroPageX,
            .cycles = 6};
  case 0x4E:
    return {.name = "Lsr",
            .mnemonic = Mnemonic::Lsr,
            .addressing_mode = AddressingMode::Absolute,
            .cycles = 6};
  case 0x5E:
    return {.name = "Lsr",
            .mnemonic = Mnemonic::Lsr,
            .addressing_mode = AddressingMode::AbsoluteX,
            .cycles = 7};
  // Ora
  case 0x09:
    return {.name = "Ora",
            .mnemonic = Mnemonic::Ora,
            .addressing_mode = AddressingMode::Immediate,
            .cycles = 2};
  case 0x05:
    return {.name = "Ora",
            .mnemonic = Mnemonic::Ora,
            .addressing_mode = AddressingMode::ZeroPage,
            .cycles = 3};
  case 0x15:
    return {.name = "Ora",
            .mnemonic = Mnemonic::Ora,
            .addressing_mode = AddressingMode::ZeroPageX,
            .cycles = 4};
  case 0x0D:
    return {.name = "Ora",
            .mnemonic = Mnemonic::Ora,
            .addressing_mode = AddressingMode::Absolute,
            .cycles = 4};
  case 0x1D:
    return {.name = "Ora",
            .mnemonic = Mnemonic::Ora,
            .addressing_mode = AddressingMode::AbsoluteX,
            .cycles = 4};
  case 0x19:
    return {.name = "Ora",
            .mnemonic = Mnemonic::Ora,
            .addressing_mode = AddressingMode::AbsoluteY,
            .cycles = 4};
  case 0x01:
    return {.name = "Ora",
            .mnemonic = Mnemonic::Ora,
            .addressing_mode = AddressingMode::IndirectX,
            .cycles = 6};
  case 0x11:
    return {.name = "Ora",
            .mnemonic = Mnemonic::Ora,
            .addressing_mode = AddressingMode::IndirectY,
            .cycles = 5};
  // Nop
  case 0xEA:
    return {.name = "Nop",
            .mnemonic = Mnemonic::Nop,
            .addressing_mode = AddressingMode::Implied,
            .cycles = 2};
  // Pha
  case 0x48:
    return {.name = "Pha",
            .mnemonic = Mnemonic::Pha,
            .addressing_mode = AddressingMode::Implied,
            .cycles = 3};
  // Php
  case 0x08:
    return {.name = "Php",
            .mnemonic = Mnemonic::Php,
            .addressing_mode = AddressingMode::Implied,
            .cycles = 3};
  // Pla
  case 0x68:
    return {.name = "Pla",
            .mnemonic = Mnemonic::Pla,
            .addressing_mode = AddressingMode::Implied,
            .cycles = 4};
  // Plp
  case 0x28:
    return {.name = "Plp",
            .mnemonic = Mnemonic::Plp,
            .addressing_mode = AddressingMode::Implied,
            .cycles = 4};
  // Rol
  case 0x2A:
    return {.name = "Rol",
            .mnemonic = Mnemonic::Rol,
            .addressing_mode = AddressingMode::Accumilate,
            .cycles = 2};
  case 0x26:
    return {.name = "Rol",
            .mnemonic = Mnemonic::Rol,
            .addressing_mode = AddressingMode::ZeroPage,
            .cycles = 5};
  case 0x36:
    return {.name = "Rol",
            .mnemonic = Mnemonic::Rol,
            .addressing_mode = AddressingMode::ZeroPageX,
            .cycles = 6};
  case 0x2E:
    return {.name = "Rol",
            .mnemonic = Mnemonic::Rol,
            .addressing_mode = AddressingMode::Absolute,
            .cycles = 6};
  case 0x3E:
    return {.name = "Rol",
            .mnemonic = Mnemonic::Rol,
            .addressing_mode = AddressingMode::AbsoluteX,
            .cycles = 7};
  // Ror
  case 0x6A:
    return {.name = "Ror",
            .mnemonic = Mnemonic::Ror,
            .addressing_mode = AddressingMode::Accumilate,
            .cycles = 2};
  case 0x66:
    return {.name = "Ror",
            .mnemonic = Mnemonic::Ror,
            .addressing_mode = AddressingMode::ZeroPage,
            .cycles = 5};
  case 0x76:
    return {.name = "Ror",
            .mnemonic = Mnemonic::Ror,
            .addressing_mode = AddressingMode::ZeroPageX,
            .cycles = 6};
  case 0x6E:
    return {.name = "Ror",
            .mnemonic = Mnemonic::Ror,
            .addressing_mode = AddressingMode::Absolute,
            .cycles = 6};
  case 0x7E:
    return {.name = "Ror",
            .mnemonic = Mnemonic::Ror,
            .addressing_mode = AddressingMode::AbsoluteX,
            .cycles = 7};
  // Rti
  case 0x40:
    return {.name = "Rti",
            .mnemonic = Mnemonic::Rti,
            .addressing_mode = AddressingMode::Implied,
            .cycles = 6};
  // Rts
  case 0x60:
    return {.name = "Rts",
            .mnemonic = Mnemonic::Rts,
            .addressing_mode = AddressingMode::Implied,
            .cycles = 6};
  // Sbc
  case 0xE9:
    return {.name = "Sbc",
            .mnemonic = Mnemonic::Sbc,
            .addressing_mode = AddressingMode::Immediate,
            .cycles = 2};
  case 0xE5:
    return {.name = "Sbc",
            .mnemonic = Mnemonic::Sbc,
            .addressing_mode = AddressingMode::ZeroPage,
            .cycles = 3};
  case 0xF5:
    return {.name = "Sbc",
            .mnemonic = Mnemonic::Sbc,
            .addressing_mode = AddressingMode::ZeroPageX,
            .cycles = 4};
  case 0xED:
    return {.name = "Sbc",
            .mnemonic = Mnemonic::Sbc,
            .addressing_mode = AddressingMode::Absolute,
            .cycles = 4};
  case 0xFD:
    return {.name = "Sbc",
            .mnemonic = Mnemonic::Sbc,
            .addressing_mode = AddressingMode::AbsoluteX,
            .cycles = 4};
  case 0xF9:
    return {.name = "Sbc",
            .mnemonic = Mnemonic::Sbc,
            .addressing_mode = AddressingMode::AbsoluteY,
            .cycles = 4};
  case 0xE1:
    return {.name = "Sbc",
            .mnemonic = Mnemonic::Sbc,
            .addressing_mode = AddressingMode::IndirectX,
            .cycles = 6};
  case 0xF1:
    return {.name = "Sbc",
            .mnemonic = Mnemonic::Sbc,
            .addressing_mode = AddressingMode::IndirectY,
            .cycles = 5};
  // Sec
  case 0x38:
    return {.name = "Sec",
            .mnemonic = Mnemonic::Sec,
            .addressing_mode = AddressingMode::Implied,
            .cycles = 2};
  // Sed
  case 0xF8:
    return {.name = "Sed",
            .mnemonic = Mnemonic::Sed,
            .addressing_mode = AddressingMode::Implied,
            .cycles = 2};
  // Sei
  case 0x78:
    return {.name = "Sei",
            .mnemonic = Mnemonic::Sei,
            .addressing_mode = AddressingMode::Implied,
            .cycles = 2};
  // Sta
  case 0x85:
    return {.name = "Sta",
            .mnemonic = Mnemonic::Sta,
            .addressing_mode = AddressingMode::ZeroPage,
            .cycles = 3};
  case 0x95:
    return {.name = "Sta",
            .mnemonic = Mnemonic::Sta,
            .addressing_mode = AddressingMode::ZeroPageX,
            .cycles = 4};
  case 0x8D:
    return {.name = "Sta",
            .mnemonic = Mnemonic::Sta,
            .addressing_mode = AddressingMode::Absolute,
            .cycles = 4};
  case 0x9D:
    return {.name = "Sta",
            .mnemonic = Mnemonic::Sta,
            .addressing_mode = AddressingMode::AbsoluteX,
            .cycles = 5};
  case 0x99:
    return {.name = "Sta",
            .mnemonic = Mnemonic::Sta,
            .addressing_mode = AddressingMode::AbsoluteY,
            .cycles = 5};
  case 0x81:
    return {.name = "Sta",
            .mnemonic = Mnemonic::Sta,
            .addressing_mode = AddressingMode::IndirectX,
            .cycles = 6};
  case 0x91:
    return {.name = "Sta",
            .mnemonic = Mnemonic::Sta,
            .addressing_mode = AddressingMode::IndirectY,
            .cycles = 6};
  // Stx
  case 0x86:
    return {.name = "Stx",
            .mnemonic = Mnemonic::Stx,
            .addressing_mode = AddressingMode::ZeroPage,
            .cycles = 3};
  case 0x96:
    return {.name = "Stx",
            .mnemonic = Mnemonic::Stx,
            .addressing_mode = AddressingMode::ZeroPageY,
            .cycles = 4};
  case 0x8E:
    return {.name = "Stx",
            .mnemonic = Mnemonic::Stx,
            .addressing_mode = AddressingMode::Absolute,
            .cycles = 4};
  // Sty
  case 0x84:
    return {.name = "Sty",
            .mnemonic = Mnemonic::Sty,
            .addressing_mode = AddressingMode::ZeroPage,
            .cycles = 3};
  case 0x94:
    return {.name = "Sty",
            .mnemonic = Mnemonic::Sty,
            .addressing_mode = AddressingMode::ZeroPageX,
            .cycles = 4};
  case 0x8C:
    return {.name = "Sty",
            .mnemonic = Mnemonic::Sty,
            .addressing_mode = AddressingMode::Absolute,
            .cycles = 4};
  // Tax
  case 0xAA:
    return {.name = "Tax",
            .mnemonic = Mnemonic::Tax,
            .addressing_mode = AddressingMode::Implied,
            .cycles = 2};
  // Tay
  case 0xA8:
    return {.name = "Tay",
            .mnemonic = Mnemonic::Tay,
            .addressing_mode = AddressingMode::Implied,
            .cycles = 2};
  // Tsx
  case 0xBA:
    return {.name = "Tsx",
            .mnemonic = Mnemonic::Tsx,
            .addressing_mode = AddressingMode::Implied,
            .cycles = 2};
  // Txa
  case 0x8A:
    return {.name = "Txa",
            .mnemonic = Mnemonic::Txa,
            .addressing_mode = AddressingMode::Implied,
            .cycles = 2};
  // Txs
  case 0x9A:
    return {.name = "Txs",
            .mnemonic = Mnemonic::Txs,
            .addressing_mode = AddressingMode::Implied,
            .cycles = 2};
  // Tya
  case 0x98:
    return {.name = "Tya",
            .mnemonic = Mnemonic::Tya,
            .addressing_mode = AddressingMode::Implied,
            .cycles = 2};
  // Illegal Instructions
  // Nop
  case 0x04:
    return {.name = "*Nop",
            .mnemonic = Mnemonic::Nop,
            .addressing_mode = AddressingMode::ZeroPage,
            .cycles = 3};
  case 0x0C:
    return {.name = "*Nop",
            .mnemonic = Mnemonic::Nop,
            .addressing_mode = AddressingMode::Absolute,
            .cycles = 4};
  case 0x14:
    return {.name = "*Nop",
            .mnemonic = Mnemonic::Nop,
            .addressing_mode = AddressingMode::ZeroPageX,
            .cycles = 4};
  case 0x1A:
    return {.name = "*Nop",
            .mnemonic = Mnemonic::Nop,
            .addressing_mode = AddressingMode::Implied,
            .cycles = 2};
  case 0x1C:
    return {.name = "*Nop",
            .mnemonic = Mnemonic::Nop,
            .addressing_mode = AddressingMode::AbsoluteX,
            .cycles = 4};
  case 0x34:
    return {.name = "*Nop",
            .mnemonic = Mnemonic::Nop,
            .addressing_mode = AddressingMode::ZeroPageX,
            .cycles = 4};
  case 0x3A:
    return {.name = "*Nop",
            .mnemonic = Mnemonic::Nop,
            .addressing_mode = AddressingMode::Implied,
            .cycles = 2};
  case 0x3C:
    return {.name = "*Nop",
            .mnemonic = Mnemonic::Nop,
            .addressing_mode = AddressingMode::AbsoluteX,
            .cycles = 4};
  case 0x44:
    return {.name = "*Nop",
            .mnemonic = Mnemonic::Nop,
            .addressing_mode = AddressingMode::ZeroPage,
            .cycles = 3};
  case 0x54:
    return {.name = "*Nop",
            .mnemonic = Mnemonic::Nop,
            .addressing_mode = AddressingMode::ZeroPageX,
            .cycles = 4};
  case 0x5A:
    return {.name = "*Nop",
            .mnemonic = Mnemonic::Nop,
            .addressing_mode = AddressingMode::Implied,
            .cycles = 2};
  case 0x5C:
    return {.name = "*Nop",
            .mnemonic = Mnemonic::Nop,
            .addressing_mode = AddressingMode::AbsoluteX,
            .cycles = 4};
  case 0x64:
    return {.name = "*Nop",
            .mnemonic = Mnemonic::Nop,
            .addressing_mode = AddressingMode::ZeroPage,
            .cycles = 3};
  case 0x74:
    return {.name = "*Nop",
            .mnemonic = Mnemonic::Nop,
            .addressing_mode = AddressingMode::ZeroPageX,
            .cycles = 4};
  case 0x7A:
    return {.name = "*Nop",
            .mnemonic = Mnemonic::Nop,
            .addressing_mode = AddressingMode::Implied,
            .cycles = 2};
  case 0x7C:
    return {.name = "*Nop",
            .mnemonic = Mnemonic::Nop,
            .addressing_mode = AddressingMode::AbsoluteX,
            .cycles = 4};
  case 0x80:
    return {.name = "*Nop",
            .mnemonic = Mnemonic::Nop,
            .addressing_mode = AddressingMode::Immediate,
            .cycles = 2};
  case 0x82:
    return {.name = "*Nop",
            .mnemonic = Mnemonic::Nop,
            .addressing_mode = AddressingMode::Immediate,
            .cycles = 2};
  case 0x89:
    return {.name = "*Nop",
            .mnemonic = Mnemonic::Nop,
            .addressing_mode = AddressingMode::Immediate,
            .cycles = 2};
  case 0xC2:
    return {.name = "*Nop",
            .mnemonic = Mnemonic::Nop,
            .addressing_mode = AddressingMode::Immediate,
            .cycles = 3};
  case 0xD4:
    return {.name = "*Nop",
            .mnemonic = Mnemonic::Nop,
            .addressing_mode = AddressingMode::ZeroPageX,
            .cycles = 4};
  case 0xDA:
    return {.name = "*Nop",
            .mnemonic = Mnemonic::Nop,
            .addressing_mode = AddressingMode::Implied,
            .cycles = 2};
  case 0xDC:
    return {.name = "*Nop",
            .mnemonic = Mnemonic::Nop,
            .addressing_mode = AddressingMode::AbsoluteX,
            .cycles = 4};
  case 0xE2:
    return {.name = "*Nop",
            .mnemonic = Mnemonic::Nop,
            .addressing_mode = AddressingMode::Immediate,
            .cycles = 2};
  case 0xF4:
    return {.name = "*Nop",
            .mnemonic = Mnemonic::Nop,
            .addressing_mode = AddressingMode::ZeroPageX,
            .cycles = 4};
  case 0xFA:
    return {.name = "*Nop",
            .mnemonic = Mnemonic::Nop,
            .addressing_mode = AddressingMode::Implied,
            .cycles = 2};
  case 0xFC:
    return {.name = "*Nop",
            .mnemonic = Mnemonic::Nop,
            .addressing_mode = AddressingMode::AbsoluteX,
            .cycles = 4};
  // Lax
  case 0xA3:
    return {.name = "*LAX",
            .mnemonic = Mnemonic::Lax,
            .addressing_mode = AddressingMode::IndirectX,
            .cycles = 6};
  case 0xA7:
    return {.name = "*LAX",
            .mnemonic = Mnemonic::Lax,
            .addressing_mode = AddressingMode::ZeroPage,
            .cycles = 3};
  case 0xAB:
    return {.name = "*LAX",
            .mnemonic = Mnemonic::Lax,
            .addressing_mode = AddressingMode::Immediate,
            .cycles = 2};
  case 0xAF:
    return {.name = "*LAX",
            .mnemonic = Mnemonic::Lax,
            .addressing_mode = AddressingMode::Absolute,
            .cycles = 4};
  case 0xB3:
    return {.name = "*LAX",
            .mnemonic = Mnemonic::Lax,
            .addressing_mode = AddressingMode::IndirectY,
            .cycles = 5};
  case 0xB7:
    return {.name = "*LAX",
            .mnemonic = Mnemonic::Lax,
            .addressing_mode = AddressingMode::ZeroPageY,
            .cycles = 4};
  case 0xBF:
    return {.name = "*LAX",
            .mnemonic = Mnemonic::Lax,
            .addressing_mode = AddressingMode::AbsoluteY,
            .cycles = 4};
  // Sax
  case 0x83:
    return {.name = "*Sax",
            .mnemonic = Mnemonic::Sax,
            .addressing_mode = AddressingMode::IndirectX,
            .cycles = 6};
  case 0x87:
    return {.name = "*Sax",
            .mnemonic = Mnemonic::Sax,
            .addressing_mode = AddressingMode::ZeroPage,
            .cycles = 3};
  case 0x8F:
    return {.name = "*Sax",
            .mnemonic = Mnemonic::Sax,
            .addressing_mode = AddressingMode::Absolute,
            .cycles = 4};
  case 0x97:
    return {.name = "*Sax",
            .mnemonic = Mnemonic::Sax,
            .addressing_mode = AddressingMode::ZeroPageY,
            .cycles = 4};
  // Sbc
  case 0xEB:
    return {.name = "*Sbc",
            .mnemonic = Mnemonic::Sbc,
            .addressing_mode = AddressingMode::Immediate,
            .cycles = 2};
  // DCP
  case 0xC3:
    return {.name = "*Dcp",
            .mnemonic = Mnemonic::Dcp,
            .addressing_mode = AddressingMode::IndirectX,
            .cycles = 8};
  case 0xC7:
    return {.name = "*Dcp",
            .mnemonic = Mnemonic::Dcp,
            .addressing_mode = AddressingMode::ZeroPage,
            .cycles = 5};
  case 0xCF:
    return {.name = "*Dcp",
            .mnemonic = Mnemonic::Dcp,
            .addressing_mode = AddressingMode::Absolute,
            .cycles = 6};
  case 0xD3:
    return {.name = "*Dcp",
            .mnemonic = Mnemonic::Dcp,
            .addressing_mode = AddressingMode::IndirectY,
            .cycles = 8};
  case 0xD7:
    return {.name = "*Dcp",
            .mnemonic = Mnemonic::Dcp,
            .addressing_mode = AddressingMode::ZeroPageX,
            .cycles = 6};
  case 0xDB:
    return {.name = "*Dcp",
            .mnemonic = Mnemonic::Dcp,
            .addressing_mode = AddressingMode::AbsoluteY,
            .cycles = 7};
  case 0xDF:
    return {.name = "*Dcp",
            .mnemonic = Mnemonic::Dcp,
            .addressing_mode = AddressingMode::AbsoluteX,
            .cycles = 7};
  // Isc
  case 0xE3:
    return {.name = "*ISB",
            .mnemonic = Mnemonic::Isb,
            .addressing_mode = AddressingMode::IndirectX,
            .cycles = 8};
  case 0xE7:
    return {.name = "*ISB",
            .mnemonic = Mnemonic::Isb,
            .addressing_mode = AddressingMode::ZeroPage,
            .cycles = 5};
  case 0xEF:
    return {.name = "*ISB",
            .mnemonic = Mnemonic::Isb,
            .addressing_mode = AddressingMode::Absolute,
            .cycles = 6};
  case 0xF3:
    return {.name = "*ISB",
            .mnemonic = Mnemonic::Isb,
            .addressing_mode = AddressingMode::IndirectY,
            .cycles = 8};
  case 0xF7:
    return {.name = "*ISB",
            .mnemonic = Mnemonic::Isb,
            .addressing_mode = AddressingMode::ZeroPageX,
            .cycles = 6};
  case 0xFB:
    return {.name = "*ISB",
            .mnemonic = Mnemonic::Isb,
            .addressing_mode = AddressingMode::AbsoluteY,
            .cycles = 7};
  case 0xFF:
    return {.name = "*ISB",
            .mnemonic = Mnemonic::Isb,
            .addressing_mode = AddressingMode::AbsoluteX,
            .cycles = 7};
  // Slo
  case 0x03:
    return {.name = "*Slo",
            .mnemonic = Mnemonic::Slo,
            .addressing_mode = AddressingMode::IndirectX,
            .cycles = 8};
  case 0x07:
    return {.name = "*Slo",
            .mnemonic = Mnemonic::Slo,
            .addressing_mode = AddressingMode::ZeroPage,
            .cycles = 5};
  case 0x0F:
    return {.name = "*Slo",
            .mnemonic = Mnemonic::Slo,
            .addressing_mode = AddressingMode::Absolute,
            .cycles = 6};
  case 0x13:
    return {.name = "*Slo",
            .mnemonic = Mnemonic::Slo,
            .addressing_mode = AddressingMode::IndirectY,
            .cycles = 8};
  case 0x17:
    return {.name = "*Slo",
            .mnemonic = Mnemonic::Slo,
            .addressing_mode = AddressingMode::ZeroPageX,
            .cycles = 6};
  case 0x1B:
    return {.name = "*Slo",
            .mnemonic = Mnemonic::Slo,
            .addressing_mode = AddressingMode::AbsoluteY,
            .cycles = 7};
  case 0x1F:
    return {.name = "*Slo",
            .mnemonic = Mnemonic::Slo,
            .addressing_mode = AddressingMode::AbsoluteX,
            .cycles = 7};
  // Rla
  case 0x23:
    return {.name = "*Rla",
            .mnemonic = Mnemonic::Rla,
            .addressing_mode = AddressingMode::IndirectX,
            .cycles = 8};
  case 0x27:
    return {.name = "*Rla",
            .mnemonic = Mnemonic::Rla,
            .addressing_mode = AddressingMode::ZeroPage,
            .cycles = 5};
  case 0x2F:
    return {.name = "*Rla",
            .mnemonic = Mnemonic::Rla,
            .addressing_mode = AddressingMode::Absolute,
            .cycles = 6};
  case 0x33:
    return {.name = "*Rla",
            .mnemonic = Mnemonic::Rla,
            .addressing_mode = AddressingMode::IndirectY,
            .cycles = 8};
  case 0x37:
    return {.name = "*Rla",
            .mnemonic = Mnemonic::Rla,
            .addressing_mode = AddressingMode::ZeroPageX,
            .cycles = 6};
  case 0x3B:
    return {.name = "*Rla",
            .mnemonic = Mnemonic::Rla,
            .addressing_mode = AddressingMode::AbsoluteY,
            .cycles = 7};
  case 0x3F:
    return {.name = "*Rla",
            .mnemonic = Mnemonic::Rla,
            .addressing_mode = AddressingMode::AbsoluteX,
            .cycles = 7};
  // Sre
  case 0x43:
    return {.name = "*Sre",
            .mnemonic = Mnemonic::Sre,
            .addressing_mode = AddressingMode::IndirectX,
            .cycles = 8};
  case 0x47:
    return {.name = "*Sre",
            .mnemonic = Mnemonic::Sre,
            .addressing_mode = AddressingMode::ZeroPage,
            .cycles = 5};
  case 0x4F:
    return {.name = "*Sre",
            .mnemonic = Mnemonic::Sre,
            .addressing_mode = AddressingMode::Absolute,
            .cycles = 6};
  case 0x53:
    return {.name = "*Sre",
            .mnemonic = Mnemonic::Sre,
            .addressing_mode = AddressingMode::IndirectY,
            .cycles = 8};
  case 0x57:
    return {.name = "*Sre",
            .mnemonic = Mnemonic::Sre,
            .addressing_mode = AddressingMode::ZeroPageX,
            .cycles = 6};
  case 0x5B:
    return {.name = "*Sre",
            .mnemonic = Mnemonic::Sre,
            .addressing_mode = AddressingMode::AbsoluteY,
            .cycles = 7};
  case 0x5F:
    return {.name = "*Sre",
            .mnemonic = Mnemonic::Sre,
            .addressing_mode = AddressingMode::AbsoluteX,
            .cycles = 7};
  // Rra
  case 0x63:
    return {.name = "*Rra",
            .mnemonic = Mnemonic::Rra,
            .addressing_mode = AddressingMode::IndirectX,
            .cycles = 8};
  case 0x67:
    return {.name = "*Rra",
            .mnemonic = Mnemonic::Rra,
            .addressing_mode = AddressingMode::ZeroPage,
            .cycles = 5};
  case 0x6F:
    return {.name = "*Rra",
            .mnemonic = Mnemonic::Rra,
            .addressing_mode = AddressingMode::Absolute,
            .cycles = 6};
  case 0x73:
    return {.name = "*Rra",
            .mnemonic = Mnemonic::Rra,
            .addressing_mode = AddressingMode::IndirectY,
            .cycles = 8};
  case 0x77:
    return {.name = "*Rra",
            .mnemonic = Mnemonic::Rra,
            .addressing_mode = AddressingMode::ZeroPageX,
            .cycles = 6};
  case 0x7B:
    return {.name = "*Rra",
            .mnemonic = Mnemonic::Rra,
            .addressing_mode = AddressingMode::AbsoluteY,
            .cycles = 7};
  case 0x7F:
    return {.name = "*Rra",
            .mnemonic = Mnemonic::Rra,
            .addressing_mode = AddressingMode::AbsoluteX,
            .cycles = 7};
  default:
    return {.name = "Nop",
            .mnemonic = Mnemonic::Nop,
            .addressing_mode = AddressingMode::Implied,
            .cycles = 2};
  };
}

} // namespace nes_emu
