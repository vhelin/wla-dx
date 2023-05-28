
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
