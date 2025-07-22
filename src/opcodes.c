#include "opcodes.h"
#include "gameboy.h"
#include <stdlib.h>

OpcodeEntry opcodeTable[256];

// mcycles is machine cycles. 1 mycycle = 4 timer cycles

void initOpcodeTable(Gameboy *gb) {

  opcodeTable[0x6] = (OpcodeEntry){
    .handler = LD_R_d8, .arg = &gb->B, .mcycles = 2, .mnemonic = "LD B,u8"};
  opcodeTable[0xE] = (OpcodeEntry){
    .handler = LD_R_d8, .arg = &gb->C, .mcycles = 2, .mnemonic = "LD C,u8"};
  opcodeTable[0x20] = (OpcodeEntry){.handler = JR_NZ,
    .mcyclesBranch = 3,
    .mcycles = 2,
    .mnemonic = "JR NZ,i8",
    .arg = NULL};
  opcodeTable[0x21] = (OpcodeEntry){.handler = LD_RR_d16,
    .arg = &gb->HL,
    .mcycles = 3,
    .mnemonic = "LD HL,u16"};
  opcodeTable[0x31] = (OpcodeEntry){.handler = LD_SP_d16,
    .arg = &gb->sp,
    .mcycles = 3,
    .mnemonic = "LD SP,d16"};
  opcodeTable[0x32] = (OpcodeEntry){.handler = LD_HLminus_A,
    .arg = &gb->A,
    .mcycles = 2,
    .mnemonic = "LD (HL-),A"};
  opcodeTable[0xAF] = (OpcodeEntry){
    .handler = XOR_R, .arg = &gb->A, .mcycles = 2, .mnemonic = "XOR A,A"};
}
