#include "gameboy.h"
#include "opcodes.h"
#include <stdlib.h>
#define UNUSED(x) (void)(x)

/*
ADD (address)    DEC r              LD A,(address)    LD rr,d16     RET
BIT n,r          INC r              LD r,r            LD (HL+),A    RLA
CALL address     INC rr             LD r,d8           LD (HL-),A    RL r
CP d8            JR cond,address    LD r,(address)    POP rr        SUB r
CP (HL)          LD (address),A     LD (address),r    PUSH rr       XOR r
*/

void LD_SP_d16(Gameboy *gb, void *arg) {
  uint16_t *SP = (uint16_t *)arg;
  *SP = gb->mem[gb->pc] | (gb->mem[gb->pc + 1] << 8);
  gb->pc += 2;
}
void LD_A_Addr() {}
void LD_R_R(Gameboy *gb, uint8_t *R, uint8_t *R2) {
  UNUSED(gb);
  *R = *R2;
}
void LD_R_d8(Gameboy *gb, void *arg) {
  uint8_t *R = (uint8_t *)arg;
  *R = gb->mem[gb->pc++];
}
void LD_R_Addr() {}
void LD_Addr_R() {}
void LD_RR_d16(Gameboy *gb, void *arg) {
  uint16_t *RR = (uint16_t *)arg;
  *RR = gb->mem[gb->pc] | (gb->mem[gb->pc + 1] << 8);
  gb->pc += 2;
}
void LD_HLplus_A(Gameboy *gb, void *arg) {
  uint8_t *A = (uint8_t *)arg;
  gb->mem[gb->HL++] = *A;
}
void LD_HLminus_A(Gameboy *gb, void *arg) {
  uint8_t *A = (uint8_t *)arg;
  gb->mem[gb->HL--] = *A;
}
void POP_RR() {}
void PUSH_RR() {}
void RET() {}
void RLA() {}
void RL_R() {}
void SUB_R() {}
void XOR_R(Gameboy *gb, void *arg) {
  uint8_t *R = (uint8_t *)arg;
  *R ^= *R;
  gb->z = 1;
  gb->n = 0;
  gb->h = 0;
  gb->c = 0;
}
void NOP(Gameboy *gb) {
  UNUSED(gb);
  gb->pc++;
}
void ADD() {}
void SUB() {}
void XOR() {}

void JR_NZ() {}

// CB
void BIT_R(Gameboy *gb, void *entryptr) {
  OpcodeEntry *entry = (OpcodeEntry *)entryptr;
  uint8_t nbit = entry->BitArgs.nbit;
  uint8_t *R = entry->BitArgs.R;
  if (!((*R >> nbit) & 1)) { gb->z = 1; }
  gb->n = 0;
  gb->h = 1;
}
void BIT_RR() {}