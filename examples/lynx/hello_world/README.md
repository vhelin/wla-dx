# Atari Lynx Hello World

This example builds a bootable `.lnx` cartridge image with WLA-65C02. It uses `.LYNXHEADER`, a small signed Lynx cart loader, and a one-entry cart directory that loads the main program to RAM at `$0200`.

The program clears the 160x102 4bpp framebuffer to black, draws `HELLO WORLD` in the middle of the screen, plays two short beeps on audio channel 0, and then loops forever.

Build it with:

```sh
make
```

Run it in Handy with:

```sh
make run
```

`make run` defaults to `C:/Users/ville/Downloads/Handy-0.95`. Override `HANDY_DIR` if Handy lives somewhere else.