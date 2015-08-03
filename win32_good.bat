@echo off

echo.
echo ****************************************************************
echo *          creating opcode decoding speedup tables             *
echo ****************************************************************
echo.

cd opcode_table_generator
start /WAIT create_tables
cd ..\


echo.
echo ****************************************************************
echo *                   compiling few tools                        *
echo ****************************************************************
echo.

cd wlab
make
copy wlab.exe ..\wlab.exe
cd ../

echo.
echo ****************************************************************
echo *                  compiling WLA binaries                      *
echo ****************************************************************
echo.

set WLA1=Z80
set WLA2=z80
make --makefile=makefile.win32
make clean --makefile=makefile.win32

set WLA1=SPC700
set WLA2=spc700
make --makefile=makefile.win32
make clean --makefile=makefile.win32

set WLA1=GB
set WLA2=gb
make --makefile=makefile.win32
make clean --makefile=makefile.win32


set WLA1=MCS6502
set WLA2=6502
make --makefile=makefile.win32
make clean --makefile=makefile.win32

set WLA1=WDC65C02
set WLA2=65c02
make --makefile=makefile.win32
make clean --makefile=makefile.win32

set WLA1=MCS6510
set WLA2=6510
make --makefile=makefile.win32
make clean --makefile=makefile.win32

set WLA1=W65816
set WLA2=65816
make --makefile=makefile.win32
make clean --makefile=makefile.win32

set WLA1=HUC6280
set WLA2=huc6280
make --makefile=makefile.win32
make clean --makefile=makefile.win32


echo.
echo ****************************************************************
echo *                    compiling WLALINK                         *
echo ****************************************************************
echo.


cd wlalink
make --makefile=makefile.win32
copy wlalink.exe ..\wlalink.exe
cd ..\


echo.
echo ****************************************************************
echo *        all done! check binaries\ for the results             *
echo ****************************************************************
echo.