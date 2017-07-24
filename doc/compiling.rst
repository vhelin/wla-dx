Compiling
=========

Compiling Object Files
----------------------

To compile an object file use the ``-o [OUT]`` option on the command line.

These object files can be linked together (or with library files) later
with WLALINK.

Name object files so that they can be recognized as object files. Normal
suffix is ``.o`` (WLA default). This can also be changed with ``.OUTNAME``.

With object files you can reduce the amount of compiling when editing
small parts of the program. Note also the possibility of using local
labels (starting with ``_``).

**Note**: When you compile objects, group 1 directives are saved for linking
time, when they are all compared and if they differ, an error message is
shown. It is advisable to use something like an include file to hold all
the group 1 directives for that particular project and include it to every
object file.

If you are interested in the WLA object file format, take a look at the
file ``txt/wla_file_formats.txt`` which is included in the release archive.

Here are some examples of definitions:

- ``-D IEXIST``
- ``-D DAY=10``
- ``-D BASE = $10``
- ``-D NAME=elvis``

And here's an WLA example creating definitions on the command line::

    wla-gb -D DEBUG -D VERBOSE=5 -D NAME = "math v1.0" -o math.o math.s

``DEBUG``'s value will be ``0``, ``VERBOSE``'s ``5`` and ``NAME`` is a
string definition with value ``math v1.0``.
Note that ``-D`` always needs a space after it, but the rest of the statement
can be optionally stuck inside one word.


Compiling Library Files
-----------------------

To compile a library file use the ``-l [OUT]`` option on the command line.

Name these files so that they can be recognized as library files. Normal
suffix is ``.lib`` (WLA default).

With library files you can reduce the amount of compiling. Library files
are meant to hold general functions that can be used in different projects.
Note also the possibility of using local labels (starting with ``_``).
Library files consist only of ``FREE`` sections.