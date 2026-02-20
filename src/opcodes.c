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
    .handler = DEC_R, .BitArgs.R = &gb->C, .mcycles = 1, .mnemonic = "DEC C"};

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

  opcodeTable[0x15] = (OpcodeEntry){
    .handler = DEC_R, .BitArgs.R = &gb->D, .mcycles = 1, .mnemonic = "DEC D"};

  opcodeTable[0x16] = (OpcodeEntry){
    .handler = LD_R_u8, .arg = &gb->D, .mcycles = 2, .mnemonic = "LD D,u8"};

  opcodeTable[0x17] =
    (OpcodeEntry){.handler = RLA, .mcycles = 1, .mnemonic = "RLA"};

  opcodeTable[0x18] =
    (OpcodeEntry){.handler = JR, .mcycles = 3, .mnemonic = "JR i8"};

  opcodeTable[0x1A] = (OpcodeEntry){.handler = LD_R_Addr,
    .BitArgs = {.R = &gb->A, .address = gb->DE},
    .mcycles = 2,
    .mnemonic = "LD A,(DE)"};

  opcodeTable[0x1D] = (OpcodeEntry){
    .handler = DEC_R, .BitArgs.R = &gb->E, .mcycles = 1, .mnemonic = "DEC E"};

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

  opcodeTable[0x24] = (OpcodeEntry){
    .handler = INC_R, .BitArgs.R = &gb->H, .mcycles = 1, .mnemonic = "INC H"};

  opcodeTable[0x28] = (OpcodeEntry){
    .handler = JR_Z, .mcycles = 2, .mcyclesTrue = 3, .mnemonic = "JR Z,i8"};

  opcodeTable[0x2E] = (OpcodeEntry){
    .handler = LD_R_u8, .arg = &gb->L, .mcycles = 2, .mnemonic = "LD L,u8"};

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

  /***  LD (0x40 - 0x7F) + HALT ***/

  /// 0x4n ///
  opcodeTable[0x40] = (OpcodeEntry){.handler = LD_R_R,
    .R1 = &gb->B,
    .R2 = &gb->B,
    .mcycles = 1,
    .mnemonic = "LD B,B"};

  opcodeTable[0x41] = (OpcodeEntry){.handler = LD_R_R,
    .R1 = &gb->B,
    .R2 = &gb->C,
    .mcycles = 1,
    .mnemonic = "LD B,C"};

  opcodeTable[0x42] = (OpcodeEntry){.handler = LD_R_R,
    .R1 = &gb->B,
    .R2 = &gb->D,
    .mcycles = 1,
    .mnemonic = "LD B,D"};

  opcodeTable[0x43] = (OpcodeEntry){.handler = LD_R_R,
    .R1 = &gb->B,
    .R2 = &gb->E,
    .mcycles = 1,
    .mnemonic = "LD B,E"};

  opcodeTable[0x44] = (OpcodeEntry){.handler = LD_R_R,
    .R1 = &gb->B,
    .R2 = &gb->H,
    .mcycles = 1,
    .mnemonic = "LD B,H"};

  opcodeTable[0x45] = (OpcodeEntry){.handler = LD_R_R,
    .R1 = &gb->B,
    .R2 = &gb->L,
    .mcycles = 1,
    .mnemonic = "LD B,L"};

  opcodeTable[0x46] = (OpcodeEntry){.handler = LD_R_Addr,
    .BitArgs = {.R = &gb->B, .address = gb->HL},
    .mcycles = 2,
    .mnemonic = "LD B,(HL)"};

  opcodeTable[0x47] = (OpcodeEntry){.handler = LD_R_R,
    .R1 = &gb->B,
    .R2 = &gb->A,
    .mcycles = 1,
    .mnemonic = "LD B,A"};

  opcodeTable[0x48] = (OpcodeEntry){.handler = LD_R_R,
    .R1 = &gb->C,
    .R2 = &gb->B,
    .mcycles = 1,
    .mnemonic = "LD C,B"};

  opcodeTable[0x49] = (OpcodeEntry){.handler = LD_R_R,
    .R1 = &gb->C,
    .R2 = &gb->C,
    .mcycles = 1,
    .mnemonic = "LD C,C"};

  opcodeTable[0x4A] = (OpcodeEntry){.handler = LD_R_R,
    .R1 = &gb->C,
    .R2 = &gb->D,
    .mcycles = 1,
    .mnemonic = "LD C,D"};
  opcodeTable[0x4B] = (OpcodeEntry){.handler = LD_R_R,
    .R1 = &gb->C,
    .R2 = &gb->E,
    .mcycles = 1,
    .mnemonic = "LD C,E"};

  opcodeTable[0x4C] = (OpcodeEntry){.handler = LD_R_R,
    .R1 = &gb->C,
    .R2 = &gb->H,
    .mcycles = 1,
    .mnemonic = "LD C,H"};

  opcodeTable[0x4D] = (OpcodeEntry){.handler = LD_R_R,
    .R1 = &gb->C,
    .R2 = &gb->L,
    .mcycles = 1,
    .mnemonic = "LD C,L"};

  opcodeTable[0x4E] = (OpcodeEntry){.handler = LD_R_Addr,
    .BitArgs = {.R = &gb->C, .address = gb->HL},
    .mcycles = 2,
    .mnemonic = "LD C,(HL)"};

  opcodeTable[0x4F] = (OpcodeEntry){.handler = LD_R_R,
    .R1 = &gb->C,
    .R2 = &gb->A,
    .mcycles = 1,
    .mnemonic = "LD C,A"};

  /// 0x5n ///
  opcodeTable[0x50] = (OpcodeEntry){.handler = LD_R_R,
    .R1 = &gb->D,
    .R2 = &gb->B,
    .mcycles = 1,
    .mnemonic = "LD D,B"};

  opcodeTable[0x51] = (OpcodeEntry){.handler = LD_R_R,
    .R1 = &gb->D,
    .R2 = &gb->C,
    .mcycles = 1,
    .mnemonic = "LD D,C"};

  opcodeTable[0x52] = (OpcodeEntry){.handler = LD_R_R,
    .R1 = &gb->D,
    .R2 = &gb->D,
    .mcycles = 1,
    .mnemonic = "LD D,D"};

  opcodeTable[0x53] = (OpcodeEntry){.handler = LD_R_R,
    .R1 = &gb->D,
    .R2 = &gb->E,
    .mcycles = 1,
    .mnemonic = "LD D,E"};

  opcodeTable[0x54] = (OpcodeEntry){.handler = LD_R_R,
    .R1 = &gb->D,
    .R2 = &gb->H,
    .mcycles = 1,
    .mnemonic = "LD D,H"};

  opcodeTable[0x55] = (OpcodeEntry){.handler = LD_R_R,
    .R1 = &gb->D,
    .R2 = &gb->L,
    .mcycles = 1,
    .mnemonic = "LD D,L"};

  opcodeTable[0x56] = (OpcodeEntry){.handler = LD_R_Addr,
    .BitArgs = {.R = &gb->D, .address = gb->HL},
    .mcycles = 2,
    .mnemonic = "LD D,(HL)"};

  opcodeTable[0x57] = (OpcodeEntry){.handler = LD_R_R,
    .R1 = &gb->D,
    .R2 = &gb->A,
    .mcycles = 1,
    .mnemonic = "LD D,A"};

  opcodeTable[0x58] = (OpcodeEntry){.handler = LD_R_R,
    .R1 = &gb->E,
    .R2 = &gb->B,
    .mcycles = 1,
    .mnemonic = "LD E,B"};

  opcodeTable[0x59] = (OpcodeEntry){.handler = LD_R_R,
    .R1 = &gb->E,
    .R2 = &gb->C,
    .mcycles = 1,
    .mnemonic = "LD E,C"};

  opcodeTable[0x5A] = (OpcodeEntry){.handler = LD_R_R,
    .R1 = &gb->E,
    .R2 = &gb->D,
    .mcycles = 1,
    .mnemonic = "LD E,D"};

  opcodeTable[0x5B] = (OpcodeEntry){.handler = LD_R_R,
    .R1 = &gb->E,
    .R2 = &gb->E,
    .mcycles = 1,
    .mnemonic = "LD E,E"};

  opcodeTable[0x5C] = (OpcodeEntry){.handler = LD_R_R,
    .R1 = &gb->E,
    .R2 = &gb->H,
    .mcycles = 1,
    .mnemonic = "LD E,H"};

  opcodeTable[0x5D] = (OpcodeEntry){.handler = LD_R_R,
    .R1 = &gb->E,
    .R2 = &gb->L,
    .mcycles = 1,
    .mnemonic = "LD E,L"};

  opcodeTable[0x5E] = (OpcodeEntry){.handler = LD_R_Addr,
    .BitArgs = {.R = &gb->E, .address = gb->HL},
    .mcycles = 2,
    .mnemonic = "LD E,(HL)"};

  opcodeTable[0x5F] = (OpcodeEntry){.handler = LD_R_R,
    .R1 = &gb->E,
    .R2 = &gb->A,
    .mcycles = 1,
    .mnemonic = "LD E,A"};

  /// 0x6n ///

  opcodeTable[0x60] = (OpcodeEntry){.handler = LD_R_R,
    .R1 = &gb->H,
    .R2 = &gb->B,
    .mcycles = 1,
    .mnemonic = "LD H,B"};

  opcodeTable[0x61] = (OpcodeEntry){.handler = LD_R_R,
    .R1 = &gb->H,
    .R2 = &gb->C,
    .mcycles = 1,
    .mnemonic = "LD H,C"};

  opcodeTable[0x62] = (OpcodeEntry){.handler = LD_R_R,
    .R1 = &gb->H,
    .R2 = &gb->D,
    .mcycles = 1,
    .mnemonic = "LD H,D"};

  opcodeTable[0x63] = (OpcodeEntry){.handler = LD_R_R,
    .R1 = &gb->H,
    .R2 = &gb->E,
    .mcycles = 1,
    .mnemonic = "LD H,E"};

  opcodeTable[0x64] = (OpcodeEntry){.handler = LD_R_R,
    .R1 = &gb->H,
    .R2 = &gb->H,
    .mcycles = 1,
    .mnemonic = "LD H,H"};

  opcodeTable[0x65] = (OpcodeEntry){.handler = LD_R_R,
    .R1 = &gb->H,
    .R2 = &gb->L,
    .mcycles = 1,
    .mnemonic = "LD H,L"};

  opcodeTable[0x66] = (OpcodeEntry){.handler = LD_R_Addr,
    .BitArgs = {.R = &gb->H, .address = gb->HL},
    .mcycles = 2,
    .mnemonic = "LD H,(HL)"};

  opcodeTable[0x67] = (OpcodeEntry){.handler = LD_R_R,
    .R1 = &gb->H,
    .R2 = &gb->A,
    .mcycles = 1,
    .mnemonic = "LD H,A"};

  opcodeTable[0x68] = (OpcodeEntry){.handler = LD_R_R,
    .R1 = &gb->L,
    .R2 = &gb->B,
    .mcycles = 1,
    .mnemonic = "LD L,B"};

  opcodeTable[0x69] = (OpcodeEntry){.handler = LD_R_R,
    .R1 = &gb->L,
    .R2 = &gb->C,
    .mcycles = 1,
    .mnemonic = "LD L,C"};

  opcodeTable[0x6A] = (OpcodeEntry){.handler = LD_R_R,
    .R1 = &gb->L,
    .R2 = &gb->D,
    .mcycles = 1,
    .mnemonic = "LD L,D"};

  opcodeTable[0x6B] = (OpcodeEntry){.handler = LD_R_R,
    .R1 = &gb->L,
    .R2 = &gb->E,
    .mcycles = 1,
    .mnemonic = "LD L,E"};

  opcodeTable[0x6C] = (OpcodeEntry){.handler = LD_R_R,
    .R1 = &gb->L,
    .R2 = &gb->H,
    .mcycles = 1,
    .mnemonic = "LD L,H"};

  opcodeTable[0x6D] = (OpcodeEntry){.handler = LD_R_R,
    .R1 = &gb->L,
    .R2 = &gb->L,
    .mcycles = 1,
    .mnemonic = "LD L,L"};

  opcodeTable[0x6E] = (OpcodeEntry){.handler = LD_R_Addr,
    .BitArgs = {.R = &gb->L, .address = gb->HL},
    .mcycles = 2,
    .mnemonic = "LD L,(HL)"};

  opcodeTable[0x6F] = (OpcodeEntry){.handler = LD_R_R,
    .R1 = &gb->L,
    .R2 = &gb->A,
    .mcycles = 1,
    .mnemonic = "LD L,A"};

  /// 0x7n ///

  opcodeTable[0x70] = (OpcodeEntry){.handler = LD_Addr_R,
    .BitArgs = {.R = &gb->B, .address = gb->HL},
    .mcycles = 2,
    .mnemonic = "LD (HL),B"};

  opcodeTable[0x71] = (OpcodeEntry){.handler = LD_Addr_R,
    .BitArgs = {.R = &gb->C, .address = gb->HL},
    .mcycles = 2,
    .mnemonic = "LD (HL),C"};

  opcodeTable[0x72] = (OpcodeEntry){.handler = LD_Addr_R,
    .BitArgs = {.R = &gb->D, .address = gb->HL},
    .mcycles = 2,
    .mnemonic = "LD (HL),D"};

  opcodeTable[0x73] = (OpcodeEntry){.handler = LD_Addr_R,
    .BitArgs = {.R = &gb->E, .address = gb->HL},
    .mcycles = 2,
    .mnemonic = "LD (HL),E"};

  opcodeTable[0x74] = (OpcodeEntry){.handler = LD_Addr_R,
    .BitArgs = {.R = &gb->H, .address = gb->HL},
    .mcycles = 2,
    .mnemonic = "LD (HL),H"};

  opcodeTable[0x75] = (OpcodeEntry){.handler = LD_Addr_R,
    .BitArgs = {.R = &gb->L, .address = gb->HL},
    .mcycles = 2,
    .mnemonic = "LD (HL),L"};

  /// HALT ///

  opcodeTable[0x76] =
    (OpcodeEntry){.handler = HALT, .mcycles = 1, .mnemonic = "HALT"};

  opcodeTable[0x77] = (OpcodeEntry){.handler = LD_Addr_R,
    .BitArgs = {.R = &gb->A, .address = gb->HL},
    .mcycles = 2,
    .mnemonic = "LD (HL),A"};

  opcodeTable[0x78] = (OpcodeEntry){.handler = LD_R_R,
    .R1 = &gb->A,
    .R2 = &gb->B,
    .mcycles = 1,
    .mnemonic = "LD A,B"};

  opcodeTable[0x79] = (OpcodeEntry){.handler = LD_R_R,
    .R1 = &gb->A,
    .R2 = &gb->C,
    .mcycles = 1,
    .mnemonic = "LD A,C"};

  opcodeTable[0x7A] = (OpcodeEntry){.handler = LD_R_R,
    .R1 = &gb->A,
    .R2 = &gb->D,
    .mcycles = 1,
    .mnemonic = "LD A,D"};

  opcodeTable[0x7B] = (OpcodeEntry){.handler = LD_R_R,
    .R1 = &gb->A,
    .R2 = &gb->E,
    .mcycles = 1,
    .mnemonic = "LD A,E"};

  opcodeTable[0x7C] = (OpcodeEntry){.handler = LD_R_R,
    .R1 = &gb->A,
    .R2 = &gb->H,
    .mcycles = 1,
    .mnemonic = "LD A,H"};

  opcodeTable[0x7D] = (OpcodeEntry){.handler = LD_R_R,
    .R1 = &gb->A,
    .R2 = &gb->L,
    .mcycles = 1,
    .mnemonic = "LD A,L"};

  opcodeTable[0x7E] = (OpcodeEntry){.handler = LD_R_Addr,
    .BitArgs = {.R = &gb->A, .address = gb->HL},
    .mcycles = 2,
    .mnemonic = "LD A,(HL)"};

  opcodeTable[0x7F] = (OpcodeEntry){.handler = LD_R_R,
    .R1 = &gb->A,
    .R2 = &gb->A,
    .mcycles = 1,
    .mnemonic = "LD A,A"};

  opcodeTable[0x90] = (OpcodeEntry){
    .handler = SUB_R, .arg = &gb->B, .mcycles = 1, .mnemonic = "SUB A,B"};

  opcodeTable[0xAF] = (OpcodeEntry){
    .handler = XOR_R, .arg = &gb->A, .mcycles = 1, .mnemonic = "XOR A,A"};

  // opcodeTable[0xBE] = (OpcodeEntry){.handler = };

  opcodeTable[0xC1] = (OpcodeEntry){.handler = POP_RR,
    .BitArgs.RR = &gb->BC,
    .mcycles = 3,
    .mnemonic = "POP BC"};

  opcodeTable[0xC3] =
    (OpcodeEntry){.handler = JP, .mcycles = 4, .mnemonic = "JP u16"};

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
