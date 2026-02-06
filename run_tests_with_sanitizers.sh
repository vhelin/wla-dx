#!/bin/bash
set -e

# Tell run_tests.sh to stay away from Valgrind
export NO_VALGRIND=1

echo "========================================="
echo "Testing WLA DX with Sanitizers"
echo "========================================="

ORIGINAL_DIR="$PWD"

# Results tracking
ASAN_RESULT="Pending"
UBSAN_RESULT="Pending"
MSAN_RESULT="Pending"

# Clean up
echo "Cleaning artifacts..."
rm -f CMakeCache.txt cmake_install.cmake Makefile
rm -rf CMakeFiles/ binaries/
mkdir -p binaries
rm -rf build-asan build-ubsan build-msan

# 1. AddressSanitizer
echo ""
echo "1/3: Building with AddressSanitizer..."
mkdir -p build-asan
cd build-asan
cmake .. -DCMAKE_BUILD_TYPE=Debug \
         -DCMAKE_C_FLAGS="-fsanitize=address -fno-omit-frame-pointer -g -O1" \
         -DCMAKE_EXE_LINKER_FLAGS="-fsanitize=address"
cmake --build . --config Debug
cd "$ORIGINAL_DIR"

echo "Running tests with ASan..."
cp -r build-asan/binaries/* binaries/
export ASAN_OPTIONS="detect_leaks=1:fast_unwind_on_malloc=0:halt_on_error=0:symbolize=1"

set +e
if ./run_tests.sh; then
    ASAN_RESULT="PASS"
else
    ASAN_RESULT="FAIL"
fi
set -e

# 2. UndefinedBehaviorSanitizer
echo ""
echo "2/3: Building with UBSan..."
rm -rf binaries/*
mkdir -p build-ubsan
cd build-ubsan
cmake .. -DCMAKE_BUILD_TYPE=Debug \
         -DCMAKE_C_FLAGS="-fsanitize=undefined -fno-omit-frame-pointer -g" \
         -DCMAKE_EXE_LINKER_FLAGS="-fsanitize=undefined"
cmake --build . --config Debug
cd "$ORIGINAL_DIR"

echo "Running tests with UBSan..."
cp -r build-ubsan/binaries/* binaries/
export UBSAN_OPTIONS="print_stacktrace=1:halt_on_error=0"

set +e
if ./run_tests.sh; then
    UBSAN_RESULT="PASS"
else
    UBSAN_RESULT="FAIL"
fi
set -e

# 3. MemorySanitizer (if Clang is available)
if command -v clang &> /dev/null; then
    echo ""
    echo "3/3: Building with MSan..."
    rm -rf binaries/*
    mkdir -p build-msan
    cd build-msan
    cmake .. -DCMAKE_BUILD_TYPE=Debug \
             -DCMAKE_C_COMPILER=clang \
             -DCMAKE_C_FLAGS="-fsanitize=memory -fno-omit-frame-pointer -g" \
             -DCMAKE_EXE_LINKER_FLAGS="-fsanitize=memory"
    cmake --build . --config Debug
    cd "$ORIGINAL_DIR"

    echo "Running tests with MSan..."
    cp -r build-msan/binaries/* binaries/
    export MSAN_OPTIONS="halt_on_error=0"

    set +e
    if ./run_tests.sh; then
        MSAN_RESULT="PASS"
    else
        MSAN_RESULT="FAIL"
    fi
    set -e
else
    MSAN_RESULT="SKIPPED (No Clang)"
fi

echo ""
echo "========================================="
echo "         SANITIZER TEST SUMMARY          "
echo "========================================="
echo " AddressSanitizer (ASan): $ASAN_RESULT"
echo " UndefinedBehavior (UBSan): $UBSAN_RESULT"
echo " MemorySanitizer (MSan): $MSAN_RESULT"
echo "========================================="

# Exit with error if any of them failed
if [[ "$ASAN_RESULT" == "FAIL" || "$UBSAN_RESULT" == "FAIL" || "$MSAN_RESULT" == "FAIL" ]]; then
    exit 1
fi
