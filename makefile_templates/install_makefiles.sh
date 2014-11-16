#!/bin/bash
if [ -z "$make_host_suffix" ]
then
  echo "Please define the \$make_host_suffix of the makefiles you want to install."
  exit 1
fi

echo "Copying makefiles..."
cp makefile.$make_host_suffix.gb ../makefiles
cp makefile.$make_host_suffix.z80 ../makefiles
cp makefile.$make_host_suffix.6502 ../makefiles
cp makefile.$make_host_suffix.65c02 ../makefiles
cp makefile.$make_host_suffix.6510 ../makefiles
cp makefile.$make_host_suffix.65816 ../makefiles
cp makefile.$make_host_suffix.spc700 ../makefiles
cp makefile.$make_host_suffix.huc6280 ../makefiles
cp makefile.$make_host_suffix.wlab ../wlab/makefile.$make_host_suffix
cp makefile.$make_host_suffix.wlad ../wlad/makefile.$make_host_suffix
cp makefile.$make_host_suffix.wlalink ../wlalink/makefile.$make_host_suffix
cp makefile.$make_host_suffix.opcode_tables ../opcode_table_generator/makefile.$make_host_suffix

#rm makefile.$make_host_suffix.gb 
#rm makefile.$make_host_suffix.z80 
#rm makefile.$make_host_suffix.6502 
#rm makefile.$make_host_suffix.65c02 
#rm makefile.$make_host_suffix.6510 
#rm makefile.$make_host_suffix.65816 
#rm makefile.$make_host_suffix.spc700 
#rm makefile.$make_host_suffix.huc6280
#rm makefile.$make_host_suffix.wlab 
#rm makefile.$make_host_suffix.wlad 
#rm makefile.$make_host_suffix.wlalink
#rm makefile.$make_host_suffix.opcode_tables

echo "Copying build scripts..."
cp create_tables.$make_host_suffix ../opcode_table_generator/create_tables.$make_host_suffix
cp build.$make_host_suffix ..

#rm create_tables.$make_host_suffix
#rm build.$make_host_suffix

echo "Copy done. Please rename build.$make_host_suffix to an appropriate name" 
echo "and extension (preferably $make_host_suffix.\$script_extension)."
