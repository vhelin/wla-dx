Assembler Directives
====================

Here's the order in which the data is placed into the output:

1. Data and group 3 directives outside sections.
2. Group 2 directives.
3. Data and group 3 directives inside sections.
4. Group 1 directives.

=== ================================================================
ALL All, GB-Z80, Z80, 6502, 65C02, 65CE02, 6510, 65816, HUC6280,
    SPC-700, 6800, 6801, 6809, 8008 and 8080 versions apply.
GB  Only the GB-Z80 version applies.
GB8 Only the GB-Z80 and 65816 versions apply.
Z80 Only the Z80 version applies.
658 Only the 65816 version applies.
680 Only the 6800, 6801 and 6809 versions apply.
800 Only the 8008 version applies.
808 Only the 8080 version applies.
SPC Only the SPC-700 version applies.
65x Only the 6502, 65C02, 65CE02, 6510, 65816 and HUC6280 versions apply.
!GB Only the Z80, 6502, 65C02, 65CE02, 6510, 65816, HUC6280 and SPC-700
    versions apply.
=== ================================================================



Group 1:

=== ==========================================================================
GB   ``.COMPUTEGBCHECKSUM``
Z80  ``.COMPUTESMSCHECKSUM``
658  ``.COMPUTESNESCHECKSUM``
Z80  ``.SDSCTAG 1.0, "DUNGEON MAN", "A wild dungeon exploration game", "Ville Helin"``
Z80  ``.SMSTAG``
=== ==========================================================================

Group 2:

=== ===============================
GB   ``.CARTRIDGETYPE 1``
GB   ``.COUNTRYCODE 1``
GB   ``.VERSION 1``
GB   ``.DESTINATIONCODE 1``
GB   ``.NINTENDOLOGO``
GB   ``.GBHEADER``
Z80  ``.SMSHEADER``
GB   ``.COMPUTEGBCOMPLEMENTCHECK``
ALL  ``.EMPTYFILL $C9``
658  ``.ENDEMUVECTOR``
658  ``.ENDNATIVEVECTOR``
658  ``.ENDSNES``
ALL  ``.EXPORT work_x``
658  ``.FASTROM``
658  ``.HIROM``
658  ``.EXHIROM``
GB   ``.LICENSEECODENEW "1A"``
GB   ``.LICENSEECODEOLD $1A``
658  ``.LOROM``
GB8  ``.NAME "NAME OF THE ROM"``
ALL  ``.OUTNAME "other.o"``
GB   ``.RAMSIZE 0``
GB   ``.ROMDMG``
GB   ``.ROMGBC``
GB   ``.ROMGBCONLY``
GB   ``.ROMSGB``
658  ``.SLOWROM``
658  ``.SMC``
658  ``.SNESEMUVECTOR``
658  ``.SNESHEADER``
658  ``.SNESNATIVEVECTOR``
=== ===============================

Group 3:

=== ================================================================
65x  ``.16BIT``
658  ``.24BIT``
65x  ``.8BIT``
658  ``.ACCU 8``
658  ``.WDC``
658  ``.NOWDC`` 
ALL  ``.ASC "HELLO WORLD!"``
ALL  ``.ASCTABLE``
ALL  ``.ASCIITABLE``
ALL  ``.ASM``
ALL  ``.BACKGROUND "parallax.gb"``
ALL  ``.BANK 0 SLOT 1``
ALL  ``.BASE $80``
ALL  ``.BLOCK "Block1"``
ALL  ``.BR``
ALL  ``.BREAKPOINT``
ALL  ``.BYT 100, $30, %1000, "HELLO WORLD!"``
ALL  ``.DATA $ff00, 2``
ALL  ``.DB 100, $30, %1000, "HELLO WORLD!"``
ALL  ``.DBM filtermacro 1, 2, "encrypt me"``
ALL  ``.DBCOS 0.2, 10, 3.2, 120, 1.3``
ALL  ``.DBRND 20, 0, 10``
ALL  ``.DBSIN 0.2, 10, 3.2, 120, 1.3``
ALL  ``.DEFINE IF $FF0F``
ALL  ``.DEF IF $FF0F``
658  ``.DL $102030, $405060``
658  ``.DLM filtermacro 1, 2, 3``
ALL  ``.DS 256, $10``
ALL  ``.DSB 256, $10``
658  ``.DSL 16, $102030``
ALL  ``.DSTRUCT waterdrop INSTANCEOF water DATA "tingle", 40, 120``
ALL  ``.DSW 128, 20``
ALL  ``.DW 16000, 10, 255``
ALL  ``.DWM filtermacro 1, 2, 3``
ALL  ``.DWCOS 0.2, 10, 3.2, 1024, 1.3``
ALL  ``.DWRND 20, 0, 10``
ALL  ``.DWSIN 0.2, 10, 3.2, 1024, 1.3``
ALL  ``.ELSE``
ALL  ``.ENDA``
ALL  ``.ENDASM``
ALL  ``.ENDB``
ALL  ``.ENDE``
ALL  ``.ENDIF``
ALL  ``.ENDM``
ALL  ``.ENDME``
ALL  ``.ENDR``
ALL  ``.ENDRO``
ALL  ``.ENDS``
ALL  ``.ENDST``
ALL  ``.ENDU``
ALL  ``.ENUM $C000``
ALL  ``.ENUMID ID_1 0``
ALL  ``.EQU IF $FF0F``
ALL  ``.FAIL``
ALL  ``.FCLOSE FP_DATABIN``
ALL  ``.FOPEN "data.bin" FP_DATABIN``
ALL  ``.FREAD FP_DATABIN DATA``
ALL  ``.FSIZE FP_DATABIN SIZE``
ALL  ``.IF DEBUG == 2``
ALL  ``.IFDEF IF``
ALL  ``.IFDEFM \2``
ALL  ``.IFEQ DEBUG 2``
ALL  ``.IFEXISTS "main.s"``
ALL  ``.IFGR DEBUG 2``
ALL  ``.IFGREQ DEBUG 1``
ALL  ``.IFLE DEBUG 2``
ALL  ``.IFLEEQ DEBUG 1``
ALL  ``.IFNDEF IF``
ALL  ``.IFNDEFM \2``
ALL  ``.IFNEQ DEBUG 2``
ALL  ``.INCBIN "sorority.bin"``
ALL  ``.INCDIR "/usr/programming/gb/include/"``
ALL  ``.INC "cgb_hardware.i"``
ALL  ``.INCLUDE "cgb_hardware.i"``
658  ``.INDEX 8``
ALL  ``.INPUT NAME``
658  ``.LONG $102030, $405060``
658  ``.FARADDR main, irq_1``
ALL  ``.MACRO TEST``
ALL  ``.MEMORYMAP``
ALL  ``.NEXTU name``
ALL  ``.ORG $150``
ALL  ``.ORGA $150``
ALL  ``.PRINT "Numbers 1 and 10: ", DEC 1, " $", HEX 10, "\n"``
ALL  ``.PRINTT "Here we are...\n"``
ALL  ``.PRINTV DEC DEBUG+1``
ALL  ``.RAMSECTION "Vars" BANK 0 SLOT 1 ALIGN 256 OFFSET 32``
ALL  ``.REDEFINE IF $F``
ALL  ``.REDEF IF $F``
ALL  ``.REPEAT 6``
ALL  ``.REPT 6``
ALL  ``.ROMBANKMAP``
ALL  ``.ROMBANKS 2``
ALL  ``.ROMBANKSIZE $4000``
ALL  ``.ROW $ff00, 1, "3"``
ALL  ``.SEED 123``
ALL  ``.SECTION "Init" FORCE``
ALL  ``.SHIFT``
ALL  ``.SLOT 1``
ALL  ``.STRUCT enemy_object``
ALL  ``.STRINGMAPTABLE script "script.tbl"``
ALL  ``.STRINGMAP script "Hello\n"``
ALL  ``.SYM SAUSAGE``
ALL  ``.SYMBOL SAUSAGE``
ALL  ``.TABLE byte, word, byte``
ALL  ``.UNBACKGROUND $1000 $1FFF``
ALL  ``.UNDEFINE DEBUG``
ALL  ``.UNDEF DEBUG``
ALL  ``.UNION name``
ALL  ``.WORD 16000, 10, 255``
ALL  ``.ADDR 16000, main, 255``
=== ================================================================

Descriptions:

``.ENUMID ID_1 0``
------------------

``.ENUMID`` will create definitions with an autoincrementing value.
For example::

    .ENUMID 0
    .ENUMID ID_1
    .ENUMID ID_2
    .ENUMID ID_3

... will create the following definitions::

    ID_1 = 0
    ID_2 = 1
    ID_3 = 2

You can also specify the adder::

    .ENUMID 0 STEP 2
    .ENUMID MONSTER_ID_1
    .ENUMID MONSTER_ID_2
    .ENUMID MONSTER_ID_3

... to create definitions::

    MONSTER_ID_1 = 0
    MONSTER_ID_2 = 2
    MONSTER_ID_3 = 4

If you wish to export the definitions automatically, use EXPORT::

    .ENUMID 16 STEP 2 EXPORT
    .ENUMID MUSIC_1
    .ENUMID MUSIC_2
    .ENUMID MUSIC_3

... will create the following definitions and export them all::

    MUSIC_1 = 16
    MUSIC_2 = 18
    MUSIC_3 = 20

This is not a compulsory directive.


``.TABLE byte, word, byte``
---------------------------

Defines table's columns. With .DATA and .ROW you can define data much
like using .DB or .DW, but .TABLE makes it convenient to feed big
amounts of data in mixed format.

For example::

    .TABLE byte, word, byte

After the columns have been defined, you can define rows using e.g.,

    .ROW $01, $0302, $04

This is the same as::

    .DB $01
    .DW $0302
    .DB $04

Note that .DATA can also be used instead of .ROW, if one wants to
give the data in pieces.

All supported column formats:
    - DB, BYT, BYTE
    - DW, WORD, ADDR
    - DL, LONG, FARADDR ; wla-65816 only
    - DS, DSB
    - DSW
    - DSL               ; wla-65816 only

This is not a compulsory directive.


``.ROW $ff00, 1, "3"``
----------------------

Defines bytes after a .TABLE has been used to define the format.
An alternative way of defining bytes to .DB/.DW.

Note that when you use .ROW you'll need to give all the items
.TABLE defines, i.e. one full row. To give more or less bytes
use .DATA.

Example::

    .TABLE word, byte, word
    .ROW $aabb, "H", $ddee

This is the same as

    .DW $aabb
    .DB "H"
    .DW $ddee

This is not a compulsory directive.


``.DATA $ff00, 2``
------------------

Defines bytes after a .TABLE has been used to define the format.
An alternative way of defining bytes to .DB/.DW.

Note that when you use .DATA you can give as many items .TABLE
defines. The next time you'll use .DATA you'll continue from
the point the previous .DATA ended.

Examples::

    .TABLE dsw 2, dsb 2

This defines two rows worth of bytes::

    .DATA $ff00, $aabb, $10, $20, $1020, $3040, $50, $60

This does the same::

    .DATA $ff00, $aabb
    .DATA $10, $20
    .DATA $1020, $3040
    .DATA $50, $60
  
This is not a compulsory directive.


``.8BIT``
---------

There are a few mnemonics that look identical, but take different sized
arguments. Here's a list of such 6502 mnemonics:

ADC, AND, ASL, BIT, CMP, CPX, CPY, DEC, EOR, INC, LDA, LDX, LDY, ORA, ROL,
SBC, STA, STX and STY.

For example::

    LSR 11       ; $46 $0B
    LSR $A000    ; $4E $00 $A0

The first one could also be ::

    LSR 11       ; $4E $0B $00

``.8BIT`` is here to help WLA to decide to choose which one of the opcodes it
selects. When you give ``.8BIT`` (default) no 8-bit address/value is expanded
to 16-bits.

