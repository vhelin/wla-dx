Quickstart
==========

Every assembly file needs to begin with the definition of ``.MEMORYMAP`` and ``.ROMBANKMAP``. You can put these
inside a separate file that is included at the beginning of the assembly files. Here's an example::

    .MEMORYMAP
    DEFAULTSLOT 1
    SLOT 0 START $0000 SIZE $2000
    SLOT 1 START $2000 SIZE $2000
    .ENDME

    .ROMBANKMAP
    BANKSTOTAL 2
    BANKSIZE $2000
    BANKS 8
    .ENDRO

Right after these, before any code is written, you should define bank, slot and org::

    .BANK 0 SLOT 0
    .ORGA $0000

Now you are ready to start programming!

The next step would be creating a linkfile for the linker as after the assembly files go through the assembler they need to be linked.
Here is a minimal example of a linkfile when your project has just main.s (that has been assembled into main.o)::

    [objects]
    main.o

Give this to the linker and you'll get the final binaries.
