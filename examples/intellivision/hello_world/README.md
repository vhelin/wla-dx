# HELLO WORLD - Intellivision homebrew example

Minimal Intellivision cartridge built with WLA DX (`wla-cp1610` + `wlalink`).
After the EXEC title screen, the program takes over, fills the background
with black, draws `HELLO WORLD` in white GROM text at the center of the
20x12 BACKTAB, and plays three short PSG beeps. A vblank ISR keeps the STIC
handshake alive (write `$0020`, color-stack mode, black border) so the
screen does not blank. Beeps use the AY-3-8914 register map at `$01F0`.

The example uses [../../../memorymaps/intellivision_cart.i](../../../memorymaps/intellivision_cart.i).
WLA tracks **byte** addresses, so the cartridge that the CP1610 sees at
`$5000` is assembled at `$A000`. Instruction operands still encode CPU word
addresses; `.DW label/2` is used in the EXEC ROM header.

## Build

From this directory:

```sh
make
```

The build produces `hello.bin` and `hello.sym`. `hello.cfg` maps the 8KB image
to CPU `$5000-$5FFF` for jzIntv.

## Run

```sh
make run
```

The `run` target launches jzIntv from
`C:/Users/ville/Downloads/jzintv-20200712-win32-sdl2/bin/jzintv.exe` by
default. On Cygwin it passes jzIntv a Windows-style ROM path with `cygpath`.
Override `JZINTV` or `JZINTV_FLAGS` to use a different emulator path or extra
options.

jzIntv needs the Master Component EXEC and GROM images (`exec.bin` and
`grom.bin`). Point `JZINTV_ROM_PATH` at the directory that contains them, or
pass `-e` / `-g` through `JZINTV_FLAGS`.
