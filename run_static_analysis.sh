#!/bin/bash

echo "========================================="
echo "WLA DX - Static Analysis (Informational)"
echo "========================================="
echo ""
echo "This analysis is informational only."
echo "Issues found here won't break the build."
echo ""

# Save original directory
ORIGINAL_DIR="$PWD"

# Clean up previous analysis builds
rm -rf build-analysis

# Track overall status
ISSUES_FOUND=0

# Install tools quietly
if ! command -v cppcheck &> /dev/null; then
    echo "Installing cppcheck..."
    sudo apt-get update >/dev/null 2>&1
    sudo apt-get install -y cppcheck >/dev/null 2>&1
fi

if ! command -v clang &> /dev/null; then
    echo "Installing clang..."
    sudo apt-get install -y clang clang-tools >/dev/null 2>&1
fi

if ! command -v clang-tidy &> /dev/null; then
    echo "Installing clang-tidy..."
    sudo apt-get install -y clang-tidy >/dev/null 2>&1
fi

# ============================================
# Cppcheck Analysis
# ============================================
echo ""
echo "========================================="
echo "Running Cppcheck"
echo "========================================="

cppcheck --enable=all \
         --inconclusive \
         --std=c90 \
         --suppress=missingIncludeSystem \
         --suppress=unusedFunction \
         . 2>&1 || ISSUES_FOUND=$((ISSUES_FOUND + 1))

# ============================================
# Clang Static Analyzer
# ============================================
echo ""
echo "========================================="
echo "Running Clang Static Analyzer"
echo "========================================="

mkdir -p build-analysis
cd build-analysis

scan-build --status-bugs \
           -enable-checker alpha.core.CastSize \
           -enable-checker alpha.core.CastToStruct \
           -enable-checker alpha.core.IdenticalExpr \
           -enable-checker alpha.core.SizeofPtr \
           -enable-checker alpha.security.ArrayBoundV2 \
           -enable-checker alpha.security.MallocOverflow \
           cmake .. -DCMAKE_BUILD_TYPE=Debug 2>&1 || true

scan-build --status-bugs \
           -enable-checker alpha.core.CastSize \
           -enable-checker alpha.core.CastToStruct \
           -enable-checker alpha.core.IdenticalExpr \
           -enable-checker alpha.core.SizeofPtr \
           -enable-checker alpha.security.ArrayBoundV2 \
           -enable-checker alpha.security.MallocOverflow \
           cmake --build . 2>&1 || ISSUES_FOUND=$((ISSUES_FOUND + 1))

cd "$ORIGINAL_DIR"

# ============================================
# Clang-Tidy Analysis
# ============================================
echo ""
echo "========================================="
echo "Running clang-tidy"
echo "========================================="

cd build-analysis
cmake .. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON >/dev/null 2>&1
cd "$ORIGINAL_DIR"

find . -name "*.c" -not -path "*/build*/*" -not -path "*/CMakeFiles/*" \
    -not -path "*/historical/*" -not -path "*/tests/*" | \
    while read -r file; do
        clang-tidy "$file" \
            -p build-analysis \
            --checks='bugprone-*,clang-analyzer-*,performance-*,portability-*,readability-*,-readability-magic-numbers,-readability-identifier-length' \
            2>&1 || true
    done

# ============================================
# Summary
# ============================================
echo ""
echo "========================================="
echo "Analysis Complete"
echo "========================================="

if [ $ISSUES_FOUND -eq 0 ]; then
    echo "✓ No critical issues found"
    exit 0
else
    echo "ℹ Issues found - see output above"
    echo ""
    echo "This is informational only."
    exit 1
fi
