/***  includes  ***/

#include "CBopcodes.h"
#include "gameboy.h"
#include "opcodes.h"
#include <SDL2/SDL.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/***  defines ***/

// resolution is 160 x 144

Gameboy gb;
MMU mmu;

uint8_t opcodeTable[256];
uint8_t CBopcodeTable[256];

/***  file i/o ***/

/***  fetch/decode/handle key  ***/

/***  init  ***/

/***  main  ***/

int main() {
  // int argc, char **argv
  gb.A = 0;
  gb.B = 255;

  printf("A = %d\tB = %d\n", gb.A, gb.B);
  LD_Reg(&gb.A, &gb.B);
  printf("A = %d\tB = %d\n", gb.A, gb.B);
  printf("AF = %d\tBC = %d\n", gb.AF, gb.BC);

  return 0;
}