By default WLA uses the smallest possible size. This is true also when WLA
finds a computation it can't solve right away. WLA assumes the result will
be inside the smallest possible bounds, which depends on the type of the
mnemonic.

You can also use the fixed argument size versions of such mnemonics by
giving the size with the operand (i.e., operand hinting). Here are few
examples::

    LSR 11.B   ; $46 $0B
    LSR 11.W   ; $4E $0B $00

In WLA-65816 ``.ACCU``/``.INDEX``/``SEP``/``REP`` override
``.8BIT``/``.16BIT``/``.24BIT`` when considering the immediate values, so be
careful. Still, operand hints override all of these, so use them to be sure.

This is not a compulsory directive.


``.16BIT``
----------

Analogous to ``.8BIT``. ``.16BIT`` forces all addresses and immediate values to
be expanded into 16-bit range, when possible, that is. ::

    LSR 11       ; $46 $0B

that would be the case, normally, but after ``.16BIT`` it becomes ::

    LSR 11       ; $4E $0B $00

This is not a compulsory directive.


``.24BIT``
----------

Analogous to ``.8BIT`` and ``.16BIT``. ``.24BIT`` forces all addresses to
be expanded into 24-bit range, when possible, that is. ::

    AND $11       ; $25 $11

that would be the case, normally, but after ``.24BIT`` it becomes ::

    AND $11       ; $2F $11 $00 $00

If it is not possible to expand the address into ``.24BIT`` range,
then WLA tries to expand it into 16-bit range.

This is not a compulsory directive.


``.ACCU 8``
-----------

Forces WLA to override the accumulator size given with ``SEP``/``REP``.
``.ACCU`` doesn't produce any code, it only affects the way WLA interprets the
immediate values (8 for 8 bit operands, 16 for 16 bit operands) for opcodes
dealing with the accumulator.

So after giving ``.ACCU 8`` ::

    AND #6

will produce ``$29 $06``, and after giving ``.ACCU 16`` ::

    AND #6

will yield ``$29 $00 $06``.

Note that ``SEP``/``REP`` again will in turn reset the accumulator/index
register size.

This is not a compulsory directive.


``.INDEX 8``
------------

Forces WLA to override the index (``X``/``Y``) register size given with
``SEP``/``REP``. ``.INDEX`` doesn't produce any code, it only affects the way
WLA interprets the immediate values (``8`` for 8 bit operands, ``16`` for 16
bit operands) for opcodes dealing with the index registers.

So after giving ``.INDEX 8`` ::

    CPX #10

will produce ``$E0 $A0``, and after giving ``.INDEX 16`` ::

    CPX #10

will yield ``$E0 $00 $A0``.

Note that ``SEP``/``REP`` again will in turn reset the accumulator/index
register size.

This is not a compulsory directive.


``.WDC``
--------

Turns WLA-65816 into a mode where it accepts WDC standard assembly code, in
addition to WLA's own syntax. In WDC standard mode ::

    AND <x  ; 8-bit
    AND |?  ; 16-bit
    AND >&  ; 24-bit

are the same as ::
    
    AND x.b ; 8-bit
    AND ?.w ; 16-bit
    AND &.l ; 24-bit

in WLA's own syntax. Beware of the situations where you use '<' and '>' to
get the low and high bytes!

This is not a compulsory directive.


``.NOWDC``
----------

Turns WLA-65816 into a mode where it accepts its default syntax assembly
code, which doesn't support WDC standard. This is the default mode for
WLA-65816.

This is not a compulsory directive.


``.ASM``
--------

Tells WLA to start assembling. Use ``.ASM`` to continue the work which has been
disabled with ``.ENDASM``. ``.ASM`` and ``.ENDASM`` can be used to mask away
big blocks of code. This is analogous to the ANSI C -comments (``/*...*/``),
but ``.ASM`` and ``.ENDASM`` can be nested, unlike the ANSI C -counterpart.

This is not a compulsory directive.


``.ENDASM``
-----------

Tells WLA to stop assembling. Use ``.ASM`` to continue the work.

This is not a compulsory directive.


``.DBRND 20, 0, 10``
--------------------

Defines bytes, just like ``.DSB`` does, only this time they are filled with
(pseudo) random numbers. We use the integrated Mersenne Twister to generate
the random numbers. If you want to seed the random number generator,
use ``.SEED``.

The first parameter (``20`` in the example) defines the number of random
numbers we want to generate. The next two tell the range of the random
numbers, i.e. min and max.

Here's how it works::

    .DBRND A, B, C

    for (i = 0; i < A; i++)
      output_data((rand() % (C-B+1)) + B);

This is not a compulsory directive.


``.DWRND 20, 0, 10``
--------------------

Analogous to ``.DBRND`` (but defines words).

This is not a compulsory directive.


``.DBCOS 0.2, 10, 3.2, 120, 1.3``
---------------------------------

Defines bytes just like ``.DSB`` does, only this time they are filled with
cosine data. ``.DBCOS`` takes five arguments.

The first argument is the starting angle. Angle value ranges from ``0`` to
``359.999``..., but you can supply WLA with values that are out of the range -
WLA fixes them ok. The value can be integer or float.

The second argument descibes the amount of additional angles. The example
will define 11 angles.

The third argument is the adder value which is added to the angle value when
next angle is calculated. The value can be integer or float.

The fourth and fifth arguments can be seen from the pseudo code below, which
also describes how ``.DBCOS`` works. The values can be integer or float.

Remember that ``cos`` (and ``sin``) here returns values ranging from
``-1`` to ``1``. ::

    .DBCOS A, B, C, D, E

    for (B++; B > 0; B--) {
      output_data((D * cos(A)) + E)
      A = keep_in_range(A + C)
    }

This is not a compulsory directive.


``.DBSIN 0.2, 10, 3.2, 120, 1.3``
---------------------------------

Analogous to ``.DBCOS``, but does ``sin()`` instead of ``cos()``.

This is not a compulsory directive.


``.DWCOS 0.2, 10, 3.2, 1024, 1.3``
----------------------------------

Analogous to ``.DBCOS`` (but defines 16-bit words).

This is not a compulsory directive.


``.DWSIN 0.2, 10, 3.2, 1024, 1.3``
----------------------------------

Analogous to ``.DBCOS`` (but defines 16-bit words and does ``sin()`` instead of
``cos()``).

This is not a compulsory directive.


``.NAME "NAME OF THE ROM"``
---------------------------

If ``.NAME`` is used with WLA-GB then the 16 bytes ranging from ``$0134``
to ``$0143`` are filled with the provided string. WLA-65816 fills
the 21 bytes from ``$FFC0`` to ``$FFD4`` in HiROM and from ``$7FC0`` to
``$7FD4`` in LoROM mode with the name string (SNES ROM title). For ExHiROM
the ranges are from ``$40FFC0`` to ``$40FFD4`` and from ``$FFC0`` to ``$FFD4``
(mirrored).

If the string is shorter than 16/21 bytes the remaining space is
filled with ``$00``.

This is not a compulsory directive.


``.ROMBANKS 2``
---------------

Indicates the size of the ROM in rombanks. This value is converted to a
standard Gameboy ROM size indicator value found at ``$148`` in a Gameboy ROM,
and there this one is put into.

This is a compulsory directive unless ``.ROMBANKMAP`` is defined.

You can redefine ``.ROMBANKS`` as many times as you wish as long as
the old and the new ROM bank maps match as much as possible. This
way you can enlarge the size of the project on the fly.


``.RAMSIZE 0``
--------------

Indicates the size of the RAM. This is a standard Gameboy RAM size indicator
value found at ``$149`` in a Gameboy ROM, and there this one is put to also.

This is not a compulsory directive.


``.EMPTYFILL $C9``
------------------

This byte is used in filling the unused areas of the ROM file. ``EMPTYFILL``
defaults to ``$00``.

This is not a compulsory directive.


``.CARTRIDGETYPE 1``
--------------------

Indicates the type of the cartridge (mapper and so on). This is a standard
Gameboy cartridge type indicator value found at ``$147`` in a Gameboy ROM, and
there this one is put to also.

This is not a compulsory directive.


``.COUNTRYCODE 1``
------------------

Indicates the country code located at ``$14A`` of a Gameboy ROM.

This is not a compulsory directive.


``.VERSION 1``
------------------

Indicates the Mask ROM version number located at ``$14C`` of a Gameboy ROM.

This is not a compulsory directive.


``.DESTINATIONCODE 1``
----------------------

``.DESTINATIONCODE`` is an alias for ``.COUNTRYCODE``.

This is not a compulsory directive.


``.NINTENDOLOGO``
-----------------

Places the required Nintendo logo into the Gameboy ROM at ``$104``.

This is not a compulsory directive.


``.GBHEADER``
-------------

This begins the GB header definition, and automatically defines
``.COMPUTEGBCHECKSUM``. End the header definition with .ENDGB.
Here's an example::

    .GBHEADER
        NAME "TANKBOMBPANIC"  ; identical to a freestanding .NAME.
        LICENSEECODEOLD $34   ; identical to a freestanding .LICENSEECODEOLD.
        LICENSEECODENEW "HI"  ; identical to a freestanding .LICENSEECODENEW.
        CARTRIDGETYPE $00     ; identical to a freestanding .CARTRIDGETYPE.
        RAMSIZE $09           ; identical to a freestanding .RAMSIZE.
        COUNTRYCODE $01       ; identical to a freestanding .COUNTRYCODE/DESTINATIONCODE.
        DESTINATIONCODE $01   ; identical to a freestanding .DESTINATIONCODE/COUNTRYCODE.
        NINTENDOLOGO          ; identical to a freestanding .NINTENDOLOGO.
	VERSION $01           ; identical to a freestanding .VERSION.
        ROMDMG                ; identical to a freestanding .ROMDMG.
                              ; Alternatively, ROMGBC or ROMGBCONLY can be used
    .ENDGB

This is not a compulsory directive.


``.SMSHEADER``
--------------

All the fields in ``.SMSHEADER`` are optional and default to zero except ROMSIZE. If
ROMSIZE is not specified it will be calculated automatically::

    .SMSHEADER
        PRODUCTCODE 26, 70, 2 ; 2.5 bytes
        VERSION 1             ; 0-15
        REGIONCODE 4          ; 3-7
        RESERVEDSPACE 0, 0    ; 2 bytes
	ROMSIZE 0             ; 0-15
    .ENDSMS

The ``REGIONCODE`` also defines the system:

======= ==================
 ``3``   SMS Japan
 ``4``   SMS Export
 ``5``   GG Japan
 ``6``   GG Export
 ``7``   GG International
======= ==================

When ``.SMSHEADER`` is defined, also the checksum is calculated, and TMR SEGA,
two reserved bytes and ROM size are defined.

See http://www.smspower.org/Development/ROMHeader for more information about
SMS header.

This is not a compulsory directive.


``.LICENSEECODEOLD $1A``
------------------------

This is a standard old licensee code found at ``$14B`` in a Gameboy ROM, and there
this one is put to also. ``.LICENSEECODEOLD`` cannot be defined with
``.LICENSEECODENEW``.

This is not a compulsory directive.


``.LICENSEECODENEW "1A"``
-------------------------

This is a standard new licensee code found at ``$144`` and ``$145`` in a
Gameboy ROM, and there this one is put to also. ``.LICENSEECODENEW`` cannot be
defined with .LICENSEECODEOLD. ``$33`` is inserted into ``$14B``, as well.

This is not a compulsory directive.


``.COMPUTEGBCHECKSUM``
----------------------

When this directive is used WLA computes the ROM checksum found at ``$14E`` and
``$14F`` in a Gameboy ROM. Note that this directive can only be used with
WLA-GB.

Note that you can also write ``.COMPUTECHECKSUM`` (the old name for this
directive), but it's not recommended.

This is not a compulsory directive.


``.COMPUTESMSCHECKSUM``
-----------------------

