#include "opcodes.h"
#include "gameboy.h"

OpcodeFunction opcodeTable[256];

void initOpcodeTable() { opcodeTable[0] = NOP; }
