#include "gameboy.h"
#include "opcodes.h"
#include <stdint.h>
#include <stdio.h>
#define UNUSED(x) (void)(x)
#define Z_BIT 7
#define N_BIT 6
#define H_BIT 5
#define C_BIT 4

/*
ADD (address)    DEC r              LD A,(address)    LD rr,u16     RET
BIT n,r          INC r              LD r,r            LD (HL+),A    RLA
CALL address     INC rr             LD r,u8           LD (HL-),A    RL r
CP u8            JR cond,address    LD r,(address)    POP rr        SUB r
CP (HL)          LD (address),A     LD (address),r    PUSH rr       XOR r
*/

static void SET_FLAGS(Gameboy *gb) {
  gb->F = gb->F & 0xF; // Clear flag bits
  // Flags are either 1 or zero, so the shift should be safe
  gb->F |= (gb->z << Z_BIT);
  gb->F |= (gb->n << N_BIT);
  gb->F |= (gb->h << H_BIT);
  gb->F |= (gb->c << C_BIT);
}

void AND(Gameboy *gb, void *entryptr) {
	UNUSED(entryptr);
  uint8_t n = gb->mem[gb->pc++];
  uint8_t value = gb->A & n;
  gb->A = value;
  gb->z = (!value);
  gb->n = 0;
  gb->h = 1;
  gb->c = 0;
  SET_FLAGS(gb);
}

void CP_i8(Gameboy *gb, void *entryptr) {
  UNUSED(entryptr);
  uint8_t A = gb->A;
  uint8_t n = gb->mem[gb->pc++];

  gb->z = (A == n);
  gb->n = 1;
  gb->h = ((A & 0xF) < (n & 0xF));
  gb->c = (A < n);
  SET_FLAGS(gb);
}

void CP(Gameboy *gb, void *entryptr) {
  OpcodeEntry *entry = (OpcodeEntry *)entryptr;
  uint8_t *R = entry->arg;
  uint8_t A = gb->A;

  gb->z = (A == *R);
  gb->n = 1;
  gb->h = ((A & 0xF) < (*R & 0xF));
  gb->c = (A < *R);
  SET_FLAGS(gb);
}

void CP_HL(Gameboy *gb, void *entryptr) {
	UNUSED(entryptr);
	UNUSED(gb);
	s8 Z;
	UNUSED(Z);
}

// Suspect
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

void CALL_NZ_u16(Gameboy *gb, void *entryptr) {
  UNUSED(entryptr);
  uint16_t nn = gb->mem[gb->pc] | (gb->mem[gb->pc + 1] << 8);
  gb->pc += 2;
  uint8_t pc_lsb = gb->pc & 0xFF;
  uint8_t pc_msb = (gb->pc >> 8) & 0xFF;
  if (!gb->z) {
    gb->sp--;
    gb->mem[gb->sp--] = pc_msb;
    gb->mem[gb->sp] = pc_lsb;
    gb->pc = nn;
  }
}

void LD_SP_u16(Gameboy *gb, void *entryptr) {
  OpcodeEntry *entry = (OpcodeEntry *)entryptr;
  uint16_t *SP = (uint16_t *)entry->arg;
  *SP = gb->mem[gb->pc] | (gb->mem[gb->pc + 1] << 8);
  gb->pc += 2;
}

void LD_A_Addr(Gameboy *gb, void *entryptr) {
  UNUSED(entryptr);
  uint8_t low = gb->mem[gb->pc++];
  uint8_t high = gb->mem[gb->pc++];
  uint16_t addr = ((uint8_t)high << 8) | low;
  gb->A = gb->mem[addr];
}

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
  uint16_t *addr = entry->address;
  *R = gb->mem[*addr];
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
  uint16_t *addr = entry->address;
  uint8_t *R = entry->arg;
  uint16_t actualAddress = (gb->mem[gb->pc-1] == 0xE2) ? 0xFF00 + *addr : *addr;
  gb->mem[actualAddress] = *R;
}

void LD_u16Addr_R(Gameboy *gb, void *entryptr) {
  OpcodeEntry *entry = (OpcodeEntry *)entryptr;
  uint16_t addr = gb->mem[gb->pc] | (gb->mem[gb->pc + 1] << 8);
  uint8_t *R = entry->arg;
  gb->mem[addr] = *R;
  gb->pc += 2;
}