When this directive is used WLA computes the ROM checksum found at ``$7FFA``
and ``$7FFB`` (or ``$3FFA`` - ``$3FFB`` is the ROM is 16KBs, or
``$1FFA`` - ``$1FFB`` for 8KB ROMs) in a SMS/GG ROM. Note that this directive
can only be used with WLA-z80. Also note that the ROM size must be at least
8KBs. The checksum is calculated using bytes
``0x0000`` - ``0x1FEF``/``0x3FEF``/``0x7FEF``.

This is not a compulsory directive.


``.COMPUTESNESCHECKSUM``
------------------------

When this directive is used WLA computes the SNES ROM checksum and
inverse checksum found at ``$7FDC`` - ``$7FDF`` (LoROM), ``$FFDC`` - ``$FFDF``
(HiROM) or ``$40FFDC`` - ``$40FFDF`` and ``$FFDC`` - ``$FFDF`` (ExHiROM).
Note that this directive can only be used with WLA-65816. Also note
that the ROM size must be at least 32KB for LoROM images, 64KB for
HiROM images and 32.5MBit for ExHiROM.

``.LOROM``, ``.HIROM`` or ``.EXHIROM`` must be issued before ``.COMPUTESNESCHECKSUM``.

This is not a compulsory directive.


``.SMSTAG``
-----------

``.SMSTAG`` forces WLA to write an ordinary SMS/GG ROM tag to the ROM file.
Currently only the string ``TMR SEGA`` and ROM checksum are written
(meaning that ``.SMSTAG`` also defines ``.COMPUTESMSCHECKSUM``). The ROM size
must be at least 8KBs.

This is not a compulsory directive.


``.SDSCTAG 1.0, "DUNGEON MAN", "A wild dungeon exploration game", "Ville Helin"``
---------------------------------------------------------------------------------

``.SDSCTAG`` adds SDSC tag to your SMS/GG ROM file. The ROM size must be at
least 8KB just like with ``.COMPUTESMSCHECKSUM`` and ``.SMSTAG``. For more
information about this header take a look at http://www.smspower.org/dev/sdsc/.
Here's an explanation of the arguments::

    .SDSCTAG {version number}, {program name}, {program release notes}, {program author}

Note that program name, release notes and program author can also be pointers
to strings instead of being only strings (which WLA terminates with zero, and
places them into suitable locations inside the ROM file). So ::

    .SDSCTAG 0.8, PRGNAME, PRGNOTES, PRGAUTHOR
    ...
    PRGNAME:  .DB "DUNGEON MAN", 0
    PRGNOTES: .DB "A wild and totally crazy dungeon exploration game", 0
    PRGAUTHOR:.DB "Ville Helin", 0

works also. All strings supplied explicitly to ``.SDSCTAG`` are placed
somewhere in ``.BANK 0 SLOT 0``. ::

    .SDSCTAG 1.0, "", "", ""
    .SDSCTAG 1.0, 0, 0, 0

are also valid, here ``0`` and ``""`` mean the user doesn't want to use any
descriptive strings. Version number can also be given as an integer, but then
the minor version number defaults to zero.

``.SDSCTAG`` also defines ``.SMSTAG`` (as it's part of the SDSC ROM tag
specification).

This is not a compulsory directive.


``.COMPUTEGBCOMPLEMENTCHECK``
-----------------------------

When this directive is used WLA computes the ROM complement check found at
``$14D`` in a Gameboy ROM.

Note that you can still use ``.COMPUTECOMPLEMENTCHECK`` (the old name for this
directive), but it's not recommended.

This is not a compulsory directive.


``.INCDIR "/usr/programming/gb/include/"``
------------------------------------------

Changes the current include root directory. Use this to specify main
directory for the following ``.INCLUDE`` and ``.INCBIN`` directives.
If you want to change to the current working directory (WLA also defaults
to this), use ::

    .INCDIR ""

If the ``INCDIR`` is specified in the command line, that directory will be
searched before the ``.INCDIR`` in the file. If the file is not found, WLA
will then silently search the specified ``.INCDIR``.

This is not a compulsory directive.


``.INC "cgb_hardware.i"``
-----------------------------

``INC`` is an alias for ``INCLUDE``.

This is not a compulsory directive.


``.INCLUDE "cgb_hardware.i"``
-----------------------------

Includes the specified file to the source file. If the file's not found
in the ``.INCDIR`` directory, WLA tries to find it in the current working
directory. If the ``INCDIR`` is specified in the command line, WLA will first
try to find the file specified in that directory. Then proceed as mentioned
before if it is not found.

If you want to prefix all labels inside the included file with something, use ::

    .INCLUDE "music_player.s" NAMESPACE "musicplayer"

In the case of this example, all sections, macros, labels and references to
those labels inside the included file are prefixed with "musicplayer.", though
there are a couple of exceptions. If a ``.SECTION`` inside the included file has
its own namespace, the ``.INCLUDE`` 's namespace doesn't affect it. If a ``.SECTION``
inside the included file uses ``APPENDTO`` with a section name that starts with
``"*:"``, that ``APPENDTO`` is considered to belong to the global namespace and we
won't prefix it with the ``.INCLUDE`` 's namespace.

Note that you can create the file name from pieces ::

    .INCLUDE ROOTDIR, SUBDIR, "cthulhu.s" NAMESPACE "cthulhu"

This might end up looking for a file "root/subdir/cthulhu.s", depending on the
definitions.

If you are using the ``.INCLUDE`` inside a ``.MACRO`` and want to have the file
included only once, use the keyword ``ONCE`` ::

    .INCLUDE "include_one.s" NAMESPACE "once" ONCE
  
This is not a compulsory directive.


``.INCBIN "sorority.bin"``
--------------------------

Includes the specified data file into the source file. ``.INCBIN`` caches
all files into memory, so you can ``.INCBIN`` any data file millions of
times, but it is loaded from hard drive only once.

You can optionally use ``SWAP`` after the file name, e.g., ::

    .INCBIN "kitten.bin" SWAP

``.INCBIN`` data is divided into blocks of two bytes, and inside every block
the bytes are exchanged (like ``SWAP r`` does to nibbles). This requires that
the size of the file is even.

You can also force WLA to skip n bytes from the beginning of the file
by writing for example::

    .INCBIN "kitten.bin" SKIP 4

Four bytes are skipped from the beginning of ``kitten.bin`` and the rest
is incbinned.

It is also possible to incbin only n bytes from a file::

    .INCBIN "kitten.bin" READ 10

Will read ten bytes from the beginning of ``kitten.bin``.

You can also force WLA to create a definition holding the size
of the file::

    .INCBIN "kitten.bin" FSIZE size_of_kitten

Want to circulate all the included bytes through a filter macro? Do this::

    .INCBIN "kitten.bin" FILTER filtermacro

The filter macro is executed for each byte of the included data, data
byte being the first argument, and offset from the beginning being the
second parameter, just like in the case of ``.DBM``, ``.DWM`` and ``.DLM``.

And you can combine all these four commands::

    .INCBIN "kitten.bin" SKIP 10 READ 8 SWAP FSIZE size_of_kitten FILTER filtermacro

This example shows how to incbin eight bytes (swapped) after skipping
10 bytes from the beginning of file ``kitten.bin``, and how to get the
size of the file into a definition label ``size_of_kitten``. All the data bytes
are circulated through a filter macro.

Here's an example of a filter macro that increments all the bytes by one::

    .macro filtermacro    ; the input byte is \1, the output byte is in "_out"
    .redefine _out \1+1
    .endm

Note that the order of the extra commands is important.

If the file's not found in the ``.INCDIR`` directory, WLA tries to find it
in the current working directory. If the ``INCDIR`` is specified in the command
line, WLA will first search for the file in that directory. If not found, it
will then proceed as aforementioned.

This is not a compulsory directive.


``.INPUT NAME``
---------------

``.INPUT`` is much like any Basic-language input: ``.INPUT`` asks the user
for a value or string. After ``.INPUT`` is the variable name used to store
the data.

``.INPUT`` works like ``.REDEFINE``, but the user gets to type in the data.

Here are few examples how to use input::

    .PRINTT "The name of the ROM? "
    .INPUT NAME
    .NAME NAME
    
    ...
    
    .PRINTT "Give the .DB amount.\n"
    .INPUT S
    .PRINTT "Give .DB data one at a time.\n"
    .REPEAT S
      .INPUT B
      .DB B
    .ENDR
    
    ...

This is not a compulsory directive.


``.BACKGROUND "parallax.gb"``
-----------------------------

This chooses an existing ROM image (``parallax.gb`` in this case) as a
background data for the project. You can overwrite the data with ``OVERWRITE``
sections only, unless you first clear memory blocks with ``.UNBACKGROUND``
after which there's room for other sections as well.

Note that ``.BACKGROUND`` can be used only when compiling an object file.

``.BACKGROUND`` is useful if you wish to patch an existing ROM image with
new code or data.

This is not a compulsory directive.


``.UNBACKGROUND $1000 $1FFF``
-----------------------------

After issuing ``.BACKGROUND`` you might want to free some parts of the
backgrounded ROM image for e.g., ``FREE`` sections. With ``.UNBACKGROUND``
you can define such regions. In the example a block starting at
``$1000`` and ending at ``$1FFF`` was released (both ends included). You can
issue ``.UNBACKGROUND`` as many times as you wish.

This is not a compulsory directive.


``.FAIL``
---------

Terminates the compiling process.

This is not a compulsory directive.


``.FCLOSE FP_DATABIN``
----------------------

Closes the filehandle ``FP_DATABIN``.

This is not a compulsory directive.


``.FOPEN "data.bin" FP_DATABIN``
--------------------------------

Opens the file ``data.bin`` for reading and associates the filehandle with
name ``FP_DATABIN``.

This is not a compulsory directive.


``.FREAD FP_DATABIN DATA``
--------------------------

Reads one byte from ``FP_DATABIN`` and creates a definition called
``DATA`` to hold it. ``DATA`` is an ordinary definition label, so you can
``.UNDEFINE`` it.

Here's an example on how to use ``.FREAD``::

    .fopen "data.bin" fp
    .fsize fp t
    .repeat t
    .fread fp d
    .db d+26
    .endr
    .undefine t, d

This is not a compulsory directive.


``.FSIZE FP_DATABIN SIZE``
--------------------------

Creates a definition called ``SIZE``, which holds the size of the file
associated with the filehandle ``FP_DATABIN``. ``SIZE`` is an ordinary
definition label, so you can ``.UNDEFINE`` it.

This is not a compulsory directive.


``.MACRO TEST``
---------------

Begins a macro called ``TEST``.

You can use ``\@`` inside a macro to e.g., separate a label from the other
macro ``TEST`` occurrences. ``\@`` is replaced with an integer number
indicating the amount of times the macro has been called previously so
it is unique to every macro call. ``\@`` can also be used inside strings
inside a macro or just as a plain value. Look at the following examples
for more information.

You can also type ``\!`` to get the name of the source file currently being
parsed. ``\.`` can be used the same way to get the name of the macro.

Also, if you want to use macro arguments in e.g., calculation, you can
type ``\X`` where ``X`` is the number of the argument. Another way to refer
to the arguments is to use their names given in the definition of the
macro (see the examples for this).

Remember to use ``.ENDM`` to finish the macro definition. Note that you
cannot use ``.INCLUDE`` inside a macro. Note that WLA's macros are in fact
more like procedures than real macros, because WLA doesn't substitute
macro calls with macro data. Instead WLA jumps to the macro when it
encounters a macro call at compile time.

You can call macros from inside a macro. Note that the preprocessor
does not expand the macros. WLA traverses through the code according to
the macro calls.

