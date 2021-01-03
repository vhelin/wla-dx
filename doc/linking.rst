Linking
=======

After you have produced one or more object files and perhaps some library
files, you might want to link them together to produce a ROM image / program
file. WLALINK is the program you use for that. Here's how you use it::

    wlalink [OPTIONS] <LINK FILE> <OUTPUT FILE>

Choose the option ``-b [OUT]`` for program file or ``-r [OUT]`` for
ROM image linking. ROM image is all the data in the ROM banks. Program file
is the data between the first used byte and the last used byte. You can also use
``-bS [START ADDRESS]`` and ``-bE [END ADDRESS]`` to specify the start and
the end addresses of the program. Both are optional.

Link file is a text file that contains information about the files you want
to link together. Here's the format:

1. You must define the group for the files. Put the name of the group
   inside brackets. Valid group definitions are ::
       
        [objects]
        [libraries]
        [header]
        [footer]
        [definitions]
	[ramsections]
	[sections]

2. Start to list the file names. ::
    
        [objects]
        main.o
        vbi.o
        level_01.o
        ...

3. Give parameters to the library files::
   
        [libraries]
        bank 0 slot 1 speed.lib
        bank 4 slot 2 map_data.lib
        ...

   Here you can also use ``base`` to define the 65816 CPU bank number
   (like ``.BASE`` works in WLA)::
   
       [libraries]
       bank 0 slot 1 base $80 speed.lib
       bank 4 slot 2 base $80 map_data.lib
       ...

   You must tell WLALINK the bank and the slot for the library files.

4. If you want to use header and/or footer in your project,
   you can type the following::
   
        [header]
        header.dat
        [footer]
        footer.dat

5. If you have RAMSECTIONs inside the libraries, you must place
   the sections inside BANKs and SLOTs (ORG and ORGA are optional).
   Note that you can also change the type and priority of the section,
   and can use appendto::

        [ramsections]
        bank 0 slot 3 org $0 "library 1 vars 1"
	bank 0 slot 3 orga $6100 priority 100 force "library 1 vars 2"
	bank 0 slot 3 appendto "library 1 vars 2" "library 1 vars 3"

6. If you want to relocate normal sections, do as follows (ORG, ORGA,
   KEEP, PRIORITY and APPENDTO are optional, but useful)::

        [sections]
	bank 0 slot 1 org $100 appendto "MusicPlayers" "MusicPlayer1"
	bank 0 slot 1 orga $2200 semisubfree priority 100 keep "EnemyAI"
     
7. If you want to make value definitions, here's your chance::
   
        [definitions]
        debug 1
        max_str_len 128
        start $150
        ...

If flag ``v`` is used, WLALINK displays information about ROM file after a
succesful linking.

If flag ``nS`` is used, WLALINK doesn't sort the sections at all, so they
are placed in the output in their order of appearance.

If flag ``s`` is used, WLALINK will produce a NO$GMB/NO$SNES symbol file. It's
useful when you work under MSDOS (NO$GMB is a very good Game Boy emulator for
MSDOS/Windows) as it contains information about the labels in your project.

If flag ``S`` is used, WLALINK will create a WLA symbol file, that is much
like NO$GMB symbol file, but shows also symbols, defines, and breakpoints, not
just labels.

If flag ``d`` is used, WLALINK discards all unreferenced ``FREE``, ``SEMIFREE``,
``SEMISUBFREE``, ``SUPERFREE`` and ``RAM`` sections. This way you can link big
libraries to your project and WLALINK will choose only the used sections, so you
won't be linking any dead code/data.

If flag ``D`` is used, WLALINK doesn't create any _sizeof_* labels. Note that
to disable fully _sizeof_* label creation, you'll also need to give WLA the
``s`` flag.

If flag ``t`` is used with ``c64PRG``, WLALINK will add a two byte header to the
program file (use with flag ``b``). The header contains the load address for
the PRG. Use the flag ``a`` to specify the load address. It can be a value or
the name of a label.

If flag ``i`` is given, WLALINK will write list files. Note that you must
compile the object and library files with ``-i`` flag as well. Otherwise
WLALINK has no extra information it needs to build list files. Here is an
example of a list file: Let's assume you've compiled a source file called
``main.s`` using the ``i`` flag. After you've linked the result also with the
``i`` flag WLALINK has created a list file called ``main.lst``. This file
contains the source text and the result data the source compiled into. List
files are good for debugging. NOTE: list file data can currently be generated
only for code inside sections. ``.MACRO`` calls and ``.REPT`` s don't produce
list file data either.

If flag ``L`` is given after the above options, WLALINK will use the
directory specified after the flag for including libraries. If WLALINK
cannot find the library in the specified directory, it will then silently
search the current working directory. This is useful when using WLA in an SDK
environment where a global path is needed.

Make sure you don't create duplicate labels in different places in the
memory map as they break the linking loop. Duplicate labels are allowed when
they overlap each other in the destination machine's memory. Look at the
following example::

    ...
    .BANK 0
    .ORG $150
    
    	...
    	LD	A, 1
    	CALL	LOAD_LEVEL
    	...
    
    LOAD_LEVEL:
    	LD	HL, $2000
    	LD	(HL), A
    	CALL	INIT_LEVEL
    	RET
    
    .BANK 1
    .ORG 0
    
    INIT_LEVEL:
    	...
    	RET
    
    .BANK 2
    .ORG $0
    
    INIT_LEVEL:
    	...
    	RET
    ...


Here duplicate ``INIT_LEVEL`` labels are accepted as they both point to the
same memory address (in the program's point of view).

Examples::

    [seravy@localhost tbp]# wlalink -r linkfile testa.sfc
    [seravy@localhost tbp]# wlalink -d -i -b linkfile testb.sfc
    [seravy@localhost tbp]# wlalink -v -S -L ../../lib linkfile testc.sfc
    [seravy@localhost tbp]# wlalink -v -b -s -t c64PRG -a LOAD_ADDRESS linkfile linked.prg
