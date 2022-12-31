
cd wlab
copy makefile.msdos makefile
make clean
make
move wlab.exe ..\binaries
make clean
cd ..

cd wlad
copy makefile.msdos makefile
make clean
make
move wlad.exe ..\binaries
make clean
cd ..

copy makefiles\makefile.msdos.gb MAKEFILE
make clean
make
move w-gb.exe binaries\
make clean

copy makefiles\makefile.msdos.superfx MAKEFILE
make clean
make
move w-superfx.exe binaries\
make clean

copy makefiles\makefile.msdos.z80 MAKEFILE
make
move w-z80.exe binaries\
make clean

copy makefiles\makefile.msdos.6502 MAKEFILE
make
move w-6502.exe binaries\
make clean

copy makefiles\makefile.msdos.65c02 MAKEFILE
make
move w-65c02.exe binaries\
make clean

copy makefiles\makefile.msdos.65ce02 MAKEFILE
make
move w-65ce02.exe binaries\
make clean

copy makefiles\makefile.msdos.68000 MAKEFILE
make
move w-68000.exe binaries\
make clean

copy makefiles\makefile.msdos.6800 MAKEFILE
make
move w-6800.exe binaries\
make clean

copy makefiles\makefile.msdos.6801 MAKEFILE
make
move w-6801.exe binaries\
make clean

copy makefiles\makefile.msdos.6809 MAKEFILE
make
move w-6809.exe binaries\
make clean

copy makefiles\makefile.msdos.8008 MAKEFILE
make
move w-8008.exe binaries\
make clean

copy makefiles\makefile.msdos.8080 MAKEFILE
make
move w-8080.exe binaries\
make clean

copy makefiles\makefile.msdos.65816 MAKEFILE
make
move w-65816.exe binaries\
make clean

copy makefiles\makefile.msdos.spc700 MAKEFILE
make
move w-spc700.exe binaries\
make clean

copy makefiles\makefile.msdos.huc6280 MAKEFILE
make
move w-huc6280.exe binaries\
make clean

cd wlalink
copy makefile.msdos MAKEFILE
make clean
make
move wlalink.exe ..\binaries
make clean
cd ..
