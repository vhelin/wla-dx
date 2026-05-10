#!/usr/bin/env python
"""Create a zero-filled ROM image of a requested size."""

import os
import sys


ZERO_CHUNK = b"\0" * (1024 * 1024)


def parse_size(text):
    return int(text, 0)


def main(argv):
    if len(argv) != 3:
        sys.stderr.write("usage: %s <output.bin> <size>\n" % argv[0])
        return 2
    out_path = argv[1]
    size = parse_size(argv[2])
    out_dir = os.path.dirname(out_path)
    if out_dir and not os.path.isdir(out_dir):
        os.makedirs(out_dir)
    with open(out_path, "wb") as handle:
        remaining = size
        while remaining > 0:
            chunk_size = min(remaining, len(ZERO_CHUNK))
            handle.write(ZERO_CHUNK[:chunk_size])
            remaining -= chunk_size
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))