#include "CBopcodes.h"
#include "gameboy.h"
#define OE OpcodeEntry

OpcodeEntry CBopcodeTable[256];

// mcycles is machine cycles. 1 mycycle = 4 timer cycles

void initCBOpcodeTable(Gameboy *gb) {
  CBopcodeTable[0x00] = (OE){.handler = RLC, .mcycles = 2, .arg = &gb->B,   .BitArgs = {.isHL = 0}, .mnemonic = "RLC B"};
  CBopcodeTable[0x01] = (OE){.handler = RLC, .mcycles = 2, .arg = &gb->C,   .BitArgs = {.isHL = 0}, .mnemonic = "RLC C"};
  CBopcodeTable[0x02] = (OE){.handler = RLC, .mcycles = 2, .arg = &gb->D,   .BitArgs = {.isHL = 0}, .mnemonic = "RLC D"};
  CBopcodeTable[0x03] = (OE){.handler = RLC, .mcycles = 2, .arg = &gb->E,   .BitArgs = {.isHL = 0}, .mnemonic = "RLC E"};
  CBopcodeTable[0x04] = (OE){.handler = RLC, .mcycles = 2, .arg = &gb->H,   .BitArgs = {.isHL = 0}, .mnemonic = "RLC H"};
  CBopcodeTable[0x05] = (OE){.handler = RLC, .mcycles = 2, .arg = &gb->L,   .BitArgs = {.isHL = 0}, .mnemonic = "RLC L"};
  CBopcodeTable[0x06] = (OE){.handler = RLC, .mcycles = 4, .arg = &gb->HL,  .BitArgs = {.isHL = 1}, .mnemonic = "RLC (HL)"};
  CBopcodeTable[0x07] = (OE){.handler = RLC, .mcycles = 2, .arg = &gb->A,   .BitArgs = {.isHL = 0}, .mnemonic = "RLC A"};
  CBopcodeTable[0x08] = (OE){.handler = RRC, .mcycles = 2, .arg = &gb->B,   .BitArgs = {.isHL = 0}, .mnemonic = "RRC B"};
  CBopcodeTable[0x09] = (OE){.handler = RRC, .mcycles = 2, .arg = &gb->C,   .BitArgs = {.isHL = 0}, .mnemonic = "RRC C"};
  CBopcodeTable[0x0A] = (OE){.handler = RRC, .mcycles = 2, .arg = &gb->D,   .BitArgs = {.isHL = 0}, .mnemonic = "RRC D"};
  CBopcodeTable[0x0B] = (OE){.handler = RRC, .mcycles = 2, .arg = &gb->E,   .BitArgs = {.isHL = 0}, .mnemonic = "RRC E"};
  CBopcodeTable[0x0C] = (OE){.handler = RRC, .mcycles = 2, .arg = &gb->H,   .BitArgs = {.isHL = 0}, .mnemonic = "RRC H"};
  CBopcodeTable[0x0D] = (OE){.handler = RRC, .mcycles = 2, .arg = &gb->L,   .BitArgs = {.isHL = 0}, .mnemonic = "RRC L"};
  CBopcodeTable[0x0E] = (OE){.handler = RRC, .mcycles = 4, .arg = &gb->HL,  .BitArgs = {.isHL = 1}, .mnemonic = "RRC (HL)"};
  CBopcodeTable[0x0F] = (OE){.handler = RRC, .mcycles = 2, .arg = &gb->A,   .BitArgs = {.isHL = 0}, .mnemonic = "RRC A"};

  CBopcodeTable[0x10] = (OE){.handler = RL_R, .mcycles = 2, .arg = &gb->B,  .BitArgs = {.isHL = 0}, .mnemonic = "RL B"};
  CBopcodeTable[0x11] = (OE){.handler = RL_R, .mcycles = 2, .arg = &gb->C,  .BitArgs = {.isHL = 0}, .mnemonic = "RL C"};
  CBopcodeTable[0x12] = (OE){.handler = RL_R, .mcycles = 2, .arg = &gb->D,  .BitArgs = {.isHL = 0}, .mnemonic = "RL D"};
  CBopcodeTable[0x13] = (OE){.handler = RL_R, .mcycles = 2, .arg = &gb->E,  .BitArgs = {.isHL = 0}, .mnemonic = "RL E"};
  CBopcodeTable[0x14] = (OE){.handler = RL_R, .mcycles = 2, .arg = &gb->H,  .BitArgs = {.isHL = 0}, .mnemonic = "RL H"};
  CBopcodeTable[0x15] = (OE){.handler = RL_R, .mcycles = 2, .arg = &gb->L,  .BitArgs = {.isHL = 0}, .mnemonic = "RL L"};
  CBopcodeTable[0x16] = (OE){.handler = RL_R, .mcycles = 4, .arg = &gb->HL, .BitArgs = {.isHL = 1}, .mnemonic = "RL (HL)"};
  CBopcodeTable[0x17] = (OE){.handler = RL_R, .mcycles = 2, .arg = &gb->A,  .BitArgs = {.isHL = 0}, .mnemonic = "RL A"};
  CBopcodeTable[0x18] = (OE){.handler = RR_R, .mcycles = 2, .arg = &gb->B,  .BitArgs = {.isHL = 0}, .mnemonic = "RR B"};
  CBopcodeTable[0x19] = (OE){.handler = RR_R, .mcycles = 2, .arg = &gb->C,  .BitArgs = {.isHL = 0}, .mnemonic = "RR C"};
  CBopcodeTable[0x1A] = (OE){.handler = RR_R, .mcycles = 2, .arg = &gb->D,  .BitArgs = {.isHL = 0}, .mnemonic = "RR D"};
  CBopcodeTable[0x1B] = (OE){.handler = RR_R, .mcycles = 2, .arg = &gb->E,  .BitArgs = {.isHL = 0}, .mnemonic = "RR E"};
  CBopcodeTable[0x1C] = (OE){.handler = RR_R, .mcycles = 2, .arg = &gb->H,  .BitArgs = {.isHL = 0}, .mnemonic = "RR H"};
  CBopcodeTable[0x1D] = (OE){.handler = RR_R, .mcycles = 2, .arg = &gb->L,  .BitArgs = {.isHL = 0}, .mnemonic = "RR L"};
  CBopcodeTable[0x1E] = (OE){.handler = RR_R, .mcycles = 4, .arg = &gb->HL, .BitArgs = {.isHL = 1}, .mnemonic = "RR (HL)"};
  CBopcodeTable[0x1F] = (OE){.handler = RR_R, .mcycles = 2, .arg = &gb->A,  .BitArgs = {.isHL = 0}, .mnemonic = "RR A"};

  
  CBopcodeTable[0x19] = (OpcodeEntry){
    .handler = RR_R, .arg = &gb->C, .mcycles = 2, .mnemonic = "RR C"};

  CBopcodeTable[0x38] = (OpcodeEntry){.handler = SRL_R, .arg = &gb->B,
    .mcycles = 2, .mnemonic = "SRL B"};

  CBopcodeTable[0x7C] = (OpcodeEntry){.handler = BIT_R,
    .BitArgs = {.nbit = 7},
    .arg = &gb->H,
    .mcycles = 2,
    .mnemonic = "BIT 7,H"};

  CBopcodeTable[0x40] = (OE){.handler = BIT_R, .arg = &gb->B,  .BitArgs = {.nbit = 0, .isHL = 0}};
  CBopcodeTable[0x41] = (OE){.handler = BIT_R, .arg = &gb->C,  .BitArgs = {.nbit = 0, .isHL = 0}};
  CBopcodeTable[0x42] = (OE){.handler = BIT_R, .arg = &gb->D,  .BitArgs = {.nbit = 0, .isHL = 0}};
  CBopcodeTable[0x43] = (OE){.handler = BIT_R, .arg = &gb->E,  .BitArgs = {.nbit = 0, .isHL = 0}};
  CBopcodeTable[0x44] = (OE){.handler = BIT_R, .arg = &gb->H,  .BitArgs = {.nbit = 0, .isHL = 0}};
  CBopcodeTable[0x45] = (OE){.handler = BIT_R, .arg = &gb->L,  .BitArgs = {.nbit = 0, .isHL = 0}};
  CBopcodeTable[0x46] = (OE){.handler = BIT_R, .arg = &gb->HL, .BitArgs = {.nbit = 0, .isHL = 1}};
  CBopcodeTable[0x47] = (OE){.handler = BIT_R, .arg = &gb->A,  .BitArgs = {.nbit = 0, .isHL = 0}};
  CBopcodeTable[0x48] = (OE){.handler = BIT_R, .arg = &gb->B,  .BitArgs = {.nbit = 1, .isHL = 0}};
  CBopcodeTable[0x49] = (OE){.handler = BIT_R, .arg = &gb->C,  .BitArgs = {.nbit = 1, .isHL = 0}};
  CBopcodeTable[0x4A] = (OE){.handler = BIT_R, .arg = &gb->D,  .BitArgs = {.nbit = 1, .isHL = 0}};
  CBopcodeTable[0x4B] = (OE){.handler = BIT_R, .arg = &gb->E,  .BitArgs = {.nbit = 1, .isHL = 0}};
  CBopcodeTable[0x4C] = (OE){.handler = BIT_R, .arg = &gb->H,  .BitArgs = {.nbit = 1, .isHL = 0}};
  CBopcodeTable[0x4D] = (OE){.handler = BIT_R, .arg = &gb->L,  .BitArgs = {.nbit = 1, .isHL = 0}};
  CBopcodeTable[0x4E] = (OE){.handler = BIT_R, .arg = &gb->HL, .BitArgs = {.nbit = 1, .isHL = 1}};
  CBopcodeTable[0x4F] = (OE){.handler = BIT_R, .arg = &gb->A,  .BitArgs = {.nbit = 1, .isHL = 0}};

  CBopcodeTable[0x50] = (OE){.handler = BIT_R, .arg = &gb->B,  .BitArgs = {.nbit = 2, .isHL = 0}};
  CBopcodeTable[0x51] = (OE){.handler = BIT_R, .arg = &gb->C,  .BitArgs = {.nbit = 2, .isHL = 0}};
  CBopcodeTable[0x52] = (OE){.handler = BIT_R, .arg = &gb->D,  .BitArgs = {.nbit = 2, .isHL = 0}};
  CBopcodeTable[0x53] = (OE){.handler = BIT_R, .arg = &gb->E,  .BitArgs = {.nbit = 2, .isHL = 0}};
  CBopcodeTable[0x54] = (OE){.handler = BIT_R, .arg = &gb->H,  .BitArgs = {.nbit = 2, .isHL = 0}};
  CBopcodeTable[0x55] = (OE){.handler = BIT_R, .arg = &gb->L,  .BitArgs = {.nbit = 2, .isHL = 0}};
  CBopcodeTable[0x56] = (OE){.handler = BIT_R, .arg = &gb->HL, .BitArgs = {.nbit = 2, .isHL = 1}};
  CBopcodeTable[0x57] = (OE){.handler = BIT_R, .arg = &gb->A,  .BitArgs = {.nbit = 2, .isHL = 0}};
  CBopcodeTable[0x58] = (OE){.handler = BIT_R, .arg = &gb->B,  .BitArgs = {.nbit = 3, .isHL = 0}};
  CBopcodeTable[0x59] = (OE){.handler = BIT_R, .arg = &gb->C,  .BitArgs = {.nbit = 3, .isHL = 0}};
  CBopcodeTable[0x5A] = (OE){.handler = BIT_R, .arg = &gb->D,  .BitArgs = {.nbit = 3, .isHL = 0}};
  CBopcodeTable[0x5B] = (OE){.handler = BIT_R, .arg = &gb->E,  .BitArgs = {.nbit = 3, .isHL = 0}};
  CBopcodeTable[0x5C] = (OE){.handler = BIT_R, .arg = &gb->H,  .BitArgs = {.nbit = 3, .isHL = 0}};
  CBopcodeTable[0x5D] = (OE){.handler = BIT_R, .arg = &gb->L,  .BitArgs = {.nbit = 3, .isHL = 0}};
  CBopcodeTable[0x5E] = (OE){.handler = BIT_R, .arg = &gb->HL, .BitArgs = {.nbit = 3, .isHL = 1}};
  CBopcodeTable[0x5F] = (OE){.handler = BIT_R, .arg = &gb->A,  .BitArgs = {.nbit = 3, .isHL = 0}};

  CBopcodeTable[0x60] = (OE){.handler = BIT_R, .arg = &gb->B,  .BitArgs = {.nbit = 4, .isHL = 0}};
  CBopcodeTable[0x61] = (OE){.handler = BIT_R, .arg = &gb->C,  .BitArgs = {.nbit = 4, .isHL = 0}};
  CBopcodeTable[0x62] = (OE){.handler = BIT_R, .arg = &gb->D,  .BitArgs = {.nbit = 4, .isHL = 0}};
  CBopcodeTable[0x63] = (OE){.handler = BIT_R, .arg = &gb->E,  .BitArgs = {.nbit = 4, .isHL = 0}};
  CBopcodeTable[0x64] = (OE){.handler = BIT_R, .arg = &gb->H,  .BitArgs = {.nbit = 4, .isHL = 0}};
  CBopcodeTable[0x65] = (OE){.handler = BIT_R, .arg = &gb->L,  .BitArgs = {.nbit = 4, .isHL = 0}};
  CBopcodeTable[0x66] = (OE){.handler = BIT_R, .arg = &gb->HL, .BitArgs = {.nbit = 4, .isHL = 1}};
  CBopcodeTable[0x67] = (OE){.handler = BIT_R, .arg = &gb->A,  .BitArgs = {.nbit = 4, .isHL = 0}};
  CBopcodeTable[0x68] = (OE){.handler = BIT_R, .arg = &gb->B,  .BitArgs = {.nbit = 5, .isHL = 0}};
  CBopcodeTable[0x69] = (OE){.handler = BIT_R, .arg = &gb->C,  .BitArgs = {.nbit = 5, .isHL = 0}};
  CBopcodeTable[0x6A] = (OE){.handler = BIT_R, .arg = &gb->D,  .BitArgs = {.nbit = 5, .isHL = 0}};
  CBopcodeTable[0x6B] = (OE){.handler = BIT_R, .arg = &gb->E,  .BitArgs = {.nbit = 5, .isHL = 0}};
  CBopcodeTable[0x6C] = (OE){.handler = BIT_R, .arg = &gb->H,  .BitArgs = {.nbit = 5, .isHL = 0}};
  CBopcodeTable[0x6D] = (OE){.handler = BIT_R, .arg = &gb->L,  .BitArgs = {.nbit = 5, .isHL = 0}};
  CBopcodeTable[0x6E] = (OE){.handler = BIT_R, .arg = &gb->HL, .BitArgs = {.nbit = 5, .isHL = 1}};
  CBopcodeTable[0x6F] = (OE){.handler = BIT_R, .arg = &gb->A,  .BitArgs = {.nbit = 5, .isHL = 0}};

  CBopcodeTable[0x70] = (OE){.handler = BIT_R, .arg = &gb->B,  .BitArgs = {.nbit = 6, .isHL = 0}};
  CBopcodeTable[0x71] = (OE){.handler = BIT_R, .arg = &gb->C,  .BitArgs = {.nbit = 6, .isHL = 0}};
  CBopcodeTable[0x72] = (OE){.handler = BIT_R, .arg = &gb->D,  .BitArgs = {.nbit = 6, .isHL = 0}};
  CBopcodeTable[0x73] = (OE){.handler = BIT_R, .arg = &gb->E,  .BitArgs = {.nbit = 6, .isHL = 0}};
  CBopcodeTable[0x74] = (OE){.handler = BIT_R, .arg = &gb->H,  .BitArgs = {.nbit = 6, .isHL = 0}};
  CBopcodeTable[0x75] = (OE){.handler = BIT_R, .arg = &gb->L,  .BitArgs = {.nbit = 6, .isHL = 0}};
  CBopcodeTable[0x76] = (OE){.handler = BIT_R, .arg = &gb->HL, .BitArgs = {.nbit = 6, .isHL = 1}};
  CBopcodeTable[0x77] = (OE){.handler = BIT_R, .arg = &gb->A,  .BitArgs = {.nbit = 6, .isHL = 0}};
  CBopcodeTable[0x78] = (OE){.handler = BIT_R, .arg = &gb->B,  .BitArgs = {.nbit = 7, .isHL = 0}};
  CBopcodeTable[0x79] = (OE){.handler = BIT_R, .arg = &gb->C,  .BitArgs = {.nbit = 7, .isHL = 0}};
  CBopcodeTable[0x7A] = (OE){.handler = BIT_R, .arg = &gb->D,  .BitArgs = {.nbit = 7, .isHL = 0}};
  CBopcodeTable[0x7B] = (OE){.handler = BIT_R, .arg = &gb->E,  .BitArgs = {.nbit = 7, .isHL = 0}};
  CBopcodeTable[0x7C] = (OE){.handler = BIT_R, .arg = &gb->H,  .BitArgs = {.nbit = 7, .isHL = 0}};
  CBopcodeTable[0x7D] = (OE){.handler = BIT_R, .arg = &gb->L,  .BitArgs = {.nbit = 7, .isHL = 0}};
  CBopcodeTable[0x7E] = (OE){.handler = BIT_R, .arg = &gb->HL, .BitArgs = {.nbit = 7, .isHL = 1}};
  CBopcodeTable[0x7F] = (OE){.handler = BIT_R, .arg = &gb->A,  .BitArgs = {.nbit = 7, .isHL = 0}};
}
