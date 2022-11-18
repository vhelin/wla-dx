Functions
=========

WLA supports functions in addition to ``.MACRO`` s. Functions are different from
``.MACRO`` s as functions always return a value.


User defined functions
----------------------

Use ``.FUNCTION`` to create your own functions.


Built-in functions
------------------

The following built-in functions can be used where ever a number is expected:

========== ================================================================================
abs()      Returns the positive version of the argument
acos()     The same as ANSI C90 acos()
asc()      Uses ``.ASCIITABLE`` to map the supplied value
asin()     The same as ANSI C90 asin()
atan()     The same as ANSI C90 atan()
atan2()    The same as ANSI C90 atan2()
bank()     Returns the bank (the same as preceding ``:``)
bankbyte() Returns the bank byte, bits 16-23
ceil()     The same as ANSI C90 ceil()
clamp()    Takes three arguments, value, min and max, clamps the value between min and max
cos()      The same as ANSI C90 cos()
cosh()     The same as ANSI C90 cosh()
defined()  Returns 1 (true) if the supplied definition exists, 0 (false) otherwise
exists()   Returns 1 (true) if the supplied file exists, 0 (false) otherwise
floor()    The same as ANSI C90 floor()
hibyte()   Returns the high byte, bits 8-15 (the same as preceding ``>``)
hiword()   Returns the high word, bits 16-31
lobyte()   Returns the low byte, bits 0-7 (the same as preceding ``<``)
log()      The same as ANSI C90 log()
log10()    The same as ANSI C90 log10()
loword()   Returns the low word, bits 0-15
max()      Takes two arguments, a and b, returns the bigger value
min()      Takes two arguments, a and b, returns the smaller value
pow()      The same as ANSI C90 pow()
random()   Takes two arguments, min and max, returns a pseudo random integer like ``.DBRND``
round()    The same as ANSI C99 round()
sign()     Return 0 if the supplied value is 0, -1 if negative and 1 if positive
sin()      The same as ANSI C90 sin()
sinh()     The same as ANSI C90 sinh()
sqrt()     Returns the square root of the supplied value
tan()      The same as ANSI C90 tan()
tanh()     The same as ANSI C90 tanh()
========== ================================================================================

Note! Use bankbyte() with WLA-65816 as on that platform the bank (+ base) bits
are 16-23. On other platforms bank() works better.


Examples of functions
---------------------

Here's an example about how these functions can be used ::

    .IF defined(USE_DEBUG) && defined(DEBUG_SHOW) && min(VALUE_A, VALUE_B) > 10
  
    LDX #loword(CPU_ADDR)           ; instead of (CPU_ADDR & $00FFFF)
    LDA #bankbyte(CPU_ADDR)         ; instead of :CPU_ADDR
    .DB random(0, 10)               ; defines a byte with value 0-10

    .ENDIF

NOTE: random() needs immediate min and max values.

