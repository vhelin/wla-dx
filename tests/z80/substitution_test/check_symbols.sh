#!/bin/sh
set -e
set -u

symbol_file="${1}"

expect_line() {
  if ! grep -q "^${1}$" <"${symbol_file}"; then
    printf 'error: expected line not present in %s: %s\n' "${symbol_file}" "${1}" >&2
    dump_symbol_file
    exit 1
  fi
}

dump_symbol_file() {
    printf 'note: content of %s:\n' "${symbol_file}"
    while read line; do
      printf '  %s\n' "${line}"
    done <"${symbol_file}"
}

# All labels are in the symbol file.
expect_line '00:0000 test_1'
expect_line '00:0000 test_1_address_0'
expect_line '00:0003 MyValueIs_3'
expect_line '00:0003 formatted_000A'
expect_line '00:0003 formatted_000A_label'
expect_line '00:0006 ABC_023'
expect_line '00:000c substitution_of_10'
expect_line '00:0020 Label_000a'
expect_line '00:0020 Label_00A_005'
expect_line '00:0020 Label_000000011'
expect_line '00:0020 Label_030'
expect_line '00:0020 All_4_MacroTests1'
expect_line '00:0020 All_4_MacroTests2'
expect_line '00:0020 Hack_1000'
expect_line '00:0020 Hack_1_Hack'
expect_line '00:0020 Hack_3_More'
expect_line '00:0020 Hack_1001'
expect_line '00:0020 Hack_2_Hack'
expect_line '00:0020 Hack_4_More'
expect_line '00:0023 LastNumberShouldBeTwo_2'
expect_line '00:0023 LastNumberShouldBeThree_3'
expect_line '00:0023 InTheMiddleShouldBeFive_5_IsThere'
expect_line '00:0023 LastNumbersShouldBeOneTwoThree_123'
expect_line '00:0023 InTheMiddleOneTwoThree_123_IsThere'
expect_line '00:0030 Hack_1002'
expect_line '00:0030 Hack_3_Hack'
expect_line '00:0030 Hack_5_More'
expect_line '00:0030 Hack_1003'
expect_line '00:0030 Hack_4_Hack'
expect_line '00:0030 Hack_6_More'
expect_line '00:1003 gfxHeader4'
expect_line '00:1003 gfxHeaderB14'
expect_line '00:1003 gfxHeaderB24'
expect_line '00:1003 gfxHeaderH14'
expect_line '00:1003 gfxHeaderH24'
expect_line '00:1003 gfxHeaderH34'
expect_line '00:1005 Data200612Label'
expect_line '00:1005 DataBEEF3DEAD'
