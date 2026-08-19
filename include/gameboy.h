#ifndef GAMEBOY_H
#define GAMEBOY_H

#include <stdint.h>

typedef uint8_t u8;
typedef int8_t s8;
typedef uint16_t u16;
typedef int16_t s16;
typedef uint32_t u32;

// 0x1000 is 4 KiB
#define MEM_SIZE (0x10000)
#define ROM_SIZE (0x4000)
#define VRAM_SIZE (0x2000)
#define WRAM_SIZE (0x2000)
#define ERAM_SIZE (0x2000)
#define EchoRAM_SIZE (0x1E00)
#define OAM_SIZE (0xA0)
// Unusable_SIZE is FEFF - FEA0 + 1
#define Unusable_SIZE (0x60)
#define io_SIZE (0x80)
#define HRAM_SIZE (0x80)
#define CartStartAddress 0x100
#define CartHeaderSize 0x50
#define IF io[0xF]

struct PPU {
  u32 framebuf[144][160];
  u8 tilemap[2][32*32];
};

typedef struct Gameboy {
  u16 pc;
  u16 sp;

  union {
    struct {
      u8 F, A;
    };
    u16 AF;
  };
  union {
    struct {
      u8 C, B;
    };
    u16 BC;
  };
  union {
    struct {
      u8 E, D;
    };
    u16 DE;
  };
  union {
    struct {
      u8 L, H;
    };
    u16 HL;
  };

  u8 state; // Tracks clock cycles
  u8 z;     // Zero flag
  u8 n;     // Subtraction Flag (BCD)
  u8 h;     // Half Carry Flag (BCD)
  u8 c;     // Carry Flag

  /* Memory Map (MMU) */
  int biosComplete;
  u8 bios[0x100];
  union {
    u8 mem[0x10000];
    struct {
      u8 ROM[2][ROM_SIZE];        // 0000 - 3FFF and 4000 - 7FFF
      u8 VRAM[VRAM_SIZE];         // 8000 - 9FFF
      u8 ERAM[ERAM_SIZE];         // A000 - BFFF
      u8 WRAM[WRAM_SIZE];         // C000 - DFFF split into two chunks.
      u8 EchoRAM[EchoRAM_SIZE];   // E000 - FDFF Echo of WRAM
      u8 OAM[OAM_SIZE];           // FE00 - FE9F Object attribute memory
      u8 Unusable[Unusable_SIZE]; // FEA0 - FEFF
      u8 io[io_SIZE];             // FF00 - FF7F
      u8 HRAM[HRAM_SIZE];         // FF80 - FFFE
      u8 IE;                      // FFFF
    };
  };

  u8 HALT_ON;
  u8 IME;
  struct PPU ppu;
} Gameboy;

// extern Gameboy gb;

/* General Opcode Functions */
/*
ADD (address)    DEC r              LD A,(address)    LD rr,u16     RET
BIT n,r          INC r              LD r,r            LD (HL+),A    RLA
CALL address     INC rr             LD r,u8           LD (HL-),A    RL r
CP u8            JR cond,address    LD r,(address)    POP rr        SUB r
CP (HL)          LD (address),A     LD (address),r    PUSH rr       XOR r
*/
// Once Cycle instructions, does not include HL as operands
// Right half of list for now

