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

    ; NOTE: gb-z80 opcodes, as WLA DX understands them, can be found in /igb.c
    ; Please verify using external documentation (e.g. https://www.masswerk.at/gb-z80/gb-z80_instruction_set.html)
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
    ; Auto-generated from igb.c + cross-checked with external gb-z80 ISA reference

    ADC A,A                      ; @BT 8F
    ADC A,B                      ; @BT 88
    ADC A,C                      ; @BT 89
    ADC A,D                      ; @BT 8A
    ADC A,E                      ; @BT 8B
    ADC A,H                      ; @BT 8C
    ADC A,(HL)                   ; @BT 8E
    ADC A,L                      ; @BT 8D
    ADC A,$28                    ; @BT CE 28
    ADC A                        ; @BT 8F
    ADC B                        ; @BT 88
    ADC C                        ; @BT 89
    ADC D                        ; @BT 8A
    ADC E                        ; @BT 8B
    ADC H                        ; @BT 8C
    ADC (HL)                     ; @BT 8E
    ADC L                        ; @BT 8D
    ADC $3F                      ; @BT CE 3F
    ADD A,A                      ; @BT 87
    ADD A,B                      ; @BT 80
    ADD A,C                      ; @BT 81
    ADD A,D                      ; @BT 82
    ADD A,E                      ; @BT 83
    ADD A,H                      ; @BT 84
    ADD A,(HL)                   ; @BT 86
    ADD A,L                      ; @BT 85
    ADD A,$56                    ; @BT C6 56
    ADD A                        ; @BT 87
    ADD B                        ; @BT 80
    ADD C                        ; @BT 81
    ADD D                        ; @BT 82
    ADD E                        ; @BT 83
    ADD H                        ; @BT 84
    ADD (HL)                     ; @BT 86
    ADD HL,BC                    ; @BT 09
    ADD HL,DE                    ; @BT 19
    ADD HL,HL                    ; @BT 29
    ADD HL,SP                    ; @BT 39
    ADD L                        ; @BT 85
    ADD SP,$2D                   ; @BT E8 2D
    ADD $84                      ; @BT C6 84
    AND A,A                      ; @BT A7
    AND A,B                      ; @BT A0
    AND A,C                      ; @BT A1
    AND A,D                      ; @BT A2
    AND A,E                      ; @BT A3
    AND A,H                      ; @BT A4
    AND A,(HL)                   ; @BT A6
    AND A,L                      ; @BT A5
    AND A,$9B                    ; @BT E6 9B
    AND A                        ; @BT A7
    AND B                        ; @BT A0
    AND C                        ; @BT A1
    AND D                        ; @BT A2
    AND E                        ; @BT A3
    AND H                        ; @BT A4
    AND (HL)                     ; @BT A6
    AND L                        ; @BT A5
    AND $B2                      ; @BT E6 B2
    BIT 0,A                      ; @BT CB 47
    BIT 0,B                      ; @BT CB 40
    BIT 0,C                      ; @BT CB 41
    BIT 0,D                      ; @BT CB 42
    BIT 0,E                      ; @BT CB 43
    BIT 0,H                      ; @BT CB 44
    BIT 0,(HL)                   ; @BT CB 46
    BIT 0,L                      ; @BT CB 45
    BIT 1,A                      ; @BT CB 4F
    BIT 1,B                      ; @BT CB 48
    BIT 1,C                      ; @BT CB 49
    BIT 1,D                      ; @BT CB 4A
    BIT 1,E                      ; @BT CB 4B
    BIT 1,H                      ; @BT CB 4C
    BIT 1,(HL)                   ; @BT CB 4E
    BIT 1,L                      ; @BT CB 4D
    BIT 2,A                      ; @BT CB 57
    BIT 2,B                      ; @BT CB 50
    BIT 2,C                      ; @BT CB 51
    BIT 2,D                      ; @BT CB 52
    BIT 2,E                      ; @BT CB 53
    BIT 2,H                      ; @BT CB 54
    BIT 2,(HL)                   ; @BT CB 56
    BIT 2,L                      ; @BT CB 55
    BIT 3,A                      ; @BT CB 5F
    BIT 3,B                      ; @BT CB 58
    BIT 3,C                      ; @BT CB 59
    BIT 3,D                      ; @BT CB 5A
    BIT 3,E                      ; @BT CB 5B
    BIT 3,H                      ; @BT CB 5C
    BIT 3,(HL)                   ; @BT CB 5E
    BIT 3,L                      ; @BT CB 5D
    BIT 4,A                      ; @BT CB 67
    BIT 4,B                      ; @BT CB 60
    BIT 4,C                      ; @BT CB 61
    BIT 4,D                      ; @BT CB 62
    BIT 4,E                      ; @BT CB 63
    BIT 4,H                      ; @BT CB 64
    BIT 4,(HL)                   ; @BT CB 66
    BIT 4,L                      ; @BT CB 65
    BIT 5,A                      ; @BT CB 6F
    BIT 5,B                      ; @BT CB 68
    BIT 5,C                      ; @BT CB 69
    BIT 5,D                      ; @BT CB 6A
    BIT 5,E                      ; @BT CB 6B
    BIT 5,H                      ; @BT CB 6C
    BIT 5,(HL)                   ; @BT CB 6E
    BIT 5,L                      ; @BT CB 6D
    BIT 6,A                      ; @BT CB 77
    BIT 6,B                      ; @BT CB 70
    BIT 6,C                      ; @BT CB 71
    BIT 6,D                      ; @BT CB 72
    BIT 6,E                      ; @BT CB 73
    BIT 6,H                      ; @BT CB 74
    BIT 6,(HL)                   ; @BT CB 76
    BIT 6,L                      ; @BT CB 75
    BIT 7,A                      ; @BT CB 7F
    BIT 7,B                      ; @BT CB 78
    BIT 7,C                      ; @BT CB 79
    BIT 7,D                      ; @BT CB 7A
    BIT 7,E                      ; @BT CB 7B
    BIT 7,H                      ; @BT CB 7C
    BIT 7,(HL)                   ; @BT CB 7E
    BIT 7,L                      ; @BT CB 7D
    CALL LSS,$1345               ; @BT DC 45 13
    CALL LST,$1456               ; @BT DC 56 14
    CALL GTE,$1567               ; @BT D4 67 15
    CALL GEQ,$1678               ; @BT D4 78 16
    CALL NEQ,$1789               ; @BT C4 89 17
    CALL EQU,$189A               ; @BT CC 9A 18
    CALL C,$19AB                 ; @BT DC AB 19
    CALL NC,$1ABC                ; @BT D4 BC 1A
    CALL NZ,$1BCD                ; @BT C4 CD 1B
    CALL Z,$1CDE                 ; @BT CC DE 1C
    CALL $1DEF                   ; @BT CD EF 1D
    CCF                          ; @BT 3F
    CP A,A                       ; @BT BF
    CP A,B                       ; @BT B8
    CP A,C                       ; @BT B9
    CP A,D                       ; @BT BA
    CP A,E                       ; @BT BB
    CP A,H                       ; @BT BC
    CP A,(HL)                    ; @BT BE
    CP A,L                       ; @BT BD
    CP A,$C9                     ; @BT FE C9
    CP A                         ; @BT BF
    CP B                         ; @BT B8
    CP C                         ; @BT B9
    CP D                         ; @BT BA
    CP E                         ; @BT BB
    CP H                         ; @BT BC
    CP (HL)                      ; @BT BE
    CP L                         ; @BT BD
    CP $E0                       ; @BT FE E0
    CPL A                        ; @BT 2F
    CPL                          ; @BT 2F
    DAA                          ; @BT 27
    DEC A                        ; @BT 3D
    DEC B                        ; @BT 05
    DEC BC                       ; @BT 0B
    DEC C                        ; @BT 0D
    DEC D                        ; @BT 15
    DEC DE                       ; @BT 1B
    DEC E                        ; @BT 1D
    DEC H                        ; @BT 25
    DEC HL                       ; @BT 2B
    DEC (HL)                     ; @BT 35
    DEC L                        ; @BT 2D
    DEC SP                       ; @BT 3B
    DI                           ; @BT F3
    EI                           ; @BT FB
    HALT                         ; @BT 76
    INC A                        ; @BT 3C
    INC B                        ; @BT 04
    INC BC                       ; @BT 03
    INC C                        ; @BT 0C
    INC D                        ; @BT 14
    INC DE                       ; @BT 13
    INC E                        ; @BT 1C
    INC H                        ; @BT 24
    INC HL                       ; @BT 23
    INC (HL)                     ; @BT 34
    INC L                        ; @BT 2C
    INC SP                       ; @BT 33
    JP LSS,$1F00                 ; @BT DA 00 1F
    JP LST,$2011                 ; @BT DA 11 20
    JP GTE,$2122                 ; @BT D2 22 21
    JP GEQ,$2233                 ; @BT D2 33 22
    JP NEQ,$2344                 ; @BT C2 44 23
    JP EQU,$2455                 ; @BT CA 55 24
    JP C,$2566                   ; @BT DA 66 25
    JP (HL)                      ; @BT E9
    JP HL                        ; @BT E9
    JP NC,$2677                  ; @BT D2 77 26
    JP NZ,$2788                  ; @BT C2 88 27
    JP Z,$2899                   ; @BT CA 99 28
    JP $29AA                     ; @BT C3 AA 29
    JR LSS,$37                   ; @BT 38 37
    JR LST,$0E                   ; @BT 38 0E
    JR GTE,$25                   ; @BT 30 25
    JR GEQ,$3C                   ; @BT 30 3C
    JR NEQ,$13                   ; @BT 20 13
    JR EQU,$2A                   ; @BT 28 2A
    JR C,$01                     ; @BT 38 01
    JR NC,$18                    ; @BT 30 18
    JR NZ,$2F                    ; @BT 20 2F
    JR Z,$06                     ; @BT 28 06
    JR $1D                       ; @BT 18 1D
    LD A,A                       ; @BT 7F
    LD A,B                       ; @BT 78
    LD A,C                       ; @BT 79
    LD A,D                       ; @BT 7A
    LD A,E                       ; @BT 7B
    LD A,H                       ; @BT 7C
    LD A,L                       ; @BT 7D
    LD A,($FF00+C)               ; @BT F2
    LD A,($FF00+$F4)             ; @BT F0 F4
    LD A,(BC)                    ; @BT 0A
    LD A,(DE)                    ; @BT 1A
    LD A,(HL+)                   ; @BT 2A
    LD A,(HL-)                   ; @BT 3A
    LD A,(HL)                    ; @BT 7E
    LD A,(HLD)                   ; @BT 3A
    LD A,(HLI)                   ; @BT 2A
    LD A,($2ABB)                 ; @BT FA BB 2A
    LD A,$0B                     ; @BT 3E 0B
    LD B,A                       ; @BT 47
    LD B,B                       ; @BT 40
    LD B,C                       ; @BT 41
    LD (BC),A                    ; @BT 02
    LD B,D                       ; @BT 42
    LD B,E                       ; @BT 43
    LD B,H                       ; @BT 44
    LD B,(HL)                    ; @BT 46
    LD B,L                       ; @BT 45
    LD B,$22                     ; @BT 06 22
    LD C,A                       ; @BT 4F
    LD C,B                       ; @BT 48
    LD C,C                       ; @BT 49
    LD C,D                       ; @BT 4A
    LD C,E                       ; @BT 4B
    LD C,H                       ; @BT 4C
    LD C,(HL)                    ; @BT 4E
    LD C,L                       ; @BT 4D
    LD C,$39                     ; @BT 0E 39
    LD D,A                       ; @BT 57
    LD D,B                       ; @BT 50
    LD D,C                       ; @BT 51
    LD D,D                       ; @BT 52
    LD D,E                       ; @BT 53
    LD (DE),A                    ; @BT 12
    LD D,H                       ; @BT 54
    LD D,(HL)                    ; @BT 56
    LD D,L                       ; @BT 55
    LD D,$50                     ; @BT 16 50
    LD E,A                       ; @BT 5F
    LD E,B                       ; @BT 58
    LD E,C                       ; @BT 59
    LD E,D                       ; @BT 5A
    LD E,E                       ; @BT 5B
    LD E,H                       ; @BT 5C
    LD E,(HL)                    ; @BT 5E
    LD E,L                       ; @BT 5D
    LD E,$67                     ; @BT 1E 67
    LD ($FF00+C),A               ; @BT E2
    LD ($FF00+$7E),A             ; @BT E0 7E
    LD H,A                       ; @BT 67
    LD H,B                       ; @BT 60
    LD H,C                       ; @BT 61
    LD H,D                       ; @BT 62
    LD H,E                       ; @BT 63
    LD H,H                       ; @BT 64
    LD H,(HL)                    ; @BT 66
    LD H,L                       ; @BT 65
    LD (HL+),A                   ; @BT 22
    LD (HL-),A                   ; @BT 32
    LD (HL),A                    ; @BT 77
    LD (HL),B                    ; @BT 70
    LD (HL),C                    ; @BT 71
    LD (HL),D                    ; @BT 72
    LD (HLD),A                   ; @BT 32
    LD (HL),E                    ; @BT 73
    LD (HL),H                    ; @BT 74
    LD (HLI),A                   ; @BT 22
    LD (HL),L                    ; @BT 75
    LD HL,SP+$15                 ; @BT F8 15
    LD HL,SP-$2C                 ; @BT F8 D4
    LD (HL),$C3                  ; @BT 36 C3
    LD H,$DA                     ; @BT 26 DA
    LD L,A                       ; @BT 6F
    LD L,B                       ; @BT 68
    LD L,C                       ; @BT 69
    LD L,D                       ; @BT 6A
    LD L,E                       ; @BT 6B
    LD L,H                       ; @BT 6C
    LD L,(HL)                    ; @BT 6E
    LD L,L                       ; @BT 6D
    LD L,$F1                     ; @BT 2E F1
    LD PC,HL                     ; @BT E9
    LD SP,HL                     ; @BT F9
    LD BC,$2BCC                  ; @BT 01 CC 2B
    LD DE,$2CDD                  ; @BT 11 DD 2C
    LD HL,$2DEE                  ; @BT 21 EE 2D
    LD SP,$2EFF                  ; @BT 31 FF 2E
    LD ($3010),SP                ; @BT 08 10 30
    LD ($3121),A                 ; @BT EA 21 31
    LDD A,(HL)                   ; @BT 3A
    LDD (HL),A                   ; @BT 32
    LDH A,(C)                    ; @BT F2
    LDH A,($08)                  ; @BT F0 08
    LDH (C),A                    ; @BT E2
    LDH ($1F),A                  ; @BT E0 1F
    LDI A,(HL)                   ; @BT 2A
    LDI (HL),A                   ; @BT 22
    NOP                          ; @BT 00
    OR A,A                       ; @BT B7
    OR A,B                       ; @BT B0
    OR A,C                       ; @BT B1
    OR A,D                       ; @BT B2
    OR A,E                       ; @BT B3
    OR A,H                       ; @BT B4
    OR A,(HL)                    ; @BT B6
    OR A,L                       ; @BT B5
    OR A,$36                     ; @BT F6 36
    OR A                         ; @BT B7
    OR B                         ; @BT B0
    OR C                         ; @BT B1
    OR D                         ; @BT B2
    OR E                         ; @BT B3
    OR H                         ; @BT B4
    OR (HL)                      ; @BT B6
    OR L                         ; @BT B5
    OR $4D                       ; @BT F6 4D
    POP AF                       ; @BT F1
    POP BC                       ; @BT C1
    POP DE                       ; @BT D1
    POP HL                       ; @BT E1
    PUSH AF                      ; @BT F5
    PUSH BC                      ; @BT C5
    PUSH DE                      ; @BT D5
    PUSH HL                      ; @BT E5
    RES 0,A                      ; @BT CB 87
    RES 0,B                      ; @BT CB 80
    RES 0,C                      ; @BT CB 81
    RES 0,D                      ; @BT CB 82
    RES 0,E                      ; @BT CB 83
    RES 0,H                      ; @BT CB 84
    RES 0,(HL)                   ; @BT CB 86
    RES 0,L                      ; @BT CB 85
    RES 1,A                      ; @BT CB 8F
    RES 1,B                      ; @BT CB 88
    RES 1,C                      ; @BT CB 89
    RES 1,D                      ; @BT CB 8A
    RES 1,E                      ; @BT CB 8B
    RES 1,H                      ; @BT CB 8C
    RES 1,(HL)                   ; @BT CB 8E
    RES 1,L                      ; @BT CB 8D
    RES 2,A                      ; @BT CB 97
    RES 2,B                      ; @BT CB 90
    RES 2,C                      ; @BT CB 91
    RES 2,D                      ; @BT CB 92
    RES 2,E                      ; @BT CB 93
    RES 2,H                      ; @BT CB 94
    RES 2,(HL)                   ; @BT CB 96
    RES 2,L                      ; @BT CB 95
    RES 3,A                      ; @BT CB 9F
    RES 3,B                      ; @BT CB 98
    RES 3,C                      ; @BT CB 99
    RES 3,D                      ; @BT CB 9A
    RES 3,E                      ; @BT CB 9B
    RES 3,H                      ; @BT CB 9C
    RES 3,(HL)                   ; @BT CB 9E
    RES 3,L                      ; @BT CB 9D
    RES 4,A                      ; @BT CB A7
    RES 4,B                      ; @BT CB A0
    RES 4,C                      ; @BT CB A1
    RES 4,D                      ; @BT CB A2
    RES 4,E                      ; @BT CB A3
    RES 4,H                      ; @BT CB A4
    RES 4,(HL)                   ; @BT CB A6
    RES 4,L                      ; @BT CB A5
    RES 5,A                      ; @BT CB AF
    RES 5,B                      ; @BT CB A8
    RES 5,C                      ; @BT CB A9
    RES 5,D                      ; @BT CB AA
    RES 5,E                      ; @BT CB AB
    RES 5,H                      ; @BT CB AC
    RES 5,(HL)                   ; @BT CB AE
    RES 5,L                      ; @BT CB AD
    RES 6,A                      ; @BT CB B7
    RES 6,B                      ; @BT CB B0
    RES 6,C                      ; @BT CB B1
    RES 6,D                      ; @BT CB B2
    RES 6,E                      ; @BT CB B3
    RES 6,H                      ; @BT CB B4
    RES 6,(HL)                   ; @BT CB B6
    RES 6,L                      ; @BT CB B5
    RES 7,A                      ; @BT CB BF
    RES 7,B                      ; @BT CB B8
    RES 7,C                      ; @BT CB B9
    RES 7,D                      ; @BT CB BA
    RES 7,E                      ; @BT CB BB
    RES 7,H                      ; @BT CB BC
    RES 7,(HL)                   ; @BT CB BE
    RES 7,L                      ; @BT CB BD
    RETI                         ; @BT D9
    RET LSS                      ; @BT D8
    RET LST                      ; @BT D8
    RET GTE                      ; @BT D0
    RET GEQ                      ; @BT D0
    RET NEQ                      ; @BT C0
    RET EQU                      ; @BT C8
    RET C                        ; @BT D8
    RET NC                       ; @BT D0
    RET NZ                       ; @BT C0
    RET Z                        ; @BT C8
    RET                          ; @BT C9
    RL A                         ; @BT CB 17
    RL B                         ; @BT CB 10
    RL C                         ; @BT CB 11
    RL D                         ; @BT CB 12
    RL E                         ; @BT CB 13
    RL H                         ; @BT CB 14
    RL (HL)                      ; @BT CB 16
    RL L                         ; @BT CB 15
    RLA                          ; @BT 17
    RLCA                         ; @BT 07
    RLC A                        ; @BT CB 07
    RLC B                        ; @BT CB 00
    RLC C                        ; @BT CB 01
    RLC D                        ; @BT CB 02
    RLC E                        ; @BT CB 03
    RLC H                        ; @BT CB 04
    RLC (HL)                     ; @BT CB 06
    RLC L                        ; @BT CB 05
    RR A                         ; @BT CB 1F
    RR B                         ; @BT CB 18
    RR C                         ; @BT CB 19
    RR D                         ; @BT CB 1A
    RR E                         ; @BT CB 1B
    RR H                         ; @BT CB 1C
    RR (HL)                      ; @BT CB 1E
    RR L                         ; @BT CB 1D
    RRA                          ; @BT 1F
    RRCA                         ; @BT 0F
    RRC A                        ; @BT CB 0F
    RRC B                        ; @BT CB 08
    RRC C                        ; @BT CB 09
    RRC D                        ; @BT CB 0A
    RRC E                        ; @BT CB 0B
    RRC H                        ; @BT CB 0C
    RRC (HL)                     ; @BT CB 0E
    RRC L                        ; @BT CB 0D
    RST $00                      ; @BT C7
    RST $08                      ; @BT CF
    RST $10                      ; @BT D7
    RST $18                      ; @BT DF
    RST $20                      ; @BT E7
    RST $28                      ; @BT EF
    RST $30                      ; @BT F7
    RST $38                      ; @BT FF
    SBC A,A                      ; @BT 9F
    SBC A,B                      ; @BT 98
    SBC A,C                      ; @BT 99
    SBC A,D                      ; @BT 9A
    SBC A,E                      ; @BT 9B
    SBC A,H                      ; @BT 9C
    SBC A,(HL)                   ; @BT 9E
    SBC A,L                      ; @BT 9D
    SBC A,$64                    ; @BT DE 64
    SBC A                        ; @BT 9F
    SBC B                        ; @BT 98
    SBC C                        ; @BT 99
    SBC D                        ; @BT 9A
    SBC E                        ; @BT 9B
    SBC H                        ; @BT 9C
    SBC (HL)                     ; @BT 9E
    SBC L                        ; @BT 9D
    SBC $7B                      ; @BT DE 7B
    SCF                          ; @BT 37
    SET 0,A                      ; @BT CB C7
    SET 0,B                      ; @BT CB C0
    SET 0,C                      ; @BT CB C1
    SET 0,D                      ; @BT CB C2
    SET 0,E                      ; @BT CB C3
    SET 0,H                      ; @BT CB C4
    SET 0,(HL)                   ; @BT CB C6
    SET 0,L                      ; @BT CB C5
    SET 1,A                      ; @BT CB CF
    SET 1,B                      ; @BT CB C8
    SET 1,C                      ; @BT CB C9
    SET 1,D                      ; @BT CB CA
    SET 1,E                      ; @BT CB CB
    SET 1,H                      ; @BT CB CC
    SET 1,(HL)                   ; @BT CB CE
    SET 1,L                      ; @BT CB CD
    SET 2,A                      ; @BT CB D7
    SET 2,B                      ; @BT CB D0
    SET 2,C                      ; @BT CB D1
    SET 2,D                      ; @BT CB D2
    SET 2,E                      ; @BT CB D3
    SET 2,H                      ; @BT CB D4
    SET 2,(HL)                   ; @BT CB D6
    SET 2,L                      ; @BT CB D5
    SET 3,A                      ; @BT CB DF
    SET 3,B                      ; @BT CB D8
    SET 3,C                      ; @BT CB D9
    SET 3,D                      ; @BT CB DA
    SET 3,E                      ; @BT CB DB
    SET 3,H                      ; @BT CB DC
    SET 3,(HL)                   ; @BT CB DE
    SET 3,L                      ; @BT CB DD
    SET 4,A                      ; @BT CB E7
    SET 4,B                      ; @BT CB E0
    SET 4,C                      ; @BT CB E1
    SET 4,D                      ; @BT CB E2
    SET 4,E                      ; @BT CB E3
    SET 4,H                      ; @BT CB E4
    SET 4,(HL)                   ; @BT CB E6
    SET 4,L                      ; @BT CB E5
    SET 5,A                      ; @BT CB EF
    SET 5,B                      ; @BT CB E8
    SET 5,C                      ; @BT CB E9
    SET 5,D                      ; @BT CB EA
    SET 5,E                      ; @BT CB EB
    SET 5,H                      ; @BT CB EC
    SET 5,(HL)                   ; @BT CB EE
    SET 5,L                      ; @BT CB ED
    SET 6,A                      ; @BT CB F7
    SET 6,B                      ; @BT CB F0
    SET 6,C                      ; @BT CB F1
    SET 6,D                      ; @BT CB F2
    SET 6,E                      ; @BT CB F3
    SET 6,H                      ; @BT CB F4
    SET 6,(HL)                   ; @BT CB F6
    SET 6,L                      ; @BT CB F5
    SET 7,A                      ; @BT CB FF
    SET 7,B                      ; @BT CB F8
    SET 7,C                      ; @BT CB F9
    SET 7,D                      ; @BT CB FA
    SET 7,E                      ; @BT CB FB
    SET 7,H                      ; @BT CB FC
    SET 7,(HL)                   ; @BT CB FE
    SET 7,L                      ; @BT CB FD
    SLA A                        ; @BT CB 27
    SLA B                        ; @BT CB 20
    SLA C                        ; @BT CB 21
    SLA D                        ; @BT CB 22
    SLA E                        ; @BT CB 23
    SLA H                        ; @BT CB 24
    SLA (HL)                     ; @BT CB 26
    SLA L                        ; @BT CB 25
    SRA A                        ; @BT CB 2F
    SRA B                        ; @BT CB 28
    SRA C                        ; @BT CB 29
    SRA D                        ; @BT CB 2A
    SRA E                        ; @BT CB 2B
    SRA H                        ; @BT CB 2C
    SRA (HL)                     ; @BT CB 2E
    SRA L                        ; @BT CB 2D
    SRL A                        ; @BT CB 3F
    SRL B                        ; @BT CB 38
    SRL C                        ; @BT CB 39
    SRL D                        ; @BT CB 3A
    SRL E                        ; @BT CB 3B
    SRL H                        ; @BT CB 3C
    SRL (HL)                     ; @BT CB 3E
    SRL L                        ; @BT CB 3D
    STOP $92                     ; @BT 10 92
    STOP                         ; @BT 10
    SUB A,A                      ; @BT 97
    SUB A,B                      ; @BT 90
    SUB A,C                      ; @BT 91
    SUB A,D                      ; @BT 92
    SUB A,E                      ; @BT 93
    SUB A,H                      ; @BT 94
    SUB A,(HL)                   ; @BT 96
    SUB A,L                      ; @BT 95
    SUB A,$A9                    ; @BT D6 A9
    SUB A                        ; @BT 97
    SUB B                        ; @BT 90
    SUB C                        ; @BT 91
    SUB D                        ; @BT 92
    SUB E                        ; @BT 93
    SUB H                        ; @BT 94
    SUB (HL)                     ; @BT 96
    SUB L                        ; @BT 95
    SUB $C0                      ; @BT D6 C0
    SWAP A                       ; @BT CB 37
    SWAP B                       ; @BT CB 30
    SWAP C                       ; @BT CB 31
    SWAP D                       ; @BT CB 32
    SWAP E                       ; @BT CB 33
    SWAP H                       ; @BT CB 34
    SWAP (HL)                    ; @BT CB 36
    SWAP L                       ; @BT CB 35
    XOR A,A                      ; @BT AF
    XOR A,B                      ; @BT A8
    XOR A,C                      ; @BT A9
    XOR A,D                      ; @BT AA
    XOR A,E                      ; @BT AB
    XOR A,H                      ; @BT AC
    XOR A,(HL)                   ; @BT AE
    XOR A,L                      ; @BT AD
    XOR A,$D7                    ; @BT EE D7
    XOR A                        ; @BT AF
    XOR B                        ; @BT A8
    XOR C                        ; @BT A9
    XOR D                        ; @BT AA
    XOR E                        ; @BT AB
    XOR H                        ; @BT AC
    XOR (HL)                     ; @BT AE
    XOR L                        ; @BT AD
    XOR $EE                      ; @BT EE EE
    .db "<01"               ; @BT END

    /////////////////////////////////////////////////////////////////////////////
    // TEST-02: same opcodes as TEST-01, randomised order, branches spread out
    /////////////////////////////////////////////////////////////////////////////

    .db "02>"               ; @BT TEST-02 02 START

    ; GENERATED: copied from TEST-01 with full TEST-02 randomization
    ; GENERATED: seed = 1704833320

    AND A,D                      ; @BT A2
    LD (HL),A                    ; @BT 77
    ADC D                        ; @BT 8A
    SCF                          ; @BT 37
    ADD A,E                      ; @BT 83
    LD A,$0B                     ; @BT 3E 0B
    CP A,C                       ; @BT B9
    SET 1,A                      ; @BT CB CF
    LD A,(HLI)                   ; @BT 2A
    LDH (C),A                    ; @BT E2
    ADD A,L                      ; @BT 85
    BIT 2,A                      ; @BT CB 57
    RLCA                         ; @BT 07
    ADC H                        ; @BT 8C
    RL A                         ; @BT CB 17
    LD H,A                       ; @BT 67
    SET 6,B                      ; @BT CB F0
    SRA E                        ; @BT CB 2B
    SET 1,(HL)                   ; @BT CB CE
    DEC DE                       ; @BT 1B
    SLA C                        ; @BT CB 21
    JP C,$2566                   ; @BT DA 66 25
    ADC A,B                      ; @BT 88
    SET 2,H                      ; @BT CB D4
    LD A,($2ABB)                 ; @BT FA BB 2A
    CP H                         ; @BT BC
    SRA D                        ; @BT CB 2A
    RLC E                        ; @BT CB 03
    ADC L                        ; @BT 8D
    CP A,L                       ; @BT BD
    RES 7,C                      ; @BT CB B9
    LD D,A                       ; @BT 57
    LD L,B                       ; @BT 68
    RES 2,D                      ; @BT CB 92
    BIT 4,D                      ; @BT CB 62
    SLA H                        ; @BT CB 24
    SET 3,C                      ; @BT CB D9
    SLA E                        ; @BT CB 23
    SRL H                        ; @BT CB 3C
    SET 7,D                      ; @BT CB FA
    EI                           ; @BT FB
    SRL L                        ; @BT CB 3D
    SRL A                        ; @BT CB 3F
    ADC (HL)                     ; @BT 8E
    INC SP                       ; @BT 33
    ADD A,A                      ; @BT 87
    SET 3,L                      ; @BT CB DD
    AND A                        ; @BT A7
    ADD A,H                      ; @BT 84
    CPL                          ; @BT 2F
    RLA                          ; @BT 17
    RES 2,E                      ; @BT CB 93
    SBC A,(HL)                   ; @BT 9E
    ADD A,D                      ; @BT 82
    BIT 0,C                      ; @BT CB 41
    BIT 5,B                      ; @BT CB 68
    CP B                         ; @BT B8
    RES 3,C                      ; @BT CB 99
    RLC D                        ; @BT CB 02
    LD A,(HLD)                   ; @BT 3A
    SET 1,C                      ; @BT CB C9
    ADC A                        ; @BT 8F
    SET 2,D                      ; @BT CB D2
    BIT 7,(HL)                   ; @BT CB 7E
    LD A,L                       ; @BT 7D
    RES 6,C                      ; @BT CB B1
    SBC H                        ; @BT 9C
    SBC D                        ; @BT 9A
    XOR A,C                      ; @BT A9
    SET 3,D                      ; @BT CB DA
    JP NZ,$2788                  ; @BT C2 88 27
    BIT 0,E                      ; @BT CB 43
    RES 6,H                      ; @BT CB B4
    SET 1,E                      ; @BT CB CB
    BIT 7,L                      ; @BT CB 7D
    SET 3,A                      ; @BT CB DF
    PUSH AF                      ; @BT F5
    RETI                         ; @BT D9
    XOR D                        ; @BT AA
    SUB A,(HL)                   ; @BT 96
    SWAP L                       ; @BT CB 35
    LD E,(HL)                    ; @BT 5E
    RR E                         ; @BT CB 1B
    SWAP D                       ; @BT CB 32
    SRL B                        ; @BT CB 38
    SET 2,A                      ; @BT CB D7
    CALL NEQ,$1789               ; @BT C4 89 17
    LD A,(HL)                    ; @BT 7E
    BIT 6,C                      ; @BT CB 71
    XOR E                        ; @BT AB
    RES 7,B                      ; @BT CB B8
    XOR L                        ; @BT AD
    ADD HL,HL                    ; @BT 29
    CP D                         ; @BT BA
    SET 4,H                      ; @BT CB E4
    RES 1,B                      ; @BT CB 88
    SLA A                        ; @BT CB 27
    ADD HL,BC                    ; @BT 09
    RST $28                      ; @BT EF
    SET 4,A                      ; @BT CB E7
    BIT 2,(HL)                   ; @BT CB 56
    RES 3,L                      ; @BT CB 9D
    LD L,A                       ; @BT 6F
    AND (HL)                     ; @BT A6
    RES 2,B                      ; @BT CB 90
    SWAP C                       ; @BT CB 31
    RES 5,A                      ; @BT CB AF
    RES 7,A                      ; @BT CB BF
    LD B,A                       ; @BT 47
    LD (DE),A                    ; @BT 12
    BIT 6,B                      ; @BT CB 70
    ADC A,$28                    ; @BT CE 28
    LD H,H                       ; @BT 64
    RES 2,(HL)                   ; @BT CB 96
    JP LSS,$1F00                 ; @BT DA 00 1F
    BIT 1,C                      ; @BT CB 49
    JR C,$01                     ; @BT 38 01
    SET 0,L                      ; @BT CB C5
    SRL E                        ; @BT CB 3B
    RET                          ; @BT C9
    RRC A                        ; @BT CB 0F
    BIT 5,L                      ; @BT CB 6D
    RES 2,A                      ; @BT CB 97
    RR C                         ; @BT CB 19
    JP HL                        ; @BT E9
    XOR A,A                      ; @BT AF
    XOR B                        ; @BT A8
    INC D                        ; @BT 14
    ADC E                        ; @BT 8B
    CP A,A                       ; @BT BF
    RET LSS                      ; @BT D8
    XOR A,L                      ; @BT AD
    SUB A,H                      ; @BT 94
    SET 3,H                      ; @BT CB DC
    LD H,L                       ; @BT 65
    RES 0,L                      ; @BT CB 85
    SET 2,(HL)                   ; @BT CB D6
    RES 5,C                      ; @BT CB A9
    SUB (HL)                     ; @BT 96
    LD D,E                       ; @BT 53
    XOR H                        ; @BT AC
    ADD L                        ; @BT 85
    SWAP A                       ; @BT CB 37
    SET 4,(HL)                   ; @BT CB E6
    LD D,$50                     ; @BT 16 50
    SET 0,H                      ; @BT CB C4
    BIT 3,L                      ; @BT CB 5D
    SUB A                        ; @BT 97
    RLC L                        ; @BT CB 05
    SET 1,L                      ; @BT CB CD
    LD B,H                       ; @BT 44
    LD D,B                       ; @BT 50
    AND L                        ; @BT A5
    LD E,L                       ; @BT 5D
    INC A                        ; @BT 3C
    BIT 1,(HL)                   ; @BT CB 4E
    BIT 1,A                      ; @BT CB 4F
    ADD A,$56                    ; @BT C6 56
    BIT 5,E                      ; @BT CB 6B
    SET 2,C                      ; @BT CB D1
    LD H,$DA                     ; @BT 26 DA
    SWAP E                       ; @BT CB 33
    INC C                        ; @BT 0C
    RES 6,(HL)                   ; @BT CB B6
    LD C,$39                     ; @BT 0E 39
    RES 6,B                      ; @BT CB B0
    BIT 0,B                      ; @BT CB 40
    BIT 6,E                      ; @BT CB 73
    SRA (HL)                     ; @BT CB 2E
    BIT 7,H                      ; @BT CB 7C
    SET 6,L                      ; @BT CB F5
    AND A,A                      ; @BT A7
    BIT 3,C                      ; @BT CB 59
    SBC A,D                      ; @BT 9A
    SET 1,D                      ; @BT CB CA
    RES 2,L                      ; @BT CB 95
    SUB B                        ; @BT 90
    SUB $C0                      ; @BT D6 C0
    BIT 0,H                      ; @BT CB 44
    OR L                         ; @BT B5
    CALL GTE,$1567               ; @BT D4 67 15
    RES 7,L                      ; @BT CB BD
    AND D                        ; @BT A2
    CP A,E                       ; @BT BB
    LDH ($1F),A                  ; @BT E0 1F
    BIT 1,E                      ; @BT CB 4B
    LDI A,(HL)                   ; @BT 2A
    DEC C                        ; @BT 0D
    LD L,(HL)                    ; @BT 6E
    OR A,$36                     ; @BT F6 36
    CALL LSS,$1345               ; @BT DC 45 13
    LD B,L                       ; @BT 45
    XOR C                        ; @BT A9
    ADD A                        ; @BT 87
    LD H,C                       ; @BT 61
    AND B                        ; @BT A0
    ADC B                        ; @BT 88
    LD L,C                       ; @BT 69
    BIT 0,A                      ; @BT CB 47
    RES 5,B                      ; @BT CB A8
    LD SP,$2EFF                  ; @BT 31 FF 2E
    BIT 3,(HL)                   ; @BT CB 5E
    CPL A                        ; @BT 2F
    LD (HL),C                    ; @BT 71
    RST $10                      ; @BT D7
    BIT 2,D                      ; @BT CB 52
    RR D                         ; @BT CB 1A
    ADD (HL)                     ; @BT 86
    SRA C                        ; @BT CB 29
    LD L,E                       ; @BT 6B
    XOR A                        ; @BT AF
    SUB A,A                      ; @BT 97
    RRC (HL)                     ; @BT CB 0E
    SET 7,L                      ; @BT CB FD
    JR GEQ,$3C                   ; @BT 30 3C
    ADD D                        ; @BT 82
    RLC (HL)                     ; @BT CB 06
    SET 7,(HL)                   ; @BT CB FE
    RST $08                      ; @BT CF
    LD (HL),$C3                  ; @BT 36 C3
    DEC H                        ; @BT 25
    RES 3,H                      ; @BT CB 9C
    JP NC,$2677                  ; @BT D2 77 26
    LD L,L                       ; @BT 6D
    LD D,D                       ; @BT 52
    RES 1,D                      ; @BT CB 8A
    SLA B                        ; @BT CB 20
    SET 3,B                      ; @BT CB D8
    RES 3,(HL)                   ; @BT CB 9E
    OR A                         ; @BT B7
    ADC A,C                      ; @BT 89
    LD C,D                       ; @BT 4A
    LD D,H                       ; @BT 54
    HALT                         ; @BT 76
    RES 4,H                      ; @BT CB A4
    SLA L                        ; @BT CB 25
    SUB A,B                      ; @BT 90
    RL D                         ; @BT CB 12
    SET 6,E                      ; @BT CB F3
    BIT 7,E                      ; @BT CB 7B
    SBC (HL)                     ; @BT 9E
    SUB L                        ; @BT 95
    SBC A                        ; @BT 9F
    SET 0,A                      ; @BT CB C7
    BIT 1,B                      ; @BT CB 48
    BIT 2,B                      ; @BT CB 50
    JP Z,$2899                   ; @BT CA 99 28
    SUB A,L                      ; @BT 95
    CP A,$C9                     ; @BT FE C9
    JR Z,$06                     ; @BT 28 06
    RES 4,B                      ; @BT CB A0
    RL L                         ; @BT CB 15
    ADD HL,SP                    ; @BT 39
    BIT 7,D                      ; @BT CB 7A
    BIT 3,D                      ; @BT CB 5A
    BIT 4,C                      ; @BT CB 61
    SUB E                        ; @BT 93
    LD E,H                       ; @BT 5C
    INC HL                       ; @BT 23
    SET 7,H                      ; @BT CB FC
    SBC A,H                      ; @BT 9C
    RET NZ                       ; @BT C0
    LD B,B                       ; @BT 40
    RRC L                        ; @BT CB 0D
    SET 0,(HL)                   ; @BT CB C6
    XOR (HL)                     ; @BT AE
    CALL EQU,$189A               ; @BT CC 9A 18
    LD B,$22                     ; @BT 06 22
    ADC A,E                      ; @BT 8B
    JR NC,$18                    ; @BT 30 18
    PUSH HL                      ; @BT E5
    LD A,A                       ; @BT 7F
    LD H,E                       ; @BT 63
    SET 4,L                      ; @BT CB E5
    BIT 3,H                      ; @BT CB 5C
    LD C,H                       ; @BT 4C
    CALL GEQ,$1678               ; @BT D4 78 16
    ADC C                        ; @BT 89
    JR LST,$0E                   ; @BT 38 0E
    LDD (HL),A                   ; @BT 32
    STOP $92                     ; @BT 10 92
    ADC A,L                      ; @BT 8D
    RES 3,A                      ; @BT CB 9F
    SET 7,A                      ; @BT CB FF
    OR D                         ; @BT B2
    INC BC                       ; @BT 03
    RET LST                      ; @BT D8
    OR H                         ; @BT B4
    RRA                          ; @BT 1F
    BIT 7,A                      ; @BT CB 7F
    BIT 4,A                      ; @BT CB 67
    OR A,(HL)                    ; @BT B6
    RES 0,E                      ; @BT CB 83
    JR LSS,$37                   ; @BT 38 37
    RES 0,(HL)                   ; @BT CB 86
    SUB C                        ; @BT 91
    LD A,($FF00+$F4)             ; @BT F0 F4
    DEC (HL)                     ; @BT 35
    CCF                          ; @BT 3F
    RRC C                        ; @BT CB 09
    ADD E                        ; @BT 83
    CP A,(HL)                    ; @BT BE
    LD E,$67                     ; @BT 1E 67
    RRC E                        ; @BT CB 0B
    SUB A,$A9                    ; @BT D6 A9
    DI                           ; @BT F3
    AND C                        ; @BT A1
    RET Z                        ; @BT C8
    OR (HL)                      ; @BT B6
    LD (HL),E                    ; @BT 73
    LD ($FF00+$7E),A             ; @BT E0 7E
    RR B                         ; @BT CB 18
    SRL (HL)                     ; @BT CB 3E
    RES 7,H                      ; @BT CB BC
    JP $29AA                     ; @BT C3 AA 29
    RES 4,D                      ; @BT CB A2
    RRC D                        ; @BT CB 0A
    SBC A,E                      ; @BT 9B
    RES 2,C                      ; @BT CB 91
    BIT 1,L                      ; @BT CB 4D
    SBC A,B                      ; @BT 98
    INC E                        ; @BT 1C
    CP (HL)                      ; @BT BE
    OR C                         ; @BT B1
    LD D,(HL)                    ; @BT 56
    SUB H                        ; @BT 94
    RET EQU                      ; @BT C8
    LD B,E                       ; @BT 43
    SLA D                        ; @BT CB 22
    CP A,H                       ; @BT BC
    BIT 6,D                      ; @BT CB 72
    RET NEQ                      ; @BT C0
    LD (HLI),A                   ; @BT 22
    LD (HL),L                    ; @BT 75
    LD ($FF00+C),A               ; @BT E2
    XOR A,E                      ; @BT AB
    AND A,L                      ; @BT A5
    RET GEQ                      ; @BT D0
    SBC A,C                      ; @BT 99
    SET 2,L                      ; @BT CB D5
    ADD A,C                      ; @BT 81
    INC DE                       ; @BT 13
    SET 4,B                      ; @BT CB E0
    INC B                        ; @BT 04
    STOP                         ; @BT 10
    LD (HL+),A                   ; @BT 22
    SBC C                        ; @BT 99
    DEC E                        ; @BT 1D
    DEC B                        ; @BT 05
    RES 7,(HL)                   ; @BT CB BE
    BIT 6,H                      ; @BT CB 74
    SET 0,C                      ; @BT CB C1
    XOR A,(HL)                   ; @BT AE
    CALL NZ,$1BCD                ; @BT C4 CD 1B
    SWAP (HL)                    ; @BT CB 36
    BIT 4,E                      ; @BT CB 63
    SET 7,B                      ; @BT CB F8
    SBC E                        ; @BT 9B
    DEC HL                       ; @BT 2B
    RES 0,A                      ; @BT CB 87
    JP (HL)                      ; @BT E9
    AND A,$9B                    ; @BT E6 9B
    BIT 4,L                      ; @BT CB 65
    ADC $3F                      ; @BT CE 3F
    LD PC,HL                     ; @BT E9
    SRA A                        ; @BT CB 2F
    CP C                         ; @BT B9
    DEC L                        ; @BT 2D
    RR H                         ; @BT CB 1C
    JR EQU,$2A                   ; @BT 28 2A
    XOR A,B                      ; @BT A8
    ADC A,D                      ; @BT 8A
    RR L                         ; @BT CB 1D
    BIT 5,A                      ; @BT CB 6F
    LDH A,(C)                    ; @BT F2
    LD B,(HL)                    ; @BT 46
    BIT 0,L                      ; @BT CB 45
    LD A,(BC)                    ; @BT 0A
    LDH A,($08)                  ; @BT F0 08
    SRA L                        ; @BT CB 2D
    AND A,B                      ; @BT A0
    BIT 3,E                      ; @BT CB 5B
    RL (HL)                      ; @BT CB 16
    SET 6,A                      ; @BT CB F7
    SBC $7B                      ; @BT DE 7B
    LD B,C                       ; @BT 41
    LD BC,$2BCC                  ; @BT 01 CC 2B
    RST $38                      ; @BT FF
    SET 4,D                      ; @BT CB E2
    SET 5,E                      ; @BT CB EB
    RES 4,A                      ; @BT CB A7
    LD (HL),H                    ; @BT 74
    LD E,B                       ; @BT 58
    ADD SP,$2D                   ; @BT E8 2D
    RST $18                      ; @BT DF
    CP A,D                       ; @BT BA
    RES 1,H                      ; @BT CB 8C
    BIT 6,A                      ; @BT CB 77
    LD SP,HL                     ; @BT F9
    RES 6,D                      ; @BT CB B2
    OR A,H                       ; @BT B4
    LD (HLD),A                   ; @BT 32
    RES 0,H                      ; @BT CB 84
    JP EQU,$2455                 ; @BT CA 55 24
    AND H                        ; @BT A4
    RLC A                        ; @BT CB 07
    RES 4,L                      ; @BT CB A5
    LD (HL),B                    ; @BT 70
    RES 6,E                      ; @BT CB B3
    RES 5,H                      ; @BT CB AC
    SRL C                        ; @BT CB 39
    BIT 6,L                      ; @BT CB 75
    POP AF                       ; @BT F1
    XOR A,$D7                    ; @BT EE D7
    ADD H                        ; @BT 84
    CALL Z,$1CDE                 ; @BT CC DE 1C
    LD D,C                       ; @BT 51
    JR $1D                       ; @BT 18 1D
    OR A,D                       ; @BT B2
    RRC H                        ; @BT CB 0C
    SWAP B                       ; @BT CB 30
    RL C                         ; @BT CB 11
    RES 4,(HL)                   ; @BT CB A6
    RES 6,L                      ; @BT CB B5
    OR A,A                       ; @BT B7
    LD C,E                       ; @BT 4B
    SUB A,C                      ; @BT 91
    DEC SP                       ; @BT 3B
    LD C,(HL)                    ; @BT 4E
    RES 5,D                      ; @BT CB AA
    JP GTE,$2122                 ; @BT D2 22 21
    JP LST,$2011                 ; @BT DA 11 20
    ADD HL,DE                    ; @BT 19
    RR (HL)                      ; @BT CB 1E
    LD A,E                       ; @BT 7B
    CALL LST,$1456               ; @BT DC 56 14
    RES 0,C                      ; @BT CB 81
    RES 1,(HL)                   ; @BT CB 8E
    CP E                         ; @BT BB
    BIT 6,(HL)                   ; @BT CB 76
    SET 2,E                      ; @BT CB D3
    SET 1,H                      ; @BT CB CC
    RES 2,H                      ; @BT CB 94
    SRL D                        ; @BT CB 3A
    CALL NC,$1ABC                ; @BT D4 BC 1A
    JR GTE,$25                   ; @BT 30 25
    LD A,(HL+)                   ; @BT 2A
    LD ($3121),A                 ; @BT EA 21 31
    POP HL                       ; @BT E1
    DAA                          ; @BT 27
    SET 7,C                      ; @BT CB F9
    SET 5,D                      ; @BT CB EA
    RES 1,C                      ; @BT CB 89
    BIT 1,D                      ; @BT CB 4A
    ADC A,A                      ; @BT 8F
    AND A,E                      ; @BT A3
    AND A,H                      ; @BT A4
    LD A,($FF00+C)               ; @BT F2
    BIT 3,A                      ; @BT CB 5F
    RES 5,E                      ; @BT CB AB
    SET 6,(HL)                   ; @BT CB F6
    SET 4,C                      ; @BT CB E1
    ADD $84                      ; @BT C6 84
    SWAP H                       ; @BT CB 34
    LD A,B                       ; @BT 78
    SUB D                        ; @BT 92
    SBC A,A                      ; @BT 9F
    LD A,C                       ; @BT 79
    SET 0,D                      ; @BT CB C2
    SET 3,(HL)                   ; @BT CB DE
    RES 0,B                      ; @BT CB 80
    ADD C                        ; @BT 81
    INC H                        ; @BT 24
    RES 6,A                      ; @BT CB B7
    BIT 2,E                      ; @BT CB 53
    SET 1,B                      ; @BT CB C8
    LD H,D                       ; @BT 62
    RES 1,E                      ; @BT CB 8B
    BIT 4,B                      ; @BT CB 60
    SET 7,E                      ; @BT CB FB
    LD C,C                       ; @BT 49
    OR B                         ; @BT B0
    BIT 4,H                      ; @BT CB 64
    LD C,B                       ; @BT 48
    ADC A,(HL)                   ; @BT 8E
    SET 3,E                      ; @BT CB DB
    SUB A,D                      ; @BT 92
    BIT 4,(HL)                   ; @BT CB 66
    JR NZ,$2F                    ; @BT 20 2F
    DEC A                        ; @BT 3D
    LD (BC),A                    ; @BT 02
    NOP                          ; @BT 00
    RR A                         ; @BT CB 1F
    SBC A,L                      ; @BT 9D
    SET 6,C                      ; @BT CB F1
    CP L                         ; @BT BD
    BIT 5,D                      ; @BT CB 6A
    RRC B                        ; @BT CB 08
    RES 4,E                      ; @BT CB A3
    OR $4D                       ; @BT F6 4D
    OR E                         ; @BT B3
    RLC C                        ; @BT CB 01
    RES 3,D                      ; @BT CB 9A
    ADD A,(HL)                   ; @BT 86
    DEC D                        ; @BT 15
    BIT 2,C                      ; @BT CB 51
    AND $B2                      ; @BT E6 B2
    JP NEQ,$2344                 ; @BT C2 44 23
    RES 1,L                      ; @BT CB 8D
    LD A,D                       ; @BT 7A
    BIT 1,H                      ; @BT CB 4C
    BIT 2,H                      ; @BT CB 54
    RST $30                      ; @BT F7
    OR A,E                       ; @BT B3
    LD L,H                       ; @BT 6C
    SET 0,B                      ; @BT CB C0
    BIT 3,B                      ; @BT CB 58
    XOR $EE                      ; @BT EE EE
    SET 5,L                      ; @BT CB ED
    RET GTE                      ; @BT D0
    SLA (HL)                     ; @BT CB 26
    SUB A,E                      ; @BT 93
    SRA H                        ; @BT CB 2C
    BIT 5,(HL)                   ; @BT CB 6E
    RST $00                      ; @BT C7
    LDI (HL),A                   ; @BT 22
    LD A,(DE)                    ; @BT 1A
    LD E,E                       ; @BT 5B
    SET 2,B                      ; @BT CB D0
    LD C,A                       ; @BT 4F
    BIT 7,B                      ; @BT CB 78
    LD H,(HL)                    ; @BT 66
    DEC BC                       ; @BT 0B
    ADD B                        ; @BT 80
    ADC A,H                      ; @BT 8C
    AND A,(HL)                   ; @BT A6
    LD E,C                       ; @BT 59
    SET 5,B                      ; @BT CB E8
    LD A,H                       ; @BT 7C
    LD DE,$2CDD                  ; @BT 11 DD 2C
    XOR A,D                      ; @BT AA
    CP $E0                       ; @BT FE E0
    OR A,B                       ; @BT B0
    JP GEQ,$2233                 ; @BT D2 33 22
    RES 7,E                      ; @BT CB BB
    CP A                         ; @BT BF
    LD B,D                       ; @BT 42
    ADD A,B                      ; @BT 80
    RL E                         ; @BT CB 13
    LD HL,$2DEE                  ; @BT 21 EE 2D
    RES 0,D                      ; @BT CB 82
    XOR A,H                      ; @BT AC
    OR A,L                       ; @BT B5
    RLC B                        ; @BT CB 00
    AND E                        ; @BT A3
    SBC B                        ; @BT 98
    RES 4,C                      ; @BT CB A1
    BIT 5,C                      ; @BT CB 69
    LD H,B                       ; @BT 60
    LD A,(HL-)                   ; @BT 3A
    LD (HL),D                    ; @BT 72
    POP DE                       ; @BT D1
    LD E,A                       ; @BT 5F
    BIT 0,(HL)                   ; @BT CB 46
    CP A,B                       ; @BT B8
    RES 1,A                      ; @BT CB 8F
    JR NEQ,$13                   ; @BT 20 13
    RES 7,D                      ; @BT CB BA
    LD HL,SP+$15                 ; @BT F8 15
    AND A,C                      ; @BT A1
    BIT 5,H                      ; @BT CB 6C
    RLC H                        ; @BT CB 04
    BIT 7,C                      ; @BT CB 79
    LD HL,SP-$2C                 ; @BT F8 D4
    INC (HL)                     ; @BT 34
    RL B                         ; @BT CB 10
    SET 6,D                      ; @BT CB F2
    RET NC                       ; @BT D0
    LD (HL-),A                   ; @BT 32
    RES 5,L                      ; @BT CB AD
    LD C,L                       ; @BT 4D
    SBC A,$64                    ; @BT DE 64
    SET 5,A                      ; @BT CB EF
    INC L                        ; @BT 2C
    SRA B                        ; @BT CB 28
    SET 5,C                      ; @BT CB E9
    PUSH DE                      ; @BT D5
    SET 6,H                      ; @BT CB F4
    PUSH BC                      ; @BT C5
    LD E,D                       ; @BT 5A
    LD L,D                       ; @BT 6A
    RST $20                      ; @BT E7
    LD D,L                       ; @BT 55
    CALL $1DEF                   ; @BT CD EF 1D
    POP BC                       ; @BT C1
    OR A,C                       ; @BT B1
    SET 5,(HL)                   ; @BT CB EE
    LD L,$F1                     ; @BT 2E F1
    RET C                        ; @BT D8
    RES 3,B                      ; @BT CB 98
    SET 0,E                      ; @BT CB C3
    RRCA                         ; @BT 0F
    RES 5,(HL)                   ; @BT CB AE
    SBC L                        ; @BT 9D
    SET 5,H                      ; @BT CB EC
    LD ($3010),SP                ; @BT 08 10 30
    BIT 2,L                      ; @BT CB 55
    SET 4,E                      ; @BT CB E3
    BIT 0,D                      ; @BT CB 42
    LDD A,(HL)                   ; @BT 3A
    CALL C,$19AB                 ; @BT DC AB 19
    RL H                         ; @BT CB 14
    RES 3,E                      ; @BT CB 9B

    .db "<02"               ; @BT END

    /////////////////////////////////////////////////////////////////////////////
    // TEST-03: same opcodes, randomised order, operands as arithmetic expressions
    /////////////////////////////////////////////////////////////////////////////

    .db "03>"               ; @BT TEST-03 03 START

    ; GENERATED: copied from TEST-02 with varied arithmetic expressions for numeric operands

    ; GENERATED: copied from TEST-01 with full TEST-02 randomization
    ; GENERATED: seed = 1704833320

    AND A,D                      ; @BT A2
    LD (HL),A                    ; @BT 77
    ADC D                        ; @BT 8A
    SCF                          ; @BT 37
    ADD A,E                      ; @BT 83
    LD A,$0B+5-5                     ; @BT 3E 0B
    CP A,C                       ; @BT B9
    SET 1,A                      ; @BT CB CF
    LD A,(HLI)                   ; @BT 2A
    LDH (C),A                    ; @BT E2
    ADD A,L                      ; @BT 85
    BIT 2,A                      ; @BT CB 57
    RLCA                         ; @BT 07
    ADC H                        ; @BT 8C
    RL A                         ; @BT CB 17
    LD H,A                       ; @BT 67
    SET 6,B                      ; @BT CB F0
    SRA E                        ; @BT CB 2B
    SET 1,(HL)                   ; @BT CB CE
    DEC DE                       ; @BT 1B
    SLA C                        ; @BT CB 21
    JP C,(($2566-7)+7)                   ; @BT DA 66 25
    ADC A,B                      ; @BT 88
    SET 2,H                      ; @BT CB D4
    LD A,(((($2ABB*6)/3)-($2ABB)))                 ; @BT FA BB 2A
    CP H                         ; @BT BC
    SRA D                        ; @BT CB 2A
    RLC E                        ; @BT CB 03
    ADC L                        ; @BT 8D
    CP A,L                       ; @BT BD
    RES 7,C                      ; @BT CB B9
    LD D,A                       ; @BT 57
    LD L,B                       ; @BT 68
    RES 2,D                      ; @BT CB 92
    BIT 4,D                      ; @BT CB 62
    SLA H                        ; @BT CB 24
    SET 3,C                      ; @BT CB D9
    SLA E                        ; @BT CB 23
    SRL H                        ; @BT CB 3C
    SET 7,D                      ; @BT CB FA
    EI                           ; @BT FB
    SRL L                        ; @BT CB 3D
    SRL A                        ; @BT CB 3F
    ADC (HL)                     ; @BT 8E
    INC SP                       ; @BT 33
    ADD A,A                      ; @BT 87
    SET 3,L                      ; @BT CB DD
    AND A                        ; @BT A7
    ADD A,H                      ; @BT 84
    CPL                          ; @BT 2F
    RLA                          ; @BT 17
    RES 2,E                      ; @BT CB 93
    SBC A,(HL)                   ; @BT 9E
    ADD A,D                      ; @BT 82
    BIT 0,C                      ; @BT CB 41
    BIT 5,B                      ; @BT CB 68
    CP B                         ; @BT B8
    RES 3,C                      ; @BT CB 99
    RLC D                        ; @BT CB 02
    LD A,(HLD)                   ; @BT 3A
    SET 1,C                      ; @BT CB C9
    ADC A                        ; @BT 8F
    SET 2,D                      ; @BT CB D2
    BIT 7,(HL)                   ; @BT CB 7E
    LD A,L                       ; @BT 7D
    RES 6,C                      ; @BT CB B1
    SBC H                        ; @BT 9C
    SBC D                        ; @BT 9A
    XOR A,C                      ; @BT A9
    SET 3,D                      ; @BT CB DA
    JP NZ,((($2788*10)/5)-($2788))                  ; @BT C2 88 27
    BIT 0,E                      ; @BT CB 43
    RES 6,H                      ; @BT CB B4
    SET 1,E                      ; @BT CB CB
    BIT 7,L                      ; @BT CB 7D
    SET 3,A                      ; @BT CB DF
    PUSH AF                      ; @BT F5
    RETI                         ; @BT D9
    XOR D                        ; @BT AA
    SUB A,(HL)                   ; @BT 96
    SWAP L                       ; @BT CB 35
    LD E,(HL)                    ; @BT 5E
    RR E                         ; @BT CB 1B
    SWAP D                       ; @BT CB 32
    SRL B                        ; @BT CB 38
    SET 2,A                      ; @BT CB D7
    CALL NEQ,(($1789*2)-(($1789)-0))               ; @BT C4 89 17
    LD A,(HL)                    ; @BT 7E
    BIT 6,C                      ; @BT CB 71
    XOR E                        ; @BT AB
    RES 7,B                      ; @BT CB B8
    XOR L                        ; @BT AD
    ADD HL,HL                    ; @BT 29
    CP D                         ; @BT BA
    SET 4,H                      ; @BT CB E4
    RES 1,B                      ; @BT CB 88
    SLA A                        ; @BT CB 27
    ADD HL,BC                    ; @BT 09
    RST ((($28+11)-4)-7)                      ; @BT EF
    SET 4,A                      ; @BT CB E7
    BIT 2,(HL)                   ; @BT CB 56
    RES 3,L                      ; @BT CB 9D
    LD L,A                       ; @BT 6F
    AND (HL)                     ; @BT A6
    RES 2,B                      ; @BT CB 90
    SWAP C                       ; @BT CB 31
    RES 5,A                      ; @BT CB AF
    RES 7,A                      ; @BT CB BF
    LD B,A                       ; @BT 47
    LD (DE),A                    ; @BT 12
    BIT 6,B                      ; @BT CB 70
    ADC A,((($28+3)-1)-2)                    ; @BT CE 28
    LD H,H                       ; @BT 64
    RES 2,(HL)                   ; @BT CB 96
    JP LSS,(($1F00+5)-5)                 ; @BT DA 00 1F
    BIT 1,C                      ; @BT CB 49
    JR C,(($01-7)+7)                     ; @BT 38 01
    SET 0,L                      ; @BT CB C5
    SRL E                        ; @BT CB 3B
    RET                          ; @BT C9
    RRC A                        ; @BT CB 0F
    BIT 5,L                      ; @BT CB 6D
    RES 2,A                      ; @BT CB 97
    RR C                         ; @BT CB 19
    JP HL                        ; @BT E9
    XOR A,A                      ; @BT AF
    XOR B                        ; @BT A8
    INC D                        ; @BT 14
    ADC E                        ; @BT 8B
    CP A,A                       ; @BT BF
    RET LSS                      ; @BT D8
    XOR A,L                      ; @BT AD
    SUB A,H                      ; @BT 94
    SET 3,H                      ; @BT CB DC
    LD H,L                       ; @BT 65
    RES 0,L                      ; @BT CB 85
    SET 2,(HL)                   ; @BT CB D6
    RES 5,C                      ; @BT CB A9
    SUB (HL)                     ; @BT 96
    LD D,E                       ; @BT 53
    XOR H                        ; @BT AC
    ADD L                        ; @BT 85
    SWAP A                       ; @BT CB 37
    SET 4,(HL)                   ; @BT CB E6
    LD D,((($50*6)/3)-($50))                     ; @BT 16 50
    SET 0,H                      ; @BT CB C4
    BIT 3,L                      ; @BT CB 5D
    SUB A                        ; @BT 97
    RLC L                        ; @BT CB 05
    SET 1,L                      ; @BT CB CD
    LD B,H                       ; @BT 44
    LD D,B                       ; @BT 50
    AND L                        ; @BT A5
    LD E,L                       ; @BT 5D
    INC A                        ; @BT 3C
    BIT 1,(HL)                   ; @BT CB 4E
    BIT 1,A                      ; @BT CB 4F
    ADD A,((($56*10)/5)-($56))                    ; @BT C6 56
    BIT 5,E                      ; @BT CB 6B
    SET 2,C                      ; @BT CB D1
    LD H,(($DA*2)-(($DA)-0))                     ; @BT 26 DA
    SWAP E                       ; @BT CB 33
    INC C                        ; @BT 0C
    RES 6,(HL)                   ; @BT CB B6
    LD C,((($39+11)-4)-7)                     ; @BT 0E 39
    RES 6,B                      ; @BT CB B0
    BIT 0,B                      ; @BT CB 40
    BIT 6,E                      ; @BT CB 73
    SRA (HL)                     ; @BT CB 2E
    BIT 7,H                      ; @BT CB 7C
    SET 6,L                      ; @BT CB F5
    AND A,A                      ; @BT A7
    BIT 3,C                      ; @BT CB 59
    SBC A,D                      ; @BT 9A
    SET 1,D                      ; @BT CB CA
    RES 2,L                      ; @BT CB 95
    SUB B                        ; @BT 90
    SUB ((($C0+3)-1)-2)                      ; @BT D6 C0
    BIT 0,H                      ; @BT CB 44
    OR L                         ; @BT B5
    CALL GTE,(($1567+5)-5)               ; @BT D4 67 15
    RES 7,L                      ; @BT CB BD
    AND D                        ; @BT A2
    CP A,E                       ; @BT BB
    LDH ($1F),A                  ; @BT E0 1F
    BIT 1,E                      ; @BT CB 4B
    LDI A,(HL)                   ; @BT 2A
    DEC C                        ; @BT 0D
    LD L,(HL)                    ; @BT 6E
    OR A,(($36-7)+7)                     ; @BT F6 36
    CALL LSS,((($1345*6)/3)-($1345))               ; @BT DC 45 13
    LD B,L                       ; @BT 45
    XOR C                        ; @BT A9
    ADD A                        ; @BT 87
    LD H,C                       ; @BT 61
    AND B                        ; @BT A0
    ADC B                        ; @BT 88
    LD L,C                       ; @BT 69
    BIT 0,A                      ; @BT CB 47
    RES 5,B                      ; @BT CB A8
    LD SP,((($2EFF*10)/5)-($2EFF))                  ; @BT 31 FF 2E
    BIT 3,(HL)                   ; @BT CB 5E
    CPL A                        ; @BT 2F
    LD (HL),C                    ; @BT 71
    RST (($10*2)-(($10)-0))                      ; @BT D7
    BIT 2,D                      ; @BT CB 52
    RR D                         ; @BT CB 1A
    ADD (HL)                     ; @BT 86
    SRA C                        ; @BT CB 29
    LD L,E                       ; @BT 6B
    XOR A                        ; @BT AF
    SUB A,A                      ; @BT 97
    RRC (HL)                     ; @BT CB 0E
    SET 7,L                      ; @BT CB FD
    JR GEQ,((($3C+11)-4)-7)                   ; @BT 30 3C
    ADD D                        ; @BT 82
    RLC (HL)                     ; @BT CB 06
    SET 7,(HL)                   ; @BT CB FE
    RST ((($08+3)-1)-2)                      ; @BT CF
    LD (HL),(($C3+5)-5)                  ; @BT 36 C3
    DEC H                        ; @BT 25
    RES 3,H                      ; @BT CB 9C
    JP NC,(($2677-7)+7)                  ; @BT D2 77 26
    LD L,L                       ; @BT 6D
    LD D,D                       ; @BT 52
    RES 1,D                      ; @BT CB 8A
    SLA B                        ; @BT CB 20
    SET 3,B                      ; @BT CB D8
    RES 3,(HL)                   ; @BT CB 9E
    OR A                         ; @BT B7
    ADC A,C                      ; @BT 89
    LD C,D                       ; @BT 4A
    LD D,H                       ; @BT 54
    HALT                         ; @BT 76
    RES 4,H                      ; @BT CB A4
    SLA L                        ; @BT CB 25
    SUB A,B                      ; @BT 90
    RL D                         ; @BT CB 12
    SET 6,E                      ; @BT CB F3
    BIT 7,E                      ; @BT CB 7B
    SBC (HL)                     ; @BT 9E
    SUB L                        ; @BT 95
    SBC A                        ; @BT 9F
    SET 0,A                      ; @BT CB C7
    BIT 1,B                      ; @BT CB 48
    BIT 2,B                      ; @BT CB 50
    JP Z,((($2899*6)/3)-($2899))                   ; @BT CA 99 28
    SUB A,L                      ; @BT 95
    CP A,((($C9*10)/5)-($C9))                     ; @BT FE C9
    JR Z,(($06*2)-(($06)-0))                     ; @BT 28 06
    RES 4,B                      ; @BT CB A0
    RL L                         ; @BT CB 15
    ADD HL,SP                    ; @BT 39
    BIT 7,D                      ; @BT CB 7A
    BIT 3,D                      ; @BT CB 5A
    BIT 4,C                      ; @BT CB 61
    SUB E                        ; @BT 93
    LD E,H                       ; @BT 5C
    INC HL                       ; @BT 23
    SET 7,H                      ; @BT CB FC
    SBC A,H                      ; @BT 9C
    RET NZ                       ; @BT C0
    LD B,B                       ; @BT 40
    RRC L                        ; @BT CB 0D
    SET 0,(HL)                   ; @BT CB C6
    XOR (HL)                     ; @BT AE
    CALL EQU,((($189A+11)-4)-7)               ; @BT CC 9A 18
    LD B,((($22+3)-1)-2)                     ; @BT 06 22
    ADC A,E                      ; @BT 8B
    JR NC,(($18+5)-5)                    ; @BT 30 18
    PUSH HL                      ; @BT E5
    LD A,A                       ; @BT 7F
    LD H,E                       ; @BT 63
    SET 4,L                      ; @BT CB E5
    BIT 3,H                      ; @BT CB 5C
    LD C,H                       ; @BT 4C
    CALL GEQ,(($1678-7)+7)               ; @BT D4 78 16
    ADC C                        ; @BT 89
    JR LST,((($0E*6)/3)-($0E))                   ; @BT 38 0E
    LDD (HL),A                   ; @BT 32
    STOP ((($92*10)/5)-($92))                     ; @BT 10 92
    ADC A,L                      ; @BT 8D
    RES 3,A                      ; @BT CB 9F
    SET 7,A                      ; @BT CB FF
    OR D                         ; @BT B2
    INC BC                       ; @BT 03
    RET LST                      ; @BT D8
    OR H                         ; @BT B4
    RRA                          ; @BT 1F
    BIT 7,A                      ; @BT CB 7F
    BIT 4,A                      ; @BT CB 67
    OR A,(HL)                    ; @BT B6
    RES 0,E                      ; @BT CB 83
    JR LSS,(($37*2)-(($37)-0))                   ; @BT 38 37
    RES 0,(HL)                   ; @BT CB 86
    SUB C                        ; @BT 91
    LD A,($FF00+$F4)             ; @BT F0 F4
    DEC (HL)                     ; @BT 35
    CCF                          ; @BT 3F
    RRC C                        ; @BT CB 09
    ADD E                        ; @BT 83
    CP A,(HL)                    ; @BT BE
    LD E,((($67+11)-4)-7)                     ; @BT 1E 67
    RRC E                        ; @BT CB 0B
    SUB A,((($A9+3)-1)-2)                    ; @BT D6 A9
    DI                           ; @BT F3
    AND C                        ; @BT A1
    RET Z                        ; @BT C8
    OR (HL)                      ; @BT B6
    LD (HL),E                    ; @BT 73
    LD ($FF00+$7E),A             ; @BT E0 7E
    RR B                         ; @BT CB 18
    SRL (HL)                     ; @BT CB 3E
    RES 7,H                      ; @BT CB BC
    JP (($29AA+5)-5)                     ; @BT C3 AA 29
    RES 4,D                      ; @BT CB A2
    RRC D                        ; @BT CB 0A
    SBC A,E                      ; @BT 9B
    RES 2,C                      ; @BT CB 91
    BIT 1,L                      ; @BT CB 4D
    SBC A,B                      ; @BT 98
    INC E                        ; @BT 1C
    CP (HL)                      ; @BT BE
    OR C                         ; @BT B1
    LD D,(HL)                    ; @BT 56
    SUB H                        ; @BT 94
    RET EQU                      ; @BT C8
    LD B,E                       ; @BT 43
    SLA D                        ; @BT CB 22
    CP A,H                       ; @BT BC
    BIT 6,D                      ; @BT CB 72
    RET NEQ                      ; @BT C0
    LD (HLI),A                   ; @BT 22
    LD (HL),L                    ; @BT 75
    LD ($FF00+C),A               ; @BT E2
    XOR A,E                      ; @BT AB
    AND A,L                      ; @BT A5
    RET GEQ                      ; @BT D0
    SBC A,C                      ; @BT 99
    SET 2,L                      ; @BT CB D5
    ADD A,C                      ; @BT 81
    INC DE                       ; @BT 13
    SET 4,B                      ; @BT CB E0
    INC B                        ; @BT 04
    STOP                         ; @BT 10
    LD (HL+),A                   ; @BT 22
    SBC C                        ; @BT 99
    DEC E                        ; @BT 1D
    DEC B                        ; @BT 05
    RES 7,(HL)                   ; @BT CB BE
    BIT 6,H                      ; @BT CB 74
    SET 0,C                      ; @BT CB C1
    XOR A,(HL)                   ; @BT AE
    CALL NZ,(($1BCD-7)+7)                ; @BT C4 CD 1B
    SWAP (HL)                    ; @BT CB 36
    BIT 4,E                      ; @BT CB 63
    SET 7,B                      ; @BT CB F8
    SBC E                        ; @BT 9B
    DEC HL                       ; @BT 2B
    RES 0,A                      ; @BT CB 87
    JP (HL)                      ; @BT E9
    AND A,((($9B*6)/3)-($9B))                    ; @BT E6 9B
    BIT 4,L                      ; @BT CB 65
    ADC ((($3F*10)/5)-($3F))                      ; @BT CE 3F
    LD PC,HL                     ; @BT E9
    SRA A                        ; @BT CB 2F
    CP C                         ; @BT B9
    DEC L                        ; @BT 2D
    RR H                         ; @BT CB 1C
    JR EQU,(($2A*2)-(($2A)-0))                   ; @BT 28 2A
    XOR A,B                      ; @BT A8
    ADC A,D                      ; @BT 8A
    RR L                         ; @BT CB 1D
    BIT 5,A                      ; @BT CB 6F
    LDH A,(C)                    ; @BT F2
    LD B,(HL)                    ; @BT 46
    BIT 0,L                      ; @BT CB 45
    LD A,(BC)                    ; @BT 0A
    LDH A,($08)                  ; @BT F0 08
    SRA L                        ; @BT CB 2D
    AND A,B                      ; @BT A0
    BIT 3,E                      ; @BT CB 5B
    RL (HL)                      ; @BT CB 16
    SET 6,A                      ; @BT CB F7
    SBC ((($7B+11)-4)-7)                      ; @BT DE 7B
    LD B,C                       ; @BT 41
    LD BC,((($2BCC+3)-1)-2)                  ; @BT 01 CC 2B
    RST (($38+5)-5)                      ; @BT FF
    SET 4,D                      ; @BT CB E2
    SET 5,E                      ; @BT CB EB
    RES 4,A                      ; @BT CB A7
    LD (HL),H                    ; @BT 74
    LD E,B                       ; @BT 58
    ADD SP,(($2D-7)+7)                   ; @BT E8 2D
    RST ((($18*6)/3)-($18))                      ; @BT DF
    CP A,D                       ; @BT BA
    RES 1,H                      ; @BT CB 8C
    BIT 6,A                      ; @BT CB 77
    LD SP,HL                     ; @BT F9
    RES 6,D                      ; @BT CB B2
    OR A,H                       ; @BT B4
    LD (HLD),A                   ; @BT 32
    RES 0,H                      ; @BT CB 84
    JP EQU,((($2455*10)/5)-($2455))                 ; @BT CA 55 24
    AND H                        ; @BT A4
    RLC A                        ; @BT CB 07
    RES 4,L                      ; @BT CB A5
    LD (HL),B                    ; @BT 70
    RES 6,E                      ; @BT CB B3
    RES 5,H                      ; @BT CB AC
    SRL C                        ; @BT CB 39
    BIT 6,L                      ; @BT CB 75
    POP AF                       ; @BT F1
    XOR A,(($D7*2)-(($D7)-0))                    ; @BT EE D7
    ADD H                        ; @BT 84
    CALL Z,((($1CDE+11)-4)-7)                 ; @BT CC DE 1C
    LD D,C                       ; @BT 51
    JR ((($1D+3)-1)-2)                       ; @BT 18 1D
    OR A,D                       ; @BT B2
    RRC H                        ; @BT CB 0C
    SWAP B                       ; @BT CB 30
    RL C                         ; @BT CB 11
    RES 4,(HL)                   ; @BT CB A6
    RES 6,L                      ; @BT CB B5
    OR A,A                       ; @BT B7
    LD C,E                       ; @BT 4B
    SUB A,C                      ; @BT 91
    DEC SP                       ; @BT 3B
    LD C,(HL)                    ; @BT 4E
    RES 5,D                      ; @BT CB AA
    JP GTE,(($2122+5)-5)                 ; @BT D2 22 21
    JP LST,(($2011-7)+7)                 ; @BT DA 11 20
    ADD HL,DE                    ; @BT 19
    RR (HL)                      ; @BT CB 1E
    LD A,E                       ; @BT 7B
    CALL LST,((($1456*6)/3)-($1456))               ; @BT DC 56 14
    RES 0,C                      ; @BT CB 81
    RES 1,(HL)                   ; @BT CB 8E
    CP E                         ; @BT BB
    BIT 6,(HL)                   ; @BT CB 76
    SET 2,E                      ; @BT CB D3
    SET 1,H                      ; @BT CB CC
    RES 2,H                      ; @BT CB 94
    SRL D                        ; @BT CB 3A
    CALL NC,((($1ABC*10)/5)-($1ABC))                ; @BT D4 BC 1A
    JR GTE,(($25*2)-(($25)-0))                   ; @BT 30 25
    LD A,(HL+)                   ; @BT 2A
    LD (((($3121+11)-4)-7)),A                 ; @BT EA 21 31
    POP HL                       ; @BT E1
    DAA                          ; @BT 27
    SET 7,C                      ; @BT CB F9
    SET 5,D                      ; @BT CB EA
    RES 1,C                      ; @BT CB 89
    BIT 1,D                      ; @BT CB 4A
    ADC A,A                      ; @BT 8F
    AND A,E                      ; @BT A3
    AND A,H                      ; @BT A4
    LD A,($FF00+C)               ; @BT F2
    BIT 3,A                      ; @BT CB 5F
    RES 5,E                      ; @BT CB AB
    SET 6,(HL)                   ; @BT CB F6
    SET 4,C                      ; @BT CB E1
    ADD ((($84+3)-1)-2)                      ; @BT C6 84
    SWAP H                       ; @BT CB 34
    LD A,B                       ; @BT 78
    SUB D                        ; @BT 92
    SBC A,A                      ; @BT 9F
    LD A,C                       ; @BT 79
    SET 0,D                      ; @BT CB C2
    SET 3,(HL)                   ; @BT CB DE
    RES 0,B                      ; @BT CB 80
    ADD C                        ; @BT 81
    INC H                        ; @BT 24
    RES 6,A                      ; @BT CB B7
    BIT 2,E                      ; @BT CB 53
    SET 1,B                      ; @BT CB C8
    LD H,D                       ; @BT 62
    RES 1,E                      ; @BT CB 8B
    BIT 4,B                      ; @BT CB 60
    SET 7,E                      ; @BT CB FB
    LD C,C                       ; @BT 49
    OR B                         ; @BT B0
    BIT 4,H                      ; @BT CB 64
    LD C,B                       ; @BT 48
    ADC A,(HL)                   ; @BT 8E
    SET 3,E                      ; @BT CB DB
    SUB A,D                      ; @BT 92
    BIT 4,(HL)                   ; @BT CB 66
    JR NZ,(($2F+5)-5)                    ; @BT 20 2F
    DEC A                        ; @BT 3D
    LD (BC),A                    ; @BT 02
    NOP                          ; @BT 00
    RR A                         ; @BT CB 1F
    SBC A,L                      ; @BT 9D
    SET 6,C                      ; @BT CB F1
    CP L                         ; @BT BD
    BIT 5,D                      ; @BT CB 6A
    RRC B                        ; @BT CB 08
    RES 4,E                      ; @BT CB A3
    OR (($4D-7)+7)                       ; @BT F6 4D
    OR E                         ; @BT B3
    RLC C                        ; @BT CB 01
    RES 3,D                      ; @BT CB 9A
    ADD A,(HL)                   ; @BT 86
    DEC D                        ; @BT 15
    BIT 2,C                      ; @BT CB 51
    AND ((($B2*6)/3)-($B2))                      ; @BT E6 B2
    JP NEQ,((($2344*10)/5)-($2344))                 ; @BT C2 44 23
    RES 1,L                      ; @BT CB 8D
    LD A,D                       ; @BT 7A
    BIT 1,H                      ; @BT CB 4C
    BIT 2,H                      ; @BT CB 54
    RST (($30*2)-(($30)-0))                      ; @BT F7
    OR A,E                       ; @BT B3
    LD L,H                       ; @BT 6C
    SET 0,B                      ; @BT CB C0
    BIT 3,B                      ; @BT CB 58
    XOR ((($EE+11)-4)-7)                      ; @BT EE EE
    SET 5,L                      ; @BT CB ED
    RET GTE                      ; @BT D0
    SLA (HL)                     ; @BT CB 26
    SUB A,E                      ; @BT 93
    SRA H                        ; @BT CB 2C
    BIT 5,(HL)                   ; @BT CB 6E
    RST ((($00+3)-1)-2)                      ; @BT C7
    LDI (HL),A                   ; @BT 22
    LD A,(DE)                    ; @BT 1A
    LD E,E                       ; @BT 5B
    SET 2,B                      ; @BT CB D0
    LD C,A                       ; @BT 4F
    BIT 7,B                      ; @BT CB 78
    LD H,(HL)                    ; @BT 66
    DEC BC                       ; @BT 0B
    ADD B                        ; @BT 80
    ADC A,H                      ; @BT 8C
    AND A,(HL)                   ; @BT A6
    LD E,C                       ; @BT 59
    SET 5,B                      ; @BT CB E8
    LD A,H                       ; @BT 7C
    LD DE,(($2CDD+5)-5)                  ; @BT 11 DD 2C
    XOR A,D                      ; @BT AA
    CP (($E0-7)+7)                       ; @BT FE E0
    OR A,B                       ; @BT B0
    JP GEQ,((($2233*6)/3)-($2233))                 ; @BT D2 33 22
    RES 7,E                      ; @BT CB BB
    CP A                         ; @BT BF
    LD B,D                       ; @BT 42
    ADD A,B                      ; @BT 80
    RL E                         ; @BT CB 13
    LD HL,((($2DEE*10)/5)-($2DEE))                  ; @BT 21 EE 2D
    RES 0,D                      ; @BT CB 82
    XOR A,H                      ; @BT AC
    OR A,L                       ; @BT B5
    RLC B                        ; @BT CB 00
    AND E                        ; @BT A3
    SBC B                        ; @BT 98
    RES 4,C                      ; @BT CB A1
    BIT 5,C                      ; @BT CB 69
    LD H,B                       ; @BT 60
    LD A,(HL-)                   ; @BT 3A
    LD (HL),D                    ; @BT 72
    POP DE                       ; @BT D1
    LD E,A                       ; @BT 5F
    BIT 0,(HL)                   ; @BT CB 46
    CP A,B                       ; @BT B8
    RES 1,A                      ; @BT CB 8F
    JR NEQ,(($13*2)-(($13)-0))                   ; @BT 20 13
    RES 7,D                      ; @BT CB BA
    LD HL,SP+((($15+11)-4)-7)                 ; @BT F8 15
    AND A,C                      ; @BT A1
    BIT 5,H                      ; @BT CB 6C
    RLC H                        ; @BT CB 04
    BIT 7,C                      ; @BT CB 79
    LD HL,SP-((($2C+3)-1)-2)                 ; @BT F8 D4
    INC (HL)                     ; @BT 34
    RL B                         ; @BT CB 10
    SET 6,D                      ; @BT CB F2
    RET NC                       ; @BT D0
    LD (HL-),A                   ; @BT 32
    RES 5,L                      ; @BT CB AD
    LD C,L                       ; @BT 4D
    SBC A,(($64+5)-5)                    ; @BT DE 64
    SET 5,A                      ; @BT CB EF
    INC L                        ; @BT 2C
    SRA B                        ; @BT CB 28
    SET 5,C                      ; @BT CB E9
    PUSH DE                      ; @BT D5
    SET 6,H                      ; @BT CB F4
    PUSH BC                      ; @BT C5
    LD E,D                       ; @BT 5A
    LD L,D                       ; @BT 6A
    RST (($20-7)+7)                      ; @BT E7
    LD D,L                       ; @BT 55
    CALL ((($1DEF*6)/3)-($1DEF))                   ; @BT CD EF 1D
    POP BC                       ; @BT C1
    OR A,C                       ; @BT B1
    SET 5,(HL)                   ; @BT CB EE
    LD L,((($F1*10)/5)-($F1))                     ; @BT 2E F1
    RET C                        ; @BT D8
    RES 3,B                      ; @BT CB 98
    SET 0,E                      ; @BT CB C3
    RRCA                         ; @BT 0F
    RES 5,(HL)                   ; @BT CB AE
    SBC L                        ; @BT 9D
    SET 5,H                      ; @BT CB EC
    LD ((($3010*2)-(($3010)-0))),SP                ; @BT 08 10 30
    BIT 2,L                      ; @BT CB 55
    SET 4,E                      ; @BT CB E3
    BIT 0,D                      ; @BT CB 42
    LDD A,(HL)                   ; @BT 3A
    CALL C,((($19AB+11)-4)-7)                 ; @BT DC AB 19
    RL H                         ; @BT CB 14
    RES 3,E                      ; @BT CB 9B

    .db "<03"               ; @BT END

    /////////////////////////////////////////////////////////////////////////////
    // TEST-04: similar to TEST-03 and then defines + expressions
    /////////////////////////////////////////////////////////////////////////////

















    ; TEST-04 generated defines
    .define T4_DEF_001 (($0B+5)-5)
    .define T4_DEF_002 ((T4_DEF_001)+(($2566)-($0B)))
    .define T4_DEF_003 (($2ABB-7)+7)
    .define T4_DEF_004 (((T4_DEF_003)+($2788))-($2ABB))
    .define T4_DEF_005 ((($1789*6)/3)-($1789))
    .define T4_DEF_006 (((T4_DEF_005)*2+($28)-(($1789)*2)))
    .define T4_DEF_007 ((($28*10)/5)-($28))
    .define T4_DEF_008 ((($1F00)-($28))+(T4_DEF_007))
    .define T4_DEF_009 (($01*2)-(($01)-0))
    .define T4_DEF_010 ((T4_DEF_009)+(($50)-(($01+3)-3)))
    .define T4_DEF_011 ((($56+11)-4)-7)
    .define T4_DEF_012 ((T4_DEF_011)+(($DA)-($56)))
    .define T4_DEF_013 ((($39+3)-1)-2)
    .define T4_DEF_014 (((T4_DEF_013)+($C0))-($39))
    .define T4_DEF_015 (($1567+5)-5)
    .define T4_DEF_016 (((T4_DEF_015)*2+($36)-(($1567)*2)))
    .define T4_DEF_017 (($1345-7)+7)
    .define T4_DEF_018 ((($2EFF)-($1345))+(T4_DEF_017))
    .define T4_DEF_019 ((($10*6)/3)-($10))
    .define T4_DEF_020 ((T4_DEF_019)+(($3C)-(($10+3)-3)))
    .define T4_DEF_021 ((($08*10)/5)-($08))
    .define T4_DEF_022 ((T4_DEF_021)+(($C3)-($08)))
    .define T4_DEF_023 (($2677*2)-(($2677)-0))
    .define T4_DEF_024 (((T4_DEF_023)+($2899))-($2677))
    .define T4_DEF_025 ((($C9+11)-4)-7)
    .define T4_DEF_026 (((T4_DEF_025)*2+($06)-(($C9)*2)))
    .define T4_DEF_027 ((($189A+3)-1)-2)
    .define T4_DEF_028 ((($22)-($189A))+(T4_DEF_027))
    .define T4_DEF_029 (($18+5)-5)
    .define T4_DEF_030 ((T4_DEF_029)+(($1678)-(($18+3)-3)))
    .define T4_DEF_031 (($0E-7)+7)
    .define T4_DEF_032 ((T4_DEF_031)+(($92)-($0E)))
    .define T4_DEF_033 ((($37*6)/3)-($37))
    .define T4_DEF_034 (((T4_DEF_033)+($67))-($37))
    .define T4_DEF_035 ((($A9*10)/5)-($A9))
    .define T4_DEF_036 (((T4_DEF_035)*2+($29AA)-(($A9)*2)))
    .define T4_DEF_037 (($1BCD*2)-(($1BCD)-0))
    .define T4_DEF_038 ((($9B)-($1BCD))+(T4_DEF_037))
    .db "04>"               ; @BT TEST-04 04 START

    ; GENERATED: copied from TEST-03 with varied define-based operand replacement
    ; GENERATED: replaced operands = 38 / 76

    ; GENERATED: copied from TEST-02 with varied arithmetic expressions for numeric operands

    ; GENERATED: copied from TEST-01 with full TEST-02 randomization
    ; GENERATED: seed = 1704833320

    AND A,D                      ; @BT A2
    LD (HL),A                    ; @BT 77
    ADC D                        ; @BT 8A
    SCF                          ; @BT 37
    ADD A,E                      ; @BT 83
    LD A,T4_DEF_001+5-5                     ; @BT 3E 0B
    CP A,C                       ; @BT B9
    SET 1,A                      ; @BT CB CF
    LD A,(HLI)                   ; @BT 2A
    LDH (C),A                    ; @BT E2
    ADD A,L                      ; @BT 85
    BIT 2,A                      ; @BT CB 57
    RLCA                         ; @BT 07
    ADC H                        ; @BT 8C
    RL A                         ; @BT CB 17
    LD H,A                       ; @BT 67
    SET 6,B                      ; @BT CB F0
    SRA E                        ; @BT CB 2B
    SET 1,(HL)                   ; @BT CB CE
    DEC DE                       ; @BT 1B
    SLA C                        ; @BT CB 21
    JP C,((T4_DEF_002-7)+7)                   ; @BT DA 66 25
    ADC A,B                      ; @BT 88
    SET 2,H                      ; @BT CB D4
    LD A,((((T4_DEF_003*6)/3)-($2ABB)))                 ; @BT FA BB 2A
    CP H                         ; @BT BC
    SRA D                        ; @BT CB 2A
    RLC E                        ; @BT CB 03
    ADC L                        ; @BT 8D
    CP A,L                       ; @BT BD
    RES 7,C                      ; @BT CB B9
    LD D,A                       ; @BT 57
    LD L,B                       ; @BT 68
    RES 2,D                      ; @BT CB 92
    BIT 4,D                      ; @BT CB 62
    SLA H                        ; @BT CB 24
    SET 3,C                      ; @BT CB D9
    SLA E                        ; @BT CB 23
    SRL H                        ; @BT CB 3C
    SET 7,D                      ; @BT CB FA
    EI                           ; @BT FB
    SRL L                        ; @BT CB 3D
    SRL A                        ; @BT CB 3F
    ADC (HL)                     ; @BT 8E
    INC SP                       ; @BT 33
    ADD A,A                      ; @BT 87
    SET 3,L                      ; @BT CB DD
    AND A                        ; @BT A7
    ADD A,H                      ; @BT 84
    CPL                          ; @BT 2F
    RLA                          ; @BT 17
    RES 2,E                      ; @BT CB 93
    SBC A,(HL)                   ; @BT 9E
    ADD A,D                      ; @BT 82
    BIT 0,C                      ; @BT CB 41
    BIT 5,B                      ; @BT CB 68
    CP B                         ; @BT B8
    RES 3,C                      ; @BT CB 99
    RLC D                        ; @BT CB 02
    LD A,(HLD)                   ; @BT 3A
    SET 1,C                      ; @BT CB C9
    ADC A                        ; @BT 8F
    SET 2,D                      ; @BT CB D2
    BIT 7,(HL)                   ; @BT CB 7E
    LD A,L                       ; @BT 7D
    RES 6,C                      ; @BT CB B1
    SBC H                        ; @BT 9C
    SBC D                        ; @BT 9A
    XOR A,C                      ; @BT A9
    SET 3,D                      ; @BT CB DA
    JP NZ,(((T4_DEF_004*10)/5)-($2788))                  ; @BT C2 88 27
    BIT 0,E                      ; @BT CB 43
    RES 6,H                      ; @BT CB B4
    SET 1,E                      ; @BT CB CB
    BIT 7,L                      ; @BT CB 7D
    SET 3,A                      ; @BT CB DF
    PUSH AF                      ; @BT F5
    RETI                         ; @BT D9
    XOR D                        ; @BT AA
    SUB A,(HL)                   ; @BT 96
    SWAP L                       ; @BT CB 35
    LD E,(HL)                    ; @BT 5E
    RR E                         ; @BT CB 1B
    SWAP D                       ; @BT CB 32
    SRL B                        ; @BT CB 38
    SET 2,A                      ; @BT CB D7
    CALL NEQ,((T4_DEF_005*2)-(($1789)-0))               ; @BT C4 89 17
    LD A,(HL)                    ; @BT 7E
    BIT 6,C                      ; @BT CB 71
    XOR E                        ; @BT AB
    RES 7,B                      ; @BT CB B8
    XOR L                        ; @BT AD
    ADD HL,HL                    ; @BT 29
    CP D                         ; @BT BA
    SET 4,H                      ; @BT CB E4
    RES 1,B                      ; @BT CB 88
    SLA A                        ; @BT CB 27
    ADD HL,BC                    ; @BT 09
    RST (((T4_DEF_006+11)-4)-7)                      ; @BT EF
    SET 4,A                      ; @BT CB E7
    BIT 2,(HL)                   ; @BT CB 56
    RES 3,L                      ; @BT CB 9D
    LD L,A                       ; @BT 6F
    AND (HL)                     ; @BT A6
    RES 2,B                      ; @BT CB 90
    SWAP C                       ; @BT CB 31
    RES 5,A                      ; @BT CB AF
    RES 7,A                      ; @BT CB BF
    LD B,A                       ; @BT 47
    LD (DE),A                    ; @BT 12
    BIT 6,B                      ; @BT CB 70
    ADC A,(((T4_DEF_007+3)-1)-2)                    ; @BT CE 28
    LD H,H                       ; @BT 64
    RES 2,(HL)                   ; @BT CB 96
    JP LSS,((T4_DEF_008+5)-5)                 ; @BT DA 00 1F
    BIT 1,C                      ; @BT CB 49
    JR C,((T4_DEF_009-7)+7)                     ; @BT 38 01
    SET 0,L                      ; @BT CB C5
    SRL E                        ; @BT CB 3B
    RET                          ; @BT C9
    RRC A                        ; @BT CB 0F
    BIT 5,L                      ; @BT CB 6D
    RES 2,A                      ; @BT CB 97
    RR C                         ; @BT CB 19
    JP HL                        ; @BT E9
    XOR A,A                      ; @BT AF
    XOR B                        ; @BT A8
    INC D                        ; @BT 14
    ADC E                        ; @BT 8B
    CP A,A                       ; @BT BF
    RET LSS                      ; @BT D8
    XOR A,L                      ; @BT AD
    SUB A,H                      ; @BT 94
    SET 3,H                      ; @BT CB DC
    LD H,L                       ; @BT 65
    RES 0,L                      ; @BT CB 85
    SET 2,(HL)                   ; @BT CB D6
    RES 5,C                      ; @BT CB A9
    SUB (HL)                     ; @BT 96
    LD D,E                       ; @BT 53
    XOR H                        ; @BT AC
    ADD L                        ; @BT 85
    SWAP A                       ; @BT CB 37
    SET 4,(HL)                   ; @BT CB E6
    LD D,(((T4_DEF_010*6)/3)-($50))                     ; @BT 16 50
    SET 0,H                      ; @BT CB C4
    BIT 3,L                      ; @BT CB 5D
    SUB A                        ; @BT 97
    RLC L                        ; @BT CB 05
    SET 1,L                      ; @BT CB CD
    LD B,H                       ; @BT 44
    LD D,B                       ; @BT 50
    AND L                        ; @BT A5
    LD E,L                       ; @BT 5D
    INC A                        ; @BT 3C
    BIT 1,(HL)                   ; @BT CB 4E
    BIT 1,A                      ; @BT CB 4F
    ADD A,(((T4_DEF_011*10)/5)-($56))                    ; @BT C6 56
    BIT 5,E                      ; @BT CB 6B
    SET 2,C                      ; @BT CB D1
    LD H,((T4_DEF_012*2)-(($DA)-0))                     ; @BT 26 DA
    SWAP E                       ; @BT CB 33
    INC C                        ; @BT 0C
    RES 6,(HL)                   ; @BT CB B6
    LD C,(((T4_DEF_013+11)-4)-7)                     ; @BT 0E 39
    RES 6,B                      ; @BT CB B0
    BIT 0,B                      ; @BT CB 40
    BIT 6,E                      ; @BT CB 73
    SRA (HL)                     ; @BT CB 2E
    BIT 7,H                      ; @BT CB 7C
    SET 6,L                      ; @BT CB F5
    AND A,A                      ; @BT A7
    BIT 3,C                      ; @BT CB 59
    SBC A,D                      ; @BT 9A
    SET 1,D                      ; @BT CB CA
    RES 2,L                      ; @BT CB 95
    SUB B                        ; @BT 90
    SUB (((T4_DEF_014+3)-1)-2)                      ; @BT D6 C0
    BIT 0,H                      ; @BT CB 44
    OR L                         ; @BT B5
    CALL GTE,((T4_DEF_015+5)-5)               ; @BT D4 67 15
    RES 7,L                      ; @BT CB BD
    AND D                        ; @BT A2
    CP A,E                       ; @BT BB
    LDH ($1F),A                  ; @BT E0 1F
    BIT 1,E                      ; @BT CB 4B
    LDI A,(HL)                   ; @BT 2A
    DEC C                        ; @BT 0D
    LD L,(HL)                    ; @BT 6E
    OR A,((T4_DEF_016-7)+7)                     ; @BT F6 36
    CALL LSS,(((T4_DEF_017*6)/3)-($1345))               ; @BT DC 45 13
    LD B,L                       ; @BT 45
    XOR C                        ; @BT A9
    ADD A                        ; @BT 87
    LD H,C                       ; @BT 61
    AND B                        ; @BT A0
    ADC B                        ; @BT 88
    LD L,C                       ; @BT 69
    BIT 0,A                      ; @BT CB 47
    RES 5,B                      ; @BT CB A8
    LD SP,(((T4_DEF_018*10)/5)-($2EFF))                  ; @BT 31 FF 2E
    BIT 3,(HL)                   ; @BT CB 5E
    CPL A                        ; @BT 2F
    LD (HL),C                    ; @BT 71
    RST ((T4_DEF_019*2)-(($10)-0))                      ; @BT D7
    BIT 2,D                      ; @BT CB 52
    RR D                         ; @BT CB 1A
    ADD (HL)                     ; @BT 86
    SRA C                        ; @BT CB 29
    LD L,E                       ; @BT 6B
    XOR A                        ; @BT AF
    SUB A,A                      ; @BT 97
    RRC (HL)                     ; @BT CB 0E
    SET 7,L                      ; @BT CB FD
    JR GEQ,(((T4_DEF_020+11)-4)-7)                   ; @BT 30 3C
    ADD D                        ; @BT 82
    RLC (HL)                     ; @BT CB 06
    SET 7,(HL)                   ; @BT CB FE
    RST (((T4_DEF_021+3)-1)-2)                      ; @BT CF
    LD (HL),((T4_DEF_022+5)-5)                  ; @BT 36 C3
    DEC H                        ; @BT 25
    RES 3,H                      ; @BT CB 9C
    JP NC,((T4_DEF_023-7)+7)                  ; @BT D2 77 26
    LD L,L                       ; @BT 6D
    LD D,D                       ; @BT 52
    RES 1,D                      ; @BT CB 8A
    SLA B                        ; @BT CB 20
    SET 3,B                      ; @BT CB D8
    RES 3,(HL)                   ; @BT CB 9E
    OR A                         ; @BT B7
    ADC A,C                      ; @BT 89
    LD C,D                       ; @BT 4A
    LD D,H                       ; @BT 54
    HALT                         ; @BT 76
    RES 4,H                      ; @BT CB A4
    SLA L                        ; @BT CB 25
    SUB A,B                      ; @BT 90
    RL D                         ; @BT CB 12
    SET 6,E                      ; @BT CB F3
    BIT 7,E                      ; @BT CB 7B
    SBC (HL)                     ; @BT 9E
    SUB L                        ; @BT 95
    SBC A                        ; @BT 9F
    SET 0,A                      ; @BT CB C7
    BIT 1,B                      ; @BT CB 48
    BIT 2,B                      ; @BT CB 50
    JP Z,(((T4_DEF_024*6)/3)-($2899))                   ; @BT CA 99 28
    SUB A,L                      ; @BT 95
    CP A,(((T4_DEF_025*10)/5)-($C9))                     ; @BT FE C9
    JR Z,((T4_DEF_026*2)-(($06)-0))                     ; @BT 28 06
    RES 4,B                      ; @BT CB A0
    RL L                         ; @BT CB 15
    ADD HL,SP                    ; @BT 39
    BIT 7,D                      ; @BT CB 7A
    BIT 3,D                      ; @BT CB 5A
    BIT 4,C                      ; @BT CB 61
    SUB E                        ; @BT 93
    LD E,H                       ; @BT 5C
    INC HL                       ; @BT 23
    SET 7,H                      ; @BT CB FC
    SBC A,H                      ; @BT 9C
    RET NZ                       ; @BT C0
    LD B,B                       ; @BT 40
    RRC L                        ; @BT CB 0D
    SET 0,(HL)                   ; @BT CB C6
    XOR (HL)                     ; @BT AE
    CALL EQU,(((T4_DEF_027+11)-4)-7)               ; @BT CC 9A 18
    LD B,(((T4_DEF_028+3)-1)-2)                     ; @BT 06 22
    ADC A,E                      ; @BT 8B
    JR NC,((T4_DEF_029+5)-5)                    ; @BT 30 18
    PUSH HL                      ; @BT E5
    LD A,A                       ; @BT 7F
    LD H,E                       ; @BT 63
    SET 4,L                      ; @BT CB E5
    BIT 3,H                      ; @BT CB 5C
    LD C,H                       ; @BT 4C
    CALL GEQ,((T4_DEF_030-7)+7)               ; @BT D4 78 16
    ADC C                        ; @BT 89
    JR LST,(((T4_DEF_031*6)/3)-($0E))                   ; @BT 38 0E
    LDD (HL),A                   ; @BT 32
    STOP (((T4_DEF_032*10)/5)-($92))                     ; @BT 10 92
    ADC A,L                      ; @BT 8D
    RES 3,A                      ; @BT CB 9F
    SET 7,A                      ; @BT CB FF
    OR D                         ; @BT B2
    INC BC                       ; @BT 03
    RET LST                      ; @BT D8
    OR H                         ; @BT B4
    RRA                          ; @BT 1F
    BIT 7,A                      ; @BT CB 7F
    BIT 4,A                      ; @BT CB 67
    OR A,(HL)                    ; @BT B6
    RES 0,E                      ; @BT CB 83
    JR LSS,((T4_DEF_033*2)-(($37)-0))                   ; @BT 38 37
    RES 0,(HL)                   ; @BT CB 86
    SUB C                        ; @BT 91
    LD A,($FF00+$F4)             ; @BT F0 F4
    DEC (HL)                     ; @BT 35
    CCF                          ; @BT 3F
    RRC C                        ; @BT CB 09
    ADD E                        ; @BT 83
    CP A,(HL)                    ; @BT BE
    LD E,(((T4_DEF_034+11)-4)-7)                     ; @BT 1E 67
    RRC E                        ; @BT CB 0B
    SUB A,(((T4_DEF_035+3)-1)-2)                    ; @BT D6 A9
    DI                           ; @BT F3
    AND C                        ; @BT A1
    RET Z                        ; @BT C8
    OR (HL)                      ; @BT B6
    LD (HL),E                    ; @BT 73
    LD ($FF00+$7E),A             ; @BT E0 7E
    RR B                         ; @BT CB 18
    SRL (HL)                     ; @BT CB 3E
    RES 7,H                      ; @BT CB BC
    JP ((T4_DEF_036+5)-5)                     ; @BT C3 AA 29
    RES 4,D                      ; @BT CB A2
    RRC D                        ; @BT CB 0A
    SBC A,E                      ; @BT 9B
    RES 2,C                      ; @BT CB 91
    BIT 1,L                      ; @BT CB 4D
    SBC A,B                      ; @BT 98
    INC E                        ; @BT 1C
    CP (HL)                      ; @BT BE
    OR C                         ; @BT B1
    LD D,(HL)                    ; @BT 56
    SUB H                        ; @BT 94
    RET EQU                      ; @BT C8
    LD B,E                       ; @BT 43
    SLA D                        ; @BT CB 22
    CP A,H                       ; @BT BC
    BIT 6,D                      ; @BT CB 72
    RET NEQ                      ; @BT C0
    LD (HLI),A                   ; @BT 22
    LD (HL),L                    ; @BT 75
    LD ($FF00+C),A               ; @BT E2
    XOR A,E                      ; @BT AB
    AND A,L                      ; @BT A5
    RET GEQ                      ; @BT D0
    SBC A,C                      ; @BT 99
    SET 2,L                      ; @BT CB D5
    ADD A,C                      ; @BT 81
    INC DE                       ; @BT 13
    SET 4,B                      ; @BT CB E0
    INC B                        ; @BT 04
    STOP                         ; @BT 10
    LD (HL+),A                   ; @BT 22
    SBC C                        ; @BT 99
    DEC E                        ; @BT 1D
    DEC B                        ; @BT 05
    RES 7,(HL)                   ; @BT CB BE
    BIT 6,H                      ; @BT CB 74
    SET 0,C                      ; @BT CB C1
    XOR A,(HL)                   ; @BT AE
    CALL NZ,((T4_DEF_037-7)+7)                ; @BT C4 CD 1B
    SWAP (HL)                    ; @BT CB 36
    BIT 4,E                      ; @BT CB 63
    SET 7,B                      ; @BT CB F8
    SBC E                        ; @BT 9B
    DEC HL                       ; @BT 2B
    RES 0,A                      ; @BT CB 87
    JP (HL)                      ; @BT E9
    AND A,(((T4_DEF_038*6)/3)-($9B))                    ; @BT E6 9B
    BIT 4,L                      ; @BT CB 65
    ADC ((($3F*10)/5)-($3F))                      ; @BT CE 3F
    LD PC,HL                     ; @BT E9
    SRA A                        ; @BT CB 2F
    CP C                         ; @BT B9
    DEC L                        ; @BT 2D
    RR H                         ; @BT CB 1C
    JR EQU,(($2A*2)-(($2A)-0))                   ; @BT 28 2A
    XOR A,B                      ; @BT A8
    ADC A,D                      ; @BT 8A
    RR L                         ; @BT CB 1D
    BIT 5,A                      ; @BT CB 6F
    LDH A,(C)                    ; @BT F2
    LD B,(HL)                    ; @BT 46
    BIT 0,L                      ; @BT CB 45
    LD A,(BC)                    ; @BT 0A
    LDH A,($08)                  ; @BT F0 08
    SRA L                        ; @BT CB 2D
    AND A,B                      ; @BT A0
    BIT 3,E                      ; @BT CB 5B
    RL (HL)                      ; @BT CB 16
    SET 6,A                      ; @BT CB F7
    SBC ((($7B+11)-4)-7)                      ; @BT DE 7B
    LD B,C                       ; @BT 41
    LD BC,((($2BCC+3)-1)-2)                  ; @BT 01 CC 2B
    RST (($38+5)-5)                      ; @BT FF
    SET 4,D                      ; @BT CB E2
    SET 5,E                      ; @BT CB EB
    RES 4,A                      ; @BT CB A7
    LD (HL),H                    ; @BT 74
    LD E,B                       ; @BT 58
    ADD SP,(($2D-7)+7)                   ; @BT E8 2D
    RST ((($18*6)/3)-($18))                      ; @BT DF
    CP A,D                       ; @BT BA
    RES 1,H                      ; @BT CB 8C
    BIT 6,A                      ; @BT CB 77
    LD SP,HL                     ; @BT F9
    RES 6,D                      ; @BT CB B2
    OR A,H                       ; @BT B4
    LD (HLD),A                   ; @BT 32
    RES 0,H                      ; @BT CB 84
    JP EQU,((($2455*10)/5)-($2455))                 ; @BT CA 55 24
    AND H                        ; @BT A4
    RLC A                        ; @BT CB 07
    RES 4,L                      ; @BT CB A5
    LD (HL),B                    ; @BT 70
    RES 6,E                      ; @BT CB B3
    RES 5,H                      ; @BT CB AC
    SRL C                        ; @BT CB 39
    BIT 6,L                      ; @BT CB 75
    POP AF                       ; @BT F1
    XOR A,(($D7*2)-(($D7)-0))                    ; @BT EE D7
    ADD H                        ; @BT 84
    CALL Z,((($1CDE+11)-4)-7)                 ; @BT CC DE 1C
    LD D,C                       ; @BT 51
    JR ((($1D+3)-1)-2)                       ; @BT 18 1D
    OR A,D                       ; @BT B2
    RRC H                        ; @BT CB 0C
    SWAP B                       ; @BT CB 30
    RL C                         ; @BT CB 11
    RES 4,(HL)                   ; @BT CB A6
    RES 6,L                      ; @BT CB B5
    OR A,A                       ; @BT B7
    LD C,E                       ; @BT 4B
    SUB A,C                      ; @BT 91
    DEC SP                       ; @BT 3B
    LD C,(HL)                    ; @BT 4E
    RES 5,D                      ; @BT CB AA
    JP GTE,(($2122+5)-5)                 ; @BT D2 22 21
    JP LST,(($2011-7)+7)                 ; @BT DA 11 20
    ADD HL,DE                    ; @BT 19
    RR (HL)                      ; @BT CB 1E
    LD A,E                       ; @BT 7B
    CALL LST,((($1456*6)/3)-($1456))               ; @BT DC 56 14
    RES 0,C                      ; @BT CB 81
    RES 1,(HL)                   ; @BT CB 8E
    CP E                         ; @BT BB
    BIT 6,(HL)                   ; @BT CB 76
    SET 2,E                      ; @BT CB D3
    SET 1,H                      ; @BT CB CC
    RES 2,H                      ; @BT CB 94
    SRL D                        ; @BT CB 3A
    CALL NC,((($1ABC*10)/5)-($1ABC))                ; @BT D4 BC 1A
    JR GTE,(($25*2)-(($25)-0))                   ; @BT 30 25
    LD A,(HL+)                   ; @BT 2A
    LD (((($3121+11)-4)-7)),A                 ; @BT EA 21 31
    POP HL                       ; @BT E1
    DAA                          ; @BT 27
    SET 7,C                      ; @BT CB F9
    SET 5,D                      ; @BT CB EA
    RES 1,C                      ; @BT CB 89
    BIT 1,D                      ; @BT CB 4A
    ADC A,A                      ; @BT 8F
    AND A,E                      ; @BT A3
    AND A,H                      ; @BT A4
    LD A,($FF00+C)               ; @BT F2
    BIT 3,A                      ; @BT CB 5F
    RES 5,E                      ; @BT CB AB
    SET 6,(HL)                   ; @BT CB F6
    SET 4,C                      ; @BT CB E1
    ADD ((($84+3)-1)-2)                      ; @BT C6 84
    SWAP H                       ; @BT CB 34
    LD A,B                       ; @BT 78
    SUB D                        ; @BT 92
    SBC A,A                      ; @BT 9F
    LD A,C                       ; @BT 79
    SET 0,D                      ; @BT CB C2
    SET 3,(HL)                   ; @BT CB DE
    RES 0,B                      ; @BT CB 80
    ADD C                        ; @BT 81
    INC H                        ; @BT 24
    RES 6,A                      ; @BT CB B7
    BIT 2,E                      ; @BT CB 53
    SET 1,B                      ; @BT CB C8
    LD H,D                       ; @BT 62
    RES 1,E                      ; @BT CB 8B
    BIT 4,B                      ; @BT CB 60
    SET 7,E                      ; @BT CB FB
    LD C,C                       ; @BT 49
    OR B                         ; @BT B0
    BIT 4,H                      ; @BT CB 64
    LD C,B                       ; @BT 48
    ADC A,(HL)                   ; @BT 8E
    SET 3,E                      ; @BT CB DB
    SUB A,D                      ; @BT 92
    BIT 4,(HL)                   ; @BT CB 66
    JR NZ,(($2F+5)-5)                    ; @BT 20 2F
    DEC A                        ; @BT 3D
    LD (BC),A                    ; @BT 02
    NOP                          ; @BT 00
    RR A                         ; @BT CB 1F
    SBC A,L                      ; @BT 9D
    SET 6,C                      ; @BT CB F1
    CP L                         ; @BT BD
    BIT 5,D                      ; @BT CB 6A
    RRC B                        ; @BT CB 08
    RES 4,E                      ; @BT CB A3
    OR (($4D-7)+7)                       ; @BT F6 4D
    OR E                         ; @BT B3
    RLC C                        ; @BT CB 01
    RES 3,D                      ; @BT CB 9A
    ADD A,(HL)                   ; @BT 86
    DEC D                        ; @BT 15
    BIT 2,C                      ; @BT CB 51
    AND ((($B2*6)/3)-($B2))                      ; @BT E6 B2
    JP NEQ,((($2344*10)/5)-($2344))                 ; @BT C2 44 23
    RES 1,L                      ; @BT CB 8D
    LD A,D                       ; @BT 7A
    BIT 1,H                      ; @BT CB 4C
    BIT 2,H                      ; @BT CB 54
    RST (($30*2)-(($30)-0))                      ; @BT F7
    OR A,E                       ; @BT B3
    LD L,H                       ; @BT 6C
    SET 0,B                      ; @BT CB C0
    BIT 3,B                      ; @BT CB 58
    XOR ((($EE+11)-4)-7)                      ; @BT EE EE
    SET 5,L                      ; @BT CB ED
    RET GTE                      ; @BT D0
    SLA (HL)                     ; @BT CB 26
    SUB A,E                      ; @BT 93
    SRA H                        ; @BT CB 2C
    BIT 5,(HL)                   ; @BT CB 6E
    RST ((($00+3)-1)-2)                      ; @BT C7
    LDI (HL),A                   ; @BT 22
    LD A,(DE)                    ; @BT 1A
    LD E,E                       ; @BT 5B
    SET 2,B                      ; @BT CB D0
    LD C,A                       ; @BT 4F
    BIT 7,B                      ; @BT CB 78
    LD H,(HL)                    ; @BT 66
    DEC BC                       ; @BT 0B
    ADD B                        ; @BT 80
    ADC A,H                      ; @BT 8C
    AND A,(HL)                   ; @BT A6
    LD E,C                       ; @BT 59
    SET 5,B                      ; @BT CB E8
    LD A,H                       ; @BT 7C
    LD DE,(($2CDD+5)-5)                  ; @BT 11 DD 2C
    XOR A,D                      ; @BT AA
    CP (($E0-7)+7)                       ; @BT FE E0
    OR A,B                       ; @BT B0
    JP GEQ,((($2233*6)/3)-($2233))                 ; @BT D2 33 22
    RES 7,E                      ; @BT CB BB
    CP A                         ; @BT BF
    LD B,D                       ; @BT 42
    ADD A,B                      ; @BT 80
    RL E                         ; @BT CB 13
    LD HL,((($2DEE*10)/5)-($2DEE))                  ; @BT 21 EE 2D
    RES 0,D                      ; @BT CB 82
    XOR A,H                      ; @BT AC
    OR A,L                       ; @BT B5
    RLC B                        ; @BT CB 00
    AND E                        ; @BT A3
    SBC B                        ; @BT 98
    RES 4,C                      ; @BT CB A1
    BIT 5,C                      ; @BT CB 69
    LD H,B                       ; @BT 60
    LD A,(HL-)                   ; @BT 3A
    LD (HL),D                    ; @BT 72
    POP DE                       ; @BT D1
    LD E,A                       ; @BT 5F
    BIT 0,(HL)                   ; @BT CB 46
    CP A,B                       ; @BT B8
    RES 1,A                      ; @BT CB 8F
    JR NEQ,(($13*2)-(($13)-0))                   ; @BT 20 13
    RES 7,D                      ; @BT CB BA
    LD HL,SP+((($15+11)-4)-7)                 ; @BT F8 15
    AND A,C                      ; @BT A1
    BIT 5,H                      ; @BT CB 6C
    RLC H                        ; @BT CB 04
    BIT 7,C                      ; @BT CB 79
    LD HL,SP-((($2C+3)-1)-2)                 ; @BT F8 D4
    INC (HL)                     ; @BT 34
    RL B                         ; @BT CB 10
    SET 6,D                      ; @BT CB F2
    RET NC                       ; @BT D0
    LD (HL-),A                   ; @BT 32
    RES 5,L                      ; @BT CB AD
    LD C,L                       ; @BT 4D
    SBC A,(($64+5)-5)                    ; @BT DE 64
    SET 5,A                      ; @BT CB EF
    INC L                        ; @BT 2C
    SRA B                        ; @BT CB 28
    SET 5,C                      ; @BT CB E9
    PUSH DE                      ; @BT D5
    SET 6,H                      ; @BT CB F4
    PUSH BC                      ; @BT C5
    LD E,D                       ; @BT 5A
    LD L,D                       ; @BT 6A
    RST (($20-7)+7)                      ; @BT E7
    LD D,L                       ; @BT 55
    CALL ((($1DEF*6)/3)-($1DEF))                   ; @BT CD EF 1D
    POP BC                       ; @BT C1
    OR A,C                       ; @BT B1
    SET 5,(HL)                   ; @BT CB EE
    LD L,((($F1*10)/5)-($F1))                     ; @BT 2E F1
    RET C                        ; @BT D8
    RES 3,B                      ; @BT CB 98
    SET 0,E                      ; @BT CB C3
    RRCA                         ; @BT 0F
    RES 5,(HL)                   ; @BT CB AE
    SBC L                        ; @BT 9D
    SET 5,H                      ; @BT CB EC
    LD ((($3010*2)-(($3010)-0))),SP                ; @BT 08 10 30
    BIT 2,L                      ; @BT CB 55
    SET 4,E                      ; @BT CB E3
    BIT 0,D                      ; @BT CB 42
    LDD A,(HL)                   ; @BT 3A
    CALL C,((($19AB+11)-4)-7)                 ; @BT DC AB 19
    RL H                         ; @BT CB 14
    RES 3,E                      ; @BT CB 9B

    .db "<04"               ; @BT END

    /////////////////////////////////////////////////////////////////////////////
    // TEST-05: similar to TEST-04 and defines + functions + expressions
    /////////////////////////////////////////////////////////////////////////////









    ; TEST-05 generated defines/functions
    .define T5_DEF_001 (($0B+5)-5)
    .define T5_DEF_002 ((T5_DEF_001)+(($2566)-($0B)))
    .define T5_DEF_003 (($2ABB-7)+7)
    .define T5_DEF_004 (((T5_DEF_003)+($2788))-($2ABB))
    .define T5_DEF_005 ((($1789*6)/3)-($1789))
    .define T5_DEF_006 (((T5_DEF_005)*2+($28)-(($1789)*2)))
    .define T5_DEF_007 ((($28*10)/5)-($28))
    .define T5_DEF_008 ((($1F00)-($28))+(T5_DEF_007))
    .define T5_DEF_009 (($01*2)-(($01)-0))
    .define T5_DEF_010 ((T5_DEF_009)+(($50)-(($01+3)-3)))
    .define T5_DEF_011 ((($56+11)-4)-7)
    .define T5_DEF_012 ((T5_DEF_011)+(($DA)-($56)))
    .define T5_DEF_013 ((($39+3)-1)-2)
    .define T5_DEF_014 (((T5_DEF_013)+($C0))-($39))
    .define T5_DEF_015 (($1567+5)-5)
    .define T5_DEF_016 (((T5_DEF_015)*2+($36)-(($1567)*2)))
    .define T5_DEF_017 (($1345-7)+7)
    .define T5_DEF_018 ((($2EFF)-($1345))+(T5_DEF_017))
    .define T5_DEF_019 ((($10*6)/3)-($10))
    .define T5_DEF_020 ((T5_DEF_019)+(($3C)-(($10+3)-3)))
    .define T5_DEF_021 ((($08*10)/5)-($08))
    .define T5_DEF_022 ((T5_DEF_021)+(($C3)-($08)))
    .define T5_DEF_023 (($2677*2)-(($2677)-0))
    .define T5_DEF_024 (((T5_DEF_023)+($2899))-($2677))
    .define T5_DEF_025 ((($C9+11)-4)-7)
    .define T5_DEF_026 (((T5_DEF_025)*2+($06)-(($C9)*2)))
    .define T5_DEF_027 ((($189A+3)-1)-2)
    .define T5_DEF_028 ((($22)-($189A))+(T5_DEF_027))
    .define T5_DEF_029 (($18+5)-5)
    .define T5_DEF_030 ((T5_DEF_029)+(($1678)-(($18+3)-3)))
    .define T5_DEF_031 (($0E-7)+7)
    .define T5_DEF_032 ((T5_DEF_031)+(($92)-($0E)))
    .define T5_DEF_033 ((($37*6)/3)-($37))
    .define T5_DEF_034 (((T5_DEF_033)+($67))-($37))
    .define T5_DEF_035 ((($A9*10)/5)-($A9))
    .define T5_DEF_036 (((T5_DEF_035)*2+($29AA)-(($A9)*2)))
    .define T5_DEF_037 (($1BCD*2)-(($1BCD)-0))
    .define T5_DEF_038 ((($9B)-($1BCD))+(T5_DEF_037))

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
    .db "05>"               ; @BT TEST-05 05 START

    ; GENERATED: copied from TEST-04; half define-label references replaced by varied function calls
    ; GENERATED: replaced define refs = 19 / 38

    ; GENERATED: copied from TEST-03 with varied define-based operand replacement
    ; GENERATED: replaced operands = 38 / 76

    ; GENERATED: copied from TEST-02 with varied arithmetic expressions for numeric operands

    ; GENERATED: copied from TEST-01 with full TEST-02 randomization
    ; GENERATED: seed = 1704833320

    AND A,D                      ; @BT A2
    LD (HL),A                    ; @BT 77
    ADC D                        ; @BT 8A
    SCF                          ; @BT 37
    ADD A,E                      ; @BT 83
    LD A,T5_FN_001(T5_DEF_001+2,2)+5-5                     ; @BT 3E 0B
    CP A,C                       ; @BT B9
    SET 1,A                      ; @BT CB CF
    LD A,(HLI)                   ; @BT 2A
    LDH (C),A                    ; @BT E2
    ADD A,L                      ; @BT 85
    BIT 2,A                      ; @BT CB 57
    RLCA                         ; @BT 07
    ADC H                        ; @BT 8C
    RL A                         ; @BT CB 17
    LD H,A                       ; @BT 67
    SET 6,B                      ; @BT CB F0
    SRA E                        ; @BT CB 2B
    SET 1,(HL)                   ; @BT CB CE
    DEC DE                       ; @BT 1B
    SLA C                        ; @BT CB 21
    JP C,((T5_FN_002(T5_DEF_002+3,3)-7)+7)                   ; @BT DA 66 25
    ADC A,B                      ; @BT 88
    SET 2,H                      ; @BT CB D4
    LD A,((((T5_FN_003(T5_DEF_003+4,5,4+5)*6)/3)-($2ABB)))                 ; @BT FA BB 2A
    CP H                         ; @BT BC
    SRA D                        ; @BT CB 2A
    RLC E                        ; @BT CB 03
    ADC L                        ; @BT 8D
    CP A,L                       ; @BT BD
    RES 7,C                      ; @BT CB B9
    LD D,A                       ; @BT 57
    LD L,B                       ; @BT 68
    RES 2,D                      ; @BT CB 92
    BIT 4,D                      ; @BT CB 62
    SLA H                        ; @BT CB 24
    SET 3,C                      ; @BT CB D9
    SLA E                        ; @BT CB 23
    SRL H                        ; @BT CB 3C
    SET 7,D                      ; @BT CB FA
    EI                           ; @BT FB
    SRL L                        ; @BT CB 3D
    SRL A                        ; @BT CB 3F
    ADC (HL)                     ; @BT 8E
    INC SP                       ; @BT 33
    ADD A,A                      ; @BT 87
    SET 3,L                      ; @BT CB DD
    AND A                        ; @BT A7
    ADD A,H                      ; @BT 84
    CPL                          ; @BT 2F
    RLA                          ; @BT 17
    RES 2,E                      ; @BT CB 93
    SBC A,(HL)                   ; @BT 9E
    ADD A,D                      ; @BT 82
    BIT 0,C                      ; @BT CB 41
    BIT 5,B                      ; @BT CB 68
    CP B                         ; @BT B8
    RES 3,C                      ; @BT CB 99
    RLC D                        ; @BT CB 02
    LD A,(HLD)                   ; @BT 3A
    SET 1,C                      ; @BT CB C9
    ADC A                        ; @BT 8F
    SET 2,D                      ; @BT CB D2
    BIT 7,(HL)                   ; @BT CB 7E
    LD A,L                       ; @BT 7D
    RES 6,C                      ; @BT CB B1
    SBC H                        ; @BT 9C
    SBC D                        ; @BT 9A
    XOR A,C                      ; @BT A9
    SET 3,D                      ; @BT CB DA
    JP NZ,(((T5_FN_004(T5_DEF_004+5,6,5+6)*10)/5)-($2788))                  ; @BT C2 88 27
    BIT 0,E                      ; @BT CB 43
    RES 6,H                      ; @BT CB B4
    SET 1,E                      ; @BT CB CB
    BIT 7,L                      ; @BT CB 7D
    SET 3,A                      ; @BT CB DF
    PUSH AF                      ; @BT F5
    RETI                         ; @BT D9
    XOR D                        ; @BT AA
    SUB A,(HL)                   ; @BT 96
    SWAP L                       ; @BT CB 35
    LD E,(HL)                    ; @BT 5E
    RR E                         ; @BT CB 1B
    SWAP D                       ; @BT CB 32
    SRL B                        ; @BT CB 38
    SET 2,A                      ; @BT CB D7
    CALL NEQ,((T5_FN_005(T5_DEF_005+6+7,8,6,7+8)*2)-(($1789)-0))               ; @BT C4 89 17
    LD A,(HL)                    ; @BT 7E
    BIT 6,C                      ; @BT CB 71
    XOR E                        ; @BT AB
    RES 7,B                      ; @BT CB B8
    XOR L                        ; @BT AD
    ADD HL,HL                    ; @BT 29
    CP D                         ; @BT BA
    SET 4,H                      ; @BT CB E4
    RES 1,B                      ; @BT CB 88
    SLA A                        ; @BT CB 27
    ADD HL,BC                    ; @BT 09
    RST (((T5_FN_006(T5_DEF_006+7+8,9,7,8+9)+11)-4)-7)                      ; @BT EF
    SET 4,A                      ; @BT CB E7
    BIT 2,(HL)                   ; @BT CB 56
    RES 3,L                      ; @BT CB 9D
    LD L,A                       ; @BT 6F
    AND (HL)                     ; @BT A6
    RES 2,B                      ; @BT CB 90
    SWAP C                       ; @BT CB 31
    RES 5,A                      ; @BT CB AF
    RES 7,A                      ; @BT CB BF
    LD B,A                       ; @BT 47
    LD (DE),A                    ; @BT 12
    BIT 6,B                      ; @BT CB 70
    ADC A,(((T5_FN_007(T5_DEF_007+8,8)+3)-1)-2)                    ; @BT CE 28
    LD H,H                       ; @BT 64
    RES 2,(HL)                   ; @BT CB 96
    JP LSS,((T5_FN_008(T5_DEF_008+9,9)+5)-5)                 ; @BT DA 00 1F
    BIT 1,C                      ; @BT CB 49
    JR C,((T5_FN_009(T5_DEF_009+10,11,10+11)-7)+7)                     ; @BT 38 01
    SET 0,L                      ; @BT CB C5
    SRL E                        ; @BT CB 3B
    RET                          ; @BT C9
    RRC A                        ; @BT CB 0F
    BIT 5,L                      ; @BT CB 6D
    RES 2,A                      ; @BT CB 97
    RR C                         ; @BT CB 19
    JP HL                        ; @BT E9
    XOR A,A                      ; @BT AF
    XOR B                        ; @BT A8
    INC D                        ; @BT 14
    ADC E                        ; @BT 8B
    CP A,A                       ; @BT BF
    RET LSS                      ; @BT D8
    XOR A,L                      ; @BT AD
    SUB A,H                      ; @BT 94
    SET 3,H                      ; @BT CB DC
    LD H,L                       ; @BT 65
    RES 0,L                      ; @BT CB 85
    SET 2,(HL)                   ; @BT CB D6
    RES 5,C                      ; @BT CB A9
    SUB (HL)                     ; @BT 96
    LD D,E                       ; @BT 53
    XOR H                        ; @BT AC
    ADD L                        ; @BT 85
    SWAP A                       ; @BT CB 37
    SET 4,(HL)                   ; @BT CB E6
    LD D,(((T5_FN_010(T5_DEF_010+11,12,11+12)*6)/3)-($50))                     ; @BT 16 50
    SET 0,H                      ; @BT CB C4
    BIT 3,L                      ; @BT CB 5D
    SUB A                        ; @BT 97
    RLC L                        ; @BT CB 05
    SET 1,L                      ; @BT CB CD
    LD B,H                       ; @BT 44
    LD D,B                       ; @BT 50
    AND L                        ; @BT A5
    LD E,L                       ; @BT 5D
    INC A                        ; @BT 3C
    BIT 1,(HL)                   ; @BT CB 4E
    BIT 1,A                      ; @BT CB 4F
    ADD A,(((T5_FN_011(T5_DEF_011+12+13,14,12,13+14)*10)/5)-($56))                    ; @BT C6 56
    BIT 5,E                      ; @BT CB 6B
    SET 2,C                      ; @BT CB D1
    LD H,((T5_FN_012(T5_DEF_012+13+14,15,13,14+15)*2)-(($DA)-0))                     ; @BT 26 DA
    SWAP E                       ; @BT CB 33
    INC C                        ; @BT 0C
    RES 6,(HL)                   ; @BT CB B6
    LD C,(((T5_FN_013(T5_DEF_013+14,14)+11)-4)-7)                     ; @BT 0E 39
    RES 6,B                      ; @BT CB B0
    BIT 0,B                      ; @BT CB 40
    BIT 6,E                      ; @BT CB 73
    SRA (HL)                     ; @BT CB 2E
    BIT 7,H                      ; @BT CB 7C
    SET 6,L                      ; @BT CB F5
    AND A,A                      ; @BT A7
    BIT 3,C                      ; @BT CB 59
    SBC A,D                      ; @BT 9A
    SET 1,D                      ; @BT CB CA
    RES 2,L                      ; @BT CB 95
    SUB B                        ; @BT 90
    SUB (((T5_FN_014(T5_DEF_014+15,15)+3)-1)-2)                      ; @BT D6 C0
    BIT 0,H                      ; @BT CB 44
    OR L                         ; @BT B5
    CALL GTE,((T5_FN_015(T5_DEF_015+16,17,16+17)+5)-5)               ; @BT D4 67 15
    RES 7,L                      ; @BT CB BD
    AND D                        ; @BT A2
    CP A,E                       ; @BT BB
    LDH ($1F),A                  ; @BT E0 1F
    BIT 1,E                      ; @BT CB 4B
    LDI A,(HL)                   ; @BT 2A
    DEC C                        ; @BT 0D
    LD L,(HL)                    ; @BT 6E
    OR A,((T5_FN_016(T5_DEF_016+17,18,17+18)-7)+7)                     ; @BT F6 36
    CALL LSS,(((T5_FN_017(T5_DEF_017+18+19,20,18,19+20)*6)/3)-($1345))               ; @BT DC 45 13
    LD B,L                       ; @BT 45
    XOR C                        ; @BT A9
    ADD A                        ; @BT 87
    LD H,C                       ; @BT 61
    AND B                        ; @BT A0
    ADC B                        ; @BT 88
    LD L,C                       ; @BT 69
    BIT 0,A                      ; @BT CB 47
    RES 5,B                      ; @BT CB A8
    LD SP,(((T5_FN_018(T5_DEF_018+19+20,21,19,20+21)*10)/5)-($2EFF))                  ; @BT 31 FF 2E
    BIT 3,(HL)                   ; @BT CB 5E
    CPL A                        ; @BT 2F
    LD (HL),C                    ; @BT 71
    RST ((T5_FN_019(T5_DEF_019+20,20)*2)-(($10)-0))                      ; @BT D7
    BIT 2,D                      ; @BT CB 52
    RR D                         ; @BT CB 1A
    ADD (HL)                     ; @BT 86
    SRA C                        ; @BT CB 29
    LD L,E                       ; @BT 6B
    XOR A                        ; @BT AF
    SUB A,A                      ; @BT 97
    RRC (HL)                     ; @BT CB 0E
    SET 7,L                      ; @BT CB FD
    JR GEQ,(((T5_DEF_020+11)-4)-7)                   ; @BT 30 3C
    ADD D                        ; @BT 82
    RLC (HL)                     ; @BT CB 06
    SET 7,(HL)                   ; @BT CB FE
    RST (((T5_DEF_021+3)-1)-2)                      ; @BT CF
    LD (HL),((T5_DEF_022+5)-5)                  ; @BT 36 C3
    DEC H                        ; @BT 25
    RES 3,H                      ; @BT CB 9C
    JP NC,((T5_DEF_023-7)+7)                  ; @BT D2 77 26
    LD L,L                       ; @BT 6D
    LD D,D                       ; @BT 52
    RES 1,D                      ; @BT CB 8A
    SLA B                        ; @BT CB 20
    SET 3,B                      ; @BT CB D8
    RES 3,(HL)                   ; @BT CB 9E
    OR A                         ; @BT B7
    ADC A,C                      ; @BT 89
    LD C,D                       ; @BT 4A
    LD D,H                       ; @BT 54
    HALT                         ; @BT 76
    RES 4,H                      ; @BT CB A4
    SLA L                        ; @BT CB 25
    SUB A,B                      ; @BT 90
    RL D                         ; @BT CB 12
    SET 6,E                      ; @BT CB F3
    BIT 7,E                      ; @BT CB 7B
    SBC (HL)                     ; @BT 9E
    SUB L                        ; @BT 95
    SBC A                        ; @BT 9F
    SET 0,A                      ; @BT CB C7
    BIT 1,B                      ; @BT CB 48
    BIT 2,B                      ; @BT CB 50
    JP Z,(((T5_DEF_024*6)/3)-($2899))                   ; @BT CA 99 28
    SUB A,L                      ; @BT 95
    CP A,(((T5_DEF_025*10)/5)-($C9))                     ; @BT FE C9
    JR Z,((T5_DEF_026*2)-(($06)-0))                     ; @BT 28 06
    RES 4,B                      ; @BT CB A0
    RL L                         ; @BT CB 15
    ADD HL,SP                    ; @BT 39
    BIT 7,D                      ; @BT CB 7A
    BIT 3,D                      ; @BT CB 5A
    BIT 4,C                      ; @BT CB 61
    SUB E                        ; @BT 93
    LD E,H                       ; @BT 5C
    INC HL                       ; @BT 23
    SET 7,H                      ; @BT CB FC
    SBC A,H                      ; @BT 9C
    RET NZ                       ; @BT C0
    LD B,B                       ; @BT 40
    RRC L                        ; @BT CB 0D
    SET 0,(HL)                   ; @BT CB C6
    XOR (HL)                     ; @BT AE
    CALL EQU,(((T5_DEF_027+11)-4)-7)               ; @BT CC 9A 18
    LD B,(((T5_DEF_028+3)-1)-2)                     ; @BT 06 22
    ADC A,E                      ; @BT 8B
    JR NC,((T5_DEF_029+5)-5)                    ; @BT 30 18
    PUSH HL                      ; @BT E5
    LD A,A                       ; @BT 7F
    LD H,E                       ; @BT 63
    SET 4,L                      ; @BT CB E5
    BIT 3,H                      ; @BT CB 5C
    LD C,H                       ; @BT 4C
    CALL GEQ,((T5_DEF_030-7)+7)               ; @BT D4 78 16
    ADC C                        ; @BT 89
    JR LST,(((T5_DEF_031*6)/3)-($0E))                   ; @BT 38 0E
    LDD (HL),A                   ; @BT 32
    STOP (((T5_DEF_032*10)/5)-($92))                     ; @BT 10 92
    ADC A,L                      ; @BT 8D
    RES 3,A                      ; @BT CB 9F
    SET 7,A                      ; @BT CB FF
    OR D                         ; @BT B2
    INC BC                       ; @BT 03
    RET LST                      ; @BT D8
    OR H                         ; @BT B4
    RRA                          ; @BT 1F
    BIT 7,A                      ; @BT CB 7F
    BIT 4,A                      ; @BT CB 67
    OR A,(HL)                    ; @BT B6
    RES 0,E                      ; @BT CB 83
    JR LSS,((T5_DEF_033*2)-(($37)-0))                   ; @BT 38 37
    RES 0,(HL)                   ; @BT CB 86
    SUB C                        ; @BT 91
    LD A,($FF00+$F4)             ; @BT F0 F4
    DEC (HL)                     ; @BT 35
    CCF                          ; @BT 3F
    RRC C                        ; @BT CB 09
    ADD E                        ; @BT 83
    CP A,(HL)                    ; @BT BE
    LD E,(((T5_DEF_034+11)-4)-7)                     ; @BT 1E 67
    RRC E                        ; @BT CB 0B
    SUB A,(((T5_DEF_035+3)-1)-2)                    ; @BT D6 A9
    DI                           ; @BT F3
    AND C                        ; @BT A1
    RET Z                        ; @BT C8
    OR (HL)                      ; @BT B6
    LD (HL),E                    ; @BT 73
    LD ($FF00+$7E),A             ; @BT E0 7E
    RR B                         ; @BT CB 18
    SRL (HL)                     ; @BT CB 3E
    RES 7,H                      ; @BT CB BC
    JP ((T5_DEF_036+5)-5)                     ; @BT C3 AA 29
    RES 4,D                      ; @BT CB A2
    RRC D                        ; @BT CB 0A
    SBC A,E                      ; @BT 9B
    RES 2,C                      ; @BT CB 91
    BIT 1,L                      ; @BT CB 4D
    SBC A,B                      ; @BT 98
    INC E                        ; @BT 1C
    CP (HL)                      ; @BT BE
    OR C                         ; @BT B1
    LD D,(HL)                    ; @BT 56
    SUB H                        ; @BT 94
    RET EQU                      ; @BT C8
    LD B,E                       ; @BT 43
    SLA D                        ; @BT CB 22
    CP A,H                       ; @BT BC
    BIT 6,D                      ; @BT CB 72
    RET NEQ                      ; @BT C0
    LD (HLI),A                   ; @BT 22
    LD (HL),L                    ; @BT 75
    LD ($FF00+C),A               ; @BT E2
    XOR A,E                      ; @BT AB
    AND A,L                      ; @BT A5
    RET GEQ                      ; @BT D0
    SBC A,C                      ; @BT 99
    SET 2,L                      ; @BT CB D5
    ADD A,C                      ; @BT 81
    INC DE                       ; @BT 13
    SET 4,B                      ; @BT CB E0
    INC B                        ; @BT 04
    STOP                         ; @BT 10
    LD (HL+),A                   ; @BT 22
    SBC C                        ; @BT 99
    DEC E                        ; @BT 1D
    DEC B                        ; @BT 05
    RES 7,(HL)                   ; @BT CB BE
    BIT 6,H                      ; @BT CB 74
    SET 0,C                      ; @BT CB C1
    XOR A,(HL)                   ; @BT AE
    CALL NZ,((T5_DEF_037-7)+7)                ; @BT C4 CD 1B
    SWAP (HL)                    ; @BT CB 36
    BIT 4,E                      ; @BT CB 63
    SET 7,B                      ; @BT CB F8
    SBC E                        ; @BT 9B
    DEC HL                       ; @BT 2B
    RES 0,A                      ; @BT CB 87
    JP (HL)                      ; @BT E9
    AND A,(((T5_DEF_038*6)/3)-($9B))                    ; @BT E6 9B
    BIT 4,L                      ; @BT CB 65
    ADC ((($3F*10)/5)-($3F))                      ; @BT CE 3F
    LD PC,HL                     ; @BT E9
    SRA A                        ; @BT CB 2F
    CP C                         ; @BT B9
    DEC L                        ; @BT 2D
    RR H                         ; @BT CB 1C
    JR EQU,(($2A*2)-(($2A)-0))                   ; @BT 28 2A
    XOR A,B                      ; @BT A8
    ADC A,D                      ; @BT 8A
    RR L                         ; @BT CB 1D
    BIT 5,A                      ; @BT CB 6F
    LDH A,(C)                    ; @BT F2
    LD B,(HL)                    ; @BT 46
    BIT 0,L                      ; @BT CB 45
    LD A,(BC)                    ; @BT 0A
    LDH A,($08)                  ; @BT F0 08
    SRA L                        ; @BT CB 2D
    AND A,B                      ; @BT A0
    BIT 3,E                      ; @BT CB 5B
    RL (HL)                      ; @BT CB 16
    SET 6,A                      ; @BT CB F7
    SBC ((($7B+11)-4)-7)                      ; @BT DE 7B
    LD B,C                       ; @BT 41
    LD BC,((($2BCC+3)-1)-2)                  ; @BT 01 CC 2B
    RST (($38+5)-5)                      ; @BT FF
    SET 4,D                      ; @BT CB E2
    SET 5,E                      ; @BT CB EB
    RES 4,A                      ; @BT CB A7
    LD (HL),H                    ; @BT 74
    LD E,B                       ; @BT 58
    ADD SP,(($2D-7)+7)                   ; @BT E8 2D
    RST ((($18*6)/3)-($18))                      ; @BT DF
    CP A,D                       ; @BT BA
    RES 1,H                      ; @BT CB 8C
    BIT 6,A                      ; @BT CB 77
    LD SP,HL                     ; @BT F9
    RES 6,D                      ; @BT CB B2
    OR A,H                       ; @BT B4
    LD (HLD),A                   ; @BT 32
    RES 0,H                      ; @BT CB 84
    JP EQU,((($2455*10)/5)-($2455))                 ; @BT CA 55 24
    AND H                        ; @BT A4
    RLC A                        ; @BT CB 07
    RES 4,L                      ; @BT CB A5
    LD (HL),B                    ; @BT 70
    RES 6,E                      ; @BT CB B3
    RES 5,H                      ; @BT CB AC
    SRL C                        ; @BT CB 39
    BIT 6,L                      ; @BT CB 75
    POP AF                       ; @BT F1
    XOR A,(($D7*2)-(($D7)-0))                    ; @BT EE D7
    ADD H                        ; @BT 84
    CALL Z,((($1CDE+11)-4)-7)                 ; @BT CC DE 1C
    LD D,C                       ; @BT 51
    JR ((($1D+3)-1)-2)                       ; @BT 18 1D
    OR A,D                       ; @BT B2
    RRC H                        ; @BT CB 0C
    SWAP B                       ; @BT CB 30
    RL C                         ; @BT CB 11
    RES 4,(HL)                   ; @BT CB A6
    RES 6,L                      ; @BT CB B5
    OR A,A                       ; @BT B7
    LD C,E                       ; @BT 4B
    SUB A,C                      ; @BT 91
    DEC SP                       ; @BT 3B
    LD C,(HL)                    ; @BT 4E
    RES 5,D                      ; @BT CB AA
    JP GTE,(($2122+5)-5)                 ; @BT D2 22 21
    JP LST,(($2011-7)+7)                 ; @BT DA 11 20
    ADD HL,DE                    ; @BT 19
    RR (HL)                      ; @BT CB 1E
    LD A,E                       ; @BT 7B
    CALL LST,((($1456*6)/3)-($1456))               ; @BT DC 56 14
    RES 0,C                      ; @BT CB 81
    RES 1,(HL)                   ; @BT CB 8E
    CP E                         ; @BT BB
    BIT 6,(HL)                   ; @BT CB 76
    SET 2,E                      ; @BT CB D3
    SET 1,H                      ; @BT CB CC
    RES 2,H                      ; @BT CB 94
    SRL D                        ; @BT CB 3A
    CALL NC,((($1ABC*10)/5)-($1ABC))                ; @BT D4 BC 1A
    JR GTE,(($25*2)-(($25)-0))                   ; @BT 30 25
    LD A,(HL+)                   ; @BT 2A
    LD (((($3121+11)-4)-7)),A                 ; @BT EA 21 31
    POP HL                       ; @BT E1
    DAA                          ; @BT 27
    SET 7,C                      ; @BT CB F9
    SET 5,D                      ; @BT CB EA
    RES 1,C                      ; @BT CB 89
    BIT 1,D                      ; @BT CB 4A
    ADC A,A                      ; @BT 8F
    AND A,E                      ; @BT A3
    AND A,H                      ; @BT A4
    LD A,($FF00+C)               ; @BT F2
    BIT 3,A                      ; @BT CB 5F
    RES 5,E                      ; @BT CB AB
    SET 6,(HL)                   ; @BT CB F6
    SET 4,C                      ; @BT CB E1
    ADD ((($84+3)-1)-2)                      ; @BT C6 84
    SWAP H                       ; @BT CB 34
    LD A,B                       ; @BT 78
    SUB D                        ; @BT 92
    SBC A,A                      ; @BT 9F
    LD A,C                       ; @BT 79
    SET 0,D                      ; @BT CB C2
    SET 3,(HL)                   ; @BT CB DE
    RES 0,B                      ; @BT CB 80
    ADD C                        ; @BT 81
    INC H                        ; @BT 24
    RES 6,A                      ; @BT CB B7
    BIT 2,E                      ; @BT CB 53
    SET 1,B                      ; @BT CB C8
    LD H,D                       ; @BT 62
    RES 1,E                      ; @BT CB 8B
    BIT 4,B                      ; @BT CB 60
    SET 7,E                      ; @BT CB FB
    LD C,C                       ; @BT 49
    OR B                         ; @BT B0
    BIT 4,H                      ; @BT CB 64
    LD C,B                       ; @BT 48
    ADC A,(HL)                   ; @BT 8E
    SET 3,E                      ; @BT CB DB
    SUB A,D                      ; @BT 92
    BIT 4,(HL)                   ; @BT CB 66
    JR NZ,(($2F+5)-5)                    ; @BT 20 2F
    DEC A                        ; @BT 3D
    LD (BC),A                    ; @BT 02
    NOP                          ; @BT 00
    RR A                         ; @BT CB 1F
    SBC A,L                      ; @BT 9D
    SET 6,C                      ; @BT CB F1
    CP L                         ; @BT BD
    BIT 5,D                      ; @BT CB 6A
    RRC B                        ; @BT CB 08
    RES 4,E                      ; @BT CB A3
    OR (($4D-7)+7)                       ; @BT F6 4D
    OR E                         ; @BT B3
    RLC C                        ; @BT CB 01
    RES 3,D                      ; @BT CB 9A
    ADD A,(HL)                   ; @BT 86
    DEC D                        ; @BT 15
    BIT 2,C                      ; @BT CB 51
    AND ((($B2*6)/3)-($B2))                      ; @BT E6 B2
    JP NEQ,((($2344*10)/5)-($2344))                 ; @BT C2 44 23
    RES 1,L                      ; @BT CB 8D
    LD A,D                       ; @BT 7A
    BIT 1,H                      ; @BT CB 4C
    BIT 2,H                      ; @BT CB 54
    RST (($30*2)-(($30)-0))                      ; @BT F7
    OR A,E                       ; @BT B3
    LD L,H                       ; @BT 6C
    SET 0,B                      ; @BT CB C0
    BIT 3,B                      ; @BT CB 58
    XOR ((($EE+11)-4)-7)                      ; @BT EE EE
    SET 5,L                      ; @BT CB ED
    RET GTE                      ; @BT D0
    SLA (HL)                     ; @BT CB 26
    SUB A,E                      ; @BT 93
    SRA H                        ; @BT CB 2C
    BIT 5,(HL)                   ; @BT CB 6E
    RST ((($00+3)-1)-2)                      ; @BT C7
    LDI (HL),A                   ; @BT 22
    LD A,(DE)                    ; @BT 1A
    LD E,E                       ; @BT 5B
    SET 2,B                      ; @BT CB D0
    LD C,A                       ; @BT 4F
    BIT 7,B                      ; @BT CB 78
    LD H,(HL)                    ; @BT 66
    DEC BC                       ; @BT 0B
    ADD B                        ; @BT 80
    ADC A,H                      ; @BT 8C
    AND A,(HL)                   ; @BT A6
    LD E,C                       ; @BT 59
    SET 5,B                      ; @BT CB E8
    LD A,H                       ; @BT 7C
    LD DE,(($2CDD+5)-5)                  ; @BT 11 DD 2C
    XOR A,D                      ; @BT AA
    CP (($E0-7)+7)                       ; @BT FE E0
    OR A,B                       ; @BT B0
    JP GEQ,((($2233*6)/3)-($2233))                 ; @BT D2 33 22
    RES 7,E                      ; @BT CB BB
    CP A                         ; @BT BF
    LD B,D                       ; @BT 42
    ADD A,B                      ; @BT 80
    RL E                         ; @BT CB 13
    LD HL,((($2DEE*10)/5)-($2DEE))                  ; @BT 21 EE 2D
    RES 0,D                      ; @BT CB 82
    XOR A,H                      ; @BT AC
    OR A,L                       ; @BT B5
    RLC B                        ; @BT CB 00
    AND E                        ; @BT A3
    SBC B                        ; @BT 98
    RES 4,C                      ; @BT CB A1
    BIT 5,C                      ; @BT CB 69
    LD H,B                       ; @BT 60
    LD A,(HL-)                   ; @BT 3A
    LD (HL),D                    ; @BT 72
    POP DE                       ; @BT D1
    LD E,A                       ; @BT 5F
    BIT 0,(HL)                   ; @BT CB 46
    CP A,B                       ; @BT B8
    RES 1,A                      ; @BT CB 8F
    JR NEQ,(($13*2)-(($13)-0))                   ; @BT 20 13
    RES 7,D                      ; @BT CB BA
    LD HL,SP+((($15+11)-4)-7)                 ; @BT F8 15
    AND A,C                      ; @BT A1
    BIT 5,H                      ; @BT CB 6C
    RLC H                        ; @BT CB 04
    BIT 7,C                      ; @BT CB 79
    LD HL,SP-((($2C+3)-1)-2)                 ; @BT F8 D4
    INC (HL)                     ; @BT 34
    RL B                         ; @BT CB 10
    SET 6,D                      ; @BT CB F2
    RET NC                       ; @BT D0
    LD (HL-),A                   ; @BT 32
    RES 5,L                      ; @BT CB AD
    LD C,L                       ; @BT 4D
    SBC A,(($64+5)-5)                    ; @BT DE 64
    SET 5,A                      ; @BT CB EF
    INC L                        ; @BT 2C
    SRA B                        ; @BT CB 28
    SET 5,C                      ; @BT CB E9
    PUSH DE                      ; @BT D5
    SET 6,H                      ; @BT CB F4
    PUSH BC                      ; @BT C5
    LD E,D                       ; @BT 5A
    LD L,D                       ; @BT 6A
    RST (($20-7)+7)                      ; @BT E7
    LD D,L                       ; @BT 55
    CALL ((($1DEF*6)/3)-($1DEF))                   ; @BT CD EF 1D
    POP BC                       ; @BT C1
    OR A,C                       ; @BT B1
    SET 5,(HL)                   ; @BT CB EE
    LD L,((($F1*10)/5)-($F1))                     ; @BT 2E F1
    RET C                        ; @BT D8
    RES 3,B                      ; @BT CB 98
    SET 0,E                      ; @BT CB C3
    RRCA                         ; @BT 0F
    RES 5,(HL)                   ; @BT CB AE
    SBC L                        ; @BT 9D
    SET 5,H                      ; @BT CB EC
    LD ((($3010*2)-(($3010)-0))),SP                ; @BT 08 10 30
    BIT 2,L                      ; @BT CB 55
    SET 4,E                      ; @BT CB E3
    BIT 0,D                      ; @BT CB 42
    LDD A,(HL)                   ; @BT 3A
    CALL C,((($19AB+11)-4)-7)                 ; @BT DC AB 19
    RL H                         ; @BT CB 14
    RES 3,E                      ; @BT CB 9B

    .db "<05"               ; @BT END
