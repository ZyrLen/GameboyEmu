#ifndef GAMEBOY_H
#define GAMEBOY_H

#include <stdint.h>

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

typedef struct Gameboy {
  uint16_t pc;
  uint16_t sp;

  union {
    struct {
      uint8_t F, A;
    };
    uint16_t AF;
  };
  union {
    struct {
      uint8_t C, B;
    };
    uint16_t BC;
  };
  union {
    struct {
      uint8_t E, D;
    };
    uint16_t DE;
  };
  union {
    struct {
      uint8_t L, H;
    };
    uint16_t HL;
  };

  uint8_t state; // Tracks clock cycles
  uint8_t z;     // Zero flag
  uint8_t n;     // Subtraction Flag (BCD)
  uint8_t h;     // Half Carry Flag (BCD)
  uint8_t c;     // Carry Flag

  /* Memory Map (MMU) */
  int biosComplete;
  uint8_t bios[0x100];
  union {
    uint8_t mem[0x10000];
    struct {
      uint8_t ROM[2][ROM_SIZE];        // 0000 - 3FFF and 4000 - 7FFF
      uint8_t VRAM[VRAM_SIZE];         // 8000 - 9FFF
      uint8_t ERAM[ERAM_SIZE];         // A000 - BFFF
      uint8_t WRAM[WRAM_SIZE];         // C000 - DFFF split into two chunks.
      uint8_t EchoRAM[EchoRAM_SIZE];   // E000 - FDFF Echo of WRAM
      uint8_t OAM[OAM_SIZE];           // FE00 - FE9F Object attribute memory
      uint8_t Unusable[Unusable_SIZE]; // FEA0 - FEFF
      uint8_t io[io_SIZE];             // FF00 - FF7F
      uint8_t HRAM[HRAM_SIZE];         // FF80 - FFFE
      uint8_t IE;                      // FFFF
    };
  };

  uint8_t HALT_ON;
  uint8_t IME;
} Gameboy;

extern Gameboy gb;

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

void CP_i8(Gameboy *gb, void *entryptr);
void CP_R(Gameboy *gb, void *entryptr); // compare, SUB R but doesn't update A
void CALL_u16(Gameboy *gb, void *entryptr);
void LD_SP_u16(Gameboy *gb, void *entryptr);
void LD_A_Addr(Gameboy *gb, void *entryptr);
void LD_R_R(Gameboy *gb, void *entryptr);
void LD_R_u8(Gameboy *gb, void *entryptr);
void LD_R_Addr(Gameboy *gb, void *entryptr);
void LD_R_AddrPlusu8(Gameboy *gb, void *entryptr);
void LD_Addr_R(Gameboy *gb, void *entryptr);
void LD_u16Addr_R(Gameboy *gb, void *entryptr);
void LD_AddrPlusu8_R(Gameboy *gb, void *entryptr);
void LD_RR_u16(Gameboy *gb, void *entryptr);
void LD_HLplus_A(Gameboy *gb, void *entryptr);
void LD_HLminus_A(Gameboy *gb, void *entryptr);
void POP_RR(Gameboy *gb, void *entryptr);
void PUSH_RR(Gameboy *gb, void *entryptr);
void RET(Gameboy *gb, void *entryptr);
void RLA(Gameboy *gb, void *entryptr);
void RL_R(Gameboy *gb, void *entryptr); // Rotate left

void SUB_R(Gameboy *gb, void *entryptr);
void SUB_R_HL(Gameboy *gb, void *entryptr);
void SUB_R_u8(Gameboy *gb, void *entryptr);

void XOR_R(Gameboy *gb, void *entryptr);
void NOP(Gameboy *gb, void *entryptr);

void XOR(Gameboy *gb, void *entryptr);
void JR_NZ(Gameboy *gb, void *entryptr);
void JR_NC(Gameboy *gb, void *entryptr);
void JR_Z(Gameboy *gb, void *entryptr);
void JR_C(Gameboy *gb, void *entryptr);
void JR(Gameboy *gb, void *entryptr);
void JP(Gameboy *gb, void *entryptr);
void INC_R(Gameboy *gb, void *entryptr);
void INC_RR(Gameboy *gb, void *entryptr);
void DEC_R(Gameboy *gb, void *entryptr);

void HALT(Gameboy *gb, void *entryptr);

// CB:

void BIT_R(Gameboy *gb, void *entryptr);
void BIT_RR(Gameboy *gb, void *entryptr);

#endif
