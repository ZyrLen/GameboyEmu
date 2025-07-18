#ifndef OPCODES_H
#define OPCODES_H

#include <gameboy.h>

// define public data structures / prototypes, macros etc.

// LD (800H), A. Loads value of A into memoery location 800H. () means address

uint8_t z; // Zero flag
uint8_t n; // Subtraction Flag (BCD)
uint8_t h; // Half Carry Flag (BCD)
uint8_t c; // Carry Flag

#endif
