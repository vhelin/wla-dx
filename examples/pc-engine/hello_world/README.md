# HELLO WORLD - PC Engine homebrew example

Minimal PC Engine / TurboGrafx-16 HuCard program built with the WLA DX
toolchain. It shows a black screen with `HELLO WORLD` centered in the background
tile map and plays two short PSG beeps.

The example uses [../../../memorymaps/pcengine.i](../../../memorymaps/pcengine.i)
and the shared [../../../include/pcengine](../../../include/pcengine) hardware,
vector, and macro helpers. It uploads a tiny embedded 4bpp tile font into VDC
VRAM, fills the background attribute table with a blank tile, writes the text,
loads a square wave into PSG channel 0, and plays two notes.

## Build

From this directory:

```sh
make
```

The build produces `hello.pce` and `hello.sym`.

## Run

```sh
make run
```

The `run` target launches Mesen from `C:/Users/ville/Desktop/Mesen.exe` by
default. On Cygwin it passes Mesen a Windows-style ROM path with `cygpath`.
Override `MESEN` or `MESEN_FLAGS` to use a different emulator path or extra
options.