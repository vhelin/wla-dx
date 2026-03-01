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

    ; NOTE: 8080 opcodes, as WLA DX understands them, can be found in /i8080.c
    ; Please verify using external documentation (e.g. https://www.masswerk.at/8080/8080_instruction_set.html)
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
    ; Auto-generated from i8080.c + cross-checked with external 8080 ISA reference

    ADD B                    ; @BT 80
    ADD C                    ; @BT 81
    ADD D                    ; @BT 82
    ADD E                    ; @BT 83
    ADD H                    ; @BT 84
    ADD L                    ; @BT 85
    ADD M                    ; @BT 86
    ADD A                    ; @BT 87
    ADC B                    ; @BT 88
    ADC C                    ; @BT 89
    ADC D                    ; @BT 8A
    ADC E                    ; @BT 8B
    ADC H                    ; @BT 8C
    ADC L                    ; @BT 8D
    ADC M                    ; @BT 8E
    ADC A                    ; @BT 8F
    ANA B                    ; @BT A0
    ANA C                    ; @BT A1
    ANA D                    ; @BT A2
    ANA E                    ; @BT A3
    ANA H                    ; @BT A4
    ANA L                    ; @BT A5
    ANA M                    ; @BT A6
    ANA A                    ; @BT A7
    ADI $28                  ; @BT C6 28
    ACI $3F                  ; @BT CE 3F
    ANI $56                  ; @BT E6 56
    CMA                      ; @BT 2F
    CMC                      ; @BT 3F
    CMP B                    ; @BT B8
    CMP C                    ; @BT B9
    CMP D                    ; @BT BA
    CMP E                    ; @BT BB
    CMP H                    ; @BT BC
    CMP L                    ; @BT BD
    CMP M                    ; @BT BE
    CMP A                    ; @BT BF
    CNZ $1345                ; @BT C4 45 13
    CNC $1456                ; @BT D4 56 14
    CPO $1567                ; @BT E4 67 15
    CPE $1678                ; @BT EC 78 16
    CPI $6D                  ; @BT FE 6D
    CALL $1789               ; @BT CD 89 17
    CC $189A                 ; @BT DC 9A 18
    CZ $19AB                 ; @BT CC AB 19
    CP $1ABC                 ; @BT F4 BC 1A
    CM $1BCD                 ; @BT FC CD 1B
    DAA                      ; @BT 27
    DAD B                    ; @BT 09
    DAD D                    ; @BT 19
    DAD H                    ; @BT 29
    DAD SP                   ; @BT 39
    DCR A                    ; @BT 3D
    DCR B                    ; @BT 05
    DCR C                    ; @BT 0D
    DCR D                    ; @BT 15
    DCR E                    ; @BT 1D
    DCR H                    ; @BT 25
    DCR L                    ; @BT 2D
    DCR M                    ; @BT 35
    DCX B                    ; @BT 0B
    DCX D                    ; @BT 1B
    DCX H                    ; @BT 2B
    DCX SP                   ; @BT 3B
    DI                       ; @BT F3
    EI                       ; @BT FB
    HLT                      ; @BT 76
    INR A                    ; @BT 3C
    INR B                    ; @BT 04
    INR C                    ; @BT 0C
    INR D                    ; @BT 14
    INR E                    ; @BT 1C
    INR H                    ; @BT 24
    INR L                    ; @BT 2C
    INR M                    ; @BT 34
    INX B                    ; @BT 03
    INX D                    ; @BT 13
    INX H                    ; @BT 23
    INX SP                   ; @BT 33
    IN $84                   ; @BT DB 84
    JMP $1CDE                ; @BT C3 DE 1C
    JNZ $1DEF                ; @BT C2 EF 1D
    JNC $1F00                ; @BT D2 00 1F
    JPO $2011                ; @BT E2 11 20
    JPE $2122                ; @BT EA 22 21
    JC $2233                 ; @BT DA 33 22
    JZ $2344                 ; @BT CA 44 23
    JP $2455                 ; @BT F2 55 24
    JM $2566                 ; @BT FA 66 25
    LDA $2677                ; @BT 3A 77 26
    LDAX B                   ; @BT 0A
    LDAX D                   ; @BT 1A
    LHLD $2788               ; @BT 2A 88 27
    LXI B,$2899              ; @BT 01 99 28
    LXI D,$29AA              ; @BT 11 AA 29
    LXI H,$2ABB              ; @BT 21 BB 2A
    LXI SP,$2BCC             ; @BT 31 CC 2B
    MOV B,B                  ; @BT 40
    MOV B,C                  ; @BT 41
    MOV B,D                  ; @BT 42
    MOV B,E                  ; @BT 43
    MOV B,H                  ; @BT 44
    MOV B,L                  ; @BT 45
    MOV B,M                  ; @BT 46
    MOV B,A                  ; @BT 47
    MOV C,B                  ; @BT 48
    MOV C,C                  ; @BT 49
    MOV C,D                  ; @BT 4A
    MOV C,E                  ; @BT 4B
    MOV C,H                  ; @BT 4C
    MOV C,L                  ; @BT 4D
    MOV C,M                  ; @BT 4E
    MOV C,A                  ; @BT 4F
    MOV D,B                  ; @BT 50
    MOV D,C                  ; @BT 51
    MOV D,D                  ; @BT 52
    MOV D,E                  ; @BT 53
    MOV D,H                  ; @BT 54
    MOV D,L                  ; @BT 55
    MOV D,M                  ; @BT 56
    MOV D,A                  ; @BT 57
    MOV E,B                  ; @BT 58
    MOV E,C                  ; @BT 59
    MOV E,D                  ; @BT 5A
    MOV E,E                  ; @BT 5B
    MOV E,H                  ; @BT 5C
    MOV E,L                  ; @BT 5D
    MOV E,M                  ; @BT 5E
    MOV E,A                  ; @BT 5F
    MOV H,B                  ; @BT 60
    MOV H,C                  ; @BT 61
    MOV H,D                  ; @BT 62
    MOV H,E                  ; @BT 63
    MOV H,H                  ; @BT 64
    MOV H,L                  ; @BT 65
    MOV H,M                  ; @BT 66
    MOV H,A                  ; @BT 67
    MOV L,B                  ; @BT 68
    MOV L,C                  ; @BT 69
    MOV L,D                  ; @BT 6A
    MOV L,E                  ; @BT 6B
    MOV L,H                  ; @BT 6C
    MOV L,L                  ; @BT 6D
    MOV L,M                  ; @BT 6E
    MOV L,A                  ; @BT 6F
    MOV M,B                  ; @BT 70
    MOV M,C                  ; @BT 71
    MOV M,D                  ; @BT 72
    MOV M,E                  ; @BT 73
    MOV M,H                  ; @BT 74
    MOV M,L                  ; @BT 75
    MOV M,A                  ; @BT 77
    MOV A,B                  ; @BT 78
    MOV A,C                  ; @BT 79
    MOV A,D                  ; @BT 7A
    MOV A,E                  ; @BT 7B
    MOV A,H                  ; @BT 7C
    MOV A,L                  ; @BT 7D
    MOV A,M                  ; @BT 7E
    MOV A,A                  ; @BT 7F
    MVI A,$9B                ; @BT 3E 9B
    MVI B,$B2                ; @BT 06 B2
    MVI C,$C9                ; @BT 0E C9
    MVI D,$E0                ; @BT 16 E0
    MVI E,$F7                ; @BT 1E F7
    MVI H,$0E                ; @BT 26 0E
    MVI L,$25                ; @BT 2E 25
    MVI M,$3C                ; @BT 36 3C
    NOP                      ; @BT 00
    ORA B                    ; @BT B0
    ORA C                    ; @BT B1
    ORA D                    ; @BT B2
    ORA E                    ; @BT B3
    ORA H                    ; @BT B4
    ORA L                    ; @BT B5
    ORA M                    ; @BT B6
    ORA A                    ; @BT B7
    ORI $53                  ; @BT F6 53
    OUT $6A                  ; @BT D3 6A
    POP PSW                  ; @BT F1
    POP B                    ; @BT C1
    POP D                    ; @BT D1
    POP H                    ; @BT E1
    PUSH PSW                 ; @BT F5
    PUSH B                   ; @BT C5
    PUSH D                   ; @BT D5
    PUSH H                   ; @BT E5
    PCHL                     ; @BT E9
    RAL                      ; @BT 17
    RAR                      ; @BT 1F
    RLC                      ; @BT 07
    RNZ                      ; @BT C0
    RNC                      ; @BT D0
    RRC                      ; @BT 0F
    RST 0                    ; @BT C7
    RST 1                    ; @BT CF
    RST 2                    ; @BT D7
    RST 3                    ; @BT DF
    RST 4                    ; @BT E7
    RST 5                    ; @BT EF
    RST 6                    ; @BT F7
    RST 7                    ; @BT FF
    RET                      ; @BT C9
    RPO                      ; @BT E0
    RPE                      ; @BT E8
    RZ                       ; @BT C8
    RC                       ; @BT D8
    RP                       ; @BT F0
    RM                       ; @BT F8
    STA $2CDD                ; @BT 32 DD 2C
    STC                      ; @BT 37
    SPHL                     ; @BT F9
    STAX B                   ; @BT 02
    STAX D                   ; @BT 12
    SHLD $2DEE               ; @BT 22 EE 2D
    SUB B                    ; @BT 90
    SUB C                    ; @BT 91
    SUB D                    ; @BT 92
    SUB E                    ; @BT 93
    SUB H                    ; @BT 94
    SUB L                    ; @BT 95
    SUB M                    ; @BT 96
    SUB A                    ; @BT 97
    SBB B                    ; @BT 98
    SBB C                    ; @BT 99
    SBB D                    ; @BT 9A
    SBB E                    ; @BT 9B
    SBB H                    ; @BT 9C
    SBB L                    ; @BT 9D
    SBB M                    ; @BT 9E
    SBB A                    ; @BT 9F
    SBI $81                  ; @BT DE 81
    SUI $98                  ; @BT D6 98
    XRA B                    ; @BT A8
    XRA C                    ; @BT A9
    XRA D                    ; @BT AA
    XRA E                    ; @BT AB
    XRA H                    ; @BT AC
    XRA L                    ; @BT AD
    XRA M                    ; @BT AE
    XRA A                    ; @BT AF
    XTHL                     ; @BT E3
    XCHG                     ; @BT EB
    XRI $AF                  ; @BT EE AF
    .db "<01"               ; @BT END

    /////////////////////////////////////////////////////////////////////////////
    // TEST-02: same opcodes as TEST-01, randomised order, branches spread out
    /////////////////////////////////////////////////////////////////////////////

    .db "02>"               ; @BT TEST-02 02 START

    ; GENERATED: copied from TEST-01 with full TEST-02 randomization
    ; GENERATED: seed = 840238450

    MOV C,D                  ; @BT 4A
    CMP L                    ; @BT BD
    ADD M                    ; @BT 86
    MOV D,M                  ; @BT 56
    RM                       ; @BT F8
    XRA L                    ; @BT AD
    SUB D                    ; @BT 92
    MOV H,A                  ; @BT 67
    SBB C                    ; @BT 99
    ANA H                    ; @BT A4
    MOV D,D                  ; @BT 52
    STC                      ; @BT 37
    ORA M                    ; @BT B6
    MOV D,H                  ; @BT 54
    MOV A,E                  ; @BT 7B
    RST 5                    ; @BT EF
    DAD H                    ; @BT 29
    JNZ $1DEF                ; @BT C2 EF 1D
    PUSH H                   ; @BT E5
    XRA E                    ; @BT AB
    INX SP                   ; @BT 33
    STAX B                   ; @BT 02
    RST 6                    ; @BT F7
    SUB E                    ; @BT 93
    INX D                    ; @BT 13
    MOV H,B                  ; @BT 60
    JC $2233                 ; @BT DA 33 22
    DCX B                    ; @BT 0B
    ANA C                    ; @BT A1
    MOV C,M                  ; @BT 4E
    MOV D,L                  ; @BT 55
    MOV L,C                  ; @BT 69
    HLT                      ; @BT 76
    RLC                      ; @BT 07
    SBI $81                  ; @BT DE 81
    SBB E                    ; @BT 9B
    MOV M,H                  ; @BT 74
    MOV M,B                  ; @BT 70
    STA $2CDD                ; @BT 32 DD 2C
    MVI E,$F7                ; @BT 1E F7
    ADD B                    ; @BT 80
    PUSH D                   ; @BT D5
    PCHL                     ; @BT E9
    ANA B                    ; @BT A0
    LHLD $2788               ; @BT 2A 88 27
    ORA A                    ; @BT B7
    DCX D                    ; @BT 1B
    SBB M                    ; @BT 9E
    SBB D                    ; @BT 9A
    MOV E,A                  ; @BT 5F
    MOV H,C                  ; @BT 61
    SPHL                     ; @BT F9
    INX H                    ; @BT 23
    MOV M,A                  ; @BT 77
    ORA H                    ; @BT B4
    MOV D,A                  ; @BT 57
    XCHG                     ; @BT EB
    STAX D                   ; @BT 12
    ORA B                    ; @BT B0
    MOV D,C                  ; @BT 51
    RST 1                    ; @BT CF
    MOV L,D                  ; @BT 6A
    ANA L                    ; @BT A5
    ANA A                    ; @BT A7
    RC                       ; @BT D8
    PUSH PSW                 ; @BT F5
    POP H                    ; @BT E1
    JM $2566                 ; @BT FA 66 25
    MOV B,C                  ; @BT 41
    RPE                      ; @BT E8
    POP PSW                  ; @BT F1
    DAA                      ; @BT 27
    RZ                       ; @BT C8
    DCR L                    ; @BT 2D
    MOV M,L                  ; @BT 75
    DCR H                    ; @BT 25
    DCR B                    ; @BT 05
    MOV L,M                  ; @BT 6E
    ADD L                    ; @BT 85
    ADC H                    ; @BT 8C
    INR M                    ; @BT 34
    SUB B                    ; @BT 90
    MOV C,E                  ; @BT 4B
    POP D                    ; @BT D1
    LDAX B                   ; @BT 0A
    ADD D                    ; @BT 82
    MOV E,B                  ; @BT 58
    ORA E                    ; @BT B3
    SUB L                    ; @BT 95
    MOV M,E                  ; @BT 73
    MOV H,M                  ; @BT 66
    DCR D                    ; @BT 15
    SBB A                    ; @BT 9F
    MOV C,C                  ; @BT 49
    CM $1BCD                 ; @BT FC CD 1B
    MOV C,A                  ; @BT 4F
    MOV E,L                  ; @BT 5D
    CZ $19AB                 ; @BT CC AB 19
    RST 7                    ; @BT FF
    MOV D,B                  ; @BT 50
    JMP $1CDE                ; @BT C3 DE 1C
    RAR                      ; @BT 1F
    ADC A                    ; @BT 8F
    MOV H,L                  ; @BT 65
    RST 0                    ; @BT C7
    DI                       ; @BT F3
    ADI $28                  ; @BT C6 28
    LXI SP,$2BCC             ; @BT 31 CC 2B
    CNC $1456                ; @BT D4 56 14
    ORA D                    ; @BT B2
    MVI L,$25                ; @BT 2E 25
    MVI H,$0E                ; @BT 26 0E
    MOV A,A                  ; @BT 7F
    CMP D                    ; @BT BA
    INR C                    ; @BT 0C
    LDAX D                   ; @BT 1A
    MOV H,D                  ; @BT 62
    ADC L                    ; @BT 8D
    INR L                    ; @BT 2C
    MOV E,M                  ; @BT 5E
    CPO $1567                ; @BT E4 67 15
    CPE $1678                ; @BT EC 78 16
    MOV L,L                  ; @BT 6D
    DAD D                    ; @BT 19
    ADC D                    ; @BT 8A
    DCR C                    ; @BT 0D
    CP $1ABC                 ; @BT F4 BC 1A
    MOV A,L                  ; @BT 7D
    CMP B                    ; @BT B8
    NOP                      ; @BT 00
    ANA M                    ; @BT A6
    RPO                      ; @BT E0
    ADC C                    ; @BT 89
    DAD B                    ; @BT 09
    MOV L,E                  ; @BT 6B
    RP                       ; @BT F0
    JPE $2122                ; @BT EA 22 21
    ORA C                    ; @BT B1
    XRA M                    ; @BT AE
    RST 4                    ; @BT E7
    SUB M                    ; @BT 96
    JNC $1F00                ; @BT D2 00 1F
    MOV L,B                  ; @BT 68
    RNC                      ; @BT D0
    XRA B                    ; @BT A8
    MOV L,A                  ; @BT 6F
    ANA D                    ; @BT A2
    MVI C,$C9                ; @BT 0E C9
    OUT $6A                  ; @BT D3 6A
    CMP H                    ; @BT BC
    SUI $98                  ; @BT D6 98
    LDA $2677                ; @BT 3A 77 26
    MOV B,B                  ; @BT 40
    INR H                    ; @BT 24
    MVI D,$E0                ; @BT 16 E0
    MOV A,B                  ; @BT 78
    SUB C                    ; @BT 91
    CMP M                    ; @BT BE
    MOV E,C                  ; @BT 59
    ADC M                    ; @BT 8E
    LXI H,$2ABB              ; @BT 21 BB 2A
    DAD SP                   ; @BT 39
    MOV D,E                  ; @BT 53
    MOV A,H                  ; @BT 7C
    ADD H                    ; @BT 84
    PUSH B                   ; @BT C5
    JPO $2011                ; @BT E2 11 20
    ANA E                    ; @BT A3
    ORI $53                  ; @BT F6 53
    ADD A                    ; @BT 87
    SUB A                    ; @BT 97
    ADC E                    ; @BT 8B
    LXI D,$29AA              ; @BT 11 AA 29
    SBB H                    ; @BT 9C
    MOV B,A                  ; @BT 47
    MOV C,L                  ; @BT 4D
    INR D                    ; @BT 14
    CNZ $1345                ; @BT C4 45 13
    DCX H                    ; @BT 2B
    MOV A,M                  ; @BT 7E
    MOV L,H                  ; @BT 6C
    MOV B,M                  ; @BT 46
    XRA C                    ; @BT A9
    RET                      ; @BT C9
    DCR M                    ; @BT 35
    CC $189A                 ; @BT DC 9A 18
    MOV B,L                  ; @BT 45
    ANI $56                  ; @BT E6 56
    SBB B                    ; @BT 98
    DCR A                    ; @BT 3D
    INX B                    ; @BT 03
    MOV B,H                  ; @BT 44
    MOV A,C                  ; @BT 79
    POP B                    ; @BT C1
    ADC B                    ; @BT 88
    ADD C                    ; @BT 81
    XTHL                     ; @BT E3
    RAL                      ; @BT 17
    JP $2455                 ; @BT F2 55 24
    XRA H                    ; @BT AC
    SUB H                    ; @BT 94
    CPI $6D                  ; @BT FE 6D
    LXI B,$2899              ; @BT 01 99 28
    MVI M,$3C                ; @BT 36 3C
    CMP E                    ; @BT BB
    MOV M,C                  ; @BT 71
    RST 2                    ; @BT D7
    MOV E,H                  ; @BT 5C
    MVI B,$B2                ; @BT 06 B2
    CMA                      ; @BT 2F
    MVI A,$9B                ; @BT 3E 9B
    MOV E,E                  ; @BT 5B
    CMC                      ; @BT 3F
    MOV C,H                  ; @BT 4C
    MOV B,D                  ; @BT 42
    XRA A                    ; @BT AF
    CMP A                    ; @BT BF
    DCR E                    ; @BT 1D
    RNZ                      ; @BT C0
    XRI $AF                  ; @BT EE AF
    MOV H,H                  ; @BT 64
    MOV B,E                  ; @BT 43
    IN $84                   ; @BT DB 84
    INR E                    ; @BT 1C
    MOV E,D                  ; @BT 5A
    INR A                    ; @BT 3C
    JZ $2344                 ; @BT CA 44 23
    MOV H,E                  ; @BT 63
    INR B                    ; @BT 04
    EI                       ; @BT FB
    SHLD $2DEE               ; @BT 22 EE 2D
    ACI $3F                  ; @BT CE 3F
    MOV A,D                  ; @BT 7A
    CMP C                    ; @BT B9
    RST 3                    ; @BT DF
    MOV M,D                  ; @BT 72
    DCX SP                   ; @BT 3B
    ADD E                    ; @BT 83
    MOV C,B                  ; @BT 48
    RRC                      ; @BT 0F
    CALL $1789               ; @BT CD 89 17
    SBB L                    ; @BT 9D
    XRA D                    ; @BT AA
    ORA L                    ; @BT B5

    .db "<02"               ; @BT END

    /////////////////////////////////////////////////////////////////////////////
    // TEST-03: same opcodes, randomised order, operands as arithmetic expressions
    /////////////////////////////////////////////////////////////////////////////

    .db "03>"               ; @BT TEST-03 03 START

    ; GENERATED: copied from TEST-02 with varied arithmetic expressions for numeric operands

    ; GENERATED: copied from TEST-01 with full TEST-02 randomization
    ; GENERATED: seed = 840238450

    MOV C,D                  ; @BT 4A
    CMP L                    ; @BT BD
    ADD M                    ; @BT 86
    MOV D,M                  ; @BT 56
    RM                       ; @BT F8
    XRA L                    ; @BT AD
    SUB D                    ; @BT 92
    MOV H,A                  ; @BT 67
    SBB C                    ; @BT 99
    ANA H                    ; @BT A4
    MOV D,D                  ; @BT 52
    STC                      ; @BT 37
    ORA M                    ; @BT B6
    MOV D,H                  ; @BT 54
    MOV A,E                  ; @BT 7B
    RST ((5+5)-5)                    ; @BT EF
    DAD H                    ; @BT 29
    JNZ (($1DEF-7)+7)                ; @BT C2 EF 1D
    PUSH H                   ; @BT E5
    XRA E                    ; @BT AB
    INX SP                   ; @BT 33
    STAX B                   ; @BT 02
    RST (((6*6)/3)-(6))                    ; @BT F7
    SUB E                    ; @BT 93
    INX D                    ; @BT 13
    MOV H,B                  ; @BT 60
    JC ((($2233*10)/5)-($2233))                 ; @BT DA 33 22
    DCX B                    ; @BT 0B
    ANA C                    ; @BT A1
    MOV C,M                  ; @BT 4E
    MOV D,L                  ; @BT 55
    MOV L,C                  ; @BT 69
    HLT                      ; @BT 76
    RLC                      ; @BT 07
    SBI (($81*2)-(($81)-0))                  ; @BT DE 81
    SBB E                    ; @BT 9B
    MOV M,H                  ; @BT 74
    MOV M,B                  ; @BT 70
    STA ((($2CDD+11)-4)-7)                ; @BT 32 DD 2C
    MVI E,((($F7+3)-1)-2)                ; @BT 1E F7
    ADD B                    ; @BT 80
    PUSH D                   ; @BT D5
    PCHL                     ; @BT E9
    ANA B                    ; @BT A0
    LHLD (($2788+5)-5)               ; @BT 2A 88 27
    ORA A                    ; @BT B7
    DCX D                    ; @BT 1B
    SBB M                    ; @BT 9E
    SBB D                    ; @BT 9A
    MOV E,A                  ; @BT 5F
    MOV H,C                  ; @BT 61
    SPHL                     ; @BT F9
    INX H                    ; @BT 23
    MOV M,A                  ; @BT 77
    ORA H                    ; @BT B4
    MOV D,A                  ; @BT 57
    XCHG                     ; @BT EB
    STAX D                   ; @BT 12
    ORA B                    ; @BT B0
    MOV D,C                  ; @BT 51
    RST ((1-7)+7)                    ; @BT CF
    MOV L,D                  ; @BT 6A
    ANA L                    ; @BT A5
    ANA A                    ; @BT A7
    RC                       ; @BT D8
    PUSH PSW                 ; @BT F5
    POP H                    ; @BT E1
    JM ((($2566*6)/3)-($2566))                 ; @BT FA 66 25
    MOV B,C                  ; @BT 41
    RPE                      ; @BT E8
    POP PSW                  ; @BT F1
    DAA                      ; @BT 27
    RZ                       ; @BT C8
    DCR L                    ; @BT 2D
    MOV M,L                  ; @BT 75
    DCR H                    ; @BT 25
    DCR B                    ; @BT 05
    MOV L,M                  ; @BT 6E
    ADD L                    ; @BT 85
    ADC H                    ; @BT 8C
    INR M                    ; @BT 34
    SUB B                    ; @BT 90
    MOV C,E                  ; @BT 4B
    POP D                    ; @BT D1
    LDAX B                   ; @BT 0A
    ADD D                    ; @BT 82
    MOV E,B                  ; @BT 58
    ORA E                    ; @BT B3
    SUB L                    ; @BT 95
    MOV M,E                  ; @BT 73
    MOV H,M                  ; @BT 66
    DCR D                    ; @BT 15
    SBB A                    ; @BT 9F
    MOV C,C                  ; @BT 49
    CM ((($1BCD*10)/5)-($1BCD))                 ; @BT FC CD 1B
    MOV C,A                  ; @BT 4F
    MOV E,L                  ; @BT 5D
    CZ (($19AB*2)-(($19AB)-0))                 ; @BT CC AB 19
    RST (((7+11)-4)-7)                    ; @BT FF
    MOV D,B                  ; @BT 50
    JMP ((($1CDE+3)-1)-2)                ; @BT C3 DE 1C
    RAR                      ; @BT 1F
    ADC A                    ; @BT 8F
    MOV H,L                  ; @BT 65
    RST ((0+5)-5)                    ; @BT C7
    DI                       ; @BT F3
    ADI (($28-7)+7)                  ; @BT C6 28
    LXI SP,((($2BCC*6)/3)-($2BCC))             ; @BT 31 CC 2B
    CNC ((($1456*10)/5)-($1456))                ; @BT D4 56 14
    ORA D                    ; @BT B2
    MVI L,(($25*2)-(($25)-0))                ; @BT 2E 25
    MVI H,((($0E+11)-4)-7)                ; @BT 26 0E
    MOV A,A                  ; @BT 7F
    CMP D                    ; @BT BA
    INR C                    ; @BT 0C
    LDAX D                   ; @BT 1A
    MOV H,D                  ; @BT 62
    ADC L                    ; @BT 8D
    INR L                    ; @BT 2C
    MOV E,M                  ; @BT 5E
    CPO ((($1567+3)-1)-2)                ; @BT E4 67 15
    CPE (($1678+5)-5)                ; @BT EC 78 16
    MOV L,L                  ; @BT 6D
    DAD D                    ; @BT 19
    ADC D                    ; @BT 8A
    DCR C                    ; @BT 0D
    CP (($1ABC-7)+7)                 ; @BT F4 BC 1A
    MOV A,L                  ; @BT 7D
    CMP B                    ; @BT B8
    NOP                      ; @BT 00
    ANA M                    ; @BT A6
    RPO                      ; @BT E0
    ADC C                    ; @BT 89
    DAD B                    ; @BT 09
    MOV L,E                  ; @BT 6B
    RP                       ; @BT F0
    JPE ((($2122*6)/3)-($2122))                ; @BT EA 22 21
    ORA C                    ; @BT B1
    XRA M                    ; @BT AE
    RST (((4*10)/5)-(4))                    ; @BT E7
    SUB M                    ; @BT 96
    JNC (($1F00*2)-(($1F00)-0))                ; @BT D2 00 1F
    MOV L,B                  ; @BT 68
    RNC                      ; @BT D0
    XRA B                    ; @BT A8
    MOV L,A                  ; @BT 6F
    ANA D                    ; @BT A2
    MVI C,((($C9+11)-4)-7)                ; @BT 0E C9
    OUT ((($6A+3)-1)-2)                  ; @BT D3 6A
    CMP H                    ; @BT BC
    SUI (($98+5)-5)                  ; @BT D6 98
    LDA (($2677-7)+7)                ; @BT 3A 77 26
    MOV B,B                  ; @BT 40
    INR H                    ; @BT 24
    MVI D,((($E0*6)/3)-($E0))                ; @BT 16 E0
    MOV A,B                  ; @BT 78
    SUB C                    ; @BT 91
    CMP M                    ; @BT BE
    MOV E,C                  ; @BT 59
    ADC M                    ; @BT 8E
    LXI H,((($2ABB*10)/5)-($2ABB))              ; @BT 21 BB 2A
    DAD SP                   ; @BT 39
    MOV D,E                  ; @BT 53
    MOV A,H                  ; @BT 7C
    ADD H                    ; @BT 84
    PUSH B                   ; @BT C5
    JPO (($2011*2)-(($2011)-0))                ; @BT E2 11 20
    ANA E                    ; @BT A3
    ORI ((($53+11)-4)-7)                  ; @BT F6 53
    ADD A                    ; @BT 87
    SUB A                    ; @BT 97
    ADC E                    ; @BT 8B
    LXI D,((($29AA+3)-1)-2)              ; @BT 11 AA 29
    SBB H                    ; @BT 9C
    MOV B,A                  ; @BT 47
    MOV C,L                  ; @BT 4D
    INR D                    ; @BT 14
    CNZ (($1345+5)-5)                ; @BT C4 45 13
    DCX H                    ; @BT 2B
    MOV A,M                  ; @BT 7E
    MOV L,H                  ; @BT 6C
    MOV B,M                  ; @BT 46
    XRA C                    ; @BT A9
    RET                      ; @BT C9
    DCR M                    ; @BT 35
    CC (($189A-7)+7)                 ; @BT DC 9A 18
    MOV B,L                  ; @BT 45
    ANI ((($56*6)/3)-($56))                  ; @BT E6 56
    SBB B                    ; @BT 98
    DCR A                    ; @BT 3D
    INX B                    ; @BT 03
    MOV B,H                  ; @BT 44
    MOV A,C                  ; @BT 79
    POP B                    ; @BT C1
    ADC B                    ; @BT 88
    ADD C                    ; @BT 81
    XTHL                     ; @BT E3
    RAL                      ; @BT 17
    JP ((($2455*10)/5)-($2455))                 ; @BT F2 55 24
    XRA H                    ; @BT AC
    SUB H                    ; @BT 94
    CPI (($6D*2)-(($6D)-0))                  ; @BT FE 6D
    LXI B,((($2899+11)-4)-7)              ; @BT 01 99 28
    MVI M,((($3C+3)-1)-2)                ; @BT 36 3C
    CMP E                    ; @BT BB
    MOV M,C                  ; @BT 71
    RST ((2+5)-5)                    ; @BT D7
    MOV E,H                  ; @BT 5C
    MVI B,(($B2-7)+7)                ; @BT 06 B2
    CMA                      ; @BT 2F
    MVI A,((($9B*6)/3)-($9B))                ; @BT 3E 9B
    MOV E,E                  ; @BT 5B
    CMC                      ; @BT 3F
    MOV C,H                  ; @BT 4C
    MOV B,D                  ; @BT 42
    XRA A                    ; @BT AF
    CMP A                    ; @BT BF
    DCR E                    ; @BT 1D
    RNZ                      ; @BT C0
    XRI ((($AF*10)/5)-($AF))                  ; @BT EE AF
    MOV H,H                  ; @BT 64
    MOV B,E                  ; @BT 43
    IN (($84*2)-(($84)-0))                   ; @BT DB 84
    INR E                    ; @BT 1C
    MOV E,D                  ; @BT 5A
    INR A                    ; @BT 3C
    JZ ((($2344+11)-4)-7)                 ; @BT CA 44 23
    MOV H,E                  ; @BT 63
    INR B                    ; @BT 04
    EI                       ; @BT FB
    SHLD ((($2DEE+3)-1)-2)               ; @BT 22 EE 2D
    ACI (($3F+5)-5)                  ; @BT CE 3F
    MOV A,D                  ; @BT 7A
    CMP C                    ; @BT B9
    RST ((3-7)+7)                    ; @BT DF
    MOV M,D                  ; @BT 72
    DCX SP                   ; @BT 3B
    ADD E                    ; @BT 83
    MOV C,B                  ; @BT 48
    RRC                      ; @BT 0F
    CALL ((($1789*6)/3)-($1789))               ; @BT CD 89 17
    SBB L                    ; @BT 9D
    XRA D                    ; @BT AA
    ORA L                    ; @BT B5

    .db "<03"               ; @BT END

    /////////////////////////////////////////////////////////////////////////////
    // TEST-04: similar to TEST-03 and then defines + expressions
    /////////////////////////////////////////////////////////////////////////////





    ; TEST-04 generated defines
    .define T4_DEF_001 ((5+5)-5)
    .define T4_DEF_002 ((T4_DEF_001)+(($1DEF)-(5)))
    .define T4_DEF_003 ((6-7)+7)
    .define T4_DEF_004 (((T4_DEF_003)+($2233))-(6))
    .define T4_DEF_005 ((($81*6)/3)-($81))
    .define T4_DEF_006 (((T4_DEF_005)*2+($2CDD)-(($81)*2)))
    .define T4_DEF_007 ((($F7*10)/5)-($F7))
    .define T4_DEF_008 ((($2788)-($F7))+(T4_DEF_007))
    .define T4_DEF_009 ((1*2)-((1)-0))
    .define T4_DEF_010 ((T4_DEF_009)+(($2566)-((1+3)-3)))
    .define T4_DEF_011 ((($1BCD+11)-4)-7)
    .define T4_DEF_012 ((T4_DEF_011)+(($19AB)-($1BCD)))
    .define T4_DEF_013 (((7+3)-1)-2)
    .define T4_DEF_014 (((T4_DEF_013)+($1CDE))-(7))
    .define T4_DEF_015 ((0+5)-5)
    .define T4_DEF_016 (((T4_DEF_015)*2+($28)-((0)*2)))
    .define T4_DEF_017 (($2BCC-7)+7)
    .define T4_DEF_018 ((($1456)-($2BCC))+(T4_DEF_017))
    .define T4_DEF_019 ((($25*6)/3)-($25))
    .define T4_DEF_020 ((T4_DEF_019)+(($0E)-(($25+3)-3)))
    .define T4_DEF_021 ((($1567*10)/5)-($1567))
    .define T4_DEF_022 ((T4_DEF_021)+(($1678)-($1567)))
    .define T4_DEF_023 (($1ABC*2)-(($1ABC)-0))
    .define T4_DEF_024 (((T4_DEF_023)+($2122))-($1ABC))
    .define T4_DEF_025 (((4+11)-4)-7)
    .define T4_DEF_026 (((T4_DEF_025)*2+($1F00)-((4)*2)))
    .db "04>"               ; @BT TEST-04 04 START

    ; GENERATED: copied from TEST-03 with varied define-based operand replacement
    ; GENERATED: replaced operands = 26 / 52

    ; GENERATED: copied from TEST-02 with varied arithmetic expressions for numeric operands

    ; GENERATED: copied from TEST-01 with full TEST-02 randomization
    ; GENERATED: seed = 840238450

    MOV C,D                  ; @BT 4A
    CMP L                    ; @BT BD
    ADD M                    ; @BT 86
    MOV D,M                  ; @BT 56
    RM                       ; @BT F8
    XRA L                    ; @BT AD
    SUB D                    ; @BT 92
    MOV H,A                  ; @BT 67
    SBB C                    ; @BT 99
    ANA H                    ; @BT A4
    MOV D,D                  ; @BT 52
    STC                      ; @BT 37
    ORA M                    ; @BT B6
    MOV D,H                  ; @BT 54
    MOV A,E                  ; @BT 7B
    RST ((T4_DEF_001+5)-5)                    ; @BT EF
    DAD H                    ; @BT 29
    JNZ ((T4_DEF_002-7)+7)                ; @BT C2 EF 1D
    PUSH H                   ; @BT E5
    XRA E                    ; @BT AB
    INX SP                   ; @BT 33
    STAX B                   ; @BT 02
    RST (((T4_DEF_003*6)/3)-(6))                    ; @BT F7
    SUB E                    ; @BT 93
    INX D                    ; @BT 13
    MOV H,B                  ; @BT 60
    JC (((T4_DEF_004*10)/5)-($2233))                 ; @BT DA 33 22
    DCX B                    ; @BT 0B
    ANA C                    ; @BT A1
    MOV C,M                  ; @BT 4E
    MOV D,L                  ; @BT 55
    MOV L,C                  ; @BT 69
    HLT                      ; @BT 76
    RLC                      ; @BT 07
    SBI ((T4_DEF_005*2)-(($81)-0))                  ; @BT DE 81
    SBB E                    ; @BT 9B
    MOV M,H                  ; @BT 74
    MOV M,B                  ; @BT 70
    STA (((T4_DEF_006+11)-4)-7)                ; @BT 32 DD 2C
    MVI E,(((T4_DEF_007+3)-1)-2)                ; @BT 1E F7
    ADD B                    ; @BT 80
    PUSH D                   ; @BT D5
    PCHL                     ; @BT E9
    ANA B                    ; @BT A0
    LHLD ((T4_DEF_008+5)-5)               ; @BT 2A 88 27
    ORA A                    ; @BT B7
    DCX D                    ; @BT 1B
    SBB M                    ; @BT 9E
    SBB D                    ; @BT 9A
    MOV E,A                  ; @BT 5F
    MOV H,C                  ; @BT 61
    SPHL                     ; @BT F9
    INX H                    ; @BT 23
    MOV M,A                  ; @BT 77
    ORA H                    ; @BT B4
    MOV D,A                  ; @BT 57
    XCHG                     ; @BT EB
    STAX D                   ; @BT 12
    ORA B                    ; @BT B0
    MOV D,C                  ; @BT 51
    RST ((T4_DEF_009-7)+7)                    ; @BT CF
    MOV L,D                  ; @BT 6A
    ANA L                    ; @BT A5
    ANA A                    ; @BT A7
    RC                       ; @BT D8
    PUSH PSW                 ; @BT F5
    POP H                    ; @BT E1
    JM (((T4_DEF_010*6)/3)-($2566))                 ; @BT FA 66 25
    MOV B,C                  ; @BT 41
    RPE                      ; @BT E8
    POP PSW                  ; @BT F1
    DAA                      ; @BT 27
    RZ                       ; @BT C8
    DCR L                    ; @BT 2D
    MOV M,L                  ; @BT 75
    DCR H                    ; @BT 25
    DCR B                    ; @BT 05
    MOV L,M                  ; @BT 6E
    ADD L                    ; @BT 85
    ADC H                    ; @BT 8C
    INR M                    ; @BT 34
    SUB B                    ; @BT 90
    MOV C,E                  ; @BT 4B
    POP D                    ; @BT D1
    LDAX B                   ; @BT 0A
    ADD D                    ; @BT 82
    MOV E,B                  ; @BT 58
    ORA E                    ; @BT B3
    SUB L                    ; @BT 95
    MOV M,E                  ; @BT 73
    MOV H,M                  ; @BT 66
    DCR D                    ; @BT 15
    SBB A                    ; @BT 9F
    MOV C,C                  ; @BT 49
    CM (((T4_DEF_011*10)/5)-($1BCD))                 ; @BT FC CD 1B
    MOV C,A                  ; @BT 4F
    MOV E,L                  ; @BT 5D
    CZ ((T4_DEF_012*2)-(($19AB)-0))                 ; @BT CC AB 19
    RST (((T4_DEF_013+11)-4)-7)                    ; @BT FF
    MOV D,B                  ; @BT 50
    JMP (((T4_DEF_014+3)-1)-2)                ; @BT C3 DE 1C
    RAR                      ; @BT 1F
    ADC A                    ; @BT 8F
    MOV H,L                  ; @BT 65
    RST ((T4_DEF_015+5)-5)                    ; @BT C7
    DI                       ; @BT F3
    ADI ((T4_DEF_016-7)+7)                  ; @BT C6 28
    LXI SP,(((T4_DEF_017*6)/3)-($2BCC))             ; @BT 31 CC 2B
    CNC (((T4_DEF_018*10)/5)-($1456))                ; @BT D4 56 14
    ORA D                    ; @BT B2
    MVI L,((T4_DEF_019*2)-(($25)-0))                ; @BT 2E 25
    MVI H,(((T4_DEF_020+11)-4)-7)                ; @BT 26 0E
    MOV A,A                  ; @BT 7F
    CMP D                    ; @BT BA
    INR C                    ; @BT 0C
    LDAX D                   ; @BT 1A
    MOV H,D                  ; @BT 62
    ADC L                    ; @BT 8D
    INR L                    ; @BT 2C
    MOV E,M                  ; @BT 5E
    CPO (((T4_DEF_021+3)-1)-2)                ; @BT E4 67 15
    CPE ((T4_DEF_022+5)-5)                ; @BT EC 78 16
    MOV L,L                  ; @BT 6D
    DAD D                    ; @BT 19
    ADC D                    ; @BT 8A
    DCR C                    ; @BT 0D
    CP ((T4_DEF_023-7)+7)                 ; @BT F4 BC 1A
    MOV A,L                  ; @BT 7D
    CMP B                    ; @BT B8
    NOP                      ; @BT 00
    ANA M                    ; @BT A6
    RPO                      ; @BT E0
    ADC C                    ; @BT 89
    DAD B                    ; @BT 09
    MOV L,E                  ; @BT 6B
    RP                       ; @BT F0
    JPE (((T4_DEF_024*6)/3)-($2122))                ; @BT EA 22 21
    ORA C                    ; @BT B1
    XRA M                    ; @BT AE
    RST (((T4_DEF_025*10)/5)-(4))                    ; @BT E7
    SUB M                    ; @BT 96
    JNC ((T4_DEF_026*2)-(($1F00)-0))                ; @BT D2 00 1F
    MOV L,B                  ; @BT 68
    RNC                      ; @BT D0
    XRA B                    ; @BT A8
    MOV L,A                  ; @BT 6F
    ANA D                    ; @BT A2
    MVI C,((($C9+11)-4)-7)                ; @BT 0E C9
    OUT ((($6A+3)-1)-2)                  ; @BT D3 6A
    CMP H                    ; @BT BC
    SUI (($98+5)-5)                  ; @BT D6 98
    LDA (($2677-7)+7)                ; @BT 3A 77 26
    MOV B,B                  ; @BT 40
    INR H                    ; @BT 24
    MVI D,((($E0*6)/3)-($E0))                ; @BT 16 E0
    MOV A,B                  ; @BT 78
    SUB C                    ; @BT 91
    CMP M                    ; @BT BE
    MOV E,C                  ; @BT 59
    ADC M                    ; @BT 8E
    LXI H,((($2ABB*10)/5)-($2ABB))              ; @BT 21 BB 2A
    DAD SP                   ; @BT 39
    MOV D,E                  ; @BT 53
    MOV A,H                  ; @BT 7C
    ADD H                    ; @BT 84
    PUSH B                   ; @BT C5
    JPO (($2011*2)-(($2011)-0))                ; @BT E2 11 20
    ANA E                    ; @BT A3
    ORI ((($53+11)-4)-7)                  ; @BT F6 53
    ADD A                    ; @BT 87
    SUB A                    ; @BT 97
    ADC E                    ; @BT 8B
    LXI D,((($29AA+3)-1)-2)              ; @BT 11 AA 29
    SBB H                    ; @BT 9C
    MOV B,A                  ; @BT 47
    MOV C,L                  ; @BT 4D
    INR D                    ; @BT 14
    CNZ (($1345+5)-5)                ; @BT C4 45 13
    DCX H                    ; @BT 2B
    MOV A,M                  ; @BT 7E
    MOV L,H                  ; @BT 6C
    MOV B,M                  ; @BT 46
    XRA C                    ; @BT A9
    RET                      ; @BT C9
    DCR M                    ; @BT 35
    CC (($189A-7)+7)                 ; @BT DC 9A 18
    MOV B,L                  ; @BT 45
    ANI ((($56*6)/3)-($56))                  ; @BT E6 56
    SBB B                    ; @BT 98
    DCR A                    ; @BT 3D
    INX B                    ; @BT 03
    MOV B,H                  ; @BT 44
    MOV A,C                  ; @BT 79
    POP B                    ; @BT C1
    ADC B                    ; @BT 88
    ADD C                    ; @BT 81
    XTHL                     ; @BT E3
    RAL                      ; @BT 17
    JP ((($2455*10)/5)-($2455))                 ; @BT F2 55 24
    XRA H                    ; @BT AC
    SUB H                    ; @BT 94
    CPI (($6D*2)-(($6D)-0))                  ; @BT FE 6D
    LXI B,((($2899+11)-4)-7)              ; @BT 01 99 28
    MVI M,((($3C+3)-1)-2)                ; @BT 36 3C
    CMP E                    ; @BT BB
    MOV M,C                  ; @BT 71
    RST ((2+5)-5)                    ; @BT D7
    MOV E,H                  ; @BT 5C
    MVI B,(($B2-7)+7)                ; @BT 06 B2
    CMA                      ; @BT 2F
    MVI A,((($9B*6)/3)-($9B))                ; @BT 3E 9B
    MOV E,E                  ; @BT 5B
    CMC                      ; @BT 3F
    MOV C,H                  ; @BT 4C
    MOV B,D                  ; @BT 42
    XRA A                    ; @BT AF
    CMP A                    ; @BT BF
    DCR E                    ; @BT 1D
    RNZ                      ; @BT C0
    XRI ((($AF*10)/5)-($AF))                  ; @BT EE AF
    MOV H,H                  ; @BT 64
    MOV B,E                  ; @BT 43
    IN (($84*2)-(($84)-0))                   ; @BT DB 84
    INR E                    ; @BT 1C
    MOV E,D                  ; @BT 5A
    INR A                    ; @BT 3C
    JZ ((($2344+11)-4)-7)                 ; @BT CA 44 23
    MOV H,E                  ; @BT 63
    INR B                    ; @BT 04
    EI                       ; @BT FB
    SHLD ((($2DEE+3)-1)-2)               ; @BT 22 EE 2D
    ACI (($3F+5)-5)                  ; @BT CE 3F
    MOV A,D                  ; @BT 7A
    CMP C                    ; @BT B9
    RST ((3-7)+7)                    ; @BT DF
    MOV M,D                  ; @BT 72
    DCX SP                   ; @BT 3B
    ADD E                    ; @BT 83
    MOV C,B                  ; @BT 48
    RRC                      ; @BT 0F
    CALL ((($1789*6)/3)-($1789))               ; @BT CD 89 17
    SBB L                    ; @BT 9D
    XRA D                    ; @BT AA
    ORA L                    ; @BT B5

    .db "<04"               ; @BT END

    /////////////////////////////////////////////////////////////////////////////
    // TEST-05: similar to TEST-04 and defines + functions + expressions
    /////////////////////////////////////////////////////////////////////////////



    ; TEST-05 generated defines/functions
    .define T5_DEF_001 ((5+5)-5)
    .define T5_DEF_002 ((T5_DEF_001)+(($1DEF)-(5)))
    .define T5_DEF_003 ((6-7)+7)
    .define T5_DEF_004 (((T5_DEF_003)+($2233))-(6))
    .define T5_DEF_005 ((($81*6)/3)-($81))
    .define T5_DEF_006 (((T5_DEF_005)*2+($2CDD)-(($81)*2)))
    .define T5_DEF_007 ((($F7*10)/5)-($F7))
    .define T5_DEF_008 ((($2788)-($F7))+(T5_DEF_007))
    .define T5_DEF_009 ((1*2)-((1)-0))
    .define T5_DEF_010 ((T5_DEF_009)+(($2566)-((1+3)-3)))
    .define T5_DEF_011 ((($1BCD+11)-4)-7)
    .define T5_DEF_012 ((T5_DEF_011)+(($19AB)-($1BCD)))
    .define T5_DEF_013 (((7+3)-1)-2)
    .define T5_DEF_014 (((T5_DEF_013)+($1CDE))-(7))
    .define T5_DEF_015 ((0+5)-5)
    .define T5_DEF_016 (((T5_DEF_015)*2+($28)-((0)*2)))
    .define T5_DEF_017 (($2BCC-7)+7)
    .define T5_DEF_018 ((($1456)-($2BCC))+(T5_DEF_017))
    .define T5_DEF_019 ((($25*6)/3)-($25))
    .define T5_DEF_020 ((T5_DEF_019)+(($0E)-(($25+3)-3)))
    .define T5_DEF_021 ((($1567*10)/5)-($1567))
    .define T5_DEF_022 ((T5_DEF_021)+(($1678)-($1567)))
    .define T5_DEF_023 (($1ABC*2)-(($1ABC)-0))
    .define T5_DEF_024 (((T5_DEF_023)+($2122))-($1ABC))
    .define T5_DEF_025 (((4+11)-4)-7)
    .define T5_DEF_026 (((T5_DEF_025)*2+($1F00)-((4)*2)))

    ; TEST-05 generated functions
    .function T5_FN_001(a,b) ((((a-b)*8)/4)-((a-b)*2)+(a-b)+T5_DEF_001-T5_DEF_001)
    .function T5_FN_002(a,b) (((T5_FN_001(a,b)*8)/4)-(T5_FN_001(a,b)*2)+T5_FN_001(a,b)+T5_DEF_002-T5_DEF_002)
    .function T5_FN_003(a,b,c) ((((a+b-c)*8)/4)-((a+b-c)*2)+(a+b-c)+T5_DEF_003-T5_DEF_003)
    .function T5_FN_004(a,b,c) (((T5_FN_003(a,b,c)+T5_DEF_004)-T5_DEF_004))
    .function T5_FN_005(a,b,c,d) ((((a+b-c-d)*8)/4)-((a+b-c-d)*2)+(a+b-c-d)+T5_DEF_005-T5_DEF_005)
    .function T5_FN_006(a,b,c,d) (((T5_FN_005(a,b,c,d))*2+T5_DEF_006-T5_DEF_006-T5_FN_005(a,b,c,d)))
    .function T5_FN_007(a,b) ((((a-b)*8)/4)-((a-b)*2)+(a-b)+T5_DEF_007-T5_DEF_007)
    .function T5_FN_008(a,b) ((T5_FN_007(a,b))+(T5_DEF_008-T5_DEF_008))
    .function T5_FN_009(a,b,c) ((((a+b-c)*8)/4)-((a+b-c)*2)+(a+b-c)+T5_DEF_009-T5_DEF_009)
    .function T5_FN_010(a,b,c) (((T5_FN_009(a,b,c)+3)-3)+(T5_DEF_010-T5_DEF_010))
    .function T5_FN_011(a,b,c,d) ((((a+b-c-d)*8)/4)-((a+b-c-d)*2)+(a+b-c-d)+T5_DEF_011-T5_DEF_011)
    .function T5_FN_012(a,b,c,d) (((T5_FN_011(a,b,c,d)*8)/4)-(T5_FN_011(a,b,c,d)*2)+T5_FN_011(a,b,c,d)+T5_DEF_012-T5_DEF_012)
    .function T5_FN_013(a,b) ((((a-b)*8)/4)-((a-b)*2)+(a-b)+T5_DEF_013-T5_DEF_013)
    .db "05>"               ; @BT TEST-05 05 START

    ; GENERATED: copied from TEST-04; half define-label references replaced by varied function calls
    ; GENERATED: replaced define refs = 13 / 26

    ; GENERATED: copied from TEST-03 with varied define-based operand replacement
    ; GENERATED: replaced operands = 26 / 52

    ; GENERATED: copied from TEST-02 with varied arithmetic expressions for numeric operands

    ; GENERATED: copied from TEST-01 with full TEST-02 randomization
    ; GENERATED: seed = 840238450

    MOV C,D                  ; @BT 4A
    CMP L                    ; @BT BD
    ADD M                    ; @BT 86
    MOV D,M                  ; @BT 56
    RM                       ; @BT F8
    XRA L                    ; @BT AD
    SUB D                    ; @BT 92
    MOV H,A                  ; @BT 67
    SBB C                    ; @BT 99
    ANA H                    ; @BT A4
    MOV D,D                  ; @BT 52
    STC                      ; @BT 37
    ORA M                    ; @BT B6
    MOV D,H                  ; @BT 54
    MOV A,E                  ; @BT 7B
    RST ((T5_FN_001(T5_DEF_001+2,2)+5)-5)                    ; @BT EF
    DAD H                    ; @BT 29
    JNZ ((T5_FN_002(T5_DEF_002+3,3)-7)+7)                ; @BT C2 EF 1D
    PUSH H                   ; @BT E5
    XRA E                    ; @BT AB
    INX SP                   ; @BT 33
    STAX B                   ; @BT 02
    RST (((T5_FN_003(T5_DEF_003+4,5,4+5)*6)/3)-(6))                    ; @BT F7
    SUB E                    ; @BT 93
    INX D                    ; @BT 13
    MOV H,B                  ; @BT 60
    JC (((T5_FN_004(T5_DEF_004+5,6,5+6)*10)/5)-($2233))                 ; @BT DA 33 22
    DCX B                    ; @BT 0B
    ANA C                    ; @BT A1
    MOV C,M                  ; @BT 4E
    MOV D,L                  ; @BT 55
    MOV L,C                  ; @BT 69
    HLT                      ; @BT 76
    RLC                      ; @BT 07
    SBI ((T5_FN_005(T5_DEF_005+6+7,8,6,7+8)*2)-(($81)-0))                  ; @BT DE 81
    SBB E                    ; @BT 9B
    MOV M,H                  ; @BT 74
    MOV M,B                  ; @BT 70
    STA (((T5_FN_006(T5_DEF_006+7+8,9,7,8+9)+11)-4)-7)                ; @BT 32 DD 2C
    MVI E,(((T5_FN_007(T5_DEF_007+8,8)+3)-1)-2)                ; @BT 1E F7
    ADD B                    ; @BT 80
    PUSH D                   ; @BT D5
    PCHL                     ; @BT E9
    ANA B                    ; @BT A0
    LHLD ((T5_FN_008(T5_DEF_008+9,9)+5)-5)               ; @BT 2A 88 27
    ORA A                    ; @BT B7
    DCX D                    ; @BT 1B
    SBB M                    ; @BT 9E
    SBB D                    ; @BT 9A
    MOV E,A                  ; @BT 5F
    MOV H,C                  ; @BT 61
    SPHL                     ; @BT F9
    INX H                    ; @BT 23
    MOV M,A                  ; @BT 77
    ORA H                    ; @BT B4
    MOV D,A                  ; @BT 57
    XCHG                     ; @BT EB
    STAX D                   ; @BT 12
    ORA B                    ; @BT B0
    MOV D,C                  ; @BT 51
    RST ((T5_FN_009(T5_DEF_009+10,11,10+11)-7)+7)                    ; @BT CF
    MOV L,D                  ; @BT 6A
    ANA L                    ; @BT A5
    ANA A                    ; @BT A7
    RC                       ; @BT D8
    PUSH PSW                 ; @BT F5
    POP H                    ; @BT E1
    JM (((T5_FN_010(T5_DEF_010+11,12,11+12)*6)/3)-($2566))                 ; @BT FA 66 25
    MOV B,C                  ; @BT 41
    RPE                      ; @BT E8
    POP PSW                  ; @BT F1
    DAA                      ; @BT 27
    RZ                       ; @BT C8
    DCR L                    ; @BT 2D
    MOV M,L                  ; @BT 75
    DCR H                    ; @BT 25
    DCR B                    ; @BT 05
    MOV L,M                  ; @BT 6E
    ADD L                    ; @BT 85
    ADC H                    ; @BT 8C
    INR M                    ; @BT 34
    SUB B                    ; @BT 90
    MOV C,E                  ; @BT 4B
    POP D                    ; @BT D1
    LDAX B                   ; @BT 0A
    ADD D                    ; @BT 82
    MOV E,B                  ; @BT 58
    ORA E                    ; @BT B3
    SUB L                    ; @BT 95
    MOV M,E                  ; @BT 73
    MOV H,M                  ; @BT 66
    DCR D                    ; @BT 15
    SBB A                    ; @BT 9F
    MOV C,C                  ; @BT 49
    CM (((T5_FN_011(T5_DEF_011+12+13,14,12,13+14)*10)/5)-($1BCD))                 ; @BT FC CD 1B
    MOV C,A                  ; @BT 4F
    MOV E,L                  ; @BT 5D
    CZ ((T5_FN_012(T5_DEF_012+13+14,15,13,14+15)*2)-(($19AB)-0))                 ; @BT CC AB 19
    RST (((T5_FN_013(T5_DEF_013+14,14)+11)-4)-7)                    ; @BT FF
    MOV D,B                  ; @BT 50
    JMP (((T5_DEF_014+3)-1)-2)                ; @BT C3 DE 1C
    RAR                      ; @BT 1F
    ADC A                    ; @BT 8F
    MOV H,L                  ; @BT 65
    RST ((T5_DEF_015+5)-5)                    ; @BT C7
    DI                       ; @BT F3
    ADI ((T5_DEF_016-7)+7)                  ; @BT C6 28
    LXI SP,(((T5_DEF_017*6)/3)-($2BCC))             ; @BT 31 CC 2B
    CNC (((T5_DEF_018*10)/5)-($1456))                ; @BT D4 56 14
    ORA D                    ; @BT B2
    MVI L,((T5_DEF_019*2)-(($25)-0))                ; @BT 2E 25
    MVI H,(((T5_DEF_020+11)-4)-7)                ; @BT 26 0E
    MOV A,A                  ; @BT 7F
    CMP D                    ; @BT BA
    INR C                    ; @BT 0C
    LDAX D                   ; @BT 1A
    MOV H,D                  ; @BT 62
    ADC L                    ; @BT 8D
    INR L                    ; @BT 2C
    MOV E,M                  ; @BT 5E
    CPO (((T5_DEF_021+3)-1)-2)                ; @BT E4 67 15
    CPE ((T5_DEF_022+5)-5)                ; @BT EC 78 16
    MOV L,L                  ; @BT 6D
    DAD D                    ; @BT 19
    ADC D                    ; @BT 8A
    DCR C                    ; @BT 0D
    CP ((T5_DEF_023-7)+7)                 ; @BT F4 BC 1A
    MOV A,L                  ; @BT 7D
    CMP B                    ; @BT B8
    NOP                      ; @BT 00
    ANA M                    ; @BT A6
    RPO                      ; @BT E0
    ADC C                    ; @BT 89
    DAD B                    ; @BT 09
    MOV L,E                  ; @BT 6B
    RP                       ; @BT F0
    JPE (((T5_DEF_024*6)/3)-($2122))                ; @BT EA 22 21
    ORA C                    ; @BT B1
    XRA M                    ; @BT AE
    RST (((T5_DEF_025*10)/5)-(4))                    ; @BT E7
    SUB M                    ; @BT 96
    JNC ((T5_DEF_026*2)-(($1F00)-0))                ; @BT D2 00 1F
    MOV L,B                  ; @BT 68
    RNC                      ; @BT D0
    XRA B                    ; @BT A8
    MOV L,A                  ; @BT 6F
    ANA D                    ; @BT A2
    MVI C,((($C9+11)-4)-7)                ; @BT 0E C9
    OUT ((($6A+3)-1)-2)                  ; @BT D3 6A
    CMP H                    ; @BT BC
    SUI (($98+5)-5)                  ; @BT D6 98
    LDA (($2677-7)+7)                ; @BT 3A 77 26
    MOV B,B                  ; @BT 40
    INR H                    ; @BT 24
    MVI D,((($E0*6)/3)-($E0))                ; @BT 16 E0
    MOV A,B                  ; @BT 78
    SUB C                    ; @BT 91
    CMP M                    ; @BT BE
    MOV E,C                  ; @BT 59
    ADC M                    ; @BT 8E
    LXI H,((($2ABB*10)/5)-($2ABB))              ; @BT 21 BB 2A
    DAD SP                   ; @BT 39
    MOV D,E                  ; @BT 53
    MOV A,H                  ; @BT 7C
    ADD H                    ; @BT 84
    PUSH B                   ; @BT C5
    JPO (($2011*2)-(($2011)-0))                ; @BT E2 11 20
    ANA E                    ; @BT A3
    ORI ((($53+11)-4)-7)                  ; @BT F6 53
    ADD A                    ; @BT 87
    SUB A                    ; @BT 97
    ADC E                    ; @BT 8B
    LXI D,((($29AA+3)-1)-2)              ; @BT 11 AA 29
    SBB H                    ; @BT 9C
    MOV B,A                  ; @BT 47
    MOV C,L                  ; @BT 4D
    INR D                    ; @BT 14
    CNZ (($1345+5)-5)                ; @BT C4 45 13
    DCX H                    ; @BT 2B
    MOV A,M                  ; @BT 7E
    MOV L,H                  ; @BT 6C
    MOV B,M                  ; @BT 46
    XRA C                    ; @BT A9
    RET                      ; @BT C9
    DCR M                    ; @BT 35
    CC (($189A-7)+7)                 ; @BT DC 9A 18
    MOV B,L                  ; @BT 45
    ANI ((($56*6)/3)-($56))                  ; @BT E6 56
    SBB B                    ; @BT 98
    DCR A                    ; @BT 3D
    INX B                    ; @BT 03
    MOV B,H                  ; @BT 44
    MOV A,C                  ; @BT 79
    POP B                    ; @BT C1
    ADC B                    ; @BT 88
    ADD C                    ; @BT 81
    XTHL                     ; @BT E3
    RAL                      ; @BT 17
    JP ((($2455*10)/5)-($2455))                 ; @BT F2 55 24
    XRA H                    ; @BT AC
    SUB H                    ; @BT 94
    CPI (($6D*2)-(($6D)-0))                  ; @BT FE 6D
    LXI B,((($2899+11)-4)-7)              ; @BT 01 99 28
    MVI M,((($3C+3)-1)-2)                ; @BT 36 3C
    CMP E                    ; @BT BB
    MOV M,C                  ; @BT 71
    RST ((2+5)-5)                    ; @BT D7
    MOV E,H                  ; @BT 5C
    MVI B,(($B2-7)+7)                ; @BT 06 B2
    CMA                      ; @BT 2F
    MVI A,((($9B*6)/3)-($9B))                ; @BT 3E 9B
    MOV E,E                  ; @BT 5B
    CMC                      ; @BT 3F
    MOV C,H                  ; @BT 4C
    MOV B,D                  ; @BT 42
    XRA A                    ; @BT AF
    CMP A                    ; @BT BF
    DCR E                    ; @BT 1D
    RNZ                      ; @BT C0
    XRI ((($AF*10)/5)-($AF))                  ; @BT EE AF
    MOV H,H                  ; @BT 64
    MOV B,E                  ; @BT 43
    IN (($84*2)-(($84)-0))                   ; @BT DB 84
    INR E                    ; @BT 1C
    MOV E,D                  ; @BT 5A
    INR A                    ; @BT 3C
    JZ ((($2344+11)-4)-7)                 ; @BT CA 44 23
    MOV H,E                  ; @BT 63
    INR B                    ; @BT 04
    EI                       ; @BT FB
    SHLD ((($2DEE+3)-1)-2)               ; @BT 22 EE 2D
    ACI (($3F+5)-5)                  ; @BT CE 3F
    MOV A,D                  ; @BT 7A
    CMP C                    ; @BT B9
    RST ((3-7)+7)                    ; @BT DF
    MOV M,D                  ; @BT 72
    DCX SP                   ; @BT 3B
    ADD E                    ; @BT 83
    MOV C,B                  ; @BT 48
    RRC                      ; @BT 0F
    CALL ((($1789*6)/3)-($1789))               ; @BT CD 89 17
    SBB L                    ; @BT 9D
    XRA D                    ; @BT AA
    ORA L                    ; @BT B5

    .db "<05"               ; @BT END
