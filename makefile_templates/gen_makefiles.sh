#!/bin/bash

#http://stackoverflow.com/questions/592620/how-to-check-if-a-program-exists-from-a-bash-script
command -v m4 >/dev/null 2>&1 || { echo >&2 "m4 is required but it's not installed.  Aborting."; exit 1; }

if [ -z "$host_defines" ]
then
  echo "Please specify the \$host_defines and \$make_host_suffix variables. The former"
  echo "should be the name of an m4 script (minus extension) in"
  echo "WLA_ROOT/makefile_templates, while the latter is used to distinguish"
  echo "makefiles for various targets."
  echo "If not set, \$make_host_suffix takes on the value of \$host_defines."
  exit 2
fi

if [ -z "$make_host_suffix" ]
then
  make_host_suffix=$host_defines
fi

echo "Creating makefiles for target $make_host_suffix..."
m4 -Dhost_defines=$host_defines.m4 -Dtarget_cpu=GB -Dout_suffix="gb" -Dopcode_prereq="opcodes_gb.c opcodes_z80.c" makefile.wla.m4 > makefile.$make_host_suffix.gb
m4 -Dhost_defines=$host_defines.m4 -Dtarget_cpu=Z80 -Dout_suffix="z80" -Dopcode_prereq="opcodes_z80.c" makefile.wla.m4 > makefile.$make_host_suffix.z80
m4 -Dhost_defines=$host_defines.m4 -Dtarget_cpu=MCS6502 -Dout_suffix="6502" -Dopcode_prereq="opcodes_6502.c" makefile.wla.m4 > makefile.$make_host_suffix.6502
m4 -Dhost_defines=$host_defines.m4 -Dtarget_cpu=WDC65C02 -Dout_suffix="65c02" -Dopcode_prereq="opcodes_65c02.c" makefile.wla.m4 > makefile.$make_host_suffix.65c02
m4 -Dhost_defines=$host_defines.m4 -Dtarget_cpu=MCS6510 -Dout_suffix="6510" -Dopcode_prereq="opcodes_6510.c" makefile.wla.m4 > makefile.$make_host_suffix.6510
m4 -Dhost_defines=$host_defines.m4 -Dtarget_cpu=W65816 -Dout_suffix="65816" -Dopcode_prereq="opcodes_65816.c" makefile.wla.m4 > makefile.$make_host_suffix.65816
m4 -Dhost_defines=$host_defines.m4 -Dtarget_cpu=SPC700 -Dout_suffix="spc700" -Dopcode_prereq="opcodes_spc700.c" makefile.wla.m4 > makefile.$make_host_suffix.spc700
m4 -Dhost_defines=$host_defines.m4 -Dtarget_cpu=HUC6280 -Dout_suffix="huc6280" -Dopcode_prereq="opcodes_huc6280.c" makefile.wla.m4 > makefile.$make_host_suffix.huc6280
m4 -Dhost_defines=$host_defines.m4 makefile.wlalink.m4 > makefile.$make_host_suffix.wlalink
m4 -Dhost_defines=$host_defines.m4 -Dout_name=wlad makefile.generic.m4 > makefile.$make_host_suffix.wlad
m4 -Dhost_defines=$host_defines.m4 -Dout_name=wlab makefile.generic.m4 > makefile.$make_host_suffix.wlab
m4 -Dhost_defines=$host_defines.m4 makefile.opcode_tables.m4 > makefile.$make_host_suffix.opcode_tables


echo "Creating build scripts for target $make_host_suffix..."
m4 -Dhost_defines=$host_defines.m4 -DMAKE_HOST_SUFFIX=$make_host_suffix -DOUTNAME=$make_host_suffix build.m4 > build.$make_host_suffix
m4 -Dhost_defines=$host_defines.m4 -DMAKE_HOST_SUFFIX=$make_host_suffix create_tables.m4 > create_tables.$make_host_suffix

echo "Done. Inspect the generated files for potential errors, and"
echo "make any desired changes, i.e."
echo "Adding conditional assignments (?=) for Unix Makefiles,"
echo "before installing the scripts."

#m4 -Dhost_defines=unix_gcc.m4 -Dtarget_cpu=Z80 -Dout_suffix="z80" -Dopcode_prereq="opcodes_z80.c" makefile.wla.m4 > makefile.unix.z80
