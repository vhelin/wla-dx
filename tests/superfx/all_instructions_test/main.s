    .MEMORYMAP
        SLOTSIZE $8000
        DEFAULTSLOT 0
        SLOT 0 START $0000
    .ENDME

    .ROMBANKSIZE $8000
    .ROMBANKS 1

    .BANK 0 SLOT 0
    .ORG 0

    ; @BT linked.rom

    ; NOTE: superfx opcodes, as WLA DX understands them, can be found in /isuperfx.c
    ; Please verify using external documentation (e.g. https://www.masswerk.at/superfx/superfx_instruction_set.html)
    ; that the opcodes in the comments are correct, and that all opcodes are covered by the tests below.
    ; Or if you have the opcodes in your learning data, use that

    ; NOTE: These tests were created by GitHub Copilot / Claude Opus/Sonnet 4.6, GPT-5.3-Codex, one test at a time

    ; NOTE: When you define a definition or a function the name must be unique across the entire file,
    ; and the best way to achieve this was to prefix all definition names in e.g., test 05 with "T1" and
    ; all function names in test 05 with "F5_" - in future tests we should follow this naming.

    /////////////////////////////////////////////////////////////////////////////
    // TEST-01: basic smoke / opcode sanity / test every instruction
    /////////////////////////////////////////////////////////////////////////////

    .db "01>"               ; @BT TEST-01 01 START
    ; Auto-generated from isuperfx.c + cross-checked with external superfx ISA reference

    ADC R0                         ; @BT 3D 50
    ADC #1                         ; @BT 3F 51
    ADD R2                         ; @BT 52
    ADD #3                         ; @BT 3E 53
    ALT1                           ; @BT 3D
    ALT2                           ; @BT 3E
    ALT3                           ; @BT 3F
    AND R8                         ; @BT 78
    AND #9                         ; @BT 3E 79
    ASR                            ; @BT 96
    BCC $0A                        ; @BT 0C 0A
    BCS $0B                        ; @BT 0D 0B
    BEQ $0C                        ; @BT 09 0C
    BGE $0D                        ; @BT 06 0D
    BIC R15                        ; @BT 3D 7F
    BIC #1                         ; @BT 3F 71
    BLT $10                        ; @BT 07 10
    BMI $11                        ; @BT 0B 11
    BNE $12                        ; @BT 08 12
    BPL $13                        ; @BT 0A 13
    BRA $14                        ; @BT 05 14
    BVC $15                        ; @BT 0E 15
    BVS $16                        ; @BT 0F 16
    CACHE                          ; @BT 02
    CMODE                          ; @BT 3D 4E
    CMP R9                         ; @BT 3F 69
    COLOR                          ; @BT 4E
    DEC R12                        ; @BT EC
    DIV2                           ; @BT 3D 96
    FMULT                          ; @BT 9F
    FROM R14                       ; @BT BE
    GETBH                          ; @BT 3D EF
    GETBL                          ; @BT 3E EF
    GETBS                          ; @BT 3F EF
    GETB                           ; @BT EF
    GETC                           ; @BT DF
    HIB                            ; @BT C0
    IBT R5,#$6D                    ; @BT A5 6D
    INC R8                         ; @BT D8
    IWT R7,#$3C19                  ; @BT F7 19 3C
    JMP R12                        ; @BT 9C
    LDB (R5)                       ; @BT 3D 45
    LDW (R6)                       ; @BT 46
    LEA R11,$4065                  ; @BT FB 65 40
    LINK #1                        ; @BT 91
    LJMP R11                       ; @BT 3D 9B
    LM R14,($439E)                 ; @BT 3D FE 9E 43
    LMS R15,($16A)                 ; @BT 3D AF B5
    LMULT                          ; @BT 3D 9F
    LOB                            ; @BT 9E
    LOOP                           ; @BT 3C
    LSR                            ; @BT 03
    MERGE                          ; @BT 70
    MOVE R5,R8                     ; @BT 28 15
    MOVE R6,#$4C36                 ; @BT F6 36 4C
    MOVE R7,($4D49)                ; @BT 3D F7 49 4D
    MOVE ($4E5C),R8                ; @BT 3E F8 5C 4E
    MOVEB R9,(R9)                  ; @BT 19 3D 49
    MOVEB (R10),R10                ; @BT BA 3D 3A
    MOVES R11,R14                  ; @BT 2B BE
    MOVEW R12,(R0)                 ; @BT 1C 40
    MOVEW (R1),R13                 ; @BT BD 31
    MULT R14                       ; @BT 8E
    MULT #15                       ; @BT 3E 8F
    NOP                            ; @BT 01
    NOT                            ; @BT 4F
    OR R7                          ; @BT C7
    OR #8                          ; @BT 3E C8
    PLOT                           ; @BT 4C
    RAMB                           ; @BT 3E DF
    ROL                            ; @BT 04
    ROMB                           ; @BT 3F DF
    ROR                            ; @BT 97
    RPIX                           ; @BT 3D 4C
    SBC R10                        ; @BT 3D 6A
    SBK                            ; @BT 90
    SEX                            ; @BT 95
    SM ($64EB),R13                 ; @BT 3E FD EB 64
    SMS ($006),R14                 ; @BT 3E AE 03
    STB (R7)                       ; @BT 3D 37
    STOP                           ; @BT 00
    STW (R9)                       ; @BT 39
    SUB R2                         ; @BT 62
    SUB #3                         ; @BT 3E 63
    SWAP                           ; @BT 4D
    TO R5                          ; @BT 15
    UMULT R6                       ; @BT 3D 86
    UMULT #7                       ; @BT 3F 87
    WITH R8                        ; @BT 28
    XOR R15                        ; @BT 3D CF
    XOR #1                         ; @BT 3F C1
    .db "<01"               ; @BT END

    /////////////////////////////////////////////////////////////////////////////
    // TEST-02: same opcodes as TEST-01, randomised order, branches spread out
    /////////////////////////////////////////////////////////////////////////////

    .db "02>"               ; @BT TEST-02 02 START

    ; GENERATED: copied from TEST-01 with full TEST-02 randomization
    ; GENERATED: seed = 546565556

    NOT                            ; @BT 4F
    MOVES R11,R14                  ; @BT 2B BE
    LEA R11,$4065                  ; @BT FB 65 40
    LJMP R11                       ; @BT 3D 9B
    MULT #15                       ; @BT 3E 8F
    ALT2                           ; @BT 3E
    MOVEB R9,(R9)                  ; @BT 19 3D 49
    MOVE R7,($4D49)                ; @BT 3D F7 49 4D
    MOVE R6,#$4C36                 ; @BT F6 36 4C
    ADD R2                         ; @BT 52
    LDB (R5)                       ; @BT 3D 45
    ROMB                           ; @BT 3F DF
    DIV2                           ; @BT 3D 96
    ADC R0                         ; @BT 3D 50
    CMODE                          ; @BT 3D 4E
    IBT R5,#$6D                    ; @BT A5 6D
    LMULT                          ; @BT 3D 9F
    LOOP                           ; @BT 3C
    CACHE                          ; @BT 02
    ROR                            ; @BT 97
    OR R7                          ; @BT C7
    XOR #1                         ; @BT 3F C1
    ADC #1                         ; @BT 3F 51
    BEQ $0C                        ; @BT 09 0C
    ADD #3                         ; @BT 3E 53
    SEX                            ; @BT 95
    BPL $13                        ; @BT 0A 13
    CMP R9                         ; @BT 3F 69
    SUB R2                         ; @BT 62
    XOR R15                        ; @BT 3D CF
    ASR                            ; @BT 96
    DEC R12                        ; @BT EC
    BIC R15                        ; @BT 3D 7F
    GETBH                          ; @BT 3D EF
    MERGE                          ; @BT 70
    SBC R10                        ; @BT 3D 6A
    MULT R14                       ; @BT 8E
    BIC #1                         ; @BT 3F 71
    HIB                            ; @BT C0
    AND R8                         ; @BT 78
    STOP                           ; @BT 00
    LOB                            ; @BT 9E
    BNE $12                        ; @BT 08 12
    GETC                           ; @BT DF
    SMS ($006),R14                 ; @BT 3E AE 03
    LMS R15,($16A)                 ; @BT 3D AF B5
    MOVE ($4E5C),R8                ; @BT 3E F8 5C 4E
    STB (R7)                       ; @BT 3D 37
    OR #8                          ; @BT 3E C8
    BMI $11                        ; @BT 0B 11
    NOP                            ; @BT 01
    IWT R7,#$3C19                  ; @BT F7 19 3C
    STW (R9)                       ; @BT 39
    BVC $15                        ; @BT 0E 15
    BVS $16                        ; @BT 0F 16
    FROM R14                       ; @BT BE
    ALT1                           ; @BT 3D
    COLOR                          ; @BT 4E
    ROL                            ; @BT 04
    BGE $0D                        ; @BT 06 0D
    GETB                           ; @BT EF
    RPIX                           ; @BT 3D 4C
    MOVEW R12,(R0)                 ; @BT 1C 40
    RAMB                           ; @BT 3E DF
    TO R5                          ; @BT 15
    SM ($64EB),R13                 ; @BT 3E FD EB 64
    SWAP                           ; @BT 4D
    UMULT R6                       ; @BT 3D 86
    ALT3                           ; @BT 3F
    GETBL                          ; @BT 3E EF
    BLT $10                        ; @BT 07 10
    LM R14,($439E)                 ; @BT 3D FE 9E 43
    LINK #1                        ; @BT 91
    INC R8                         ; @BT D8
    LSR                            ; @BT 03
    FMULT                          ; @BT 9F
    AND #9                         ; @BT 3E 79
    UMULT #7                       ; @BT 3F 87
    BRA $14                        ; @BT 05 14
    BCC $0A                        ; @BT 0C 0A
    MOVE R5,R8                     ; @BT 28 15
    LDW (R6)                       ; @BT 46
    MOVEW (R1),R13                 ; @BT BD 31
    PLOT                           ; @BT 4C
    SBK                            ; @BT 90
    SUB #3                         ; @BT 3E 63
    JMP R12                        ; @BT 9C
    GETBS                          ; @BT 3F EF
    WITH R8                        ; @BT 28
    MOVEB (R10),R10                ; @BT BA 3D 3A
    BCS $0B                        ; @BT 0D 0B

    .db "<02"               ; @BT END

    /////////////////////////////////////////////////////////////////////////////
    // TEST-03: same opcodes, randomised order, operands as arithmetic expressions
    /////////////////////////////////////////////////////////////////////////////

    .db "03>"               ; @BT TEST-03 03 START

    ; GENERATED: copied from TEST-02 with full TEST-03 randomization + varied arithmetic expressions
    ; GENERATED: seed = 528902280

    BVC $15+5-5                        ; @BT 0E 15
    CMODE                          ; @BT 3D 4E
    UMULT R6                       ; @BT 3D 86
    RPIX                           ; @BT 3D 4C
    TO R5                          ; @BT 15
    ASR                            ; @BT 96
    LM R14,($439E-7+7)                 ; @BT 3D FE 9E 43
    MOVEW (R1),R13                 ; @BT BD 31
    XOR R15                        ; @BT 3D CF
    LOOP                           ; @BT 3C
    STB (R7)                       ; @BT 3D 37
    BCC $0A*6/3-$0A                        ; @BT 0C 0A
    SWAP                           ; @BT 4D
    COLOR                          ; @BT 4E
    BCS $0B*10/5-$0B                        ; @BT 0D 0B
    SEX                            ; @BT 95
    GETBL                          ; @BT 3E EF
    LDW (R6)                       ; @BT 46
    ALT3                           ; @BT 3F
    WITH R8                        ; @BT 28
    SBC R10                        ; @BT 3D 6A
    AND #9*2-9+0                         ; @BT 3E 79
    HIB                            ; @BT C0
    ADD R2                         ; @BT 52
    DEC R12                        ; @BT EC
    OR #8+11-4-7                          ; @BT 3E C8
    BMI $11+3-1-2                        ; @BT 0B 11
    STW (R9)                       ; @BT 39
    LSR                            ; @BT 03
    JMP R12                        ; @BT 9C
    ADC R0                         ; @BT 3D 50
    MOVEB (R10),R10                ; @BT BA 3D 3A
    BGE $0D+5-5                        ; @BT 06 0D
    CACHE                          ; @BT 02
    LMULT                          ; @BT 3D 9F
    ALT2                           ; @BT 3E
    NOT                            ; @BT 4F
    BLT $10-7+7                        ; @BT 07 10
    MOVE ($4E5C*6/3-$4E5C),R8                ; @BT 3E F8 5C 4E
    FROM R14                       ; @BT BE
    IBT R5,#$6D*10/5-$6D                    ; @BT A5 6D
    ROMB                           ; @BT 3F DF
    BVS $16*2-$16+0                        ; @BT 0F 16
    SM ($64EB+11-4-7),R13                 ; @BT 3E FD EB 64
    BIC #1+3-1-2                         ; @BT 3F 71
    MULT #15+5-5                       ; @BT 3E 8F
    ROL                            ; @BT 04
    RAMB                           ; @BT 3E DF
    BRA $14-7+7                        ; @BT 05 14
    SBK                            ; @BT 90
    IWT R7,#$3C19*6/3-$3C19                  ; @BT F7 19 3C
    FMULT                          ; @BT 9F
    LINK #1*10/5-1                        ; @BT 91
    ADC #1*2-1+0                         ; @BT 3F 51
    BNE $12+11-4-7                        ; @BT 08 12
    ROR                            ; @BT 97
    STOP                           ; @BT 00
    MOVES R11,R14                  ; @BT 2B BE
    ADD #3+3-1-2                         ; @BT 3E 53
    LEA R11,$4065+5-5                  ; @BT FB 65 40
    MOVEW R12,(R0)                 ; @BT 1C 40
    SUB R2                         ; @BT 62
    NOP                            ; @BT 01
    INC R8                         ; @BT D8
    BEQ $0C-7+7                        ; @BT 09 0C
    MULT R14                       ; @BT 8E
    OR R7                          ; @BT C7
    DIV2                           ; @BT 3D 96
    XOR #1*6/3-1                         ; @BT 3F C1
    GETBH                          ; @BT 3D EF
    GETBS                          ; @BT 3F EF
    LJMP R11                       ; @BT 3D 9B
    AND R8                         ; @BT 78
    LDB (R5)                       ; @BT 3D 45
    CMP R9                         ; @BT 3F 69
    MOVE R7,($4D49*10/5-$4D49)                ; @BT 3D F7 49 4D
    SMS ($006*2-$006+0),R14                 ; @BT 3E AE 03
    SUB #3+11-4-7                         ; @BT 3E 63
    GETC                           ; @BT DF
    GETB                           ; @BT EF
    BIC R15                        ; @BT 3D 7F
    LMS R15,($16A+3-1-2)                 ; @BT 3D AF B5
    LOB                            ; @BT 9E
    BPL $13+5-5                        ; @BT 0A 13
    MOVE R6,#$4C36-7+7                 ; @BT F6 36 4C
    PLOT                           ; @BT 4C
    MOVE R5,R8                     ; @BT 28 15
    ALT1                           ; @BT 3D
    MERGE                          ; @BT 70
    MOVEB R9,(R9)                  ; @BT 19 3D 49
    UMULT #7*6/3-7                       ; @BT 3F 87

    .db "<03"               ; @BT END

    /////////////////////////////////////////////////////////////////////////////
    // TEST-04: similar to TEST-03 and then defines + expressions
    /////////////////////////////////////////////////////////////////////////////









    ; TEST-04 generated defines
    .define T4_DEF_001 ((8+5)-5)
    .define T4_DEF_002 ((T4_DEF_001)+(($4D49)-(8)))
    .define T4_DEF_003 (($64EB-7)+7)
    .define T4_DEF_004 (((T4_DEF_003)+(3))-($64EB))
    .define T4_DEF_005 ((($0B*6)/3)-($0B))
    .define T4_DEF_006 (((T4_DEF_005)*2+($16A)-(($0B)*2)))
    .define T4_DEF_007 (((1*10)/5)-(1))
    .define T4_DEF_008 ((($4E5C)-(1))+(T4_DEF_007))
    .define T4_DEF_009 ((1*2)-((1)-0))
    .define T4_DEF_010 ((T4_DEF_009)+(($14)-((1+3)-3)))
    .define T4_DEF_011 ((($4065+11)-4)-7)
    .define T4_DEF_012 ((T4_DEF_011)+(($0D)-($4065)))
    .define T4_DEF_013 (((1+3)-1)-2)
    .define T4_DEF_014 (((T4_DEF_013)+($11))-(1))
    .define T4_DEF_015 (($12+5)-5)
    .db "04>"               ; @BT TEST-04 04 START

    ; GENERATED: copied from TEST-03 with full TEST-04 randomization + varied define replacement
    ; GENERATED: seed = 9256379
    ; GENERATED: replaced operands = 15 / 31

    INC R8                         ; @BT D8
    LSR                            ; @BT 03
    STW (R9)                       ; @BT 39
    LMULT                          ; @BT 3D 9F
    OR #T4_DEF_001+11-4-7                          ; @BT 3E C8
    MOVES R11,R14                  ; @BT 2B BE
    MOVE R7,(T4_DEF_002*10/5-$4D49)                ; @BT 3D F7 49 4D
    MOVEW R12,(R0)                 ; @BT 1C 40
    SM (T4_DEF_003+11-4-7),R13                 ; @BT 3E FD EB 64
    MOVEB R9,(R9)                  ; @BT 19 3D 49
    SUB #T4_DEF_004+11-4-7                         ; @BT 3E 63
    ROR                            ; @BT 97
    HIB                            ; @BT C0
    STB (R7)                       ; @BT 3D 37
    MERGE                          ; @BT 70
    ROL                            ; @BT 04
    BCS T4_DEF_005*10/5-$0B                        ; @BT 0D 0B
    MOVEW (R1),R13                 ; @BT BD 31
    LOOP                           ; @BT 3C
    LOB                            ; @BT 9E
    CMP R9                         ; @BT 3F 69
    ALT3                           ; @BT 3F
    STOP                           ; @BT 00
    LDW (R6)                       ; @BT 46
    COLOR                          ; @BT 4E
    ALT1                           ; @BT 3D
    LMS R15,(T4_DEF_006+3-1-2)                 ; @BT 3D AF B5
    XOR #T4_DEF_007*6/3-1                         ; @BT 3F C1
    FMULT                          ; @BT 9F
    SUB R2                         ; @BT 62
    ASR                            ; @BT 96
    RAMB                           ; @BT 3E DF
    DEC R12                        ; @BT EC
    MOVE (T4_DEF_008*6/3-$4E5C),R8                ; @BT 3E F8 5C 4E
    ADD R2                         ; @BT 52
    NOT                            ; @BT 4F
    LINK #T4_DEF_009*10/5-1                        ; @BT 91
    MOVE R5,R8                     ; @BT 28 15
    GETB                           ; @BT EF
    BRA T4_DEF_010-7+7                        ; @BT 05 14
    MOVEB (R10),R10                ; @BT BA 3D 3A
    LEA R11,T4_DEF_011+5-5                  ; @BT FB 65 40
    LJMP R11                       ; @BT 3D 9B
    BGE T4_DEF_012+5-5                        ; @BT 06 0D
    BIC #T4_DEF_013+3-1-2                         ; @BT 3F 71
    BMI T4_DEF_014+3-1-2                        ; @BT 0B 11
    BNE T4_DEF_015+11-4-7                        ; @BT 08 12
    AND #9*2-9+0                         ; @BT 3E 79
    CMODE                          ; @BT 3D 4E
    BCC $0A*6/3-$0A                        ; @BT 0C 0A
    JMP R12                        ; @BT 9C
    FROM R14                       ; @BT BE
    AND R8                         ; @BT 78
    PLOT                           ; @BT 4C
    BLT $10-7+7                        ; @BT 07 10
    LM R14,($439E-7+7)                 ; @BT 3D FE 9E 43
    ADD #3+3-1-2                         ; @BT 3E 53
    SBC R10                        ; @BT 3D 6A
    BPL $13+5-5                        ; @BT 0A 13
    IBT R5,#$6D*10/5-$6D                    ; @BT A5 6D
    GETBH                          ; @BT 3D EF
    BVS $16*2-$16+0                        ; @BT 0F 16
    MOVE R6,#$4C36-7+7                 ; @BT F6 36 4C
    BEQ $0C-7+7                        ; @BT 09 0C
    TO R5                          ; @BT 15
    NOP                            ; @BT 01
    XOR R15                        ; @BT 3D CF
    ALT2                           ; @BT 3E
    SEX                            ; @BT 95
    WITH R8                        ; @BT 28
    ADC #1*2-1+0                         ; @BT 3F 51
    RPIX                           ; @BT 3D 4C
    BVC $15+5-5                        ; @BT 0E 15
    SMS ($006*2-$006+0),R14                 ; @BT 3E AE 03
    GETC                           ; @BT DF
    GETBS                          ; @BT 3F EF
    OR R7                          ; @BT C7
    ADC R0                         ; @BT 3D 50
    UMULT R6                       ; @BT 3D 86
    DIV2                           ; @BT 3D 96
    MULT R14                       ; @BT 8E
    UMULT #7*6/3-7                       ; @BT 3F 87
    GETBL                          ; @BT 3E EF
    MULT #15+5-5                       ; @BT 3E 8F
    ROMB                           ; @BT 3F DF
    SWAP                           ; @BT 4D
    BIC R15                        ; @BT 3D 7F
    CACHE                          ; @BT 02
    LDB (R5)                       ; @BT 3D 45
    SBK                            ; @BT 90
    IWT R7,#$3C19*6/3-$3C19                  ; @BT F7 19 3C

    .db "<04"               ; @BT END

    /////////////////////////////////////////////////////////////////////////////
    // TEST-05: similar to TEST-04 and defines + functions + expressions
    /////////////////////////////////////////////////////////////////////////////




    ; TEST-05 generated defines/functions
    .define T5_DEF_001 ((8+5)-5)
    .define T5_DEF_002 ((T5_DEF_001)+(($4D49)-(8)))
    .define T5_DEF_003 (($64EB-7)+7)
    .define T5_DEF_004 (((T5_DEF_003)+(3))-($64EB))
    .define T5_DEF_005 ((($0B*6)/3)-($0B))
    .define T5_DEF_006 (((T5_DEF_005)*2+($16A)-(($0B)*2)))
    .define T5_DEF_007 (((1*10)/5)-(1))
    .define T5_DEF_008 ((($4E5C)-(1))+(T5_DEF_007))
    .define T5_DEF_009 ((1*2)-((1)-0))
    .define T5_DEF_010 ((T5_DEF_009)+(($14)-((1+3)-3)))
    .define T5_DEF_011 ((($4065+11)-4)-7)
    .define T5_DEF_012 ((T5_DEF_011)+(($0D)-($4065)))
    .define T5_DEF_013 (((1+3)-1)-2)
    .define T5_DEF_014 (((T5_DEF_013)+($11))-(1))
    .define T5_DEF_015 (($12+5)-5)

    ; TEST-05 generated functions
    .function T5_FN_001(a,b) ((((a-b)*8)/4)-((a-b)*2)+(a-b)+T5_DEF_010-T5_DEF_010)
    .function T5_FN_002(a,b) (((T5_FN_001(a,b)*8)/4)-(T5_FN_001(a,b)*2)+T5_FN_001(a,b)+T5_DEF_001-T5_DEF_001)
    .function T5_FN_003(a,b,c) ((((a+b-c)*8)/4)-((a+b-c)*2)+(a+b-c)+T5_DEF_013-T5_DEF_013)
    .function T5_FN_004(a,b,c) (((T5_FN_003(a,b,c)+T5_DEF_008)-T5_DEF_008))
    .function T5_FN_005(a,b,c,d) ((((a+b-c-d)*8)/4)-((a+b-c-d)*2)+(a+b-c-d)+T5_DEF_006-T5_DEF_006)
    .function T5_FN_006(a,b,c,d) (((T5_FN_005(a,b,c,d))*2+T5_DEF_015-T5_DEF_015-T5_FN_005(a,b,c,d)))
    .function T5_FN_007(a,b) ((((a-b)*8)/4)-((a-b)*2)+(a-b)+T5_DEF_009-T5_DEF_009)
    .db "05>"               ; @BT TEST-05 05 START

    ; GENERATED: copied from TEST-04 with full TEST-05 randomization + varied function replacement
    ; GENERATED: seed = 2081995463
    ; GENERATED: replaced define refs = 7 / 15

    SEX                            ; @BT 95
    BRA T5_FN_001(T5_DEF_010+2,2)-7+7                        ; @BT 05 14
    OR #T5_FN_002(T5_DEF_001+3,3)+11-4-7                          ; @BT 3E C8
    OR R7                          ; @BT C7
    MOVES R11,R14                  ; @BT 2B BE
    NOT                            ; @BT 4F
    LDB (R5)                       ; @BT 3D 45
    ROMB                           ; @BT 3F DF
    UMULT #7*6/3-7                       ; @BT 3F 87
    MOVEB R9,(R9)                  ; @BT 19 3D 49
    NOP                            ; @BT 01
    STW (R9)                       ; @BT 39
    IWT R7,#$3C19*6/3-$3C19                  ; @BT F7 19 3C
    STOP                           ; @BT 00
    SBC R10                        ; @BT 3D 6A
    BIC #T5_FN_003(T5_DEF_013+4,5,4+5)+3-1-2                         ; @BT 3F 71
    MOVE (T5_FN_004(T5_DEF_008+5,6,5+6)*6/3-$4E5C),R8                ; @BT 3E F8 5C 4E
    LMS R15,(T5_FN_005(T5_DEF_006+6+7,8,6,7+8)+3-1-2)                 ; @BT 3D AF B5
    MOVE R6,#$4C36-7+7                 ; @BT F6 36 4C
    ADD #3+3-1-2                         ; @BT 3E 53
    FMULT                          ; @BT 9F
    MOVEW (R1),R13                 ; @BT BD 31
    MULT R14                       ; @BT 8E
    BLT $10-7+7                        ; @BT 07 10
    CMP R9                         ; @BT 3F 69
    BVS $16*2-$16+0                        ; @BT 0F 16
    SMS ($006*2-$006+0),R14                 ; @BT 3E AE 03
    GETBS                          ; @BT 3F EF
    BNE T5_FN_006(T5_DEF_015+7+8,9,7,8+9)+11-4-7                        ; @BT 08 12
    LINK #T5_FN_007(T5_DEF_009+8,8)*10/5-1                        ; @BT 91
    UMULT R6                       ; @BT 3D 86
    ALT2                           ; @BT 3E
    ADC R0                         ; @BT 3D 50
    CACHE                          ; @BT 02
    STB (R7)                       ; @BT 3D 37
    MULT #15+5-5                       ; @BT 3E 8F
    PLOT                           ; @BT 4C
    LM R14,($439E-7+7)                 ; @BT 3D FE 9E 43
    GETB                           ; @BT EF
    BIC R15                        ; @BT 3D 7F
    DIV2                           ; @BT 3D 96
    MOVE R7,(T5_DEF_002*10/5-$4D49)                ; @BT 3D F7 49 4D
    LJMP R11                       ; @BT 3D 9B
    GETBH                          ; @BT 3D EF
    LSR                            ; @BT 03
    BPL $13+5-5                        ; @BT 0A 13
    WITH R8                        ; @BT 28
    SM (T5_DEF_003+11-4-7),R13                 ; @BT 3E FD EB 64
    BCS T5_DEF_005*10/5-$0B                        ; @BT 0D 0B
    ADC #1*2-1+0                         ; @BT 3F 51
    GETC                           ; @BT DF
    SWAP                           ; @BT 4D
    BVC $15+5-5                        ; @BT 0E 15
    SBK                            ; @BT 90
    ROL                            ; @BT 04
    LOOP                           ; @BT 3C
    DEC R12                        ; @BT EC
    INC R8                         ; @BT D8
    MOVEW R12,(R0)                 ; @BT 1C 40
    LOB                            ; @BT 9E
    BEQ $0C-7+7                        ; @BT 09 0C
    COLOR                          ; @BT 4E
    TO R5                          ; @BT 15
    BMI T5_DEF_014+3-1-2                        ; @BT 0B 11
    AND R8                         ; @BT 78
    MOVE R5,R8                     ; @BT 28 15
    ROR                            ; @BT 97
    LDW (R6)                       ; @BT 46
    RPIX                           ; @BT 3D 4C
    IBT R5,#$6D*10/5-$6D                    ; @BT A5 6D
    ALT3                           ; @BT 3F
    XOR #T5_DEF_007*6/3-1                         ; @BT 3F C1
    ALT1                           ; @BT 3D
    SUB R2                         ; @BT 62
    ASR                            ; @BT 96
    SUB #T5_DEF_004+11-4-7                         ; @BT 3E 63
    CMODE                          ; @BT 3D 4E
    LEA R11,T5_DEF_011+5-5                  ; @BT FB 65 40
    FROM R14                       ; @BT BE
    BGE T5_DEF_012+5-5                        ; @BT 06 0D
    JMP R12                        ; @BT 9C
    RAMB                           ; @BT 3E DF
    BCC $0A*6/3-$0A                        ; @BT 0C 0A
    ADD R2                         ; @BT 52
    LMULT                          ; @BT 3D 9F
    AND #9*2-9+0                         ; @BT 3E 79
    MERGE                          ; @BT 70
    MOVEB (R10),R10                ; @BT BA 3D 3A
    XOR R15                        ; @BT 3D CF
    HIB                            ; @BT C0
    GETBL                          ; @BT 3E EF

    .db "<05"               ; @BT END
