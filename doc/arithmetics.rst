Arithmetics
===========

WLA is able to solve really complex calculations like ::

    -((HELLO / 2) | 3)
    skeletor_end-skeletor
    10/2.5

so you can write something like ::

    LD HL, data_end-data
    LD A, (pointer + 1)
    CP (TEST + %100) & %10101010

WLALINK also has this ability so it can compute the pending calculations
WLA wasn't able to solve.

NOTE! The assembler has only a limited capability to turn labels into addresses. Often
label references are left for the linker to solve. Currently the assembler
can do so when the label is outside ``.SECTION`` s or inside ``FORCE`` or
``OVERWRITE`` ``.SECTION`` s and the label is defined before it is referenced.
Many directives like ``.ASSERT`` require data that the assembler can immediately
solve so you might run into problems when feeding labels to directives.

The following operators are valid:

====== ===============================
``|``  bitwise or
``&``  bitwise and
``^``  power
``<<`` bitwise shift left
``>>`` bitwise shift right
``+``  plus
``-``  minus
``#``  modulo
``~``  bitwise xor
``*``  multiply
``/``  divide
``<``  get the low byte
``>``  get the high byte
``:``  get the bank byte of an address
====== ===============================

Note that you can do NOT using XOR ::

- ``VALUE_A ~ $FF``   is  8-bit NOT
- ``VALUE_B ~ $FFFF`` is 16-bit NOT

Unary XOR (e.g., ``~$FF``) is the same as NOT.
  
``.IF`` conditions have the following additional operators:

====== ===============================================================
``!``  not
``<``  smaller than (note that outside ``.IF`` this is something else)
``>``  larger than (note that outside ``.IF`` this is something else)
``<=`` smaller or equal
``>=`` larger or equal
``==`` equal
``!=`` unequal
``||`` logical or
``&&`` logical and
====== ===============================================================

Here's a table of the precedence of the operators in calculations and conditions
(higher priority operators come first):

========================= =============================================
``(`` ``)``               expression
``~`` ``!``               unary
``<`` ``>`` ``:``         low byte / high byte / bank (outside ``.IF``)
``/`` ``*`` ``#`` ``^``   multiplicative
``+`` ``-``               additive
``<<`` ``>>``             bitwise shift
``<`` ``>`` ``<=`` ``>=`` relational (only inside ``.IF``)
``==`` ``!=``             equality (only inside ``.IF``)
``&``                     bitwise and
``~``                     bitwise xor
``|``                     bitwise or
``&&``                    logical and
``||``                    logical or
========================= =============================================

WLA computes internally with real numbers so ``(5/2)*2`` produces ``5``,
not ``4``.
