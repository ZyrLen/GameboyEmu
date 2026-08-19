#include "opcodes.h"
#include "gameboy.h"
#include <stdlib.h>
#define OE OpcodeEntry

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
  opcodeTable[0x0] = (OE){.handler = NOP, .mcycles = 1, .mnemonic = "NOP"};
  opcodeTable[0x1] = (OE){.handler = LD_RR_u16, .arg = &gb->BC, .mcycles = 3, .mnemonic = "LD BC, u16"};
  opcodeTable[0x2] = (OE){.handler = LD_Addr_R, .address = &gb->BC, 
                          .arg = &gb->A, .mcycles = 2, .mnemonic = "LD (BC), A"};
  opcodeTable[0x3] = (OE){.handler = INC_RR, .arg = &gb->BC, .mcycles = 2, .mnemonic = "INC BC"};
  opcodeTable[0x4] = (OE){ .handler = INC_R, .arg = &gb->B, .mcycles = 1, .mnemonic = "INC B"};
  opcodeTable[0x5] = (OE){.handler = DEC_R, .arg = &gb->B, .mcycles = 1, .mnemonic = "DEC B"};
  opcodeTable[0x6] = (OE){.handler = LD_R_u8, .arg = &gb->B, .mcycles = 2, .mnemonic = "LD B,u8"};
  // opcodeTable[0x7] = (OE){.handler = , .mcycles = , .mnemonic = ""};
  opcodeTable[0x8] = (OE){.handler = LD_u16Addr_RR, .arg = &gb->sp, .mcycles = 5, .mnemonic = "LD (u16), SP"};
  // opcodeTable[0x9] = (OE){.handler = , .mcycles = , .mnemonic = ""};
  // opcodeTable[0xA] = (OE){.handler = , .mcycles = , .mnemonic = ""};
  // opcodeTable[0xB] = (OE){.handler = , .mcycles = , .mnemonic = ""};
  opcodeTable[0xC] = (OE){.handler = INC_R, .arg = &gb->C, .mcycles = 1, .mnemonic = "INC C"};
  opcodeTable[0xD] = (OE){.handler = DEC_R, .arg = &gb->C, .mcycles = 1, .mnemonic = "DEC C"};
  opcodeTable[0xE] = (OE){.handler = LD_R_u8, .arg = &gb->C, .mcycles = 2, .mnemonic = "LD C,u8"};
  // opcodeTable[0xF] = (OE){.handler = , .mcycles = , .mnemonic = ""};

  // opcodeTable[0x10] = (OE){.handler = STOP, .mcycles = 1, .mnemonic = "STOP"};
  opcodeTable[0x11] = (OE){.handler = LD_RR_u16, .arg = &gb->DE, .mcycles = 3, .mnemonic = "LD DE,u16"};
  opcodeTable[0x12] = (OE){.handler = LD_Addr_R, .arg = &gb->A, .address = &gb->DE, .mcycles = 2,
    .mnemonic = "LD (DE),A"};
  opcodeTable[0x13] = (OE){.handler = INC_RR, .arg = &gb->DE, .mcycles = 2, .mnemonic = "INC DE"};
  opcodeTable[0x14] = (OE){.handler = INC_R, .arg = &gb->D, .mcycles = 1, .mnemonic = "INC D"};
  opcodeTable[0x15] = (OE){.handler = DEC_R, .arg = &gb->D, .mcycles = 1, .mnemonic = "DEC D"};
  opcodeTable[0x16] = (OE){.handler = LD_R_u8, .arg = &gb->D, .mcycles = 2, .mnemonic = "LD D,u8"};
  opcodeTable[0x17] = (OE){.handler = RLA, .mcycles = 1, .mnemonic = "RLA"};
  opcodeTable[0x18] = (OE){.handler = JR, .mcycles = 3, .mnemonic = "JR i8"};
  opcodeTable[0x19] = (OE){.handler = ADD_HL, .arg = &gb->DE, .mcycles = 2, .mnemonic = "ADD HL,DE"};
  opcodeTable[0x1A] = (OE){.handler = LD_R_Addr, .arg = &gb->A, .address = &gb->DE, .mcycles = 2,
    .mnemonic = "LD A,(DE)"};
  opcodeTable[0x1B] = (OE){.handler = DEC_RR, .arg = &gb->DE, .mcycles = 2, .mnemonic = "DEC DE"};
  opcodeTable[0x1C] = (OE){.handler = INC_R, .arg = &gb->E, .mcycles = 1, .mnemonic = "INC E"};
  opcodeTable[0x1D] = (OE){.handler = DEC_R, .arg = &gb->E, .mcycles = 1, .mnemonic = "DEC E"};
  opcodeTable[0x1E] = (OE){.handler = LD_R_u8, .arg = &gb->E, .mcycles = 2, .mnemonic = "LD E,u8"};
  opcodeTable[0x1F] = (OE){.handler = RRA, .mcycles = 1, .mnemonic = "RRA"};

  opcodeTable[0x20] = (OE){.handler = JR_NZ, .mcyclesTrue = 3, .mcycles = 2, .mnemonic = "JR NZ,i8", .arg = NULL};
  opcodeTable[0x21] = (OE){.handler = LD_RR_u16, .arg = &gb->HL, .mcycles = 3, .mnemonic = "LD HL,u16"};
  opcodeTable[0x22] = (OE){.handler = LD_HLplus_A, .mcycles = 2, .mnemonic = "LD (HL+),A"};
  opcodeTable[0x23] = (OE){.handler = INC_RR, .arg = &gb->HL, .mcycles = 2, .mnemonic = "INC HL"};
  opcodeTable[0x24] = (OE){.handler = INC_R, .arg = &gb->H, .mcycles = 1, .mnemonic = "INC H"};
  opcodeTable[0x25] = (OE){.handler = DEC_R, .arg = &gb->H, .mcycles = 1, .mnemonic = "DEC H"};
  opcodeTable[0x26] = (OE){.handler = LD_R_u8, .arg = &gb->H, .mcycles = 2, .mnemonic = "LD H,u8"};
  opcodeTable[0x27] = (OE){.handler = DAA, .mcycles = 1, .mnemonic = "DAA"};
  opcodeTable[0x28] = (OE){.handler = JR_Z, .mcycles = 2, .mcyclesTrue = 3, .mnemonic = "JR Z,i8"};
  // opcodeTable[0x29] = (OE){};
	opcodeTable[0x2A] = (OE){.handler = LD_A_HLplus, .mcycles = 2, .mnemonic = "LD A,(HL+)"};
  // opcodeTable[0x2B] = (OE){};
  opcodeTable[0x2C] = (OE){.handler = INC_R, .arg = &gb->L,.mcycles = 1, .mnemonic = "INC L"};
  opcodeTable[0x2D] = (OE){.handler = DEC_R, .arg = &gb->L, .mcycles = 1, .mnemonic = "DEC L"};
  opcodeTable[0x2E] = (OE){.handler = LD_R_u8, .arg = &gb->L, .mcycles = 2, .mnemonic = "LD L,u8"};
  // opcodeTable[0x2F] = (OE){};

  opcodeTable[0x30] = (OE){.handler = JR_NC, .mcycles = 2, .mcyclesTrue = 3, .mnemonic = "JR NC,i8"};
  opcodeTable[0x31] = (OE){.handler = LD_SP_u16, .arg = &gb->sp, .mcycles = 3, .mnemonic = "LD SP,u16"};
  opcodeTable[0x32] = (OE){.handler = LD_HLminus_A, .mcycles = 2, .mnemonic = "LD (HL-),A"};
  opcodeTable[0x33] = (OE){.handler = INC_RR, .mcycles = 2, .mnemonic = "INC SP"};
  opcodeTable[0x34] = (OE){.handler = INC_HL, .mcycles = 3, .mnemonic = "INC (HL)"};
  opcodeTable[0x35] = (OE){.handler = DEC_HL, .mcycles = 3, .mnemonic = "DEC (HL)"};
  opcodeTable[0x36] = (OE){.handler = LD_HL_u8, .mcycles = 3, .mnemonic = "DEC (HL)"};
  opcodeTable[0x37] = (OE){.handler = SCF, .mcycles = 1, .mnemonic = "SCF"};
  opcodeTable[0x38] = (OE){.handler = ADD_HL, .arg = &gb->sp, .mcycles = 2, .mnemonic = "ADD HL,SP"};
  opcodeTable[0x39] = (OE){.handler = LD_A_HLminus, .mcycles = 2, .mnemonic = "LD A,(HL-)"};
  opcodeTable[0x3A] = (OE){.handler = LD_A_HLminus, .mcycles = 2, .mnemonic = "LD A,(HL-)"};
  opcodeTable[0x3B] = (OE){.handler = DEC_RR, .arg = &gb->sp, .mcycles = 2, .mnemonic = "DEC SP"};
  opcodeTable[0x3C] = (OE){.handler = INC_R, .arg = &gb->A, .mcycles = 1, .mnemonic = "INC A"};
  opcodeTable[0x3D] = (OE){.handler = DEC_R, .arg = &gb->A, .mcycles = 1, .mnemonic = "DEC A"};
  opcodeTable[0x3E] = (OE){.handler = LD_R_u8, .arg = &gb->A, .mcycles = 2, .mnemonic = "LD A,u8"};
  opcodeTable[0x3F] = (OE){.handler = CCF, .mcycles = 1, .mnemonic = "CCF"};

	opcodeTable[0x40] = (OE){.handler = LD_R_R, .R1 = &gb->B, .R2 = &gb->B, .mcycles = 1, .mnemonic = "LD B,B"};
	opcodeTable[0x41] = (OE){.handler = LD_R_R, .R1 = &gb->B, .R2 = &gb->C, .mcycles = 1, .mnemonic = "LD B,C"};
	opcodeTable[0x42] = (OE){.handler = LD_R_R, .R1 = &gb->B, .R2 = &gb->D, .mcycles = 1, .mnemonic = "LD B,D"};
	opcodeTable[0x43] = (OE){.handler = LD_R_R, .R1 = &gb->B, .R2 = &gb->E, .mcycles = 1, .mnemonic = "LD B,E"};
	opcodeTable[0x44] = (OE){.handler = LD_R_R, .R1 = &gb->B, .R2 = &gb->H, .mcycles = 1, .mnemonic = "LD B,H"};
	opcodeTable[0x45] = (OE){.handler = LD_R_R, .R1 = &gb->B, .R2 = &gb->L, .mcycles = 1, .mnemonic = "LD B,L"};
	opcodeTable[0x46] = (OE){.handler = LD_R_Addr, .arg = &gb->B, .address = &gb->HL, .mcycles = 1, 
                           .mnemonic = "LD B,(HL)"};
	opcodeTable[0x47] = (OE){.handler = LD_R_R, .R1 = &gb->B, .R2 = &gb->A, .mcycles = 1, .mnemonic = "LD B,A"};
	opcodeTable[0x48] = (OE){.handler = LD_R_R, .R1 = &gb->C, .R2 = &gb->B, .mcycles = 1, .mnemonic = "LD C,B"};
	opcodeTable[0x49] = (OE){.handler = LD_R_R, .R1 = &gb->C, .R2 = &gb->C, .mcycles = 1, .mnemonic = "LD C,C"};
	opcodeTable[0x4A] = (OE){.handler = LD_R_R, .R1 = &gb->C, .R2 = &gb->D, .mcycles = 1, .mnemonic = "LD C,D"};
	opcodeTable[0x4B] = (OE){.handler = LD_R_R, .R1 = &gb->C, .R2 = &gb->E, .mcycles = 1, .mnemonic = "LD C,E"};
	opcodeTable[0x4C] = (OE){.handler = LD_R_R, .R1 = &gb->C, .R2 = &gb->H, .mcycles = 1, .mnemonic = "LD C,H"};
	opcodeTable[0x4D] = (OE){.handler = LD_R_R, .R1 = &gb->C, .R2 = &gb->L, .mcycles = 1, .mnemonic = "LD C,L"};
	opcodeTable[0x4E] = (OE){.handler = LD_R_Addr, .arg = &gb->C, .address = &gb->HL, .mcycles = 1, 
                           .mnemonic = "LD C,(HL)"};
	opcodeTable[0x4F] = (OE){.handler = LD_R_R, .R1 = &gb->C, .R2 = &gb->A, .mcycles = 1, .mnemonic = "LD C,A"};

	opcodeTable[0x50] = (OE){.handler = LD_R_R, .R1 = &gb->D, .R2 = &gb->B, .mcycles = 1, .mnemonic = "LD D,B"};
	opcodeTable[0x51] = (OE){.handler = LD_R_R, .R1 = &gb->D, .R2 = &gb->C, .mcycles = 1, .mnemonic = "LD D,C"};
	opcodeTable[0x52] = (OE){.handler = LD_R_R, .R1 = &gb->D, .R2 = &gb->D, .mcycles = 1, .mnemonic = "LD D,D"};
	opcodeTable[0x53] = (OE){.handler = LD_R_R, .R1 = &gb->D, .R2 = &gb->E, .mcycles = 1, .mnemonic = "LD D,E"};
	opcodeTable[0x54] = (OE){.handler = LD_R_R, .R1 = &gb->D, .R2 = &gb->H, .mcycles = 1, .mnemonic = "LD D,H"};
	opcodeTable[0x55] = (OE){.handler = LD_R_R, .R1 = &gb->D, .R2 = &gb->L, .mcycles = 1, .mnemonic = "LD D,L"};
	opcodeTable[0x56] = (OE){.handler = LD_R_Addr, .arg = &gb->D, .address = &gb->HL, .mcycles = 1, 
                           .mnemonic = "LD D,(HL)"};
	opcodeTable[0x57] = (OE){.handler = LD_R_R, .R1 = &gb->D, .R2 = &gb->A, .mcycles = 1, .mnemonic = "LD D,A"};
	opcodeTable[0x58] = (OE){.handler = LD_R_R, .R1 = &gb->E, .R2 = &gb->B, .mcycles = 1, .mnemonic = "LD E,B"};
	opcodeTable[0x59] = (OE){.handler = LD_R_R, .R1 = &gb->E, .R2 = &gb->C, .mcycles = 1, .mnemonic = "LD E,C"};
	opcodeTable[0x5A] = (OE){.handler = LD_R_R, .R1 = &gb->E, .R2 = &gb->D, .mcycles = 1, .mnemonic = "LD E,D"};
	opcodeTable[0x5B] = (OE){.handler = LD_R_R, .R1 = &gb->E, .R2 = &gb->E, .mcycles = 1, .mnemonic = "LD E,E"};
	opcodeTable[0x5C] = (OE){.handler = LD_R_R, .R1 = &gb->E, .R2 = &gb->H, .mcycles = 1, .mnemonic = "LD E,H"};
	opcodeTable[0x5D] = (OE){.handler = LD_R_R, .R1 = &gb->E, .R2 = &gb->L, .mcycles = 1, .mnemonic = "LD E,L"};
	opcodeTable[0x5E] = (OE){.handler = LD_R_Addr, .arg = &gb->E, .address = &gb->HL, .mcycles = 1, 
                           .mnemonic = "LD E,(HL)"};
	opcodeTable[0x5F] = (OE){.handler = LD_R_R, .R1 = &gb->E, .R2 = &gb->A, .mcycles = 1, .mnemonic = "LD E,A"};

	opcodeTable[0x60] = (OE){.handler = LD_R_R, .R1 = &gb->H, .R2 = &gb->B, .mcycles = 1, .mnemonic = "LD H,B"};
	opcodeTable[0x61] = (OE){.handler = LD_R_R, .R1 = &gb->H, .R2 = &gb->C, .mcycles = 1, .mnemonic = "LD H,C"};
	opcodeTable[0x62] = (OE){.handler = LD_R_R, .R1 = &gb->H, .R2 = &gb->D, .mcycles = 1, .mnemonic = "LD H,D"};
	opcodeTable[0x63] = (OE){.handler = LD_R_R, .R1 = &gb->H, .R2 = &gb->E, .mcycles = 1, .mnemonic = "LD H,E"};
	opcodeTable[0x64] = (OE){.handler = LD_R_R, .R1 = &gb->H, .R2 = &gb->H, .mcycles = 1, .mnemonic = "LD H,H"};
	opcodeTable[0x65] = (OE){.handler = LD_R_R, .R1 = &gb->H, .R2 = &gb->L, .mcycles = 1, .mnemonic = "LD H,L"};
	opcodeTable[0x66] = (OE){.handler = LD_R_Addr, .arg = &gb->H, .address = &gb->HL, .mcycles = 1, 
                           .mnemonic = "LD H,(HL)"};
	opcodeTable[0x67] = (OE){.handler = LD_R_R, .R1 = &gb->H, .R2 = &gb->A, .mcycles = 1, .mnemonic = "LD H,A"};
	opcodeTable[0x68] = (OE){.handler = LD_R_R, .R1 = &gb->L, .R2 = &gb->B, .mcycles = 1, .mnemonic = "LD L,B"};
	opcodeTable[0x69] = (OE){.handler = LD_R_R, .R1 = &gb->L, .R2 = &gb->C, .mcycles = 1, .mnemonic = "LD L,C"};
	opcodeTable[0x6A] = (OE){.handler = LD_R_R, .R1 = &gb->L, .R2 = &gb->D, .mcycles = 1, .mnemonic = "LD L,D"};
	opcodeTable[0x6B] = (OE){.handler = LD_R_R, .R1 = &gb->L, .R2 = &gb->E, .mcycles = 1, .mnemonic = "LD L,E"};
	opcodeTable[0x6C] = (OE){.handler = LD_R_R, .R1 = &gb->L, .R2 = &gb->H, .mcycles = 1, .mnemonic = "LD L,H"};
	opcodeTable[0x6D] = (OE){.handler = LD_R_R, .R1 = &gb->L, .R2 = &gb->L, .mcycles = 1, .mnemonic = "LD L,L"};
	opcodeTable[0x6E] = (OE){.handler = LD_R_Addr, .arg = &gb->L, .address = &gb->HL, .mcycles = 1, 
                           .mnemonic = "LD L,(HL)"};
	opcodeTable[0x6F] = (OE){.handler = LD_R_R, .R1 = &gb->L, .R2 = &gb->A, .mcycles = 1, .mnemonic = "LD L,A"};

  opcodeTable[0x70] = (OE){.handler = LD_Addr_R, .arg = &gb->B, .address = &gb->HL, .mcycles = 2, 
                           .mnemonic = "LD (HL),B"};
  opcodeTable[0x71] = (OE){.handler = LD_Addr_R, .arg = &gb->C, .address = &gb->HL, .mcycles = 2, 
                           .mnemonic = "LD (HL),C"};
  opcodeTable[0x72] = (OE){.handler = LD_Addr_R, .arg = &gb->D, .address = &gb->HL, .mcycles = 2, 
                           .mnemonic = "LD (HL),D"};
  opcodeTable[0x73] = (OE){.handler = LD_Addr_R, .arg = &gb->E, .address = &gb->HL, .mcycles = 2, 
                           .mnemonic = "LD (HL),E"};
  opcodeTable[0x74] = (OE){.handler = LD_Addr_R, .arg = &gb->H, .address = &gb->HL, .mcycles = 2, 
                           .mnemonic = "LD (HL),H"};
  opcodeTable[0x75] = (OE){.handler = LD_Addr_R, .arg = &gb->L, .address = &gb->HL, .mcycles = 2, 
                           .mnemonic = "LD (HL),L"};
  opcodeTable[0x76] =(OE){.handler = HALT, .mcycles = 1, .mnemonic = "HALT"};
  opcodeTable[0x77] = (OE){.handler = LD_Addr_R, .arg = &gb->A, .address = &gb->HL,.mcycles = 2, 
                           .mnemonic = "LD (HL),A"};
  opcodeTable[0x78] = (OE){.handler = LD_R_R, .R1 = &gb->A, .R2 = &gb->B, .mcycles = 1, .mnemonic = "LD A,B"};
  opcodeTable[0x79] = (OE){.handler = LD_R_R, .R1 = &gb->A, .R2 = &gb->C, .mcycles = 1, .mnemonic = "LD A,C"};
  opcodeTable[0x7A] = (OE){.handler = LD_R_R, .R1 = &gb->A, .R2 = &gb->D, .mcycles = 1, .mnemonic = "LD A,D"};
  opcodeTable[0x7B] = (OE){.handler = LD_R_R, .R1 = &gb->A, .R2 = &gb->E, .mcycles = 1, .mnemonic = "LD A,E"};
  opcodeTable[0x7C] = (OE){.handler = LD_R_R, .R1 = &gb->A, .R2 = &gb->H, .mcycles = 1, .mnemonic = "LD A,H"};
  opcodeTable[0x7D] = (OE){.handler = LD_R_R, .R1 = &gb->A, .R2 = &gb->L, .mcycles = 1, .mnemonic = "LD A,L"};
  opcodeTable[0x7E] = (OE){.handler = LD_R_Addr, .arg = &gb->A, .address = &gb->HL, .mcycles = 2, 
                           .mnemonic = "LD A,(HL)"};
  opcodeTable[0x7F] = (OE){.handler = LD_R_R, .R1 = &gb->A, .R2 = &gb->A, .mcycles = 1, .mnemonic = "LD A,A"};

	opcodeTable[0x80] = (OE){.handler = ADD, .arg = &gb->B,	.mcycles = 1, .mnemonic = "ADD A,B"};
	opcodeTable[0x81] = (OE){.handler = ADD, .arg = &gb->C, 	.mcycles = 1, .mnemonic = "ADD A,C"};
	opcodeTable[0x82] = (OE){.handler = ADD, .arg = &gb->D, 	.mcycles = 1, .mnemonic = "ADD A,D"};
	opcodeTable[0x83] = (OE){.handler = ADD, .arg = &gb->E, 	.mcycles = 1, .mnemonic = "ADD A,E"};
	opcodeTable[0x84] = (OE){.handler = ADD, .arg = &gb->H, 	.mcycles = 1, .mnemonic = "ADD A,H"};
	opcodeTable[0x85] = (OE){.handler = ADD, .arg = &gb->L, 	.mcycles = 1, .mnemonic = "ADD A,L"};
	opcodeTable[0x86] = (OE){.handler = ADD_HL, .arg = &gb->HL,	.mcycles = 2, .mnemonic = "ADD A,(HL)"};
	opcodeTable[0x87] = (OE){.handler = ADD, .arg = &gb->A, 	.mcycles = 1, .mnemonic = "ADD A,A"};
	opcodeTable[0x88] = (OE){.handler = ADC, .arg = &gb->B, 	.mcycles = 1, .mnemonic = "ADC A,B"};
	opcodeTable[0x89] = (OE){.handler = ADC, .arg = &gb->C, 	.mcycles = 1, .mnemonic = "ADC A,C"};
	opcodeTable[0x8A] = (OE){.handler = ADC, .arg = &gb->D, 	.mcycles = 1, .mnemonic = "ADC A,D"};
	opcodeTable[0x8B] = (OE){.handler = ADC, .arg = &gb->E, 	.mcycles = 1, .mnemonic = "ADC A,E"};
	opcodeTable[0x8C] = (OE){.handler = ADC, .arg = &gb->H, 	.mcycles = 1, .mnemonic = "ADC A,H"};
	opcodeTable[0x8D] = (OE){.handler = ADC, .arg = &gb->L, 	.mcycles = 1, .mnemonic = "ADC A,L"};
	opcodeTable[0x8E] = (OE){.handler = ADC_HL, .arg = &gb->HL,	.mcycles = 2, .mnemonic = "ADC A,(HL)"};
	opcodeTable[0x8F] = (OE){.handler = ADC, .arg = &gb->A, 	.mcycles = 1, .mnemonic = "ADC A,A"};

	opcodeTable[0x90] = (OE){.handler = SUB, .arg = &gb->B,	.mcycles = 1, .mnemonic = "SUB B"};
	opcodeTable[0x91] = (OE){.handler = SUB, .arg = &gb->C, 	.mcycles = 1, .mnemonic = "SUB C"};
	opcodeTable[0x92] = (OE){.handler = SUB, .arg = &gb->D, 	.mcycles = 1, .mnemonic = "SUB D"};
	opcodeTable[0x93] = (OE){.handler = SUB, .arg = &gb->E, 	.mcycles = 1, .mnemonic = "SUB E"};
	opcodeTable[0x94] = (OE){.handler = SUB, .arg = &gb->H, 	.mcycles = 1, .mnemonic = "SUB H"};
	opcodeTable[0x95] = (OE){.handler = SUB, .arg = &gb->L, 	.mcycles = 1, .mnemonic = "SUB L"};
	opcodeTable[0x96] = (OE){.handler = SUB_HL, .arg = &gb->HL,	.mcycles = 2, .mnemonic = "SUB (HL)"};
	opcodeTable[0x97] = (OE){.handler = SUB, .arg = &gb->A, 	.mcycles = 1, .mnemonic = "SUB A"};
	opcodeTable[0x98] = (OE){.handler = SBC, .arg = &gb->B, 	.mcycles = 1, .mnemonic = "SBC A,B"};
	opcodeTable[0x99] = (OE){.handler = SBC, .arg = &gb->C, 	.mcycles = 1, .mnemonic = "SBC A,C"};
	opcodeTable[0x9A] = (OE){.handler = SBC, .arg = &gb->D, 	.mcycles = 1, .mnemonic = "SBC A,D"};
	opcodeTable[0x9B] = (OE){.handler = SBC, .arg = &gb->E, 	.mcycles = 1, .mnemonic = "SBC A,E"};
	opcodeTable[0x9C] = (OE){.handler = SBC, .arg = &gb->H, 	.mcycles = 1, .mnemonic = "SBC A,H"};
	opcodeTable[0x9D] = (OE){.handler = SBC, .arg = &gb->L, 	.mcycles = 1, .mnemonic = "SBC A,L"};
	opcodeTable[0x9E] = (OE){.handler = SBC_HL, .arg = &gb->HL,	.mcycles = 2, .mnemonic = "SBC A,(HL)"};
	opcodeTable[0x9F] = (OE){.handler = SBC, .arg = &gb->A, 	.mcycles = 1, .mnemonic = "SBC A,A"};

	opcodeTable[0xA0] = (OE){.handler = AND, .arg = &gb->B,  .mcycles = 1, .mnemonic = "AND B"};
	opcodeTable[0xA1] = (OE){.handler = AND, .arg = &gb->C,  .mcycles = 1, .mnemonic = "AND C"};
	opcodeTable[0xA2] = (OE){.handler = AND, .arg = &gb->D,  .mcycles = 1, .mnemonic = "AND D"};
	opcodeTable[0xA3] = (OE){.handler = AND, .arg = &gb->E,  .mcycles = 1, .mnemonic = "AND E"};
	opcodeTable[0xA4] = (OE){.handler = AND, .arg = &gb->H,  .mcycles = 1, .mnemonic = "AND H"};
	opcodeTable[0xA5] = (OE){.handler = AND, .arg = &gb->L,  .mcycles = 1, .mnemonic = "AND L"};
	opcodeTable[0xA6] = (OE){.handler = AND, .arg = &gb->HL, .mcycles = 2, .mnemonic = "AND (HL)"};
	opcodeTable[0xA7] = (OE){.handler = AND, .arg = &gb->A,  .mcycles = 1, .mnemonic = "AND A"};
	opcodeTable[0xA8] = (OE){.handler = XOR, .arg = &gb->B,  .mcycles = 1, .mnemonic = "XOR B"};
	opcodeTable[0xA9] = (OE){.handler = XOR, .arg = &gb->C,  .mcycles = 1, .mnemonic = "XOR C"};
	opcodeTable[0xAA] = (OE){.handler = XOR, .arg = &gb->D,  .mcycles = 1, .mnemonic = "XOR D"};
	opcodeTable[0xAB] = (OE){.handler = XOR, .arg = &gb->E,  .mcycles = 1, .mnemonic = "XOR E"};
	opcodeTable[0xAC] = (OE){.handler = XOR, .arg = &gb->H,  .mcycles = 1, .mnemonic = "XOR H"};
	opcodeTable[0xAD] = (OE){.handler = XOR, .arg = &gb->L,  .mcycles = 1, .mnemonic = "XOR L"};
	opcodeTable[0xAE] = (OE){.handler = XOR_HL, .arg = &gb->HL, .mcycles = 2, .mnemonic = "XOR (HL)"};
	opcodeTable[0xAF] = (OE){.handler = XOR, .arg = &gb->A,  .mcycles = 1, .mnemonic = "XOR A"};

  opcodeTable[0xB0] = (OE){.handler = OR, .arg = &gb->B,  .mnemonic = "OR B"};
  opcodeTable[0xB1] = (OE){.handler = OR, .arg = &gb->C,  .mnemonic = "OR C"};
  opcodeTable[0xB2] = (OE){.handler = OR, .arg = &gb->D,  .mnemonic = "OR D"};
  opcodeTable[0xB3] = (OE){.handler = OR, .arg = &gb->E,  .mnemonic = "OR E"};
  opcodeTable[0xB4] = (OE){.handler = OR, .arg = &gb->H,  .mnemonic = "OR H"};
  opcodeTable[0xB5] = (OE){.handler = OR, .arg = &gb->L,  .mnemonic = "OR L"};
  opcodeTable[0xB6] = (OE){.handler = OR, .arg = &gb->HL, .mnemonic = "OR (HL)"};
  opcodeTable[0xB7] = (OE){.handler = OR, .arg = &gb->A,  .mnemonic = "OR A"};
  opcodeTable[0xB8] = (OE){.handler = CP, .arg = &gb->B,  .mnemonic = "CP B"};
  opcodeTable[0xB9] = (OE){.handler = CP, .arg = &gb->C,  .mnemonic = "CP C"};
  opcodeTable[0xBA] = (OE){.handler = CP, .arg = &gb->D,  .mnemonic = "CP D"};
  opcodeTable[0xBB] = (OE){.handler = CP, .arg = &gb->E,  .mnemonic = "CP E"};
  opcodeTable[0xBC] = (OE){.handler = CP, .arg = &gb->H,  .mnemonic = "CP H"};
  opcodeTable[0xBD] = (OE){.handler = CP, .arg = &gb->L,  .mnemonic = "CP L"};
  opcodeTable[0xBE] = (OE){.handler = CP, .arg = &gb->HL, .mnemonic = "CP (HL)"};
  opcodeTable[0xBF] = (OE){.handler = CP, .arg = &gb->A,  .mnemonic = "CP A"};

  // opcodeTable[0xC0] = (OE){.handler = RET_NZ
  opcodeTable[0xC1] = (OE){.handler = POP, .arg = &gb->BC, .mcycles = 3, .mnemonic = "POP BC"};
  opcodeTable[0xC2] = (OE){.handler = JP_NZ, .mcycles = 3, .mcyclesTrue = 4, .mnemonic = "JP NZ,u16"};
  opcodeTable[0xC3] =(OE){.handler = JP, .mcycles = 4, .mnemonic = "JP u16"};
  opcodeTable[0xC4] = (OE){.handler = CALL_NZ_u16, .mcycles = 3,.mcyclesTrue = 6, .mnemonic = "CALL NZ,u16"};
  opcodeTable[0xC5] = (OE){.handler = PUSH, .arg = &gb->BC, .mcycles = 4, .mnemonic = "PUSH BC"};
  opcodeTable[0xC6] = (OE){.handler = ADD_u8, .mcycles = 2, .mnemonic = "ADD A,u8"};
  opcodeTable[0xC9] =(OE){.handler = RET, .mcycles = 4, .mnemonic = "RET"};
  opcodeTable[0xCA] =(OE){.handler = JP_Z, .mcycles = 3, .mcyclesTrue = 4, .mnemonic = "JP Z,u16"};
  opcodeTable[0xCD] =(OE){.handler = CALL_u16, .mcycles = 6, .mnemonic = "CALL u16"};
  opcodeTable[0xCE] =(OE){.handler = ADC_u8, .mcycles = 2, .mnemonic = "ADC A,u8"};

  opcodeTable[0xD1] = (OE){.handler = POP, .arg = &gb->DE, .mcycles = 3, .mnemonic = "POP DE"};
  opcodeTable[0xD5] = (OE){.handler = PUSH, 
    .arg = &gb->DE, .mcycles = 4, .mnemonic = "PUSH DE"};

  opcodeTable[0xD6] = (OE){.handler = SUB_u8, 
    .mcycles = 2, .mnemonic = "SUB A,u8"};

  opcodeTable[0xE0] = (OE){.handler = LD_AddrPlusu8_R,
    .arg = &gb->A, .mcycles = 3, .mnemonic = "LD (FF00+u8),A"};

  opcodeTable[0xE1] = (OE){.handler = POP,
    .arg = &gb->HL,
    .mcycles = 3,
    .mnemonic = "POP HL"};

  opcodeTable[0xE2] = (OE){.handler = LD_Addr_R,
    .arg = &gb->A,
    .address = (uint16_t *)&gb->C,
    .mcycles = 2,
    .mnemonic = "LD (FF00+C),A"};

  opcodeTable[0xE5] = (OE){.handler = PUSH,
    .arg = &gb->HL,
    .mcycles = 4,
    .mnemonic = "PUSH HL"};

  opcodeTable[0xE6] = (OE){.handler = AND,
    .mcycles = 2, .mnemonic = "AND A,u8"};

  opcodeTable[0xEA] = (OE){.handler = LD_u16Addr_R,
    .arg = &gb->A,
    .mcycles = 4,
    .mnemonic = "LD (u16),A"};

  opcodeTable[0xEE] = (OE){.handler = XOR_u8, .mcycles = 2, .mnemonic = "XOR A,u8"};

  opcodeTable[0xF0] = (OE){.handler = LD_R_AddrPlusu8,
    .arg = &gb->A,
    .mcycles = 3,
    .mnemonic = "LD A,(FF00+u8)"};

  opcodeTable[0xF1] = (OE){.handler = POP,
    .arg = &gb->AF,
    .mcycles = 3,
    .mnemonic = "POP AF"};

  opcodeTable[0xF3] = (OE){.handler = DissableInterrupts, 
    .mcycles = 1, .mnemonic ="DI"};

  opcodeTable[0xF5] = (OE){.handler = PUSH,
    .arg = &gb->AF,
    .mcycles = 4,
    .mnemonic = "PUSH AF"};

  opcodeTable[0xFA] =(OE){.handler = LD_A_Addr,
    .mcycles = 4, .mnemonic = "LD A,(u16)"};

  opcodeTable[0xFE] =
    (OE){.handler = CP_i8, .mcycles = 2, .mnemonic = "CP A,i8"};

  opcodeTable[0xFF] =
    (OE){.handler = RST_38h, .mcycles = 4, .mnemonic = "RST 38h"};
}
