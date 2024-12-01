Assembler Syntax
================

Case Sensitivity
----------------

WLA is case sensitive, except with directives, so be careful.


Comments
--------

Comments begin with ``;`` or ``*`` and end along with the line. ``;`` can be
used anywhere, but ``*`` can be placed only at the beginning of a new line.

WLA supports also ANSI C style commenting. This means you can start a
multiline comment with ``/*`` and end it with ``*/``.

What also is supported are C++ style comments. This means you can start a
comment with ``//``.

You can also use ``.ASM`` and ``.ENDASM`` directives to skip characters. These
function much like ANSI C comments, but unlike the ANSI C comments these can be
nested.


Line splitting
--------------

Lines can be split using a ``\`` between elements. So instead of writing

    .db 1, 2, 3, 4, 5, 6, 7, 8

it's possible to write

    .db 1, 2, 3, 4 \\
        5, 6, 7, 8

Note that line splitting works only in places where WLA expects a new label,
number, calculation, etc. String splitting isn't currently supported.


Using Commas
------------

In many places it's possible to give parameters without commas between them::

    .db 1 2 3 4 5 ; 01 02 03 04 05

**CAVEAT! CAVEAT! CAVEAT!**

If you specify the following

    .db 1 -2 3 -4 5 ; FF FF 05

WLA will detect and compute calculations, so to be sure, always use commas::

    .db 1, -2, 3, -4, 5 ; 01 FE 03 FC 05

    
Labels
------

Labels are ordinary strings (which can also end with a ``:``). Labels starting
with ``_`` are considered to be local labels and do not show outside sections
where they were defined, or outside object files, if they were not defined
inside a section.

Here are few examples of different labels::

    VBI_IRQ:
    VBI_IRQ2
    _VBI_LOOP:
    main:

Labels starting with ``@`` are considered to be child labels. They can only be
referenced within the scope of their parent labels, unless the full name is
specified. When there is more than one ``@``, the label is considered to be
a child of a child.

Here are some examples of child labels::

    PARENT1:
    @CHILD:
    @@SUBCHILD
    
    PARENT2:
    @CHILD:

This is legal, since each of the ``@CHILD`` labels has a different parent.
You can specify a parent to be explicit, like so::

    jr PARENT1@CHILD@SUBCHILD

You can also use ``__label__`` to refer to the last defined parent label::

    main:                 ; #
            nop
            nop
    @child:
            nop
            nop
    @@grandchild:
            nop
            nop
            jmp __label__ ; jump -> #
    loop:   nop           ; %
            nop
            jmp __label__ ; jump -> %

Note that when you place ``:`` in front of the label string when referring to
it, you'll get the bank number of the label, instead of the label's address.
Here's an example::

    LD A, :LOOP
    .BANK 2 SLOT 0
    LOOP:

Here ``LD A, :LOOP`` will be replaced with ``LD A, 2`` as the label ``LOOP``
is inside the bank number two.

When you are referring to a label and you are adding something to its address
(or subtracting, any arithmetics apply) the result will always be bytes. ::

    .org 20
    DATA:  .dw 100, 200, 300
           ld  a, DATA+1
                  ^^^^^^ = r

So here the result ``r`` will be the address of ``DATA`` plus one, here 21.
Some x86 assemblers would give here ``22`` as the result ``r`` as ``DATA``
points to an array or machine words, but WLA isn't that smart (and some people
including me think this is the better solution).

Note that each CPU WLA supports contains opcodes that either generate an
absolute reference or a relative reference to the given label. For example, ::

    .org 20
    DATA:  ld  a, DATA   ; DATA becomes 20 (absolute)
           jr  DATA      ; DATA becomes -4 (relative)

Check out section 14 for the list of opcodes that generate relative references.

You can also use ``-``, ``--``, ``---``, ``+``, ``++``, ``+++``, ... as
un-named labels. Labels consisting of ``-`` are meant for reverse jumps and
labels consisting of ``+`` are meant for forward jumps. You can reuse un-named
labels as much as you wish inside your source code. Here's an example of this::

        dec e
        beq ++      ; jump -> ?
        dec e
        beq +       ; jump -> %
        ld d, 14
    --- ld a, 10    ; !
    --  ld b, c     ; #
    -   dec b       ; *
        jp nz, -    ; jump -> *
        dec c
        jp nz, --   ; jump -> #
        dec d
        jp nz, ---  ; jump -> !
        ld a, 20
    -   dec a       ; $
        jp nz, -    ; jump -> $
    +   halt        ; %
    ++  nop         ; ?

