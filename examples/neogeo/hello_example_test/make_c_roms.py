#!/usr/bin/env python
"""Generate blank Neo Geo C1/C2 sprite ROMs for the hello example."""
from __future__ import print_function

import sys

C_ROM_SIZE = 0x80000


def write_blank(path):
    with open(path, "wb") as out_file:
        out_file.write(bytearray(C_ROM_SIZE))
    print("wrote %s (%d bytes)" % (path, C_ROM_SIZE))


def main(argv):
    if len(argv) != 3:
        sys.stderr.write("usage: make_c_roms.py <output-c1.bin> <output-c2.bin>\n")
        return 2

    write_blank(argv[1])
    write_blank(argv[2])
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))