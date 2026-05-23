# HELLO WORLD - NES homebrew example

Minimal NES NROM program built with the WLA DX toolchain. It shows a black
screen with `HELLO WORLD` centered in the nametable and plays two short pulse
channel beeps.

The example uses CHR RAM, uploads a tiny embedded tile font at boot, clears the
nametable, writes the text, enables background rendering, and then plays the
two beeps through the NES APU.

## Build

From this directory:

```sh
make
```

The build produces `hello.nes` and `hello.sym`.

## Run

```sh
make run
```

The `run` target launches Mesen from `C:/Users/ville/Desktop/Mesen.exe` by
default. On Cygwin it passes Mesen a Windows-style ROM path with `cygpath`.
Override `MESEN` or `MESEN_FLAGS` to use a different emulator path or extra
options.