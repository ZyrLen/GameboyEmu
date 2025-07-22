#ifndef OPCODES_H
#define OPCODES_H

#include <stdint.h>

typedef struct Gameboy Gameboy;

typedef struct {
  void *arg;
  uint8_t mcycles;
  uint8_t mcyclesBranch;
  void (*handler)(Gameboy *gb, void *arg);
  const char *mnemonic;
  struct {
    uint8_t nbit;
    uint8_t *R;
    uint16_t *RR;
  } BitArgs;

} OpcodeEntry;

extern OpcodeEntry opcodeTable[256];

void initOpcodeTable();

#endif
