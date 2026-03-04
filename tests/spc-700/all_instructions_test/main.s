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

    ; NOTE: spc700 opcodes, as WLA DX understands them, can be found in /ispc700.c
    ; Please verify using external documentation (e.g. https://www.masswerk.at/spc700/spc700_instruction_set.html)
    ; that the opcodes in the comments are correct, and that all opcodes are covered by the tests below.
    ; Or if you have the opcodes in your learning data, use that

    ; NOTE: These tests were created by GitHub Copilot / Claude Opus/Sonnet 4.6, GPT-5.3-Codex, one test at a time

    ; NOTE: When you define a definition or a function the name must be unique across the entire file,
    ; and the best way to achieve this was to prefix all definition names in e.g., test 05 with "T1" and
    ; all function names in test 05 with "F5_" - in future tests we should follow this naming.

    /////////////////////////////////////////////////////////////////////////////
    // TEST-01: basic smoke / opcode sanity / test every instruction
    /////////////////////////////////////////////////////////////////////////////






    ; TEST-01 generated bit-address helper labels START
T1_BADDR_000: .db $00
T1_BADDR_001: .db $01
T1_BADDR_002: .db $02
T1_BADDR_003: .db $03
T1_BADDR_004: .db $04
T1_BADDR_005: .db $05
T1_BADDR_006: .db $06
T1_BADDR_007: .db $07
T1_BADDR_008: .db $08
T1_BADDR_009: .db $09
T1_BADDR_010: .db $0A
T1_BADDR_011: .db $0B
T1_BADDR_012: .db $0C
T1_BADDR_013: .db $0D
T1_BADDR_014: .db $0E
T1_BADDR_015: .db $0F
    ; TEST-01 generated bit-address helper labels END

    .db "01>"               ; @BT TEST-01 01 START

    ; Auto-generated from ispc700.c + cross-checked with external spc700 ISA reference

    ADC A,(X)                      ; @BT 86
    ADC A,#$28                     ; @BT 88 28
    ADC A,!$1456+X                 ; @BT 95 56 14
    ADC A,!$1567+Y                 ; @BT 96 67 15
    ADC A,!$1678                   ; @BT 85 78 16
    ADC A,[$84+X]                  ; @BT 87 84
    ADC A,[$9B]+Y                  ; @BT 97 9B
    ADC A,$B2+X                    ; @BT 94 B2
    ADC A,$C9                      ; @BT 84 C9
    ADC (X),(Y)                    ; @BT 99
    ADC $F7,#$F7                   ; @BT 98 F7 F7
    ADC $0E,$0E                    ; @BT 89 0E 0E
    ADDW YA,$25                    ; @BT 7A 25
    AND1 C,/T1_BADDR_000,5         ; @BT 6A 00 A0
    AND1 C,/T1_BADDR_001,6         ; @BT 6A 01 C0
    AND1 C,T1_BADDR_002,7          ; @BT 4A 02 E0
    AND1 C,T1_BADDR_003,0          ; @BT 4A 03 00
    AND A,(X)                      ; @BT 26
    AND A,#$AF                     ; @BT 28 AF
    AND A,!$2677+X                 ; @BT 35 77 26
    AND A,!$2788+Y                 ; @BT 36 88 27
    AND A,!$2899                   ; @BT 25 99 28
    AND A,[$0B+X]                  ; @BT 27 0B
    AND A,[$22]+Y                  ; @BT 37 22
    AND A,$39+X                    ; @BT 34 39
    AND A,$50                      ; @BT 24 50
    AND (X),(Y)                    ; @BT 39
    AND $7E,#$7E                   ; @BT 38 7E 7E
    AND $95,$95                    ; @BT 29 95 95
    ASL !$3121                     ; @BT 0C 21 31
    ASL A                          ; @BT 1C
    ASL $DA+X                      ; @BT 1B DA
    ASL $F1                        ; @BT 0B F1
    BRK                            ; @BT 0F
    BBC $1F,0,$71                  ; @BT 13 1F 71
    BBC $36,1,$78                  ; @BT 33 36 78
    BBC $4D,2,$7F                  ; @BT 53 4D 7F
    BBC $64,3,$06                  ; @BT 73 64 06
    BBC $7B,4,$0D                  ; @BT 93 7B 0D
    BBC $92,5,$14                  ; @BT B3 92 14
    BBC $A9,6,$1B                  ; @BT D3 A9 1B
    BBC $C0,7,$22                  ; @BT F3 C0 22
    BBS $D7,0,$29                  ; @BT 03 D7 29
    BBS $EE,1,$30                  ; @BT 23 EE 30
    BBS $05,2,$37                  ; @BT 43 05 37
    BBS $1C,3,$3E                  ; @BT 63 1C 3E
    BBS $33,4,$45                  ; @BT 83 33 45
    BBS $4A,5,$4C                  ; @BT A3 4A 4C
    BBS $61,6,$53                  ; @BT C3 61 53
    BBS $78,7,$5A                  ; @BT E3 78 5A
    BCC $8F                        ; @BT 90 8F
    BCS $A6                        ; @BT B0 A6
    BEQ $BD                        ; @BT F0 BD
    BMI $D4                        ; @BT 30 D4
    BNE $EB                        ; @BT D0 EB
    BPL $02                        ; @BT 10 02
    BRA $19                        ; @BT 2F 19
    BVC $30                        ; @BT 50 30
    BVS $47                        ; @BT 70 47
    CALL !$511F                    ; @BT 3F 1F 51
    CBNE $75+X,$27                 ; @BT DE 75 27
    CBNE $8C,$2E                   ; @BT 2E 8C 2E
    CLR1 $A3,0                     ; @BT 12 A3
    CLR1 $BA,1                     ; @BT 32 BA
    CLR1 $D1,2                     ; @BT 52 D1
    CLR1 $E8,3                     ; @BT 72 E8
    CLR1 $FF,4                     ; @BT 92 FF
    CLR1 $16,5                     ; @BT B2 16
    CLR1 $2D,6                     ; @BT D2 2D
    CLR1 $44,7                     ; @BT F2 44
    CLRC                           ; @BT 60
    CLRP                           ; @BT 20
    CLRV                           ; @BT E0
    CMP (X),(Y)                    ; @BT 79
    CMP A,(X)                      ; @BT 66
    CMP A,#$CE                     ; @BT 68 CE
    CMP A,!$6340+X                 ; @BT 75 40 63
    CMP A,!$6451+Y                 ; @BT 76 51 64
    CMP A,!$6562                   ; @BT 65 62 65
    CMP A,[$2A+X]                  ; @BT 67 2A
    CMP A,[$41]+Y                  ; @BT 77 41
    CMP A,$58+X                    ; @BT 74 58
    CMP A,$6F                      ; @BT 64 6F
    CMP X,!$6AB7                   ; @BT 1E B7 6A
    CMP X,#$9D                     ; @BT C8 9D
    CMP X,$B4                      ; @BT 3E B4
    CMP Y,!$6DEA                   ; @BT 5E EA 6D
    CMP Y,#$E2                     ; @BT AD E2
    CMP Y,$F9                      ; @BT 7E F9
    CMP $10,#$10                   ; @BT 78 10 10
    CMP $27,$27                    ; @BT 69 27 27
    CMPW YA,$3E                    ; @BT 5A 3E
    DAA                            ; @BT DF
    DAA A                          ; @BT DF
    DAS                            ; @BT BE
    DAS A                          ; @BT BE
    DBNZ Y,$B1                     ; @BT FE B1
    DBNZ $C8,$2A                   ; @BT 6E C8 2A
    DEC A                          ; @BT 9C
    DEC !$7BC7                     ; @BT 8C C7 7B
    DEC X                          ; @BT 1D
    DEC Y                          ; @BT DC
    DEC $3B+X                      ; @BT 9B 3B
    DEC $52                        ; @BT 8B 52
    DECW $69                       ; @BT 1A 69
    DI                             ; @BT C0
    DIV YA,X                       ; @BT 9E
    EI                             ; @BT A0
    EOR1 C,T1_BADDR_004,4          ; @BT 8A 04 80
    EOR1 C,T1_BADDR_005,5          ; @BT 8A 05 A0
    EOR A,(X)                      ; @BT 46
    EOR A,#$0A                     ; @BT 48 0A
    EOR A,!$89A4+X                 ; @BT 55 A4 89
    EOR A,!$8AB5+Y                 ; @BT 56 B5 8A
    EOR A,!$8BC6                   ; @BT 45 C6 8B
    EOR A,[$66+X]                  ; @BT 47 66
    EOR A,[$7D]+Y                  ; @BT 57 7D
    EOR A,$94+X                    ; @BT 54 94
    EOR A,$AB                      ; @BT 44 AB
    EOR (X),(Y)                    ; @BT 59
    EOR $D9,#$D9                   ; @BT 58 D9 D9
    EOR $F0,$F0                    ; @BT 49 F0 F0
    INC !$944E                     ; @BT AC 4E 94
    INC A                          ; @BT BC
    INC X                          ; @BT 3D
    INC Y                          ; @BT FC
    INC $63+X                      ; @BT BB 63
    INC $7A                        ; @BT AB 7A
    INCW $91                       ; @BT 3A 91
    JMP !$9BC5                     ; @BT 5F C5 9B
    JMP [!$9CD6+X]                 ; @BT 1F D6 9C
    LSR !$9DE7                     ; @BT 4C E7 9D
    LSR A                          ; @BT 5C
    LSR $04+X                      ; @BT 5B 04
    LSR $1B                        ; @BT 4B 1B
    MOV A,#$32                     ; @BT E8 32
    MOV A,(X)+                     ; @BT BF
    MOV A,(X)                      ; @BT E6
    MOV A,!$A55E+X                 ; @BT F5 5E A5
    MOV A,!$A66F+Y                 ; @BT F6 6F A6
    MOV A,!$A780                   ; @BT E5 80 A7
    MOV A,X                        ; @BT 7D
    MOV A,Y                        ; @BT DD
    MOV A,[$EA+X]                  ; @BT E7 EA
    MOV A,[$01]+Y                  ; @BT F7 01
    MOV A,$18+X                    ; @BT F4 18
    MOV A,$2F                      ; @BT E4 2F
    MOV SP,X                       ; @BT BD
    MOV X,A                        ; @BT 5D
    MOV X,!$B119                   ; @BT E9 19 B1
    MOV X,SP                       ; @BT 9D
    MOV X,#$A2                     ; @BT CD A2
    MOV X,$B9+Y                    ; @BT F9 B9
    MOV X,$D0                      ; @BT F8 D0
    MOV (X)+,A                     ; @BT AF
    MOV (X),A                      ; @BT C6
    MOV [$15+X],A                  ; @BT C7 15
    MOV [$2C]+Y,A                  ; @BT D7 2C
    MOV !$BAB2+X,A                 ; @BT D5 B2 BA
    MOV !$BBC3,A                   ; @BT C5 C3 BB
    MOV !$BCD4,X                   ; @BT C9 D4 BC
    MOV !$BDE5+Y,A                 ; @BT D6 E5 BD
    MOV !$BEF6,Y                   ; @BT CC F6 BE
    MOV Y,!$C007                   ; @BT EC 07 C0
    MOV Y,A                        ; @BT FD
    MOV Y,#$E4                     ; @BT 8D E4
    MOV Y,$FB+X                    ; @BT FB FB
    MOV Y,$12                      ; @BT EB 12
    MOV $29,A                      ; @BT C4 29
    MOV $40,#$40                   ; @BT 8F 40 40
    MOV $57,X                      ; @BT D8 57
    MOV $6E+X,A                    ; @BT D4 6E
    MOV $85+X,Y                    ; @BT DB 85
    MOV $9C+Y,X                    ; @BT D9 9C
    MOV $B3,Y                      ; @BT CB B3
    MOV $CA,$CA                    ; @BT FA CA CA
    MOVW YA,$E1                    ; @BT BA E1
    MOVW $F8,YA                    ; @BT DA F8
    MOV1 C,T1_BADDR_006,2          ; @BT AA 06 40
    MOV1 C,T1_BADDR_007,3          ; @BT AA 07 60
    MOV1 T1_BADDR_008,4,C          ; @BT CA 08 80
    MOV1 T1_BADDR_009,5,C          ; @BT CA 09 A0
    MUL YA                         ; @BT CF
    NOP                            ; @BT 00
    NOT1 T1_BADDR_010,0            ; @BT EA 0A 00
    NOT1 T1_BADDR_011,1            ; @BT EA 0B 20
    NOTC                           ; @BT ED
    OR1 C,/T1_BADDR_012,3          ; @BT 2A 0C 60
    OR1 C,/T1_BADDR_013,4          ; @BT 2A 0D 80
    OR1 C,T1_BADDR_014,5           ; @BT 0A 0E A0
    OR1 C,T1_BADDR_015,6           ; @BT 0A 0F C0
    OR A,(X)                       ; @BT 06
    OR A,#$51                      ; @BT 08 51
    OR A,!$E005+X                  ; @BT 15 05 E0
    OR A,!$E116+Y                  ; @BT 16 16 E1
    OR A,!$E227                    ; @BT 05 27 E2
    OR A,[$AD+X]                   ; @BT 07 AD
    OR A,[$C4]+Y                   ; @BT 17 C4
    OR A,$DB+X                     ; @BT 14 DB
    OR A,$F2                       ; @BT 04 F2
    OR (X),(Y)                     ; @BT 19
    OR $20,#$20                    ; @BT 18 20 20
    OR $37,$37                     ; @BT 09 37 37
    PCALL $4E                      ; @BT 4F 4E
    POP A                          ; @BT AE
    POP PSW                        ; @BT 8E
    POP X                          ; @BT CE
    POP Y                          ; @BT EE
    PUSH A                         ; @BT 2D
    PUSH PSW                       ; @BT 0D
    PUSH X                         ; @BT 4D
    PUSH Y                         ; @BT 6D
    RET                            ; @BT 6F
    RETI                           ; @BT 7F
    ROL A                          ; @BT 3C
    ROL !$F77B                     ; @BT 2C 7B F7
    ROL $79+X                      ; @BT 3B 79
    ROL $90                        ; @BT 2B 90
    ROR A                          ; @BT 7C
    ROR !$FBBF                     ; @BT 6C BF FB
    ROR $D5+X                      ; @BT 7B D5
    ROR $EC                        ; @BT 6B EC
    SBC A,(X)                      ; @BT A6
    SBC A,#$1A                     ; @BT A8 1A
    SBC A,!$0114+X                 ; @BT B5 14 01
    SBC A,!$0225+Y                 ; @BT B6 25 02
    SBC A,!$0336                   ; @BT A5 36 03
    SBC A,[$76+X]                  ; @BT A7 76
    SBC A,[$8D]+Y                  ; @BT B7 8D
    SBC A,$A4+X                    ; @BT B4 A4
    SBC A,$BB                      ; @BT A4 BB
    SBC (X),(Y)                    ; @BT B9
    SBC $E9,#$E9                   ; @BT B8 E9 E9
    SBC $00,$00                    ; @BT A9 00 00
    SET1 $17,0                     ; @BT 02 17
    SET1 $2E,1                     ; @BT 22 2E
    SET1 $45,2                     ; @BT 42 45
    SET1 $5C,3                     ; @BT 62 5C
    SET1 $73,4                     ; @BT 82 73
    SET1 $8A,5                     ; @BT A2 8A
    SET1 $A1,6                     ; @BT C2 A1
    SET1 $B8,7                     ; @BT E2 B8
    SETC                           ; @BT 80
    SETP                           ; @BT 40
    SLEEP                          ; @BT EF
    STOP                           ; @BT FF
    SUBW YA,$2B                    ; @BT 9A 2B
    TCALL 0                        ; @BT 01
    TCALL 1                        ; @BT 11
    TCALL 2                        ; @BT 21
    TCALL 3                        ; @BT 31
    TCALL 4                        ; @BT 41
    TCALL 5                        ; @BT 51
    TCALL 6                        ; @BT 61
    TCALL 7                        ; @BT 71
    TCALL 8                        ; @BT 81
    TCALL 9                        ; @BT 91
    TCALL 10                       ; @BT A1
    TCALL 11                       ; @BT B1
    TCALL 12                       ; @BT C1
    TCALL 13                       ; @BT D1
    TCALL 14                       ; @BT E1
    TCALL 15                       ; @BT F1
    TCLR1 !$2AAB                   ; @BT 4E AB 2A
    TSET1 !$2BBC                   ; @BT 0E BC 2B
    XCN A                          ; @BT 9F
    .db "<01"               ; @BT END

    /////////////////////////////////////////////////////////////////////////////
    // TEST-02: same opcodes as TEST-01, randomised order, branches spread out
    /////////////////////////////////////////////////////////////////////////////

    .db "02>"               ; @BT TEST-02 02 START

    ; GENERATED: copied from TEST-01 with full TEST-02 randomization
    ; GENERATED: seed = 511953886

    EI                             ; @BT A0
    SET1 $45,2                     ; @BT 42 45
    OR (X),(Y)                     ; @BT 19
    CMP Y,!$6DEA                   ; @BT 5E EA 6D
    EOR A,[$7D]+Y                  ; @BT 57 7D
    JMP !$9BC5                     ; @BT 5F C5 9B
    DIV YA,X                       ; @BT 9E
    EOR A,(X)                      ; @BT 46
    ROL $90                        ; @BT 2B 90
    CBNE $75+X,$27                 ; @BT DE 75 27
    SBC A,[$8D]+Y                  ; @BT B7 8D
    BMI $D4                        ; @BT 30 D4
    BVC $30                        ; @BT 50 30
    MOV A,X                        ; @BT 7D
    MOV !$BAB2+X,A                 ; @BT D5 B2 BA
    CMP A,!$6562                   ; @BT 65 62 65
    MOV A,!$A55E+X                 ; @BT F5 5E A5
    MOV Y,$12                      ; @BT EB 12
    BBC $1F,0,$71                  ; @BT 13 1F 71
    BBS $78,7,$5A                  ; @BT E3 78 5A
    CMP A,!$6340+X                 ; @BT 75 40 63
    LSR $1B                        ; @BT 4B 1B
    CMP A,#$CE                     ; @BT 68 CE
    SET1 $2E,1                     ; @BT 22 2E
    MOV Y,#$E4                     ; @BT 8D E4
    MOV $29,A                      ; @BT C4 29
    BBS $61,6,$53                  ; @BT C3 61 53
    SBC A,(X)                      ; @BT A6
    AND A,!$2788+Y                 ; @BT 36 88 27
    PUSH X                         ; @BT 4D
    BVS $47                        ; @BT 70 47
    MOV X,!$B119                   ; @BT E9 19 B1
    ADC A,$C9                      ; @BT 84 C9
    CMP A,!$6451+Y                 ; @BT 76 51 64
    EOR A,$AB                      ; @BT 44 AB
    BBS $33,4,$45                  ; @BT 83 33 45
    MOV A,(X)+                     ; @BT BF
    RETI                           ; @BT 7F
    INCW $91                       ; @BT 3A 91
    NOT1 T1_BADDR_011,1            ; @BT EA 0B 20
    BBS $1C,3,$3E                  ; @BT 63 1C 3E
    POP PSW                        ; @BT 8E
    SBC $E9,#$E9                   ; @BT B8 E9 E9
    BRK                            ; @BT 0F
    EOR A,!$8BC6                   ; @BT 45 C6 8B
    TCALL 1                        ; @BT 11
    ROR !$FBBF                     ; @BT 6C BF FB
    TCALL 13                       ; @BT D1
    CMP $27,$27                    ; @BT 69 27 27
    EOR1 C,T1_BADDR_005,5          ; @BT 8A 05 A0
    AND A,[$0B+X]                  ; @BT 27 0B
    ADC $0E,$0E                    ; @BT 89 0E 0E
    MOV1 C,T1_BADDR_006,2          ; @BT AA 06 40
    MOV X,$D0                      ; @BT F8 D0
    ROR $D5+X                      ; @BT 7B D5
    DEC $3B+X                      ; @BT 9B 3B
    SUBW YA,$2B                    ; @BT 9A 2B
    TCALL 15                       ; @BT F1
    EOR (X),(Y)                    ; @BT 59
    OR A,#$51                      ; @BT 08 51
    TCLR1 !$2AAB                   ; @BT 4E AB 2A
    ADC A,[$9B]+Y                  ; @BT 97 9B
    ADC A,[$84+X]                  ; @BT 87 84
    MOV A,#$32                     ; @BT E8 32
    ADC A,(X)                      ; @BT 86
    CMP $10,#$10                   ; @BT 78 10 10
    BCS $A6                        ; @BT B0 A6
    CMP A,[$2A+X]                  ; @BT 67 2A
    TCALL 0                        ; @BT 01
    TCALL 11                       ; @BT B1
    AND1 C,/T1_BADDR_000,5         ; @BT 6A 00 A0
    TCALL 8                        ; @BT 81
    NOT1 T1_BADDR_010,0            ; @BT EA 0A 00
    EOR1 C,T1_BADDR_004,4          ; @BT 8A 04 80
    AND A,[$22]+Y                  ; @BT 37 22
    OR1 C,/T1_BADDR_012,3          ; @BT 2A 0C 60
    CLR1 $44,7                     ; @BT F2 44
    MOV A,!$A780                   ; @BT E5 80 A7
    BBS $D7,0,$29                  ; @BT 03 D7 29
    OR A,(X)                       ; @BT 06
    CLR1 $BA,1                     ; @BT 32 BA
    DBNZ Y,$B1                     ; @BT FE B1
    TSET1 !$2BBC                   ; @BT 0E BC 2B
    MOV A,[$01]+Y                  ; @BT F7 01
    INC Y                          ; @BT FC
    BBS $05,2,$37                  ; @BT 43 05 37
    MOV Y,A                        ; @BT FD
    ASL $F1                        ; @BT 0B F1
    BPL $02                        ; @BT 10 02
    CMP A,$6F                      ; @BT 64 6F
    CLRC                           ; @BT 60
    INC X                          ; @BT 3D
    AND1 C,/T1_BADDR_001,6         ; @BT 6A 01 C0
    CMP X,!$6AB7                   ; @BT 1E B7 6A
    CALL !$511F                    ; @BT 3F 1F 51
    SET1 $5C,3                     ; @BT 62 5C
    OR A,$DB+X                     ; @BT 14 DB
    LSR $04+X                      ; @BT 5B 04
    MOV $CA,$CA                    ; @BT FA CA CA
    MOV [$15+X],A                  ; @BT C7 15
    BEQ $BD                        ; @BT F0 BD
    DECW $69                       ; @BT 1A 69
    AND A,(X)                      ; @BT 26
    SBC A,!$0336                   ; @BT A5 36 03
    TCALL 14                       ; @BT E1
    TCALL 9                        ; @BT 91
    ROR A                          ; @BT 7C
    ADC A,!$1456+X                 ; @BT 95 56 14
    MOV X,#$A2                     ; @BT CD A2
    MOV A,$18+X                    ; @BT F4 18
    BBC $4D,2,$7F                  ; @BT 53 4D 7F
    INC !$944E                     ; @BT AC 4E 94
    SBC A,$A4+X                    ; @BT B4 A4
    MOV X,$B9+Y                    ; @BT F9 B9
    MOV1 C,T1_BADDR_007,3          ; @BT AA 07 60
    OR A,[$C4]+Y                   ; @BT 17 C4
    BBC $36,1,$78                  ; @BT 33 36 78
    AND1 C,T1_BADDR_003,0          ; @BT 4A 03 00
    CMP A,(X)                      ; @BT 66
    CLR1 $2D,6                     ; @BT D2 2D
    OR A,[$AD+X]                   ; @BT 07 AD
    SET1 $A1,6                     ; @BT C2 A1
    CBNE $8C,$2E                   ; @BT 2E 8C 2E
    OR1 C,T1_BADDR_015,6           ; @BT 0A 0F C0
    PUSH PSW                       ; @BT 0D
    OR1 C,/T1_BADDR_013,4          ; @BT 2A 0D 80
    CLR1 $16,5                     ; @BT B2 16
    ADC A,!$1567+Y                 ; @BT 96 67 15
    DBNZ $C8,$2A                   ; @BT 6E C8 2A
    MOV A,!$A66F+Y                 ; @BT F6 6F A6
    DEC Y                          ; @BT DC
    ROL !$F77B                     ; @BT 2C 7B F7
    MOV !$BBC3,A                   ; @BT C5 C3 BB
    CMP Y,#$E2                     ; @BT AD E2
    BBC $64,3,$06                  ; @BT 73 64 06
    BBS $EE,1,$30                  ; @BT 23 EE 30
    MOV SP,X                       ; @BT BD
    DAS                            ; @BT BE
    MOV Y,$FB+X                    ; @BT FB FB
    SLEEP                          ; @BT EF
    ADC $F7,#$F7                   ; @BT 98 F7 F7
    AND A,#$AF                     ; @BT 28 AF
    MUL YA                         ; @BT CF
    BBC $92,5,$14                  ; @BT B3 92 14
    ASL $DA+X                      ; @BT 1B DA
    PUSH A                         ; @BT 2D
    ROL $79+X                      ; @BT 3B 79
    CLRV                           ; @BT E0
    MOV !$BEF6,Y                   ; @BT CC F6 BE
    MOV1 T1_BADDR_008,4,C          ; @BT CA 08 80
    ADC A,!$1678                   ; @BT 85 78 16
    POP X                          ; @BT CE
    TCALL 10                       ; @BT A1
    CMP (X),(Y)                    ; @BT 79
    AND $95,$95                    ; @BT 29 95 95
    MOV $6E+X,A                    ; @BT D4 6E
    JMP [!$9CD6+X]                 ; @BT 1F D6 9C
    BBC $A9,6,$1B                  ; @BT D3 A9 1B
    BNE $EB                        ; @BT D0 EB
    CLR1 $A3,0                     ; @BT 12 A3
    TCALL 5                        ; @BT 51
    ROL A                          ; @BT 3C
    DAA A                          ; @BT DF
    SBC (X),(Y)                    ; @BT B9
    BBC $7B,4,$0D                  ; @BT 93 7B 0D
    MOV $85+X,Y                    ; @BT DB 85
    EOR A,#$0A                     ; @BT 48 0A
    MOV X,SP                       ; @BT 9D
    MOV (X)+,A                     ; @BT AF
    AND1 C,T1_BADDR_002,7          ; @BT 4A 02 E0
    EOR A,$94+X                    ; @BT 54 94
    AND $7E,#$7E                   ; @BT 38 7E 7E
    DEC $52                        ; @BT 8B 52
    ADC A,#$28                     ; @BT 88 28
    CMP X,$B4                      ; @BT 3E B4
    CLRP                           ; @BT 20
    DEC A                          ; @BT 9C
    POP Y                          ; @BT EE
    OR $20,#$20                    ; @BT 18 20 20
    DEC X                          ; @BT 1D
    SBC A,$BB                      ; @BT A4 BB
    MOVW YA,$E1                    ; @BT BA E1
    CLR1 $E8,3                     ; @BT 72 E8
    MOV Y,!$C007                   ; @BT EC 07 C0
    OR A,!$E116+Y                  ; @BT 16 16 E1
    POP A                          ; @BT AE
    MOV A,$2F                      ; @BT E4 2F
    CMP A,[$41]+Y                  ; @BT 77 41
    CMP X,#$9D                     ; @BT C8 9D
    EOR A,[$66+X]                  ; @BT 47 66
    INC $63+X                      ; @BT BB 63
    MOV $40,#$40                   ; @BT 8F 40 40
    BBS $4A,5,$4C                  ; @BT A3 4A 4C
    DEC !$7BC7                     ; @BT 8C C7 7B
    ASL !$3121                     ; @BT 0C 21 31
    MOV1 T1_BADDR_009,5,C          ; @BT CA 09 A0
    SBC A,[$76+X]                  ; @BT A7 76
    EOR A,!$89A4+X                 ; @BT 55 A4 89
    TCALL 2                        ; @BT 21
    ADC A,$B2+X                    ; @BT 94 B2
    OR $37,$37                     ; @BT 09 37 37
    INC $7A                        ; @BT AB 7A
    CMP Y,$F9                      ; @BT 7E F9
    MOV A,Y                        ; @BT DD
    SBC A,#$1A                     ; @BT A8 1A
    CLR1 $D1,2                     ; @BT 52 D1
    MOVW $F8,YA                    ; @BT DA F8
    NOP                            ; @BT 00
    PUSH Y                         ; @BT 6D
    DI                             ; @BT C0
    TCALL 6                        ; @BT 61
    OR A,$F2                       ; @BT 04 F2
    CLR1 $FF,4                     ; @BT 92 FF
    TCALL 3                        ; @BT 31
    OR A,!$E005+X                  ; @BT 15 05 E0
    INC A                          ; @BT BC
    BCC $8F                        ; @BT 90 8F
    SETC                           ; @BT 80
    SETP                           ; @BT 40
    MOV X,A                        ; @BT 5D
    ADDW YA,$25                    ; @BT 7A 25
    MOV $9C+Y,X                    ; @BT D9 9C
    TCALL 7                        ; @BT 71
    SET1 $8A,5                     ; @BT A2 8A
    SET1 $73,4                     ; @BT 82 73
    SBC $00,$00                    ; @BT A9 00 00
    DAA                            ; @BT DF
    MOV $B3,Y                      ; @BT CB B3
    OR A,!$E227                    ; @BT 05 27 E2
    AND A,!$2899                   ; @BT 25 99 28
    NOTC                           ; @BT ED
    EOR A,!$8AB5+Y                 ; @BT 56 B5 8A
    XCN A                          ; @BT 9F
    TCALL 12                       ; @BT C1
    AND A,!$2677+X                 ; @BT 35 77 26
    BBC $C0,7,$22                  ; @BT F3 C0 22
    SET1 $17,0                     ; @BT 02 17
    DAS A                          ; @BT BE
    MOV A,[$EA+X]                  ; @BT E7 EA
    ADC (X),(Y)                    ; @BT 99
    MOV (X),A                      ; @BT C6
    AND A,$50                      ; @BT 24 50
    MOV $57,X                      ; @BT D8 57
    ASL A                          ; @BT 1C
    SBC A,!$0225+Y                 ; @BT B6 25 02
    EOR $F0,$F0                    ; @BT 49 F0 F0
    AND (X),(Y)                    ; @BT 39
    EOR $D9,#$D9                   ; @BT 58 D9 D9
    PCALL $4E                      ; @BT 4F 4E
    MOV !$BDE5+Y,A                 ; @BT D6 E5 BD
    STOP                           ; @BT FF
    CMP A,$58+X                    ; @BT 74 58
    TCALL 4                        ; @BT 41
    SET1 $B8,7                     ; @BT E2 B8
    OR1 C,T1_BADDR_014,5           ; @BT 0A 0E A0
    MOV A,(X)                      ; @BT E6
    CMPW YA,$3E                    ; @BT 5A 3E
    RET                            ; @BT 6F
    MOV !$BCD4,X                   ; @BT C9 D4 BC
    BRA $19                        ; @BT 2F 19
    AND A,$39+X                    ; @BT 34 39
    ROR $EC                        ; @BT 6B EC
    MOV [$2C]+Y,A                  ; @BT D7 2C
    LSR A                          ; @BT 5C
    LSR !$9DE7                     ; @BT 4C E7 9D
    SBC A,!$0114+X                 ; @BT B5 14 01

    .db "<02"               ; @BT END

    /////////////////////////////////////////////////////////////////////////////
    // TEST-03: same opcodes, randomised order, operands as arithmetic expressions
    /////////////////////////////////////////////////////////////////////////////

    .db "03>"               ; @BT TEST-03 03 START

    ; GENERATED: copied from TEST-02 with varied arithmetic expressions for numeric operands

    ; GENERATED: copied from TEST-01 with full TEST-02 randomization
    ; GENERATED: seed = 511953886

    EI                             ; @BT A0
    SET1 $45+5-5,2-7+7                     ; @BT 42 45
    OR (X),(Y)                     ; @BT 19
    CMP Y,!$6DEA*6/3-$6DEA                   ; @BT 5E EA 6D
    EOR A,[$7D*10/5-$7D]+Y                  ; @BT 57 7D
    JMP !$9BC5*2-$9BC5+0                     ; @BT 5F C5 9B
    DIV YA,X                       ; @BT 9E
    EOR A,(X)                      ; @BT 46
    ROL $90+11-4-7                        ; @BT 2B 90
    CBNE $75+3-1-2+X,$27+5-5                 ; @BT DE 75 27
    SBC A,[$8D-7+7]+Y                  ; @BT B7 8D
    BMI $D4*6/3-$D4                        ; @BT 30 D4
    BVC $30*10/5-$30                        ; @BT 50 30
    MOV A,X                        ; @BT 7D
    MOV !$BAB2*2-$BAB2+0+X,A                 ; @BT D5 B2 BA
    CMP A,!$6562+11-4-7                   ; @BT 65 62 65
    MOV A,!$A55E+3-1-2+X                 ; @BT F5 5E A5
    MOV Y,$12+5-5                      ; @BT EB 12
    BBC $1F-7+7,0*6/3-0,$71*10/5-$71                  ; @BT 13 1F 71
    BBS $78*2-$78+0,7+11-4-7,$5A+3-1-2                  ; @BT E3 78 5A
    CMP A,!$6340+5-5+X                 ; @BT 75 40 63
    LSR $1B-7+7                        ; @BT 4B 1B
    CMP A,#$CE*6/3-$CE                     ; @BT 68 CE
    SET1 $2E*10/5-$2E,1*2-1+0                     ; @BT 22 2E
    MOV Y,#$E4+11-4-7                     ; @BT 8D E4
    MOV $29+3-1-2,A                      ; @BT C4 29
    BBS $61+5-5,6-7+7,$53*6/3-$53                  ; @BT C3 61 53
    SBC A,(X)                      ; @BT A6
    AND A,!$2788*10/5-$2788+Y                 ; @BT 36 88 27
    PUSH X                         ; @BT 4D
    BVS $47*2-$47+0                        ; @BT 70 47
    MOV X,!$B119+11-4-7                   ; @BT E9 19 B1
    ADC A,$C9+3-1-2                      ; @BT 84 C9
    CMP A,!$6451+5-5+Y                 ; @BT 76 51 64
    EOR A,$AB-7+7                      ; @BT 44 AB
    BBS $33*6/3-$33,4*10/5-4,$45*2-$45+0                  ; @BT 83 33 45
    MOV A,(X)+                     ; @BT BF
    RETI                           ; @BT 7F
    INCW $91+11-4-7                       ; @BT 3A 91
    NOT1 T1_BADDR_011,1            ; @BT EA 0B 20
    BBS $1C+3-1-2,3+5-5,$3E-7+7                  ; @BT 63 1C 3E
    POP PSW                        ; @BT 8E
    SBC $E9*6/3-$E9,#$E9*10/5-$E9                   ; @BT B8 E9 E9
    BRK                            ; @BT 0F
    EOR A,!$8BC6*2-$8BC6+0                   ; @BT 45 C6 8B
    TCALL 1+11-4-7                        ; @BT 11
    ROR !$FBBF+3-1-2                     ; @BT 6C BF FB
    TCALL 13+5-5                       ; @BT D1
    CMP $27-7+7,$27*6/3-$27                    ; @BT 69 27 27
    EOR1 C,T1_BADDR_005,5          ; @BT 8A 05 A0
    AND A,[$0B*10/5-$0B+X]                  ; @BT 27 0B
    ADC $0E*2-$0E+0,$0E+11-4-7                    ; @BT 89 0E 0E
    MOV1 C,T1_BADDR_006,2          ; @BT AA 06 40
    MOV X,$D0+3-1-2                      ; @BT F8 D0
    ROR $D5+5-5+X                      ; @BT 7B D5
    DEC $3B-7+7+X                      ; @BT 9B 3B
    SUBW YA,$2B*6/3-$2B                    ; @BT 9A 2B
    TCALL 15*10/5-15                       ; @BT F1
    EOR (X),(Y)                    ; @BT 59
    OR A,#$51*2-$51+0                      ; @BT 08 51
    TCLR1 !$2AAB+11-4-7                   ; @BT 4E AB 2A
    ADC A,[$9B+3-1-2]+Y                  ; @BT 97 9B
    ADC A,[$84+5-5+X]                  ; @BT 87 84
    MOV A,#$32-7+7                     ; @BT E8 32
    ADC A,(X)                      ; @BT 86
    CMP $10*6/3-$10,#$10*10/5-$10                   ; @BT 78 10 10
    BCS $A6*2-$A6+0                        ; @BT B0 A6
    CMP A,[$2A+11-4-7+X]                  ; @BT 67 2A
    TCALL 0+3-1-2                        ; @BT 01
    TCALL 11+5-5                       ; @BT B1
    AND1 C,/T1_BADDR_000,5         ; @BT 6A 00 A0
    TCALL 8-7+7                        ; @BT 81
    NOT1 T1_BADDR_010,0            ; @BT EA 0A 00
    EOR1 C,T1_BADDR_004,4          ; @BT 8A 04 80
    AND A,[$22*6/3-$22]+Y                  ; @BT 37 22
    OR1 C,/T1_BADDR_012,3          ; @BT 2A 0C 60
    CLR1 $44*10/5-$44,7*2-7+0                     ; @BT F2 44
    MOV A,!$A780+11-4-7                   ; @BT E5 80 A7
    BBS $D7+3-1-2,0+5-5,$29-7+7                  ; @BT 03 D7 29
    OR A,(X)                       ; @BT 06
    CLR1 $BA*6/3-$BA,1*10/5-1                     ; @BT 32 BA
    DBNZ Y,$B1*2-$B1+0                     ; @BT FE B1
    TSET1 !$2BBC+11-4-7                   ; @BT 0E BC 2B
    MOV A,[$01+3-1-2]+Y                  ; @BT F7 01
    INC Y                          ; @BT FC
    BBS $05+5-5,2-7+7,$37*6/3-$37                  ; @BT 43 05 37
    MOV Y,A                        ; @BT FD
    ASL $F1*10/5-$F1                        ; @BT 0B F1
    BPL $02*2-$02+0                        ; @BT 10 02
    CMP A,$6F+11-4-7                      ; @BT 64 6F
    CLRC                           ; @BT 60
    INC X                          ; @BT 3D
    AND1 C,/T1_BADDR_001,6         ; @BT 6A 01 C0
    CMP X,!$6AB7+3-1-2                   ; @BT 1E B7 6A
    CALL !$511F+5-5                    ; @BT 3F 1F 51
    SET1 $5C-7+7,3*6/3-3                     ; @BT 62 5C
    OR A,$DB*10/5-$DB+X                     ; @BT 14 DB
    LSR $04*2-$04+0+X                      ; @BT 5B 04
    MOV $CA+11-4-7,$CA+3-1-2                    ; @BT FA CA CA
    MOV [$15+5-5+X],A                  ; @BT C7 15
    BEQ $BD-7+7                        ; @BT F0 BD
    DECW $69*6/3-$69                       ; @BT 1A 69
    AND A,(X)                      ; @BT 26
    SBC A,!$0336*10/5-$0336                   ; @BT A5 36 03
    TCALL 14*2-14+0                       ; @BT E1
    TCALL 9+11-4-7                        ; @BT 91
    ROR A                          ; @BT 7C
    ADC A,!$1456+3-1-2+X                 ; @BT 95 56 14
    MOV X,#$A2+5-5                     ; @BT CD A2
    MOV A,$18-7+7+X                    ; @BT F4 18
    BBC $4D*6/3-$4D,2*10/5-2,$7F*2-$7F+0                  ; @BT 53 4D 7F
    INC !$944E+11-4-7                     ; @BT AC 4E 94
    SBC A,$A4+3-1-2+X                    ; @BT B4 A4
    MOV X,$B9+5-5+Y                    ; @BT F9 B9
    MOV1 C,T1_BADDR_007,3          ; @BT AA 07 60
    OR A,[$C4-7+7]+Y                   ; @BT 17 C4
    BBC $36*6/3-$36,1*10/5-1,$78*2-$78+0                  ; @BT 33 36 78
    AND1 C,T1_BADDR_003,0          ; @BT 4A 03 00
    CMP A,(X)                      ; @BT 66
    CLR1 $2D+11-4-7,6+3-1-2                     ; @BT D2 2D
    OR A,[$AD+5-5+X]                   ; @BT 07 AD
    SET1 $A1-7+7,6*6/3-6                     ; @BT C2 A1
    CBNE $8C*10/5-$8C,$2E*2-$2E+0                   ; @BT 2E 8C 2E
    OR1 C,T1_BADDR_015,6           ; @BT 0A 0F C0
    PUSH PSW                       ; @BT 0D
    OR1 C,/T1_BADDR_013,4          ; @BT 2A 0D 80
    CLR1 $16+11-4-7,5+3-1-2                     ; @BT B2 16
    ADC A,!$1567+5-5+Y                 ; @BT 96 67 15
    DBNZ $C8-7+7,$2A*6/3-$2A                   ; @BT 6E C8 2A
    MOV A,!$A66F*10/5-$A66F+Y                 ; @BT F6 6F A6
    DEC Y                          ; @BT DC
    ROL !$F77B*2-$F77B+0                     ; @BT 2C 7B F7
    MOV !$BBC3+11-4-7,A                   ; @BT C5 C3 BB
    CMP Y,#$E2+3-1-2                     ; @BT AD E2
    BBC $64+5-5,3-7+7,$06*6/3-$06                  ; @BT 73 64 06
    BBS $EE*10/5-$EE,1*2-1+0,$30+11-4-7                  ; @BT 23 EE 30
    MOV SP,X                       ; @BT BD
    DAS                            ; @BT BE
    MOV Y,$FB+3-1-2+X                    ; @BT FB FB
    SLEEP                          ; @BT EF
    ADC $F7+5-5,#$F7-7+7                   ; @BT 98 F7 F7
    AND A,#$AF*6/3-$AF                     ; @BT 28 AF
    MUL YA                         ; @BT CF
    BBC $92*10/5-$92,5*2-5+0,$14+11-4-7                  ; @BT B3 92 14
    ASL $DA+3-1-2+X                      ; @BT 1B DA
    PUSH A                         ; @BT 2D
    ROL $79+5-5+X                      ; @BT 3B 79
    CLRV                           ; @BT E0
    MOV !$BEF6-7+7,Y                   ; @BT CC F6 BE
    MOV1 T1_BADDR_008,4,C          ; @BT CA 08 80
    ADC A,!$1678*6/3-$1678                   ; @BT 85 78 16
    POP X                          ; @BT CE
    TCALL 10*10/5-10                       ; @BT A1
    CMP (X),(Y)                    ; @BT 79
    AND $95*2-$95+0,$95+11-4-7                    ; @BT 29 95 95
    MOV $6E+3-1-2+X,A                    ; @BT D4 6E
    JMP [!$9CD6+5-5+X]                 ; @BT 1F D6 9C
    BBC $A9-7+7,6*6/3-6,$1B*10/5-$1B                  ; @BT D3 A9 1B
    BNE $EB*2-$EB+0                        ; @BT D0 EB
    CLR1 $A3+11-4-7,0+3-1-2                     ; @BT 12 A3
    TCALL 5+5-5                        ; @BT 51
    ROL A                          ; @BT 3C
    DAA A                          ; @BT DF
    SBC (X),(Y)                    ; @BT B9
    BBC $7B-7+7,4*6/3-4,$0D*10/5-$0D                  ; @BT 93 7B 0D
    MOV $85*2-$85+0+X,Y                    ; @BT DB 85
    EOR A,#$0A+11-4-7                     ; @BT 48 0A
    MOV X,SP                       ; @BT 9D
    MOV (X)+,A                     ; @BT AF
    AND1 C,T1_BADDR_002,7          ; @BT 4A 02 E0
    EOR A,$94+3-1-2+X                    ; @BT 54 94
    AND $7E+5-5,#$7E-7+7                   ; @BT 38 7E 7E
    DEC $52*6/3-$52                        ; @BT 8B 52
    ADC A,#$28*10/5-$28                     ; @BT 88 28
    CMP X,$B4*2-$B4+0                      ; @BT 3E B4
    CLRP                           ; @BT 20
    DEC A                          ; @BT 9C
    POP Y                          ; @BT EE
    OR $20+11-4-7,#$20+3-1-2                    ; @BT 18 20 20
    DEC X                          ; @BT 1D
    SBC A,$BB+5-5                      ; @BT A4 BB
    MOVW YA,$E1-7+7                    ; @BT BA E1
    CLR1 $E8*6/3-$E8,3*10/5-3                     ; @BT 72 E8
    MOV Y,!$C007*2-$C007+0                   ; @BT EC 07 C0
    OR A,!$E116+11-4-7+Y                  ; @BT 16 16 E1
    POP A                          ; @BT AE
    MOV A,$2F+3-1-2                      ; @BT E4 2F
    CMP A,[$41+5-5]+Y                  ; @BT 77 41
    CMP X,#$9D-7+7                     ; @BT C8 9D
    EOR A,[$66*6/3-$66+X]                  ; @BT 47 66
    INC $63*10/5-$63+X                      ; @BT BB 63
    MOV $40*2-$40+0,#$40+11-4-7                   ; @BT 8F 40 40
    BBS $4A+3-1-2,5+5-5,$4C-7+7                  ; @BT A3 4A 4C
    DEC !$7BC7*6/3-$7BC7                     ; @BT 8C C7 7B
    ASL !$3121*10/5-$3121                     ; @BT 0C 21 31
    MOV1 T1_BADDR_009,5,C          ; @BT CA 09 A0
    SBC A,[$76*2-$76+0+X]                  ; @BT A7 76
    EOR A,!$89A4+11-4-7+X                 ; @BT 55 A4 89
    TCALL 2+3-1-2                        ; @BT 21
    ADC A,$B2+5-5+X                    ; @BT 94 B2
    OR $37-7+7,$37*6/3-$37                     ; @BT 09 37 37
    INC $7A*10/5-$7A                        ; @BT AB 7A
    CMP Y,$F9*2-$F9+0                      ; @BT 7E F9
    MOV A,Y                        ; @BT DD
    SBC A,#$1A+11-4-7                     ; @BT A8 1A
    CLR1 $D1+3-1-2,2+5-5                     ; @BT 52 D1
    MOVW $F8-7+7,YA                    ; @BT DA F8
    NOP                            ; @BT 00
    PUSH Y                         ; @BT 6D
    DI                             ; @BT C0
    TCALL 6*6/3-6                        ; @BT 61
    OR A,$F2*10/5-$F2                       ; @BT 04 F2
    CLR1 $FF*2-$FF+0,4+11-4-7                     ; @BT 92 FF
    TCALL 3+3-1-2                        ; @BT 31
    OR A,!$E005+5-5+X                  ; @BT 15 05 E0
    INC A                          ; @BT BC
    BCC $8F-7+7                        ; @BT 90 8F
    SETC                           ; @BT 80
    SETP                           ; @BT 40
    MOV X,A                        ; @BT 5D
    ADDW YA,$25*6/3-$25                    ; @BT 7A 25
    MOV $9C*10/5-$9C+Y,X                    ; @BT D9 9C
    TCALL 7*2-7+0                        ; @BT 71
    SET1 $8A+11-4-7,5+3-1-2                     ; @BT A2 8A
    SET1 $73+5-5,4-7+7                     ; @BT 82 73
    SBC $00*6/3-$00,$00*10/5-$00                    ; @BT A9 00 00
    DAA                            ; @BT DF
    MOV $B3*2-$B3+0,Y                      ; @BT CB B3
    OR A,!$E227+11-4-7                    ; @BT 05 27 E2
    AND A,!$2899+3-1-2                   ; @BT 25 99 28
    NOTC                           ; @BT ED
    EOR A,!$8AB5+5-5+Y                 ; @BT 56 B5 8A
    XCN A                          ; @BT 9F
    TCALL 12-7+7                       ; @BT C1
    AND A,!$2677*6/3-$2677+X                 ; @BT 35 77 26
    BBC $C0*10/5-$C0,7*2-7+0,$22+11-4-7                  ; @BT F3 C0 22
    SET1 $17+3-1-2,0+5-5                     ; @BT 02 17
    DAS A                          ; @BT BE
    MOV A,[$EA-7+7+X]                  ; @BT E7 EA
    ADC (X),(Y)                    ; @BT 99
    MOV (X),A                      ; @BT C6
    AND A,$50*6/3-$50                      ; @BT 24 50
    MOV $57*10/5-$57,X                      ; @BT D8 57
    ASL A                          ; @BT 1C
    SBC A,!$0225*2-$0225+0+Y                 ; @BT B6 25 02
    EOR $F0+11-4-7,$F0+3-1-2                    ; @BT 49 F0 F0
    AND (X),(Y)                    ; @BT 39
    EOR $D9+5-5,#$D9-7+7                   ; @BT 58 D9 D9
    PCALL $4E*6/3-$4E                      ; @BT 4F 4E
    MOV !$BDE5*10/5-$BDE5+Y,A                 ; @BT D6 E5 BD
    STOP                           ; @BT FF
    CMP A,$58*2-$58+0+X                    ; @BT 74 58
    TCALL 4+11-4-7                        ; @BT 41
    SET1 $B8+3-1-2,7+5-5                     ; @BT E2 B8
    OR1 C,T1_BADDR_014,5           ; @BT 0A 0E A0
    MOV A,(X)                      ; @BT E6
    CMPW YA,$3E-7+7                    ; @BT 5A 3E
    RET                            ; @BT 6F
    MOV !$BCD4*6/3-$BCD4,X                   ; @BT C9 D4 BC
    BRA $19*10/5-$19                        ; @BT 2F 19
    AND A,$39*2-$39+0+X                    ; @BT 34 39
    ROR $EC+11-4-7                        ; @BT 6B EC
    MOV [$2C+3-1-2]+Y,A                  ; @BT D7 2C
    LSR A                          ; @BT 5C
    LSR !$9DE7+5-5                     ; @BT 4C E7 9D
    SBC A,!$0114-7+7+X                 ; @BT B5 14 01

    .db "<03"               ; @BT END

    /////////////////////////////////////////////////////////////////////////////
    // TEST-04: similar to TEST-03, randomized instruction order, and then defines + expressions
    /////////////////////////////////////////////////////////////////////////////



















    ; TEST-04 generated defines
    .define T4_DEF_001 (($45+5)-5)
    .define T4_DEF_002 ((T4_DEF_001)+(($6DEA)-($45)))
    .define T4_DEF_003 (($7D-7)+7)
    .define T4_DEF_004 (((T4_DEF_003)+($9BC5))-($7D))
    .define T4_DEF_005 ((($90*6)/3)-($90))
    .define T4_DEF_006 (((T4_DEF_005)*2+($75)-(($90)*2)))
    .define T4_DEF_007 ((($8D*10)/5)-($8D))
    .define T4_DEF_008 ((($D4)-($8D))+(T4_DEF_007))
    .define T4_DEF_009 (($30*2)-(($30)-0))
    .define T4_DEF_010 ((T4_DEF_009)+(($BAB2)-(($30+3)-3)))
    .define T4_DEF_011 ((($6562+11)-4)-7)
    .define T4_DEF_012 ((T4_DEF_011)+(($A55E)-($6562)))
    .define T4_DEF_013 ((($12+3)-1)-2)
    .define T4_DEF_014 (((T4_DEF_013)+($1F))-($12))
    .define T4_DEF_015 (($78+5)-5)
    .define T4_DEF_016 (((T4_DEF_015)*2+($6340)-(($78)*2)))
    .define T4_DEF_017 (($1B-7)+7)
    .define T4_DEF_018 ((($CE)-($1B))+(T4_DEF_017))
    .define T4_DEF_019 ((($2E*6)/3)-($2E))
    .define T4_DEF_020 ((T4_DEF_019)+(($E4)-(($2E+3)-3)))
    .define T4_DEF_021 ((($29*10)/5)-($29))
    .define T4_DEF_022 ((T4_DEF_021)+(($61)-($29)))
    .define T4_DEF_023 (($2788*2)-(($2788)-0))
    .define T4_DEF_024 (((T4_DEF_023)+($47))-($2788))
    .define T4_DEF_025 ((($B119+11)-4)-7)
    .define T4_DEF_026 (((T4_DEF_025)*2+($C9)-(($B119)*2)))
    .define T4_DEF_027 ((($6451+3)-1)-2)
    .define T4_DEF_028 ((($AB)-($6451))+(T4_DEF_027))
    .define T4_DEF_029 (($33+5)-5)
    .define T4_DEF_030 ((T4_DEF_029)+(($91)-(($33+3)-3)))
    .define T4_DEF_031 (($1C-7)+7)
    .define T4_DEF_032 ((T4_DEF_031)+(($E9)-($1C)))
    .define T4_DEF_033 ((($8BC6*6)/3)-($8BC6))
    .define T4_DEF_034 (((T4_DEF_033)+(1))-($8BC6))
    .define T4_DEF_035 ((($FBBF*10)/5)-($FBBF))
    .define T4_DEF_036 (((T4_DEF_035)*2+(13)-(($FBBF)*2)))
    .define T4_DEF_037 (($27*2)-(($27)-0))
    .define T4_DEF_038 ((($0B)-($27))+(T4_DEF_037))
    .define T4_DEF_039 ((($0E+11)-4)-7)
    .define T4_DEF_040 ((T4_DEF_039)+(($D0)-(($0E+3)-3)))
    .define T4_DEF_041 ((($D5+3)-1)-2)
    .define T4_DEF_042 ((T4_DEF_041)+(($3B)-($D5)))
    .define T4_DEF_043 (($2B+5)-5)
    .define T4_DEF_044 (((T4_DEF_043)+(15))-($2B))
    .define T4_DEF_045 (($51-7)+7)
    .define T4_DEF_046 (((T4_DEF_045)*2+($2AAB)-(($51)*2)))
    .define T4_DEF_047 ((($9B*6)/3)-($9B))
    .define T4_DEF_048 ((($84)-($9B))+(T4_DEF_047))
    .define T4_DEF_049 ((($32*10)/5)-($32))
    .define T4_DEF_050 ((T4_DEF_049)+(($10)-(($32+3)-3)))
    .define T4_DEF_051 (($A6*2)-(($A6)-0))
    .define T4_DEF_052 ((T4_DEF_051)+(($2A)-($A6)))
    .define T4_DEF_053 (((0+11)-4)-7)
    .define T4_DEF_054 (((T4_DEF_053)+(11))-(0))
    .define T4_DEF_055 (((8+3)-1)-2)
    .define T4_DEF_056 (((T4_DEF_055)*2+($22)-((8)*2)))
    .define T4_DEF_057 (($44+5)-5)
    .define T4_DEF_058 ((($A780)-($44))+(T4_DEF_057))
    .define T4_DEF_059 (($D7-7)+7)
    .define T4_DEF_060 ((T4_DEF_059)+(($BA)-(($D7+3)-3)))
    .define T4_DEF_061 ((($B1*6)/3)-($B1))
    .define T4_DEF_062 ((T4_DEF_061)+(($2BBC)-($B1)))
    .define T4_DEF_063 ((($01*10)/5)-($01))
    .define T4_DEF_064 (((T4_DEF_063)+($05))-($01))
    .define T4_DEF_065 (($F1*2)-(($F1)-0))
    .define T4_DEF_066 (((T4_DEF_065)*2+($02)-(($F1)*2)))
    .define T4_DEF_067 ((($6F+11)-4)-7)
    .define T4_DEF_068 ((($6AB7)-($6F))+(T4_DEF_067))
    .define T4_DEF_069 ((($511F+3)-1)-2)
    .define T4_DEF_070 ((T4_DEF_069)+(($5C)-(($511F+3)-3)))
    .define T4_DEF_071 (($DB+5)-5)
    .define T4_DEF_072 ((T4_DEF_071)+(($04)-($DB)))
    .define T4_DEF_073 (($CA-7)+7)
    .define T4_DEF_074 (((T4_DEF_073)+($15))-($CA))
    .define T4_DEF_075 ((($BD*6)/3)-($BD))
    .define T4_DEF_076 (((T4_DEF_075)*2+($69)-(($BD)*2)))
    .define T4_DEF_077 ((($0336*10)/5)-($0336))
    .define T4_DEF_078 (((14)-($0336))+(T4_DEF_077))
    .define T4_DEF_079 ((9*2)-((9)-0))
    .define T4_DEF_080 ((T4_DEF_079)+(($1456)-((9+3)-3)))
    .define T4_DEF_081 ((($A2+11)-4)-7)
    .define T4_DEF_082 ((T4_DEF_081)+(($18)-($A2)))
    .define T4_DEF_083 ((($4D+3)-1)-2)
    .define T4_DEF_084 (((T4_DEF_083)+($944E))-($4D))
    .define T4_DEF_085 (($A4+5)-5)
    .define T4_DEF_086 (((T4_DEF_085)*2+($B9)-(($A4)*2)))
    .define T4_DEF_087 (($C4-7)+7)
    .define T4_DEF_088 ((($36)-($C4))+(T4_DEF_087))
    .define T4_DEF_089 ((($2D*6)/3)-($2D))
    .define T4_DEF_090 ((T4_DEF_089)+(($AD)-(($2D+3)-3)))
    .define T4_DEF_091 ((($A1*10)/5)-($A1))
    .define T4_DEF_092 ((T4_DEF_091)+(($8C)-($A1)))
    .define T4_DEF_093 (($16*2)-(($16)-0))
    .define T4_DEF_094 (((T4_DEF_093)+($1567))-($16))
    .db "04>"               ; @BT TEST-04 04 START

    ; GENERATED: copied from TEST-03 with varied define-based operand replacement
    ; GENERATED: replaced operands = 94 / 189

    ; GENERATED: copied from TEST-02 with varied arithmetic expressions for numeric operands

    ; GENERATED: copied from TEST-01 with full TEST-02 randomization
    ; GENERATED: seed = 511953886

    EI                             ; @BT A0
    SET1 T4_DEF_001+5-5,2-7+7                     ; @BT 42 45
    OR (X),(Y)                     ; @BT 19
    CMP Y,!T4_DEF_002*6/3-$6DEA                   ; @BT 5E EA 6D
    EOR A,[T4_DEF_003*10/5-$7D]+Y                  ; @BT 57 7D
    JMP !T4_DEF_004*2-$9BC5+0                     ; @BT 5F C5 9B
    DIV YA,X                       ; @BT 9E
    EOR A,(X)                      ; @BT 46
    ROL T4_DEF_005+11-4-7                        ; @BT 2B 90
    CBNE T4_DEF_006+3-1-2+X,$27+5-5                 ; @BT DE 75 27
    SBC A,[T4_DEF_007-7+7]+Y                  ; @BT B7 8D
    BMI T4_DEF_008*6/3-$D4                        ; @BT 30 D4
    BVC T4_DEF_009*10/5-$30                        ; @BT 50 30
    MOV A,X                        ; @BT 7D
    MOV !T4_DEF_010*2-$BAB2+0+X,A                 ; @BT D5 B2 BA
    CMP A,!T4_DEF_011+11-4-7                   ; @BT 65 62 65
    MOV A,!T4_DEF_012+3-1-2+X                 ; @BT F5 5E A5
    MOV Y,T4_DEF_013+5-5                      ; @BT EB 12
    BBC T4_DEF_014-7+7,0*6/3-0,$71*10/5-$71                  ; @BT 13 1F 71
    BBS T4_DEF_015*2-$78+0,7+11-4-7,$5A+3-1-2                  ; @BT E3 78 5A
    CMP A,!T4_DEF_016+5-5+X                 ; @BT 75 40 63
    LSR T4_DEF_017-7+7                        ; @BT 4B 1B
    CMP A,#T4_DEF_018*6/3-$CE                     ; @BT 68 CE
    SET1 T4_DEF_019*10/5-$2E,1*2-1+0                     ; @BT 22 2E
    MOV Y,#T4_DEF_020+11-4-7                     ; @BT 8D E4
    MOV T4_DEF_021+3-1-2,A                      ; @BT C4 29
    BBS T4_DEF_022+5-5,6-7+7,$53*6/3-$53                  ; @BT C3 61 53
    SBC A,(X)                      ; @BT A6
    AND A,!T4_DEF_023*10/5-$2788+Y                 ; @BT 36 88 27
    PUSH X                         ; @BT 4D
    BVS T4_DEF_024*2-$47+0                        ; @BT 70 47
    MOV X,!T4_DEF_025+11-4-7                   ; @BT E9 19 B1
    ADC A,T4_DEF_026+3-1-2                      ; @BT 84 C9
    CMP A,!T4_DEF_027+5-5+Y                 ; @BT 76 51 64
    EOR A,T4_DEF_028-7+7                      ; @BT 44 AB
    BBS T4_DEF_029*6/3-$33,4*10/5-4,$45*2-$45+0                  ; @BT 83 33 45
    MOV A,(X)+                     ; @BT BF
    RETI                           ; @BT 7F
    INCW T4_DEF_030+11-4-7                       ; @BT 3A 91
    NOT1 T1_BADDR_011,1            ; @BT EA 0B 20
    BBS T4_DEF_031+3-1-2,3+5-5,$3E-7+7                  ; @BT 63 1C 3E
    POP PSW                        ; @BT 8E
    SBC T4_DEF_032*6/3-$E9,#$E9*10/5-$E9                   ; @BT B8 E9 E9
    BRK                            ; @BT 0F
    EOR A,!T4_DEF_033*2-$8BC6+0                   ; @BT 45 C6 8B
    TCALL T4_DEF_034+11-4-7                        ; @BT 11
    ROR !T4_DEF_035+3-1-2                     ; @BT 6C BF FB
    TCALL T4_DEF_036+5-5                       ; @BT D1
    CMP T4_DEF_037-7+7,$27*6/3-$27                    ; @BT 69 27 27
    EOR1 C,T1_BADDR_005,5          ; @BT 8A 05 A0
    AND A,[T4_DEF_038*10/5-$0B+X]                  ; @BT 27 0B
    ADC T4_DEF_039*2-$0E+0,$0E+11-4-7                    ; @BT 89 0E 0E
    MOV1 C,T1_BADDR_006,2          ; @BT AA 06 40
    MOV X,T4_DEF_040+3-1-2                      ; @BT F8 D0
    ROR T4_DEF_041+5-5+X                      ; @BT 7B D5
    DEC T4_DEF_042-7+7+X                      ; @BT 9B 3B
    SUBW YA,T4_DEF_043*6/3-$2B                    ; @BT 9A 2B
    TCALL T4_DEF_044*10/5-15                       ; @BT F1
    EOR (X),(Y)                    ; @BT 59
    OR A,#T4_DEF_045*2-$51+0                      ; @BT 08 51
    TCLR1 !T4_DEF_046+11-4-7                   ; @BT 4E AB 2A
    ADC A,[T4_DEF_047+3-1-2]+Y                  ; @BT 97 9B
    ADC A,[T4_DEF_048+5-5+X]                  ; @BT 87 84
    MOV A,#T4_DEF_049-7+7                     ; @BT E8 32
    ADC A,(X)                      ; @BT 86
    CMP T4_DEF_050*6/3-$10,#$10*10/5-$10                   ; @BT 78 10 10
    BCS T4_DEF_051*2-$A6+0                        ; @BT B0 A6
    CMP A,[T4_DEF_052+11-4-7+X]                  ; @BT 67 2A
    TCALL T4_DEF_053+3-1-2                        ; @BT 01
    TCALL T4_DEF_054+5-5                       ; @BT B1
    AND1 C,/T1_BADDR_000,5         ; @BT 6A 00 A0
    TCALL T4_DEF_055-7+7                        ; @BT 81
    NOT1 T1_BADDR_010,0            ; @BT EA 0A 00
    EOR1 C,T1_BADDR_004,4          ; @BT 8A 04 80
    AND A,[T4_DEF_056*6/3-$22]+Y                  ; @BT 37 22
    OR1 C,/T1_BADDR_012,3          ; @BT 2A 0C 60
    CLR1 T4_DEF_057*10/5-$44,7*2-7+0                     ; @BT F2 44
    MOV A,!T4_DEF_058+11-4-7                   ; @BT E5 80 A7
    BBS T4_DEF_059+3-1-2,0+5-5,$29-7+7                  ; @BT 03 D7 29
    OR A,(X)                       ; @BT 06
    CLR1 T4_DEF_060*6/3-$BA,1*10/5-1                     ; @BT 32 BA
    DBNZ Y,T4_DEF_061*2-$B1+0                     ; @BT FE B1
    TSET1 !T4_DEF_062+11-4-7                   ; @BT 0E BC 2B
    MOV A,[T4_DEF_063+3-1-2]+Y                  ; @BT F7 01
    INC Y                          ; @BT FC
    BBS T4_DEF_064+5-5,2-7+7,$37*6/3-$37                  ; @BT 43 05 37
    MOV Y,A                        ; @BT FD
    ASL T4_DEF_065*10/5-$F1                        ; @BT 0B F1
    BPL T4_DEF_066*2-$02+0                        ; @BT 10 02
    CMP A,T4_DEF_067+11-4-7                      ; @BT 64 6F
    CLRC                           ; @BT 60
    INC X                          ; @BT 3D
    AND1 C,/T1_BADDR_001,6         ; @BT 6A 01 C0
    CMP X,!T4_DEF_068+3-1-2                   ; @BT 1E B7 6A
    CALL !T4_DEF_069+5-5                    ; @BT 3F 1F 51
    SET1 T4_DEF_070-7+7,3*6/3-3                     ; @BT 62 5C
    OR A,T4_DEF_071*10/5-$DB+X                     ; @BT 14 DB
    LSR T4_DEF_072*2-$04+0+X                      ; @BT 5B 04
    MOV T4_DEF_073+11-4-7,$CA+3-1-2                    ; @BT FA CA CA
    MOV [T4_DEF_074+5-5+X],A                  ; @BT C7 15
    BEQ T4_DEF_075-7+7                        ; @BT F0 BD
    DECW T4_DEF_076*6/3-$69                       ; @BT 1A 69
    AND A,(X)                      ; @BT 26
    SBC A,!T4_DEF_077*10/5-$0336                   ; @BT A5 36 03
    TCALL T4_DEF_078*2-14+0                       ; @BT E1
    TCALL T4_DEF_079+11-4-7                        ; @BT 91
    ROR A                          ; @BT 7C
    ADC A,!T4_DEF_080+3-1-2+X                 ; @BT 95 56 14
    MOV X,#T4_DEF_081+5-5                     ; @BT CD A2
    MOV A,T4_DEF_082-7+7+X                    ; @BT F4 18
    BBC T4_DEF_083*6/3-$4D,2*10/5-2,$7F*2-$7F+0                  ; @BT 53 4D 7F
    INC !T4_DEF_084+11-4-7                     ; @BT AC 4E 94
    SBC A,T4_DEF_085+3-1-2+X                    ; @BT B4 A4
    MOV X,T4_DEF_086+5-5+Y                    ; @BT F9 B9
    MOV1 C,T1_BADDR_007,3          ; @BT AA 07 60
    OR A,[T4_DEF_087-7+7]+Y                   ; @BT 17 C4
    BBC T4_DEF_088*6/3-$36,1*10/5-1,$78*2-$78+0                  ; @BT 33 36 78
    AND1 C,T1_BADDR_003,0          ; @BT 4A 03 00
    CMP A,(X)                      ; @BT 66
    CLR1 T4_DEF_089+11-4-7,6+3-1-2                     ; @BT D2 2D
    OR A,[T4_DEF_090+5-5+X]                   ; @BT 07 AD
    SET1 T4_DEF_091-7+7,6*6/3-6                     ; @BT C2 A1
    CBNE T4_DEF_092*10/5-$8C,$2E*2-$2E+0                   ; @BT 2E 8C 2E
    OR1 C,T1_BADDR_015,6           ; @BT 0A 0F C0
    PUSH PSW                       ; @BT 0D
    OR1 C,/T1_BADDR_013,4          ; @BT 2A 0D 80
    CLR1 T4_DEF_093+11-4-7,5+3-1-2                     ; @BT B2 16
    ADC A,!T4_DEF_094+5-5+Y                 ; @BT 96 67 15
    DBNZ $C8-7+7,$2A*6/3-$2A                   ; @BT 6E C8 2A
    MOV A,!$A66F*10/5-$A66F+Y                 ; @BT F6 6F A6
    DEC Y                          ; @BT DC
    ROL !$F77B*2-$F77B+0                     ; @BT 2C 7B F7
    MOV !$BBC3+11-4-7,A                   ; @BT C5 C3 BB
    CMP Y,#$E2+3-1-2                     ; @BT AD E2
    BBC $64+5-5,3-7+7,$06*6/3-$06                  ; @BT 73 64 06
    BBS $EE*10/5-$EE,1*2-1+0,$30+11-4-7                  ; @BT 23 EE 30
    MOV SP,X                       ; @BT BD
    DAS                            ; @BT BE
    MOV Y,$FB+3-1-2+X                    ; @BT FB FB
    SLEEP                          ; @BT EF
    ADC $F7+5-5,#$F7-7+7                   ; @BT 98 F7 F7
    AND A,#$AF*6/3-$AF                     ; @BT 28 AF
    MUL YA                         ; @BT CF
    BBC $92*10/5-$92,5*2-5+0,$14+11-4-7                  ; @BT B3 92 14
    ASL $DA+3-1-2+X                      ; @BT 1B DA
    PUSH A                         ; @BT 2D
    ROL $79+5-5+X                      ; @BT 3B 79
    CLRV                           ; @BT E0
    MOV !$BEF6-7+7,Y                   ; @BT CC F6 BE
    MOV1 T1_BADDR_008,4,C          ; @BT CA 08 80
    ADC A,!$1678*6/3-$1678                   ; @BT 85 78 16
    POP X                          ; @BT CE
    TCALL 10*10/5-10                       ; @BT A1
    CMP (X),(Y)                    ; @BT 79
    AND $95*2-$95+0,$95+11-4-7                    ; @BT 29 95 95
    MOV $6E+3-1-2+X,A                    ; @BT D4 6E
    JMP [!$9CD6+5-5+X]                 ; @BT 1F D6 9C
    BBC $A9-7+7,6*6/3-6,$1B*10/5-$1B                  ; @BT D3 A9 1B
    BNE $EB*2-$EB+0                        ; @BT D0 EB
    CLR1 $A3+11-4-7,0+3-1-2                     ; @BT 12 A3
    TCALL 5+5-5                        ; @BT 51
    ROL A                          ; @BT 3C
    DAA A                          ; @BT DF
    SBC (X),(Y)                    ; @BT B9
    BBC $7B-7+7,4*6/3-4,$0D*10/5-$0D                  ; @BT 93 7B 0D
    MOV $85*2-$85+0+X,Y                    ; @BT DB 85
    EOR A,#$0A+11-4-7                     ; @BT 48 0A
    MOV X,SP                       ; @BT 9D
    MOV (X)+,A                     ; @BT AF
    AND1 C,T1_BADDR_002,7          ; @BT 4A 02 E0
    EOR A,$94+3-1-2+X                    ; @BT 54 94
    AND $7E+5-5,#$7E-7+7                   ; @BT 38 7E 7E
    DEC $52*6/3-$52                        ; @BT 8B 52
    ADC A,#$28*10/5-$28                     ; @BT 88 28
    CMP X,$B4*2-$B4+0                      ; @BT 3E B4
    CLRP                           ; @BT 20
    DEC A                          ; @BT 9C
    POP Y                          ; @BT EE
    OR $20+11-4-7,#$20+3-1-2                    ; @BT 18 20 20
    DEC X                          ; @BT 1D
    SBC A,$BB+5-5                      ; @BT A4 BB
    MOVW YA,$E1-7+7                    ; @BT BA E1
    CLR1 $E8*6/3-$E8,3*10/5-3                     ; @BT 72 E8
    MOV Y,!$C007*2-$C007+0                   ; @BT EC 07 C0
    OR A,!$E116+11-4-7+Y                  ; @BT 16 16 E1
    POP A                          ; @BT AE
    MOV A,$2F+3-1-2                      ; @BT E4 2F
    CMP A,[$41+5-5]+Y                  ; @BT 77 41
    CMP X,#$9D-7+7                     ; @BT C8 9D
    EOR A,[$66*6/3-$66+X]                  ; @BT 47 66
    INC $63*10/5-$63+X                      ; @BT BB 63
    MOV $40*2-$40+0,#$40+11-4-7                   ; @BT 8F 40 40
    BBS $4A+3-1-2,5+5-5,$4C-7+7                  ; @BT A3 4A 4C
    DEC !$7BC7*6/3-$7BC7                     ; @BT 8C C7 7B
    ASL !$3121*10/5-$3121                     ; @BT 0C 21 31
    MOV1 T1_BADDR_009,5,C          ; @BT CA 09 A0
    SBC A,[$76*2-$76+0+X]                  ; @BT A7 76
    EOR A,!$89A4+11-4-7+X                 ; @BT 55 A4 89
    TCALL 2+3-1-2                        ; @BT 21
    ADC A,$B2+5-5+X                    ; @BT 94 B2
    OR $37-7+7,$37*6/3-$37                     ; @BT 09 37 37
    INC $7A*10/5-$7A                        ; @BT AB 7A
    CMP Y,$F9*2-$F9+0                      ; @BT 7E F9
    MOV A,Y                        ; @BT DD
    SBC A,#$1A+11-4-7                     ; @BT A8 1A
    CLR1 $D1+3-1-2,2+5-5                     ; @BT 52 D1
    MOVW $F8-7+7,YA                    ; @BT DA F8
    NOP                            ; @BT 00
    PUSH Y                         ; @BT 6D
    DI                             ; @BT C0
    TCALL 6*6/3-6                        ; @BT 61
    OR A,$F2*10/5-$F2                       ; @BT 04 F2
    CLR1 $FF*2-$FF+0,4+11-4-7                     ; @BT 92 FF
    TCALL 3+3-1-2                        ; @BT 31
    OR A,!$E005+5-5+X                  ; @BT 15 05 E0
    INC A                          ; @BT BC
    BCC $8F-7+7                        ; @BT 90 8F
    SETC                           ; @BT 80
    SETP                           ; @BT 40
    MOV X,A                        ; @BT 5D
    ADDW YA,$25*6/3-$25                    ; @BT 7A 25
    MOV $9C*10/5-$9C+Y,X                    ; @BT D9 9C
    TCALL 7*2-7+0                        ; @BT 71
    SET1 $8A+11-4-7,5+3-1-2                     ; @BT A2 8A
    SET1 $73+5-5,4-7+7                     ; @BT 82 73
    SBC $00*6/3-$00,$00*10/5-$00                    ; @BT A9 00 00
    DAA                            ; @BT DF
    MOV $B3*2-$B3+0,Y                      ; @BT CB B3
    OR A,!$E227+11-4-7                    ; @BT 05 27 E2
    AND A,!$2899+3-1-2                   ; @BT 25 99 28
    NOTC                           ; @BT ED
    EOR A,!$8AB5+5-5+Y                 ; @BT 56 B5 8A
    XCN A                          ; @BT 9F
    TCALL 12-7+7                       ; @BT C1
    AND A,!$2677*6/3-$2677+X                 ; @BT 35 77 26
    BBC $C0*10/5-$C0,7*2-7+0,$22+11-4-7                  ; @BT F3 C0 22
    SET1 $17+3-1-2,0+5-5                     ; @BT 02 17
    DAS A                          ; @BT BE
    MOV A,[$EA-7+7+X]                  ; @BT E7 EA
    ADC (X),(Y)                    ; @BT 99
    MOV (X),A                      ; @BT C6
    AND A,$50*6/3-$50                      ; @BT 24 50
    MOV $57*10/5-$57,X                      ; @BT D8 57
    ASL A                          ; @BT 1C
    SBC A,!$0225*2-$0225+0+Y                 ; @BT B6 25 02
    EOR $F0+11-4-7,$F0+3-1-2                    ; @BT 49 F0 F0
    AND (X),(Y)                    ; @BT 39
    EOR $D9+5-5,#$D9-7+7                   ; @BT 58 D9 D9
    PCALL $4E*6/3-$4E                      ; @BT 4F 4E
    MOV !$BDE5*10/5-$BDE5+Y,A                 ; @BT D6 E5 BD
    STOP                           ; @BT FF
    CMP A,$58*2-$58+0+X                    ; @BT 74 58
    TCALL 4+11-4-7                        ; @BT 41
    SET1 $B8+3-1-2,7+5-5                     ; @BT E2 B8
    OR1 C,T1_BADDR_014,5           ; @BT 0A 0E A0
    MOV A,(X)                      ; @BT E6
    CMPW YA,$3E-7+7                    ; @BT 5A 3E
    RET                            ; @BT 6F
    MOV !$BCD4*6/3-$BCD4,X                   ; @BT C9 D4 BC
    BRA $19*10/5-$19                        ; @BT 2F 19
    AND A,$39*2-$39+0+X                    ; @BT 34 39
    ROR $EC+11-4-7                        ; @BT 6B EC
    MOV [$2C+3-1-2]+Y,A                  ; @BT D7 2C
    LSR A                          ; @BT 5C
    LSR !$9DE7+5-5                     ; @BT 4C E7 9D
    SBC A,!$0114-7+7+X                 ; @BT B5 14 01

    .db "<04"               ; @BT END

    /////////////////////////////////////////////////////////////////////////////
    // TEST-05: similar to TEST-04, randomized instruction order, and defines + functions + expressions
    /////////////////////////////////////////////////////////////////////////////










    ; TEST-05 generated defines/functions
    .define T5_DEF_001 (($45+5)-5)
    .define T5_DEF_002 ((T5_DEF_001)+(($6DEA)-($45)))
    .define T5_DEF_003 (($7D-7)+7)
    .define T5_DEF_004 (((T5_DEF_003)+($9BC5))-($7D))
    .define T5_DEF_005 ((($90*6)/3)-($90))
    .define T5_DEF_006 (((T5_DEF_005)*2+($75)-(($90)*2)))
    .define T5_DEF_007 ((($8D*10)/5)-($8D))
    .define T5_DEF_008 ((($D4)-($8D))+(T5_DEF_007))
    .define T5_DEF_009 (($30*2)-(($30)-0))
    .define T5_DEF_010 ((T5_DEF_009)+(($BAB2)-(($30+3)-3)))
    .define T5_DEF_011 ((($6562+11)-4)-7)
    .define T5_DEF_012 ((T5_DEF_011)+(($A55E)-($6562)))
    .define T5_DEF_013 ((($12+3)-1)-2)
    .define T5_DEF_014 (((T5_DEF_013)+($1F))-($12))
    .define T5_DEF_015 (($78+5)-5)
    .define T5_DEF_016 (((T5_DEF_015)*2+($6340)-(($78)*2)))
    .define T5_DEF_017 (($1B-7)+7)
    .define T5_DEF_018 ((($CE)-($1B))+(T5_DEF_017))
    .define T5_DEF_019 ((($2E*6)/3)-($2E))
    .define T5_DEF_020 ((T5_DEF_019)+(($E4)-(($2E+3)-3)))
    .define T5_DEF_021 ((($29*10)/5)-($29))
    .define T5_DEF_022 ((T5_DEF_021)+(($61)-($29)))
    .define T5_DEF_023 (($2788*2)-(($2788)-0))
    .define T5_DEF_024 (((T5_DEF_023)+($47))-($2788))
    .define T5_DEF_025 ((($B119+11)-4)-7)
    .define T5_DEF_026 (((T5_DEF_025)*2+($C9)-(($B119)*2)))
    .define T5_DEF_027 ((($6451+3)-1)-2)
    .define T5_DEF_028 ((($AB)-($6451))+(T5_DEF_027))
    .define T5_DEF_029 (($33+5)-5)
    .define T5_DEF_030 ((T5_DEF_029)+(($91)-(($33+3)-3)))
    .define T5_DEF_031 (($1C-7)+7)
    .define T5_DEF_032 ((T5_DEF_031)+(($E9)-($1C)))
    .define T5_DEF_033 ((($8BC6*6)/3)-($8BC6))
    .define T5_DEF_034 (((T5_DEF_033)+(1))-($8BC6))
    .define T5_DEF_035 ((($FBBF*10)/5)-($FBBF))
    .define T5_DEF_036 (((T5_DEF_035)*2+(13)-(($FBBF)*2)))
    .define T5_DEF_037 (($27*2)-(($27)-0))
    .define T5_DEF_038 ((($0B)-($27))+(T5_DEF_037))
    .define T5_DEF_039 ((($0E+11)-4)-7)
    .define T5_DEF_040 ((T5_DEF_039)+(($D0)-(($0E+3)-3)))
    .define T5_DEF_041 ((($D5+3)-1)-2)
    .define T5_DEF_042 ((T5_DEF_041)+(($3B)-($D5)))
    .define T5_DEF_043 (($2B+5)-5)
    .define T5_DEF_044 (((T5_DEF_043)+(15))-($2B))
    .define T5_DEF_045 (($51-7)+7)
    .define T5_DEF_046 (((T5_DEF_045)*2+($2AAB)-(($51)*2)))
    .define T5_DEF_047 ((($9B*6)/3)-($9B))
    .define T5_DEF_048 ((($84)-($9B))+(T5_DEF_047))
    .define T5_DEF_049 ((($32*10)/5)-($32))
    .define T5_DEF_050 ((T5_DEF_049)+(($10)-(($32+3)-3)))
    .define T5_DEF_051 (($A6*2)-(($A6)-0))
    .define T5_DEF_052 ((T5_DEF_051)+(($2A)-($A6)))
    .define T5_DEF_053 (((0+11)-4)-7)
    .define T5_DEF_054 (((T5_DEF_053)+(11))-(0))
    .define T5_DEF_055 (((8+3)-1)-2)
    .define T5_DEF_056 (((T5_DEF_055)*2+($22)-((8)*2)))
    .define T5_DEF_057 (($44+5)-5)
    .define T5_DEF_058 ((($A780)-($44))+(T5_DEF_057))
    .define T5_DEF_059 (($D7-7)+7)
    .define T5_DEF_060 ((T5_DEF_059)+(($BA)-(($D7+3)-3)))
    .define T5_DEF_061 ((($B1*6)/3)-($B1))
    .define T5_DEF_062 ((T5_DEF_061)+(($2BBC)-($B1)))
    .define T5_DEF_063 ((($01*10)/5)-($01))
    .define T5_DEF_064 (((T5_DEF_063)+($05))-($01))
    .define T5_DEF_065 (($F1*2)-(($F1)-0))
    .define T5_DEF_066 (((T5_DEF_065)*2+($02)-(($F1)*2)))
    .define T5_DEF_067 ((($6F+11)-4)-7)
    .define T5_DEF_068 ((($6AB7)-($6F))+(T5_DEF_067))
    .define T5_DEF_069 ((($511F+3)-1)-2)
    .define T5_DEF_070 ((T5_DEF_069)+(($5C)-(($511F+3)-3)))
    .define T5_DEF_071 (($DB+5)-5)
    .define T5_DEF_072 ((T5_DEF_071)+(($04)-($DB)))
    .define T5_DEF_073 (($CA-7)+7)
    .define T5_DEF_074 (((T5_DEF_073)+($15))-($CA))
    .define T5_DEF_075 ((($BD*6)/3)-($BD))
    .define T5_DEF_076 (((T5_DEF_075)*2+($69)-(($BD)*2)))
    .define T5_DEF_077 ((($0336*10)/5)-($0336))
    .define T5_DEF_078 (((14)-($0336))+(T5_DEF_077))
    .define T5_DEF_079 ((9*2)-((9)-0))
    .define T5_DEF_080 ((T5_DEF_079)+(($1456)-((9+3)-3)))
    .define T5_DEF_081 ((($A2+11)-4)-7)
    .define T5_DEF_082 ((T5_DEF_081)+(($18)-($A2)))
    .define T5_DEF_083 ((($4D+3)-1)-2)
    .define T5_DEF_084 (((T5_DEF_083)+($944E))-($4D))
    .define T5_DEF_085 (($A4+5)-5)
    .define T5_DEF_086 (((T5_DEF_085)*2+($B9)-(($A4)*2)))
    .define T5_DEF_087 (($C4-7)+7)
    .define T5_DEF_088 ((($36)-($C4))+(T5_DEF_087))
    .define T5_DEF_089 ((($2D*6)/3)-($2D))
    .define T5_DEF_090 ((T5_DEF_089)+(($AD)-(($2D+3)-3)))
    .define T5_DEF_091 ((($A1*10)/5)-($A1))
    .define T5_DEF_092 ((T5_DEF_091)+(($8C)-($A1)))
    .define T5_DEF_093 (($16*2)-(($16)-0))
    .define T5_DEF_094 (((T5_DEF_093)+($1567))-($16))

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
    .function T5_FN_014(a,b) (((T5_FN_013(a,b)+T5_DEF_014)-T5_DEF_014))
    .function T5_FN_015(a,b,c) ((((a+b-c)*8)/4)-((a+b-c)*2)+(a+b-c)+T5_DEF_015-T5_DEF_015)
    .function T5_FN_016(a,b,c) (((T5_FN_015(a,b,c))*2+T5_DEF_016-T5_DEF_016-T5_FN_015(a,b,c)))
    .function T5_FN_017(a,b,c,d) ((((a+b-c-d)*8)/4)-((a+b-c-d)*2)+(a+b-c-d)+T5_DEF_017-T5_DEF_017)
    .function T5_FN_018(a,b,c,d) ((T5_FN_017(a,b,c,d))+(T5_DEF_018-T5_DEF_018))
    .function T5_FN_019(a,b) ((((a-b)*8)/4)-((a-b)*2)+(a-b)+T5_DEF_019-T5_DEF_019)
    .function T5_FN_020(a,b) (((T5_FN_019(a,b)+3)-3)+(T5_DEF_020-T5_DEF_020))
    .function T5_FN_021(a,b,c) ((((a+b-c)*8)/4)-((a+b-c)*2)+(a+b-c)+T5_DEF_021-T5_DEF_021)
    .function T5_FN_022(a,b,c) (((T5_FN_021(a,b,c)*8)/4)-(T5_FN_021(a,b,c)*2)+T5_FN_021(a,b,c)+T5_DEF_022-T5_DEF_022)
    .function T5_FN_023(a,b,c,d) ((((a+b-c-d)*8)/4)-((a+b-c-d)*2)+(a+b-c-d)+T5_DEF_023-T5_DEF_023)
    .function T5_FN_024(a,b,c,d) (((T5_FN_023(a,b,c,d)+T5_DEF_024)-T5_DEF_024))
    .function T5_FN_025(a,b) ((((a-b)*8)/4)-((a-b)*2)+(a-b)+T5_DEF_025-T5_DEF_025)
    .function T5_FN_026(a,b) (((T5_FN_025(a,b))*2+T5_DEF_026-T5_DEF_026-T5_FN_025(a,b)))
    .function T5_FN_027(a,b,c) ((((a+b-c)*8)/4)-((a+b-c)*2)+(a+b-c)+T5_DEF_027-T5_DEF_027)
    .function T5_FN_028(a,b,c) ((T5_FN_027(a,b,c))+(T5_DEF_028-T5_DEF_028))
    .function T5_FN_029(a,b,c,d) ((((a+b-c-d)*8)/4)-((a+b-c-d)*2)+(a+b-c-d)+T5_DEF_029-T5_DEF_029)
    .function T5_FN_030(a,b,c,d) (((T5_FN_029(a,b,c,d)+3)-3)+(T5_DEF_030-T5_DEF_030))
    .function T5_FN_031(a,b) ((((a-b)*8)/4)-((a-b)*2)+(a-b)+T5_DEF_031-T5_DEF_031)
    .function T5_FN_032(a,b) (((T5_FN_031(a,b)*8)/4)-(T5_FN_031(a,b)*2)+T5_FN_031(a,b)+T5_DEF_032-T5_DEF_032)
    .function T5_FN_033(a,b,c) ((((a+b-c)*8)/4)-((a+b-c)*2)+(a+b-c)+T5_DEF_033-T5_DEF_033)
    .function T5_FN_034(a,b,c) (((T5_FN_033(a,b,c)+T5_DEF_034)-T5_DEF_034))
    .function T5_FN_035(a,b,c,d) ((((a+b-c-d)*8)/4)-((a+b-c-d)*2)+(a+b-c-d)+T5_DEF_035-T5_DEF_035)
    .function T5_FN_036(a,b,c,d) (((T5_FN_035(a,b,c,d))*2+T5_DEF_036-T5_DEF_036-T5_FN_035(a,b,c,d)))
    .function T5_FN_037(a,b) ((((a-b)*8)/4)-((a-b)*2)+(a-b)+T5_DEF_037-T5_DEF_037)
    .function T5_FN_038(a,b) ((T5_FN_037(a,b))+(T5_DEF_038-T5_DEF_038))
    .function T5_FN_039(a,b,c) ((((a+b-c)*8)/4)-((a+b-c)*2)+(a+b-c)+T5_DEF_039-T5_DEF_039)
    .function T5_FN_040(a,b,c) (((T5_FN_039(a,b,c)+3)-3)+(T5_DEF_040-T5_DEF_040))
    .function T5_FN_041(a,b,c,d) ((((a+b-c-d)*8)/4)-((a+b-c-d)*2)+(a+b-c-d)+T5_DEF_041-T5_DEF_041)
    .function T5_FN_042(a,b,c,d) (((T5_FN_041(a,b,c,d)*8)/4)-(T5_FN_041(a,b,c,d)*2)+T5_FN_041(a,b,c,d)+T5_DEF_042-T5_DEF_042)
    .function T5_FN_043(a,b) ((((a-b)*8)/4)-((a-b)*2)+(a-b)+T5_DEF_043-T5_DEF_043)
    .function T5_FN_044(a,b) (((T5_FN_043(a,b)+T5_DEF_044)-T5_DEF_044))
    .function T5_FN_045(a,b,c) ((((a+b-c)*8)/4)-((a+b-c)*2)+(a+b-c)+T5_DEF_045-T5_DEF_045)
    .function T5_FN_046(a,b,c) (((T5_FN_045(a,b,c))*2+T5_DEF_046-T5_DEF_046-T5_FN_045(a,b,c)))
    .function T5_FN_047(a,b,c,d) ((((a+b-c-d)*8)/4)-((a+b-c-d)*2)+(a+b-c-d)+T5_DEF_047-T5_DEF_047)
    .db "05>"               ; @BT TEST-05 05 START

    ; GENERATED: copied from TEST-04; half define-label references replaced by varied function calls
    ; GENERATED: replaced define refs = 47 / 94

    ; GENERATED: copied from TEST-03 with varied define-based operand replacement
    ; GENERATED: replaced operands = 94 / 189

    ; GENERATED: copied from TEST-02 with varied arithmetic expressions for numeric operands

    ; GENERATED: copied from TEST-01 with full TEST-02 randomization
    ; GENERATED: seed = 511953886

    EI                             ; @BT A0
    SET1 T5_FN_001(T5_DEF_001+2,2)+5-5,2-7+7                     ; @BT 42 45
    OR (X),(Y)                     ; @BT 19
    CMP Y,!T5_FN_002(T5_DEF_002+3,3)*6/3-$6DEA                   ; @BT 5E EA 6D
    EOR A,[T5_FN_003(T5_DEF_003+4,5,4+5)*10/5-$7D]+Y                  ; @BT 57 7D
    JMP !T5_FN_004(T5_DEF_004+5,6,5+6)*2-$9BC5+0                     ; @BT 5F C5 9B
    DIV YA,X                       ; @BT 9E
    EOR A,(X)                      ; @BT 46
    ROL T5_FN_005(T5_DEF_005+6+7,8,6,7+8)+11-4-7                        ; @BT 2B 90
    CBNE T5_FN_006(T5_DEF_006+7+8,9,7,8+9)+3-1-2+X,$27+5-5                 ; @BT DE 75 27
    SBC A,[T5_FN_007(T5_DEF_007+8,8)-7+7]+Y                  ; @BT B7 8D
    BMI T5_FN_008(T5_DEF_008+9,9)*6/3-$D4                        ; @BT 30 D4
    BVC T5_FN_009(T5_DEF_009+10,11,10+11)*10/5-$30                        ; @BT 50 30
    MOV A,X                        ; @BT 7D
    MOV !T5_FN_010(T5_DEF_010+11,12,11+12)*2-$BAB2+0+X,A                 ; @BT D5 B2 BA
    CMP A,!T5_FN_011(T5_DEF_011+12+13,14,12,13+14)+11-4-7                   ; @BT 65 62 65
    MOV A,!T5_FN_012(T5_DEF_012+13+14,15,13,14+15)+3-1-2+X                 ; @BT F5 5E A5
    MOV Y,T5_FN_013(T5_DEF_013+14,14)+5-5                      ; @BT EB 12
    BBC T5_FN_014(T5_DEF_014+15,15)-7+7,0*6/3-0,$71*10/5-$71                  ; @BT 13 1F 71
    BBS T5_FN_015(T5_DEF_015+16,17,16+17)*2-$78+0,7+11-4-7,$5A+3-1-2                  ; @BT E3 78 5A
    CMP A,!T5_FN_016(T5_DEF_016+17,18,17+18)+5-5+X                 ; @BT 75 40 63
    LSR T5_FN_017(T5_DEF_017+18+19,20,18,19+20)-7+7                        ; @BT 4B 1B
    CMP A,#T5_FN_018(T5_DEF_018+19+20,21,19,20+21)*6/3-$CE                     ; @BT 68 CE
    SET1 T5_FN_019(T5_DEF_019+20,20)*10/5-$2E,1*2-1+0                     ; @BT 22 2E
    MOV Y,#T5_FN_020(T5_DEF_020+21,21)+11-4-7                     ; @BT 8D E4
    MOV T5_FN_021(T5_DEF_021+22,23,22+23)+3-1-2,A                      ; @BT C4 29
    BBS T5_FN_022(T5_DEF_022+23,24,23+24)+5-5,6-7+7,$53*6/3-$53                  ; @BT C3 61 53
    SBC A,(X)                      ; @BT A6
    AND A,!T5_FN_023(T5_DEF_023+24+25,26,24,25+26)*10/5-$2788+Y                 ; @BT 36 88 27
    PUSH X                         ; @BT 4D
    BVS T5_FN_024(T5_DEF_024+25+26,27,25,26+27)*2-$47+0                        ; @BT 70 47
    MOV X,!T5_FN_025(T5_DEF_025+26,26)+11-4-7                   ; @BT E9 19 B1
    ADC A,T5_FN_026(T5_DEF_026+27,27)+3-1-2                      ; @BT 84 C9
    CMP A,!T5_FN_027(T5_DEF_027+28,29,28+29)+5-5+Y                 ; @BT 76 51 64
    EOR A,T5_FN_028(T5_DEF_028+29,30,29+30)-7+7                      ; @BT 44 AB
    BBS T5_FN_029(T5_DEF_029+30+31,32,30,31+32)*6/3-$33,4*10/5-4,$45*2-$45+0                  ; @BT 83 33 45
    MOV A,(X)+                     ; @BT BF
    RETI                           ; @BT 7F
    INCW T5_FN_030(T5_DEF_030+31+32,33,31,32+33)+11-4-7                       ; @BT 3A 91
    NOT1 T1_BADDR_011,1            ; @BT EA 0B 20
    BBS T5_FN_031(T5_DEF_031+32,32)+3-1-2,3+5-5,$3E-7+7                  ; @BT 63 1C 3E
    POP PSW                        ; @BT 8E
    SBC T5_FN_032(T5_DEF_032+33,33)*6/3-$E9,#$E9*10/5-$E9                   ; @BT B8 E9 E9
    BRK                            ; @BT 0F
    EOR A,!T5_FN_033(T5_DEF_033+34,35,34+35)*2-$8BC6+0                   ; @BT 45 C6 8B
    TCALL T5_FN_034(T5_DEF_034+35,36,35+36)+11-4-7                        ; @BT 11
    ROR !T5_FN_035(T5_DEF_035+36+37,38,36,37+38)+3-1-2                     ; @BT 6C BF FB
    TCALL T5_FN_036(T5_DEF_036+37+38,39,37,38+39)+5-5                       ; @BT D1
    CMP T5_FN_037(T5_DEF_037+38,38)-7+7,$27*6/3-$27                    ; @BT 69 27 27
    EOR1 C,T1_BADDR_005,5          ; @BT 8A 05 A0
    AND A,[T5_FN_038(T5_DEF_038+39,39)*10/5-$0B+X]                  ; @BT 27 0B
    ADC T5_FN_039(T5_DEF_039+40,41,40+41)*2-$0E+0,$0E+11-4-7                    ; @BT 89 0E 0E
    MOV1 C,T1_BADDR_006,2          ; @BT AA 06 40
    MOV X,T5_FN_040(T5_DEF_040+41,42,41+42)+3-1-2                      ; @BT F8 D0
    ROR T5_FN_041(T5_DEF_041+42+43,44,42,43+44)+5-5+X                      ; @BT 7B D5
    DEC T5_FN_042(T5_DEF_042+43+44,45,43,44+45)-7+7+X                      ; @BT 9B 3B
    SUBW YA,T5_FN_043(T5_DEF_043+44,44)*6/3-$2B                    ; @BT 9A 2B
    TCALL T5_FN_044(T5_DEF_044+45,45)*10/5-15                       ; @BT F1
    EOR (X),(Y)                    ; @BT 59
    OR A,#T5_FN_045(T5_DEF_045+46,47,46+47)*2-$51+0                      ; @BT 08 51
    TCLR1 !T5_FN_046(T5_DEF_046+47,48,47+48)+11-4-7                   ; @BT 4E AB 2A
    ADC A,[T5_FN_047(T5_DEF_047+48+49,50,48,49+50)+3-1-2]+Y                  ; @BT 97 9B
    ADC A,[T5_DEF_048+5-5+X]                  ; @BT 87 84
    MOV A,#T5_DEF_049-7+7                     ; @BT E8 32
    ADC A,(X)                      ; @BT 86
    CMP T5_DEF_050*6/3-$10,#$10*10/5-$10                   ; @BT 78 10 10
    BCS T5_DEF_051*2-$A6+0                        ; @BT B0 A6
    CMP A,[T5_DEF_052+11-4-7+X]                  ; @BT 67 2A
    TCALL T5_DEF_053+3-1-2                        ; @BT 01
    TCALL T5_DEF_054+5-5                       ; @BT B1
    AND1 C,/T1_BADDR_000,5         ; @BT 6A 00 A0
    TCALL T5_DEF_055-7+7                        ; @BT 81
    NOT1 T1_BADDR_010,0            ; @BT EA 0A 00
    EOR1 C,T1_BADDR_004,4          ; @BT 8A 04 80
    AND A,[T5_DEF_056*6/3-$22]+Y                  ; @BT 37 22
    OR1 C,/T1_BADDR_012,3          ; @BT 2A 0C 60
    CLR1 T5_DEF_057*10/5-$44,7*2-7+0                     ; @BT F2 44
    MOV A,!T5_DEF_058+11-4-7                   ; @BT E5 80 A7
    BBS T5_DEF_059+3-1-2,0+5-5,$29-7+7                  ; @BT 03 D7 29
    OR A,(X)                       ; @BT 06
    CLR1 T5_DEF_060*6/3-$BA,1*10/5-1                     ; @BT 32 BA
    DBNZ Y,T5_DEF_061*2-$B1+0                     ; @BT FE B1
    TSET1 !T5_DEF_062+11-4-7                   ; @BT 0E BC 2B
    MOV A,[T5_DEF_063+3-1-2]+Y                  ; @BT F7 01
    INC Y                          ; @BT FC
    BBS T5_DEF_064+5-5,2-7+7,$37*6/3-$37                  ; @BT 43 05 37
    MOV Y,A                        ; @BT FD
    ASL T5_DEF_065*10/5-$F1                        ; @BT 0B F1
    BPL T5_DEF_066*2-$02+0                        ; @BT 10 02
    CMP A,T5_DEF_067+11-4-7                      ; @BT 64 6F
    CLRC                           ; @BT 60
    INC X                          ; @BT 3D
    AND1 C,/T1_BADDR_001,6         ; @BT 6A 01 C0
    CMP X,!T5_DEF_068+3-1-2                   ; @BT 1E B7 6A
    CALL !T5_DEF_069+5-5                    ; @BT 3F 1F 51
    SET1 T5_DEF_070-7+7,3*6/3-3                     ; @BT 62 5C
    OR A,T5_DEF_071*10/5-$DB+X                     ; @BT 14 DB
    LSR T5_DEF_072*2-$04+0+X                      ; @BT 5B 04
    MOV T5_DEF_073+11-4-7,$CA+3-1-2                    ; @BT FA CA CA
    MOV [T5_DEF_074+5-5+X],A                  ; @BT C7 15
    BEQ T5_DEF_075-7+7                        ; @BT F0 BD
    DECW T5_DEF_076*6/3-$69                       ; @BT 1A 69
    AND A,(X)                      ; @BT 26
    SBC A,!T5_DEF_077*10/5-$0336                   ; @BT A5 36 03
    TCALL T5_DEF_078*2-14+0                       ; @BT E1
    TCALL T5_DEF_079+11-4-7                        ; @BT 91
    ROR A                          ; @BT 7C
    ADC A,!T5_DEF_080+3-1-2+X                 ; @BT 95 56 14
    MOV X,#T5_DEF_081+5-5                     ; @BT CD A2
    MOV A,T5_DEF_082-7+7+X                    ; @BT F4 18
    BBC T5_DEF_083*6/3-$4D,2*10/5-2,$7F*2-$7F+0                  ; @BT 53 4D 7F
    INC !T5_DEF_084+11-4-7                     ; @BT AC 4E 94
    SBC A,T5_DEF_085+3-1-2+X                    ; @BT B4 A4
    MOV X,T5_DEF_086+5-5+Y                    ; @BT F9 B9
    MOV1 C,T1_BADDR_007,3          ; @BT AA 07 60
    OR A,[T5_DEF_087-7+7]+Y                   ; @BT 17 C4
    BBC T5_DEF_088*6/3-$36,1*10/5-1,$78*2-$78+0                  ; @BT 33 36 78
    AND1 C,T1_BADDR_003,0          ; @BT 4A 03 00
    CMP A,(X)                      ; @BT 66
    CLR1 T5_DEF_089+11-4-7,6+3-1-2                     ; @BT D2 2D
    OR A,[T5_DEF_090+5-5+X]                   ; @BT 07 AD
    SET1 T5_DEF_091-7+7,6*6/3-6                     ; @BT C2 A1
    CBNE T5_DEF_092*10/5-$8C,$2E*2-$2E+0                   ; @BT 2E 8C 2E
    OR1 C,T1_BADDR_015,6           ; @BT 0A 0F C0
    PUSH PSW                       ; @BT 0D
    OR1 C,/T1_BADDR_013,4          ; @BT 2A 0D 80
    CLR1 T5_DEF_093+11-4-7,5+3-1-2                     ; @BT B2 16
    ADC A,!T5_DEF_094+5-5+Y                 ; @BT 96 67 15
    DBNZ $C8-7+7,$2A*6/3-$2A                   ; @BT 6E C8 2A
    MOV A,!$A66F*10/5-$A66F+Y                 ; @BT F6 6F A6
    DEC Y                          ; @BT DC
    ROL !$F77B*2-$F77B+0                     ; @BT 2C 7B F7
    MOV !$BBC3+11-4-7,A                   ; @BT C5 C3 BB
    CMP Y,#$E2+3-1-2                     ; @BT AD E2
    BBC $64+5-5,3-7+7,$06*6/3-$06                  ; @BT 73 64 06
    BBS $EE*10/5-$EE,1*2-1+0,$30+11-4-7                  ; @BT 23 EE 30
    MOV SP,X                       ; @BT BD
    DAS                            ; @BT BE
    MOV Y,$FB+3-1-2+X                    ; @BT FB FB
    SLEEP                          ; @BT EF
    ADC $F7+5-5,#$F7-7+7                   ; @BT 98 F7 F7
    AND A,#$AF*6/3-$AF                     ; @BT 28 AF
    MUL YA                         ; @BT CF
    BBC $92*10/5-$92,5*2-5+0,$14+11-4-7                  ; @BT B3 92 14
    ASL $DA+3-1-2+X                      ; @BT 1B DA
    PUSH A                         ; @BT 2D
    ROL $79+5-5+X                      ; @BT 3B 79
    CLRV                           ; @BT E0
    MOV !$BEF6-7+7,Y                   ; @BT CC F6 BE
    MOV1 T1_BADDR_008,4,C          ; @BT CA 08 80
    ADC A,!$1678*6/3-$1678                   ; @BT 85 78 16
    POP X                          ; @BT CE
    TCALL 10*10/5-10                       ; @BT A1
    CMP (X),(Y)                    ; @BT 79
    AND $95*2-$95+0,$95+11-4-7                    ; @BT 29 95 95
    MOV $6E+3-1-2+X,A                    ; @BT D4 6E
    JMP [!$9CD6+5-5+X]                 ; @BT 1F D6 9C
    BBC $A9-7+7,6*6/3-6,$1B*10/5-$1B                  ; @BT D3 A9 1B
    BNE $EB*2-$EB+0                        ; @BT D0 EB
    CLR1 $A3+11-4-7,0+3-1-2                     ; @BT 12 A3
    TCALL 5+5-5                        ; @BT 51
    ROL A                          ; @BT 3C
    DAA A                          ; @BT DF
    SBC (X),(Y)                    ; @BT B9
    BBC $7B-7+7,4*6/3-4,$0D*10/5-$0D                  ; @BT 93 7B 0D
    MOV $85*2-$85+0+X,Y                    ; @BT DB 85
    EOR A,#$0A+11-4-7                     ; @BT 48 0A
    MOV X,SP                       ; @BT 9D
    MOV (X)+,A                     ; @BT AF
    AND1 C,T1_BADDR_002,7          ; @BT 4A 02 E0
    EOR A,$94+3-1-2+X                    ; @BT 54 94
    AND $7E+5-5,#$7E-7+7                   ; @BT 38 7E 7E
    DEC $52*6/3-$52                        ; @BT 8B 52
    ADC A,#$28*10/5-$28                     ; @BT 88 28
    CMP X,$B4*2-$B4+0                      ; @BT 3E B4
    CLRP                           ; @BT 20
    DEC A                          ; @BT 9C
    POP Y                          ; @BT EE
    OR $20+11-4-7,#$20+3-1-2                    ; @BT 18 20 20
    DEC X                          ; @BT 1D
    SBC A,$BB+5-5                      ; @BT A4 BB
    MOVW YA,$E1-7+7                    ; @BT BA E1
    CLR1 $E8*6/3-$E8,3*10/5-3                     ; @BT 72 E8
    MOV Y,!$C007*2-$C007+0                   ; @BT EC 07 C0
    OR A,!$E116+11-4-7+Y                  ; @BT 16 16 E1
    POP A                          ; @BT AE
    MOV A,$2F+3-1-2                      ; @BT E4 2F
    CMP A,[$41+5-5]+Y                  ; @BT 77 41
    CMP X,#$9D-7+7                     ; @BT C8 9D
    EOR A,[$66*6/3-$66+X]                  ; @BT 47 66
    INC $63*10/5-$63+X                      ; @BT BB 63
    MOV $40*2-$40+0,#$40+11-4-7                   ; @BT 8F 40 40
    BBS $4A+3-1-2,5+5-5,$4C-7+7                  ; @BT A3 4A 4C
    DEC !$7BC7*6/3-$7BC7                     ; @BT 8C C7 7B
    ASL !$3121*10/5-$3121                     ; @BT 0C 21 31
    MOV1 T1_BADDR_009,5,C          ; @BT CA 09 A0
    SBC A,[$76*2-$76+0+X]                  ; @BT A7 76
    EOR A,!$89A4+11-4-7+X                 ; @BT 55 A4 89
    TCALL 2+3-1-2                        ; @BT 21
    ADC A,$B2+5-5+X                    ; @BT 94 B2
    OR $37-7+7,$37*6/3-$37                     ; @BT 09 37 37
    INC $7A*10/5-$7A                        ; @BT AB 7A
    CMP Y,$F9*2-$F9+0                      ; @BT 7E F9
    MOV A,Y                        ; @BT DD
    SBC A,#$1A+11-4-7                     ; @BT A8 1A
    CLR1 $D1+3-1-2,2+5-5                     ; @BT 52 D1
    MOVW $F8-7+7,YA                    ; @BT DA F8
    NOP                            ; @BT 00
    PUSH Y                         ; @BT 6D
    DI                             ; @BT C0
    TCALL 6*6/3-6                        ; @BT 61
    OR A,$F2*10/5-$F2                       ; @BT 04 F2
    CLR1 $FF*2-$FF+0,4+11-4-7                     ; @BT 92 FF
    TCALL 3+3-1-2                        ; @BT 31
    OR A,!$E005+5-5+X                  ; @BT 15 05 E0
    INC A                          ; @BT BC
    BCC $8F-7+7                        ; @BT 90 8F
    SETC                           ; @BT 80
    SETP                           ; @BT 40
    MOV X,A                        ; @BT 5D
    ADDW YA,$25*6/3-$25                    ; @BT 7A 25
    MOV $9C*10/5-$9C+Y,X                    ; @BT D9 9C
    TCALL 7*2-7+0                        ; @BT 71
    SET1 $8A+11-4-7,5+3-1-2                     ; @BT A2 8A
    SET1 $73+5-5,4-7+7                     ; @BT 82 73
    SBC $00*6/3-$00,$00*10/5-$00                    ; @BT A9 00 00
    DAA                            ; @BT DF
    MOV $B3*2-$B3+0,Y                      ; @BT CB B3
    OR A,!$E227+11-4-7                    ; @BT 05 27 E2
    AND A,!$2899+3-1-2                   ; @BT 25 99 28
    NOTC                           ; @BT ED
    EOR A,!$8AB5+5-5+Y                 ; @BT 56 B5 8A
    XCN A                          ; @BT 9F
    TCALL 12-7+7                       ; @BT C1
    AND A,!$2677*6/3-$2677+X                 ; @BT 35 77 26
    BBC $C0*10/5-$C0,7*2-7+0,$22+11-4-7                  ; @BT F3 C0 22
    SET1 $17+3-1-2,0+5-5                     ; @BT 02 17
    DAS A                          ; @BT BE
    MOV A,[$EA-7+7+X]                  ; @BT E7 EA
    ADC (X),(Y)                    ; @BT 99
    MOV (X),A                      ; @BT C6
    AND A,$50*6/3-$50                      ; @BT 24 50
    MOV $57*10/5-$57,X                      ; @BT D8 57
    ASL A                          ; @BT 1C
    SBC A,!$0225*2-$0225+0+Y                 ; @BT B6 25 02
    EOR $F0+11-4-7,$F0+3-1-2                    ; @BT 49 F0 F0
    AND (X),(Y)                    ; @BT 39
    EOR $D9+5-5,#$D9-7+7                   ; @BT 58 D9 D9
    PCALL $4E*6/3-$4E                      ; @BT 4F 4E
    MOV !$BDE5*10/5-$BDE5+Y,A                 ; @BT D6 E5 BD
    STOP                           ; @BT FF
    CMP A,$58*2-$58+0+X                    ; @BT 74 58
    TCALL 4+11-4-7                        ; @BT 41
    SET1 $B8+3-1-2,7+5-5                     ; @BT E2 B8
    OR1 C,T1_BADDR_014,5           ; @BT 0A 0E A0
    MOV A,(X)                      ; @BT E6
    CMPW YA,$3E-7+7                    ; @BT 5A 3E
    RET                            ; @BT 6F
    MOV !$BCD4*6/3-$BCD4,X                   ; @BT C9 D4 BC
    BRA $19*10/5-$19                        ; @BT 2F 19
    AND A,$39*2-$39+0+X                    ; @BT 34 39
    ROR $EC+11-4-7                        ; @BT 6B EC
    MOV [$2C+3-1-2]+Y,A                  ; @BT D7 2C
    LSR A                          ; @BT 5C
    LSR !$9DE7+5-5                     ; @BT 4C E7 9D
    SBC A,!$0114-7+7+X                 ; @BT B5 14 01

    .db "<05"               ; @BT END
