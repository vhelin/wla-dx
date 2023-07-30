WLA Flags
=========

Here are short descriptions for the flags you can give to WLA:

You can supply WLA with some (or all or none) of the following option flags::

    -c  Continue parsing after an error. Currently we can only continue after
        encountering an unknown symbol or a mistyped instruction.
    -d  Disable WLA's ability to calculate A-B where A and B are labels
    -h  Assume that all label references are 16-bit by default (size hints
        still work). Without this flag it's assumed that label references are
        8-bit unless otherwise specified.
    -i  Add list file information. Adds extra information to the output so
        WLALINK can produce list files.
    -k  Keep all empty sections. By default they are discarded.
    -M  WLA generates makefile rules describing the dependencies of the main
        source file.
    -MP Create a phony target for each dependency other than the main file,
        use this with -M.
    -MF Specify a file to write the dependencies to, use this with -M.
    -q  Quiet mode. ``.PRINT*`` -directives output nothing.
    -s  Don't create _sizeof_* and _padding_* definitions.
    -t  Test assemble. Doesn't output any files.
    -v  Verbose mode. Shows a lot of information about the compiling process.
    -v1 Verbose messages (only discard sections)
    -v2 Verbose messages (-v1 plus short summary)
    -w  Require labels to end in a colon.
    -x  Extra compile time labels and definitions. WLA does extra work by creating
        few helpful definitions, and labels SECTIONSTART_[section name] and
        SECTIONEND_[section name] at the beginning and end of a section.
    -D  Declare a definition.
    
One (and only one) of the following command flags must be defined.

    -l  Output a library file.
    -o  Output an object file.

You may also use an extra option to specify the include directory. WLA will
search this directory for included files before defaulting to the specified
``.INCDIR`` or current working directory::

    -I  Directory to include files.

Examples::

    [seravy@localhost tbp]# wla -D VERSION=255 -x -v -i -o testa.o testa.s
    [seravy@localhost tbp]# wla -M testa.s
    [seravy@localhost tbp]# wla -D VERSION=$FF -D MESSAGE=\"Hello world\" -l testb.lib testb.s
    [seravy@localhost tbp]# wla -I ../../include -l testb.lib testb.s
    [seravy@localhost tbp]# wla -M -I myfiles -l testa.lib testa.s
    
NOTE: If you use -M and -l/-o at the same time, specify -M first on the command line.
    
NOTE: The first example produces a file named ``testa.o``.