void LD_u16Addr_RR(Gameboy *gb, void *entryptr) {
  OpcodeEntry *entry = (OpcodeEntry *)entryptr;
  uint16_t *RR = entry->arg;
  uint16_t addr = gb->mem[gb->pc] | (uint16_t)(gb->mem[gb->pc + 1] << 8);
  gb->pc += 2;
  u8 low_bit = *RR & 0xFF;
  u8 high_bit = *RR >> 8;
  // gb->mem[addr] = *RR;
  gb->mem[addr] = low_bit;
  gb->mem[addr+1] =high_bit ;
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

void LD_A_HLplus(Gameboy *gb, void *entryptr) {
	UNUSED(entryptr);
	gb->A = gb->mem[gb->HL++];
}

void LD_A_HLminus(Gameboy *gb, void *entryptr) {
	UNUSED(entryptr);
	gb->A = gb->mem[gb->HL--];
}

void OR(Gameboy *gb, void *entryptr) {
	UNUSED(gb);
	OpcodeEntry *entry = (OpcodeEntry *)entryptr;
  uint8_t *R = entry->arg;
  uint8_t result = gb->A | *R;
  gb->A = result;
  gb->z = (!result);
  gb->n = 0;
  gb->h = 0;
  gb->c = 0;
  SET_FLAGS(gb);
}

void PUSH(Gameboy *gb, void *entryptr) {
  OpcodeEntry *entry = (OpcodeEntry *)entryptr;
  uint16_t *RR = entry->arg;
  uint8_t RR_msb = (*RR >> 8) & 0xFF;
  uint8_t RR_lsb = *RR & 0xFF;
  gb->sp--;
  gb->mem[gb->sp--] = RR_msb;
  gb->mem[gb->sp] = RR_lsb;
}

void POP(Gameboy *gb, void *entryptr) {
  OpcodeEntry *entry = (OpcodeEntry *)entryptr;
  uint16_t *RR = entry->arg;
  uint8_t SP_low = gb->mem[gb->sp++];
  uint8_t SP_high = gb->mem[gb->sp++];
  *RR = (SP_high << 8) | SP_low;
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
  SET_FLAGS(gb);
}

void XOR(Gameboy *gb, void *entryptr) {
  OpcodeEntry *entry = (OpcodeEntry *)entryptr;
  uint8_t *R = (uint8_t *)entry->arg;
  uint8_t value = gb->A ^ *R;
  gb->A = value;
  gb->z = (!value);
  gb->n = 0;
  gb->h = 0;
  gb->c = 0;
  SET_FLAGS(gb);
}

void XOR_HL(Gameboy *gb, void *entryptr) {
  UNUSED(entryptr);
  uint16_t addr = gb->HL;
  uint8_t n = gb->mem[addr];
  uint8_t value = gb->A ^ n;
  gb->A = value;
  gb->z = (!value);
  gb->n = 0;
  gb->h = 0;
  gb->c = 0;
  SET_FLAGS(gb);
}

void NOP(Gameboy *gb, void *entryptr) {
  UNUSED(entryptr);
  UNUSED(gb);
}

void ADD(Gameboy *gb, void *entryptr) {
  OpcodeEntry *entry = (OpcodeEntry *)entryptr;
  uint8_t *R = entry->arg;
  uint8_t old = gb->A;
  uint8_t result = gb->A + *R;
  gb->A = result;
  gb->z = (!result);
  gb->n = 0;
  gb->h = ((old & 0xF) + (*R & 0xF) > 0xF);
  gb->c = ((uint16_t)(*R + old) > 0xFF);
  SET_FLAGS(gb);
}

void ADD_HL(Gameboy *gb, void *entryptr) {
	UNUSED(entryptr);
	UNUSED(gb);
}

void ADD_u8(Gameboy *gb, void *entryptr) {
  UNUSED(entryptr);
  uint8_t n = gb->mem[gb->pc++];
  uint8_t old = gb->A;
  uint8_t result = gb->A + n;
  gb->A = result;
  gb->z = (!result);
  gb->n = 0;
  gb->h = ((old & 0xF) + (n & 0xF) > 0xF);
  gb->c = ((uint16_t)(n+old) > 0xFF);
  SET_FLAGS(gb);
}

void ADC(Gameboy *gb, void *entryptr) {
	UNUSED(entryptr);
	UNUSED(gb);
}

void ADC_HL(Gameboy *gb, void *entryptr) {
	UNUSED(entryptr);
	UNUSED(gb);
}

void ADC_u8(Gameboy *gb, void *entryptr) {
	UNUSED(entryptr);
	UNUSED(gb);
}

void SUB(Gameboy *gb, void *entryptr) { // SUB A,R
  OpcodeEntry *entry = (OpcodeEntry *)entryptr;
  uint8_t *R = entry->arg;
  uint8_t value = *R;
  uint8_t A = gb->A;
  uint8_t result = A - value;

  gb->z = (result == 0);
  gb->n = 1;
  gb->h = ((A & 0xF) < (value & 0xF));
  gb->c = (A < value);
  SET_FLAGS(gb);
}

void SUB_HL(Gameboy *gb, void *entryptr) {
  UNUSED(entryptr);
  UNUSED(gb);
  // OpcodeEntry *entry = (OpcodeEntry *)entryptr;
}

void SUB_u8(Gameboy *gb, void *entryptr) {
  UNUSED(entryptr);
  uint8_t n = gb->mem[gb->pc++];
  uint8_t old = gb->A;
  uint8_t result = gb->A - n;
  gb->A = result;
  gb->z = (!result);
  gb->n = 1;
  gb->h = ((old & 0xF) < (n & 0xF));
  gb->c = (n > old);
  SET_FLAGS(gb);
}

void SBC(Gameboy *gb, void *entryptr) {
	UNUSED(entryptr);
	UNUSED(gb);
}

void SBC_HL(Gameboy *gb, void *entryptr) {
	UNUSED(entryptr);
	UNUSED(gb);
}

void SBC_u8(Gameboy *gb, void *entryptr) {
	UNUSED(entryptr);
	UNUSED(gb);
}

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
  if (gb->z) gb->pc += e;
}

