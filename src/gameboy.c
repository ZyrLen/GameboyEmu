#include "gameboy.h"

/*
ADD (address)    DEC r              LD A,(address)    LD rr,d16     RET
BIT n,r          INC r              LD r,r            LD (HL+),A    RLA
CALL address     INC rr             LD r,d8           LD (HL-),A    RL r
CP d8            JR cond,address    LD r,(address)    POP rr        SUB r
CP (HL)          LD (address),A     LD (address),r    PUSH rr       XOR r
*/

void LD_A_Addr() {}
void LD_R_R(uint8_t *x, uint8_t *y) { *x = *y; }
void LD_R_d8() {}
void LD_R_Addr() {}
void LD_Addr_R() {}
void LD_RR_d16() {}
void LD_HLplus_A() {}
void LD_HLminus_A() {}
void POP_RR() {}
void PUSH_RR() {}
void RET() {}
void RLA() {}
void RL_R() {}
void SUB_R() {}
void XOR_R() {}
void NOP(Gameboy *gb) { gb->pc++; }
void ADD() {}
void SUB() {}
void XOR() {}