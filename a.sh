#!/usr/bin/env bash

regs=(
    "B"
    "C"
    "D"
    "E"
    "H"
    "L"
    "HL"
    "A"
)
#  struct {
#    uint8_t nbit;
#  } BitArgs;

opcodeStart=0x40
bitNum=0
for ((i=0x80; i<=0xBF; i++)); do
  isHL=$(( ($i & 0xF) == 0x6 || ($i & 0xF) == 0xE ))
  nbit=$(( ($i - 0x80) / 8))
  ((bitNum++))
  printf "CBOpcodeTable[0x%02X] = \
(OE){.handler = RES_R, .arg = &gb->%s, \
.BitArgs = {nbit = %u, .isHL = %u}};\n" $i "${regs[$(( $i % 8  ))]}" $nbit $isHL
done
