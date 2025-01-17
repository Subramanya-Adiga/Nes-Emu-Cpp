#pragma once

namespace nes_emu {

enum class AddressingMode : uint8_t {
  Accumilate,
  Implied,
  Immediate,
  ZeroPage,
  ZeroPageX,
  ZeroPageY,
  Absolute,
  AbsoluteX,
  AbsoluteY,
  Indirect,
  IndirectX,
  IndirectY,
  Relative,
};

enum class Mnemonic : uint8_t {
  Nop,
  Adc,
  And,
  Asl,
  Bcc,
  Bcs,
  Beq,
  Bit,
  Bmi,
  Bne,
  Bpl,
  Brk,
  Bvc,
  Bvs,
  Clc,
  Cld,
  Cli,
  Clv,
  Cmp,
  Cpx,
  Cpy,
  Dec,
  Dex,
  Dey,
  Eor,
  Inc,
  Inx,
  Iny,
  Jmp,
  Jsr,
  Lda,
  Ldx,
  Ldy,
  Lsr,
  Ora,
  Pha,
  Php,
  Pla,
  Plp,
  Rol,
  Ror,
  Rti,
  Rts,
  Sbc,
  Sec,
  Sed,
  Sei,
  Sta,
  Stx,
  Sty,
  Tax,
  Tay,
  Tsx,
  Txa,
  Txs,
  Tya,
  // Illegal
  Lax,
  Sax,
  Dcp,
  Isb,
  Slo,
  Rla,
  Sre,
  Rra,
};

struct Instruction {
  const char *name;
  Mnemonic mnemonic;
  AddressingMode addressing_mode;
  uint8_t cycles;
};

[[nodiscard]] Instruction process_opcode(uint8_t opcode);

} // namespace nes_emu