Here are some examples::

    .MACRO NOPMONSTER
        .REPT 32         ; gives us 32 NOPs
        NOP
        .ENDR
    .ENDM
    
    .MACRO LOAD_ABCD
        LD A, \1
        LD B, \2
        LD C, \3
        LD D, :\4        ; load the bank number of \4 into register D.
        NOPMONSTER       ; note that \4 must be a label or ROM address
        LD HL, 1<<\1     ; for this to work...
    .INCBIN \5
    .ENDM
    
    .MACRO QUEEN
    
    QUEEN\@:
        LD   A, \1
    	LD   B, \1
    	CALL QUEEN\@

    	.DB  "\@", 0     ; will translate into a zero terminated string
                         ; holding the amount of macro QUEEN calls.
        .DB  "\\@", 0    ; will translate into a string containing
                         ; \@.
        .DB  \@          ; will translate into a number indicating
                         ; the amount of macro QUEEN calls.

    .ENDM
    
    .MACRO LOAD_ABCD_2 ARGS ONE, TWO, THREE, FOUR, FIVE
        LD A, ONE
        LD B, TWO
        LD C, THREE
        LD D, FOUR
        NOPMONSTER
        LD HL, 1<<ONE
    .INCBIN FIVE
    .ENDM
    
    .MACRO TEST NARGS 3
        .DB \1, \2, \3
    .ENDM

And here's how they can be used::

    NOPMONSTER
    LOAD_ABCD $10, $20, $30, XYZ, "merman.bin"
    QUEEN 123
    LOAD_ABCD_2 $10, $20, $30, XYZ, "merman.bin"
    TEST 1, 2, 3

Note that you must separate the arguments with commas.

Here is a special case::

    .DEF prev_test $0000

    .MACRO .test ARGS str
    __\._\@+1:                     ; this will become __.test_1 during
        .PRINT __\._\@+1, "\n"     ; the first call, __.test_2 during the
        .WORD  prev_test           ; second call...
        .REDEF prev_test __\._\@+1
        .BYTE  str.length, str, 0
    .ENDM

When creating a label inside a macro, you can add a super simple
addition or subtraction after ``\@`` to adjust the value. Only one
digit number is supported.

If you want to give names to the macro's arguments you can do that
by listing them in order after supplying ARGS after the macro's name.

Every time a macro is called a definition ``NARGS`` is created. It shows
only inside the macro and holds the number of arguments the macro
was called with. So don't have your own definition called ``NARGS``.
Here's an example::

    .MACRO LUPIN
      .IF NARGS != 1
        .FAIL
      .ENDIF
      
      .PRINTT "Totsan! Ogenki ka?\n"
    .ENDM

You can also use ``\?`` to ask for the type of the argument in the
following fashion::
    
    .macro .differentThings
      .if \?1 == ARG_NUMBER
        .db 1
      .endif
      .if \?1 == ARG_STRING
        .db 2
      .endif
      .if \?1 == ARG_LABEL
        .db 3
      .endif
      .if \?1 == ARG_PENDING_CALCULATION
        .db 4
      .endif
    .endm
  
    .section "TestingDifferentThings"
    TDT1:
        .differentThings 100
        .differentThings "HELLO"
        .differentThings TDT1
        .differentThings TDT1+1
    .ends

The previous example will result in .db 1, 2, 3, 4
    
This is not a compulsory directive.


``.ENDM``
---------

Ends a ``.MACRO``.

This is not a compulsory directive, but when ``.MACRO`` is used this one is
required to terminate it.

``.SHIFT``
----------

Shifts the macro arguments one down (``\2`` becomes ``\1``, ``\3`` -> ``\2``,
etc.). ``.SHIFT`` can thus only be used inside a ``.MACRO``.

This is not a compulsory directive.


``.FASTROM``
------------

Sets the ROM memory speed bit in ``$FFD5`` (``.HIROM``), ``$7FD5`` (``.LOROM``)
or ``$FFD5`` and ``$40FFD5`` (``.EXHIROM``) to indicate that the SNES ROM chips
are 120ns chips.

This is not a compulsory directive.


``.SLOWROM``
------------

Clears the ROM memory speed bit in ``$FFD5`` (``.HIROM``), ``$7FD5`` (``.LOROM``)
or ``$FFD5`` and ``$40FFD5`` (``.EXHIROM``) to indicate that the SNES ROM chips
are 200ns chips.


This is not a compulsory directive.


``.SMC``
--------

Forces WLALINK to compute a proper SMC header for the ROM file.

SMC header is a chunk of 512 bytes. WLALINK touches only its first three
bytes, and sets the rest to zeroes. Here's what will be inside the first
three bytes:

====== ===================================================================
 Byte   Description
------ -------------------------------------------------------------------
``0``   low byte of 8KB page count.
``1``   high byte of 8KB page count.
``2``   * Bit ``7``: ``0``
        * Bit ``6``: ``0``
        * Bit ``5``: ``0`` = LoROM, ``1`` = HiROM
        * Bit ``4``: ``0`` = LoROM, ``1`` = HiROM
        * Bit ``3`` and ``2``: SRAM size (``00`` = 256Kb, ``01`` = 64Kb,
          ``10`` = 16Kb, ``11`` = 0Kb)
        * Bit ``1``: ``0``
        * Bit ``0``: ``0``
====== ===================================================================

This is not a compulsory directive.


``.HIROM``
----------

With this directive you can define the SNES ROM mode to be HiROM.
Issuing ``.HIROM`` will override the user's ROM bank map when
WLALINK computes 24-bit addresses and bank references. If no
``.HIROM``, ``.LOROM`` or ``.EXHIROM`` are given then WLALINK obeys the
banking defined in ``.ROMBANKMAP``.

``.HIROM`` also sets the ROM mode bit in ``$FFD5``.

This is not a compulsory directive.

``.EXHIROM``
------------

With this directive you can define the SNES ROM mode to be ExHiROM.
Issuing ``.EXHIROM`` will override the user's ROM bank map when
WLALINK computes 24-bit addresses and bank references. If no
``.HIROM``, ``.LOROM`` or ``.EXHIROM`` are given then WLALINK obeys the
banking defined in ``.ROMBANKMAP``.

``.EXHIROM`` also sets the ROM mode bit in ``$40FFD5`` (mirrored in
``$FFD5``).

This is not a compulsory directive.

``.LOROM``
----------

With this directive you can define the SNES ROM mode to be LoROM.
Issuing ``.LOROM`` will override the user's ROM bank map when
WLALINK computes 24-bit addresses and bank references. If no
``.HIROM``, ``.LOROM`` or ``.EXHIROM`` are given then WLALINK obeys the
banking defined in ``.ROMBANKMAP``.

WLA defaults to ``.LOROM``.

This is not a compulsory directive.


``.BASE $80``
-------------