Note that ``__`` (that's two underline characters) serves also as a un-named
label. You can refer to this label from both directions. Use ``_b`` when
you are jumping backwards and ``_f`` when you are jumping forwards label ``__``.

Example::

       dec e
       jp z, _f     ; jump -> *
       dec e
    __ ldi a, (hl)  ; *
       dec e
       jp nz, _b    ; jump -> *

**CAVEAT! CAVEAT! CAVEAT!**

The following code doesn't work as it would if WLA would determine the
distance lexically (but in practice it's WLALINK that does all the
calculations and sees only the preprocessed output of WLA)::

    .macro dummy
    -  dec a        ; #
       jp nz, -     ; jump -> #
    .endm

       ...
    -  nop          ; *
       dummy
       dec e
       jp nz, -     ; i'd like to jump to *, but i'll end up jumping
                    ; to # as it's closest to me in the output WLA produces
                    ; for WLALINK (so it's better to use \@ with labels inside
                    ; a macro).

To make un-named labels inside a ``.MACRO`` isolated, and the previous example
to work, use the keyword ``ISOLATED`` ::

    .macro dummy isolated
    -  dec a        ; #
       jp nz, -     ; jump -> #
    .endm

The same issue exists with child labels. See ``.MACRO``'s documentation for
more details.

WLALINK will also generate ``_sizeof_[label]`` defines that measure the
distance between two consecutive labels. These labels have the same scope as
the labels they describe. Here is an example::

    Label1:
        .db 1, 2, 3, 4
    Label2:

In this case you'll get a definition ``_sizeof_Label1`` that will have value
``4``.

WLA will skip over any child labels when calculating ``_sizeof``. So, in this
example::

    Label1:
    .db 1, 2
    @child:
        .db 3, 4
    Label2:

The value of ``_sizeof_Label1`` will still have a value of ``4``.

NOTE: If your code is outside ``.SECTION`` s then an empty byte in the output
will mark the end of the ``_sizeof_[label]``. Example:

    .ORG $0000
    Label1:
    .db 1, 2, 3

    .ORG $0100
    Label2:
    .db 4, 5

Here ``_sizeof_Label1`` will be 3 as there is empty space between $0003 - $0100.


Number Types
------------

======== ===========
``1000`` decimal
``$100`` hexadecimal
``100h`` hexadecimal
``0x10`` hexadecimal
``%100`` binary
``0b10`` binary
``'x'``  character
======== ===========

Remember that if you use the suffix ``h`` to give a hexadecimal value,
and the value begins with an alphabet, you must place a zero in front of it
so WLA knows it's not a label (e.g., ``0ah`` instead of ``ah``).


Strings
-------

Strings begin with and end to ``"``. Note that no ``0`` is inserted to
indicate the termination of the string like in e.g., ANSI C. You'll have to do
it yourself. You can place quotation marks inside strings the way C
preprocessors accept them.

Here are some examples of strings::

    "Hello world!"
    "He said: \"Please, kiss me honey.\""


Substitution
------------

It's possible to substitute definition's name with its value inside a label.

Here's an example::

    .REPEAT 10 INDEX COUNT
    Label_{COUNT}:                      ; -> Label_0, Label_1, Label_2...
    .DW Label_{COUNT}
    .ENDR

Substitution supports minimal formatting for integers::

    .DEFINE COUNT = 10
    .DEFINE UNIT = 5
    Label_{%.4x{COUNT}}:                ; -> Label_000a
    Label_{%.3X{COUNT}}_{%.3X{UNIT}}:   ; -> Label_00A_005
    Label_{%.9d{COUNT}}:                ; -> Label_000000010
    Label_{%.3i{COUNT}}:                ; -> Label_010

The examples show all the formatting symbols currently supported.

The same substitution works for strings inside quotes when the quoted string is as follows::

    .db { "HELLO_{COUNT}" }             ; -> "HELLO_10"

Note that only WLA can do the substitution and it needs to know the value of the definition
at the time the substitution is done, i.e., the time a string containing a substitution is
parsed.

Also note that you can embed calculations into substitutions::

    .DEFINE COUNT = 1
    Label_{COUNT+1}:                    ; -> Label_2

It's also possible to substitute text inside .MACROs::

    .MACRO INST_ABCD
       abcd.b d1, \1
    .ENDM

    INST_ABCD "d2"                      ; -> abcd.b d1, d2
    INST_ABCD "d7"                      ; -> abcd.b d1, d7

Note that this kind of substitution only works for lines with assembly instructions inside
a .MACRO, it doesn't work e.g., for lines with directive calls.

    
Mnemonics
---------

You can give the operand size with the operand itself (and this is
highly recommended) in WLA 6502/65C02/65CE02/HUC6280/65816/6800/6801/6809::

    and #20.b
    and #20.w
    bit loop.b
    bit loop.w


Brackets?
---------

You can write ::

    LDI (HL), A

or  ::

    LDI [HL], A

as both mean the same thing in the syntax of most of the supported CPUs.
Yes, you could write ::

    LDI [HL), A

but that is not recommended.

Note that brackets have special meaning when dealing with a 65816/SPC-700
system so you can't use ::

    AND [$65]

instead of ::

    AND ($65)

as they mean different things.
