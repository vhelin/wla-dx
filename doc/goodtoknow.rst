Good things to know about WLA
=============================

- Is 64 bytes too little for a string (file names, labels, definition labels,
  etc)? Check out ``MAX_NAME_LENGTH`` in ``defines.h``.
- WLA preprocessor doesn't expand macros and repetitions. Those are actually
  traversed in the assembling phase.
- WLA's source code is mainly a huge mess, but WLALINK is quite well
  structured and written. So beware!
- Do not write ``.E`` into your sources as WLA uses it internally to mark
  the end of a file.