Defines the base value for the bank number (used only in 24-bit addresses and
when getting a label's bank number with ``:``). Here are few examples of how
to use ``.BASE`` (both examples assume the label resides in the first ROM
bank)::

    .BASE $00
    label1:
    .BASE $80
    label2:

      JSL label1   ; if label1 address is $1234, this will assemble into
                   ; JSL $001234
      JSL label2   ; label2 is also $1234, but this time the result will be
                   ; JSL $801234

``.BASE`` defaults to ``$00``. Note that the address of the label will also
contribute to the bank number (bank number == ``.BASE`` + ROM bank of the
label).

On 65816, use ``.LOROM``, ``.HIROM`` or ``.EXHIROM`` to define the ROM mode. 

This is not a compulsory directive.


``.BLOCK "Block1"``
-------------------

Begins a block (called ``Block1`` in the example). These blocks have only
one function: to display the number of bytes they contain. When you
embed such a block into your code, WLA displays its size when it assembles
the source file.

Use ``.ENDB`` to terminate a ``.BLOCK``. Note that you can nest ``.BLOCK`` s.

This is not a compulsory directive.


``.ENDB``
---------

Terminates ``.BLOCK``.

This is not a compulsory directive, but when ``.BLOCK`` is used this one is
required to terminate it.


``.BANK 0 SLOT 1``
------------------

Defines the ROM bank and the slot it is inserted into in the memory. You
can also type the following::

    .BANK 0

This tells WLA to move into BANK 0 which will be put into the ``DEFAULTSLOT``
of ``.MEMORYMAP``.

Every time you use ``.BANK``, supply ``.ORG``/``.ORGA`` as well, just to make
sure WLA calculates addresses correctly.

This is a compulsory directive.


``.SLOT 1``
-----------

Changes the currently active memory slot. This directive is meant to be
used with ``SUPERFREE`` sections, where only the slot number is constant
when placing the sections.

You can use the number, address or name of the slot here::

    .SLOT 1           ; Use slot 1.
    .SLOT $2000       ; Use a slot with starting address of $2000.
    .SLOT "SlotOne"   ; Use a slot with a name "SlotOne"

This is not a compulsory directive.


``.ROMBANKSIZE $4000``
----------------------

Defines the ROM bank size. Old syntax is ``.BANKSIZE x``.

This is a compulsory directive unless ``.ROMBANKMAP`` is defined.


``.ORG $150``
-------------

Defines the starting address. The value supplied here is relative to the
ROM bank given with ``.BANK``.

When WLA starts to parse a source file, ``.ORG`` is set to ``$0``, but it's
always a good idea to explicitly use ``.ORG``, for clarity.

This is a compulsory directive.


``.ORGA $150``
--------------

Defines the starting address. The value supplied here is absolute and used
directly in address computations. WLA computes the right position in
ROM file. By using ``.ORGA`` you can instantly see from the source file where
the following code is located in the 16-bit memory.

Here's an example::

    .MEMORYMAP
    SLOTSIZE $4000
    DEFAULTSLOT 0
    SLOT 0 $0000
    SLOT 1 $4000
    .ENDME
    
    .ROMBANKMAP
    BANKSTOTAL 2
    BANKSIZE $4000
    BANKS 2
    .ENDRO
    
    .BANK 0 SLOT 1
    .ORGA $4000
    
    MAIN:	JP	MAIN

Here ``MAIN`` is at ``$0000`` in the ROM file, but the address for label
``MAIN`` is ``$4000``. By using ``.ORGA`` instead of ``.ORG``, you can directly
see from the value the address where you want the code to be as ``.ORG`` is
just an offset to the ``SLOT``.


``.DS 256, $10``
----------------

``.DS`` is an alias for ``.DSB``.

This is not a compulsory directive.


``.DSB 256, $10``
-----------------

Defines ``256`` bytes of ``$10``.

This is not a compulsory directive.


``.DSTRUCT waterdrop INSTANCEOF water VALUES``
--------------------------------------------------------------

Defines an instance of struct water, called waterdrop, and fills
it with the given data. Before calling ``.DSTRUCT`` we must have defined
the structure, and in this example it could be like::

    .STRUCT water
        name   ds 8
        age    db
        weight dw
    .ENDST

There are two syntaxes for .DSTRUCT; the new and legacy versions. To use
the new syntax, put the keyword "VALUES" at the end of the first line.
The old syntax uses the keyword "DATA" or none at all.

The new syntax looks like this::

    .DSTRUCT waterdrop INSTANCEOF water VALUES
        name:   .db "tingle"
        age:    .db 40
        weight: .dw 120
    .ENDST

The fields can be put in any order. Any omitted fields are set to the
``.EMPTYFILL`` value (``$00`` by default). Any data-defining directive
can be used within .DSTRUCT, as long as it does not exceed the size of
the data it is being defined for. The only exception is .DSTRUCT itself,
which cannot be nested.

The old syntax looks like this::

    .DSTRUCT waterdrop INSTANCEOF water DATA "tingle", 40, 120

The ``DATA`` and ``INSTANCEOF`` keywords are optional. This will assign
data for each field of the struct in the order they were defined.

In either example you would get the following labels::

    waterdrop
    waterdrop.name
    waterdrop.age
    waterdrop.weight
    _sizeof_waterdrop        = 11
    _sizeof_waterdrop.name   = 8
    _sizeof_waterdrop.age    = 1
    _sizeof_waterdrop.weight = 2

The legacy syntax does not support unions; it will give an error if you
attempt to define data for a union.

For the new syntax, nested structs are supported like so (assume the
``water`` struct is also defined::

    .STRUCT drop_pair
        waterdrops: instanceof water 2
    .ENDST

    .DSTRUCT drops INSTANCEOF drop_pair VALUES
        waterdrops.1:        .db "qwertyui" 40
                             .dw 120
        waterdrops.2.name:   .db "tingle"
        waterdrops.2.age:    .db 40
        waterdrops.2.weight: .dw 12
    .ENDST

In this case, the properties of ``waterdrops.1`` were defined
implicitly; 8 bytes for the name, followed by a byte for the age,
followed by a word for the weight. The values for ``waterdrops.2`` were
defined in a more clear way.

In this case, ``waterdrops`` and ``waterdrops.1`` are equivalent.
``waterdrops.1.name`` is different, even though its address is the same,
because it has a size of 8. If you attempted to do this::

    .DSTRUCT drops INSTANCEOF drop_pair VALUES
        waterdrops.1.name:   .db "qwertyui" 40
                             .dw 120
    .ENDST

It would fail, because only the 8 name bytes are available to be defined
in this context, as opposed to the 11 bytes for the entire
``waterdrops.1`` structure.

Named unions can be assigned to in a similar way, by writing its full
name with a ``.`` separating the union name and the field name.

The struct can be defined namelessly, in which case no labels will be
generated, like so::

    .DSTRUCT INSTANCEOF drop_pair VALUES
        ...
    .ENDST

This is not a compulsory directive.


``.DSW 128, 20``
----------------

Defines ``128`` words (two bytes) of ``20``.

This is not a compulsory directive.


``.DSL 16, $102030``
--------------------

Defines ``16`` long words (three bytes) of ``$102030``. Works only
on wla-65816.

This is not a compulsory directive.


``.DB 100, $30, %1000, "HELLO WORLD!"``
---------------------------------------

Defines bytes.

This is not a compulsory directive.


``.BYT 100, $30, %1000, "HELLO WORLD!"``
----------------------------------------

``.BYT`` is an alias for ``.DB``.

This is not a compulsory directive.


``.DBM filtermacro 1, 2, "encrypt me"``
---------------------------------------

Defines bytes using a filter macro. All the data is passed to ``filtermacro``
in the first argument, one byte at a time, and the byte that actually gets
defined is the value of definition ``_OUT`` (``_out`` works as well). The
second macro argument holds the offset from the beginning (the first byte) in
bytes (the series being ``0``, ``1``, ``2``, ``3``, ...).

Here's an example of a filter macro that increments all the bytes by one::

    .macro increment
    .redefine _out \1+1
    .endm

This is not a compulsory directive.


``.SYM SAUSAGE``
----------------

WLA treats symbols (``SAUSAGE`` in this example) like labels, but they
only appear in the symbol files WLALINK outputs. Useful for finding out
the location where WLALINK puts data.

This is not a compulsory directive.


``.SYMBOL SAUSAGE``
-------------------

``.SYMBOL`` is an alias for ``.SYM``.

This is not a compulsory directive.


``.BR``
-------

Inserts a breakpoint that behaves like a ``.SYM`` without a name. Breakpoints
can only be seen in WLALINK's symbol file.

This is not a compulsory directive.


``.BREAKPOINT``
---------------

``.BREAKPOINT`` is an alias for ``.BR``.

This is not a compulsory directive.



``.ASCIITABLE``
---------------

``.ASCIITABLE``'s only purpose is to provide character mapping for ``.ASC``.
Take a look at the example::

    .ASCIITABLE
    MAP "A" TO "Z" = 0
    MAP "!" = 90
    .ENDA

Here we set such a mapping that character ``A`` is equal to ``0``, ``B`` is
equal to ``1``, ``C`` is equal to ``2``, and so on, and ``!`` is equal
to ``90``.

After you've given the ``.ASCIITABLE``, use ``.ASC`` to define bytes using
this mapping (``.ASC`` is an alias for ``.DB``, but with ``.ASCIITABLE``
mapping). For example, ``.ASC "ABZ"`` would define bytes ``0``, ``1`` and
``25`` in our previous example.

Note that the following works as well::

    .ASCIITABLE
    MAP 'A' TO 'Z' = 0
    MAP 65 = 90          ; 65 is the decimal for ASCII 'A'
    .ENDA

Also note that the characters that are not given any mapping in
``.ASCIITABLE`` map to themselves (i.e., ``2`` maps to ``2`` in our previous
example, etc.).

This is not a compulsory directive.


``.ENDA``
---------

Ends the ASCII table.

This is not a compulsory directive, but when ``.ASCIITABLE`` or ``.ASCTABLE``
are used this one is required to terminate them.


``.ASCTABLE``
-------------

``.ASCTABLE`` is an alias for ``.ASCIITABLE``.

This is not a compulsory directive.


``.ASC "HELLO WORLD!"``
-----------------------

``.ASC`` is an alias for ``.DB``, but if you use ``.ASC`` it will remap
the characters using the mapping given via ``.ASCIITABLE``.

This is not a compulsory directive.


``.STRINGMAPTABLE script "script.tbl"``
---------------------------------------

``.STRINGMAPTABLE``'s only purpose is to provide string mapping for 
``.STRINGMAP``. Take a look at the example::

    .STRINGMAPTABLE script "script.tbl"

This will load the file "script.tbl" and define a new string mapping called 
"script". This file is in the "table file" format commonly used for game 
translations; take a look at an example of one::

    00=A
    01=B
    ; This is a comment
    ff01=あ
    ff02=いうえ
    fe=\n

The values to the left of the '=' are a variable number of bytes expressed
in hex, which map to the text value on the right. Note that depending on the
text encoding of the file, this may be a variable number of bytes too. Thus
this is a more flexible version of ``.ASCIITABLE``.

After you've given the ``.STRINGMAPTABLE``, use ``.STRINGMAP`` to define bytes 
using this mapping. For example::

    .STRINGMAP script, "いうえA\n"

This will map to the byte values ``FF 02 00 FE``, provided the source file and
TBL file use the same string encoding - use of UTF-8 is advised. 

Note that all characters must be defined in the mapping - there is no fallback 
to ASCII encoding. You also cannot mix in byte values like with ``.DB`` and 
``.ASC``.

You can define multiple named string map tables.

This is not a compulsory directive.


``.STRINGMAP script "Hello\n"``
-------------------------------

``.ASC`` is an alias for ``.DB``, but if you use ``.ASC`` it will remap
the characters using the mapping given via ``.ASCIITABLE``.

This is not a compulsory directive.


``.DW 16000, 10, 255``
----------------------

Defines words (two bytes each). ``.DW`` takes only numbers, labels and
characters as input, not strings.

This is not a compulsory directive.


``.WORD 16000, 10, 255``
------------------------

``.WORD`` is an alias for ``.DW``.

This is not a compulsory directive.


``.ADDR 16000, main, 255``
--------------------------

``.ADDR`` is an alias for ``.DW``.

This is not a compulsory directive.


``.DWM filtermacro 1, 2, 3``
----------------------------

Defines 16-bit words using a filter macro. Works just like ``.DBM`` and ``.DLM``.

This is not a compulsory directive.


``.DL $102030, $405060``
------------------------

Defines long words (three bytes each). ``.DL`` takes only numbers, labels and
characters as input, not strings. Works only on wla-65816.

This is not a compulsory directive.


``.LONG $102030, $405060``
--------------------------

``.LONG`` is an alias for ``.DL``. Works only on wla-65816.

This is not a compulsory directive.


``.FARADDR main, irq_1``
------------------------

``.FARADDR`` is an alias for ``.DL``. Works only on wla-65816.

This is not a compulsory directive.


``.DLM filtermacro 1, 2, 3``
----------------------------

Defines 24-bit words using a filter macro. Works just like ``.DBM`` and ``.DWM``.
Works only on wla-65816.

This is not a compulsory directive.


``.DEFINE IF $FF0F``
--------------------

Assigns a number or a string to a definition label.

By default all defines are local to the file where they are
presented. If you want to make the definition visible to all the
files in the project, use ``.EXPORT`` or add EXPORT to the end of .DEFINE::

    .DEFINE ID_0 0 EXPORT

WARNING: Please declare your definition lexically before using it as otherwise
the assembler might make incorrect assumptions about its value and size and
choose e.g. wrong opcodes and generate binary that doesn't run properly.

Here are some examples::

    .DEFINE X 1000
    .DEFINE FILE "level01.bin"
    .DEFINE TXT1 "hello and welcome", 1, "to a new world...", 0
    .DEFINE BYTES 1, 2, 3, 4, 5
    .DEFINE COMPUTATION X+1
    .DEFINE DEFAULTV

All definitions with multiple values are marked as data strings,
and ``.DB`` is about the only place where you can later on use them. ::

    .DEFINE BYTES 1, 2, 3, 4, 5
    .DB 0, BYTES, 6

is the same as ::

    .DB 0, 1, 2, 3, 4, 5, 6

If you omit the definition value (in our example ``DEFAULTV``), WLA
will default to ``0``.

Note that you must do your definition before you use it, otherwise
WLA will use the final value of the definition. Here's an example
of this::

    .DEFINE AAA 10
    .DB AAA            ; will be 10.
    .REDEFINE AAA 11

but ::

    .DB AAA            ; will be 11.
    .DEFINE AAA 10
    .REDEFINE AAA 11

You can also create definitions on the command line. Here's an
example of this::

    wla-gb -vl -DMOON -DNAME=john -DPRICE=100 -DADDRESS=$100 math.s

``MOON``'s value will be ``0``, ``NAME`` is a string definition with value
``john``, ``PRICE``'s value will be ``100``, and ``ADDRESS``'s value will be
``$100``.

Note that ::

    .DEFINE AAA = 10   ; the same as ".DEFINE AAA 10".

works as well. And this works also ::

    AAA = 10

This is not a compulsory directive.


``.DEF IF $FF0F``
-----------------

``.DEF`` is an alias for ``.DEFINE``.

This is not a compulsory directive.


``.EQU IF $FF0F``
-----------------

``.EQU`` is an alias for ``.DEFINE``.

This is not a compulsory directive.


``.REDEFINE IF $0F``
--------------------

Assigns a new value or a string to an old definition. If the
definition doesn't exist, ``.REDEFINE`` performs ``.DEFINE``'s work.

When used with ``.REPT`` ``REDEFINE`` helps creating tables::

    .DEFINE CNT 0
    
    .REPT 256
    .DB CNT
    .REDEFINE CNT CNT+1
    .ENDR

This is not a compulsory directive.


``.REDEF IF $0F``
-----------------

``.REDEF`` is an alias for ``.REDEFINE``.

This is not a compulsory directive.


``.IF DEBUG == 2``
------------------

If the condition is fulfilled the following piece of code is
acknowledged until ``.ENDIF``/``.ELSE`` occurs in the text, otherwise
it is skipped. Operands must be immediate values or strings.

The following operators are supported:

======= =====================
 ``<``   less than
 ``<=``  less or equal to
 ``>``   greater than
 ``>=``  greater or equal to
 ``==``  equals to
 ``!=``  doesn't equal to
======= =====================

All ``IF`` directives (yes, including ``.IFDEF``, ``.IFNDEF``, etc) can be
nested. They can also be used within ``ENUM`` s, ``RAMSECTION`` s,
``STRUCT`` s, ``ROMBANKMAP`` s, and most other directives that occupy multiple
lines.


This is not a compulsory directive.


``.IFDEF IF``
-------------

If ``IF`` is defined, then the following piece of code is acknowledged
until ``.ENDIF``/``.ELSE`` occurs in the text, otherwise it is skipped.

This is not a compulsory directive.


``.IFEXISTS "main.s"``
----------------------

If ``main.s`` file can be found, then the following piece of code is
acknowledged until ``.ENDIF``/``.LESE`` occurs in the text, otherwise it is
skipped.

By writing the following few lines you can include a file if it exists
without breaking the compiling loop if it doesn't exist. ::

    .IFEXISTS FILE
    .INCLUDE FILE
    .ENDIF

This is not a compulsory directive.


``.UNDEFINE DEBUG``
-------------------

Removes the supplied definition label from system. If there is no
such label as given no error is displayed as the result would be the
same.

You can undefine as many definitions as you wish with one ``.UNDEFINE``::

    .UNDEFINE NUMBER, NAME, ADDRESS, COUNTRY
    .UNDEFINE NAME, AGE

This is not a compulsory directive.


``.UNDEF DEBUG``
----------------

``.UNDEF`` is an alias for ``.UNDEFINE``.

This is not a compulsory directive.


``.IFNDEF IF``
--------------

If ``IF`` is not defined, then the following piece of code is acknowledged
until ``.ENDIF``/``.ELSE`` occurs in the text, otherwise it is skipped.

This is not a compulsory directive.


``.IFDEFM \2``
--------------

If the specified argument is defined (argument number two, in the example),
then the following piece of code is acknowledged until ``.ENDIF``/``.ELSE``
occurs in the macro, otherwise it is skipped.

This is not a compulsory directive. ``.IFDEFM`` works only inside a macro.


``.IFNDEFM \2``
---------------

If the specified argument is not defined, then the following piece of
code is acknowledged until ``.ENDIF``/``.ELSE`` occurs in the macro, otherwise
it is skipped.

This is not a compulsory directive. ``.IFNDEFM`` works only inside a macro.


``.IFEQ DEBUG 2``
-----------------

If the value of ``DEBUG`` equals to ``2``, then the following piece of code is
acknowledged until ``.ENDIF``/``.ELSE`` occurs in the text, otherwise it is
skipped. Both arguments can be computations, defines or immediate values.

This is not a compulsory directive.


``.IFNEQ DEBUG 2``
------------------

If the value of ``DEBUG`` doesn't equal to ``2``, then the following piece of
code is acknowledged until ``.ENDIF``/``.ELSE`` occurs in the text, otherwise
it is skipped. Both arguments can be computations, defines or immediate values.

This is not a compulsory directive.


``.IFLE DEBUG 2``
-----------------

If the value of ``DEBUG`` is less than ``2``, then the following piece of code
is acknowledged until ``.ENDIF``/``.ELSE`` occurs in the text, otherwise it is
skipped. Both arguments can be computations, defines or immediate values.

This is not a compulsory directive.


``.IFLEEQ DEBUG 2``
-------------------

If the value of ``DEBUG`` is less or equal to ``2``, then the following piece of
code is acknowledged until ``.ENDIF``/``.ELSE`` occurs in the text, otherwise
it is skipped. Both arguments can be computations, defines or immediate values.

This is not a compulsory directive.


``.IFGR DEBUG 2``
-----------------

If the value of ``DEBUG`` is greater than ``2``, then the following piece of
code is acknowledged until ``.ENDIF``/``.ELSE`` occurs in the text, otherwise
it is skipped. Both arguments can be computations, defines or immediate values.

This is not a compulsory directive.


``.IFGREQ DEBUG 2``
-------------------

If the value of ``DEBUG`` is greater or equal to ``2``, then the following
pieceof code is acknowledged until ``.ENDIF``/``.ELSE`` occurs in the text,
otherwise it is skipped. Both arguments can be computations, defines or
immediate values.

This is not a compulsory directive.


``.ELSE``
---------

If the previous ``.IFxxx`` failed then the following text until
``.ENDIF`` is acknowledged.

This is not a compulsory directive.


``.ENDIF``
----------

This terminates any ``.IFxxx`` directive.

This is not a compulsory directive, but if you use any ``.IFxxx`` then
you need also to apply this.


``.REPEAT 6``
-------------

Repeats the text enclosed between ``.REPEAT x`` and ``.ENDR`` ``x`` times
(``6`` in this example). You can use ``.REPEAT`` s inside ``.REPEAT`` s.
``x`` must be bigger or equal than ``0``.

It's also possible to have the repeat counter/index in a definition::

    .REPEAT 6 INDEX COUNT
    .DB COUNT
    .ENDR

This would define bytes ``0``, ``1``, ``2``, ``3``, ``4`` and ``5``.

This is not a compulsory directive.


``.REPT 6``
-----------

``.REPT`` is an alias for ``.REPEAT``.

This is not a compulsory directive.


``.ENDR``
---------

Ends the repetition.

This is not a compulsory directive, but when ``.REPEAT`` is used this one is
required to terminate it.


``.ENUM $C000``
---------------

Starts enumeration from ``$C000``. Very useful for defining variables.

To start a descending enumeration, put ``DESC`` after the starting
value. WLA defaults to ``ASC`` (ascending enumeration).

You can also add ``EXPORT`` after these if you want to export all
the generated definitions automatically.

Here's an example of ``.ENUM``::

    ...
    .STRUCT mon                ; check out the documentation on
    name ds 2                  ; .STRUCT
    age  db
    .ENDST

    .ENUM $A000
    _scroll_x DB               ; db  - define byte (byt and byte work also)
    _scroll_y DB
    player_x: DW               ; dw  - define word (word works also)
    player_y: DW
    map_01:   DS  16           ; ds  - define size (bytes)
    map_02    DSB 16           ; dsb - define size (bytes)
    map_03    DSW  8           ; dsw - define size (words)
    monster   INSTANCEOF mon 3 ; three instances of structure mon
    dragon    INSTANCEOF mon   ; one mon
    .ENDE
    ...

Previous example transforms into following definitions::

    .DEFINE _scroll_x      $A000
    .DEFINE _scroll_y      $A001
    .DEFINE player_x       $A002
    .DEFINE player_y       $A004
    .DEFINE map_01         $A006
    .DEFINE map_02         $A016
    .DEFINE map_03         $A026
    .DEFINE monster        $A036
    .DEFINE monster.1      $A036
    .DEFINE monster.1.name $A036
    .DEFINE monster.1.age  $A038
    .DEFINE monster.2      $A039
    .DEFINE monster.2.name $A039
    .DEFINE monster.2.age  $A03B
    .DEFINE monster.3      $A03C
    .DEFINE monster.3.name $A03C
    .DEFINE monster.3.age  $A03E
    .DEFINE dragon         $A03F
    .DEFINE dragon.name    $A03F
    .DEFINE dragon.age     $A041

``DB``, ``DW``, ``DS``, ``DSB``, ``DSW`` and ``INSTANCEOF`` can also be in
lowercase. You can also use a dotted version of the symbols, but it doesn't
advance the memory address. Here's an example::

    .ENUM $C000 DESC EXPORT
    bigapple_h db
    bigapple_l db
    bigapple:  .dw
    .ENDE

And this is what is generated::

    .DEFINE bigapple_h $BFFF
    .DEFINE bigapple_l $BFFE
    .DEFINE bigapple   $BFFE
    .EXPORT bigapple, bigapple_l, bigapple_h

This way you can generate a 16-bit variable address along with pointers
to its parts.

Note that you can also use ``DL`` (define long word, a 24-bit value) and
``DSL`` (define size, long words) when running wla-65816.

If you want more flexible variable positioning, take a look at
``.RAMSECTION`` s.

This is not a compulsory directive.


``.ENDE``
---------

Ends the enumeration.

This is not a compulsory directive, but when ``.ENUM`` is used this one is
required to terminate it.


``.STRUCT enemy_object``
------------------------

Begins the definition of a structure. These structures can be placed
inside ``RAMSECTION`` s and ``ENUM`` s. Here's an example::

    .STRUCT enemy_object
    id      dw             ; the insides of a .STRUCT are 1:1 like in .ENUM
    x       db             ; except that no structs inside structs are
    y       db             ; allowed.
    data    ds  10
    info    dsb 16
    stats   dsw  4
    .ENDST

This also creates a definition ``_sizeof_[struct name]``, in our example
this would be ``_sizeof_enemy_object``, and the value of this definition
is the size of the object, in bytes (2+1+1+10+16+4*2 = 38 in the example).

You'll get the following definitions as well::

    enemy_object.id    (== 0)
    enemy_object.x     (== 2)
    enemy_object.y     (== 3)
    enemy_object.data  (== 4)
    enemy_object.info  (== 14)
    enemy_object.stats (== 30)

After defining a ``.STRUCT`` you can create an instance of it in a
``.RAMSECTION`` / ``.ENUM`` by typing ::

    <instance name> INSTANCEOF <struct name> [optional, the number of structures]

Here's an example::

    .RAMSECTION "enemies" BANK 4 SLOT 4
    enemies   INSTANCEOF enemy_object 4
    enemyman  INSTANCEOF enemy_object
    enemyboss INSTANCEOF enemy_object
    .ENDS

This will create definitions like ``enemies``, ``enemies.1.id``, ``enemies.1.x``,
``enemies.1.y`` and so on. Definition ``enemies`` is followed by four ``enemy_object``
instances. After those four come ``enemyman`` and ``enemyboss`` instances, but
as they are single instances, their definitions lack the index: ``enemyman``,
``enemyman.id``, ``enemyman.x``, ``enemyman.y`` and so on.

Take a look at the documentation on ``.RAMSECTION`` & ``.ENUM``, they have more
examples of how you can use ``.STRUCT`` s.

**A WORD OF WARNING**: Don't use labels ``b``, ``B``, ``w`` and ``W`` inside a
structure as e.g., WLA sees ``enemy.b`` as a byte sized reference to enemy. All
other labels should be safe. ::

    lda enemy1.b  ; load a byte from zeropage address enemy1 or from the address
                  ; of enemy1.b??? i can't tell you, and WLA can't tell you...

This is not a compulsory directive.


``.ENDST``
----------

Ends the structure definition.

This is not a compulsory directive, but when ``.STRUCT`` is used this one is
required to terminate it.


``.MEMORYMAP``
--------------

Begins the memory map definition. Using ``.MEMORYMAP`` you must first
describe the target system's memory architecture to WLA before it
can start to compile the code. ``.MEMORYMAP`` gives you the freedom to
use WLA to compile data for numerous different real
Z80/6502/65C02/65CE02/6510/6800/6801/6809/8008/8080/65816/HUC6280/SPC-700
based systems.

Examples::

    .MEMORYMAP
    DEFAULTSLOT 0
    SLOTSIZE $4000
    SLOT 0 $0000
    SLOT 1 $4000
    .ENDME
    
    .MEMORYMAP
    DEFAULTSLOT 0
    SLOT 0 $0000 $4000 "ROMSlot"
    SLOT 1 $4000 $4000 "RAMSlot"
    .ENDME
    
    .MEMORYMAP
    DEFAULTSLOT 0
    SLOT 0 START $0000 SIZE $4000 NAME "ROMSlot"
    SLOT 1 START $4000 SIZE $4000 NAME "RAMSlot"
    .ENDME
    
    .MEMORYMAP
    DEFAULTSLOT 1
    SLOTSIZE $6000
    SLOT 0 $0000
    SLOTSIZE $2000
    SLOT 1 $6000
    SLOT 2 $8000
    .ENDME

Here's a real life example from Adam Klotblixt. It should be interesting
for all the ZX81 coders::

    ...
    
    .MEMORYMAP
    DEFAULTSLOT 1
    SLOTSIZE $2000
    SLOT 0 $0000
    SLOTSIZE $6000
    SLOT 1 $2000
    .ENDME
    
    .ROMBANKMAP
    BANKSTOTAL 2
    BANKSIZE $2000
    BANKS 1
    BANKSIZE $6000
    BANKS 1
    .ENDRO
    
    .BANK 1 SLOT 1
    .ORGA $2000
    
    ...

``SLOTSIZE`` defines the size of the following slots, unless you explicitly
specify the size of the slot, like in the second and third examples. You
can redefine ``SLOTSIZE`` as many times as you wish.

``DEFAULTSLOT`` describes the default slot for banks which aren't explicitly
inserted anywhere. Check ``.BANK`` definition for more information.

``SLOT`` defines a slot and its starting address. ``SLOT`` numbering starts at
``0`` and ends to ``255`` so you have 256 slots at your disposal.

This is a compulsory directive, and make sure all the object files share
the same ``.MEMORYMAP`` or you can't link them together.


``.ENDME``
----------

Terminates ``.MEMORYMAP``.

This is not a compulsory directive, but when ``.MEMORYMAP`` is used this one
is required to terminate it.


``.ROMBANKMAP``
---------------

Begins the ROM bank map definition. You can use this directive to
describe the project's ROM banks. Use ``.ROMBANKMAP`` when not all the
ROM banks are of equal size. Note that you can use ``.ROMBANKSIZE`` and
``.ROMBANKS`` instead of ``.ROMBANKMAP``, but that's only when the ROM banks
are equal in size. Some systems based on a real Z80 chip,
6502/65C02/65CE02/6510/65816/6800/6801/6809/8008/8080/HUC6280/SPC-700 CPUs and
Pocket Voice cartridges for Game Boy require the usage of this directive.

Examples::

    .ROMBANKMAP
    BANKSTOTAL 16
    BANKSIZE $4000
    BANKS 16
    .ENDRO
    
    .ROMBANKMAP
    BANKSTOTAL 510
    BANKSIZE $6000
    BANKS 1
    BANKSIZE $2000
    BANKS 509
    .ENDRO

The first one describes an ordinary ROM image of 16 equal sized
banks. The second one defines a 4MB Pocket Voice ROM image.
In the PV ROM image the first bank is ``$6000`` bytes and the remaining
``509`` banks are smaller ones, ``$2000`` bytes each.

``BANKSTOTAL`` tells the total amount of ROM banks. It must be
defined prior to anything else.

``BANKSIZE`` tells the size of the following ROM banks. You can
supply WLA with ``BANKSIZE`` as many times as you wish.

``BANKS`` tells the amount of banks that follow and that are of
the size ``BANKSIZE`` which has been previously defined.

This is not a compulsory directive when ``.ROMBANKSIZE`` and
``.ROMBANKS`` are defined.

You can redefine ``.ROMBANKMAP`` as many times as you wish as long as
the old and the new ROM bank maps match as much as possible. This
way you can enlarge the size of the project on the fly.


``.ENDRO``
----------

Ends the rom bank map.

This is not a compulsory directive, but when ``.ROMBANKMAP`` is used this
one is required to terminate it.


``.SEED 123``
-------------

Seeds the random number generator.

This is not a compulsory directive. The random number generator is
initially seeded with the output of ``time()``, which is, according to
the manual, *the time since the Epoch (00:00:00 UTC, January 1, 1970),
measured in seconds*. So if you don't ``.SEED`` the random number generator
yourself with a constant value, ``.DBRND`` and ``.DWRND`` give you different
values every time you run WLA.

In WLA DX 9.4a and before we used the stdlib's ``srand()`` and ``rand()``
functions making the output differ on different platforms. Since v9.4 WLA DX
contains its own Mersenne Twister pseudo random number generator.


``.SECTION "Init" FORCE``
-------------------------

Section is a continuous area of data which is placed into the output
file according to the section type and ``.BANK`` and ``.ORG`` directive
values.

The example begins a section called ``Init``. Before a section can be
declared, ``.BANK`` and ``.ORG`` must be used unless WLA is in library file
output mode. Library file's sections must all be ``FREE`` ones. ``.BANK``
tells the bank number where this section will be later relocated into. ``.ORG``
tells the offset for the relocation from the beginning of ``.BANK``.

You can put sections inside a namespace. For instance, if you put a section
into a namespace called ``bank0``, then labels in that section can be
accessed with ``bank0.label``. This is not necessary inside the section
itself. The namespace directive should immediately follow the name. ::

    .SECTION "Init" NAMESPACE "bank0"

You can give the size of the section, if you wish to force the section
to some specific size, the following way::

    .SECTION "Init" SIZE 100 FREE

It's possible to force WLALINK to align the ``FREE``, ``SEMIFREE`` and
``SUPERFREE`` sections by giving the alignment as follows::

    .SECTION "Init" SIZE 100 ALIGN 4 FREE

If you need an offset from the alignment, use OFFSET::

    .SECTION "Init" SIZE 10 ALIGN 256 OFFSET 32 FREE

And if you want that WLA returns the ``ORG`` to what it was before issuing
the section, put ``RETURNORG`` at the end of the parameter list::

    .SECTION "Init" SIZE 100 ALIGN 4 FREE RETURNORG

By default WLA advances the ``ORG``, so, for example, if your ``ORG`` was
``$0`` before a section of 16 bytes, then the ``ORG`` will be ``16`` after the
section.

Note also that if your section name begins with double underlines (e.g.,
``__UNIQUE_SECTION!!!``) the section will be unique in the sense that
when WLALINK recieves files containing sections which share the same
name, WLALINK will save only the first of them for further processing,
all others are deleted from memory with corresponding labels, references
and calculations.

If a section name begins with an exclamation mark (``!``) it tells
WLALINK to not to drop it, even if you use WLALINK's ability to discard
all unreferenced sections and there are no references to the section. You can
achieve the same effect by adding ``KEEP`` to the end of the list::

    .SECTION "Init" SIZE 100 ALIGN 4 FREE RETURNORG KEEP  

``FORCE`` after the name of the section tells WLA that the section *must* be
inserted so it starts at ``.ORG``. ``FORCE`` can be replaced with ``FREE``
which means that the section can be inserted somewhere in the defined bank,
where there is room. You can also use ``OVERWRITE`` to insert the section into
the memory regardless of data collisions. Using ``OVERWRITE`` you can easily
patch an existing ROM image just by ``.BACKGROUND``'ing the ROM image and
inserting ``OVERWRITE`` sections into it. ``SEMIFREE`` sections are also
possible and they behave much like ``FREE`` sections. The only difference is
that they are positioned somewhere in the bank starting from ``.ORG``.
``SEMISUBFREE`` sections on the other hand are positioned somewhere in the bank
starting from ``$0`` and ending to ``.ORG``.

``SUPERFREE`` sections are also available, and they will be positioned into
the first suitable place inside the first suitable bank (candidates for these
suitable banks have the same size with the slot of the section, no other banks
are considered). You can also leave away the type specifier as the default type
for the section is ``FREE``.

You can name the sections as you wish, but there is one special name. A section
called ``BANKHEADER`` is placed in the front of the bank where it is defined.
These sections contain data that is not in the memory map of the machine, so
you can't refer to the data of a ``BANKHEADER`` section, but you can write
references to outside. So no labels inside ``BANKHEADER`` sections. These
special sections are useful when writing e.g., MSX programs. Note that library
files don't take ``BANKHEADER`` sections.

Here's an example of a ``BANKHEADER`` section::

    .BANK 0
    .ORG 0
    .SECTION "BANKHEADER"
    	.DW MAIN
    	.DW VBI
    .ENDS
    
    .SECTION "Program"
    MAIN: CALL MONTY_ON_THE_RUN
    VBI:  PUSH HL
    	  ...
    	  POP HL
          RETI
    .ENDS

Here's an example of an ordinary section::

    .BANK 0
    .ORG $150
    .SECTION "Init" FREE PRIORITY 1000
            DI
            LD  SP, $FFFE
            SUB A
            LD  ($FF00+R_IE), A
    .ENDS

This tells WLA that a ``FREE`` section called ``Init`` must be located
somewhere in bank ``0`` and it has a sorting ``PRIORITY`` of 1000. If you replace
``FREE`` with ``SEMIFREE`` the section will be inserted somewhere in the bank ``0``,
but not in the ``$0`` - ``$14F`` area. If you replace ``FREE`` with ``SUPERFREE``
the section will be inserted somewhere in any bank with the same size as bank ``0``.

Here's the order in which WLA writes the sections:

1. ``FORCE``
2. ``SEMISUBFREE``
3. ``SEMIFREE`` & ``FREE``
4. ``SUPERFREE``
5. ``OVERWRITE``

Before the sections are inserted into the output file, they are sorted by
priorities, so that the section with the highest priority is processed first.
If priorities are the same, then the size of the section matters, and bigger
sections are processed before smaller ones. The default ``PRIORITY``, when not
explicitly given, is 0.

You can also create a RAM section. For more information about them, please
read the ``.RAMSECTION`` directive explanation.

It is also possible to merge two or more sections using ``APPENDTO``::

    .SECTION "Base"
    .DB 0
    .ENDS
    
    .SECTION "AppendToBase" FREE RETURNORG APPENDTO "Base"
    .DB 1
    .ENDS

This is not a compulsory directive.


``.RAMSECTION "Vars" BANK 0 SLOT 1 ALIGN 256 OFFSET 32``
--------------------------------------------------------

``RAMSECTION`` s accept only variable labels and variable sizes, and the
syntax to define these is identical to ``.ENUM`` (all the syntax rules that
apply to ``.ENUM`` apply also to ``.RAMSECTION``). Additionally you can embed
structures (``.STRUCT``) into a ``RAMSECTION``. Here's an example::

    .RAMSECTION "Some of my variables" BANK 0 SLOT 1 RETURNORG PRIORITY 100
    vbi_counter:   db
    player_lives:  db
    .ENDS

By default ``RAMSECTION`` s behave like ``FREE`` sections, but instead of
filling any banks RAM sections will occupy RAM banks inside slots. You can
fill different slots with different variable labels. It's recommend that
you create separate slots for holding variables (as ROM and RAM don't
usually overlap).

If you want that WLA returns the ``ORG`` to what it was before issuing
the ``RAMSECTION``, use the keyword ``RETURNORG``.

Keyword ``PRIORITY`` means just the same as ``PRIORITY`` of a ``.SECTION``,
it is used to prioritize some sections when placing them in the output ROM/PRG.
The ``RAMSECTION`` s with higher ``PRIORITY`` are placed first in the output,
and if the priorities match, then bigger ``RAMSECTION`` s are placed first.

NOTE! Currently WLA-DX assumes that there are 256 RAM banks available for
each slot in the memory map. There is no other way to limit this number at the
moment than manually keep the ``BANK`` number inside real limits.

Anyway, here's another example::

    .MEMORYMAP
    SLOTSIZE $4000
    DEFAULTSLOT 0
    SLOT 0 $0000           ; ROM slot 0.
    SLOT 1 $4000           ; ROM slot 1.
    SLOT 2 $A000 "RAMSlot" ; variable RAM is here!
    .ENDME

    .STRUCT game_object
    x DB
    y DB
    .ENDST

    .RAMSECTION "vars 1" BANK 0 SLOT 2
    moomin1   DW
    phantom   DB
    nyanko    DB
    enemy     INSTANCEOF game_object
    .ENDS

    .RAMSECTION "vars 2" BANK 1 SLOT "RAMSlot"  ; Here we use slot 2
    moomin2   DW
    .ENDS

    .RAMSECTION "vars 3" BANK 1 SLOT $A000      ; Slot 2 here as well...
    moomin3_all .DSB 3
    moomin3_a    DB
    moomin3_b    DB
    moomin3_c    DB
    .ENDS

    .RAMSECTION "vars 4" BANK 1 SLOT $A000
    enemies      INSTANCEOF game_object 2 STARTFROM 0 ; If you leave away "STARTFROM 0" the indexing will start from 1
    .ENDS

If no other RAM sections are used, then this is what you will get::

    .DEFINE moomin1     $A000
    .DEFINE phantom     $A002
    .DEFINE nyanko      $A003
    .DEFINE enemy       $A004
    .DEFINE enemy.x     $A004
    .DEFINE enemy.y     $A005
    .DEFINE moomin2     $A000
    .DEFINE moomin3_all $A002
    .DEFINE moomin3_a   $A002
    .DEFINE moomin3_b   $A003
    .DEFINE moomin3_c   $A004
    .DEFINE enemies     $A005
    .DEFINE enemies.0   $A005
    .DEFINE enemies.0.x $A005
    .DEFINE enemies.0.y $A006
    .DEFINE enemies.1   $A007
    .DEFINE enemies.1.x $A007
    .DEFINE enemies.1.y $A008

``BANK`` in ``.RAMSECTION`` is optional so you can leave it away if you
don't switch RAM banks, or the target doesn't have them (defaults to 0).

NOTE! The generated _sizeof_ labels for ``.RAMSECTION`` "vars 3" will be::

    _sizeof_moomin3_all (== 3)
    _sizeof_moomin3_a   (== 1)
    _sizeof_moomin3_b   (== 1)
    _sizeof_moomin3_c   (== 1)

It is also possible to merge two or more sections using ``APPENDTO``::

    .RAMSECTION "RAMSection1" BANK 0 SLOT 0
    label1    DB
    .ENDS
    
    .RAMSECTION "RAMSection2" APPENDTO "RAMSection1"
    label2    DB
    .ENDS

If you wist to skip some bytes without giving them labels, use ``.`` as
a label::

    .RAMSECTION "ZERO_PAGE" BANK 0 SLOT 0
    UsingThisByte1: DB
    .               DB ; RESERVED
    .               DB ; RESERVED
    UsingThisByte2: DB
    .               DB ; RESERVED
    UsingThisByte3: DB
    .ENDS

If you want to use ``FORCE`` RAMSECTIONs that are fixed to a specified
address, do as follows::

    .RAMSECTION "FixedRAMSection" BANK 0 SLOT 0 ORGA $0 FORCE
    .               DB ; SYSTEM RESERVED
    .               DB ; SYSTEM RESERVED
    PlayerX         DB
    PlayerY         DB
    .ENDS

Other types that are supported: ``SEMIFREE`` and ``SEMISUBFREE``.

Here's the order in which WLA writes the RAM sections:

1. ``FORCE``
2. ``SEMISUBFREE``
3. ``SEMIFREE`` & ``FREE``

NOTE: You can use ``ORGA`` to specify the fixed address for a ``FORCE``
``RAMSECTION``. ``ORG`` is also supported.

NOTE: When you have ``RAMSECTION`` s inside libraries, you must give
them BANKs and SLOTs in the linkfile, under [ramsections].

This is not a compulsory directive.


``.ENDS``
---------

Ends the section.

This is not a compulsory directive, but when ``.SECTION`` or ``.RAMSECTION``
is used this one is required to terminate it.


``.ROMGBC``
-----------

Inserts data into the specific ROM location to mark the ROM as a dual-mode ROM
(``$80`` -> ``$0143``, so ROM name is max. 15 characters long). It will run in
either DMG or GBC mode.

This is not a compulsory directive.


``.ROMGBCONLY``
---------------

Inserts data into the specific ROM location to mark the ROM as a Gameboy Color
ROM (``$C0`` -> ``$0143``, so ROM name is max. 15 characters long). It will
only run in GBC mode.

This is not a compulsory directive.


``.ROMDMG``
-----------

Inserts data into the specific ROM location to mark the ROM as a DMG
(Gameboy) ROM (``$00`` -> ``$0146``). It will only run in DMG mode.

This is not a compulsory directive. ``.ROMDMG`` cannot be used with ``.ROMSGB``.


``.ROMSGB``
-----------

Inserts data into the specific ROM location to mark the ROM as a Super
Gameboy enhanced ROM (``$03`` -> ``$0146``).

This is not a compulsory directive. ``.ROMSGB`` cannot be used with ``.ROMDMG``.


``.EXPORT work_x``
------------------

Exports the definition ``work_x`` to outside world. Exported definitions are
visible to all object files and libraries in the linking procedure. Note
that you can only export value definitions, not string definitions.

You can export as many definitions as you wish with one ``.EXPORT``::

    .EXPORT NUMBER, NAME, ADDRESS, COUNTRY
    .EXPORT NAME, AGE

This is not a compulsory directive.


``.PRINT "Numbers 1 and 10: ", DEC 1, " $", HEX 10, "\n"``
----------------------------------------------------------

Prints strings and numbers to stdout. A combination and a more usable version
of .PRINTT and .PRINTV. Useful for debugging.

Optional: Give ``DEC`` (decimal) or ``HEX`` (hexadecimal) before the value you
want to print.

This is not a compulsory directive.


``.PRINTT "Here we are...\n"``
------------------------------

Prints the given text into stdout. Good for debugging stuff. ``PRINTT`` takes
only a string as argument, and the only supported formatting symbol is ``\n``
(line feed).

This is not a compulsory directive.


``.PRINTV DEC DEBUG+1``
-----------------------

Prints the value of the supplied definition or computation into stdout.
Computation must be solvable at the time of printing (just like definitions
values). ``PRINTV`` takes max two parameters. The first describes the type of the
print output. ``DEC`` means decimal, ``HEX`` means hexadecimal. This is optional.
Default is ``DEC``.

Use ``PRINTV`` with ``PRINTT`` as ``PRINTV`` doesn't print linefeeds, only the
result. Here's an example::

    .PRINTT "Value of \"DEBUG\" = $"
    .PRINTV HEX DEBUG
    .PRINTT "\n"

This is not a compulsory directive.


``.OUTNAME "other.o"``
----------------------

Changes the name of the output file. Here's and example::

    wla-gb -o test.o test.s

would normally output ``test.o``, but if you had written ::

    .OUTNAME "new.o"

somewhere in the code WLA would write the output to ``new.o`` instead.

This is not a compulsory directive.


``.SNESHEADER``
---------------

This begins the SNES header definition, and automatically defines
``.COMPUTESNESCHECKSUM``. From here you may define any of the following:


* ``ID "ABCD"`` - inserts a one to four letter string starting at ``$7FB2``
  (lorom) or ``$FFB2`` (hirom).
* ``NAME "Hello World!"`` - identical to a freestanding ``.NAME``.
* ``LOROM`` - identical to a freestanding ``.LOROM``.
* ``HIROM`` - identical to a freestanding ``.HIROM``.
* ``EXHIROM`` - identical to a freestanding ``.EXHIROM``.
* ``SLOWROM`` - identical to a freestanding ``.SLOWROM``.
* ``FASTROM`` - identical to a freestanding ``.FASTROM``.
* ``CARTRIDGETYPE $00`` - Places the given 8-bit value in ``$7FD6`` (``$FFD6``
  in HiROM, ``$40FFD6`` and ``$FFD6`` in ExHiROM). Some possible values I've
  come across but cannot guarantee the accuracy of:
   
   ======== ====== ====== ==========
    ``$00``  ROM
    ``$01``  ROM     RAM
    ``$02``  ROM    SRAM
    ``$03``  ROM            DSP1
    ``$04``  ROM     RAM    DSP1
    ``$05``  ROM    SRAM    DSP1
    ``$13``  ROM            Super FX
   ======== ====== ====== ==========
* ``ROMSIZE $09`` - Places the given 8-bit value in ``$7FD7`` (``$FFD7`` in
  HiROM, ``$40FFD7`` and ``$FFD7`` in ExHiROM). Possible values include (but
  may not be limited to):

   ======== =============
    ``$08``   2 Megabits
    ``$09``   4 Megabits
    ``$0A``   8 Megabits
    ``$0B``  16 Megabits
    ``$0C``  32 Megabits
   ======== =============
* ``SRAMSIZE $01`` - Places the given 2-bit value into ``$7FD8`` (``$FFD8`` in
  HiROM, ``$40FFD8`` and ``$FFD8`` in ExHiROM). I believe these are the only possible
  values:

   ======== =============
    ``$00``   0 kilobits
    ``$01``  16 kilobits
    ``$02``  32 kilobits
    ``$03``  64 kilobits
   ======== =============
* ``COUNTRY $00`` - Places the given 8-bit value into ``$7FD9`` (``$FFD9`` in
  HiROM, ``$40FFD9`` and ``$FFD9`` in ExHiROM). ``$00`` is Japan and ``$01`` is the
  United States, and there several more for other regions that I cannot recall off
  the top of my head.
* ``LICENSEECODE $00`` - Places the given 8-bit value into ``$7FDA`` (``$FFDA``
  in HiROM, ``$40FFDA`` and ``$FFDA`` in ExHiROM). You must find the legal values
  yourself as there are plenty of them. ;)
* ``VERSION $01`` - Places the given 8-bit value into ``$7FDB`` (``$FFDB`` in
  HiROM, ``$40FFDB`` and ``$FFDB`` in ExHiROM). This is supposedly interpreted as
  version 1.byte, so a ``$01`` here would be version 1.01.

This is not a compulsory directive.


``.ENDSNES``
------------

This ends the SNES header definition.

This is not a compulsory directive, but when ``.SNESHEADER`` is used this
one is required to terminate it.


``.SNESNATIVEVECTOR``
---------------------

Begins definition of the native mode interrupt vector table. ::

    .SNESNATIVEVECTOR
    COP    COPHandler
    BRK    BRKHandler
    ABORT  ABORTHandler
    NMI    VBlank
    UNUSED $0000
    IRQ    IRQHandler
    .ENDNATIVEVECTOR

These can be defined in any order, but they will be placed into
memory starting at ``$7FE4`` (``$FFE4`` in HiROM, ``$40FFE4`` and ``$FFE4`` in
ExHiROM) in the order listed above. All the vectors default to ``$0000``.

This is not a compulsory directive.


``.ENDNATIVEVECTOR``
--------------------

Ends definition of the native mode interrupt vector table.

This is not a compulsory directive, but when ``.SNESNATIVEVECTOR``
is used this one is required to terminate it.


``.SNESEMUVECTOR``
------------------

Begins definition of the emulation mode interrupt vector table. ::

    .SNESEMUVECTOR
    COP    COPHandler
    UNUSED $0000
    ABORT  BRKHandler
    NMI    VBlank
    RESET  Main
    IRQBRK IRQBRKHandler
    .ENDEMUVECTOR

These can be defined in any order, but they will be placed into
memory starting at ``$7FF4`` (``$FFF4`` in HiROM, ``$40FFF4`` and ``$FFF4`` in
ExHiROM) in the order listed above. All the vectors default to ``$0000``.

This is not a compulsory directive.


``.ENDEMUVECTOR``
-----------------

Ends definition of the emulation mode interrupt vector table.

This is not a compulsory directive, but when ``.SNESEMUVECTOR``
is used this one is required to terminate it.


``.UNION name``
---------------

Begins a "union". This can only be used in enums, ramsections, and structs.

When entering a union, the current address in the enum is saved, and the
following data is processed as normal. When the ``.NEXTU`` directive is
encountered, the address is reverted back to the start of the union. This allows
one to assign an area of memory to multiple labels. ::

    .ENUM $C000
        .UNION
            pos_lowbyte:  db
            pos_highbyte: db
            extra_word:   dw
        .NEXTU
            pos:          dw
        .ENDU
        after: db
    .ENDE

This example is equivalent to::

    .DEFINE pos_lowbyte  $c000
    .DEFINE pos_highbyte $c001
    .DEFINE extra_word   $c002
    .DEFINE pos          $c000
    .DEFINE after        $c004

The ``.UNION`` and ``.NEXTU`` commands can be given an argument to assign
a prefix to the labels that follow. ::

    .ENUM $C000
        .UNION union1
            byte1: db
            byte2: db
        .NEXTU union2
            word1: dw
        .ENDU
    .ENDE

This example is equivalent to::

    .DEFINE union1.byte1 $c000
    .DEFINE union1.byte2 $c001
    .DEFINE union2.word1 $c000

Unions can be nested.


``.NEXTU name``
---------------

Proceeds to the next entry in a union.


``.ENDU``
---------

Ends the current union.
