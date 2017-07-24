Temporary Files
===============

Note that WLA will generate two temporary files while it works. Both files
are placed into the current working directory.

======== ============== ==============
System   File A         File B
======== ============== ==============
Amiga    ``wla_a.tmp``  ``wla_b.tmp``
MSDOS    ``wla_a.tmp``  ``wla_b.tmp``
Win32    ``.wla%PID%a`` ``.wla%PID%b``
Unix     ``.wla%PID%a`` ``.wla%PID%b``
AmigaOS4 ``.wla%PID%a`` ``.wla%PID%b``
======== ============== ==============

(``%PID%`` is the process id in cases where it is included)

When WLA finishes its work these two files are deleted as they serve
of no further use.