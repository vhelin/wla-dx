Binary to DB Conversion
=======================

WLAB converts binary files to WLA's byte definition strings. Here's how
you use it::

    wlab -[ap]{bdh} <BIN FILE>

Give it the binary file and WLAB will output the WLA DB formatted data of it
into stdout. Here's an example from real life::

    wlab -da gayskeletor.bin > gayskeletor.s

WLAB has three command flags of which one must be given to WLAB:

-b  Output data in binary format.
-d  Output data in decimal format.
-h  Output data in hexadecimal format.

WLAB has also two option flags:

-a  Print the address (relative to the beginning of the data).
-p  Don't print file header.

Examples::

    [seravy@localhost src]# wlab -bap iscandar.bin > iscandar.s
    [seravy@localhost src]# wlab -h starsha.bin > starsha.s
