SH-2 support
============

WLA DX includes a plain Hitachi SH-2 target named ``wla-sh2``. It supports the
base SH-2 integer instruction set only. Later SH extensions are outside this
target. Sega 32X support is provided as memory maps and include files on top of
the plain SH-2 target; see :doc:`sega32x`.

The assembler emits fixed-width 16-bit instructions in big-endian order.
General purpose registers are written as ``R0`` through ``R15``. Control and
system registers supported by the transfer instructions are ``SR``, ``GBR``,
``VBR``, ``MACH``, ``MACL`` and ``PR``.

Addressing syntax follows the SH manuals, including ``@Rn``, ``@Rn+``,
``@-Rn``, ``@(disp,Rn)``, ``@(R0,Rn)``, ``@(disp,GBR)`` and ``@(disp,PC)``.
Displacements are range-checked and scaled according to the instruction size.
For PC-relative word and longword loads, labels are resolved by WLAlink and must
be aligned for the selected access size.

Branch labels for ``bf``, ``bt``, ``bf/s``, ``bt/s``, ``bra`` and ``bsr`` are
resolved relative to ``PC+4``. WLA DX does not currently insert delay-slot
instructions automatically, but it does perform some validation and emits
warnings for problematic delay-slot contents such as branch, jump and return
instructions, so source code must still provide correct delay-slot instructions.
