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

The following operators are valid:

====== ==================
``,``  comma
``|``  or
``&``  and
``^``  power
``<<`` shift left
``>>`` shift right
``+``  plus
``-``  minus
``#``  modulo
``~``  xor
``*``  multiply
``/``  divide
``<``  get the low byte
``>``  get the high byte
====== ==================

Note that you can do NOT using XOR:

- ``VALUE_A ~ $FF``   is  8-bit NOT
- ``VALUE_B ~ $FFFF`` is 16-bit NOT

Unary XOR (e.g., ``~$FF``) is the same as NOT.
  
WLA computes internally with real numbers so ``(5/2)*2`` produces ``5``,
not ``4``.
