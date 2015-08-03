#!/bin/sh

if [ $# -ne 1 ]
then
	echo " "
	echo "USAGE: ./unix.sh {JOBS}"
	echo "Where {JOBS} is the number of jobs (i.e., compiler instances) to run simultaneously."
	echo " "
	exit 1
fi

export CC=gcc
export LD=gcc

echo ""
echo "****************************************************************"
echo "*          creating opcode decoding speedup tables             *"
echo "****************************************************************"
echo ""

cd opcode_table_generator
./create_tables.sh
cd ..

echo ""
echo "****************************************************************"
echo "*                   compiling few tools                        *"
echo "****************************************************************"
echo ""

cd wlab
make --makefile=makefile.unix
cp wlab ../binaries
make  clean --makefile=makefile.unix
cd ..

cd wlad
make --makefile=makefile.unix
cp wlad ../binaries
make  clean --makefile=makefile.unix
cd ..

echo ""
echo "****************************************************************"
echo "*                  compiling WLA binaries                      *"
echo "****************************************************************"
echo ""

set WLA1=Z80
set WLA2=z80
make --makefile=makefile.unix
make clean --makefile=makefile.unix

export WLA1=SPC700
export WLA2=spc700
make --makefile=makefile.unix
make clean --makefile=makefile.unix

export WLA1=GB
export WLA2=gb
make --makefile=makefile.unix
make clean --makefile=makefile.unix


export WLA1=MCS6502
export WLA2=6502
make --makefile=makefile.unix
make clean --makefile=makefile.unix

export WLA1=WDC65C02
export WLA2=65c02
make --makefile=makefile.unix
make clean --makefile=makefile.unix

export WLA1=MCS6510
export WLA2=6510
make --makefile=makefile.unix
make clean --makefile=makefile.unix

export WLA1=W65816
export WLA2=65816
make --makefile=makefile.unix
make clean --makefile=makefile.unix

export WLA1=HUC6280
export WLA2=huc6280
make --makefile=makefile.unix
make clean --makefile=makefile.unix


echo ""
echo "****************************************************************"
echo "*                    compiling WLALINK                         *"
echo "****************************************************************"
echo ""

cd wlalink
make --makefile=makefile.unix
cp wlalink ../binaries
make clean --makefile=makefile.unix
cd ..

echo ""
echo "****************************************************************"
echo "*        all done! check ./binaries/ for the results           *"
echo "****************************************************************"
echo ""
