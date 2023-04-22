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
