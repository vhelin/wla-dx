#!/bin/sh

# create all the opcode decoding speedup tables

export WLA_QUIET=1

make  clean --makefile=makefile.unix
export WLA_TARGET=GB
make --makefile=makefile.unix  
./gen

make  clean --makefile=makefile.unix
export WLA_TARGET=Z80
make --makefile=makefile.unix
./gen

make  clean --makefile=makefile.unix
export WLA_TARGET=MCS6502
make --makefile=makefile.unix
./gen

make  clean --makefile=makefile.unix
export WLA_TARGET=WDC65C02
make --makefile=makefile.unix
./gen

make  clean --makefile=makefile.unix
export WLA_TARGET=MCS6510
make --makefile=makefile.unix
./gen

make  clean --makefile=makefile.unix
export WLA_TARGET=W65816
make --makefile=makefile.unix
./gen

make  clean --makefile=makefile.unix
export WLA_TARGET=SPC700
make --makefile=makefile.unix
./gen

make  clean --makefile=makefile.unix
export WLA_TARGET=HUC6280
make --makefile=makefile.unix
./gen

make  clean --makefile=makefile.unix