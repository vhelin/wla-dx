Files
=====

``tests``
---------

The main purpose of the files in the ``tests`` directory is to test that WLA
and WLALINK can assemble and link the tiny project correctly. You can also
take a look at the code and syntax in the files, but beware: if you run the
rom files you probably don't see anything on screen.

``include`` directory under ``gb-z80`` could be very useful as the six include
files there have all the Game Boy hardware register address and memory
definitions you could ever need and more.

``include/megadrive`` contains Sega Mega Drive/Genesis and Sega CD register
definitions and helper macros for the 68000 side, Z80 side, VDP, Sega CD main
CPU, Sega CD sub CPU, J-Cart and common serial EEPROM wiring.


``tests/gb-z80/lib``
--------------------

This folder holds few very useful libraries for you to use in your Game Boy
projects. Instead of reinventing the wheel, use the stuff found in here.
Remember to compile the libraries right after you've installed WLA by
executing ``make`` in the ``lib`` directory.


``memorymaps``
--------------

Here you can find default memory maps (see ``.MEMORYMAP``) for various computers
and video game consoles.

The Mega Drive/Genesis maps include the normal 68000 and Z80 maps, SSF banking,
serial EEPROM cartridges, J-Cart cartridges, lock-on cartridges, and Sega CD
main/sub CPU boot program layouts.
