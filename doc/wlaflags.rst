WLA Flags
=========

Here are short descriptions for the flags you can give to WLA:

You can supply WLA with some (or all or none) of the following option flags.

-i  Add list file information. Adds extra information to the output so
    WLALINK can produce list files.
-M  WLA generates makefile rules describing the dependencies of the main
    source file. Use only with flags ``o`` and ``l``.
-q  Quiet mode. ``.PRINT*`` -directives output nothing.
-t  Test compile. Doesn't output any files.
-v  Verbose mode. Shows a lot of information about the compiling process.
-x  Extra compile time definitions. WLA does extra work by creating
    few helpful definitions on the fly.

One (and only one) of the following command flags must be defined.

-l  Output a library file.
-o  Output an object file.

You may also use an extra option to specify the include directory. WLA will
search this directory for included files before defaulting to the specified
``.INCDIR`` or current working directory.

-I  Directory to include files.

Examples::

    [seravy@localhost tbp]# wla -v -i -o testa.o testa.s
    [seravy@localhost tbp]# wla -M -o testa.o testa.s
    [seravy@localhost tbp]# wla -l testb.lib testb.s
    [seravy@localhost tbp]# wla -I ../../include -l testb.lib testb.s
    [seravy@localhost tbp]# wla -M -I myfiles -l testa.lib testa.s

Note that the first example produces file named ``testa.o``.