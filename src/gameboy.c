#include "gameboy.h"
#include "opcodes.h"
#include <stdio.h>
#include <stdlib.h>
#define UNUSED(x) (void)(x)

/*
ADD (address)    DEC r              LD A,(address)    LD rr,u16     RET
BIT n,r          INC r              LD r,r            LD (HL+),A    RLA
CALL address     INC rr             LD r,u8           LD (HL-),A    RL r
CP u8            JR cond,address    LD r,(address)    POP rr        SUB r
CP (HL)          LD (address),A     LD (address),r    PUSH rr       XOR r
*/

void CP_i8(Gameboy *gb, void *entryptr) {
  UNUSED(entryptr);
  uint8_t A = gb->A;
  uint8_t n = gb->mem[gb->pc++];

  gb->z = (A == n);
  gb->n = 1;
  gb->h = ((A & 0xF) < (n & 0xF));
  gb->c = (A < n);
}

void CP_R(Gameboy *gb, void *entryptr) {
  OpcodeEntry *entry = (OpcodeEntry *)entryptr;
  uint8_t *R = entry->arg;
  uint8_t A = gb->A;

  gb->z = (A == *R);
  gb->n = 1;
  gb->h = ((A & 0xF) < (*R & 0xF));
  gb->c = (A < *R);
}

void CALL_u16(Gameboy *gb, void *entryptr) { // Pushes PC to stack, jumps to u16
  UNUSED(entryptr);
  uint16_t nn = gb->mem[gb->pc] | (gb->mem[gb->pc + 1] << 8);
  gb->pc += 2; // Puts the return address past the u16
  uint8_t pc_lsb = gb->pc & 0xFF;
  uint8_t pc_msb = (gb->pc >> 8) & 0xFF;
  gb->sp--;
  gb->mem[gb->sp--] = pc_msb;
  gb->mem[gb->sp] = pc_lsb;
  gb->pc = nn;
  // printf("u16 that is the arg of CALL: %04X\n", nn);
  // printf("PC = %04X\n", gb->pc);
}

void LD_SP_u16(Gameboy *gb, void *entryptr) {
  OpcodeEntry *entry = (OpcodeEntry *)entryptr;
  uint16_t *SP = (uint16_t *)entry->arg;
  *SP = gb->mem[gb->pc] | (gb->mem[gb->pc + 1] << 8);
  gb->pc += 2;
}

// void LD_A_Addr(Gameboy *gb, void *entryptr) {}

void LD_R_R(Gameboy *gb, void *entryptr) {
  UNUSED(gb);
  OpcodeEntry *entry = (OpcodeEntry *)entryptr;
  *entry->R1 = *entry->R2;
}

void LD_R_u8(Gameboy *gb, void *entryptr) {
  OpcodeEntry *entry = (OpcodeEntry *)entryptr;
  uint8_t *R = (uint8_t *)entry->arg;
  *R = gb->mem[gb->pc++];
}

void LD_R_Addr(Gameboy *gb, void *entryptr) {
  OpcodeEntry *entry = (OpcodeEntry *)entryptr;
  uint8_t *R = entry->arg;
  uint16_t addr = entry->address;
  *R = gb->mem[addr];
}

void LD_R_AddrPlusu8(Gameboy *gb, void *entryptr) {
  OpcodeEntry *entry = (OpcodeEntry *)entryptr;
  uint8_t *R = entry->arg;
  uint16_t addr = 0xFF00 + gb->mem[gb->pc++];
  // DEBUG
  gb->mem[0xFF44] = 0x90;

  *R = gb->mem[addr];
}

void LD_Addr_R(Gameboy *gb, void *entryptr) {
  OpcodeEntry *entry = (OpcodeEntry *)entryptr;
  uint16_t addr = entry->address;
  uint8_t *R = entry->arg;
  gb->mem[addr] = *R;
}

