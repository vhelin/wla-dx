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
expect_line '00008000 _private_label_8000'
expect_line '00008000 public_label_8001'
expect_line '00108000 label_108001'
