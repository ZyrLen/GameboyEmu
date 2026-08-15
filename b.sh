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
handler=(SLA SRA)
#  struct {
#    uint8_t nbit;
#  } BitArgs;

opcodeStart=0x20
bitNum=0
for ((i=opcodeStart; i<=opcodeStart + 0x0F; i++)); do
  isHL=$(( ($i & 0xF) == 0x6 || ($i & 0xF) == 0xE ))
  nbit=$(( ($i - opcodeStart) / 8))
  ((bitNum++))
  printf "CBOpcodeTable[0x%02X] = (OE){.handler = %s, .arg = &gb->%s};\n" $i "${handler[$(( $nbit ))]}" "${regs[$(( $i % 8 ))]}"
done
