Functions
=========

WLA supports functions in addition to ``.MACRO`` s. Functions are different from
``.MACRO`` s as functions always return a value.


Built-in functions
------------------

The following built-in functions can be used where ever a number is expected ::

========== =======================================================================
asc()      Uses ``.ASCIITABLE`` to map the supplied value
defined()  Returns 1 (true) if the supplied definition exists, 0 (false) otherwise
exists()   Returns 1 (true) if the supplied file exists, 0 (false) otherwise
lobyte()   Returns the low byte, bits 0-7 (the same as preceding ``<``)
hibyte()   Returns the high byte, bits 8-15 (the same as preceding ``>``)
loword()   Returns the low word, bits 0-15
hiword()   Returns the high word, bits 16-31
bankbyte() Returns the bank byte, bits 16-23
bank()     Returns the bank (the same as preceding ``:``)
========== =======================================================================

Note! Use bankbyte() with WLA-65816 as on that platform the bank (+ base) bits
are 16-23. On other platforms bank() works better.


Examples of functions
---------------------

Here's an example about how these functions can be used ::

    .IF DEFINED(USE_DEBUG) && DEFINED(DEBUG_SHOW)
  
    LDX #loword(CPU_ADDR)           ; instead of (CPU_ADDR & $00FFFF)
    LDA #bankbyte(CPU_ADDR)         ; instead of :CPU_ADDR

    .ENDIF
