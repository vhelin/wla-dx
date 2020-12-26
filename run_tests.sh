#!/bin/sh

#
# NOTE! All the small and ugly projects inside the "examples" directory
# are here used as unit tests / if the projects assemble and link without
# errors, we'll assume that WLA DX is in ok shape. Feel free to add more
# test projects to the lot.
#

set -e

changePlatform() {
    echo ""
    echo "************************************************************"
    echo $1
    echo "************************************************************"
    cd $1
}

runTest() {
    cd $1
    echo ""
    echo $1
    echo ""
    make clean
    make
    if test -f "testsfile"; then
        eval "$BYTE_TESTER testsfile"
    fi
    make clean
    cd ..
}

export PATH=$PATH:$PWD/binaries
echo $PATH

# byte_tester

cd byte_tester
make install

if test -f "byte_tester"; then
    BYTE_TESTER="byte_tester"
fi
if test -f "byte_tester.exe"; then
    BYTE_TESTER="byte_tester.exe"
fi

cd ..

cd examples

#####################################################################
# 6502
#####################################################################

changePlatform 6502
runTest compiler_test
runTest linker_test
runTest operand_hint_test
runTest slots_test
runTest struct_test
cd ..

#####################################################################
# 65c02
#####################################################################

changePlatform 65c02
runTest linker_test
cd ..

#####################################################################
# 65ce02
#####################################################################

changePlatform 65ce02
runTest linker_test
cd ..

#####################################################################
# 6510
#####################################################################

changePlatform 6510
runTest zero_page
runTest linker_test
runTest operand_hint_test
runTest c64_prg_test
cd ..

#####################################################################
# 65816
#####################################################################

changePlatform 65816
runTest bank_number
runTest base_test_1
runTest base_test_2
runTest base_test_3
runTest base_test_4
runTest checksum_12mbit_lorom
runTest checksum_24mbit_lorom
runTest checksum_8mbit_hirom
runTest checksum_8mbit_lorom
runTest dlm_test
runTest linker_test
runTest macro_test
runTest name_test
runTest nocashsns_symbol_file
runTest opcode_test
runTest operand_hint_test
runTest wdc_test
cd ..

#####################################################################
# GB-Z80
#####################################################################

changePlatform gb-z80
runTest appendto_test
runTest background_test
runTest bank_number
runTest linker_test
runTest namespace_test
runTest sintest
runTest union_test
cd ..

#####################################################################
# HUC6280
#####################################################################

changePlatform huc6280
runTest linker_test
runTest ram_sections
cd ..

#####################################################################
# SPC-700
#####################################################################

changePlatform spc-700
runTest linker_test
cd ..

#####################################################################
# Z80
#####################################################################

changePlatform z80
runTest caddr_test
runTest rst_test
runTest linker_header_test
runTest linker_test_1
runTest linker_test_2
runTest ram_sections
runTest rept_test
runTest sdsc_test
runTest sms_test
runTest smschecksum_test_1
runTest smschecksum_test_2
runTest smsheader_test
runTest z81_test
cd ..

#####################################################################
# 6800
#####################################################################

changePlatform 6800
runTest linker_test
cd ..

#####################################################################
# 6801
#####################################################################

changePlatform 6801
runTest linker_test
cd ..

#####################################################################
# 6809
#####################################################################

changePlatform 6809
runTest linker_test
cd ..

#####################################################################
# 8008
#####################################################################

changePlatform 8008
runTest linker_test
runTest rst_test
cd ..

#####################################################################
# 8080
#####################################################################

changePlatform 8080
runTest linker_test
runTest rst_test
cd ..
