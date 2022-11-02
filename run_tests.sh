#!/bin/sh

#
# NOTE! All the small and ugly projects inside the "tests" directory
# are here used as regression tests / if the projects assemble and link without
# errors, we'll assume that WLA DX is in ok shape. Feel free to add more
# test projects to the lot.
#

set -e

runTest() {
    set -e
    cd $1
    make clean
    make
    if test -f "testsfile"; then
        eval "$BYTE_TESTER testsfile"
    fi
    make clean
    cd ..
}

if [ $# -eq 1 ]; then
    if [ "$1" = "-windows-x86" ]; then
        export PATH=$PATH:$PWD/windows/Release
    elif [ "$1" = "-windows-x64" ]; then
        export PATH=$PATH:$PWD/windows/x64/Release
    else
	export PATH=$PATH:$PWD/binaries:$PWD/build/binaries
    fi
else
    export PATH=$PATH:$PWD/binaries:$PWD/build/binaries
fi

# byte_tester
echo Building byte_tester...
cd byte_tester
make install
# just in case...
cp byte_tester* ../binaries/

if test -f "byte_tester"; then
    BYTE_TESTER="byte_tester"
fi
if test -f "byte_tester.exe"; then
    BYTE_TESTER="byte_tester.exe"
fi

SHOW_ALL_OUTPUT=false
if [ $# -eq 1 ]; then
    if [ "$1" = "-v" ]; then
        SHOW_ALL_OUTPUT=true
    fi
fi

# Valgrind test...
# Makefiles in the tests folder use WLAVALGRIND to run Valgrind at the same time
# with WLA and WLALINK
if ! [ -x "$(command -v valgrind)" ]; then
  echo
  echo '########################################################################'
  echo 'WARNING: Valgrind is not installed so we cannot perform memory checks...'
  echo '########################################################################'
  export WLAVALGRIND=
else
  export WLAVALGRIND='valgrind --error-exitcode=1 --tool=memcheck --leak-check=full --errors-for-leak-kinds=all --track-origins=yes -s'
fi

cd ..

echo
echo Running tests...
cd tests

if $SHOW_ALL_OUTPUT; then
    echo
fi

set +e
TEST_COUNT=0
for PLATFORM in */; do
    cd $PLATFORM
        for TEST in */; do
            # Skip tests starting with _
            FIRST_CHAR=$(echo $TEST | cut -c1)
            if [ "$FIRST_CHAR" = "_" ]; then
                continue
            fi

            # Run test
            if [ -e "$TEST""makefile" ]; then
                OUT=$(runTest $TEST 2>&1)
                if [ $? -ne 0 ]; then
                    printf "\n\n%s\n\n" "$OUT"
                    echo "########"
                    echo FAILURE!
                    echo "########"
                    echo Test \"$TEST\" of platform \"$PLATFORM\" failed.
                    exit 1
                elif $SHOW_ALL_OUTPUT; then
                    echo "#####################################################################"
                    echo Test \"$TEST\" of platform \"$PLATFORM\" succeeded:
                    echo "#####################################################################"
                    printf "\n%s\n\n" "$OUT"
                else
                    printf .
                fi
                # printf "%s" "$OUT"
                TEST_COUNT=$((TEST_COUNT+1))
            fi
        done
    cd ..
done

printf "\n\n"
echo "OK ($TEST_COUNT tests)"
