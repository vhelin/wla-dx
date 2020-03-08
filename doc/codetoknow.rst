Things you should know about coding for...
==========================================

Please check out the source code examples (in ``examples`` directory) for
quick target system specific information.

Z80
---

Check the Z80 specific directives. All SMS/GG coders should find ``.SMSTAG``,
``.SDSCTAG`` and ``.COMPUTESMSCHECKSUM`` very useful...

There are shadow register aliases for opcodes that use registers A, F, BC, DE and HL.
The shadow register versions are just for convenience, if the programmer wants to
explicitly show that he is now using the shadow registers. For example:

  AND A     ; (original, assembles to 0xA7)
  AND A'    ; (alias, assembles to 0xA7 and is in reality "AND A")

Opcodes that make relative label references::

    JR *
    DJNZ

.. _codesub6502:

6502
----

For example mnemonics ``ADC``, ``AND``, ``ASL``, etc... cause problems to WLA,
because they take different sized arguments. Take a look at this::

    LSR 11       ; $46 $0B
    LSR $A000    ; $4E $00 $A0

The first one could also be ::

    LSR 11       ; $4E $0B $00

To really get what you want, use ``.8BIT``, ``.16BIT`` and ``.24BIT``
directives. Or even better, supply WLA the size of the argument::

    LSR 11.W     ; $4E $0B $00

Opcodes that make relative label references::

    BCC
    BCS
    BEQ
    BMI
    BNE
    BPL
    BVC
    BVS


65C02
-----

Read the subsection :ref:`codesub6502` as the information applies also to 65C02
coding...

Opcodes that make relative label references::

    BCC
    BCS
    BEQ
    BMI
    BNE
    BPL
    BVC
    BVS
    BRA
    BBR*
    BBS*


65CE02
------

Read the subsection :ref:`codesub6502` as the information applies also to 65CE02
coding...

Opcodes that make relative label references::

    BCC
    BCS
    BEQ
    BMI
    BNE
    BPL
    BVC
    BVS
    BRA
    BSR
    BBR*
    BBS*


6510
----

Read the subsection :ref:`codesub6502` as the information applies also to 6510
coding...

Opcodes that make relative label references::

    BCC
    BCS
    BEQ
    BMI
    BNE
    BPL
    BVC
    BVS


65816
-----

Read the subsection :ref:`codesub6502` as the information applies also to 65816
coding...

WLA-65816 has also few SNES specific directives which are all very
helpful. Remember that when you use ``.LOROM``, ``.HIROM``, ``.SLOWROM`` and
``.FASTROM`` WLA automatically writes the information into the output.
``.COMPUTESNESCHECKSUM``, ``.SNESHEADER`` and few others could also be useful.

Use ``.BASE`` to set the upmost eight bits of 24-bit addresses.

If possible, use operand hints to specify the size of the operand.
WLA is able to deduce the accumulator/index mode to some extent from
``REP``/``SEP``-mnemonics and ``.ACCU`` and ``.INDEX``-directives, but just to
be sure, terminate the operand with ``.B``, ``.W`` or ``.L``. ::

    AND #10     ; can be two different things, depending on the size of the accu.
    AND #10.B   ; forces 8-bit immediate value.
    AND #10.W   ; forces 16-bit immediate value.

Or if you must, these work as well::

    AND.B #10   ; the same as "AND #10.B".
    AND.W #10   ; the same as "AND #10.W".

Opcodes that make relative label references::

    BCC
    BCS
    BEQ
    BMI
    BNE
    BPL
    BVC
    BVS
    BRA
    BRL
    PER

Use ``.WDC`` to start parsing WDC standard assembly code. ``.NOWDC`` sets
the parser to parse WLA syntax assembly code.


HUC6280
-------

Read the subsection :ref:`codesub6502` as the information applies also to
HUC6280 coding...

Opcodes that make relative label references::

    BCC
    BCS
    BEQ
    BMI
    BNE
    BPL
    BVC
    BVS
    BSR
    BBR*
    BBS*


SPC-700
-------

Note that you'll have to put an exclamation mark before a 16-bit value.
For example, ::

    CALL !Main
    AND  A, !$1000

Opcodes that make relative label references::

    BCC
    BCS
    BEQ
    BMI
    BNE
    BPL
    BVC
    BVS
    BRA
    BBS
    BBC
    CBNE *
    DBNZ *


Pocket Voice (GB-Z80)
---------------------

Pocket Voice uses its own MBC. You can enable Pocket Voice mode by selecting
Pocket Voice cartridge type (``$BE`` in ``$0147``) and defining correct
``.ROMBANKMAP`` and ``.MEMORYMAP``. In PV mode bank 0 is 24KB and the rest are
8KB.

Note that WLA assumes that ROM offset is all the time 0. If you use
something else as the offset, make sure to compute the jumps by hand as WLA
cannot do that.

Check out ``examples/gb-z80/include/pocket_voice.i`` for more information.


GB-Z80
------

WLA outputs only ``$10`` when it decodes ``STOP``. Often it's necessary to put
an extra ``NOP`` (``$00``) after a ``STOP``, and sometimes something else, but
that's left entirely to the user.

Opcodes that make relative label references::

    JR *
