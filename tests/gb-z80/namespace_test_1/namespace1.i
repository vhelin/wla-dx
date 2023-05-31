
  .macro MacroNamespace1
    .dw labelNamespace2
  .endm

  .macro "someMacro" args value
    .ifndef value
        .print "<undefined>"
    .endif

    .if \?1 == ARG_STRING
        .print "<string> \"", \1, "\""
    .elif \?1 == ARG_LABEL
        .print "<label> " \1
    .elif \?1 == ARG_NUMBER
        .print DEC value, " ($", HEX \1, ")"
        .db value
        .db \1
    .elif \?1 == ARG_IMMEDIATE
        .print "<immediate>"
    .elif \?1 == ARG_PENDING_CALCULATION
        .print "<pending calculation>"
        .db value
        .db \1
    .else
        .print "<unknown>"
        .fail
    .endif
    .print "\n"
  .endm

.macro "myMacro" args someOptionalArg
    .ifndef someOptionalArg
        .define someOptionalArg 0
    .endif
.endm

    .define VALUE_6 = 6

.macro "myMacro2" args myArg
    ld a, myArg
.endm

    myMacro2 1
    myMacro2 2
    
.macro "macroA" args someArg
    .print "macroA someArg = ", someArg, "\n"
.endm

.macro "macroB" args someArg
    .print "macroB someArg = ", someArg, "\n"
    macroA ; call macroA without an argument
.endm

.macro "caller"
    macroB 123
.endm

    caller
