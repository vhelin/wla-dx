WLA DX's architectural overview
===============================

The two most important executables inside WLA DX are WLA (the assembler)
and WLALINK (the linker).


WLA
---

WLA has four separate phases:

1. ``phase_1.c``: ``phase_1()``:

   - The biggest data processor in WLA.
   - Includes the include files: every time this happens the file is read in,
     white space is removed, lines formatted, etc.
   - Macros are processed along with directives
   - All textual data, code, etc. are transformed into WLA's internal byte
     code that gets written into a tmp (TMP) file, and after this phase
     the assembler or the linker has no idea of target CPU's opcodes - all is
     just pure WLA byte code.
   - The first and the only pass that handles the assembly source files
     supplied by the user.
   - The parser in this pass starts from the first byte of the first source
     file, then moves forward parsing everything that it encounters, but
     when a macro is called, the parser jumps to the beginning of the macro,
     and continues parsing from there.

2. ``phase_2.c``: ``phase_2()``:

   - If the user has issued directives like ``.SDSCTAG``, here we generate the
     needed data and write that into TMP.
      
3. ``phase_3.c``: ``phase_3()``:

   - Here we read in TMP and do some sanity checks for the data, give labels
     addresses (if possible), generate internal structures for labels and
     sections.

4. ``phase_4.c``: ``phase_4()``:

   - Again we read in TMP.
   - Now we check that if there is a reference to a calculation, and that
     calculation has been succesfully calculated, then we can replace the
     reference with the result.
   - This phase writes out object and library files, i.e., transforms TMP
     to final output files (this write out could actually be ``pass_5``)...


WLALINK
-------

WLALINK is much simpler and more straight forward than WLA; WLALINK just
reads in all the objects and library files, places the sections along with
labels into the target memory map, solves pending calculations, calculates
checksums, and writes out the final ROM/PRG files.
``wlalink/main.c:main()`` should quite clearly display all the higher level
phases in the linking process.
