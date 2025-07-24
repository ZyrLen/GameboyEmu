#ifndef OPCODES_H
#define OPCODES_H

#include <stdint.h>

typedef struct Gameboy Gameboy;

typedef struct {
  void *arg;
  void (*handler)(Gameboy *gb, void *arg);
  const char *mnemonic;
  uint8_t mcycles;
  uint8_t mcyclesTrue;
  uint8_t *R1;
  uint8_t *R2;
  struct {
    uint8_t nbit;
    uint8_t *R;
    uint16_t *RR;
    uint16_t address;
  } BitArgs;

} OpcodeEntry;

extern OpcodeEntry opcodeTable[256];

void initOpcodeTable();

#endif
