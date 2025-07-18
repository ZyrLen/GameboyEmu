#ifndef OPCODES_H
#define OPCODES_H

typedef struct Gameboy Gameboy;

typedef void (*OpcodeFunction)(Gameboy *gb);
extern OpcodeFunction opcodeTable[256];

void initOpcodeTable();

#endif
