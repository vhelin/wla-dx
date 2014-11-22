#!/bin/sh

export host_defines=unix_gcc
export make_host_suffix=unix 
./gen_makefiles.sh
./install_makefiles.sh
mv ../build.unix ../unix.sh
rm makefile.$make_host_suffix.*
rm create_tables.$make_host_suffix
rm build.$make_host_suffix

export host_defines=mingw 
export make_host_suffix=win32
./gen_makefiles.sh
./install_makefiles.sh
mv ../build.win32 ../win32
rm makefile.$make_host_suffix.*
rm create_tables.$make_host_suffix
rm build.$make_host_suffix

export host_defines=win32cl 
export make_host_suffix=win32cl 
./gen_makefiles.sh
./install_makefiles.sh
mv ../build.win32cl ../win32cl.bat
rm makefile.$make_host_suffix.*
rm create_tables.$make_host_suffix
rm build.$make_host_suffix
