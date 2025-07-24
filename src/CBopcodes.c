#include "CBopcodes.h"
#include "gameboy.h"

OpcodeEntry CBopcodeTable[256];

// mcycles is machine cycles. 1 mycycle = 4 timer cycles

void initCBOpcodeTable(Gameboy *gb) {
  CBopcodeTable[0x11] = (OpcodeEntry){
    .handler = RL_R, .BitArgs.R = &gb->C, .mcycles = 2, .mnemonic = "RL C"};

  CBopcodeTable[0x7C] = (OpcodeEntry){.handler = BIT_R,
    .BitArgs = {.nbit = 7, .R = &gb->H},
    .mcycles = 2,
    .mnemonic = "BIT 7,H"};
}
