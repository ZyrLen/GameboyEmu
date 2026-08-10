#include "opcodes.h"
#include "gameboy.h"
#include <stdlib.h>

OpcodeEntry opcodeTable[256];

// mcycles is machine cycles. 1 mycycle = 4 timer cycles
// Note: machine cycles listed below have some errors for now, will come back to it after more of the opcodes are done

/* Might do something like this if memory usage is too high later*/

// char *regStr[] = {"B", "C", "D", "E", "H", "L", "(HL)", "A"};
// u8 *regs[] = {&gb->B, &gb->C, &gb->D, 
//							&gb->E, &gb->H, &gb->L, &gb->HL, &gb->A};

// if (0x40 <= opcode && opcode < 0x80) {
// 	return (OpcodeEntry){.handler = LD_R_R, .R1 = regs[]}
// }
// June 6, 2026: 0x40-0xBF are in table now

void initOpcodeTable(Gameboy *gb) {
  opcodeTable[0x0] =
    (OpcodeEntry){.handler = NOP, .mcycles = 1, .mnemonic = "NOP"};

  opcodeTable[0x1] =
    (OpcodeEntry){.handler = LD_RR_u16, .arg = &gb->BC, .mcycles = 3, .mnemonic = "LD BC, u16"};

  opcodeTable[0x2] =
    (OpcodeEntry){.handler = LD_Addr_R, .address = &gb->BC, .arg = &gb->A, .mcycles = 2, .mnemonic = "LD (BC), A"};

  opcodeTable[0x3] =
    (OpcodeEntry){.handler = INC_RR, .arg = &gb->BC, .mcycles = 2, .mnemonic = "INC BC"};

  opcodeTable[0x4] = (OpcodeEntry){
    .handler = INC_R, .arg = &gb->B, .mcycles = 1, .mnemonic = "INC B"};

  opcodeTable[0x5] = (OpcodeEntry){
    .handler = DEC_R, .arg = &gb->B, .mcycles = 1, .mnemonic = "DEC B"};

  opcodeTable[0x6] = (OpcodeEntry){
    .handler = LD_R_u8, .arg = &gb->B, .mcycles = 2, .mnemonic = "LD B,u8"};

  // opcodeTable[0x7] =
  //   (OpcodeEntry){.handler = , .mcycles = , .mnemonic = ""};

  opcodeTable[0x8] =
    (OpcodeEntry){.handler = LD_u16Addr_RR, .arg = &gb->sp, .mcycles = 5, .mnemonic = "LD (u16), SP"};

  // opcodeTable[0x9] =
  //   (OpcodeEntry){.handler = , .mcycles = , .mnemonic = ""};

  // opcodeTable[0xA] =
  //   (OpcodeEntry){.handler = , .mcycles = , .mnemonic = ""};

  // opcodeTable[0xB] =
  //   (OpcodeEntry){.handler = , .mcycles = , .mnemonic = ""};

  opcodeTable[0xC] = (OpcodeEntry){
    .handler = INC_R, .arg = &gb->C, .mcycles = 1, .mnemonic = "INC C"};

  opcodeTable[0xD] = (OpcodeEntry){
    .handler = DEC_R, .arg = &gb->C, .mcycles = 1, .mnemonic = "DEC C"};

  opcodeTable[0xE] = (OpcodeEntry){
    .handler = LD_R_u8, .arg = &gb->C, .mcycles = 2, .mnemonic = "LD C,u8"};

  // opcodeTable[0xF] =
  //   (OpcodeEntry){.handler = , .mcycles = , .mnemonic = ""};

  opcodeTable[0x11] = (OpcodeEntry){.handler = LD_RR_u16,
    .arg = &gb->DE,
    .mcycles = 3,
    .mnemonic = "LD DE,u16"};
  opcodeTable[0x12] = (OpcodeEntry){.handler = LD_Addr_R,
    .arg = &gb->A,
    .address = &gb->DE,
    .mcycles = 2,
    .mnemonic = "LD (DE),A"};

  opcodeTable[0x13] = (OpcodeEntry){
    .handler = INC_RR, .arg = &gb->DE, .mcycles = 2, .mnemonic = "INC DE"};

  opcodeTable[0x14] = (OpcodeEntry){
    .handler = INC_R, .arg = &gb->D, .mcycles = 1, .mnemonic = "INC D"};

  opcodeTable[0x15] = (OpcodeEntry){
    .handler = DEC_R, .arg = &gb->D, .mcycles = 1, .mnemonic = "DEC D"};

  opcodeTable[0x16] = (OpcodeEntry){
    .handler = LD_R_u8, .arg = &gb->D, .mcycles = 2, .mnemonic = "LD D,u8"};

  opcodeTable[0x17] =
    (OpcodeEntry){.handler = RLA, .mcycles = 1, .mnemonic = "RLA"};

  opcodeTable[0x18] =
    (OpcodeEntry){.handler = JR, .mcycles = 3, .mnemonic = "JR i8"};
  opcodeTable[0x1C] = (OpcodeEntry){.handler = INC_R, 
    .arg = &gb->E,
    .mcycles = 1,
    .mnemonic = "INC E"};

  opcodeTable[0x1A] = (OpcodeEntry){.handler = LD_R_Addr,
    .arg = &gb->A,
    .address = &gb->DE,
    .mcycles = 2,
    .mnemonic = "LD A,(DE)"};

  opcodeTable[0x1D] = (OpcodeEntry){
    .handler = DEC_R, .arg = &gb->E, .mcycles = 1, .mnemonic = "DEC E"};

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

  opcodeTable[0x23] = (OpcodeEntry){
    .handler = INC_RR, .arg = &gb->HL, .mcycles = 2, .mnemonic = "INC HL"};

  opcodeTable[0x24] = (OpcodeEntry){
    .handler = INC_R, .arg = &gb->H, .mcycles = 1, .mnemonic = "INC H"};

  opcodeTable[0x28] = (OpcodeEntry){
    .handler = JR_Z, .mcycles = 2, .mcyclesTrue = 3, .mnemonic = "JR Z,i8"};

	opcodeTable[0x2A] = (OpcodeEntry){.handler = LD_A_HLplus, .mcycles = 2, .mnemonic = "LD A,(HL+)"};

  opcodeTable[0x2E] = (OpcodeEntry){
    .handler = LD_R_u8, .arg = &gb->L, .mcycles = 2, .mnemonic = "LD L,u8"};

  opcodeTable[0x31] = (OpcodeEntry){.handler = LD_SP_u16,
    .arg = &gb->sp, .mcycles = 3, .mnemonic = "LD SP,u16"};

  opcodeTable[0x32] = (OpcodeEntry){
    .handler = LD_HLminus_A, .mcycles = 2, .mnemonic = "LD (HL-),A"};

  opcodeTable[0x3D] = (OpcodeEntry){
    .handler = DEC_R, .arg = &gb->A, .mcycles = 1, .mnemonic = "DEC A"};

  opcodeTable[0x3E] = (OpcodeEntry){
    .handler = LD_R_u8, .arg = &gb->A, .mcycles = 2, .mnemonic = "LD A,u8"};

  opcodeTable[0x3F] = (OpcodeEntry){
    .handler = CCF, .mcycles = 1, .mnemonic = "CCF"};

	opcodeTable[0x40] = (OpcodeEntry){.handler = LD_R_R, .R1 = &gb->B, .R2 = &gb->B,
		.mcycles = 1, .mnemonic = "LD B,B"};

	opcodeTable[0x41] = (OpcodeEntry){.handler = LD_R_R, .R1 = &gb->B, .R2 = &gb->C,
		.mcycles = 1, .mnemonic = "LD B,C"};

	opcodeTable[0x42] = (OpcodeEntry){.handler = LD_R_R, .R1 = &gb->B, .R2 = &gb->D,
		.mcycles = 1, .mnemonic = "LD B,D"};

	opcodeTable[0x43] = (OpcodeEntry){.handler = LD_R_R, .R1 = &gb->B, .R2 = &gb->E,
		.mcycles = 1, .mnemonic = "LD B,E"};

	opcodeTable[0x44] = (OpcodeEntry){.handler = LD_R_R, .R1 = &gb->B, .R2 = &gb->H,
		.mcycles = 1, .mnemonic = "LD B,H"};

	opcodeTable[0x45] = (OpcodeEntry){.handler = LD_R_R, .R1 = &gb->B, .R2 = &gb->L,
		.mcycles = 1, .mnemonic = "LD B,L"};

	opcodeTable[0x46] = (OpcodeEntry){.handler = LD_R_Addr,
		.arg = &gb->B, .address = &gb->HL,
		.mcycles = 1, .mnemonic = "LD B,(HL)"};

	opcodeTable[0x47] = (OpcodeEntry){.handler = LD_R_R, .R1 = &gb->B, .R2 = &gb->A,
		.mcycles = 1, .mnemonic = "LD B,A"};

	opcodeTable[0x48] = (OpcodeEntry){.handler = LD_R_R, .R1 = &gb->C, .R2 = &gb->B,
		.mcycles = 1, .mnemonic = "LD C,B"};

	opcodeTable[0x49] = (OpcodeEntry){.handler = LD_R_R, .R1 = &gb->C, .R2 = &gb->C,
		.mcycles = 1, .mnemonic = "LD C,C"};

	opcodeTable[0x4A] = (OpcodeEntry){.handler = LD_R_R, .R1 = &gb->C, .R2 = &gb->D,
		.mcycles = 1, .mnemonic = "LD C,D"};

	opcodeTable[0x4B] = (OpcodeEntry){.handler = LD_R_R, .R1 = &gb->C, .R2 = &gb->E,
		.mcycles = 1, .mnemonic = "LD C,E"};

	opcodeTable[0x4C] = (OpcodeEntry){.handler = LD_R_R, .R1 = &gb->C, .R2 = &gb->H,
		.mcycles = 1, .mnemonic = "LD C,H"};

	opcodeTable[0x4D] = (OpcodeEntry){.handler = LD_R_R, .R1 = &gb->C, .R2 = &gb->L,
		.mcycles = 1, .mnemonic = "LD C,L"};

	opcodeTable[0x4E] = (OpcodeEntry){.handler = LD_R_Addr,
		.arg = &gb->C, .address = &gb->HL,
		.mcycles = 1, .mnemonic = "LD C,(HL)"};

	opcodeTable[0x4F] = (OpcodeEntry){.handler = LD_R_R, .R1 = &gb->C, .R2 = &gb->A,
		.mcycles = 1, .mnemonic = "LD C,A"};

	opcodeTable[0x50] = (OpcodeEntry){.handler = LD_R_R, .R1 = &gb->D, .R2 = &gb->B,
		.mcycles = 1, .mnemonic = "LD D,B"};

	opcodeTable[0x51] = (OpcodeEntry){.handler = LD_R_R, .R1 = &gb->D, .R2 = &gb->C,
		.mcycles = 1, .mnemonic = "LD D,C"};

	opcodeTable[0x52] = (OpcodeEntry){.handler = LD_R_R, .R1 = &gb->D, .R2 = &gb->D,
		.mcycles = 1, .mnemonic = "LD D,D"};

	opcodeTable[0x53] = (OpcodeEntry){.handler = LD_R_R, .R1 = &gb->D, .R2 = &gb->E,
		.mcycles = 1, .mnemonic = "LD D,E"};

	opcodeTable[0x54] = (OpcodeEntry){.handler = LD_R_R, .R1 = &gb->D, .R2 = &gb->H,
		.mcycles = 1, .mnemonic = "LD D,H"};

	opcodeTable[0x55] = (OpcodeEntry){.handler = LD_R_R, .R1 = &gb->D, .R2 = &gb->L,
		.mcycles = 1, .mnemonic = "LD D,L"};

	opcodeTable[0x56] = (OpcodeEntry){.handler = LD_R_Addr,
		.arg = &gb->D, .address = &gb->HL,
		.mcycles = 1, .mnemonic = "LD D,(HL)"};

	opcodeTable[0x57] = (OpcodeEntry){.handler = LD_R_R, .R1 = &gb->D, .R2 = &gb->A,
		.mcycles = 1, .mnemonic = "LD D,A"};

	opcodeTable[0x58] = (OpcodeEntry){.handler = LD_R_R, .R1 = &gb->E, .R2 = &gb->B,
		.mcycles = 1, .mnemonic = "LD E,B"};

	opcodeTable[0x59] = (OpcodeEntry){.handler = LD_R_R, .R1 = &gb->E, .R2 = &gb->C,
		.mcycles = 1, .mnemonic = "LD E,C"};

	opcodeTable[0x5A] = (OpcodeEntry){.handler = LD_R_R, .R1 = &gb->E, .R2 = &gb->D,
		.mcycles = 1, .mnemonic = "LD E,D"};

	opcodeTable[0x5B] = (OpcodeEntry){.handler = LD_R_R, .R1 = &gb->E, .R2 = &gb->E,
		.mcycles = 1, .mnemonic = "LD E,E"};

	opcodeTable[0x5C] = (OpcodeEntry){.handler = LD_R_R, .R1 = &gb->E, .R2 = &gb->H,
		.mcycles = 1, .mnemonic = "LD E,H"};

	opcodeTable[0x5D] = (OpcodeEntry){.handler = LD_R_R, .R1 = &gb->E, .R2 = &gb->L,
		.mcycles = 1, .mnemonic = "LD E,L"};

	opcodeTable[0x5E] = (OpcodeEntry){.handler = LD_R_Addr,
		.arg = &gb->E, .address = &gb->HL,
		.mcycles = 1, .mnemonic = "LD E,(HL)"};

	opcodeTable[0x5F] = (OpcodeEntry){.handler = LD_R_R, .R1 = &gb->E, .R2 = &gb->A,
		.mcycles = 1, .mnemonic = "LD E,A"};

	opcodeTable[0x60] = (OpcodeEntry){.handler = LD_R_R, .R1 = &gb->H, .R2 = &gb->B,
		.mcycles = 1, .mnemonic = "LD H,B"};

	opcodeTable[0x61] = (OpcodeEntry){.handler = LD_R_R, .R1 = &gb->H, .R2 = &gb->C,
		.mcycles = 1, .mnemonic = "LD H,C"};

	opcodeTable[0x62] = (OpcodeEntry){.handler = LD_R_R, .R1 = &gb->H, .R2 = &gb->D,
		.mcycles = 1, .mnemonic = "LD H,D"};

	opcodeTable[0x63] = (OpcodeEntry){.handler = LD_R_R, .R1 = &gb->H, .R2 = &gb->E,
		.mcycles = 1, .mnemonic = "LD H,E"};

	opcodeTable[0x64] = (OpcodeEntry){.handler = LD_R_R, .R1 = &gb->H, .R2 = &gb->H,
		.mcycles = 1, .mnemonic = "LD H,H"};

	opcodeTable[0x65] = (OpcodeEntry){.handler = LD_R_R, .R1 = &gb->H, .R2 = &gb->L,
		.mcycles = 1, .mnemonic = "LD H,L"};

	opcodeTable[0x66] = (OpcodeEntry){.handler = LD_R_Addr,
		.arg = &gb->H, .address = &gb->HL,
		.mcycles = 1, .mnemonic = "LD H,(HL)"};

	opcodeTable[0x67] = (OpcodeEntry){.handler = LD_R_R, .R1 = &gb->H, .R2 = &gb->A,
		.mcycles = 1, .mnemonic = "LD H,A"};

	opcodeTable[0x68] = (OpcodeEntry){.handler = LD_R_R, .R1 = &gb->L, .R2 = &gb->B,
		.mcycles = 1, .mnemonic = "LD L,B"};

	opcodeTable[0x69] = (OpcodeEntry){.handler = LD_R_R, .R1 = &gb->L, .R2 = &gb->C,
		.mcycles = 1, .mnemonic = "LD L,C"};

	opcodeTable[0x6A] = (OpcodeEntry){.handler = LD_R_R, .R1 = &gb->L, .R2 = &gb->D,
		.mcycles = 1, .mnemonic = "LD L,D"};

	opcodeTable[0x6B] = (OpcodeEntry){.handler = LD_R_R, .R1 = &gb->L, .R2 = &gb->E,
		.mcycles = 1, .mnemonic = "LD L,E"};

	opcodeTable[0x6C] = (OpcodeEntry){.handler = LD_R_R, .R1 = &gb->L, .R2 = &gb->H,
		.mcycles = 1, .mnemonic = "LD L,H"};

	opcodeTable[0x6D] = (OpcodeEntry){.handler = LD_R_R, .R1 = &gb->L, .R2 = &gb->L,
		.mcycles = 1, .mnemonic = "LD L,L"};

	opcodeTable[0x6E] = (OpcodeEntry){.handler = LD_R_Addr,
		.arg = &gb->L, .address = &gb->HL,
		.mcycles = 1, .mnemonic = "LD L,(HL)"};

	opcodeTable[0x6F] = (OpcodeEntry){.handler = LD_R_R, .R1 = &gb->L, .R2 = &gb->A,
		.mcycles = 1, .mnemonic = "LD L,A"};

  opcodeTable[0x70] = (OpcodeEntry){.handler = LD_Addr_R, .arg = &gb->B, .address = &gb->HL,
    .mcycles = 2, .mnemonic = "LD (HL),B"};

  opcodeTable[0x71] = (OpcodeEntry){.handler = LD_Addr_R, .arg = &gb->C, .address = &gb->HL,
    .mcycles = 2, .mnemonic = "LD (HL),C"};

  opcodeTable[0x72] = (OpcodeEntry){.handler = LD_Addr_R, .arg = &gb->D, .address = &gb->HL,
    .mcycles = 2, .mnemonic = "LD (HL),D"};

  opcodeTable[0x73] = (OpcodeEntry){.handler = LD_Addr_R, .arg = &gb->E, .address = &gb->HL,
    .mcycles = 2, .mnemonic = "LD (HL),E"};

  opcodeTable[0x74] = (OpcodeEntry){.handler = LD_Addr_R, .arg = &gb->H, .address = &gb->HL,
    .mcycles = 2, .mnemonic = "LD (HL),H"};

  opcodeTable[0x75] = (OpcodeEntry){.handler = LD_Addr_R, .arg = &gb->L, .address = &gb->HL,
    .mcycles = 2, .mnemonic = "LD (HL),L"};

  opcodeTable[0x76] =
    (OpcodeEntry){.handler = HALT, .mcycles = 1, .mnemonic = "HALT"};

  opcodeTable[0x77] = (OpcodeEntry){.handler = LD_Addr_R, .arg = &gb->A, .address = &gb->HL,
    .mcycles = 2, .mnemonic = "LD (HL),A"};

  opcodeTable[0x78] = (OpcodeEntry){.handler = LD_R_R, .R1 = &gb->A, .R2 = &gb->B,
    .mcycles = 1, .mnemonic = "LD A,B"};

  opcodeTable[0x79] = (OpcodeEntry){.handler = LD_R_R, .R1 = &gb->A, .R2 = &gb->C,
    .mcycles = 1, .mnemonic = "LD A,C"};

  opcodeTable[0x7A] = (OpcodeEntry){.handler = LD_R_R, .R1 = &gb->A, .R2 = &gb->D,
    .mcycles = 1, .mnemonic = "LD A,D"};

  opcodeTable[0x7B] = (OpcodeEntry){.handler = LD_R_R, .R1 = &gb->A, .R2 = &gb->E,
    .mcycles = 1, .mnemonic = "LD A,E"};

  opcodeTable[0x7C] = (OpcodeEntry){.handler = LD_R_R, .R1 = &gb->A, .R2 = &gb->H,
    .mcycles = 1, .mnemonic = "LD A,H"};

  opcodeTable[0x7D] = (OpcodeEntry){.handler = LD_R_R, .R1 = &gb->A, .R2 = &gb->L,
    .mcycles = 1, .mnemonic = "LD A,L"};

  opcodeTable[0x7E] = (OpcodeEntry){.handler = LD_R_Addr, .arg = &gb->A, .address = &gb->HL,
    .mcycles = 2, .mnemonic = "LD A,(HL)"};

  opcodeTable[0x7F] = (OpcodeEntry){.handler = LD_R_R, .R1 = &gb->A, .R2 = &gb->A,
    .mcycles = 1, .mnemonic = "LD A,A"};

	opcodeTable[0x80] = (OpcodeEntry){.handler = ADD, .arg = &gb->B,	.mcycles = 1, .mnemonic = "ADD A,B"};
	opcodeTable[0x81] = (OpcodeEntry){.handler = ADD, .arg = &gb->C, 	.mcycles = 1, .mnemonic = "ADD A,C"};
	opcodeTable[0x82] = (OpcodeEntry){.handler = ADD, .arg = &gb->D, 	.mcycles = 1, .mnemonic = "ADD A,D"};
	opcodeTable[0x83] = (OpcodeEntry){.handler = ADD, .arg = &gb->E, 	.mcycles = 1, .mnemonic = "ADD A,E"};
	opcodeTable[0x84] = (OpcodeEntry){.handler = ADD, .arg = &gb->H, 	.mcycles = 1, .mnemonic = "ADD A,H"};
	opcodeTable[0x85] = (OpcodeEntry){.handler = ADD, .arg = &gb->L, 	.mcycles = 1, .mnemonic = "ADD A,L"};
	opcodeTable[0x86] = (OpcodeEntry){.handler = ADD, .arg = &gb->HL,	.mcycles = 2, .mnemonic = "ADD A,(HL)"};
	opcodeTable[0x87] = (OpcodeEntry){.handler = ADD, .arg = &gb->A, 	.mcycles = 1, .mnemonic = "ADD A,A"};
	opcodeTable[0x88] = (OpcodeEntry){.handler = ADC, .arg = &gb->B, 	.mcycles = 1, .mnemonic = "ADC A,B"};
	opcodeTable[0x89] = (OpcodeEntry){.handler = ADC, .arg = &gb->C, 	.mcycles = 1, .mnemonic = "ADC A,C"};
	opcodeTable[0x8A] = (OpcodeEntry){.handler = ADC, .arg = &gb->D, 	.mcycles = 1, .mnemonic = "ADC A,D"};
	opcodeTable[0x8B] = (OpcodeEntry){.handler = ADC, .arg = &gb->E, 	.mcycles = 1, .mnemonic = "ADC A,E"};
	opcodeTable[0x8C] = (OpcodeEntry){.handler = ADC, .arg = &gb->H, 	.mcycles = 1, .mnemonic = "ADC A,H"};
	opcodeTable[0x8D] = (OpcodeEntry){.handler = ADC, .arg = &gb->L, 	.mcycles = 1, .mnemonic = "ADC A,L"};
	opcodeTable[0x8E] = (OpcodeEntry){.handler = ADC, .arg = &gb->HL,	.mcycles = 2, .mnemonic = "ADC A,(HL)"};
	opcodeTable[0x8F] = (OpcodeEntry){.handler = ADC, .arg = &gb->A, 	.mcycles = 1, .mnemonic = "ADC A,A"};

	opcodeTable[0x90] = (OpcodeEntry){.handler = SUB, .arg = &gb->B,	.mcycles = 1, .mnemonic = "SUB B"};
	opcodeTable[0x91] = (OpcodeEntry){.handler = SUB, .arg = &gb->C, 	.mcycles = 1, .mnemonic = "SUB C"};
	opcodeTable[0x92] = (OpcodeEntry){.handler = SUB, .arg = &gb->D, 	.mcycles = 1, .mnemonic = "SUB D"};
	opcodeTable[0x93] = (OpcodeEntry){.handler = SUB, .arg = &gb->E, 	.mcycles = 1, .mnemonic = "SUB E"};
	opcodeTable[0x94] = (OpcodeEntry){.handler = SUB, .arg = &gb->H, 	.mcycles = 1, .mnemonic = "SUB H"};
	opcodeTable[0x95] = (OpcodeEntry){.handler = SUB, .arg = &gb->L, 	.mcycles = 1, .mnemonic = "SUB L"};
	opcodeTable[0x96] = (OpcodeEntry){.handler = SUB, .arg = &gb->HL,	.mcycles = 2, .mnemonic = "SUB (HL)"};
	opcodeTable[0x97] = (OpcodeEntry){.handler = SUB, .arg = &gb->A, 	.mcycles = 1, .mnemonic = "SUB A"};
	opcodeTable[0x98] = (OpcodeEntry){.handler = SBC, .arg = &gb->B, 	.mcycles = 1, .mnemonic = "SBC A,B"};
	opcodeTable[0x99] = (OpcodeEntry){.handler = SBC, .arg = &gb->C, 	.mcycles = 1, .mnemonic = "SBC A,C"};
	opcodeTable[0x9A] = (OpcodeEntry){.handler = SBC, .arg = &gb->D, 	.mcycles = 1, .mnemonic = "SBC A,D"};
	opcodeTable[0x9B] = (OpcodeEntry){.handler = SBC, .arg = &gb->E, 	.mcycles = 1, .mnemonic = "SBC A,E"};
	opcodeTable[0x9C] = (OpcodeEntry){.handler = SBC, .arg = &gb->H, 	.mcycles = 1, .mnemonic = "SBC A,H"};
	opcodeTable[0x9D] = (OpcodeEntry){.handler = SBC, .arg = &gb->L, 	.mcycles = 1, .mnemonic = "SBC A,L"};
	opcodeTable[0x9E] = (OpcodeEntry){.handler = SBC, .arg = &gb->HL,	.mcycles = 2, .mnemonic = "SBC A,(HL)"};
	opcodeTable[0x9F] = (OpcodeEntry){.handler = SBC, .arg = &gb->A, 	.mcycles = 1, .mnemonic = "SBC A,A"};

	opcodeTable[0xA0] = (OpcodeEntry){.handler = AND, .arg = &gb->B,  .mcycles = 1, .mnemonic = "AND B"};
	opcodeTable[0xA1] = (OpcodeEntry){.handler = AND, .arg = &gb->C,  .mcycles = 1, .mnemonic = "AND C"};
	opcodeTable[0xA2] = (OpcodeEntry){.handler = AND, .arg = &gb->D,  .mcycles = 1, .mnemonic = "AND D"};
	opcodeTable[0xA3] = (OpcodeEntry){.handler = AND, .arg = &gb->E,  .mcycles = 1, .mnemonic = "AND E"};
	opcodeTable[0xA4] = (OpcodeEntry){.handler = AND, .arg = &gb->H,  .mcycles = 1, .mnemonic = "AND H"};
	opcodeTable[0xA5] = (OpcodeEntry){.handler = AND, .arg = &gb->L,  .mcycles = 1, .mnemonic = "AND L"};
	opcodeTable[0xA6] = (OpcodeEntry){.handler = AND, .arg = &gb->HL, .mcycles = 2, .mnemonic = "AND (HL)"};
	opcodeTable[0xA7] = (OpcodeEntry){.handler = AND, .arg = &gb->A,  .mcycles = 1, .mnemonic = "AND A"};
	opcodeTable[0xA8] = (OpcodeEntry){.handler = XOR, .arg = &gb->B,  .mcycles = 1, .mnemonic = "XOR B"};
	opcodeTable[0xA9] = (OpcodeEntry){.handler = XOR, .arg = &gb->C,  .mcycles = 1, .mnemonic = "XOR C"};
	opcodeTable[0xAA] = (OpcodeEntry){.handler = XOR, .arg = &gb->D,  .mcycles = 1, .mnemonic = "XOR D"};
	opcodeTable[0xAB] = (OpcodeEntry){.handler = XOR, .arg = &gb->E,  .mcycles = 1, .mnemonic = "XOR E"};
	opcodeTable[0xAC] = (OpcodeEntry){.handler = XOR, .arg = &gb->H,  .mcycles = 1, .mnemonic = "XOR H"};
	opcodeTable[0xAD] = (OpcodeEntry){.handler = XOR, .arg = &gb->L,  .mcycles = 1, .mnemonic = "XOR L"};
	opcodeTable[0xAE] = (OpcodeEntry){.handler = XOR, .arg = &gb->HL, .mcycles = 2, .mnemonic = "XOR (HL)"};
	opcodeTable[0xAF] = (OpcodeEntry){.handler = XOR, .arg = &gb->A,  .mcycles = 1, .mnemonic = "XOR A"};

opcodeTable[0xB0] = (OpcodeEntry){.handler = OR, .arg = &gb->B,  .mnemonic = "OR B"};
opcodeTable[0xB1] = (OpcodeEntry){.handler = OR, .arg = &gb->C,  .mnemonic = "OR C"};
opcodeTable[0xB2] = (OpcodeEntry){.handler = OR, .arg = &gb->D,  .mnemonic = "OR D"};
opcodeTable[0xB3] = (OpcodeEntry){.handler = OR, .arg = &gb->E,  .mnemonic = "OR E"};
opcodeTable[0xB4] = (OpcodeEntry){.handler = OR, .arg = &gb->H,  .mnemonic = "OR H"};
opcodeTable[0xB5] = (OpcodeEntry){.handler = OR, .arg = &gb->L,  .mnemonic = "OR L"};
opcodeTable[0xB6] = (OpcodeEntry){.handler = OR, .arg = &gb->HL, .mnemonic = "OR (HL)"};
opcodeTable[0xB7] = (OpcodeEntry){.handler = OR, .arg = &gb->A,  .mnemonic = "OR A"};
opcodeTable[0xB8] = (OpcodeEntry){.handler = CP, .arg = &gb->B,  .mnemonic = "CP B"};
opcodeTable[0xB9] = (OpcodeEntry){.handler = CP, .arg = &gb->C,  .mnemonic = "CP C"};
opcodeTable[0xBA] = (OpcodeEntry){.handler = CP, .arg = &gb->D,  .mnemonic = "CP D"};
opcodeTable[0xBB] = (OpcodeEntry){.handler = CP, .arg = &gb->E,  .mnemonic = "CP E"};
opcodeTable[0xBC] = (OpcodeEntry){.handler = CP, .arg = &gb->H,  .mnemonic = "CP H"};
opcodeTable[0xBD] = (OpcodeEntry){.handler = CP, .arg = &gb->L,  .mnemonic = "CP L"};
opcodeTable[0xBE] = (OpcodeEntry){.handler = CP, .arg = &gb->HL, .mnemonic = "CP (HL)"};
opcodeTable[0xBF] = (OpcodeEntry){.handler = CP, .arg = &gb->A,  .mnemonic = "CP A"};

  opcodeTable[0xC1] = (OpcodeEntry){
    .handler = POP, .arg = &gb->BC, .mcycles = 3, .mnemonic = "POP BC"};

  opcodeTable[0xC3] =
    (OpcodeEntry){.handler = JP, .mcycles = 4, .mnemonic = "JP u16"};

  opcodeTable[0xC5] = (OpcodeEntry){
    .handler = PUSH, .arg = &gb->BC, .mcycles = 4, .mnemonic = "PUSH BC"};

  opcodeTable[0xC9] =
    (OpcodeEntry){.handler = RET, .mcycles = 4, .mnemonic = "RET"};

  opcodeTable[0xCD] =
    (OpcodeEntry){.handler = CALL_u16, .mcycles = 6, .mnemonic = "CALL u16"};

  opcodeTable[0xE0] = (OpcodeEntry){.handler = LD_AddrPlusu8_R,
    .arg = &gb->A,
    .mcycles = 3,
    .mnemonic = "LD (FF00+u8),A"};

  opcodeTable[0xE2] = (OpcodeEntry){.handler = LD_Addr_R,
    .arg = &gb->A,
    .address = (uint16_t *)&gb->C,
    .mcycles = 2,
    .mnemonic = "LD (FF00+C),A"};

  opcodeTable[0xEA] = (OpcodeEntry){.handler = LD_u16Addr_R,
    .arg = &gb->A,
    .mcycles = 4,
    .mnemonic = "LD (u16),A"};

  opcodeTable[0xF0] = (OpcodeEntry){.handler = LD_R_AddrPlusu8,
    .arg = &gb->A,
    .mcycles = 3,
    .mnemonic = "LD A,(FF00+u8)"};

  opcodeTable[0xF3] = (OpcodeEntry){.handler = DissableInterrupts, 
    .mcycles = 1, .mnemonic ="DI"};

  opcodeTable[0xFE] =
    (OpcodeEntry){.handler = CP_i8, .mcycles = 2, .mnemonic = "CP A,i8"};

  opcodeTable[0xFF] =
    (OpcodeEntry){.handler = RST_38h, .mcycles = 4, .mnemonic = "RST 38h"};
}
