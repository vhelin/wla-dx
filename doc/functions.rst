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

Note! If you use bank() with labels, it'll return the ROM bank in which the label
was defined in. If you give it a number (ROM address) it calculates in which ROM
bank the number (ROM address) is. WLA-65816 adds the .BASE to this value.

bankbyte() instead just gives you bits 16-23 (one byte) of a number.


Examples of functions
---------------------

Here's an example about how these functions can be used ::

    .IF DEFINED(USE_DEBUG) && DEFINED(DEBUG_SHOW)
  
    LDX #loword(CPU_ADDR)           ; instead of (CPU_ADDR & $00FFFF)
    LDA #bankbyte(CPU_ADDR)         ; instead of :CPU_ADDR

    .ENDIF
