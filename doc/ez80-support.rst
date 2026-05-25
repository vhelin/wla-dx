eZ80 Support
============

WLA DX includes a Zilog eZ80 target named ``wla-ez80``. The target keeps WLA's
Z80 syntax where possible and adds the documented eZ80 instruction set,
including ADL-mode address and immediate handling.


ADL Mode And Suffixes
---------------------

``.ADL ON`` makes Z80-style 16-bit immediate and address operands assemble as
24-bit values where the eZ80 encoding allows it. ``.ADL OFF`` returns to the
default Z80-compatible 16-bit behavior.

Per-instruction mode suffixes emit one prefix byte before the instruction and
override the current ADL immediate width for that instruction:

- ``.S`` or ``.SIS``: ``40h``, short instruction and short immediate
- ``.LIS``: ``49h``, long instruction and short immediate
- ``.SIL``: ``52h``, short instruction and long immediate
- ``.L`` or ``.LIL``: ``5Bh``, long instruction and long immediate

The short ``.S`` and ``.L`` forms match manual entries that only override the
instruction mode, such as ``JP.S HL`` and ``JP.L IX``. For instructions with an
immediate or absolute address operand, use the full ``.SIS``, ``.LIS``, ``.SIL``
or ``.LIL`` suffix when you want to state both the instruction and immediate
mode explicitly.

Examples::

    .ADL ON
    jp $123456      ; C3 56 34 12
    jp.lis $1234    ; 49 C3 34 12
    jp.s hl         ; 40 E9
    jp.l ix         ; 5B DD E9


Instruction Coverage
--------------------

The eZ80 target supports the documented Z80-compatible base instruction set and
the documented eZ80 additions currently covered by WLA DX, including:

- ``IN0`` and ``OUT0``
- ``TST`` and ``TSTIO``
- ``LEA`` and ``PEA``
- ``MLT``, ``SLP``, ``STMIX`` and ``RSMIX``
- ``LD A,MB`` and ``LD MB,A``
- eZ80 block I/O variants such as ``INI2``, ``IND2``, ``INIM``, ``INIRX``,
  ``OTIM``, ``OTDM`` and ``OTIRX``
- multibyte ``LD`` forms through ``(HL)``, ``(IX+d)`` and ``(IY+d)``
- documented ``IXH``, ``IXL``, ``IYH`` and ``IYL`` byte-register operations

The target intentionally does not expose WLA's Z80-only per-register shadow
aliases, ``HX``/``LX``/``HY``/``LY`` aliases, undocumented ``SLL`` forms, or
DDCB/FDCB result-copy aliases.


Agon MOS Binaries
-----------------

``wla-ez80`` includes small source-level helpers for Agon MOS binaries. Include
the flat Agon MOS memory map from ``memorymaps/agon_mos.i`` and the header
macros from ``include/agon/mos.inc``. The MOS metadata starts at byte ``$40`` and
contains the bytes ``"MOS", $00, mode`` where mode is ``$00`` for Z80 mode or
``$01`` for ADL mode.

Example::

        .INCDIR "include/agon"
        .INCLUDE "memorymaps/agon_mos.i"
        .INCLUDE "mos.inc"

        .ROMBANKMAP
            BANKSTOTAL 1
            BANKSIZE $1000000
            BANKS 1
        .ENDRO

        .BANK 0 SLOT 0
        .ORG 0
        .ADL ON

        jp start
        AGON_MOS_EXEC_NAME "APP.BIN"

        .ORG AGON_MOS_HEADER_OFFSET
        AGON_MOS_HEADER_ADL

        start:
            ret

``AGON_MOS_HEADER_Z80`` emits the same header with the Z80-mode byte. For custom
mode selection, use ``AGON_MOS_HEADER AGON_MOS_RUNMODE_ADL`` or
``AGON_MOS_HEADER AGON_MOS_RUNMODE_Z80``.
