Files
=====

``examples``
------------

The main purpose of the files in the ``examples`` directory is to test that WLA
and WLALINK can assemble and link the tiny project correctly. You can also
take a look at the code and syntax in the files, but beware: if you run the
rom files you probably don't see anything on screen.

``include`` directory under ``gb-z80`` could be very useful as the six include
files there have all the Game Boy hardware register address and memory
definitions you could ever need and more.


``examples/gb-z80/lib``
-----------------------

This folder holds few very useful libraries for you to use in your Game Boy
projects. Instead of reinventing the wheel, use the stuff found in here.
Remember to compile the libraries right after you've installed WLA by
executing ``make`` in the ``lib`` directory.


``memorymaps``
--------------

Here you can find default memory maps (see ``.MEMORYMAP``) for various computers
and video game consoles.
