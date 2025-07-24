#include "opcodes.h"
#include "gameboy.h"
#include <stdlib.h>

OpcodeEntry opcodeTable[256];

// mcycles is machine cycles. 1 mycycle = 4 timer cycles
// Note: machine cycles listed below have some errors for now, will come back to
//       it after more of the opcodes are done

void initOpcodeTable(Gameboy *gb) {
  opcodeTable[0x0] =
    (OpcodeEntry){.handler = NOP, .mcycles = 1, .mnemonic = "NOP"};

  opcodeTable[0x4] = (OpcodeEntry){
    .handler = INC_R, .BitArgs.R = &gb->B, .mcycles = 1, .mnemonic = "INC B"};

  opcodeTable[0x5] = (OpcodeEntry){
    .handler = DEC_R, .BitArgs.R = &gb->B, .mcycles = 1, .mnemonic = "DEC B"};

  opcodeTable[0x6] = (OpcodeEntry){
    .handler = LD_R_u8, .arg = &gb->B, .mcycles = 2, .mnemonic = "LD B,u8"};

  opcodeTable[0xC] = (OpcodeEntry){
    .handler = INC_R, .BitArgs.R = &gb->C, .mcycles = 1, .mnemonic = "INC C"};

  opcodeTable[0xD] = (OpcodeEntry){
    .handler = DEC_R, .BitArgs.R = &gb->D, .mcycles = 1, .mnemonic = "DEC C"};

  opcodeTable[0xE] = (OpcodeEntry){
    .handler = LD_R_u8, .arg = &gb->C, .mcycles = 2, .mnemonic = "LD C,u8"};

  opcodeTable[0x11] = (OpcodeEntry){.handler = LD_RR_u16,
    .arg = &gb->DE,
    .mcycles = 3,
    .mnemonic = "LD DE,u16"};

  opcodeTable[0x13] = (OpcodeEntry){.handler = INC_RR,
    .BitArgs.RR = &gb->DE,
    .mcycles = 2,
    .mnemonic = "INC DE"};

  opcodeTable[0x17] =
    (OpcodeEntry){.handler = RLA, .mcycles = 1, .mnemonic = "RLA"};

  opcodeTable[0x1A] = (OpcodeEntry){.handler = LD_R_Addr,
    .BitArgs = {.R = &gb->A, .address = gb->DE},
    .mcycles = 2,
    .mnemonic = "LD A,(DE)"};

  opcodeTable[0x1E] = (OpcodeEntry){
    .handler = LD_R_u8, .arg = &gb->E, .mcycles = 2, .mnemonic = "LD E,u8"};

  opcodeTable[0x20] = (OpcodeEntry){.handler = JR_NZ,
    .mcyclesTrue = 3,
    .mcycles = 2,
    .mnemonic = "JR NZ,i8",
    .arg = NULL};

  opcodeTable[0x21] = (OpcodeEntry){.handler = LD_RR_u16,
    .arg = &gb->HL,
    .mcycles = 3,
    .mnemonic = "LD HL,u16"};

  opcodeTable[0x22] = (OpcodeEntry){
    .handler = LD_HLplus_A, .mcycles = 2, .mnemonic = "LD (HL+),A"};

  opcodeTable[0x23] = (OpcodeEntry){.handler = INC_RR,
    .BitArgs.RR = &gb->HL,
    .mcycles = 2,
    .mnemonic = "INC HL"};

  opcodeTable[0x28] = (OpcodeEntry){
    .handler = JR_Z, .mcycles = 2, .mcyclesTrue = 3, .mnemonic = "JR Z,i8"};

  opcodeTable[0x31] = (OpcodeEntry){.handler = LD_SP_u16,
    .arg = &gb->sp,
    .mcycles = 3,
    .mnemonic = "LD SP,u16"};

  opcodeTable[0x32] = (OpcodeEntry){
    .handler = LD_HLminus_A, .mcycles = 2, .mnemonic = "LD (HL-),A"};

  opcodeTable[0x3D] = (OpcodeEntry){
    .handler = DEC_R, .BitArgs.R = &gb->A, .mcycles = 1, .mnemonic = "DEC A"};

  opcodeTable[0x3E] = (OpcodeEntry){
    .handler = LD_R_u8, .arg = &gb->A, .mcycles = 2, .mnemonic = "LD A,u8"};

  opcodeTable[0x4F] = (OpcodeEntry){.handler = LD_R_R,
    .R1 = &gb->C,
    .R2 = &gb->A,
    .mcycles = 1,
    .mnemonic = "LD C,A"};

  opcodeTable[0x57] = (OpcodeEntry){.handler = LD_R_R,
    .R1 = &gb->D,
    .R2 = &gb->A,
    .mcycles = 1,
    .mnemonic = "LD D,A"};

  opcodeTable[0x67] = (OpcodeEntry){.handler = LD_R_R,
    .R1 = &gb->H,
    .R2 = &gb->A,
    .mcycles = 1,
    .mnemonic = "LD H,A"};

  opcodeTable[0x77] = (OpcodeEntry){.handler = LD_Addr_R,
    .BitArgs = {.R = &gb->A, .address = gb->HL},
    .mcycles = 2,
    .mnemonic = "LD (HL),A"};

  opcodeTable[0x7B] = (OpcodeEntry){.handler = LD_R_R,
    .R1 = &gb->A,
    .R2 = &gb->E,
    .mcycles = 1,
    .mnemonic = "LD A,E"};

  opcodeTable[0xAF] = (OpcodeEntry){
    .handler = XOR_R, .arg = &gb->A, .mcycles = 1, .mnemonic = "XOR A,A"};

  opcodeTable[0xC1] = (OpcodeEntry){.handler = POP_RR,
    .BitArgs.RR = &gb->BC,
    .mcycles = 3,
    .mnemonic = "POP BC"};

  opcodeTable[0xC5] = (OpcodeEntry){.handler = PUSH_RR,
    .BitArgs.RR = &gb->BC,
    .mcycles = 4,
    .mnemonic = "PUSH BC"};

  opcodeTable[0xC9] =
    (OpcodeEntry){.handler = RET, .mcycles = 4, .mnemonic = "RET"};

  opcodeTable[0xCD] =
    (OpcodeEntry){.handler = CALL_u16, .mcycles = 6, .mnemonic = "CALL u16"};

  opcodeTable[0xE0] = (OpcodeEntry){.handler = LD_AddrPlusu8_R,
    .arg = &gb->A,
    .mcycles = 3,
    .mnemonic = "LD (FF00+u8),A"};

  opcodeTable[0xE2] = (OpcodeEntry){.handler = LD_Addr_R,
    .BitArgs = {.R = &gb->A, .address = 0xFF00 + gb->C},
    .mcycles = 2,
    .mnemonic = "LD (FF00+C),A"};

  opcodeTable[0xEA] = (OpcodeEntry){.handler = LD_u16Addr_R,
    .BitArgs.R = &gb->A,
    .mcycles = 4,
    .mnemonic = "LD (u16),A"};

  opcodeTable[0xF0] = (OpcodeEntry){.handler = LD_R_AddrPlusu8,
    .BitArgs.R = &gb->A,
    .mcycles = 3,
    .mnemonic = "LD A,(FF00+u8)"};

  opcodeTable[0xFE] =
    (OpcodeEntry){.handler = CP_i8, .mcycles = 2, .mnemonic = "CP A,u8"};
}