void JR_NZ(Gameboy *gb, void *entryptr) {
  UNUSED(entryptr);
  int8_t e = gb->mem[gb->pc++];
  // e is negative if the msb = 1. (2's complement), so e is in [-128,+127]
  if (!gb->z) gb->pc += e;
}

void JR_C(Gameboy *gb, void *entryptr) {
  UNUSED(entryptr);
  int8_t e = gb->mem[gb->pc++];
  // e is negative if the msb = 1. (2's complement), so e is in [-128,+127]
  if (gb->c) gb->pc += e;
}

void JP(Gameboy *gb, void *entryptr) {
  UNUSED(entryptr);
  uint16_t addr = ((uint16_t)gb->mem[gb->pc + 1] << 8) | gb->mem[gb->pc];
  gb->pc = addr;
}

void JP_NZ(Gameboy *gb, void *entryptr) {
  UNUSED(entryptr);
  uint16_t addr = ((uint16_t)gb->mem[gb->pc + 1] << 8) | gb->mem[gb->pc];
  if (!gb->z)
    gb->pc = addr;
}

void JP_Z(Gameboy *gb, void *entryptr) {
  UNUSED(entryptr);
  uint16_t addr = ((uint16_t)gb->mem[gb->pc + 1] << 8) | gb->mem[gb->pc];
  if (gb->z)
    gb->pc = addr;
}

void INC_R(Gameboy *gb, void *entryptr) {
  OpcodeEntry *entry = (OpcodeEntry *)entryptr;
  uint8_t *R = entry->arg;
  u8 result = *R + 1;
  gb->z = (!result);
  gb->n = 0;
  gb->h = ((*R & 0xF) == 0xF); // if R = XXXX1111, it will half overflow
  *R = result;
  SET_FLAGS(gb);
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
  uint8_t result = (uint8_t)(value - 1);
  // 0bxxxx0000 Half carry only occurs if low nibble == 0
  gb->z = (result == 0);
  gb->n = 1;
  gb->h = ((value & 0x0F) == 0);
  *R = result;
  SET_FLAGS(gb);
}

void HALT(Gameboy *gb, void *entryptr) {
  UNUSED(entryptr);
  gb->HALT_ON = 1;
}

void DissableInterrupts(Gameboy *gb, void *entryptr) {
  UNUSED(entryptr);
  gb->IME = 0;
}

void CCF(Gameboy *gb, void *entryptr) { // Complement Carry Flag
  UNUSED(entryptr);
  gb->n = 0;
  gb->h = 0;
  gb->c ^= 1;
  SET_FLAGS(gb);
}

void RST(Gameboy *gb, void *entryptr) {
  OpcodeEntry *entry = (OpcodeEntry *)entryptr;
  gb->mem[gb->sp++] = gb->pc & 0xF;
  gb->mem[gb->sp++] = (gb->pc >> 8);
  gb->pc = *entry->address;
}

void RST_38h(Gameboy *gb, void *entryptr) {
  OpcodeEntry *entry = (OpcodeEntry *)entryptr;
  *entry->address = 0x38;
  RST(gb, entryptr);
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
  SET_FLAGS(gb);
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
  SET_FLAGS(gb);
}

#undef Z_BIT
#undef N_BIT
#undef H_BIT
#undef C_BIT