void LD_u16Addr_R(Gameboy *gb, void *entryptr) {
  OpcodeEntry *entry = (OpcodeEntry *)entryptr;
  uint16_t addr = gb->mem[gb->pc] | (uint16_t)(gb->mem[gb->pc + 1] << 8);
  uint8_t *R = entry->arg;
  gb->mem[addr] = *R;
  gb->pc += 2;
}

void LD_AddrPlusu8_R(Gameboy *gb, void *entryptr) {
  OpcodeEntry *entry = (OpcodeEntry *)entryptr;
  uint16_t addr = 0xFF00 + gb->mem[gb->pc++];
  uint8_t *R = entry->arg;
  gb->mem[addr] = *R;
}

void LD_RR_u16(Gameboy *gb, void *entryptr) {
  OpcodeEntry *entry = (OpcodeEntry *)entryptr;
  uint16_t *RR = (uint16_t *)entry->arg;
  *RR = gb->mem[gb->pc] | (gb->mem[gb->pc + 1] << 8);
  gb->pc += 2;
}

void LD_HLplus_A(Gameboy *gb, void *entryptr) {
  UNUSED(entryptr);
  gb->mem[gb->HL++] = gb->A;
}

void LD_HLminus_A(Gameboy *gb, void *entryptr) {
  UNUSED(entryptr);
  gb->mem[gb->HL--] = gb->A;
}

void PUSH_RR(Gameboy *gb, void *entryptr) {
  OpcodeEntry *entry = (OpcodeEntry *)entryptr;
  uint16_t *RR = entry->arg;
  uint8_t RR_msb = (*RR >> 8) & 0xFF;
  uint8_t RR_lsb = *RR & 0xFF;
  gb->sp--;
  gb->mem[gb->sp--] = RR_msb;
  gb->mem[gb->sp] = RR_lsb;
}

void POP_RR(Gameboy *gb, void *entryptr) {
  OpcodeEntry *entry = (OpcodeEntry *)entryptr;
  uint16_t *RR = entry->arg;
  uint8_t SP_lsb = gb->mem[gb->sp++];
  uint8_t SP_msb = gb->mem[gb->sp++];
  *RR = ((uint16_t)SP_msb << 8) | SP_lsb;
}

void RET(Gameboy *gb, void *entryptr) {
  UNUSED(entryptr);
  uint8_t lsb = gb->mem[gb->sp++];
  uint8_t msb = gb->mem[gb->sp++];
  gb->pc = ((uint16_t)msb << 8) | lsb;
}

void RLA(Gameboy *gb, void *entryptr) {
  UNUSED(entryptr);
  uint8_t *R = &gb->A;
  uint8_t newCarryBit = (*R >> 7) & 1;
  uint8_t new0thBit = gb->c;
  *R = *R << 1;
  // 0th bit is now 0
  *R += new0thBit;

  gb->z = 0; // Different from RL_R
  gb->n = 0;
  gb->h = 0;
  gb->c = newCarryBit;
}

// void SUB_R(Gameboy *gb, void *entryptr) {}

void XOR_R(Gameboy *gb, void *entryptr) {
  OpcodeEntry *entry = (OpcodeEntry *)entryptr;
  uint8_t *R = (uint8_t *)entry->arg;
  *R ^= *R;
  gb->z = 1;
  gb->n = 0;
  gb->h = 0;
  gb->c = 0;
}

void NOP(Gameboy *gb, void *entryptr) {
  UNUSED(entryptr);
  UNUSED(gb);
}

// void ADD(Gameboy *gb, void *entryptr) {}

void SUB_R(Gameboy *gb, void *entryptr) { // SUB A,R
  OpcodeEntry *entry = (OpcodeEntry *)entryptr;
  uint8_t *R = entry->arg;
  uint8_t value = *R;
  uint8_t A = gb->A;
  uint8_t result = A - value;

  gb->z = (result == 0);
  gb->n = 1;
  gb->h = ((A & 0xF) < (value & 0xF));
  gb->c = (A < value);
}

