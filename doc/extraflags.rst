Extra compile time definitions
==============================

When you supply WLA with the flag ``x`` it will maintain few useful definitions
and labels while compiling your source codes. Please use the enhanced error
reporting engine (so don't use flag ``f``) in conjunction with flag ``x`` as
some of the definitions require extra information about the flow of the data
which isn't available when using the old, crippled error reporting engine.

Here's a list of definitions you get when you use flag ``x``:

+------------------+--------------------------------------------------------+
| ``WLA_FILENAME`` | A string definition holding the file name WLA is       |
|                  | currently processing.                                  |
+------------------+--------------------------------------------------------+
| ``WLA_TIME``     | A string definition holding the calendar time          |
|                  | (obtained using C's ``ctime()``).                      |
+------------------+--------------------------------------------------------+
| ``WLA_VERSION``  | A string definition holding the version number of WLA. |
+------------------+--------------------------------------------------------+

So you can do for example something like

::

    .DB WLA_TIME

to store the time when the build process started into the ROM file you
are compiling.

Definition ``CADDR``, which is present without supplying the flag ``x``,
contains the current 16-bit memory address. So

::

    LD HL, CADDR

will load the address of the operand data into registers ``H`` and ``L``.

**CAVEAT**:
Remember when using defines that ``CADDR`` gets the address of the place where
the definition is used, not the address of the definition, which contains
the ``CADDR``.

Note that you'll also get all these definitions in lower case
(e.g., ``wla_filename``).

But that is not all. You will also get SECTIONSTART_[section name] labels
that are inserted into the start of every section, and SECTIONEND_[section name]
labels that are inserted into the end of every section.
