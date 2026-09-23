#!/bin/sh
set -e
set -u

symbol_file="${1}"

dump_symbol_file() {
    printf 'note: content of %s:\n' "${symbol_file}"
    while read line; do
      printf '  %s\n' "${line}"
    done <"${symbol_file}"
}

expect_unique_line() {
  line="${1}"
  count=$(grep -c "^${line}$" <"${symbol_file}" || true)
  if [ "${count}" -ne 1 ]; then
    printf 'error: expected exactly one occurrence of "%s" in %s (found %s)\n' "${line}" "${symbol_file}" "${count}" >&2
    dump_symbol_file
    exit 1
  fi
}

# Shared .ENUM EXPORT / .DEFINE EXPORT must appear once after linking with -C.
expect_unique_line '00000600 MyLabel'
expect_unique_line '00000001 _sizeof_MyLabel'
expect_unique_line '0000002a SharedConst'