void ADD(Gameboy *gb, void *entryptr);
void ADD_HL(Gameboy *gb, void *entryptr);
void ADD_u8(Gameboy *gb, void *entryptr);
void ADC(Gameboy *gb, void *entryptr);
void ADC_HL(Gameboy *gb, void *entryptr);
void ADC_u8(Gameboy *gb, void *entryptr);
void SUB(Gameboy *gb, void *entryptr);
void SUB_HL(Gameboy *gb, void *entryptr);
void SUB_u8(Gameboy *gb, void *entryptr);
void AND(Gameboy *gb, void *entryptr);
void SBC(Gameboy *gb, void *entryptr);
void SBC_HL(Gameboy *gb, void *entryptr);
void SBC_u8(Gameboy *gb, void *entryptr);
void CP_i8(Gameboy *gb, void *entryptr);
void CP(Gameboy *gb, void *entryptr); // compare, SUB R but doesn't update A
void CALL_u16(Gameboy *gb, void *entryptr);
void CALL_NZ_u16(Gameboy *gb, void *entryptr);
void LD_SP_u16(Gameboy *gb, void *entryptr);
void LD_A_Addr(Gameboy *gb, void *entryptr);
void LD_R_R(Gameboy *gb, void *entryptr);
void LD_R_u8(Gameboy *gb, void *entryptr);
void LD_R_Addr(Gameboy *gb, void *entryptr);
void LD_R_AddrPlusu8(Gameboy *gb, void *entryptr);
void LD_Addr_R(Gameboy *gb, void *entryptr);
void LD_u16Addr_R(Gameboy *gb, void *entryptr);
void LD_u16Addr_RR(Gameboy *gb, void *entryptr);
void LD_AddrPlusu8_R(Gameboy *gb, void *entryptr);
void LD_RR_u16(Gameboy *gb, void *entryptr);
void LD_HLplus_A(Gameboy *gb, void *entryptr);
void LD_HLminus_A(Gameboy *gb, void *entryptr);
void LD_HL_u8(Gameboy *gb, void *entryptr);
void LD_A_HLplus(Gameboy *gb, void *entryptr);
void LD_A_HLminus(Gameboy *gb, void *entryptr);
void OR(Gameboy *gb, void *entryptr);
void POP(Gameboy *gb, void *entryptr);
void PUSH(Gameboy *gb, void *entryptr);
void RET(Gameboy *gb, void *entryptr);
void RLA(Gameboy *gb, void *entryptr);
void RL_R(Gameboy *gb, void *entryptr); // Rotate left

void NOP(Gameboy *gb, void *entryptr);

void XOR(Gameboy *gb, void *entryptr);
void XOR_u8(Gameboy *gb, void *entryptr);
void XOR_HL(Gameboy *gb, void *entryptr);
void JR_NZ(Gameboy *gb, void *entryptr);
void JR_NC(Gameboy *gb, void *entryptr);
void JR_Z(Gameboy *gb, void *entryptr);
void JR_C(Gameboy *gb, void *entryptr);
void JR(Gameboy *gb, void *entryptr);
void JP(Gameboy *gb, void *entryptr);
void JP_Z(Gameboy *gb, void *entryptr);
void JP_NZ(Gameboy *gb, void *entryptr);
void INC_R(Gameboy *gb, void *entryptr);
void INC_RR(Gameboy *gb, void *entryptr);
void INC_HL(Gameboy *gb, void *entryptr);
void DEC_R(Gameboy *gb, void *entryptr);
void DEC_RR(Gameboy *gb, void *entryptr);
void DEC_HL(Gameboy *gb, void *entryptr);

void HALT(Gameboy *gb, void *entryptr);

void DissableInterrupts(Gameboy *gb, void *entryptr);
void CCF(Gameboy *gb, void *entryptr);
void SCF(Gameboy *gb, void *entryptr);
void DAA(Gameboy *gb, void *entryptr);
void RRA(Gameboy *gb, void *entryptr);
void RST(Gameboy *gb, void *entryptr);
void RST_38h(Gameboy *gb, void *entryptr);

// CB:

void RRC(Gameboy *gb, void *entryptr);
void RLC(Gameboy *gb, void *entryptr);
void BIT_R(Gameboy *gb, void *entryptr);
void BIT_RR(Gameboy *gb, void *entryptr);
void SRL_R(Gameboy *gb, void *entryptr);
void RR_R(Gameboy *gb, void *entryptr);
void RL_R(Gameboy *gb, void *entryptr);
void RES_R(Gameboy *gb, void *entryptr);

#endif
