#!/usr/bin/env python
"""Generate a tiny Neo Geo S1 fix-layer ROM for the hello example."""
from __future__ import print_function

import sys

S1_SIZE = 0x20000
PIX_OFFSETS = (0x10, 0x18, 0x00, 0x08)

FONT_5X7 = {
    " ": (
        "00000",
        "00000",
        "00000",
        "00000",
        "00000",
        "00000",
        "00000",
    ),
    "!": (
        "00100",
        "00100",
        "00100",
        "00100",
        "00100",
        "00000",
        "00100",
    ),
    "D": (
        "11110",
        "10001",
        "10001",
        "10001",
        "10001",
        "10001",
        "11110",
    ),
    "E": (
        "11111",
        "10000",
        "10000",
        "11110",
        "10000",
        "10000",
        "11111",
    ),
    "H": (
        "10001",
        "10001",
        "10001",
        "11111",
        "10001",
        "10001",
        "10001",
    ),
    "L": (
        "10000",
        "10000",
        "10000",
        "10000",
        "10000",
        "10000",
        "11111",
    ),
    "O": (
        "01110",
        "10001",
        "10001",
        "10001",
        "10001",
        "10001",
        "01110",
    ),
    "R": (
        "11110",
        "10001",
        "10001",
        "11110",
        "10100",
        "10010",
        "10001",
    ),
    "W": (
        "10001",
        "10001",
        "10001",
        "10101",
        "10101",
        "10101",
        "01010",
    ),
}


def put_tile(rom, code, rows):
    base = code << 5
    for y in range(8):
        if y == 0:
            pixels = [0] * 8
        else:
            pattern = rows[y - 1]
            pixels = [0] + [1 if bit == "1" else 0 for bit in pattern] + [0, 0]
        for pair in range(4):
            low = pixels[pair * 2]
            high = pixels[(pair * 2) + 1]
            rom[base + y + PIX_OFFSETS[pair]] = low | (high << 4)


def main(argv):
    if len(argv) != 2:
        sys.stderr.write("usage: make_s1.py <output-s1.bin>\n")
        return 2

    rom = bytearray(S1_SIZE)
    for char, rows in FONT_5X7.items():
        put_tile(rom, ord(char), rows)

    out_file = open(argv[1], "wb")
    try:
        out_file.write(rom)
    finally:
        out_file.close()
    print("wrote %s (%d bytes)" % (argv[1], len(rom)))
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))
