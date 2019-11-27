Temporary Files
===============

Note that WLA will generate an temporary files in the current working
directory while it works. On Windows and Unix-like systems, the file is
called ``.wla%PID%a``, where ``%PID%`` is the PID of the process.
For other system, it's just ``wla_a.tmp``.

When WLA finishes its work these two files are deleted as they serve
of no further use.
