@echo off

 
set CC=cl
set LD=link

echo ""
echo "****************************************************************"
echo "*          creating opcode decoding speedup tables             *"
echo "****************************************************************"
echo ""

cd opcode_table_generator
copy create_tables.win32cl create_tables.bat
copy makefile.win32cl makefile
call .\create_tables.bat
cd ..

echo ""
echo "****************************************************************"
echo "*                   compiling few tools                        *"
echo "****************************************************************"
echo ""

cd wlab
copy makefile.win32cl makefile
nmake 
copy wlab.exe ..\binaries
nmake clean
cd ..

cd wlad
copy makefile.win32cl makefile
nmake 
copy wlad.exe ..\binaries
nmake clean
cd ..

echo ""
echo "****************************************************************"
echo "*                  compiling WLA binaries                      *"
echo "****************************************************************"
echo ""

nmake clean
copy makefiles\makefile.win32cl.gb makefile
nmake 
copy wla-gb.exe binaries

nmake clean
copy makefiles\makefile.win32cl.z80 makefile
nmake 
copy wla-z80.exe binaries

nmake clean
copy makefiles\makefile.win32cl.6502 makefile
nmake 
copy wla-6502.exe binaries

nmake clean
copy makefiles\makefile.win32cl.65c02 makefile
nmake 
copy wla-65c02.exe binaries

nmake clean
copy makefiles\makefile.win32cl.6510 makefile
nmake 
copy wla-6510.exe binaries

nmake clean
copy makefiles\makefile.win32cl.65816 makefile
nmake 
copy wla-65816.exe binaries

nmake clean
copy makefiles\makefile.win32cl.spc700 makefile
nmake 
copy wla-spc700.exe binaries

nmake clean
copy makefiles\makefile.win32cl.huc6280 makefile
nmake 
copy wla-huc6280.exe binaries

nmake clean

echo ""
echo "****************************************************************"
echo "*                    compiling WLALINK                         *"
echo "****************************************************************"
echo ""

cd wlalink
copy makefile.win32cl makefile
nmake 
copy wlalink.exe ..\binaries
nmake clean
cd ..

echo ""
echo "****************************************************************"
echo "*        all done! check ./binaries/ for the results           *"
echo "****************************************************************"
echo ""