/*
void SUB_R_HL(Gameboy *gb, void *entryptr) {
  OpcodeEntry *entry = (OpcodeEntry *)entryptr;
}

void SUB_R_u8(Gameboy *gb, void *entryptr) {
  OpcodeEntry *entry = (OpcodeEntry *)entryptr;
}
*/

// void XOR(Gameboy *gb, void *entryptr) {}

void JR(Gameboy *gb, void *entryptr) {
  UNUSED(entryptr);
  int8_t e = gb->mem[gb->pc++];
  // e is negative if the msb = 1. (2's complement), so e is in [-128,+127]
  gb->pc += e;
}

void JR_Z(Gameboy *gb, void *entryptr) {
  UNUSED(entryptr);
  int8_t e = gb->mem[gb->pc++];
  // e is negative if the msb = 1. (2's complement), so e is in [-128,+127]
  if (gb->z == 1) gb->pc += e;
}

void JR_NZ(Gameboy *gb, void *entryptr) {
  UNUSED(entryptr);
  int8_t e = gb->mem[gb->pc++];
  // e is negative if the msb = 1. (2's complement), so e is in [-128,+127]
  if (gb->z == 0) gb->pc += e;
}

void JP(Gameboy *gb, void *entryptr) {
  UNUSED(entryptr);
  uint16_t addr = ((uint16_t)gb->mem[gb->pc + 1] << 8) | gb->mem[gb->pc];
  gb->pc = addr;
}

void INC_R(Gameboy *gb, void *entryptr) {
  OpcodeEntry *entry = (OpcodeEntry *)entryptr;
  uint8_t *R = entry->arg;
  if (*R + 1 == 0) {
    gb->z = 1;
  } else {
    gb->z = 0;
  }
  gb->n = 0;
  uint8_t half_overflow = ((*R * 0xF) + 1 > 0xF);
  if (half_overflow) {
    gb->h = 1;
  } else {
    gb->h = 0;
  }
  (*R)++;
}

void INC_RR(Gameboy *gb, void *entryptr) {
  UNUSED(gb);
  OpcodeEntry *entry = (OpcodeEntry *)entryptr;
  uint16_t *RR = entry->arg;
  (*RR)++;
}

void DEC_R(Gameboy *gb, void *entryptr) {
  OpcodeEntry *entry = (OpcodeEntry *)entryptr;
  uint8_t *R = entry->arg;
  uint8_t value = *R;
  uint8_t result = value - 1;
  // 0bxxxx0000 Half carry only occurs if low nibble == 0
  gb->z = (result == 0);
  gb->n = 1;
  gb->h = ((value & 0x0F) == 0);
  *R = result;
}

void HALT(Gameboy *gb, void *entryptr) {
  UNUSED(entryptr);
  gb->HALT_ON = 1;
}

// CB
void BIT_R(Gameboy *gb, void *entryptr) {
  OpcodeEntry *entry = (OpcodeEntry *)entryptr;
  uint8_t nbit = entry->BitArgs.nbit;
  uint8_t *R = entry->arg;

  uint8_t bit_set = ((*R >> nbit) & 1);
  if (!bit_set) {
    gb->z = 1;
  } else {
    gb->z = 0;
  }
  gb->n = 0;
  gb->h = 1;
}

// void BIT_RR(Gameboy *gb, void *entryptr) {}

void RL_R(Gameboy *gb, void *entryptr) {
  OpcodeEntry *entry = (OpcodeEntry *)entryptr;
  uint8_t *R = entry->arg;
  uint8_t newCarryBit = (*R >> 7) & 1;
  uint8_t new0thBit = gb->c;
  *R = *R << 1;
  // 0th bit is now 0
  *R += new0thBit;

  gb->z = (*R == 0);
  gb->n = 0;
  gb->h = 0;
  gb->c = newCarryBit;
}