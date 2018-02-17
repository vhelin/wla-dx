@echo off

# create all the opcode decoding speedup tables

set WLA_QUIET=1

set WLA_TARGET=GB
make
start gen

if exist main.o del main.o
set WLA_TARGET=Z80
make 
start gen

if exist main.o del main.o
set WLA_TARGET=MCS6502
make 
start gen

if exist main.o del main.o
set WLA_TARGET=WDC65C02
make
start gen

if exist main.o del main.o
set WLA_TARGET=MCS6510
make 
start gen

if exist main.o del main.o
set WLA_TARGET=W65816
make
start gen

if exist main.o del main.o
set WLA_TARGET=SPC700
make 
start gen

if exist main.o del main.o
set WLA_TARGET=HUC6280
make 
start gen

if exist main.o del main.o

exit