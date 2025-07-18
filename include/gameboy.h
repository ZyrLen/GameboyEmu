#ifndef GAMEBOY_H
#define GAMEBOY_H

#include <stdint.h>

#define KiB 1024 // 1024 bytes
// 0x1000 is 4 KiB
#define ROM_SIZE (0x4000)
#define VRAM_SIZE (0x2000)
#define WRAM_SIZE (0x2000)
#define ERAM_SIZE (0x2000)
#define EchoRAM_SIZE (0x1E00)
#define OAM_SIZE (0xA0)
#define Unusable_SIZE (0x80)
#define io_SIZE (0x80)
#define HRAM_SIZE (0x80)

#define CartHeaderStartAddress 0x100
#define CartHeaderSize 0x50

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

  uint8_t state;
} Gameboy;

/* Memory map */
typedef struct MMU {
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
  uint8_t *finished_bios;
} MMU;

/* General Opcode Functions */
// Once Cycle instructions, does not include HL as operands
void LD_Reg(uint8_t *x, uint8_t *y) { *x = *y; }
void NOP() {}
void ADD() {}
void SUB() {}
void XOR() {}

#endif
