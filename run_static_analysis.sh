#!/bin/bash
set -e

echo "========================================="
echo "WLA DX - Static Analysis"
echo "========================================="

# Save original directory
ORIGINAL_DIR="$PWD"

# Clean up previous analysis builds
rm -rf build-analysis

# Track overall status
ISSUES_FOUND=0

echo ""
echo "========================================="
echo "1. Installing Static Analysis Tools"
echo "========================================="

# Check and install tools
if ! command -v cppcheck &> /dev/null; then
    echo "Installing cppcheck..." >&2
    sudo apt-get update >&2
    sudo apt-get install -y cppcheck >&2
fi

if ! command -v clang &> /dev/null; then
    echo "Installing clang and scan-build..." >&2
    sudo apt-get install -y clang clang-tools >&2
fi

if ! command -v clang-tidy &> /dev/null; then
    echo "Installing clang-tidy..." >&2
    sudo apt-get install -y clang-tidy >&2
fi

echo "✓ All tools installed"

# ============================================
# 2. Cppcheck Analysis
# ============================================
echo ""
echo "========================================="
echo "2. Running Cppcheck"
echo "========================================="

if ! cppcheck --enable=all \
         --inconclusive \
         --std=c90 \
         --suppress=missingIncludeSystem \
         --suppress=unusedFunction \
         --error-exitcode=1 \
         . 2>&1; then
    echo "⚠ Cppcheck found issues" >&2
    ISSUES_FOUND=$((ISSUES_FOUND + 1))
else
    echo "✓ Cppcheck: No issues found"
fi

# ============================================
# 3. Clang Static Analyzer (scan-build)
# ============================================
echo ""
echo "========================================="
echo "3. Running Clang Static Analyzer"
echo "========================================="

mkdir -p build-analysis
cd build-analysis

# Use scan-build to analyze during compilation
if ! scan-build --status-bugs \
           -enable-checker alpha.core.CastSize \
           -enable-checker alpha.core.CastToStruct \
           -enable-checker alpha.core.IdenticalExpr \
           -enable-checker alpha.core.SizeofPtr \
OAOAOA           -enable-checker alpha.security.ArrayBoundV2 \
           -enable-checker alpha.security.MallocOverflow \
           -enable-checker alpha.security.ReturnPtrRange \
           -enable-checker alpha.unix.SimpleStream \
           -enable-checker alpha.unix.cstring.BufferOverlap \
           -enable-checker alpha.unix.cstring.NotNullTerminated \
           -enable-checker alpha.unix.cstring.OutOfBounds \
           cmake .. -DCMAKE_BUILD_TYPE=Debug 2>&1; then
    echo "⚠ Clang Static Analyzer configuration found issues" >&2
    ISSUES_FOUND=$((ISSUES_FOUND + 1))
fi

if ! scan-build --status-bugs \
           -enable-checker alpha.core.CastSize \
           -enable-checker alpha.core.CastToStruct \
           -enable-checker alpha.core.IdenticalExpr \
           -enable-checker alpha.core.SizeofPtr \
           -enable-checker alpha.security.ArrayBoundV2 \
           -enable-checker alpha.security.MallocOverflow \
           -enable-checker alpha.security.ReturnPtrRange \
           -enable-checker alpha.unix.SimpleStream \
           -enable-checker alpha.unix.cstring.BufferOverlap \
           -enable-checker alpha.unix.cstring.NotNullTerminated \
           -enable-checker alpha.unix.cstring.OutOfBounds \
           cmake --build . 2>&1; then
    echo "⚠ Clang Static Analyzer found issues" >&2
    ISSUES_FOUND=$((ISSUES_FOUND + 1))
else
    echo "✓ Clang Static Analyzer: No issues found"
fi

cd "$ORIGINAL_DIR"

# ============================================
# 4. Clang-Tidy Analysis
# ============================================
echo ""
echo "========================================="
echo "4. Running clang-tidy"
echo "========================================="

# Generate compile_commands.json for clang-tidy
cd build-analysis
cmake .. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON >/dev/null 2>&1
cd "$ORIGINAL_DIR"

# Run clang-tidy on all source files
echo "Analyzing source files..."
TIDY_FAILED=0
find . -name "*.c" -not -path "*/build*/*" -not -path "*/CMakeFiles/*" \
    -not -path "*/historical/*" -not -path "*/tests/*" | \
    while read -r file; do
        echo "  Checking: $file"
        if ! clang-tidy "$file" \
            -p build-analysis \
            --warnings-as-errors='*' \
            --checks='bugprone-*,clang-analyzer-*,performance-*,portability-*,readability-*,-readability-magic-numbers,-readability-identifier-length' \
            2>&1; then
            TIDY_FAILED=1
        fi
    done

if [ $TIDY_FAILED -ne 0 ]; then
    echo "⚠ clang-tidy found warnings" >&2
    ISSUES_FOUND=$((ISSUES_FOUND + 1))
else
    echo "✓ clang-tidy: No warnings found"
fi

# ============================================
# 5. Summary
# ============================================
echo ""
echo "========================================="
echo "Static Analysis Complete"
echo "========================================="

if [ $ISSUES_FOUND -eq 0 ]; then
    echo "✓ All static analysis checks passed!"
    exit 0
else
    echo "⚠ Static analysis found issues in $ISSUES_FOUND tool(s)" >&2
    exit 1
fi
