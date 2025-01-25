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

====================== ================================================================================
abs(exp)               Returns the positive version of the argument
acos(exp)              The same as ANSI C90 acos()
asc(exp)               Uses ``.ASCIITABLE`` to map the supplied value
asin(exp)              The same as ANSI C90 asin()
atan(exp)              The same as ANSI C90 atan()
atan2(exp)             The same as ANSI C90 atan2()
bank()                 Returns the current bank
bank(label)            Returns the bank of the supplied label (the same as preceding ``:``)
bankbyte()             Returns the current bank byte, bits 16-23
bankbyte(label)        Returns the bank byte, bits 16-23, of the supplied label
base()                 Returns current base
base(label)            Returns the base of the supplied label
ceil(exp)              The same as ANSI C90 ceil()
clamp(value, min, max) Clamps the value between min and max
cos(exp)               The same as ANSI C90 cos()
cosh(exp)              The same as ANSI C90 cosh()
defined(definition)    Returns 1 (true) if the supplied definition exists, 0 (false) otherwise
exists(path)           Returns 1 (true) if the supplied file exists, 0 (false) otherwise
floor(exp)             The same as ANSI C90 floor()
get(keyword1)
hibyte(exp)            Returns the high byte, bits 8-15 (the same as preceding ``>``)
hiword(exp)            Returns the high word, bits 16-31
is(keyword2)           Return 0 (false) or 1 (true), see below for "keyword2"
lobyte(exp)            Returns the low byte, bits 0-7 (the same as preceding ``<``)
log(exp)               The same as ANSI C90 log()
log10(exp)             The same as ANSI C90 log10()
loword(exp)            Returns the low word, bits 0-15
max(exp1, exp2)        Returns the bigger value
min(exp1, exp2)        Returns the smaller value
pow(base, power)       The same as ANSI C90 pow()
random(min, max)       Returns a pseudo random integer like ``.DBRND`` [min, max]
round(exp)             The same as ANSI C99 round()
sign(exp)              Return 0 if the supplied value is 0, -1 if negative and 1 if positive
sin(exp)               The same as ANSI C90 sin()
sinh(exp)              The same as ANSI C90 sinh()
slot()                 Returns the current slot
sqrt(exp)              Returns the square root of the supplied value
tan(exp)               The same as ANSI C90 tan()
tanh(exp)              The same as ANSI C90 tanh()
====================== ================================================================================

Note! Use bankbyte() with WLA-65816 as on that platform the bank (+ base) bits
are 16-23. On other platforms bank() works better.


Examples of functions
---------------------

Here's an example about how these functions can be used ::

    .IF defined(USE_DEBUG) && defined(DEBUG_SHOW) && min(VALUE_A, VALUE_B) > 10
  
    LDX #lobyte(playMusic)          ; instead of (playMusic & $FF)
    LDA #bank(playMusic)            ; instead of :playMusic
    .DB random(0, 10)               ; defines a byte with value 0-10

    .ENDIF

NOTE: random() needs immediate min and max values.

NOTE: If you use e.g., bank(label) inside an .IF then the current bank for the label
is given. If WLALINK e.g., changes the label's .SECTION's bank for some reason
(e.g., the .SECTION is relocated) then .IF is not calculated correctly.

NOTE: bank(), bankbyte(), base() and slot() without arguments use the information
the assembler knows, not the linker.


Keywords
--------

Here are the supported keyword1s for get() ::

    "section.aligment"              ; returns the current .SECTION's alignment (int)
    "section.name"                  ; returns the current .SECTION's name (string)
    "section.offset"                ; returns the current .SECTION's offset (int)
    "section.priority"              ; returns the current .SECTION's priority (int)
    "section.type"                  ; returns the current .SECTION's type (string),
                                    ;   possible strings being "FREE", "FORCE", "OVERWRITE",
                                    ;   "HEADER", "SEMIFREE", "ABSOLUTE", "SUPERFREE",
                                    ;   "SEMISUBFREE" and "SEMISUPERFREE"

Here are the supported keyword2s for is() ::

    "insidesection"                 ; is the parser at this point inside a .SECTION?
    
