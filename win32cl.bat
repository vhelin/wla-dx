
cd wlab
copy makefile.win32cl makefile
nmake clean
nmake
move wlab.exe ..\binaries
nmake clean
cd ..

cd wlad
copy makefile.win32cl makefile
nmake clean
nmake
move wlad.exe ..\binaries
nmake clean
cd ..

copy makefiles\makefile.win32cl.gb MAKEFILE
nmake clean
nmake
move wla-gb.exe binaries\
nmake clean

copy makefiles\makefile.win32cl.z80 MAKEFILE
nmake
move wla-z80.exe binaries\
nmake clean

copy makefiles\makefile.win32cl.6502 MAKEFILE
nmake
move wla-6502.exe binaries\
nmake clean

copy makefiles\makefile.win32cl.65c02 MAKEFILE
nmake
move wla-65c02.exe binaries\
nmake clean

copy makefiles\makefile.win32cl.6510 MAKEFILE
nmake
move wla-6510.exe binaries\
nmake clean

copy makefiles\makefile.win32cl.65816 MAKEFILE
nmake
move wla-65816.exe binaries\
nmake clean

copy makefiles\makefile.win32cl.spc700 MAKEFILE
nmake
move wla-spc700.exe binaries\
nmake clean

copy makefiles\makefile.win32cl.huc6280 MAKEFILE
nmake
move wla-huc6280.exe binaries\
nmake clean

cd wlalink
copy makefile.win32cl makefile
nmake clean
nmake
move wlalink.exe ..\binaries
nmake clean
cd ..
