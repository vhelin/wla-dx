#!/usr/bin/env python3
"""Generate a one-entry MAME Neo Geo softlist for a local homebrew ROM set."""

import argparse
import hashlib
import html
import os
import sys
import zlib


def rom_hashes(path):
    crc = 0
    sha1 = hashlib.sha1()
    size = 0

    with open(path, "rb") as handle:
        for data in iter(lambda: handle.read(1024 * 1024), b""):
            size += len(data)
            crc = zlib.crc32(data, crc)
            sha1.update(data)

    return size, "%08x" % (crc & 0xFFFFFFFF), sha1.hexdigest()


def rom_line(name, size, crc, sha1, offset, loadflag=None):
    attrs = [
        'name="%s"' % html.escape(name, quote=True),
        'size="0x%x"' % size,
    ]
    if loadflag:
        attrs.append('loadflag="%s"' % loadflag)
    attrs.extend([
        'crc="%s"' % crc,
        'sha1="%s"' % sha1,
        'offset="0x%06x"' % offset,
    ])
    return "\t\t\t\t<rom %s/>\n" % " ".join(attrs)


def add_dataarea(lines, name, size, roms):
    lines.append('\t\t\t<dataarea name="%s" size="0x%x">\n' % (name, size))
    lines.extend(roms)
    lines.append("\t\t\t</dataarea>\n")


def add_single_rom_area(lines, area_name, rom_name, path, loadflag=None):
    size, crc, sha1 = rom_hashes(path)
    add_dataarea(lines, area_name, size, [rom_line(rom_name, size, crc, sha1, 0, loadflag)])
    return size


def main(argv):
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--out", required=True)
    parser.add_argument("--game-id", required=True)
    parser.add_argument("--description", default=None)
    parser.add_argument("--year", default="2026")
    parser.add_argument("--publisher", default="WLA DX")
    parser.add_argument("--p1", required=True)
    parser.add_argument("--m1")
    parser.add_argument("--s1")
    parser.add_argument("--c1")
    parser.add_argument("--c2")
    parser.add_argument("--c3")
    parser.add_argument("--c4")
    parser.add_argument("--v1")
    parser.add_argument("--v2")
    args = parser.parse_args(argv[1:])

    game_id = args.game_id
    description = args.description or ("WLA DX %s homebrew" % game_id)

    lines = [
        '<?xml version="1.0"?>\n',
        '<!DOCTYPE softwarelist SYSTEM "softwarelist.dtd">\n',
        '<softwarelist name="neogeo" description="Neo-Geo cartridges (homebrew)">\n',
        '\t<software name="%s">\n' % html.escape(game_id, quote=True),
        '\t\t<description>%s</description>\n' % html.escape(description),
        '\t\t<year>%s</year>\n' % html.escape(args.year),
        '\t\t<publisher>%s</publisher>\n' % html.escape(args.publisher),
        '\t\t<part name="cart" interface="neo_cart">\n',
    ]

    p1_size = rom_hashes(args.p1)[0]
    lines.append('\t\t\t<feature name="prom_size" value="0x%x"/>\n' % p1_size)
    lines.append('\t\t\t<feature name="slot" value="rom"/>\n')

    add_single_rom_area(lines, "maincpu", "%s-p1.bin" % game_id, args.p1, "load16_word_swap")

    if args.s1:
        add_single_rom_area(lines, "fixed", "%s-s1.bin" % game_id, args.s1)

    if args.m1:
        add_single_rom_area(lines, "audiocpu", "%s-m1.bin" % game_id, args.m1)

    sprite_pairs = []
    if args.c1 or args.c2:
        if not (args.c1 and args.c2):
            parser.error("--c1 and --c2 must be supplied together")
        sprite_pairs.append((1, args.c1, args.c2))
    if args.c3 or args.c4:
        if not (args.c3 and args.c4):
            parser.error("--c3 and --c4 must be supplied together")
        if not sprite_pairs:
            parser.error("--c3/--c4 require --c1/--c2")
        sprite_pairs.append((3, args.c3, args.c4))

    if sprite_pairs:
        sprite_roms = []
        sprite_size = 0
        for first_rom_number, first_path, second_path in sprite_pairs:
            first_size, first_crc, first_sha1 = rom_hashes(first_path)
            second_size, second_crc, second_sha1 = rom_hashes(second_path)
            sprite_roms.extend([
                rom_line("%s-c%d.bin" % (game_id, first_rom_number), first_size, first_crc, first_sha1, sprite_size, "load16_byte"),
                rom_line("%s-c%d.bin" % (game_id, first_rom_number + 1), second_size, second_crc, second_sha1, sprite_size + 1, "load16_byte"),
            ])
            sprite_size += max(first_size, second_size) * 2
        add_dataarea(lines, "sprites", sprite_size, sprite_roms)

    if args.v1:
        v1_size, v1_crc, v1_sha1 = rom_hashes(args.v1)
        roms = [rom_line("%s-v1.bin" % game_id, v1_size, v1_crc, v1_sha1, 0)]
        total_size = v1_size
        if args.v2:
            v2_size, v2_crc, v2_sha1 = rom_hashes(args.v2)
            roms.append(rom_line("%s-v2.bin" % game_id, v2_size, v2_crc, v2_sha1, v1_size))
            total_size += v2_size
        add_dataarea(lines, "ymsnd:adpcma", total_size, roms)

    lines.extend([
        "\t\t</part>\n",
        "\t</software>\n",
        "</softwarelist>\n",
    ])

    out_dir = os.path.dirname(args.out)
    if out_dir and not os.path.isdir(out_dir):
        os.makedirs(out_dir)
    with open(args.out, "w") as handle:
        handle.writelines(lines)
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv))