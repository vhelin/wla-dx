#!/bin/sh

#
# NOTE! All the small and ugly projects inside the "examples" directory
# are here used as unit tests / if the projects assemble and link without
# errors, we'll assume that WLA DX is in ok shape. Feel free to add more
# test projects to the lot.
#
# these tests aren't really unit tests, but hopefully one day they will be. :)
#

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
    make clean
    cd ..
}

export PATH=$PATH:$PWD/binaries
echo $PATH
cd examples

#####################################################################
# 6502
#####################################################################

changePlatform 6502
runTest compiler_test
runTest linker_test
runTest operand_hint_test
runTest struct_test
cd ..

#####################################################################
# 6510
#####################################################################

changePlatform 6510
runTest zero_page
runTest linker_test
runTest operand_hint_test
cd ..

#####################################################################
# 65816
#####################################################################

changePlatform 65816
runTest base_test_1
runTest base_test_2
runTest base_test_3
runTest linker_test
runTest name_test
runTest operand_hint_test
cd ..

#####################################################################
# 65c02
#####################################################################

changePlatform 65c02
runTest linker_test
cd ..

#####################################################################
# GB-Z80
#####################################################################

changePlatform gb-z80
runTest background_test
runTest linker_test
runTest namespace_test
runTest sintest
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
runTest linker_header_test
runTest linker_test_1
runTest linker_test_2
runTest rept_test
runTest sdsc_test
runTest sms_test
runTest ram_sections
cd ..
