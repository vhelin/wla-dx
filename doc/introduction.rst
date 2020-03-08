Introduction
============

The history behind WLA DX, from the original author, Ville Helin:

  I wrote this because I had never written an assembler before and I really
  needed a macro assembler which could compile the GB-Z80 code I wrote. ;)
  Gaelan Griffin needed real Z80 support for his SMS projects so I thought
  I could write WLA to be a little more open and nowadays it supports all
  the Z80 systems you can think of. You'll just have to define the memorymap
  of the destination machine for your project. After fixing some bugs I thought
  I could add support for 6502 systems so all NES-people would get their
  share of WLA as well. After finishing that few people said they'd like 65816
  support (they had SNES developing in mind) so I added support for that. And
  then I thought I should write a 6510 version of WLA as well...
 
  This is my ideal GB-Z80 macro assembler (not in final form, not yet). ;)
  Tastes differ. Thus WLA! Notice that WLA was initially made for Game Boy
  developers so the GB-Z80 version and the rest differ a little.

Good to know about WLA DX:

Almost all rules that apply to Z80 source code processing with WLA DX
apply also to 6502, 65C02, 65CE02, 6510, 65816, 6800, 6801, 6809, 8008, 8080,
HUC6280 and SPC-700.

About the names: WLA DX means all the tools covered in this documentation.
So WLA DX includes WLA GB-Z80/Z80/6502/65C02/65CE02/6510/65816/6800/6801/6809/
8008/8080/HUC6280/SPC-700 macro assembler (what a horribly long name), WLAB,
and WLALINK GB-Z80/Z80/6502/65C02/65CE02/6510/65816/6800/6801/6809/8008/8080/
HUC6280/SPC-700 linker. We use plain WLA to refer to the macro assembler
in this document.

There was WLAD, an GB-Z80 dissassembler, but it has been discontinued and
removed from the project and the documentation.

Currently WLA can also be used as a patch tool. Just include the original
ROM image into the project with ``.BACKGROUND`` and insert e.g.,
``OVERWRITE .SECTION`` s to patch the desired areas. Output the data into a new
ROM image and there you have it. 100% readable (asm coded) patches are reality!

Note that you can directly compile only object and library files. You must
use WLALINK to link these (or only one, if you must) into a ROM/program file.

| WLA DX's old homepage: http://www.iki.fi/~vhelin/wla.html
| WLA DX's new homepage: https://github.com/vhelin/wla-dx
