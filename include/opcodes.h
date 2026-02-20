#ifndef OPCODES_H
#define OPCODES_H

#include <stdint.h>

typedef struct Gameboy Gameboy;

typedef struct {
  void *arg; // Used for single Register (R or RR) instructions, and SP value
  void (*handler)(Gameboy *gb, void *arg);
  const char *mnemonic;
  uint8_t mcycles;
  uint8_t mcyclesTrue;
  uint8_t *R1; // Used for Two Register instructions
  uint8_t *R2; // Used for Two Register instructions
  struct {
    uint8_t nbit;
  } BitArgs;
  uint16_t address;

} OpcodeEntry;

extern OpcodeEntry opcodeTable[256];

void initOpcodeTable(Gameboy *gb);

#endif
