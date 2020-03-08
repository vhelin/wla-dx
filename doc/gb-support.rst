Supported ROM/RAM/Cartridge Types (WLA-GB)
==========================================

ROM Size
--------

GB-Z80 version of WLA supports the following ROM bank sizes. There's no such limit
in the Z80/6502/65C02/65CE02/6510/65816/6800/6801/6809/8008/8080/HUC6280/SPC-700
version of WLA. Supply one of the following values to ``.ROMBANKS``.

======= ======== ========== ===========
``$00`` 256Kbit    32KByte     2 banks
``$01`` 512Kbit    64KByte     4 banks
``$02``   1Mbit   128KByte     8 banks
``$03``   2Mbit   256KByte    16 banks
``$04``   4Mbit   512KByte    32 banks
``$05``   8Mbit     1MByte    64 banks
``$06``  16Mbit     2MByte   128 banks
``$07``  32Mbit     4MByte   256 banks
``$08``  64Mbit     8MByte   512 banks
``$52``   9Mbit   1.1MByte    72 banks
``$53``  10Mbit   1.2MByte    80 banks
``$54``  12Mbit   1.5MByte    96 banks
======= ======== ========== ===========

RAM Size
--------

Supply one of the following hex values to ``.RAMSIZE`` in the GB-Z80 version
of WLA.

======= ======== ========== ==========
``$00``    None       None      None
``$01``  16kbit     2kByte    1 bank
``$02``  64kbit     8kByte    1 bank
``$03`` 256kbit    32kByte    4 banks
``$04``   1Mbit   128kByte   16 banks
``$05`` 512kbit    64kByte    8 banks
======= ======== ========== ==========

Cartridge Type
--------------

It's up to the user to check that the cartridge type is valid and
can be used combined with the supplied ROM and RAM sizes. Give
one the the following values to ``.CARTRIDGETYPE`` in the GB-Z80 version of WLA.

======= =============== ====== ====== ========= ======= ========
``$00`` ROM
``$01`` ROM              MBC1
``$02`` ROM              MBC1    RAM
``$03`` ROM              MBC1    RAM   BATTERY
``$05`` ROM              MBC2
``$06`` ROM              MBC2          BATTERY
``$08`` ROM                      RAM
``$09`` ROM                      RAM   BATTERY
``$0B`` ROM              MMM01
``$0C`` ROM              MMM01  SRAM
``$0D`` ROM              MMM01  SRAM   BATTERY
``$0F`` ROM              MBC3          BATTERY   TIMER
``$10`` ROM              MBC3    RAM   BATTERY   TIMER
``$11`` ROM              MBC3
``$12`` ROM              MBC3    RAM
``$13`` ROM              MBC3    RAM   BATTERY
``$19`` ROM              MBC5
``$1A`` ROM              MBC5    RAM
``$1B`` ROM              MBC5    RAM   BATTERY
``$1C`` ROM              MBC5                            RUMBLE
``$1D`` ROM              MBC5   SRAM                     RUMBLE
``$1E`` ROM              MBC5   SRAM   BATTERY           RUMBLE
``$20`` MBC6
``$22`` MBC7
``$BE`` Pocket Voice
``$FC`` Pocket Camera
``$FD`` Bandai TAMA5
``$FE`` Hudson HuC-3
``$FF`` Hudson HuC-1
======= =============== ====== ====== ========= ======= ========
