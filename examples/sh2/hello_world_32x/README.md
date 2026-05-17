Sega 32X SH-2 HELLO WORLD
=========================

This example builds a raw Sega 32X cartridge image that starts from the standard
32X startup blob, then lets the SH-2 side draw `HELLO WORLD` into the 32X
framebuffer and play two PWM beeps. The output cartridge is `hello32x.32x` so
Ares can recognize it as a 32X ROM.

The startup blob is expected at `mars_startup.blob` by default and must be 1040
bytes, covering cartridge offsets `$3F0..$7FF`. You can override the path with
`MARS_STARTUP_BLOB=/path/to/blob`. The blob used while developing this example
was generated from Marsdev's proven 32X skeleton startup data and has SHA-256
`1560453EBB511BFBEC67BE147C0C067F926C4012659BF8BF65D9478AE2AC7103`.

Build it with:

```sh
make
```

Run it in Ares with:

```sh
make run
```

The `run` target uses `ARES` if set, otherwise it auto-detects
`~/Downloads/ares-v147/ares.exe` under Cygwin. It launches the ROM with
`--system "Mega 32X"`. Override `ARES_FLAGS` to pass extra emulator options, for
example:

```sh
make run ARES_FLAGS="--fullscreen"
```

The build deliberately keeps the 68000 startup blob, 68000 hot-start support
code, and SH-2 payload in separate steps. The final cartridge image is composed
by the SH-2 source with `.INCBIN` payloads, preserving WLALINK's SH-2/non-SH-2
object mixing checks. The Mars header asks the startup blob to copy the SH-2
payload from ROM offset `$1000` to SDRAM before jumping to it.