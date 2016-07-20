Disassembling
=============

**WARNING**: WLAD is not being maintained any longer, and no plans regarding it
have been made.

WLAD is a disassembler that can be used to disassemble an existing GB-ROM
image into a WLA-syntax compatible source file. It won't produce very readable
code as all the labels etc. are discarded in the last phase of linking when
the ROM image is made. In the current state WLAD doesn't generate labels,
but it will in the future.

Here's how you use it::

    wlad [-dsa] <BIN FILE>

Give it the binary file (usually the extension is ``.gb`` or ``.cgb``) and
WLAD will output the source code into stdout (the shell you are using). Here's
an example what you can do if you want the source code into a file instead
of the screen::

    wlad tbp.cgb > tbp.s

WLAD has three option flags:

-a  Disable address output.
-d  The upper banks (1->) are disassembled as data.
-s  The upper banks (1->) are disassembled as data with string detection
    enabled.

Use the ``s``-flag if you intend to change texts in games. The flags ``d``
and ``s`` cannot be used at the same time.

Examples::

    [seravy@localhost parallax]# wlad -da parallax.gb > parallax.s
    [seravy@localhost parallax]# wlad -s parallax.gb > parallax.s

WLAD supports both normal GB-ROMs and Pocket Voice™ ROM images.
Only WLAD GB-Z80 exists curently.