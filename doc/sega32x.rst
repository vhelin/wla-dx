Sega 32X support
================

WLA DX provides initial Sega 32X support through reusable memory maps and include
files. The 32X is a Mega Drive/Genesis cartridge system with two SH-2 CPUs, so a
complete ROM normally has both 68000-side boot/support code and SH-2-side code.

Keep the two CPU families in separate link steps. WLALINK intentionally rejects
mixing SH-2 objects with non-SH-2 objects. A 32X project should link the 68000
payload and SH-2 payload separately, then compose the final raw cartridge image
from binaries.

Memory maps
-----------

``memorymaps/md32x68k.i`` is for 68000-side 32X support code. It includes the
raw cartridge window, the common Mars 68000 hot-start mirror around ``$880000``,
32X framebuffer and palette windows, adapter registers, Z80 RAM, Mega Drive VDP
ports, and 68000 work RAM.

``memorymaps/sh232x.i`` is for SH-2-side code. It maps raw cartridge file offset
0 to SH-2 address ``$02000000``, matching the way MAME exposes the 32X
``gamecart_sh2`` region. It also includes SDRAM, Mars registers, palette RAM,
and framebuffer windows. SH-2 internal registers such as ``$FFFFFE92`` are
provided as constants in ``include/sega32x/sh2.inc`` rather than as memory-map
slots because WLA memory-map slot starts are signed-positive values.

Include files
-------------

``include/sega32x/68k.inc`` defines the 68000-visible 32X adapter registers at
``$A15100`` and related constants. It also includes small helper macros for
enabling the adapter, emitting a source-built boot jump to the 32X hot-start
mirror, toggling FM/32X hardware access, asserting command interrupts to both
SH-2s, and setting or clearing the RV bit.

``include/sega32x/sh2.inc`` defines SH-2-visible Mars registers, communication
ports, PWM registers, 32X VDP registers, framebuffer and palette addresses, SH-2
cache/internal register constants, and simple interrupt-mask helper macros.

``include/sega32x/macros.inc`` contains shared video and color constants usable
from either side.

ROM layout notes
----------------

Standard 32X cartridges use a Mega Drive ROM header at ``$100``, a Mars header
around ``$3C0``, and 68000 bootstrap code at ``$3F0``. The Mars header contains
the primary and secondary SH-2 entry points and vector-base addresses. Use
``.DD`` for 32-bit SH-2/Mars header fields; ``.DL`` is a 24-bit directive.

The ``examples/sh2/hello_world_32x`` example shows the intended packaging
pattern: keep the standard 32X startup blob, the 68000 hot-start support code,
and the SH-2 payload in separate steps, then compose the raw cartridge image
with ``.INCBIN``. The Mars header can point the startup blob at the SH-2 payload
in ROM and request that it be copied to SDRAM before the SH-2 CPUs start.

The ``examples/sh2/hello_world_32x`` makefile launches Ares with the ``Mega 32X``
system for true SH-2 execution. It auto-detects
``~/Downloads/ares-v147/ares.exe`` under Cygwin, or you can override the
``ARES`` variable.