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

    ; NOTE: huc6280 opcodes, as WLA DX understands them, can be found in /ihuc6280.c
    ; Please verify using external documentation (e.g. https://www.masswerk.at/huc6280/huc6280_instruction_set.html)
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
    ; Auto-generated from ihuc6280.c + cross-checked with external huc6280 ISA reference

    ADC #$28                       ; @BT 69 28
    ADC ($3F,X)                    ; @BT 61 3F
    ADC ($56),Y                    ; @BT 71 56
    ADC ($6D)                      ; @BT 72 6D
    ADC $84,X                      ; @BT 75 84
    ADC $1345,X                    ; @BT 7D 45 13
    ADC $1456,Y                    ; @BT 79 56 14
    ADC $9B                        ; @BT 65 9B
    ADC $1567                      ; @BT 6D 67 15
    ADC.B #$B2                     ; @BT 69 B2
    ADC.B ($C9,X)                  ; @BT 61 C9
    ADC.B ($E0),Y                  ; @BT 71 E0
    ADC.B ($F7)                    ; @BT 72 F7
    ADC.B $0E,X                    ; @BT 75 0E
    ADC.W $1678,X                  ; @BT 7D 78 16
    ADC.W $1789,Y                  ; @BT 79 89 17
    ADC.B $25                      ; @BT 65 25
    ADC.W $189A                    ; @BT 6D 9A 18
    AND #$3C                       ; @BT 29 3C
    AND ($53,X)                    ; @BT 21 53
    AND ($6A),Y                    ; @BT 31 6A
    AND ($81)                      ; @BT 32 81
    AND $98,X                      ; @BT 35 98
    AND $19AB,X                    ; @BT 3D AB 19
    AND $1ABC,Y                    ; @BT 39 BC 1A
    AND $AF                        ; @BT 25 AF
    AND $1BCD                      ; @BT 2D CD 1B
    AND.B #$C6                     ; @BT 29 C6
    AND.B ($DD,X)                  ; @BT 21 DD
    AND.B ($F4),Y                  ; @BT 31 F4
    AND.B ($0B)                    ; @BT 32 0B
    AND.B $22,X                    ; @BT 35 22
    AND.W $1CDE,X                  ; @BT 3D DE 1C
    AND.W $1DEF,Y                  ; @BT 39 EF 1D
    AND.B $39                      ; @BT 25 39
    AND.W $1F00                    ; @BT 2D 00 1F
    ASL A                          ; @BT 0A
    ASL $50,X                      ; @BT 16 50
    ASL $2011,X                    ; @BT 1E 11 20
    ASL $67                        ; @BT 06 67
    ASL $2122                      ; @BT 0E 22 21
    ASL.B $7E,X                    ; @BT 16 7E
    ASL.W $2233,X                  ; @BT 1E 33 22
    ASL.B $95                      ; @BT 06 95
    ASL.W $2344                    ; @BT 0E 44 23
    ASL                            ; @BT 0A
    BBR0 $AC,$43                   ; @BT 0F AC 43
    BBR1 $C3,$5A                   ; @BT 1F C3 5A
    BBR2 $DA,$71                   ; @BT 2F DA 71
    BBR3 $F1,$08                   ; @BT 3F F1 08
    BBR4 $08,$1F                   ; @BT 4F 08 1F
    BBR5 $1F,$36                   ; @BT 5F 1F 36
    BBR6 $36,$4D                   ; @BT 6F 36 4D
    BBR7 $4D,$64                   ; @BT 7F 4D 64
    BBS0 $64,$7B                   ; @BT 8F 64 7B
    BBS1 $7B,$12                   ; @BT 9F 7B 12
    BBS2 $92,$29                   ; @BT AF 92 29
    BBS3 $A9,$40                   ; @BT BF A9 40
    BBS4 $C0,$57                   ; @BT CF C0 57
    BBS5 $D7,$6E                   ; @BT DF D7 6E
    BBS6 $EE,$05                   ; @BT EF EE 05
    BBS7 $05,$1C                   ; @BT FF 05 1C
    BCC $1C                        ; @BT 90 1C
    BCS $33                        ; @BT B0 33
    BEQ $4A                        ; @BT F0 4A
    BMI $61                        ; @BT 30 61
    BNE $78                        ; @BT D0 78
    BPL $0F                        ; @BT 10 0F
    BVC $26                        ; @BT 50 26
    BVS $3D                        ; @BT 70 3D
    BSR $54                        ; @BT 44 54
    BRA $6B                        ; @BT 80 6B
    BCC.B $02                      ; @BT 90 02
    BCS.B $19                      ; @BT B0 19
    BEQ.B $30                      ; @BT F0 30
    BMI.B $47                      ; @BT 30 47
    BNE.B $5E                      ; @BT D0 5E
    BPL.B $75                      ; @BT 10 75
    BVC.B $0C                      ; @BT 50 0C
    BVS.B $23                      ; @BT 70 23
    BSR.B $3A                      ; @BT 44 3A
    BRA.B $51                      ; @BT 80 51
    BIT #$E8                       ; @BT 89 E8
    BIT $FF,X                      ; @BT 34 FF
    BIT $2455,X                    ; @BT 3C 55 24
    BIT $16                        ; @BT 24 16
    BIT $2566                      ; @BT 2C 66 25
    BIT.B $2D,X                    ; @BT 34 2D
    BIT.W $2677,X                  ; @BT 3C 77 26
    BIT.B $44                      ; @BT 24 44
    BIT.W $2788                    ; @BT 2C 88 27
    BRK $5B                        ; @BT 00 5B
    BRK.B $72                      ; @BT 00 72
    BRK                            ; @BT 00 00
    CLC                            ; @BT 18
    CLD                            ; @BT D8
    CLI                            ; @BT 58
    CLV                            ; @BT B8
    CSL                            ; @BT 54
    CLA                            ; @BT 62
    CLX                            ; @BT 82
    CLY                            ; @BT C2
    CSH                            ; @BT D4
    CMP #$89                       ; @BT C9 89
    CMP ($A0,X)                    ; @BT C1 A0
    CMP ($B7),Y                    ; @BT D1 B7
    CMP ($CE)                      ; @BT D2 CE
    CMP $E5,X                      ; @BT D5 E5
    CMP $2899,X                    ; @BT DD 99 28
    CMP $29AA,Y                    ; @BT D9 AA 29
    CMP $FC                        ; @BT C5 FC
    CMP $2ABB                      ; @BT CD BB 2A
    CMP.B #$13                     ; @BT C9 13
    CMP.B ($2A,X)                  ; @BT C1 2A
    CMP.B ($41),Y                  ; @BT D1 41
    CMP.B ($58)                    ; @BT D2 58
    CMP.B $6F,X                    ; @BT D5 6F
    CMP.W $2BCC,X                  ; @BT DD CC 2B
    CMP.W $2CDD,Y                  ; @BT D9 DD 2C
    CMP.B $86                      ; @BT C5 86
    CMP.W $2DEE                    ; @BT CD EE 2D
    CPX #$9D                       ; @BT E0 9D
    CPX $B4                        ; @BT E4 B4
    CPX $2EFF                      ; @BT EC FF 2E
    CPX.B #$CB                     ; @BT E0 CB
    CPX.B $E2                      ; @BT E4 E2
    CPX.W $3010                    ; @BT EC 10 30
    CPY #$F9                       ; @BT C0 F9
    CPY $10                        ; @BT C4 10
    CPY $3121                      ; @BT CC 21 31
    CPY.B #$27                     ; @BT C0 27
    CPY.B $3E                      ; @BT C4 3E
    CPY.W $3232                    ; @BT CC 32 32
    DEC $55,X                      ; @BT D6 55
    DEC $3343,X                    ; @BT DE 43 33
    DEC $6C                        ; @BT C6 6C
    DEC $3454                      ; @BT CE 54 34
    DEA                            ; @BT 3A
    DEX                            ; @BT CA
    DEY                            ; @BT 88
    DEC.B $83,X                    ; @BT D6 83
    DEC.W $3565,X                  ; @BT DE 65 35
    DEC.B $9A                      ; @BT C6 9A
    DEC.W $3676                    ; @BT CE 76 36
    EOR #$B1                       ; @BT 49 B1
    EOR ($C8,X)                    ; @BT 41 C8
    EOR ($DF),Y                    ; @BT 51 DF
    EOR ($F6)                      ; @BT 52 F6
    EOR $0D,X                      ; @BT 55 0D
    EOR $3787,X                    ; @BT 5D 87 37
    EOR $3898,Y                    ; @BT 59 98 38
    EOR $24                        ; @BT 45 24
    EOR $39A9                      ; @BT 4D A9 39
    EOR.B #$3B                     ; @BT 49 3B
    EOR.B ($52,X)                  ; @BT 41 52
    EOR.B ($69),Y                  ; @BT 51 69
    EOR.B ($80)                    ; @BT 52 80
    EOR.B $97,X                    ; @BT 55 97
    EOR.W $3ABA,X                  ; @BT 5D BA 3A
    EOR.W $3BCB,Y                  ; @BT 59 CB 3B
    EOR.B $AE                      ; @BT 45 AE
    EOR.W $3CDC                    ; @BT 4D DC 3C
    INC $C5,X                      ; @BT F6 C5
    INC $3DED,X                    ; @BT FE ED 3D
    INC $DC                        ; @BT E6 DC
    INC $3EFE                      ; @BT EE FE 3E
    INA                            ; @BT 1A
    INX                            ; @BT E8
    INY                            ; @BT C8
    INC.B $F3,X                    ; @BT F6 F3
    INC.W $400F,X                  ; @BT FE 0F 40
    INC.B $0A                      ; @BT E6 0A
    INC.W $4120                    ; @BT EE 20 41
    JMP ($4231,X)                  ; @BT 7C 31 42
    JMP ($4342)                    ; @BT 6C 42 43
    JMP $4453                      ; @BT 4C 53 44
    JSR $4564                      ; @BT 20 64 45
    JMP.W ($4675,X)                ; @BT 7C 75 46
    JMP.W ($4786)                  ; @BT 6C 86 47
    JMP.W $4897                    ; @BT 4C 97 48
    JSR.W $49A8                    ; @BT 20 A8 49
    LDA #$21                       ; @BT A9 21
    LDA ($38,X)                    ; @BT A1 38
    LDA ($4F),Y                    ; @BT B1 4F
    LDA ($66)                      ; @BT B2 66
    LDA $7D,X                      ; @BT B5 7D
    LDA $4AB9,X                    ; @BT BD B9 4A
    LDA $4BCA,Y                    ; @BT B9 CA 4B
    LDA $94                        ; @BT A5 94
    LDA $4CDB                      ; @BT AD DB 4C
    LDA.B #$AB                     ; @BT A9 AB
    LDA.B ($C2,X)                  ; @BT A1 C2
    LDA.B ($D9),Y                  ; @BT B1 D9
    LDA.B ($F0)                    ; @BT B2 F0
    LDA.B $07,X                    ; @BT B5 07
    LDA.W $4DEC,X                  ; @BT BD EC 4D
    LDA.W $4EFD,Y                  ; @BT B9 FD 4E
    LDA.B $1E                      ; @BT A5 1E
    LDA.W $500E                    ; @BT AD 0E 50
    LDX #$35                       ; @BT A2 35
    LDX $4C,Y                      ; @BT B6 4C
    LDX $511F,Y                    ; @BT BE 1F 51
    LDX $63                        ; @BT A6 63
    LDX $5230                      ; @BT AE 30 52
    LDX.B #$7A                     ; @BT A2 7A
    LDX.B $91,Y                    ; @BT B6 91
    LDX.W $5341,Y                  ; @BT BE 41 53
    LDX.B $A8                      ; @BT A6 A8
    LDX.W $5452                    ; @BT AE 52 54
    LDY #$BF                       ; @BT A0 BF
    LDY $D6,X                      ; @BT B4 D6
    LDY $5563,X                    ; @BT BC 63 55
    LDY $ED                        ; @BT A4 ED
    LDY $5674                      ; @BT AC 74 56
    LDY.B #$04                     ; @BT A0 04
    LDY.B $1B,X                    ; @BT B4 1B
    LDY.W $5785,X                  ; @BT BC 85 57
    LDY.B $32                      ; @BT A4 32
    LDY.W $5896                    ; @BT AC 96 58
    LSR A                          ; @BT 4A
    LSR $49,X                      ; @BT 56 49
    LSR $59A7,X                    ; @BT 5E A7 59
    LSR $60                        ; @BT 46 60
    LSR $5AB8                      ; @BT 4E B8 5A
    LSR.B $77,X                    ; @BT 56 77
    LSR.W $5BC9,X                  ; @BT 5E C9 5B
    LSR.B $8E                      ; @BT 46 8E
    LSR.W $5CDA                    ; @BT 4E DA 5C
    LSR                            ; @BT 4A
    NOP                            ; @BT EA
    ORA #$A5                       ; @BT 09 A5
    ORA ($BC,X)                    ; @BT 01 BC
    ORA ($D3),Y                    ; @BT 11 D3
    ORA ($EA)                      ; @BT 12 EA
    ORA $01,X                      ; @BT 15 01
    ORA $5DEB,X                    ; @BT 1D EB 5D
    ORA $5EFC,Y                    ; @BT 19 FC 5E
    ORA $18                        ; @BT 05 18
    ORA $600D                      ; @BT 0D 0D 60
    ORA.B #$2F                     ; @BT 09 2F
    ORA.B ($46,X)                  ; @BT 01 46
    ORA.B ($5D),Y                  ; @BT 11 5D
    ORA.B ($74)                    ; @BT 12 74
    ORA.B $8B,X                    ; @BT 15 8B
    ORA.W $611E,X                  ; @BT 1D 1E 61
    ORA.W $622F,Y                  ; @BT 19 2F 62
    ORA.B $A2                      ; @BT 05 A2
    ORA.W $6340                    ; @BT 0D 40 63
    PHA                            ; @BT 48
    PHP                            ; @BT 08
    PHX                            ; @BT DA
    PHY                            ; @BT 5A
    PLA                            ; @BT 68
    PLP                            ; @BT 28
    PLX                            ; @BT FA
    PLY                            ; @BT 7A
    ROL A                          ; @BT 2A
    ROL $B9,X                      ; @BT 36 B9
    ROL $6451,X                    ; @BT 3E 51 64
    ROL $D0                        ; @BT 26 D0
    ROL $6562                      ; @BT 2E 62 65
    ROL.B $E7,X                    ; @BT 36 E7
    ROL.W $6673,X                  ; @BT 3E 73 66
    ROL.B $FE                      ; @BT 26 FE
    ROL.W $6784                    ; @BT 2E 84 67
    ROL                            ; @BT 2A
    ROR A                          ; @BT 6A
    ROR $15,X                      ; @BT 76 15
    ROR $6895,X                    ; @BT 7E 95 68
    ROR $2C                        ; @BT 66 2C
    ROR $69A6                      ; @BT 6E A6 69
    ROR.B $43,X                    ; @BT 76 43
    ROR.W $6AB7,X                  ; @BT 7E B7 6A
    ROR.B $5A                      ; @BT 66 5A
    ROR.W $6BC8                    ; @BT 6E C8 6B
    ROR                            ; @BT 6A
    RMB0 $71                       ; @BT 07 71
    RMB1 $88                       ; @BT 17 88
    RMB2 $9F                       ; @BT 27 9F
    RMB3 $B6                       ; @BT 37 B6
    RMB4 $CD                       ; @BT 47 CD
    RMB5 $E4                       ; @BT 57 E4
    RMB6 $FB                       ; @BT 67 FB
    RMB7 $12                       ; @BT 77 12
    RTI                            ; @BT 40
    RTS                            ; @BT 60
    SMB0 $29                       ; @BT 87 29
    SMB1 $40                       ; @BT 97 40
    SMB2 $57                       ; @BT A7 57
    SMB3 $6E                       ; @BT B7 6E
    SMB4 $85                       ; @BT C7 85
    SMB5 $9C                       ; @BT D7 9C
    SMB6 $B3                       ; @BT E7 B3
    SMB7 $CA                       ; @BT F7 CA
    SBC #$E1                       ; @BT E9 E1
    SBC ($F8,X)                    ; @BT E1 F8
    SBC ($0F),Y                    ; @BT F1 0F
    SBC ($26)                      ; @BT F2 26
    SBC $3D,X                      ; @BT F5 3D
    SBC $6CD9,X                    ; @BT FD D9 6C
    SBC $6DEA,Y                    ; @BT F9 EA 6D
    SBC $54                        ; @BT E5 54
    SBC $6EFB                      ; @BT ED FB 6E
    SBC.B #$6B                     ; @BT E9 6B
    SBC.B ($82,X)                  ; @BT E1 82
    SBC.B ($99),Y                  ; @BT F1 99
    SBC.B ($B0)                    ; @BT F2 B0
    SBC.B $C7,X                    ; @BT F5 C7
    SBC.W $700C,X                  ; @BT FD 0C 70
    SBC.W $711D,Y                  ; @BT F9 1D 71
    SBC.B $DE                      ; @BT E5 DE
    SBC.W $722E                    ; @BT ED 2E 72
    SEC                            ; @BT 38
    SED                            ; @BT F8
    SEI                            ; @BT 78
    SET                            ; @BT F4
    STA ($F5,X)                    ; @BT 81 F5
    STA ($0C),Y                    ; @BT 91 0C
    STA ($23)                      ; @BT 92 23
    STA $3A,X                      ; @BT 95 3A
    STA $733F,X                    ; @BT 9D 3F 73
    STA $7450,Y                    ; @BT 99 50 74
    STA $51                        ; @BT 85 51
    STA $7561                      ; @BT 8D 61 75
    STA.B ($68,X)                  ; @BT 81 68
    STA.B ($7F),Y                  ; @BT 91 7F
    STA.B ($96)                    ; @BT 92 96
    STA.B $AD,X                    ; @BT 95 AD
    STA.W $7672,X                  ; @BT 9D 72 76
    STA.W $7783,Y                  ; @BT 99 83 77
    STA.B $C4                      ; @BT 85 C4
    STA.W $7894                    ; @BT 8D 94 78
    STX $DB,Y                      ; @BT 96 DB
    STX $F2                        ; @BT 86 F2
    STX $79A5                      ; @BT 8E A5 79
    STX.B $09,Y                    ; @BT 96 09
    STX.B $20                      ; @BT 86 20
    STX.W $7AB6                    ; @BT 8E B6 7A
    STY $37,X                      ; @BT 94 37
    STY $4E                        ; @BT 84 4E
    STY $7BC7                      ; @BT 8C C7 7B
    STY.B $65,X                    ; @BT 94 65
    STY.B $7C                      ; @BT 84 7C
    STY.W $7CD8                    ; @BT 8C D8 7C
    STZ $93,X                      ; @BT 74 93
    STZ $7DE9,X                    ; @BT 9E E9 7D
    STZ $AA                        ; @BT 64 AA
    STZ $7EFA                      ; @BT 9C FA 7E
    STZ.B $C1,X                    ; @BT 74 C1
    STZ $800B,X                    ; @BT 9E 0B 80
    STZ.B $D8                      ; @BT 64 D8
    STZ.W $811C                    ; @BT 9C 1C 81
    SXY                            ; @BT 02
    SAX                            ; @BT 22
    SAY                            ; @BT 42
    ST0 #$EF                       ; @BT 03 EF
    ST1 #$06                       ; @BT 13 06
    ST2 #$1D                       ; @BT 23 1D
    ST0.B #$34                     ; @BT 03 34
    ST1.B #$4B                     ; @BT 13 4B
    ST2.B #$62                     ; @BT 23 62
    TMA #$79                       ; @BT 43 79
    TAM #$90                       ; @BT 53 90
    TMA.B #$A7                     ; @BT 43 A7
    TAM.B #$BE                     ; @BT 53 BE
    TAX                            ; @BT AA
    TAY                            ; @BT A8
    TSX                            ; @BT BA
    TXA                            ; @BT 8A
    TXS                            ; @BT 9A
    TYA                            ; @BT 98
    TRB $D5                        ; @BT 14 D5
    TRB $822D                      ; @BT 1C 2D 82
    TRB.B $EC                      ; @BT 14 EC
    TRB.W $833E                    ; @BT 1C 3E 83
    TSB $03                        ; @BT 04 03
    TSB $844F                      ; @BT 0C 4F 84
    TSB.B $1A                      ; @BT 04 1A
    TSB.W $8560                    ; @BT 0C 60 85
    TII $8671,$8782,$8893          ; @BT 73 71 86 82 87 93 88
    TDD $8782,$8893,$89A4          ; @BT C3 82 87 93 88 A4 89
    TIN $8893,$89A4,$8AB5          ; @BT D3 93 88 A4 89 B5 8A
    TIA $89A4,$8AB5,$8BC6          ; @BT E3 A4 89 B5 8A C6 8B
    TAI $8AB5,$8BC6,$8CD7          ; @BT F3 B5 8A C6 8B D7 8C
    TST #$31,$52,X                 ; @BT A3 31 52
    TST #$48,$8BC6,X               ; @BT B3 48 C6 8B
    TST #$5F,$80                   ; @BT 83 5F 80
    TST #$76,$8CD7                 ; @BT 93 76 D7 8C
    .db "<01"               ; @BT END

    /////////////////////////////////////////////////////////////////////////////
    // TEST-02: same opcodes as TEST-01, randomised order, branches spread out
    /////////////////////////////////////////////////////////////////////////////

    .db "02>"               ; @BT TEST-02 02 START

    ; GENERATED: copied from TEST-01 with full TEST-02 randomization
    ; GENERATED: seed = 35714263

    SAY                            ; @BT 42
    STY $4E                        ; @BT 84 4E
    TST #$5F,$80                   ; @BT 83 5F 80
    DEY                            ; @BT 88
    RMB5 $E4                       ; @BT 57 E4
    LSR.W $5BC9,X                  ; @BT 5E C9 5B
    LSR.B $77,X                    ; @BT 56 77
    TYA                            ; @BT 98
    BRA.B $51                      ; @BT 80 51
    CMP #$89                       ; @BT C9 89
    BVS $3D                        ; @BT 70 3D
    BIT.W $2677,X                  ; @BT 3C 77 26
    BBS6 $EE,$05                   ; @BT EF EE 05
    CMP $29AA,Y                    ; @BT D9 AA 29
    CLY                            ; @BT C2
    STY $7BC7                      ; @BT 8C C7 7B
    CPY.W $3232                    ; @BT CC 32 32
    AND.B #$C6                     ; @BT 29 C6
    STA.W $7672,X                  ; @BT 9D 72 76
    ORA ($BC,X)                    ; @BT 01 BC
    DEC.W $3676                    ; @BT CE 76 36
    ADC $84,X                      ; @BT 75 84
    AND.B $39                      ; @BT 25 39
    TSB $03                        ; @BT 04 03
    LDA.B $1E                      ; @BT A5 1E
    SBC $54                        ; @BT E5 54
    SBC.W $711D,Y                  ; @BT F9 1D 71
    SBC.W $700C,X                  ; @BT FD 0C 70
    JMP.W $4897                    ; @BT 4C 97 48
    AND $1BCD                      ; @BT 2D CD 1B
    ROR $2C                        ; @BT 66 2C
    LDY.W $5785,X                  ; @BT BC 85 57
    BRK                            ; @BT 00 00
    ROR                            ; @BT 6A
    EOR.B ($69),Y                  ; @BT 51 69
    AND.B $22,X                    ; @BT 35 22
    SXY                            ; @BT 02
    ROL $D0                        ; @BT 26 D0
    BVS.B $23                      ; @BT 70 23
    ORA.B ($5D),Y                  ; @BT 11 5D
    EOR $3787,X                    ; @BT 5D 87 37
    AND $19AB,X                    ; @BT 3D AB 19
    LDA $7D,X                      ; @BT B5 7D
    BBS2 $92,$29                   ; @BT AF 92 29
    BBR1 $C3,$5A                   ; @BT 1F C3 5A
    SMB2 $57                       ; @BT A7 57
    LDY $D6,X                      ; @BT B4 D6
    ADC $1567                      ; @BT 6D 67 15
    STA.B $AD,X                    ; @BT 95 AD
    ASL $2122                      ; @BT 0E 22 21
    BNE.B $5E                      ; @BT D0 5E
    BVC.B $0C                      ; @BT 50 0C
    CPY.B $3E                      ; @BT C4 3E
    AND $1ABC,Y                    ; @BT 39 BC 1A
    ADC.W $189A                    ; @BT 6D 9A 18
    CSH                            ; @BT D4
    SBC.B #$6B                     ; @BT E9 6B
    STA ($23)                      ; @BT 92 23
    ADC.B #$B2                     ; @BT 69 B2
    BRA $6B                        ; @BT 80 6B
    ADC.B $0E,X                    ; @BT 75 0E
    LDA $94                        ; @BT A5 94
    AND $AF                        ; @BT 25 AF
    LDX.B $91,Y                    ; @BT B6 91
    SBC #$E1                       ; @BT E9 E1
    LSR $59A7,X                    ; @BT 5E A7 59
    STX $79A5                      ; @BT 8E A5 79
    LDY $5674                      ; @BT AC 74 56
    LDA.B $07,X                    ; @BT B5 07
    CPY.B #$27                     ; @BT C0 27
    ASL A                          ; @BT 0A
    BIT.B $2D,X                    ; @BT 34 2D
    DEC.W $3565,X                  ; @BT DE 65 35
    ROL $B9,X                      ; @BT 36 B9
    BIT $16                        ; @BT 24 16
    EOR.W $3CDC                    ; @BT 4D DC 3C
    ORA.W $6340                    ; @BT 0D 40 63
    INC $C5,X                      ; @BT F6 C5
    TMA.B #$A7                     ; @BT 43 A7
    STA $3A,X                      ; @BT 95 3A
    STZ $93,X                      ; @BT 74 93
    STA.B ($68,X)                  ; @BT 81 68
    EOR.W $3BCB,Y                  ; @BT 59 CB 3B
    ORA $5EFC,Y                    ; @BT 19 FC 5E
    ORA.B ($74)                    ; @BT 12 74
    BCC $1C                        ; @BT 90 1C
    INA                            ; @BT 1A
    SBC $6DEA,Y                    ; @BT F9 EA 6D
    BBS1 $7B,$12                   ; @BT 9F 7B 12
    BBS3 $A9,$40                   ; @BT BF A9 40
    BIT #$E8                       ; @BT 89 E8
    EOR #$B1                       ; @BT 49 B1
    CMP.B $6F,X                    ; @BT D5 6F
    AND.W $1F00                    ; @BT 2D 00 1F
    CPY $10                        ; @BT C4 10
    PHX                            ; @BT DA
    BBR4 $08,$1F                   ; @BT 4F 08 1F
    STA.W $7894                    ; @BT 8D 94 78
    ADC.B $25                      ; @BT 65 25
    STX $DB,Y                      ; @BT 96 DB
    STA.W $7783,Y                  ; @BT 99 83 77
    DEX                            ; @BT CA
    ADC.W $1678,X                  ; @BT 7D 78 16
    AND.W $1DEF,Y                  ; @BT 39 EF 1D
    SBC.B ($99),Y                  ; @BT F1 99
    ADC ($3F,X)                    ; @BT 61 3F
    CMP $E5,X                      ; @BT D5 E5
    ST1 #$06                       ; @BT 13 06
    TSB.B $1A                      ; @BT 04 1A
    CSL                            ; @BT 54
    TSX                            ; @BT BA
    BBS7 $05,$1C                   ; @BT FF 05 1C
    SED                            ; @BT F8
    TST #$48,$8BC6,X               ; @BT B3 48 C6 8B
    STA $7450,Y                    ; @BT 99 50 74
    CMP.W $2DEE                    ; @BT CD EE 2D
    BPL.B $75                      ; @BT 10 75
    SMB3 $6E                       ; @BT B7 6E
    STX.W $7AB6                    ; @BT 8E B6 7A
    PLX                            ; @BT FA
    DEC $55,X                      ; @BT D6 55
    ROL                            ; @BT 2A
    LDA $4AB9,X                    ; @BT BD B9 4A
    SBC.B $C7,X                    ; @BT F5 C7
    SBC.B $DE                      ; @BT E5 DE
    JMP $4453                      ; @BT 4C 53 44
    TRB.W $833E                    ; @BT 1C 3E 83
    BIT.B $44                      ; @BT 24 44
    TAM #$90                       ; @BT 53 90
    BIT $FF,X                      ; @BT 34 FF
    LDA.W $500E                    ; @BT AD 0E 50
    DEC $3343,X                    ; @BT DE 43 33
    JMP.W ($4675,X)                ; @BT 7C 75 46
    TXA                            ; @BT 8A
    SMB7 $CA                       ; @BT F7 CA
    LDA #$21                       ; @BT A9 21
    ROR.W $6AB7,X                  ; @BT 7E B7 6A
    RMB4 $CD                       ; @BT 47 CD
    LDX.W $5341,Y                  ; @BT BE 41 53
    TSB $844F                      ; @BT 0C 4F 84
    ORA.W $611E,X                  ; @BT 1D 1E 61
    TIN $8893,$89A4,$8AB5          ; @BT D3 93 88 A4 89 B5 8A
    STZ $7DE9,X                    ; @BT 9E E9 7D
    CPX.B $E2                      ; @BT E4 E2
    LDA.W $4EFD,Y                  ; @BT B9 FD 4E
    BEQ $4A                        ; @BT F0 4A
    BBR5 $1F,$36                   ; @BT 5F 1F 36
    ADC.B ($E0),Y                  ; @BT 71 E0
    PLA                            ; @BT 68
    STZ.B $C1,X                    ; @BT 74 C1
    STA ($0C),Y                    ; @BT 91 0C
    EOR.B $AE                      ; @BT 45 AE
    INX                            ; @BT E8
    RMB7 $12                       ; @BT 77 12
    CLA                            ; @BT 62
    RMB0 $71                       ; @BT 07 71
    AND.B ($0B)                    ; @BT 32 0B
    PLY                            ; @BT 7A
    ADC.W $1789,Y                  ; @BT 79 89 17
    ADC ($6D)                      ; @BT 72 6D
    LDY.B $32                      ; @BT A4 32
    BPL $0F                        ; @BT 10 0F
    SBC.W $722E                    ; @BT ED 2E 72
    ASL $50,X                      ; @BT 16 50
    EOR ($C8,X)                    ; @BT 41 C8
    PHP                            ; @BT 08
    SBC ($26)                      ; @BT F2 26
    STA.B ($7F),Y                  ; @BT 91 7F
    BIT.W $2788                    ; @BT 2C 88 27
    STA.B $C4                      ; @BT 85 C4
    CPY #$F9                       ; @BT C0 F9
    STZ.W $811C                    ; @BT 9C 1C 81
    CPY $3121                      ; @BT CC 21 31
    BSR.B $3A                      ; @BT 44 3A
    BBS0 $64,$7B                   ; @BT 8F 64 7B
    LDA $4BCA,Y                    ; @BT B9 CA 4B
    CMP.B $86                      ; @BT C5 86
    CMP ($A0,X)                    ; @BT C1 A0
    EOR $3898,Y                    ; @BT 59 98 38
    DEA                            ; @BT 3A
    STX.B $20                      ; @BT 86 20
    LDA ($4F),Y                    ; @BT B1 4F
    BIT $2566                      ; @BT 2C 66 25
    CMP.W $2CDD,Y                  ; @BT D9 DD 2C
    CMP ($B7),Y                    ; @BT D1 B7
    DEC.B $9A                      ; @BT C6 9A
    LDA.B ($D9),Y                  ; @BT B1 D9
    STZ $7EFA                      ; @BT 9C FA 7E
    ROL $6562                      ; @BT 2E 62 65
    TAM.B #$BE                     ; @BT 53 BE
    SBC.B ($B0)                    ; @BT F2 B0
    BBS4 $C0,$57                   ; @BT CF C0 57
    LDY.W $5896                    ; @BT AC 96 58
    STA $7561                      ; @BT 8D 61 75
    INC $3DED,X                    ; @BT FE ED 3D
    ORA ($D3),Y                    ; @BT 11 D3
    AND.B ($DD,X)                  ; @BT 21 DD
    ROL.B $E7,X                    ; @BT 36 E7
    STA $51                        ; @BT 85 51
    CPX $B4                        ; @BT E4 B4
    EOR $39A9                      ; @BT 4D A9 39
    CLD                            ; @BT D8
    ORA $18                        ; @BT 05 18
    SBC ($F8,X)                    ; @BT E1 F8
    CMP ($CE)                      ; @BT D2 CE
    ASL.B $7E,X                    ; @BT 16 7E
    AND.B ($F4),Y                  ; @BT 31 F4
    CLC                            ; @BT 18
    ADC #$28                       ; @BT 69 28
    ORA.B $8B,X                    ; @BT 15 8B
    EOR.B ($80)                    ; @BT 52 80
    LDX $4C,Y                      ; @BT B6 4C
    JMP ($4342)                    ; @BT 6C 42 43
    STY $37,X                      ; @BT 94 37
    SMB5 $9C                       ; @BT D7 9C
    ORA #$A5                       ; @BT 09 A5
    ORA $600D                      ; @BT 0D 0D 60
    ST2 #$1D                       ; @BT 23 1D
    STX.B $09,Y                    ; @BT 96 09
    ROL.W $6784                    ; @BT 2E 84 67
    LDA.B ($F0)                    ; @BT B2 F0
    CMP.B ($2A,X)                  ; @BT C1 2A
    SEC                            ; @BT 38
    BBS5 $D7,$6E                   ; @BT DF D7 6E
    CPX #$9D                       ; @BT E0 9D
    CPX.W $3010                    ; @BT EC 10 30
    AND #$3C                       ; @BT 29 3C
    ROR.W $6BC8                    ; @BT 6E C8 6B
    INC.B $F3,X                    ; @BT F6 F3
    TSB.W $8560                    ; @BT 0C 60 85
    STA $733F,X                    ; @BT 9D 3F 73
    TST #$76,$8CD7                 ; @BT 93 76 D7 8C
    BEQ.B $30                      ; @BT F0 30
    BBR2 $DA,$71                   ; @BT 2F DA 71
    EOR ($DF),Y                    ; @BT 51 DF
    CPX.B #$CB                     ; @BT E0 CB
    LDY.B $1B,X                    ; @BT B4 1B
    ORA.B #$2F                     ; @BT 09 2F
    BIT $2455,X                    ; @BT 3C 55 24
    TST #$31,$52,X                 ; @BT A3 31 52
    BVC $26                        ; @BT 50 26
    ORA.B ($46,X)                  ; @BT 01 46
    RMB6 $FB                       ; @BT 67 FB
    ORA.W $622F,Y                  ; @BT 19 2F 62
    SBC ($0F),Y                    ; @BT F1 0F
    JSR $4564                      ; @BT 20 64 45
    CLX                            ; @BT 82
    STZ $AA                        ; @BT 64 AA
    TAI $8AB5,$8BC6,$8CD7          ; @BT F3 B5 8A C6 8B D7 8C
    LDA ($66)                      ; @BT B2 66
    TRB $D5                        ; @BT 14 D5
    TDD $8782,$8893,$89A4          ; @BT C3 82 87 93 88 A4 89
    TII $8671,$8782,$8893          ; @BT 73 71 86 82 87 93 88
    BNE $78                        ; @BT D0 78
    INC $3EFE                      ; @BT EE FE 3E
    CMP $2ABB                      ; @BT CD BB 2A
    TRB $822D                      ; @BT 1C 2D 82
    BRK $5B                        ; @BT 00 5B
    AND ($6A),Y                    ; @BT 31 6A
    SMB4 $85                       ; @BT C7 85
    PLP                            ; @BT 28
    BBR7 $4D,$64                   ; @BT 7F 4D 64
    EOR.W $3ABA,X                  ; @BT 5D BA 3A
    LDA.B #$AB                     ; @BT A9 AB
    CLV                            ; @BT B8
    ROL.B $FE                      ; @BT 26 FE
    INY                            ; @BT C8
    SBC $3D,X                      ; @BT F5 3D
    ADC $9B                        ; @BT 65 9B
    BCC.B $02                      ; @BT 90 02
    LSR A                          ; @BT 4A
    STY.B $65,X                    ; @BT 94 65
    ASL                            ; @BT 0A
    BRK.B $72                      ; @BT 00 72
    LDY.B #$04                     ; @BT A0 04
    ROR $69A6                      ; @BT 6E A6 69
    ROR $6895,X                    ; @BT 7E 95 68
    ST0 #$EF                       ; @BT 03 EF
    ORA $5DEB,X                    ; @BT 1D EB 5D
    LSR $5AB8                      ; @BT 4E B8 5A
    LDA.B ($C2,X)                  ; @BT A1 C2
    LDX.B $A8                      ; @BT A6 A8
    ST1.B #$4B                     ; @BT 13 4B
    CMP.W $2BCC,X                  ; @BT DD CC 2B
    ASL.W $2233,X                  ; @BT 1E 33 22
    ASL $2011,X                    ; @BT 1E 11 20
    LDX.B #$7A                     ; @BT A2 7A
    ROL $6451,X                    ; @BT 3E 51 64
    LDA ($38,X)                    ; @BT A1 38
    SMB6 $B3                       ; @BT E7 B3
    SET                            ; @BT F4
    SAX                            ; @BT 22
    AND $98,X                      ; @BT 35 98
    STA ($F5,X)                    ; @BT 81 F5
    STZ.B $D8                      ; @BT 64 D8
    LSR $49,X                      ; @BT 56 49
    INC $DC                        ; @BT E6 DC
    ADC.B ($C9,X)                  ; @BT 61 C9
    SBC.B ($82,X)                  ; @BT E1 82
    DEC $3454                      ; @BT CE 54 34
    ADC $1345,X                    ; @BT 7D 45 13
    STX $F2                        ; @BT 86 F2
    RTI                            ; @BT 40
    INC.B $0A                      ; @BT E6 0A
    LDX #$35                       ; @BT A2 35
    BBR3 $F1,$08                   ; @BT 3F F1 08
    RTS                            ; @BT 60
    STA.B ($96)                    ; @BT 92 96
    LSR $60                        ; @BT 46 60
    LSR                            ; @BT 4A
    ROR $15,X                      ; @BT 76 15
    BCS.B $19                      ; @BT B0 19
    DEC $6C                        ; @BT C6 6C
    PHA                            ; @BT 48
    LDX $511F,Y                    ; @BT BE 1F 51
    ADC.B ($F7)                    ; @BT 72 F7
    TRB.B $EC                      ; @BT 14 EC
    SEI                            ; @BT 78
    DEC.B $83,X                    ; @BT D6 83
    PHY                            ; @BT 5A
    ADC ($56),Y                    ; @BT 71 56
    LSR.B $8E                      ; @BT 46 8E
    ASL $67                        ; @BT 06 67
    BCS $33                        ; @BT B0 33
    NOP                            ; @BT EA
    ROL.W $6673,X                  ; @BT 3E 73 66
    ASL.W $2344                    ; @BT 0E 44 23
    ORA.B $A2                      ; @BT 05 A2
    LSR.W $5CDA                    ; @BT 4E DA 5C
    LDX $5230                      ; @BT AE 30 52
    ROL A                          ; @BT 2A
    AND.W $1CDE,X                  ; @BT 3D DE 1C
    BMI $61                        ; @BT 30 61
    ST0.B #$34                     ; @BT 03 34
    STY.B $7C                      ; @BT 84 7C
    CLI                            ; @BT 58
    EOR ($F6)                      ; @BT 52 F6
    EOR $0D,X                      ; @BT 55 0D
    LDX.W $5452                    ; @BT AE 52 54
    TXS                            ; @BT 9A
    CMP.B ($58)                    ; @BT D2 58
    RMB3 $B6                       ; @BT 37 B6
    ADC $1456,Y                    ; @BT 79 56 14
    INC.W $400F,X                  ; @BT FE 0F 40
    ROR.B $5A                      ; @BT 66 5A
    CMP $2899,X                    ; @BT DD 99 28
    CMP.B ($41),Y                  ; @BT D1 41
    TAY                            ; @BT A8
    CPX $2EFF                      ; @BT EC FF 2E
    AND ($81)                      ; @BT 32 81
    SBC $6CD9,X                    ; @BT FD D9 6C
    INC.W $4120                    ; @BT EE 20 41
    SMB1 $40                       ; @BT 97 40
    STY.W $7CD8                    ; @BT 8C D8 7C
    JSR.W $49A8                    ; @BT 20 A8 49
    LDX $63                        ; @BT A6 63
    EOR.B $97,X                    ; @BT 55 97
    TIA $89A4,$8AB5,$8BC6          ; @BT E3 A4 89 B5 8A C6 8B
    BMI.B $47                      ; @BT 30 47
    RMB1 $88                       ; @BT 17 88
    EOR.B #$3B                     ; @BT 49 3B
    ASL.B $95                      ; @BT 06 95
    ORA $01,X                      ; @BT 15 01
    LDA.W $4DEC,X                  ; @BT BD EC 4D
    BBR6 $36,$4D                   ; @BT 6F 36 4D
    EOR $24                        ; @BT 45 24
    AND ($53,X)                    ; @BT 21 53
    EOR.B ($52,X)                  ; @BT 41 52
    STZ $800B,X                    ; @BT 9E 0B 80
    LDY $5563,X                    ; @BT BC 63 55
    TAX                            ; @BT AA
    ST2.B #$62                     ; @BT 23 62
    LDY $ED                        ; @BT A4 ED
    SBC $6EFB                      ; @BT ED FB 6E
    TMA #$79                       ; @BT 43 79
    LDY #$BF                       ; @BT A0 BF
    JMP ($4231,X)                  ; @BT 7C 31 42
    ORA ($EA)                      ; @BT 12 EA
    BBR0 $AC,$43                   ; @BT 0F AC 43
    CMP.B #$13                     ; @BT C9 13
    ROR.B $43,X                    ; @BT 76 43
    CMP $FC                        ; @BT C5 FC
    JMP.W ($4786)                  ; @BT 6C 86 47
    ROR A                          ; @BT 6A
    SMB0 $29                       ; @BT 87 29
    BSR $54                        ; @BT 44 54
    RMB2 $9F                       ; @BT 27 9F
    LDA $4CDB                      ; @BT AD DB 4C

    .db "<02"               ; @BT END

    /////////////////////////////////////////////////////////////////////////////
    // TEST-03: same opcodes, randomised order, operands as arithmetic expressions
    /////////////////////////////////////////////////////////////////////////////

    .db "03>"               ; @BT TEST-03 03 START

    ; GENERATED: copied from TEST-02 with varied arithmetic expressions for numeric operands

    ; GENERATED: copied from TEST-01 with full TEST-02 randomization
    ; GENERATED: seed = 35714263

    SAY                            ; @BT 42
    STY $4E+5-5                        ; @BT 84 4E
    TST #$5F-7+7,$80*6/3-$80                   ; @BT 83 5F 80
    DEY                            ; @BT 88
    RMB5 $E4*10/5-$E4                       ; @BT 57 E4
    LSR.W $5BC9*2-$5BC9+0,X                  ; @BT 5E C9 5B
    LSR.B $77+11-4-7,X                    ; @BT 56 77
    TYA                            ; @BT 98
    BRA.B $51+3-1-2                      ; @BT 80 51
    CMP #$89+5-5                       ; @BT C9 89
    BVS $3D-7+7                        ; @BT 70 3D
    BIT.W $2677*6/3-$2677,X                  ; @BT 3C 77 26
    BBS6 $EE*10/5-$EE,$05*2-$05+0                   ; @BT EF EE 05
    CMP $29AA+11-4-7,Y                    ; @BT D9 AA 29
    CLY                            ; @BT C2
    STY $7BC7+3-1-2                      ; @BT 8C C7 7B
    CPY.W $3232+5-5                    ; @BT CC 32 32
    AND.B #$C6-7+7                     ; @BT 29 C6
    STA.W $7672*6/3-$7672,X                  ; @BT 9D 72 76
    ORA ($BC*10/5-$BC,X)                    ; @BT 01 BC
    DEC.W $3676*2-$3676+0                    ; @BT CE 76 36
    ADC $84+11-4-7,X                      ; @BT 75 84
    AND.B $39+3-1-2                      ; @BT 25 39
    TSB $03+5-5                        ; @BT 04 03
    LDA.B $1E-7+7                      ; @BT A5 1E
    SBC $54*6/3-$54                        ; @BT E5 54
    SBC.W $711D*10/5-$711D,Y                  ; @BT F9 1D 71
    SBC.W $700C*2-$700C+0,X                  ; @BT FD 0C 70
    JMP.W $4897+11-4-7                    ; @BT 4C 97 48
    AND $1BCD+3-1-2                      ; @BT 2D CD 1B
    ROR $2C+5-5                        ; @BT 66 2C
    LDY.W $5785-7+7,X                  ; @BT BC 85 57
    BRK                            ; @BT 00 00
    ROR                            ; @BT 6A
    EOR.B ($69*6/3-$69),Y                  ; @BT 51 69
    AND.B $22*10/5-$22,X                    ; @BT 35 22
    SXY                            ; @BT 02
    ROL $D0*2-$D0+0                        ; @BT 26 D0
    BVS.B $23+11-4-7                      ; @BT 70 23
    ORA.B ($5D+3-1-2),Y                  ; @BT 11 5D
    EOR $3787+5-5,X                    ; @BT 5D 87 37
    AND $19AB-7+7,X                    ; @BT 3D AB 19
    LDA $7D*6/3-$7D,X                      ; @BT B5 7D
    BBS2 $92*10/5-$92,$29*2-$29+0                   ; @BT AF 92 29
    BBR1 $C3+11-4-7,$5A+3-1-2                   ; @BT 1F C3 5A
    SMB2 $57+5-5                       ; @BT A7 57
    LDY $D6-7+7,X                      ; @BT B4 D6
    ADC $1567*6/3-$1567                      ; @BT 6D 67 15
    STA.B $AD*10/5-$AD,X                    ; @BT 95 AD
    ASL $2122*2-$2122+0                      ; @BT 0E 22 21
    BNE.B $5E+11-4-7                      ; @BT D0 5E
    BVC.B $0C+3-1-2                      ; @BT 50 0C
    CPY.B $3E+5-5                      ; @BT C4 3E
    AND $1ABC-7+7,Y                    ; @BT 39 BC 1A
    ADC.W $189A*6/3-$189A                    ; @BT 6D 9A 18
    CSH                            ; @BT D4
    SBC.B #$6B*10/5-$6B                     ; @BT E9 6B
    STA ($23*2-$23+0)                      ; @BT 92 23
    ADC.B #$B2+11-4-7                     ; @BT 69 B2
    BRA $6B+3-1-2                        ; @BT 80 6B
    ADC.B $0E+5-5,X                    ; @BT 75 0E
    LDA $94-7+7                        ; @BT A5 94
    AND $AF*6/3-$AF                        ; @BT 25 AF
    LDX.B $91*10/5-$91,Y                    ; @BT B6 91
    SBC #$E1*2-$E1+0                       ; @BT E9 E1
    LSR $59A7+11-4-7,X                    ; @BT 5E A7 59
    STX $79A5+3-1-2                      ; @BT 8E A5 79
    LDY $5674+5-5                      ; @BT AC 74 56
    LDA.B $07-7+7,X                    ; @BT B5 07
    CPY.B #$27*6/3-$27                     ; @BT C0 27
    ASL A                          ; @BT 0A
    BIT.B $2D*10/5-$2D,X                    ; @BT 34 2D
    DEC.W $3565*2-$3565+0,X                  ; @BT DE 65 35
    ROL $B9+11-4-7,X                      ; @BT 36 B9
    BIT $16+3-1-2                        ; @BT 24 16
    EOR.W $3CDC+5-5                    ; @BT 4D DC 3C
    ORA.W $6340-7+7                    ; @BT 0D 40 63
    INC $C5*6/3-$C5,X                      ; @BT F6 C5
    TMA.B #$A7*10/5-$A7                     ; @BT 43 A7
    STA $3A*2-$3A+0,X                      ; @BT 95 3A
    STZ $93+11-4-7,X                      ; @BT 74 93
    STA.B ($68+3-1-2,X)                  ; @BT 81 68
    EOR.W $3BCB+5-5,Y                  ; @BT 59 CB 3B
    ORA $5EFC-7+7,Y                    ; @BT 19 FC 5E
    ORA.B ($74*6/3-$74)                    ; @BT 12 74
    BCC $1C*10/5-$1C                        ; @BT 90 1C
    INA                            ; @BT 1A
    SBC $6DEA*2-$6DEA+0,Y                    ; @BT F9 EA 6D
    BBS1 $7B+11-4-7,$12+3-1-2                   ; @BT 9F 7B 12
    BBS3 $A9+5-5,$40-7+7                   ; @BT BF A9 40
    BIT #$E8*6/3-$E8                       ; @BT 89 E8
    EOR #$B1*10/5-$B1                       ; @BT 49 B1
    CMP.B $6F*2-$6F+0,X                    ; @BT D5 6F
    AND.W $1F00+11-4-7                    ; @BT 2D 00 1F
    CPY $10+3-1-2                        ; @BT C4 10
    PHX                            ; @BT DA
    BBR4 $08+5-5,$1F-7+7                   ; @BT 4F 08 1F
    STA.W $7894*6/3-$7894                    ; @BT 8D 94 78
    ADC.B $25*10/5-$25                      ; @BT 65 25
    STX $DB*2-$DB+0,Y                      ; @BT 96 DB
    STA.W $7783+11-4-7,Y                  ; @BT 99 83 77
    DEX                            ; @BT CA
    ADC.W $1678+3-1-2,X                  ; @BT 7D 78 16
    AND.W $1DEF+5-5,Y                  ; @BT 39 EF 1D
    SBC.B ($99-7+7),Y                  ; @BT F1 99
    ADC ($3F*6/3-$3F,X)                    ; @BT 61 3F
    CMP $E5*10/5-$E5,X                      ; @BT D5 E5
    ST1 #$06*2-$06+0                       ; @BT 13 06
    TSB.B $1A+11-4-7                      ; @BT 04 1A
    CSL                            ; @BT 54
    TSX                            ; @BT BA
    BBS7 $05+3-1-2,$1C+5-5                   ; @BT FF 05 1C
    SED                            ; @BT F8
    TST #$48-7+7,$8BC6*6/3-$8BC6,X               ; @BT B3 48 C6 8B
    STA $7450*10/5-$7450,Y                    ; @BT 99 50 74
    CMP.W $2DEE*2-$2DEE+0                    ; @BT CD EE 2D
    BPL.B $75+11-4-7                      ; @BT 10 75
    SMB3 $6E+3-1-2                       ; @BT B7 6E
    STX.W $7AB6+5-5                    ; @BT 8E B6 7A
    PLX                            ; @BT FA
    DEC $55-7+7,X                      ; @BT D6 55
    ROL                            ; @BT 2A
    LDA $4AB9*6/3-$4AB9,X                    ; @BT BD B9 4A
    SBC.B $C7*10/5-$C7,X                    ; @BT F5 C7
    SBC.B $DE*2-$DE+0                      ; @BT E5 DE
    JMP $4453+11-4-7                      ; @BT 4C 53 44
    TRB.W $833E+3-1-2                    ; @BT 1C 3E 83
    BIT.B $44+5-5                      ; @BT 24 44
    TAM #$90-7+7                       ; @BT 53 90
    BIT $FF*6/3-$FF,X                      ; @BT 34 FF
    LDA.W $500E*10/5-$500E                    ; @BT AD 0E 50
    DEC $3343*2-$3343+0,X                    ; @BT DE 43 33
    JMP.W ($4675+11-4-7,X)                ; @BT 7C 75 46
    TXA                            ; @BT 8A
    SMB7 $CA+3-1-2                       ; @BT F7 CA
    LDA #$21+5-5                       ; @BT A9 21
    ROR.W $6AB7-7+7,X                  ; @BT 7E B7 6A
    RMB4 $CD*6/3-$CD                       ; @BT 47 CD
    LDX.W $5341*10/5-$5341,Y                  ; @BT BE 41 53
    TSB $844F*2-$844F+0                      ; @BT 0C 4F 84
    ORA.W $611E+11-4-7,X                  ; @BT 1D 1E 61
    TIN $8893+3-1-2,$89A4+5-5,$8AB5-7+7          ; @BT D3 93 88 A4 89 B5 8A
    STZ $7DE9*6/3-$7DE9,X                    ; @BT 9E E9 7D
    CPX.B $E2*10/5-$E2                      ; @BT E4 E2
    LDA.W $4EFD*2-$4EFD+0,Y                  ; @BT B9 FD 4E
    BEQ $4A+11-4-7                        ; @BT F0 4A
    BBR5 $1F+3-1-2,$36+5-5                   ; @BT 5F 1F 36
    ADC.B ($E0-7+7),Y                  ; @BT 71 E0
    PLA                            ; @BT 68
    STZ.B $C1*6/3-$C1,X                    ; @BT 74 C1
    STA ($0C*10/5-$0C),Y                    ; @BT 91 0C
    EOR.B $AE*2-$AE+0                      ; @BT 45 AE
    INX                            ; @BT E8
    RMB7 $12+11-4-7                       ; @BT 77 12
    CLA                            ; @BT 62
    RMB0 $71+3-1-2                       ; @BT 07 71
    AND.B ($0B+5-5)                    ; @BT 32 0B
    PLY                            ; @BT 7A
    ADC.W $1789-7+7,Y                  ; @BT 79 89 17
    ADC ($6D*6/3-$6D)                      ; @BT 72 6D
    LDY.B $32*10/5-$32                      ; @BT A4 32
    BPL $0F*2-$0F+0                        ; @BT 10 0F
    SBC.W $722E+11-4-7                    ; @BT ED 2E 72
    ASL $50+3-1-2,X                      ; @BT 16 50
    EOR ($C8+5-5,X)                    ; @BT 41 C8
    PHP                            ; @BT 08
    SBC ($26-7+7)                      ; @BT F2 26
    STA.B ($7F*6/3-$7F),Y                  ; @BT 91 7F
    BIT.W $2788*10/5-$2788                    ; @BT 2C 88 27
    STA.B $C4*2-$C4+0                      ; @BT 85 C4
    CPY #$F9+11-4-7                       ; @BT C0 F9
    STZ.W $811C+3-1-2                    ; @BT 9C 1C 81
    CPY $3121+5-5                      ; @BT CC 21 31
    BSR.B $3A-7+7                      ; @BT 44 3A
    BBS0 $64*6/3-$64,$7B*10/5-$7B                   ; @BT 8F 64 7B
    LDA $4BCA*2-$4BCA+0,Y                    ; @BT B9 CA 4B
    CMP.B $86+11-4-7                      ; @BT C5 86
    CMP ($A0+3-1-2,X)                    ; @BT C1 A0
    EOR $3898+5-5,Y                    ; @BT 59 98 38
    DEA                            ; @BT 3A
    STX.B $20-7+7                      ; @BT 86 20
    LDA ($4F*6/3-$4F),Y                    ; @BT B1 4F
    BIT $2566*10/5-$2566                      ; @BT 2C 66 25
    CMP.W $2CDD*2-$2CDD+0,Y                  ; @BT D9 DD 2C
    CMP ($B7+11-4-7),Y                    ; @BT D1 B7
    DEC.B $9A+3-1-2                      ; @BT C6 9A
    LDA.B ($D9+5-5),Y                  ; @BT B1 D9
    STZ $7EFA-7+7                      ; @BT 9C FA 7E
    ROL $6562*6/3-$6562                      ; @BT 2E 62 65
    TAM.B #$BE*10/5-$BE                     ; @BT 53 BE
    SBC.B ($B0*2-$B0+0)                    ; @BT F2 B0
    BBS4 $C0+11-4-7,$57+3-1-2                   ; @BT CF C0 57
    LDY.W $5896+5-5                    ; @BT AC 96 58
    STA $7561-7+7                      ; @BT 8D 61 75
    INC $3DED*6/3-$3DED,X                    ; @BT FE ED 3D
    ORA ($D3*10/5-$D3),Y                    ; @BT 11 D3
    AND.B ($DD*2-$DD+0,X)                  ; @BT 21 DD
    ROL.B $E7+11-4-7,X                    ; @BT 36 E7
    STA $51+3-1-2                        ; @BT 85 51
    CPX $B4+5-5                        ; @BT E4 B4
    EOR $39A9-7+7                      ; @BT 4D A9 39
    CLD                            ; @BT D8
    ORA $18*6/3-$18                        ; @BT 05 18
    SBC ($F8*10/5-$F8,X)                    ; @BT E1 F8
    CMP ($CE*2-$CE+0)                      ; @BT D2 CE
    ASL.B $7E+11-4-7,X                    ; @BT 16 7E
    AND.B ($F4+3-1-2),Y                  ; @BT 31 F4
    CLC                            ; @BT 18
    ADC #$28+5-5                       ; @BT 69 28
    ORA.B $8B-7+7,X                    ; @BT 15 8B
    EOR.B ($80*6/3-$80)                    ; @BT 52 80
    LDX $4C*10/5-$4C,Y                      ; @BT B6 4C
    JMP ($4342*2-$4342+0)                    ; @BT 6C 42 43
    STY $37+11-4-7,X                      ; @BT 94 37
    SMB5 $9C+3-1-2                       ; @BT D7 9C
    ORA #$A5+5-5                       ; @BT 09 A5
    ORA $600D-7+7                      ; @BT 0D 0D 60
    ST2 #$1D*6/3-$1D                       ; @BT 23 1D
    STX.B $09*10/5-$09,Y                    ; @BT 96 09
    ROL.W $6784*2-$6784+0                    ; @BT 2E 84 67
    LDA.B ($F0+11-4-7)                    ; @BT B2 F0
    CMP.B ($2A+3-1-2,X)                  ; @BT C1 2A
    SEC                            ; @BT 38
    BBS5 $D7+5-5,$6E-7+7                   ; @BT DF D7 6E
    CPX #$9D*6/3-$9D                       ; @BT E0 9D
    CPX.W $3010*10/5-$3010                    ; @BT EC 10 30
    AND #$3C*2-$3C+0                       ; @BT 29 3C
    ROR.W $6BC8+11-4-7                    ; @BT 6E C8 6B
    INC.B $F3+3-1-2,X                    ; @BT F6 F3
    TSB.W $8560+5-5                    ; @BT 0C 60 85
    STA $733F-7+7,X                    ; @BT 9D 3F 73
    TST #$76*6/3-$76,$8CD7*10/5-$8CD7                 ; @BT 93 76 D7 8C
    BEQ.B $30*2-$30+0                      ; @BT F0 30
    BBR2 $DA+11-4-7,$71+3-1-2                   ; @BT 2F DA 71
    EOR ($DF+5-5),Y                    ; @BT 51 DF
    CPX.B #$CB-7+7                     ; @BT E0 CB
    LDY.B $1B*6/3-$1B,X                    ; @BT B4 1B
    ORA.B #$2F*10/5-$2F                     ; @BT 09 2F
    BIT $2455*2-$2455+0,X                    ; @BT 3C 55 24
    TST #$31+11-4-7,$52+3-1-2,X                 ; @BT A3 31 52
    BVC $26+5-5                        ; @BT 50 26
    ORA.B ($46-7+7,X)                  ; @BT 01 46
    RMB6 $FB*6/3-$FB                       ; @BT 67 FB
    ORA.W $622F*10/5-$622F,Y                  ; @BT 19 2F 62
    SBC ($0F*2-$0F+0),Y                    ; @BT F1 0F
    JSR $4564+11-4-7                      ; @BT 20 64 45
    CLX                            ; @BT 82
    STZ $AA+3-1-2                        ; @BT 64 AA
    TAI $8AB5+5-5,$8BC6-7+7,$8CD7*6/3-$8CD7          ; @BT F3 B5 8A C6 8B D7 8C
    LDA ($66*10/5-$66)                      ; @BT B2 66
    TRB $D5*2-$D5+0                        ; @BT 14 D5
    TDD $8782+11-4-7,$8893+3-1-2,$89A4+5-5          ; @BT C3 82 87 93 88 A4 89
    TII $8671-7+7,$8782*6/3-$8782,$8893*10/5-$8893          ; @BT 73 71 86 82 87 93 88
    BNE $78*2-$78+0                        ; @BT D0 78
    INC $3EFE+11-4-7                      ; @BT EE FE 3E
    CMP $2ABB+3-1-2                      ; @BT CD BB 2A
    TRB $822D+5-5                      ; @BT 1C 2D 82
    BRK $5B-7+7                        ; @BT 00 5B
    AND ($6A*6/3-$6A),Y                    ; @BT 31 6A
    SMB4 $85*10/5-$85                       ; @BT C7 85
    PLP                            ; @BT 28
    BBR7 $4D*2-$4D+0,$64+11-4-7                   ; @BT 7F 4D 64
    EOR.W $3ABA+3-1-2,X                  ; @BT 5D BA 3A
    LDA.B #$AB+5-5                     ; @BT A9 AB
    CLV                            ; @BT B8
    ROL.B $FE-7+7                      ; @BT 26 FE
    INY                            ; @BT C8
    SBC $3D*6/3-$3D,X                      ; @BT F5 3D
    ADC $9B*10/5-$9B                        ; @BT 65 9B
    BCC.B $02*2-$02+0                      ; @BT 90 02
    LSR A                          ; @BT 4A
    STY.B $65+11-4-7,X                    ; @BT 94 65
    ASL                            ; @BT 0A
    BRK.B $72+3-1-2                      ; @BT 00 72
    LDY.B #$04+5-5                     ; @BT A0 04
    ROR $69A6-7+7                      ; @BT 6E A6 69
    ROR $6895*6/3-$6895,X                    ; @BT 7E 95 68
    ST0 #$EF*10/5-$EF                       ; @BT 03 EF
    ORA $5DEB*2-$5DEB+0,X                    ; @BT 1D EB 5D
    LSR $5AB8+11-4-7                      ; @BT 4E B8 5A
    LDA.B ($C2+3-1-2,X)                  ; @BT A1 C2
    LDX.B $A8+5-5                      ; @BT A6 A8
    ST1.B #$4B-7+7                     ; @BT 13 4B
    CMP.W $2BCC*6/3-$2BCC,X                  ; @BT DD CC 2B
    ASL.W $2233*10/5-$2233,X                  ; @BT 1E 33 22
    ASL $2011*2-$2011+0,X                    ; @BT 1E 11 20
    LDX.B #$7A+11-4-7                     ; @BT A2 7A
    ROL $6451+3-1-2,X                    ; @BT 3E 51 64
    LDA ($38+5-5,X)                    ; @BT A1 38
    SMB6 $B3-7+7                       ; @BT E7 B3
    SET                            ; @BT F4
    SAX                            ; @BT 22
    AND $98*6/3-$98,X                      ; @BT 35 98
    STA ($F5*10/5-$F5,X)                    ; @BT 81 F5
    STZ.B $D8*2-$D8+0                      ; @BT 64 D8
    LSR $49+11-4-7,X                      ; @BT 56 49
    INC $DC+3-1-2                        ; @BT E6 DC
    ADC.B ($C9+5-5,X)                  ; @BT 61 C9
    SBC.B ($82-7+7,X)                  ; @BT E1 82
    DEC $3454*6/3-$3454                      ; @BT CE 54 34
    ADC $1345*10/5-$1345,X                    ; @BT 7D 45 13
    STX $F2*2-$F2+0                        ; @BT 86 F2
    RTI                            ; @BT 40
    INC.B $0A+11-4-7                      ; @BT E6 0A
    LDX #$35+3-1-2                       ; @BT A2 35
    BBR3 $F1+5-5,$08-7+7                   ; @BT 3F F1 08
    RTS                            ; @BT 60
    STA.B ($96*6/3-$96)                    ; @BT 92 96
    LSR $60*10/5-$60                        ; @BT 46 60
    LSR                            ; @BT 4A
    ROR $15*2-$15+0,X                      ; @BT 76 15
    BCS.B $19+11-4-7                      ; @BT B0 19
    DEC $6C+3-1-2                        ; @BT C6 6C
    PHA                            ; @BT 48
    LDX $511F+5-5,Y                    ; @BT BE 1F 51
    ADC.B ($F7-7+7)                    ; @BT 72 F7
    TRB.B $EC*6/3-$EC                      ; @BT 14 EC
    SEI                            ; @BT 78
    DEC.B $83*10/5-$83,X                    ; @BT D6 83
    PHY                            ; @BT 5A
    ADC ($56*2-$56+0),Y                    ; @BT 71 56
    LSR.B $8E+11-4-7                      ; @BT 46 8E
    ASL $67+3-1-2                        ; @BT 06 67
    BCS $33+5-5                        ; @BT B0 33
    NOP                            ; @BT EA
    ROL.W $6673-7+7,X                  ; @BT 3E 73 66
    ASL.W $2344*6/3-$2344                    ; @BT 0E 44 23
    ORA.B $A2*10/5-$A2                      ; @BT 05 A2
    LSR.W $5CDA*2-$5CDA+0                    ; @BT 4E DA 5C
    LDX $5230+11-4-7                      ; @BT AE 30 52
    ROL A                          ; @BT 2A
    AND.W $1CDE+3-1-2,X                  ; @BT 3D DE 1C
    BMI $61+5-5                        ; @BT 30 61
    ST0.B #$34-7+7                     ; @BT 03 34
    STY.B $7C*6/3-$7C                      ; @BT 84 7C
    CLI                            ; @BT 58
    EOR ($F6*10/5-$F6)                      ; @BT 52 F6
    EOR $0D*2-$0D+0,X                      ; @BT 55 0D
    LDX.W $5452+11-4-7                    ; @BT AE 52 54
    TXS                            ; @BT 9A
    CMP.B ($58+3-1-2)                    ; @BT D2 58
    RMB3 $B6+5-5                       ; @BT 37 B6
    ADC $1456-7+7,Y                    ; @BT 79 56 14
    INC.W $400F*6/3-$400F,X                  ; @BT FE 0F 40
    ROR.B $5A*10/5-$5A                      ; @BT 66 5A
    CMP $2899*2-$2899+0,X                    ; @BT DD 99 28
    CMP.B ($41+11-4-7),Y                  ; @BT D1 41
    TAY                            ; @BT A8
    CPX $2EFF+3-1-2                      ; @BT EC FF 2E
    AND ($81+5-5)                      ; @BT 32 81
    SBC $6CD9-7+7,X                    ; @BT FD D9 6C
    INC.W $4120*6/3-$4120                    ; @BT EE 20 41
    SMB1 $40*10/5-$40                       ; @BT 97 40
    STY.W $7CD8*2-$7CD8+0                    ; @BT 8C D8 7C
    JSR.W $49A8+11-4-7                    ; @BT 20 A8 49
    LDX $63+3-1-2                        ; @BT A6 63
    EOR.B $97+5-5,X                    ; @BT 55 97
    TIA $89A4-7+7,$8AB5*6/3-$8AB5,$8BC6*10/5-$8BC6          ; @BT E3 A4 89 B5 8A C6 8B
    BMI.B $47*2-$47+0                      ; @BT 30 47
    RMB1 $88+11-4-7                       ; @BT 17 88
    EOR.B #$3B+3-1-2                     ; @BT 49 3B
    ASL.B $95+5-5                      ; @BT 06 95
    ORA $01-7+7,X                      ; @BT 15 01
    LDA.W $4DEC*6/3-$4DEC,X                  ; @BT BD EC 4D
    BBR6 $36*10/5-$36,$4D*2-$4D+0                   ; @BT 6F 36 4D
    EOR $24+11-4-7                        ; @BT 45 24
    AND ($53+3-1-2,X)                    ; @BT 21 53
    EOR.B ($52+5-5,X)                  ; @BT 41 52
    STZ $800B-7+7,X                    ; @BT 9E 0B 80
    LDY $5563*6/3-$5563,X                    ; @BT BC 63 55
    TAX                            ; @BT AA
    ST2.B #$62*10/5-$62                     ; @BT 23 62
    LDY $ED*2-$ED+0                        ; @BT A4 ED
    SBC $6EFB+11-4-7                      ; @BT ED FB 6E
    TMA #$79+3-1-2                       ; @BT 43 79
    LDY #$BF+5-5                       ; @BT A0 BF
    JMP ($4231-7+7,X)                  ; @BT 7C 31 42
    ORA ($EA*6/3-$EA)                      ; @BT 12 EA
    BBR0 $AC*10/5-$AC,$43*2-$43+0                   ; @BT 0F AC 43
    CMP.B #$13+11-4-7                     ; @BT C9 13
    ROR.B $43+3-1-2,X                    ; @BT 76 43
    CMP $FC+5-5                        ; @BT C5 FC
    JMP.W ($4786-7+7)                  ; @BT 6C 86 47
    ROR A                          ; @BT 6A
    SMB0 $29*6/3-$29                       ; @BT 87 29
    BSR $54*10/5-$54                        ; @BT 44 54
    RMB2 $9F*2-$9F+0                       ; @BT 27 9F
    LDA $4CDB+11-4-7                      ; @BT AD DB 4C

    .db "<03"               ; @BT END

    /////////////////////////////////////////////////////////////////////////////
    // TEST-04: similar to TEST-03 and then defines + expressions
    /////////////////////////////////////////////////////////////////////////////









    ; TEST-04 generated defines
    .define T4_DEF_001 (($4E+5)-5)
    .define T4_DEF_002 ((T4_DEF_001)+(($5F)-($4E)))
    .define T4_DEF_003 (($E4-7)+7)
    .define T4_DEF_004 (((T4_DEF_003)+($5BC9))-($E4))
    .define T4_DEF_005 ((($77*6)/3)-($77))
    .define T4_DEF_006 (((T4_DEF_005)*2+($51)-(($77)*2)))
    .define T4_DEF_007 ((($89*10)/5)-($89))
    .define T4_DEF_008 ((($3D)-($89))+(T4_DEF_007))
    .define T4_DEF_009 (($2677*2)-(($2677)-0))
    .define T4_DEF_010 ((T4_DEF_009)+(($EE)-(($2677+3)-3)))
    .define T4_DEF_011 ((($29AA+11)-4)-7)
    .define T4_DEF_012 ((T4_DEF_011)+(($7BC7)-($29AA)))
    .define T4_DEF_013 ((($3232+3)-1)-2)
    .define T4_DEF_014 (((T4_DEF_013)+($C6))-($3232))
    .define T4_DEF_015 (($7672+5)-5)
    .define T4_DEF_016 (((T4_DEF_015)*2+($BC)-(($7672)*2)))
    .define T4_DEF_017 (($3676-7)+7)
    .define T4_DEF_018 ((($84)-($3676))+(T4_DEF_017))
    .define T4_DEF_019 ((($39*6)/3)-($39))
    .define T4_DEF_020 ((T4_DEF_019)+(($03)-(($39+3)-3)))
    .define T4_DEF_021 ((($1E*10)/5)-($1E))
    .define T4_DEF_022 ((T4_DEF_021)+(($54)-($1E)))
    .define T4_DEF_023 (($711D*2)-(($711D)-0))
    .define T4_DEF_024 (((T4_DEF_023)+($700C))-($711D))
    .define T4_DEF_025 ((($4897+11)-4)-7)
    .define T4_DEF_026 (((T4_DEF_025)*2+($1BCD)-(($4897)*2)))
    .define T4_DEF_027 ((($2C+3)-1)-2)
    .define T4_DEF_028 ((($5785)-($2C))+(T4_DEF_027))
    .define T4_DEF_029 (($69+5)-5)
    .define T4_DEF_030 ((T4_DEF_029)+(($22)-(($69+3)-3)))
    .define T4_DEF_031 (($D0-7)+7)
    .define T4_DEF_032 ((T4_DEF_031)+(($23)-($D0)))
    .define T4_DEF_033 ((($5D*6)/3)-($5D))
    .define T4_DEF_034 (((T4_DEF_033)+($3787))-($5D))
    .define T4_DEF_035 ((($19AB*10)/5)-($19AB))
    .define T4_DEF_036 (((T4_DEF_035)*2+($7D)-(($19AB)*2)))
    .define T4_DEF_037 (($92*2)-(($92)-0))
    .define T4_DEF_038 ((($C3)-($92))+(T4_DEF_037))
    .define T4_DEF_039 ((($57+11)-4)-7)
    .define T4_DEF_040 ((T4_DEF_039)+(($D6)-(($57+3)-3)))
    .define T4_DEF_041 ((($1567+3)-1)-2)
    .define T4_DEF_042 ((T4_DEF_041)+(($AD)-($1567)))
    .define T4_DEF_043 (($2122+5)-5)
    .define T4_DEF_044 (((T4_DEF_043)+($5E))-($2122))
    .define T4_DEF_045 (($0C-7)+7)
    .define T4_DEF_046 (((T4_DEF_045)*2+($3E)-(($0C)*2)))
    .define T4_DEF_047 ((($1ABC*6)/3)-($1ABC))
    .define T4_DEF_048 ((($189A)-($1ABC))+(T4_DEF_047))
    .define T4_DEF_049 ((($6B*10)/5)-($6B))
    .define T4_DEF_050 ((T4_DEF_049)+(($23)-(($6B+3)-3)))
    .define T4_DEF_051 (($B2*2)-(($B2)-0))
    .define T4_DEF_052 ((T4_DEF_051)+(($6B)-($B2)))
    .define T4_DEF_053 ((($0E+11)-4)-7)
    .define T4_DEF_054 (((T4_DEF_053)+($94))-($0E))
    .define T4_DEF_055 ((($AF+3)-1)-2)
    .define T4_DEF_056 (((T4_DEF_055)*2+($91)-(($AF)*2)))
    .define T4_DEF_057 (($E1+5)-5)
    .define T4_DEF_058 ((($59A7)-($E1))+(T4_DEF_057))
    .define T4_DEF_059 (($79A5-7)+7)
    .define T4_DEF_060 ((T4_DEF_059)+(($5674)-(($79A5+3)-3)))
    .define T4_DEF_061 ((($07*6)/3)-($07))
    .define T4_DEF_062 ((T4_DEF_061)+(($27)-($07)))
    .define T4_DEF_063 ((($2D*10)/5)-($2D))
    .define T4_DEF_064 (((T4_DEF_063)+($3565))-($2D))
    .define T4_DEF_065 (($B9*2)-(($B9)-0))
    .define T4_DEF_066 (((T4_DEF_065)*2+($16)-(($B9)*2)))
    .define T4_DEF_067 ((($3CDC+11)-4)-7)
    .define T4_DEF_068 ((($6340)-($3CDC))+(T4_DEF_067))
    .define T4_DEF_069 ((($C5+3)-1)-2)
    .define T4_DEF_070 ((T4_DEF_069)+(($A7)-(($C5+3)-3)))
    .define T4_DEF_071 (($3A+5)-5)
    .define T4_DEF_072 ((T4_DEF_071)+(($93)-($3A)))
    .define T4_DEF_073 (($68-7)+7)
    .define T4_DEF_074 (((T4_DEF_073)+($3BCB))-($68))
    .define T4_DEF_075 ((($5EFC*6)/3)-($5EFC))
    .define T4_DEF_076 (((T4_DEF_075)*2+($74)-(($5EFC)*2)))
    .define T4_DEF_077 ((($1C*10)/5)-($1C))
    .define T4_DEF_078 ((($6DEA)-($1C))+(T4_DEF_077))
    .define T4_DEF_079 (($7B*2)-(($7B)-0))
    .define T4_DEF_080 ((T4_DEF_079)+(($A9)-(($7B+3)-3)))
    .define T4_DEF_081 ((($E8+11)-4)-7)
    .define T4_DEF_082 ((T4_DEF_081)+(($B1)-($E8)))
    .define T4_DEF_083 ((($6F+3)-1)-2)
    .define T4_DEF_084 (((T4_DEF_083)+($1F00))-($6F))
    .define T4_DEF_085 (($10+5)-5)
    .define T4_DEF_086 (((T4_DEF_085)*2+($08)-(($10)*2)))
    .define T4_DEF_087 (($7894-7)+7)
    .define T4_DEF_088 ((($25)-($7894))+(T4_DEF_087))
    .define T4_DEF_089 ((($DB*6)/3)-($DB))
    .define T4_DEF_090 ((T4_DEF_089)+(($7783)-(($DB+3)-3)))
    .define T4_DEF_091 ((($1678*10)/5)-($1678))
    .define T4_DEF_092 ((T4_DEF_091)+(($1DEF)-($1678)))
    .define T4_DEF_093 (($99*2)-(($99)-0))
    .define T4_DEF_094 (((T4_DEF_093)+($3F))-($99))
    .define T4_DEF_095 ((($E5+11)-4)-7)
    .define T4_DEF_096 (((T4_DEF_095)*2+($06)-(($E5)*2)))
    .define T4_DEF_097 ((($1A+3)-1)-2)
    .define T4_DEF_098 ((($05)-($1A))+(T4_DEF_097))
    .define T4_DEF_099 (($48+5)-5)
    .define T4_DEF_100 ((T4_DEF_099)+(($7450)-(($48+3)-3)))
    .define T4_DEF_101 (($2DEE-7)+7)
    .define T4_DEF_102 ((T4_DEF_101)+(($75)-($2DEE)))
    .define T4_DEF_103 ((($6E*6)/3)-($6E))
    .define T4_DEF_104 (((T4_DEF_103)+($7AB6))-($6E))
    .define T4_DEF_105 ((($55*10)/5)-($55))
    .define T4_DEF_106 (((T4_DEF_105)*2+($4AB9)-(($55)*2)))
    .define T4_DEF_107 (($C7*2)-(($C7)-0))
    .define T4_DEF_108 ((($DE)-($C7))+(T4_DEF_107))
    .define T4_DEF_109 ((($4453+11)-4)-7)
    .define T4_DEF_110 ((T4_DEF_109)+(($833E)-(($4453+3)-3)))
    .define T4_DEF_111 ((($44+3)-1)-2)
    .define T4_DEF_112 ((T4_DEF_111)+(($90)-($44)))
    .define T4_DEF_113 (($FF+5)-5)
    .define T4_DEF_114 (((T4_DEF_113)+($500E))-($FF))
    .define T4_DEF_115 (($3343-7)+7)
    .define T4_DEF_116 (((T4_DEF_115)*2+($4675)-(($3343)*2)))
    .define T4_DEF_117 ((($CA*6)/3)-($CA))
    .define T4_DEF_118 ((($21)-($CA))+(T4_DEF_117))
    .define T4_DEF_119 ((($6AB7*10)/5)-($6AB7))
    .define T4_DEF_120 ((T4_DEF_119)+(($CD)-(($6AB7+3)-3)))
    .define T4_DEF_121 (($5341*2)-(($5341)-0))
    .define T4_DEF_122 ((T4_DEF_121)+(($844F)-($5341)))
    .define T4_DEF_123 ((($611E+11)-4)-7)
    .define T4_DEF_124 (((T4_DEF_123)+($8893))-($611E))
    .define T4_DEF_125 ((($7DE9+3)-1)-2)
    .define T4_DEF_126 (((T4_DEF_125)*2+($E2)-(($7DE9)*2)))
    .define T4_DEF_127 (($4EFD+5)-5)
    .define T4_DEF_128 ((($4A)-($4EFD))+(T4_DEF_127))
    .define T4_DEF_129 (($1F-7)+7)
    .define T4_DEF_130 ((T4_DEF_129)+(($E0)-(($1F+3)-3)))
    .define T4_DEF_131 ((($C1*6)/3)-($C1))
    .define T4_DEF_132 ((T4_DEF_131)+(($0C)-($C1)))
    .define T4_DEF_133 ((($AE*10)/5)-($AE))
    .define T4_DEF_134 (((T4_DEF_133)+($12))-($AE))
    .define T4_DEF_135 (($71*2)-(($71)-0))
    .define T4_DEF_136 (((T4_DEF_135)*2+($0B)-(($71)*2)))
    .define T4_DEF_137 ((($1789+11)-4)-7)
    .define T4_DEF_138 ((($6D)-($1789))+(T4_DEF_137))
    .define T4_DEF_139 ((($32+3)-1)-2)
    .define T4_DEF_140 ((T4_DEF_139)+(($0F)-(($32+3)-3)))
    .define T4_DEF_141 (($722E+5)-5)
    .define T4_DEF_142 ((T4_DEF_141)+(($50)-($722E)))
    .define T4_DEF_143 (($C8-7)+7)
    .define T4_DEF_144 (((T4_DEF_143)+($26))-($C8))
    .define T4_DEF_145 ((($7F*6)/3)-($7F))
    .define T4_DEF_146 (((T4_DEF_145)*2+($2788)-(($7F)*2)))
    .define T4_DEF_147 ((($C4*10)/5)-($C4))
    .define T4_DEF_148 ((($F9)-($C4))+(T4_DEF_147))
    .define T4_DEF_149 (($811C*2)-(($811C)-0))
    .define T4_DEF_150 ((T4_DEF_149)+(($3121)-(($811C+3)-3)))
    .define T4_DEF_151 ((($3A+11)-4)-7)
    .define T4_DEF_152 ((T4_DEF_151)+(($64)-($3A)))
    .define T4_DEF_153 ((($4BCA+3)-1)-2)
    .define T4_DEF_154 (((T4_DEF_153)+($86))-($4BCA))
    .define T4_DEF_155 (($A0+5)-5)
    .define T4_DEF_156 (((T4_DEF_155)*2+($3898)-(($A0)*2)))
    .define T4_DEF_157 (($20-7)+7)
    .define T4_DEF_158 ((($4F)-($20))+(T4_DEF_157))
    .define T4_DEF_159 ((($2566*6)/3)-($2566))
    .define T4_DEF_160 ((T4_DEF_159)+(($2CDD)-(($2566+3)-3)))
    .define T4_DEF_161 ((($B7*10)/5)-($B7))
    .define T4_DEF_162 ((T4_DEF_161)+(($9A)-($B7)))
    .define T4_DEF_163 (($D9*2)-(($D9)-0))
    .define T4_DEF_164 (((T4_DEF_163)+($7EFA))-($D9))
    .define T4_DEF_165 ((($6562+11)-4)-7)
    .define T4_DEF_166 (((T4_DEF_165)*2+($BE)-(($6562)*2)))
    .define T4_DEF_167 ((($B0+3)-1)-2)
    .define T4_DEF_168 ((($C0)-($B0))+(T4_DEF_167))
    .define T4_DEF_169 (($5896+5)-5)
    .define T4_DEF_170 ((T4_DEF_169)+(($7561)-(($5896+3)-3)))
    .db "04>"               ; @BT TEST-04 04 START

    ; GENERATED: copied from TEST-03 with varied define-based operand replacement
    ; GENERATED: replaced operands = 170 / 340

    ; GENERATED: copied from TEST-02 with varied arithmetic expressions for numeric operands

    ; GENERATED: copied from TEST-01 with full TEST-02 randomization
    ; GENERATED: seed = 35714263

    SAY                            ; @BT 42
    STY T4_DEF_001+5-5                        ; @BT 84 4E
    TST #T4_DEF_002-7+7,$80*6/3-$80                   ; @BT 83 5F 80
    DEY                            ; @BT 88
    RMB5 T4_DEF_003*10/5-$E4                       ; @BT 57 E4
    LSR.W T4_DEF_004*2-$5BC9+0,X                  ; @BT 5E C9 5B
    LSR.B T4_DEF_005+11-4-7,X                    ; @BT 56 77
    TYA                            ; @BT 98
    BRA.B T4_DEF_006+3-1-2                      ; @BT 80 51
    CMP #T4_DEF_007+5-5                       ; @BT C9 89
    BVS T4_DEF_008-7+7                        ; @BT 70 3D
    BIT.W T4_DEF_009*6/3-$2677,X                  ; @BT 3C 77 26
    BBS6 T4_DEF_010*10/5-$EE,$05*2-$05+0                   ; @BT EF EE 05
    CMP T4_DEF_011+11-4-7,Y                    ; @BT D9 AA 29
    CLY                            ; @BT C2
    STY T4_DEF_012+3-1-2                      ; @BT 8C C7 7B
    CPY.W T4_DEF_013+5-5                    ; @BT CC 32 32
    AND.B #T4_DEF_014-7+7                     ; @BT 29 C6
    STA.W T4_DEF_015*6/3-$7672,X                  ; @BT 9D 72 76
    ORA (T4_DEF_016*10/5-$BC,X)                    ; @BT 01 BC
    DEC.W T4_DEF_017*2-$3676+0                    ; @BT CE 76 36
    ADC T4_DEF_018+11-4-7,X                      ; @BT 75 84
    AND.B T4_DEF_019+3-1-2                      ; @BT 25 39
    TSB T4_DEF_020+5-5                        ; @BT 04 03
    LDA.B T4_DEF_021-7+7                      ; @BT A5 1E
    SBC T4_DEF_022*6/3-$54                        ; @BT E5 54
    SBC.W T4_DEF_023*10/5-$711D,Y                  ; @BT F9 1D 71
    SBC.W T4_DEF_024*2-$700C+0,X                  ; @BT FD 0C 70
    JMP.W T4_DEF_025+11-4-7                    ; @BT 4C 97 48
    AND T4_DEF_026+3-1-2                      ; @BT 2D CD 1B
    ROR T4_DEF_027+5-5                        ; @BT 66 2C
    LDY.W T4_DEF_028-7+7,X                  ; @BT BC 85 57
    BRK                            ; @BT 00 00
    ROR                            ; @BT 6A
    EOR.B (T4_DEF_029*6/3-$69),Y                  ; @BT 51 69
    AND.B T4_DEF_030*10/5-$22,X                    ; @BT 35 22
    SXY                            ; @BT 02
    ROL T4_DEF_031*2-$D0+0                        ; @BT 26 D0
    BVS.B T4_DEF_032+11-4-7                      ; @BT 70 23
    ORA.B (T4_DEF_033+3-1-2),Y                  ; @BT 11 5D
    EOR T4_DEF_034+5-5,X                    ; @BT 5D 87 37
    AND T4_DEF_035-7+7,X                    ; @BT 3D AB 19
    LDA T4_DEF_036*6/3-$7D,X                      ; @BT B5 7D
    BBS2 T4_DEF_037*10/5-$92,$29*2-$29+0                   ; @BT AF 92 29
    BBR1 T4_DEF_038+11-4-7,$5A+3-1-2                   ; @BT 1F C3 5A
    SMB2 T4_DEF_039+5-5                       ; @BT A7 57
    LDY T4_DEF_040-7+7,X                      ; @BT B4 D6
    ADC T4_DEF_041*6/3-$1567                      ; @BT 6D 67 15
    STA.B T4_DEF_042*10/5-$AD,X                    ; @BT 95 AD
    ASL T4_DEF_043*2-$2122+0                      ; @BT 0E 22 21
    BNE.B T4_DEF_044+11-4-7                      ; @BT D0 5E
    BVC.B T4_DEF_045+3-1-2                      ; @BT 50 0C
    CPY.B T4_DEF_046+5-5                      ; @BT C4 3E
    AND T4_DEF_047-7+7,Y                    ; @BT 39 BC 1A
    ADC.W T4_DEF_048*6/3-$189A                    ; @BT 6D 9A 18
    CSH                            ; @BT D4
    SBC.B #T4_DEF_049*10/5-$6B                     ; @BT E9 6B
    STA (T4_DEF_050*2-$23+0)                      ; @BT 92 23
    ADC.B #T4_DEF_051+11-4-7                     ; @BT 69 B2
    BRA T4_DEF_052+3-1-2                        ; @BT 80 6B
    ADC.B T4_DEF_053+5-5,X                    ; @BT 75 0E
    LDA T4_DEF_054-7+7                        ; @BT A5 94
    AND T4_DEF_055*6/3-$AF                        ; @BT 25 AF
    LDX.B T4_DEF_056*10/5-$91,Y                    ; @BT B6 91
    SBC #T4_DEF_057*2-$E1+0                       ; @BT E9 E1
    LSR T4_DEF_058+11-4-7,X                    ; @BT 5E A7 59
    STX T4_DEF_059+3-1-2                      ; @BT 8E A5 79
    LDY T4_DEF_060+5-5                      ; @BT AC 74 56
    LDA.B T4_DEF_061-7+7,X                    ; @BT B5 07
    CPY.B #T4_DEF_062*6/3-$27                     ; @BT C0 27
    ASL A                          ; @BT 0A
    BIT.B T4_DEF_063*10/5-$2D,X                    ; @BT 34 2D
    DEC.W T4_DEF_064*2-$3565+0,X                  ; @BT DE 65 35
    ROL T4_DEF_065+11-4-7,X                      ; @BT 36 B9
    BIT T4_DEF_066+3-1-2                        ; @BT 24 16
    EOR.W T4_DEF_067+5-5                    ; @BT 4D DC 3C
    ORA.W T4_DEF_068-7+7                    ; @BT 0D 40 63
    INC T4_DEF_069*6/3-$C5,X                      ; @BT F6 C5
    TMA.B #T4_DEF_070*10/5-$A7                     ; @BT 43 A7
    STA T4_DEF_071*2-$3A+0,X                      ; @BT 95 3A
    STZ T4_DEF_072+11-4-7,X                      ; @BT 74 93
    STA.B (T4_DEF_073+3-1-2,X)                  ; @BT 81 68
    EOR.W T4_DEF_074+5-5,Y                  ; @BT 59 CB 3B
    ORA T4_DEF_075-7+7,Y                    ; @BT 19 FC 5E
    ORA.B (T4_DEF_076*6/3-$74)                    ; @BT 12 74
    BCC T4_DEF_077*10/5-$1C                        ; @BT 90 1C
    INA                            ; @BT 1A
    SBC T4_DEF_078*2-$6DEA+0,Y                    ; @BT F9 EA 6D
    BBS1 T4_DEF_079+11-4-7,$12+3-1-2                   ; @BT 9F 7B 12
    BBS3 T4_DEF_080+5-5,$40-7+7                   ; @BT BF A9 40
    BIT #T4_DEF_081*6/3-$E8                       ; @BT 89 E8
    EOR #T4_DEF_082*10/5-$B1                       ; @BT 49 B1
    CMP.B T4_DEF_083*2-$6F+0,X                    ; @BT D5 6F
    AND.W T4_DEF_084+11-4-7                    ; @BT 2D 00 1F
    CPY T4_DEF_085+3-1-2                        ; @BT C4 10
    PHX                            ; @BT DA
    BBR4 T4_DEF_086+5-5,$1F-7+7                   ; @BT 4F 08 1F
    STA.W T4_DEF_087*6/3-$7894                    ; @BT 8D 94 78
    ADC.B T4_DEF_088*10/5-$25                      ; @BT 65 25
    STX T4_DEF_089*2-$DB+0,Y                      ; @BT 96 DB
    STA.W T4_DEF_090+11-4-7,Y                  ; @BT 99 83 77
    DEX                            ; @BT CA
    ADC.W T4_DEF_091+3-1-2,X                  ; @BT 7D 78 16
    AND.W T4_DEF_092+5-5,Y                  ; @BT 39 EF 1D
    SBC.B (T4_DEF_093-7+7),Y                  ; @BT F1 99
    ADC (T4_DEF_094*6/3-$3F,X)                    ; @BT 61 3F
    CMP T4_DEF_095*10/5-$E5,X                      ; @BT D5 E5
    ST1 #T4_DEF_096*2-$06+0                       ; @BT 13 06
    TSB.B T4_DEF_097+11-4-7                      ; @BT 04 1A
    CSL                            ; @BT 54
    TSX                            ; @BT BA
    BBS7 T4_DEF_098+3-1-2,$1C+5-5                   ; @BT FF 05 1C
    SED                            ; @BT F8
    TST #T4_DEF_099-7+7,$8BC6*6/3-$8BC6,X               ; @BT B3 48 C6 8B
    STA T4_DEF_100*10/5-$7450,Y                    ; @BT 99 50 74
    CMP.W T4_DEF_101*2-$2DEE+0                    ; @BT CD EE 2D
    BPL.B T4_DEF_102+11-4-7                      ; @BT 10 75
    SMB3 T4_DEF_103+3-1-2                       ; @BT B7 6E
    STX.W T4_DEF_104+5-5                    ; @BT 8E B6 7A
    PLX                            ; @BT FA
    DEC T4_DEF_105-7+7,X                      ; @BT D6 55
    ROL                            ; @BT 2A
    LDA T4_DEF_106*6/3-$4AB9,X                    ; @BT BD B9 4A
    SBC.B T4_DEF_107*10/5-$C7,X                    ; @BT F5 C7
    SBC.B T4_DEF_108*2-$DE+0                      ; @BT E5 DE
    JMP T4_DEF_109+11-4-7                      ; @BT 4C 53 44
    TRB.W T4_DEF_110+3-1-2                    ; @BT 1C 3E 83
    BIT.B T4_DEF_111+5-5                      ; @BT 24 44
    TAM #T4_DEF_112-7+7                       ; @BT 53 90
    BIT T4_DEF_113*6/3-$FF,X                      ; @BT 34 FF
    LDA.W T4_DEF_114*10/5-$500E                    ; @BT AD 0E 50
    DEC T4_DEF_115*2-$3343+0,X                    ; @BT DE 43 33
    JMP.W (T4_DEF_116+11-4-7,X)                ; @BT 7C 75 46
    TXA                            ; @BT 8A
    SMB7 T4_DEF_117+3-1-2                       ; @BT F7 CA
    LDA #T4_DEF_118+5-5                       ; @BT A9 21
    ROR.W T4_DEF_119-7+7,X                  ; @BT 7E B7 6A
    RMB4 T4_DEF_120*6/3-$CD                       ; @BT 47 CD
    LDX.W T4_DEF_121*10/5-$5341,Y                  ; @BT BE 41 53
    TSB T4_DEF_122*2-$844F+0                      ; @BT 0C 4F 84
    ORA.W T4_DEF_123+11-4-7,X                  ; @BT 1D 1E 61
    TIN T4_DEF_124+3-1-2,$89A4+5-5,$8AB5-7+7          ; @BT D3 93 88 A4 89 B5 8A
    STZ T4_DEF_125*6/3-$7DE9,X                    ; @BT 9E E9 7D
    CPX.B T4_DEF_126*10/5-$E2                      ; @BT E4 E2
    LDA.W T4_DEF_127*2-$4EFD+0,Y                  ; @BT B9 FD 4E
    BEQ T4_DEF_128+11-4-7                        ; @BT F0 4A
    BBR5 T4_DEF_129+3-1-2,$36+5-5                   ; @BT 5F 1F 36
    ADC.B (T4_DEF_130-7+7),Y                  ; @BT 71 E0
    PLA                            ; @BT 68
    STZ.B T4_DEF_131*6/3-$C1,X                    ; @BT 74 C1
    STA (T4_DEF_132*10/5-$0C),Y                    ; @BT 91 0C
    EOR.B T4_DEF_133*2-$AE+0                      ; @BT 45 AE
    INX                            ; @BT E8
    RMB7 T4_DEF_134+11-4-7                       ; @BT 77 12
    CLA                            ; @BT 62
    RMB0 T4_DEF_135+3-1-2                       ; @BT 07 71
    AND.B (T4_DEF_136+5-5)                    ; @BT 32 0B
    PLY                            ; @BT 7A
    ADC.W T4_DEF_137-7+7,Y                  ; @BT 79 89 17
    ADC (T4_DEF_138*6/3-$6D)                      ; @BT 72 6D
    LDY.B T4_DEF_139*10/5-$32                      ; @BT A4 32
    BPL T4_DEF_140*2-$0F+0                        ; @BT 10 0F
    SBC.W T4_DEF_141+11-4-7                    ; @BT ED 2E 72
    ASL T4_DEF_142+3-1-2,X                      ; @BT 16 50
    EOR (T4_DEF_143+5-5,X)                    ; @BT 41 C8
    PHP                            ; @BT 08
    SBC (T4_DEF_144-7+7)                      ; @BT F2 26
    STA.B (T4_DEF_145*6/3-$7F),Y                  ; @BT 91 7F
    BIT.W T4_DEF_146*10/5-$2788                    ; @BT 2C 88 27
    STA.B T4_DEF_147*2-$C4+0                      ; @BT 85 C4
    CPY #T4_DEF_148+11-4-7                       ; @BT C0 F9
    STZ.W T4_DEF_149+3-1-2                    ; @BT 9C 1C 81
    CPY T4_DEF_150+5-5                      ; @BT CC 21 31
    BSR.B T4_DEF_151-7+7                      ; @BT 44 3A
    BBS0 T4_DEF_152*6/3-$64,$7B*10/5-$7B                   ; @BT 8F 64 7B
    LDA T4_DEF_153*2-$4BCA+0,Y                    ; @BT B9 CA 4B
    CMP.B T4_DEF_154+11-4-7                      ; @BT C5 86
    CMP (T4_DEF_155+3-1-2,X)                    ; @BT C1 A0
    EOR T4_DEF_156+5-5,Y                    ; @BT 59 98 38
    DEA                            ; @BT 3A
    STX.B T4_DEF_157-7+7                      ; @BT 86 20
    LDA (T4_DEF_158*6/3-$4F),Y                    ; @BT B1 4F
    BIT T4_DEF_159*10/5-$2566                      ; @BT 2C 66 25
    CMP.W T4_DEF_160*2-$2CDD+0,Y                  ; @BT D9 DD 2C
    CMP (T4_DEF_161+11-4-7),Y                    ; @BT D1 B7
    DEC.B T4_DEF_162+3-1-2                      ; @BT C6 9A
    LDA.B (T4_DEF_163+5-5),Y                  ; @BT B1 D9
    STZ T4_DEF_164-7+7                      ; @BT 9C FA 7E
    ROL T4_DEF_165*6/3-$6562                      ; @BT 2E 62 65
    TAM.B #T4_DEF_166*10/5-$BE                     ; @BT 53 BE
    SBC.B (T4_DEF_167*2-$B0+0)                    ; @BT F2 B0
    BBS4 T4_DEF_168+11-4-7,$57+3-1-2                   ; @BT CF C0 57
    LDY.W T4_DEF_169+5-5                    ; @BT AC 96 58
    STA T4_DEF_170-7+7                      ; @BT 8D 61 75
    INC $3DED*6/3-$3DED,X                    ; @BT FE ED 3D
    ORA ($D3*10/5-$D3),Y                    ; @BT 11 D3
    AND.B ($DD*2-$DD+0,X)                  ; @BT 21 DD
    ROL.B $E7+11-4-7,X                    ; @BT 36 E7
    STA $51+3-1-2                        ; @BT 85 51
    CPX $B4+5-5                        ; @BT E4 B4
    EOR $39A9-7+7                      ; @BT 4D A9 39
    CLD                            ; @BT D8
    ORA $18*6/3-$18                        ; @BT 05 18
    SBC ($F8*10/5-$F8,X)                    ; @BT E1 F8
    CMP ($CE*2-$CE+0)                      ; @BT D2 CE
    ASL.B $7E+11-4-7,X                    ; @BT 16 7E
    AND.B ($F4+3-1-2),Y                  ; @BT 31 F4
    CLC                            ; @BT 18
    ADC #$28+5-5                       ; @BT 69 28
    ORA.B $8B-7+7,X                    ; @BT 15 8B
    EOR.B ($80*6/3-$80)                    ; @BT 52 80
    LDX $4C*10/5-$4C,Y                      ; @BT B6 4C
    JMP ($4342*2-$4342+0)                    ; @BT 6C 42 43
    STY $37+11-4-7,X                      ; @BT 94 37
    SMB5 $9C+3-1-2                       ; @BT D7 9C
    ORA #$A5+5-5                       ; @BT 09 A5
    ORA $600D-7+7                      ; @BT 0D 0D 60
    ST2 #$1D*6/3-$1D                       ; @BT 23 1D
    STX.B $09*10/5-$09,Y                    ; @BT 96 09
    ROL.W $6784*2-$6784+0                    ; @BT 2E 84 67
    LDA.B ($F0+11-4-7)                    ; @BT B2 F0
    CMP.B ($2A+3-1-2,X)                  ; @BT C1 2A
    SEC                            ; @BT 38
    BBS5 $D7+5-5,$6E-7+7                   ; @BT DF D7 6E
    CPX #$9D*6/3-$9D                       ; @BT E0 9D
    CPX.W $3010*10/5-$3010                    ; @BT EC 10 30
    AND #$3C*2-$3C+0                       ; @BT 29 3C
    ROR.W $6BC8+11-4-7                    ; @BT 6E C8 6B
    INC.B $F3+3-1-2,X                    ; @BT F6 F3
    TSB.W $8560+5-5                    ; @BT 0C 60 85
    STA $733F-7+7,X                    ; @BT 9D 3F 73
    TST #$76*6/3-$76,$8CD7*10/5-$8CD7                 ; @BT 93 76 D7 8C
    BEQ.B $30*2-$30+0                      ; @BT F0 30
    BBR2 $DA+11-4-7,$71+3-1-2                   ; @BT 2F DA 71
    EOR ($DF+5-5),Y                    ; @BT 51 DF
    CPX.B #$CB-7+7                     ; @BT E0 CB
    LDY.B $1B*6/3-$1B,X                    ; @BT B4 1B
    ORA.B #$2F*10/5-$2F                     ; @BT 09 2F
    BIT $2455*2-$2455+0,X                    ; @BT 3C 55 24
    TST #$31+11-4-7,$52+3-1-2,X                 ; @BT A3 31 52
    BVC $26+5-5                        ; @BT 50 26
    ORA.B ($46-7+7,X)                  ; @BT 01 46
    RMB6 $FB*6/3-$FB                       ; @BT 67 FB
    ORA.W $622F*10/5-$622F,Y                  ; @BT 19 2F 62
    SBC ($0F*2-$0F+0),Y                    ; @BT F1 0F
    JSR $4564+11-4-7                      ; @BT 20 64 45
    CLX                            ; @BT 82
    STZ $AA+3-1-2                        ; @BT 64 AA
    TAI $8AB5+5-5,$8BC6-7+7,$8CD7*6/3-$8CD7          ; @BT F3 B5 8A C6 8B D7 8C
    LDA ($66*10/5-$66)                      ; @BT B2 66
    TRB $D5*2-$D5+0                        ; @BT 14 D5
    TDD $8782+11-4-7,$8893+3-1-2,$89A4+5-5          ; @BT C3 82 87 93 88 A4 89
    TII $8671-7+7,$8782*6/3-$8782,$8893*10/5-$8893          ; @BT 73 71 86 82 87 93 88
    BNE $78*2-$78+0                        ; @BT D0 78
    INC $3EFE+11-4-7                      ; @BT EE FE 3E
    CMP $2ABB+3-1-2                      ; @BT CD BB 2A
    TRB $822D+5-5                      ; @BT 1C 2D 82
    BRK $5B-7+7                        ; @BT 00 5B
    AND ($6A*6/3-$6A),Y                    ; @BT 31 6A
    SMB4 $85*10/5-$85                       ; @BT C7 85
    PLP                            ; @BT 28
    BBR7 $4D*2-$4D+0,$64+11-4-7                   ; @BT 7F 4D 64
    EOR.W $3ABA+3-1-2,X                  ; @BT 5D BA 3A
    LDA.B #$AB+5-5                     ; @BT A9 AB
    CLV                            ; @BT B8
    ROL.B $FE-7+7                      ; @BT 26 FE
    INY                            ; @BT C8
    SBC $3D*6/3-$3D,X                      ; @BT F5 3D
    ADC $9B*10/5-$9B                        ; @BT 65 9B
    BCC.B $02*2-$02+0                      ; @BT 90 02
    LSR A                          ; @BT 4A
    STY.B $65+11-4-7,X                    ; @BT 94 65
    ASL                            ; @BT 0A
    BRK.B $72+3-1-2                      ; @BT 00 72
    LDY.B #$04+5-5                     ; @BT A0 04
    ROR $69A6-7+7                      ; @BT 6E A6 69
    ROR $6895*6/3-$6895,X                    ; @BT 7E 95 68
    ST0 #$EF*10/5-$EF                       ; @BT 03 EF
    ORA $5DEB*2-$5DEB+0,X                    ; @BT 1D EB 5D
    LSR $5AB8+11-4-7                      ; @BT 4E B8 5A
    LDA.B ($C2+3-1-2,X)                  ; @BT A1 C2
    LDX.B $A8+5-5                      ; @BT A6 A8
    ST1.B #$4B-7+7                     ; @BT 13 4B
    CMP.W $2BCC*6/3-$2BCC,X                  ; @BT DD CC 2B
    ASL.W $2233*10/5-$2233,X                  ; @BT 1E 33 22
    ASL $2011*2-$2011+0,X                    ; @BT 1E 11 20
    LDX.B #$7A+11-4-7                     ; @BT A2 7A
    ROL $6451+3-1-2,X                    ; @BT 3E 51 64
    LDA ($38+5-5,X)                    ; @BT A1 38
    SMB6 $B3-7+7                       ; @BT E7 B3
    SET                            ; @BT F4
    SAX                            ; @BT 22
    AND $98*6/3-$98,X                      ; @BT 35 98
    STA ($F5*10/5-$F5,X)                    ; @BT 81 F5
    STZ.B $D8*2-$D8+0                      ; @BT 64 D8
    LSR $49+11-4-7,X                      ; @BT 56 49
    INC $DC+3-1-2                        ; @BT E6 DC
    ADC.B ($C9+5-5,X)                  ; @BT 61 C9
    SBC.B ($82-7+7,X)                  ; @BT E1 82
    DEC $3454*6/3-$3454                      ; @BT CE 54 34
    ADC $1345*10/5-$1345,X                    ; @BT 7D 45 13
    STX $F2*2-$F2+0                        ; @BT 86 F2
    RTI                            ; @BT 40
    INC.B $0A+11-4-7                      ; @BT E6 0A
    LDX #$35+3-1-2                       ; @BT A2 35
    BBR3 $F1+5-5,$08-7+7                   ; @BT 3F F1 08
    RTS                            ; @BT 60
    STA.B ($96*6/3-$96)                    ; @BT 92 96
    LSR $60*10/5-$60                        ; @BT 46 60
    LSR                            ; @BT 4A
    ROR $15*2-$15+0,X                      ; @BT 76 15
    BCS.B $19+11-4-7                      ; @BT B0 19
    DEC $6C+3-1-2                        ; @BT C6 6C
    PHA                            ; @BT 48
    LDX $511F+5-5,Y                    ; @BT BE 1F 51
    ADC.B ($F7-7+7)                    ; @BT 72 F7
    TRB.B $EC*6/3-$EC                      ; @BT 14 EC
    SEI                            ; @BT 78
    DEC.B $83*10/5-$83,X                    ; @BT D6 83
    PHY                            ; @BT 5A
    ADC ($56*2-$56+0),Y                    ; @BT 71 56
    LSR.B $8E+11-4-7                      ; @BT 46 8E
    ASL $67+3-1-2                        ; @BT 06 67
    BCS $33+5-5                        ; @BT B0 33
    NOP                            ; @BT EA
    ROL.W $6673-7+7,X                  ; @BT 3E 73 66
    ASL.W $2344*6/3-$2344                    ; @BT 0E 44 23
    ORA.B $A2*10/5-$A2                      ; @BT 05 A2
    LSR.W $5CDA*2-$5CDA+0                    ; @BT 4E DA 5C
    LDX $5230+11-4-7                      ; @BT AE 30 52
    ROL A                          ; @BT 2A
    AND.W $1CDE+3-1-2,X                  ; @BT 3D DE 1C
    BMI $61+5-5                        ; @BT 30 61
    ST0.B #$34-7+7                     ; @BT 03 34
    STY.B $7C*6/3-$7C                      ; @BT 84 7C
    CLI                            ; @BT 58
    EOR ($F6*10/5-$F6)                      ; @BT 52 F6
    EOR $0D*2-$0D+0,X                      ; @BT 55 0D
    LDX.W $5452+11-4-7                    ; @BT AE 52 54
    TXS                            ; @BT 9A
    CMP.B ($58+3-1-2)                    ; @BT D2 58
    RMB3 $B6+5-5                       ; @BT 37 B6
    ADC $1456-7+7,Y                    ; @BT 79 56 14
    INC.W $400F*6/3-$400F,X                  ; @BT FE 0F 40
    ROR.B $5A*10/5-$5A                      ; @BT 66 5A
    CMP $2899*2-$2899+0,X                    ; @BT DD 99 28
    CMP.B ($41+11-4-7),Y                  ; @BT D1 41
    TAY                            ; @BT A8
    CPX $2EFF+3-1-2                      ; @BT EC FF 2E
    AND ($81+5-5)                      ; @BT 32 81
    SBC $6CD9-7+7,X                    ; @BT FD D9 6C
    INC.W $4120*6/3-$4120                    ; @BT EE 20 41
    SMB1 $40*10/5-$40                       ; @BT 97 40
    STY.W $7CD8*2-$7CD8+0                    ; @BT 8C D8 7C
    JSR.W $49A8+11-4-7                    ; @BT 20 A8 49
    LDX $63+3-1-2                        ; @BT A6 63
    EOR.B $97+5-5,X                    ; @BT 55 97
    TIA $89A4-7+7,$8AB5*6/3-$8AB5,$8BC6*10/5-$8BC6          ; @BT E3 A4 89 B5 8A C6 8B
    BMI.B $47*2-$47+0                      ; @BT 30 47
    RMB1 $88+11-4-7                       ; @BT 17 88
    EOR.B #$3B+3-1-2                     ; @BT 49 3B
    ASL.B $95+5-5                      ; @BT 06 95
    ORA $01-7+7,X                      ; @BT 15 01
    LDA.W $4DEC*6/3-$4DEC,X                  ; @BT BD EC 4D
    BBR6 $36*10/5-$36,$4D*2-$4D+0                   ; @BT 6F 36 4D
    EOR $24+11-4-7                        ; @BT 45 24
    AND ($53+3-1-2,X)                    ; @BT 21 53
    EOR.B ($52+5-5,X)                  ; @BT 41 52
    STZ $800B-7+7,X                    ; @BT 9E 0B 80
    LDY $5563*6/3-$5563,X                    ; @BT BC 63 55
    TAX                            ; @BT AA
    ST2.B #$62*10/5-$62                     ; @BT 23 62
    LDY $ED*2-$ED+0                        ; @BT A4 ED
    SBC $6EFB+11-4-7                      ; @BT ED FB 6E
    TMA #$79+3-1-2                       ; @BT 43 79
    LDY #$BF+5-5                       ; @BT A0 BF
    JMP ($4231-7+7,X)                  ; @BT 7C 31 42
    ORA ($EA*6/3-$EA)                      ; @BT 12 EA
    BBR0 $AC*10/5-$AC,$43*2-$43+0                   ; @BT 0F AC 43
    CMP.B #$13+11-4-7                     ; @BT C9 13
    ROR.B $43+3-1-2,X                    ; @BT 76 43
    CMP $FC+5-5                        ; @BT C5 FC
    JMP.W ($4786-7+7)                  ; @BT 6C 86 47
    ROR A                          ; @BT 6A
    SMB0 $29*6/3-$29                       ; @BT 87 29
    BSR $54*10/5-$54                        ; @BT 44 54
    RMB2 $9F*2-$9F+0                       ; @BT 27 9F
    LDA $4CDB+11-4-7                      ; @BT AD DB 4C

    .db "<04"               ; @BT END

    /////////////////////////////////////////////////////////////////////////////
    // TEST-05: similar to TEST-04 and defines + functions + expressions
    /////////////////////////////////////////////////////////////////////////////





    ; TEST-05 generated defines/functions
    .define T5_DEF_001 (($4E+5)-5)
    .define T5_DEF_002 ((T5_DEF_001)+(($5F)-($4E)))
    .define T5_DEF_003 (($E4-7)+7)
    .define T5_DEF_004 (((T5_DEF_003)+($5BC9))-($E4))
    .define T5_DEF_005 ((($77*6)/3)-($77))
    .define T5_DEF_006 (((T5_DEF_005)*2+($51)-(($77)*2)))
    .define T5_DEF_007 ((($89*10)/5)-($89))
    .define T5_DEF_008 ((($3D)-($89))+(T5_DEF_007))
    .define T5_DEF_009 (($2677*2)-(($2677)-0))
    .define T5_DEF_010 ((T5_DEF_009)+(($EE)-(($2677+3)-3)))
    .define T5_DEF_011 ((($29AA+11)-4)-7)
    .define T5_DEF_012 ((T5_DEF_011)+(($7BC7)-($29AA)))
    .define T5_DEF_013 ((($3232+3)-1)-2)
    .define T5_DEF_014 (((T5_DEF_013)+($C6))-($3232))
    .define T5_DEF_015 (($7672+5)-5)
    .define T5_DEF_016 (((T5_DEF_015)*2+($BC)-(($7672)*2)))
    .define T5_DEF_017 (($3676-7)+7)
    .define T5_DEF_018 ((($84)-($3676))+(T5_DEF_017))
    .define T5_DEF_019 ((($39*6)/3)-($39))
    .define T5_DEF_020 ((T5_DEF_019)+(($03)-(($39+3)-3)))
    .define T5_DEF_021 ((($1E*10)/5)-($1E))
    .define T5_DEF_022 ((T5_DEF_021)+(($54)-($1E)))
    .define T5_DEF_023 (($711D*2)-(($711D)-0))
    .define T5_DEF_024 (((T5_DEF_023)+($700C))-($711D))
    .define T5_DEF_025 ((($4897+11)-4)-7)
    .define T5_DEF_026 (((T5_DEF_025)*2+($1BCD)-(($4897)*2)))
    .define T5_DEF_027 ((($2C+3)-1)-2)
    .define T5_DEF_028 ((($5785)-($2C))+(T5_DEF_027))
    .define T5_DEF_029 (($69+5)-5)
    .define T5_DEF_030 ((T5_DEF_029)+(($22)-(($69+3)-3)))
    .define T5_DEF_031 (($D0-7)+7)
    .define T5_DEF_032 ((T5_DEF_031)+(($23)-($D0)))
    .define T5_DEF_033 ((($5D*6)/3)-($5D))
    .define T5_DEF_034 (((T5_DEF_033)+($3787))-($5D))
    .define T5_DEF_035 ((($19AB*10)/5)-($19AB))
    .define T5_DEF_036 (((T5_DEF_035)*2+($7D)-(($19AB)*2)))
    .define T5_DEF_037 (($92*2)-(($92)-0))
    .define T5_DEF_038 ((($C3)-($92))+(T5_DEF_037))
    .define T5_DEF_039 ((($57+11)-4)-7)
    .define T5_DEF_040 ((T5_DEF_039)+(($D6)-(($57+3)-3)))
    .define T5_DEF_041 ((($1567+3)-1)-2)
    .define T5_DEF_042 ((T5_DEF_041)+(($AD)-($1567)))
    .define T5_DEF_043 (($2122+5)-5)
    .define T5_DEF_044 (((T5_DEF_043)+($5E))-($2122))
    .define T5_DEF_045 (($0C-7)+7)
    .define T5_DEF_046 (((T5_DEF_045)*2+($3E)-(($0C)*2)))
    .define T5_DEF_047 ((($1ABC*6)/3)-($1ABC))
    .define T5_DEF_048 ((($189A)-($1ABC))+(T5_DEF_047))
    .define T5_DEF_049 ((($6B*10)/5)-($6B))
    .define T5_DEF_050 ((T5_DEF_049)+(($23)-(($6B+3)-3)))
    .define T5_DEF_051 (($B2*2)-(($B2)-0))
    .define T5_DEF_052 ((T5_DEF_051)+(($6B)-($B2)))
    .define T5_DEF_053 ((($0E+11)-4)-7)
    .define T5_DEF_054 (((T5_DEF_053)+($94))-($0E))
    .define T5_DEF_055 ((($AF+3)-1)-2)
    .define T5_DEF_056 (((T5_DEF_055)*2+($91)-(($AF)*2)))
    .define T5_DEF_057 (($E1+5)-5)
    .define T5_DEF_058 ((($59A7)-($E1))+(T5_DEF_057))
    .define T5_DEF_059 (($79A5-7)+7)
    .define T5_DEF_060 ((T5_DEF_059)+(($5674)-(($79A5+3)-3)))
    .define T5_DEF_061 ((($07*6)/3)-($07))
    .define T5_DEF_062 ((T5_DEF_061)+(($27)-($07)))
    .define T5_DEF_063 ((($2D*10)/5)-($2D))
    .define T5_DEF_064 (((T5_DEF_063)+($3565))-($2D))
    .define T5_DEF_065 (($B9*2)-(($B9)-0))
    .define T5_DEF_066 (((T5_DEF_065)*2+($16)-(($B9)*2)))
    .define T5_DEF_067 ((($3CDC+11)-4)-7)
    .define T5_DEF_068 ((($6340)-($3CDC))+(T5_DEF_067))
    .define T5_DEF_069 ((($C5+3)-1)-2)
    .define T5_DEF_070 ((T5_DEF_069)+(($A7)-(($C5+3)-3)))
    .define T5_DEF_071 (($3A+5)-5)
    .define T5_DEF_072 ((T5_DEF_071)+(($93)-($3A)))
    .define T5_DEF_073 (($68-7)+7)
    .define T5_DEF_074 (((T5_DEF_073)+($3BCB))-($68))
    .define T5_DEF_075 ((($5EFC*6)/3)-($5EFC))
    .define T5_DEF_076 (((T5_DEF_075)*2+($74)-(($5EFC)*2)))
    .define T5_DEF_077 ((($1C*10)/5)-($1C))
    .define T5_DEF_078 ((($6DEA)-($1C))+(T5_DEF_077))
    .define T5_DEF_079 (($7B*2)-(($7B)-0))
    .define T5_DEF_080 ((T5_DEF_079)+(($A9)-(($7B+3)-3)))
    .define T5_DEF_081 ((($E8+11)-4)-7)
    .define T5_DEF_082 ((T5_DEF_081)+(($B1)-($E8)))
    .define T5_DEF_083 ((($6F+3)-1)-2)
    .define T5_DEF_084 (((T5_DEF_083)+($1F00))-($6F))
    .define T5_DEF_085 (($10+5)-5)
    .define T5_DEF_086 (((T5_DEF_085)*2+($08)-(($10)*2)))
    .define T5_DEF_087 (($7894-7)+7)
    .define T5_DEF_088 ((($25)-($7894))+(T5_DEF_087))
    .define T5_DEF_089 ((($DB*6)/3)-($DB))
    .define T5_DEF_090 ((T5_DEF_089)+(($7783)-(($DB+3)-3)))
    .define T5_DEF_091 ((($1678*10)/5)-($1678))
    .define T5_DEF_092 ((T5_DEF_091)+(($1DEF)-($1678)))
    .define T5_DEF_093 (($99*2)-(($99)-0))
    .define T5_DEF_094 (((T5_DEF_093)+($3F))-($99))
    .define T5_DEF_095 ((($E5+11)-4)-7)
    .define T5_DEF_096 (((T5_DEF_095)*2+($06)-(($E5)*2)))
    .define T5_DEF_097 ((($1A+3)-1)-2)
    .define T5_DEF_098 ((($05)-($1A))+(T5_DEF_097))
    .define T5_DEF_099 (($48+5)-5)
    .define T5_DEF_100 ((T5_DEF_099)+(($7450)-(($48+3)-3)))
    .define T5_DEF_101 (($2DEE-7)+7)
    .define T5_DEF_102 ((T5_DEF_101)+(($75)-($2DEE)))
    .define T5_DEF_103 ((($6E*6)/3)-($6E))
    .define T5_DEF_104 (((T5_DEF_103)+($7AB6))-($6E))
    .define T5_DEF_105 ((($55*10)/5)-($55))
    .define T5_DEF_106 (((T5_DEF_105)*2+($4AB9)-(($55)*2)))
    .define T5_DEF_107 (($C7*2)-(($C7)-0))
    .define T5_DEF_108 ((($DE)-($C7))+(T5_DEF_107))
    .define T5_DEF_109 ((($4453+11)-4)-7)
    .define T5_DEF_110 ((T5_DEF_109)+(($833E)-(($4453+3)-3)))
    .define T5_DEF_111 ((($44+3)-1)-2)
    .define T5_DEF_112 ((T5_DEF_111)+(($90)-($44)))
    .define T5_DEF_113 (($FF+5)-5)
    .define T5_DEF_114 (((T5_DEF_113)+($500E))-($FF))
    .define T5_DEF_115 (($3343-7)+7)
    .define T5_DEF_116 (((T5_DEF_115)*2+($4675)-(($3343)*2)))
    .define T5_DEF_117 ((($CA*6)/3)-($CA))
    .define T5_DEF_118 ((($21)-($CA))+(T5_DEF_117))
    .define T5_DEF_119 ((($6AB7*10)/5)-($6AB7))
    .define T5_DEF_120 ((T5_DEF_119)+(($CD)-(($6AB7+3)-3)))
    .define T5_DEF_121 (($5341*2)-(($5341)-0))
    .define T5_DEF_122 ((T5_DEF_121)+(($844F)-($5341)))
    .define T5_DEF_123 ((($611E+11)-4)-7)
    .define T5_DEF_124 (((T5_DEF_123)+($8893))-($611E))
    .define T5_DEF_125 ((($7DE9+3)-1)-2)
    .define T5_DEF_126 (((T5_DEF_125)*2+($E2)-(($7DE9)*2)))
    .define T5_DEF_127 (($4EFD+5)-5)
    .define T5_DEF_128 ((($4A)-($4EFD))+(T5_DEF_127))
    .define T5_DEF_129 (($1F-7)+7)
    .define T5_DEF_130 ((T5_DEF_129)+(($E0)-(($1F+3)-3)))
    .define T5_DEF_131 ((($C1*6)/3)-($C1))
    .define T5_DEF_132 ((T5_DEF_131)+(($0C)-($C1)))
    .define T5_DEF_133 ((($AE*10)/5)-($AE))
    .define T5_DEF_134 (((T5_DEF_133)+($12))-($AE))
    .define T5_DEF_135 (($71*2)-(($71)-0))
    .define T5_DEF_136 (((T5_DEF_135)*2+($0B)-(($71)*2)))
    .define T5_DEF_137 ((($1789+11)-4)-7)
    .define T5_DEF_138 ((($6D)-($1789))+(T5_DEF_137))
    .define T5_DEF_139 ((($32+3)-1)-2)
    .define T5_DEF_140 ((T5_DEF_139)+(($0F)-(($32+3)-3)))
    .define T5_DEF_141 (($722E+5)-5)
    .define T5_DEF_142 ((T5_DEF_141)+(($50)-($722E)))
    .define T5_DEF_143 (($C8-7)+7)
    .define T5_DEF_144 (((T5_DEF_143)+($26))-($C8))
    .define T5_DEF_145 ((($7F*6)/3)-($7F))
    .define T5_DEF_146 (((T5_DEF_145)*2+($2788)-(($7F)*2)))
    .define T5_DEF_147 ((($C4*10)/5)-($C4))
    .define T5_DEF_148 ((($F9)-($C4))+(T5_DEF_147))
    .define T5_DEF_149 (($811C*2)-(($811C)-0))
    .define T5_DEF_150 ((T5_DEF_149)+(($3121)-(($811C+3)-3)))
    .define T5_DEF_151 ((($3A+11)-4)-7)
    .define T5_DEF_152 ((T5_DEF_151)+(($64)-($3A)))
    .define T5_DEF_153 ((($4BCA+3)-1)-2)
    .define T5_DEF_154 (((T5_DEF_153)+($86))-($4BCA))
    .define T5_DEF_155 (($A0+5)-5)
    .define T5_DEF_156 (((T5_DEF_155)*2+($3898)-(($A0)*2)))
    .define T5_DEF_157 (($20-7)+7)
    .define T5_DEF_158 ((($4F)-($20))+(T5_DEF_157))
    .define T5_DEF_159 ((($2566*6)/3)-($2566))
    .define T5_DEF_160 ((T5_DEF_159)+(($2CDD)-(($2566+3)-3)))
    .define T5_DEF_161 ((($B7*10)/5)-($B7))
    .define T5_DEF_162 ((T5_DEF_161)+(($9A)-($B7)))
    .define T5_DEF_163 (($D9*2)-(($D9)-0))
    .define T5_DEF_164 (((T5_DEF_163)+($7EFA))-($D9))
    .define T5_DEF_165 ((($6562+11)-4)-7)
    .define T5_DEF_166 (((T5_DEF_165)*2+($BE)-(($6562)*2)))
    .define T5_DEF_167 ((($B0+3)-1)-2)
    .define T5_DEF_168 ((($C0)-($B0))+(T5_DEF_167))
    .define T5_DEF_169 (($5896+5)-5)
    .define T5_DEF_170 ((T5_DEF_169)+(($7561)-(($5896+3)-3)))

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
    .function T5_FN_048(a,b,c,d) ((T5_FN_047(a,b,c,d))+(T5_DEF_048-T5_DEF_048))
    .function T5_FN_049(a,b) ((((a-b)*8)/4)-((a-b)*2)+(a-b)+T5_DEF_049-T5_DEF_049)
    .function T5_FN_050(a,b) (((T5_FN_049(a,b)+3)-3)+(T5_DEF_050-T5_DEF_050))
    .function T5_FN_051(a,b,c) ((((a+b-c)*8)/4)-((a+b-c)*2)+(a+b-c)+T5_DEF_051-T5_DEF_051)
    .function T5_FN_052(a,b,c) (((T5_FN_051(a,b,c)*8)/4)-(T5_FN_051(a,b,c)*2)+T5_FN_051(a,b,c)+T5_DEF_052-T5_DEF_052)
    .function T5_FN_053(a,b,c,d) ((((a+b-c-d)*8)/4)-((a+b-c-d)*2)+(a+b-c-d)+T5_DEF_053-T5_DEF_053)
    .function T5_FN_054(a,b,c,d) (((T5_FN_053(a,b,c,d)+T5_DEF_054)-T5_DEF_054))
    .function T5_FN_055(a,b) ((((a-b)*8)/4)-((a-b)*2)+(a-b)+T5_DEF_055-T5_DEF_055)
    .function T5_FN_056(a,b) (((T5_FN_055(a,b))*2+T5_DEF_056-T5_DEF_056-T5_FN_055(a,b)))
    .function T5_FN_057(a,b,c) ((((a+b-c)*8)/4)-((a+b-c)*2)+(a+b-c)+T5_DEF_057-T5_DEF_057)
    .function T5_FN_058(a,b,c) ((T5_FN_057(a,b,c))+(T5_DEF_058-T5_DEF_058))
    .function T5_FN_059(a,b,c,d) ((((a+b-c-d)*8)/4)-((a+b-c-d)*2)+(a+b-c-d)+T5_DEF_059-T5_DEF_059)
    .function T5_FN_060(a,b,c,d) (((T5_FN_059(a,b,c,d)+3)-3)+(T5_DEF_060-T5_DEF_060))
    .function T5_FN_061(a,b) ((((a-b)*8)/4)-((a-b)*2)+(a-b)+T5_DEF_061-T5_DEF_061)
    .function T5_FN_062(a,b) (((T5_FN_061(a,b)*8)/4)-(T5_FN_061(a,b)*2)+T5_FN_061(a,b)+T5_DEF_062-T5_DEF_062)
    .function T5_FN_063(a,b,c) ((((a+b-c)*8)/4)-((a+b-c)*2)+(a+b-c)+T5_DEF_063-T5_DEF_063)
    .function T5_FN_064(a,b,c) (((T5_FN_063(a,b,c)+T5_DEF_064)-T5_DEF_064))
    .function T5_FN_065(a,b,c,d) ((((a+b-c-d)*8)/4)-((a+b-c-d)*2)+(a+b-c-d)+T5_DEF_065-T5_DEF_065)
    .function T5_FN_066(a,b,c,d) (((T5_FN_065(a,b,c,d))*2+T5_DEF_066-T5_DEF_066-T5_FN_065(a,b,c,d)))
    .function T5_FN_067(a,b) ((((a-b)*8)/4)-((a-b)*2)+(a-b)+T5_DEF_067-T5_DEF_067)
    .function T5_FN_068(a,b) ((T5_FN_067(a,b))+(T5_DEF_068-T5_DEF_068))
    .function T5_FN_069(a,b,c) ((((a+b-c)*8)/4)-((a+b-c)*2)+(a+b-c)+T5_DEF_069-T5_DEF_069)
    .function T5_FN_070(a,b,c) (((T5_FN_069(a,b,c)+3)-3)+(T5_DEF_070-T5_DEF_070))
    .function T5_FN_071(a,b,c,d) ((((a+b-c-d)*8)/4)-((a+b-c-d)*2)+(a+b-c-d)+T5_DEF_071-T5_DEF_071)
    .function T5_FN_072(a,b,c,d) (((T5_FN_071(a,b,c,d)*8)/4)-(T5_FN_071(a,b,c,d)*2)+T5_FN_071(a,b,c,d)+T5_DEF_072-T5_DEF_072)
    .function T5_FN_073(a,b) ((((a-b)*8)/4)-((a-b)*2)+(a-b)+T5_DEF_073-T5_DEF_073)
    .function T5_FN_074(a,b) (((T5_FN_073(a,b)+T5_DEF_074)-T5_DEF_074))
    .function T5_FN_075(a,b,c) ((((a+b-c)*8)/4)-((a+b-c)*2)+(a+b-c)+T5_DEF_075-T5_DEF_075)
    .function T5_FN_076(a,b,c) (((T5_FN_075(a,b,c))*2+T5_DEF_076-T5_DEF_076-T5_FN_075(a,b,c)))
    .function T5_FN_077(a,b,c,d) ((((a+b-c-d)*8)/4)-((a+b-c-d)*2)+(a+b-c-d)+T5_DEF_077-T5_DEF_077)
    .function T5_FN_078(a,b,c,d) ((T5_FN_077(a,b,c,d))+(T5_DEF_078-T5_DEF_078))
    .function T5_FN_079(a,b) ((((a-b)*8)/4)-((a-b)*2)+(a-b)+T5_DEF_079-T5_DEF_079)
    .function T5_FN_080(a,b) (((T5_FN_079(a,b)+3)-3)+(T5_DEF_080-T5_DEF_080))
    .function T5_FN_081(a,b,c) ((((a+b-c)*8)/4)-((a+b-c)*2)+(a+b-c)+T5_DEF_081-T5_DEF_081)
    .function T5_FN_082(a,b,c) (((T5_FN_081(a,b,c)*8)/4)-(T5_FN_081(a,b,c)*2)+T5_FN_081(a,b,c)+T5_DEF_082-T5_DEF_082)
    .function T5_FN_083(a,b,c,d) ((((a+b-c-d)*8)/4)-((a+b-c-d)*2)+(a+b-c-d)+T5_DEF_083-T5_DEF_083)
    .function T5_FN_084(a,b,c,d) (((T5_FN_083(a,b,c,d)+T5_DEF_084)-T5_DEF_084))
    .function T5_FN_085(a,b) ((((a-b)*8)/4)-((a-b)*2)+(a-b)+T5_DEF_085-T5_DEF_085)
    .db "05>"               ; @BT TEST-05 05 START

    ; GENERATED: copied from TEST-04; half define-label references replaced by varied function calls
    ; GENERATED: replaced define refs = 85 / 170

    ; GENERATED: copied from TEST-03 with varied define-based operand replacement
    ; GENERATED: replaced operands = 170 / 340

    ; GENERATED: copied from TEST-02 with varied arithmetic expressions for numeric operands

    ; GENERATED: copied from TEST-01 with full TEST-02 randomization
    ; GENERATED: seed = 35714263

    SAY                            ; @BT 42
    STY T5_FN_001(T5_DEF_001+2,2)+5-5                        ; @BT 84 4E
    TST #T5_FN_002(T5_DEF_002+3,3)-7+7,$80*6/3-$80                   ; @BT 83 5F 80
    DEY                            ; @BT 88
    RMB5 T5_FN_003(T5_DEF_003+4,5,4+5)*10/5-$E4                       ; @BT 57 E4
    LSR.W T5_FN_004(T5_DEF_004+5,6,5+6)*2-$5BC9+0,X                  ; @BT 5E C9 5B
    LSR.B T5_FN_005(T5_DEF_005+6+7,8,6,7+8)+11-4-7,X                    ; @BT 56 77
    TYA                            ; @BT 98
    BRA.B T5_FN_006(T5_DEF_006+7+8,9,7,8+9)+3-1-2                      ; @BT 80 51
    CMP #T5_FN_007(T5_DEF_007+8,8)+5-5                       ; @BT C9 89
    BVS T5_FN_008(T5_DEF_008+9,9)-7+7                        ; @BT 70 3D
    BIT.W T5_FN_009(T5_DEF_009+10,11,10+11)*6/3-$2677,X                  ; @BT 3C 77 26
    BBS6 T5_FN_010(T5_DEF_010+11,12,11+12)*10/5-$EE,$05*2-$05+0                   ; @BT EF EE 05
    CMP T5_FN_011(T5_DEF_011+12+13,14,12,13+14)+11-4-7,Y                    ; @BT D9 AA 29
    CLY                            ; @BT C2
    STY T5_FN_012(T5_DEF_012+13+14,15,13,14+15)+3-1-2                      ; @BT 8C C7 7B
    CPY.W T5_FN_013(T5_DEF_013+14,14)+5-5                    ; @BT CC 32 32
    AND.B #T5_FN_014(T5_DEF_014+15,15)-7+7                     ; @BT 29 C6
    STA.W T5_FN_015(T5_DEF_015+16,17,16+17)*6/3-$7672,X                  ; @BT 9D 72 76
    ORA (T5_FN_016(T5_DEF_016+17,18,17+18)*10/5-$BC,X)                    ; @BT 01 BC
    DEC.W T5_FN_017(T5_DEF_017+18+19,20,18,19+20)*2-$3676+0                    ; @BT CE 76 36
    ADC T5_FN_018(T5_DEF_018+19+20,21,19,20+21)+11-4-7,X                      ; @BT 75 84
    AND.B T5_FN_019(T5_DEF_019+20,20)+3-1-2                      ; @BT 25 39
    TSB T5_FN_020(T5_DEF_020+21,21)+5-5                        ; @BT 04 03
    LDA.B T5_FN_021(T5_DEF_021+22,23,22+23)-7+7                      ; @BT A5 1E
    SBC T5_FN_022(T5_DEF_022+23,24,23+24)*6/3-$54                        ; @BT E5 54
    SBC.W T5_FN_023(T5_DEF_023+24+25,26,24,25+26)*10/5-$711D,Y                  ; @BT F9 1D 71
    SBC.W T5_FN_024(T5_DEF_024+25+26,27,25,26+27)*2-$700C+0,X                  ; @BT FD 0C 70
    JMP.W T5_FN_025(T5_DEF_025+26,26)+11-4-7                    ; @BT 4C 97 48
    AND T5_FN_026(T5_DEF_026+27,27)+3-1-2                      ; @BT 2D CD 1B
    ROR T5_FN_027(T5_DEF_027+28,29,28+29)+5-5                        ; @BT 66 2C
    LDY.W T5_FN_028(T5_DEF_028+29,30,29+30)-7+7,X                  ; @BT BC 85 57
    BRK                            ; @BT 00 00
    ROR                            ; @BT 6A
    EOR.B (T5_FN_029(T5_DEF_029+30+31,32,30,31+32)*6/3-$69),Y                  ; @BT 51 69
    AND.B T5_FN_030(T5_DEF_030+31+32,33,31,32+33)*10/5-$22,X                    ; @BT 35 22
    SXY                            ; @BT 02
    ROL T5_FN_031(T5_DEF_031+32,32)*2-$D0+0                        ; @BT 26 D0
    BVS.B T5_FN_032(T5_DEF_032+33,33)+11-4-7                      ; @BT 70 23
    ORA.B (T5_FN_033(T5_DEF_033+34,35,34+35)+3-1-2),Y                  ; @BT 11 5D
    EOR T5_FN_034(T5_DEF_034+35,36,35+36)+5-5,X                    ; @BT 5D 87 37
    AND T5_FN_035(T5_DEF_035+36+37,38,36,37+38)-7+7,X                    ; @BT 3D AB 19
    LDA T5_FN_036(T5_DEF_036+37+38,39,37,38+39)*6/3-$7D,X                      ; @BT B5 7D
    BBS2 T5_FN_037(T5_DEF_037+38,38)*10/5-$92,$29*2-$29+0                   ; @BT AF 92 29
    BBR1 T5_FN_038(T5_DEF_038+39,39)+11-4-7,$5A+3-1-2                   ; @BT 1F C3 5A
    SMB2 T5_FN_039(T5_DEF_039+40,41,40+41)+5-5                       ; @BT A7 57
    LDY T5_FN_040(T5_DEF_040+41,42,41+42)-7+7,X                      ; @BT B4 D6
    ADC T5_FN_041(T5_DEF_041+42+43,44,42,43+44)*6/3-$1567                      ; @BT 6D 67 15
    STA.B T5_FN_042(T5_DEF_042+43+44,45,43,44+45)*10/5-$AD,X                    ; @BT 95 AD
    ASL T5_FN_043(T5_DEF_043+44,44)*2-$2122+0                      ; @BT 0E 22 21
    BNE.B T5_FN_044(T5_DEF_044+45,45)+11-4-7                      ; @BT D0 5E
    BVC.B T5_FN_045(T5_DEF_045+46,47,46+47)+3-1-2                      ; @BT 50 0C
    CPY.B T5_FN_046(T5_DEF_046+47,48,47+48)+5-5                      ; @BT C4 3E
    AND T5_FN_047(T5_DEF_047+48+49,50,48,49+50)-7+7,Y                    ; @BT 39 BC 1A
    ADC.W T5_FN_048(T5_DEF_048+49+50,51,49,50+51)*6/3-$189A                    ; @BT 6D 9A 18
    CSH                            ; @BT D4
    SBC.B #T5_FN_049(T5_DEF_049+50,50)*10/5-$6B                     ; @BT E9 6B
    STA (T5_FN_050(T5_DEF_050+51,51)*2-$23+0)                      ; @BT 92 23
    ADC.B #T5_FN_051(T5_DEF_051+52,53,52+53)+11-4-7                     ; @BT 69 B2
    BRA T5_FN_052(T5_DEF_052+53,54,53+54)+3-1-2                        ; @BT 80 6B
    ADC.B T5_FN_053(T5_DEF_053+54+55,56,54,55+56)+5-5,X                    ; @BT 75 0E
    LDA T5_FN_054(T5_DEF_054+55+56,57,55,56+57)-7+7                        ; @BT A5 94
    AND T5_FN_055(T5_DEF_055+56,56)*6/3-$AF                        ; @BT 25 AF
    LDX.B T5_FN_056(T5_DEF_056+57,57)*10/5-$91,Y                    ; @BT B6 91
    SBC #T5_FN_057(T5_DEF_057+58,59,58+59)*2-$E1+0                       ; @BT E9 E1
    LSR T5_FN_058(T5_DEF_058+59,60,59+60)+11-4-7,X                    ; @BT 5E A7 59
    STX T5_FN_059(T5_DEF_059+60+61,62,60,61+62)+3-1-2                      ; @BT 8E A5 79
    LDY T5_FN_060(T5_DEF_060+61+62,63,61,62+63)+5-5                      ; @BT AC 74 56
    LDA.B T5_FN_061(T5_DEF_061+62,62)-7+7,X                    ; @BT B5 07
    CPY.B #T5_FN_062(T5_DEF_062+63,63)*6/3-$27                     ; @BT C0 27
    ASL A                          ; @BT 0A
    BIT.B T5_FN_063(T5_DEF_063+64,65,64+65)*10/5-$2D,X                    ; @BT 34 2D
    DEC.W T5_FN_064(T5_DEF_064+65,66,65+66)*2-$3565+0,X                  ; @BT DE 65 35
    ROL T5_FN_065(T5_DEF_065+66+67,68,66,67+68)+11-4-7,X                      ; @BT 36 B9
    BIT T5_FN_066(T5_DEF_066+67+68,69,67,68+69)+3-1-2                        ; @BT 24 16
    EOR.W T5_FN_067(T5_DEF_067+68,68)+5-5                    ; @BT 4D DC 3C
    ORA.W T5_FN_068(T5_DEF_068+69,69)-7+7                    ; @BT 0D 40 63
    INC T5_FN_069(T5_DEF_069+70,71,70+71)*6/3-$C5,X                      ; @BT F6 C5
    TMA.B #T5_FN_070(T5_DEF_070+71,72,71+72)*10/5-$A7                     ; @BT 43 A7
    STA T5_FN_071(T5_DEF_071+72+73,74,72,73+74)*2-$3A+0,X                      ; @BT 95 3A
    STZ T5_FN_072(T5_DEF_072+73+74,75,73,74+75)+11-4-7,X                      ; @BT 74 93
    STA.B (T5_FN_073(T5_DEF_073+74,74)+3-1-2,X)                  ; @BT 81 68
    EOR.W T5_FN_074(T5_DEF_074+75,75)+5-5,Y                  ; @BT 59 CB 3B
    ORA T5_FN_075(T5_DEF_075+76,77,76+77)-7+7,Y                    ; @BT 19 FC 5E
    ORA.B (T5_FN_076(T5_DEF_076+77,78,77+78)*6/3-$74)                    ; @BT 12 74
    BCC T5_FN_077(T5_DEF_077+78+79,80,78,79+80)*10/5-$1C                        ; @BT 90 1C
    INA                            ; @BT 1A
    SBC T5_FN_078(T5_DEF_078+79+80,81,79,80+81)*2-$6DEA+0,Y                    ; @BT F9 EA 6D
    BBS1 T5_FN_079(T5_DEF_079+80,80)+11-4-7,$12+3-1-2                   ; @BT 9F 7B 12
    BBS3 T5_FN_080(T5_DEF_080+81,81)+5-5,$40-7+7                   ; @BT BF A9 40
    BIT #T5_FN_081(T5_DEF_081+82,83,82+83)*6/3-$E8                       ; @BT 89 E8
    EOR #T5_FN_082(T5_DEF_082+83,84,83+84)*10/5-$B1                       ; @BT 49 B1
    CMP.B T5_FN_083(T5_DEF_083+84+85,86,84,85+86)*2-$6F+0,X                    ; @BT D5 6F
    AND.W T5_FN_084(T5_DEF_084+85+86,87,85,86+87)+11-4-7                    ; @BT 2D 00 1F
    CPY T5_FN_085(T5_DEF_085+86,86)+3-1-2                        ; @BT C4 10
    PHX                            ; @BT DA
    BBR4 T5_DEF_086+5-5,$1F-7+7                   ; @BT 4F 08 1F
    STA.W T5_DEF_087*6/3-$7894                    ; @BT 8D 94 78
    ADC.B T5_DEF_088*10/5-$25                      ; @BT 65 25
    STX T5_DEF_089*2-$DB+0,Y                      ; @BT 96 DB
    STA.W T5_DEF_090+11-4-7,Y                  ; @BT 99 83 77
    DEX                            ; @BT CA
    ADC.W T5_DEF_091+3-1-2,X                  ; @BT 7D 78 16
    AND.W T5_DEF_092+5-5,Y                  ; @BT 39 EF 1D
    SBC.B (T5_DEF_093-7+7),Y                  ; @BT F1 99
    ADC (T5_DEF_094*6/3-$3F,X)                    ; @BT 61 3F
    CMP T5_DEF_095*10/5-$E5,X                      ; @BT D5 E5
    ST1 #T5_DEF_096*2-$06+0                       ; @BT 13 06
    TSB.B T5_DEF_097+11-4-7                      ; @BT 04 1A
    CSL                            ; @BT 54
    TSX                            ; @BT BA
    BBS7 T5_DEF_098+3-1-2,$1C+5-5                   ; @BT FF 05 1C
    SED                            ; @BT F8
    TST #T5_DEF_099-7+7,$8BC6*6/3-$8BC6,X               ; @BT B3 48 C6 8B
    STA T5_DEF_100*10/5-$7450,Y                    ; @BT 99 50 74
    CMP.W T5_DEF_101*2-$2DEE+0                    ; @BT CD EE 2D
    BPL.B T5_DEF_102+11-4-7                      ; @BT 10 75
    SMB3 T5_DEF_103+3-1-2                       ; @BT B7 6E
    STX.W T5_DEF_104+5-5                    ; @BT 8E B6 7A
    PLX                            ; @BT FA
    DEC T5_DEF_105-7+7,X                      ; @BT D6 55
    ROL                            ; @BT 2A
    LDA T5_DEF_106*6/3-$4AB9,X                    ; @BT BD B9 4A
    SBC.B T5_DEF_107*10/5-$C7,X                    ; @BT F5 C7
    SBC.B T5_DEF_108*2-$DE+0                      ; @BT E5 DE
    JMP T5_DEF_109+11-4-7                      ; @BT 4C 53 44
    TRB.W T5_DEF_110+3-1-2                    ; @BT 1C 3E 83
    BIT.B T5_DEF_111+5-5                      ; @BT 24 44
    TAM #T5_DEF_112-7+7                       ; @BT 53 90
    BIT T5_DEF_113*6/3-$FF,X                      ; @BT 34 FF
    LDA.W T5_DEF_114*10/5-$500E                    ; @BT AD 0E 50
    DEC T5_DEF_115*2-$3343+0,X                    ; @BT DE 43 33
    JMP.W (T5_DEF_116+11-4-7,X)                ; @BT 7C 75 46
    TXA                            ; @BT 8A
    SMB7 T5_DEF_117+3-1-2                       ; @BT F7 CA
    LDA #T5_DEF_118+5-5                       ; @BT A9 21
    ROR.W T5_DEF_119-7+7,X                  ; @BT 7E B7 6A
    RMB4 T5_DEF_120*6/3-$CD                       ; @BT 47 CD
    LDX.W T5_DEF_121*10/5-$5341,Y                  ; @BT BE 41 53
    TSB T5_DEF_122*2-$844F+0                      ; @BT 0C 4F 84
    ORA.W T5_DEF_123+11-4-7,X                  ; @BT 1D 1E 61
    TIN T5_DEF_124+3-1-2,$89A4+5-5,$8AB5-7+7          ; @BT D3 93 88 A4 89 B5 8A
    STZ T5_DEF_125*6/3-$7DE9,X                    ; @BT 9E E9 7D
    CPX.B T5_DEF_126*10/5-$E2                      ; @BT E4 E2
    LDA.W T5_DEF_127*2-$4EFD+0,Y                  ; @BT B9 FD 4E
    BEQ T5_DEF_128+11-4-7                        ; @BT F0 4A
    BBR5 T5_DEF_129+3-1-2,$36+5-5                   ; @BT 5F 1F 36
    ADC.B (T5_DEF_130-7+7),Y                  ; @BT 71 E0
    PLA                            ; @BT 68
    STZ.B T5_DEF_131*6/3-$C1,X                    ; @BT 74 C1
    STA (T5_DEF_132*10/5-$0C),Y                    ; @BT 91 0C
    EOR.B T5_DEF_133*2-$AE+0                      ; @BT 45 AE
    INX                            ; @BT E8
    RMB7 T5_DEF_134+11-4-7                       ; @BT 77 12
    CLA                            ; @BT 62
    RMB0 T5_DEF_135+3-1-2                       ; @BT 07 71
    AND.B (T5_DEF_136+5-5)                    ; @BT 32 0B
    PLY                            ; @BT 7A
    ADC.W T5_DEF_137-7+7,Y                  ; @BT 79 89 17
    ADC (T5_DEF_138*6/3-$6D)                      ; @BT 72 6D
    LDY.B T5_DEF_139*10/5-$32                      ; @BT A4 32
    BPL T5_DEF_140*2-$0F+0                        ; @BT 10 0F
    SBC.W T5_DEF_141+11-4-7                    ; @BT ED 2E 72
    ASL T5_DEF_142+3-1-2,X                      ; @BT 16 50
    EOR (T5_DEF_143+5-5,X)                    ; @BT 41 C8
    PHP                            ; @BT 08
    SBC (T5_DEF_144-7+7)                      ; @BT F2 26
    STA.B (T5_DEF_145*6/3-$7F),Y                  ; @BT 91 7F
    BIT.W T5_DEF_146*10/5-$2788                    ; @BT 2C 88 27
    STA.B T5_DEF_147*2-$C4+0                      ; @BT 85 C4
    CPY #T5_DEF_148+11-4-7                       ; @BT C0 F9
    STZ.W T5_DEF_149+3-1-2                    ; @BT 9C 1C 81
    CPY T5_DEF_150+5-5                      ; @BT CC 21 31
    BSR.B T5_DEF_151-7+7                      ; @BT 44 3A
    BBS0 T5_DEF_152*6/3-$64,$7B*10/5-$7B                   ; @BT 8F 64 7B
    LDA T5_DEF_153*2-$4BCA+0,Y                    ; @BT B9 CA 4B
    CMP.B T5_DEF_154+11-4-7                      ; @BT C5 86
    CMP (T5_DEF_155+3-1-2,X)                    ; @BT C1 A0
    EOR T5_DEF_156+5-5,Y                    ; @BT 59 98 38
    DEA                            ; @BT 3A
    STX.B T5_DEF_157-7+7                      ; @BT 86 20
    LDA (T5_DEF_158*6/3-$4F),Y                    ; @BT B1 4F
    BIT T5_DEF_159*10/5-$2566                      ; @BT 2C 66 25
    CMP.W T5_DEF_160*2-$2CDD+0,Y                  ; @BT D9 DD 2C
    CMP (T5_DEF_161+11-4-7),Y                    ; @BT D1 B7
    DEC.B T5_DEF_162+3-1-2                      ; @BT C6 9A
    LDA.B (T5_DEF_163+5-5),Y                  ; @BT B1 D9
    STZ T5_DEF_164-7+7                      ; @BT 9C FA 7E
    ROL T5_DEF_165*6/3-$6562                      ; @BT 2E 62 65
    TAM.B #T5_DEF_166*10/5-$BE                     ; @BT 53 BE
    SBC.B (T5_DEF_167*2-$B0+0)                    ; @BT F2 B0
    BBS4 T5_DEF_168+11-4-7,$57+3-1-2                   ; @BT CF C0 57
    LDY.W T5_DEF_169+5-5                    ; @BT AC 96 58
    STA T5_DEF_170-7+7                      ; @BT 8D 61 75
    INC $3DED*6/3-$3DED,X                    ; @BT FE ED 3D
    ORA ($D3*10/5-$D3),Y                    ; @BT 11 D3
    AND.B ($DD*2-$DD+0,X)                  ; @BT 21 DD
    ROL.B $E7+11-4-7,X                    ; @BT 36 E7
    STA $51+3-1-2                        ; @BT 85 51
    CPX $B4+5-5                        ; @BT E4 B4
    EOR $39A9-7+7                      ; @BT 4D A9 39
    CLD                            ; @BT D8
    ORA $18*6/3-$18                        ; @BT 05 18
    SBC ($F8*10/5-$F8,X)                    ; @BT E1 F8
    CMP ($CE*2-$CE+0)                      ; @BT D2 CE
    ASL.B $7E+11-4-7,X                    ; @BT 16 7E
    AND.B ($F4+3-1-2),Y                  ; @BT 31 F4
    CLC                            ; @BT 18
    ADC #$28+5-5                       ; @BT 69 28
    ORA.B $8B-7+7,X                    ; @BT 15 8B
    EOR.B ($80*6/3-$80)                    ; @BT 52 80
    LDX $4C*10/5-$4C,Y                      ; @BT B6 4C
    JMP ($4342*2-$4342+0)                    ; @BT 6C 42 43
    STY $37+11-4-7,X                      ; @BT 94 37
    SMB5 $9C+3-1-2                       ; @BT D7 9C
    ORA #$A5+5-5                       ; @BT 09 A5
    ORA $600D-7+7                      ; @BT 0D 0D 60
    ST2 #$1D*6/3-$1D                       ; @BT 23 1D
    STX.B $09*10/5-$09,Y                    ; @BT 96 09
    ROL.W $6784*2-$6784+0                    ; @BT 2E 84 67
    LDA.B ($F0+11-4-7)                    ; @BT B2 F0
    CMP.B ($2A+3-1-2,X)                  ; @BT C1 2A
    SEC                            ; @BT 38
    BBS5 $D7+5-5,$6E-7+7                   ; @BT DF D7 6E
    CPX #$9D*6/3-$9D                       ; @BT E0 9D
    CPX.W $3010*10/5-$3010                    ; @BT EC 10 30
    AND #$3C*2-$3C+0                       ; @BT 29 3C
    ROR.W $6BC8+11-4-7                    ; @BT 6E C8 6B
    INC.B $F3+3-1-2,X                    ; @BT F6 F3
    TSB.W $8560+5-5                    ; @BT 0C 60 85
    STA $733F-7+7,X                    ; @BT 9D 3F 73
    TST #$76*6/3-$76,$8CD7*10/5-$8CD7                 ; @BT 93 76 D7 8C
    BEQ.B $30*2-$30+0                      ; @BT F0 30
    BBR2 $DA+11-4-7,$71+3-1-2                   ; @BT 2F DA 71
    EOR ($DF+5-5),Y                    ; @BT 51 DF
    CPX.B #$CB-7+7                     ; @BT E0 CB
    LDY.B $1B*6/3-$1B,X                    ; @BT B4 1B
    ORA.B #$2F*10/5-$2F                     ; @BT 09 2F
    BIT $2455*2-$2455+0,X                    ; @BT 3C 55 24
    TST #$31+11-4-7,$52+3-1-2,X                 ; @BT A3 31 52
    BVC $26+5-5                        ; @BT 50 26
    ORA.B ($46-7+7,X)                  ; @BT 01 46
    RMB6 $FB*6/3-$FB                       ; @BT 67 FB
    ORA.W $622F*10/5-$622F,Y                  ; @BT 19 2F 62
    SBC ($0F*2-$0F+0),Y                    ; @BT F1 0F
    JSR $4564+11-4-7                      ; @BT 20 64 45
    CLX                            ; @BT 82
    STZ $AA+3-1-2                        ; @BT 64 AA
    TAI $8AB5+5-5,$8BC6-7+7,$8CD7*6/3-$8CD7          ; @BT F3 B5 8A C6 8B D7 8C
    LDA ($66*10/5-$66)                      ; @BT B2 66
    TRB $D5*2-$D5+0                        ; @BT 14 D5
    TDD $8782+11-4-7,$8893+3-1-2,$89A4+5-5          ; @BT C3 82 87 93 88 A4 89
    TII $8671-7+7,$8782*6/3-$8782,$8893*10/5-$8893          ; @BT 73 71 86 82 87 93 88
    BNE $78*2-$78+0                        ; @BT D0 78
    INC $3EFE+11-4-7                      ; @BT EE FE 3E
    CMP $2ABB+3-1-2                      ; @BT CD BB 2A
    TRB $822D+5-5                      ; @BT 1C 2D 82
    BRK $5B-7+7                        ; @BT 00 5B
    AND ($6A*6/3-$6A),Y                    ; @BT 31 6A
    SMB4 $85*10/5-$85                       ; @BT C7 85
    PLP                            ; @BT 28
    BBR7 $4D*2-$4D+0,$64+11-4-7                   ; @BT 7F 4D 64
    EOR.W $3ABA+3-1-2,X                  ; @BT 5D BA 3A
    LDA.B #$AB+5-5                     ; @BT A9 AB
    CLV                            ; @BT B8
    ROL.B $FE-7+7                      ; @BT 26 FE
    INY                            ; @BT C8
    SBC $3D*6/3-$3D,X                      ; @BT F5 3D
    ADC $9B*10/5-$9B                        ; @BT 65 9B
    BCC.B $02*2-$02+0                      ; @BT 90 02
    LSR A                          ; @BT 4A
    STY.B $65+11-4-7,X                    ; @BT 94 65
    ASL                            ; @BT 0A
    BRK.B $72+3-1-2                      ; @BT 00 72
    LDY.B #$04+5-5                     ; @BT A0 04
    ROR $69A6-7+7                      ; @BT 6E A6 69
    ROR $6895*6/3-$6895,X                    ; @BT 7E 95 68
    ST0 #$EF*10/5-$EF                       ; @BT 03 EF
    ORA $5DEB*2-$5DEB+0,X                    ; @BT 1D EB 5D
    LSR $5AB8+11-4-7                      ; @BT 4E B8 5A
    LDA.B ($C2+3-1-2,X)                  ; @BT A1 C2
    LDX.B $A8+5-5                      ; @BT A6 A8
    ST1.B #$4B-7+7                     ; @BT 13 4B
    CMP.W $2BCC*6/3-$2BCC,X                  ; @BT DD CC 2B
    ASL.W $2233*10/5-$2233,X                  ; @BT 1E 33 22
    ASL $2011*2-$2011+0,X                    ; @BT 1E 11 20
    LDX.B #$7A+11-4-7                     ; @BT A2 7A
    ROL $6451+3-1-2,X                    ; @BT 3E 51 64
    LDA ($38+5-5,X)                    ; @BT A1 38
    SMB6 $B3-7+7                       ; @BT E7 B3
    SET                            ; @BT F4
    SAX                            ; @BT 22
    AND $98*6/3-$98,X                      ; @BT 35 98
    STA ($F5*10/5-$F5,X)                    ; @BT 81 F5
    STZ.B $D8*2-$D8+0                      ; @BT 64 D8
    LSR $49+11-4-7,X                      ; @BT 56 49
    INC $DC+3-1-2                        ; @BT E6 DC
    ADC.B ($C9+5-5,X)                  ; @BT 61 C9
    SBC.B ($82-7+7,X)                  ; @BT E1 82
    DEC $3454*6/3-$3454                      ; @BT CE 54 34
    ADC $1345*10/5-$1345,X                    ; @BT 7D 45 13
    STX $F2*2-$F2+0                        ; @BT 86 F2
    RTI                            ; @BT 40
    INC.B $0A+11-4-7                      ; @BT E6 0A
    LDX #$35+3-1-2                       ; @BT A2 35
    BBR3 $F1+5-5,$08-7+7                   ; @BT 3F F1 08
    RTS                            ; @BT 60
    STA.B ($96*6/3-$96)                    ; @BT 92 96
    LSR $60*10/5-$60                        ; @BT 46 60
    LSR                            ; @BT 4A
    ROR $15*2-$15+0,X                      ; @BT 76 15
    BCS.B $19+11-4-7                      ; @BT B0 19
    DEC $6C+3-1-2                        ; @BT C6 6C
    PHA                            ; @BT 48
    LDX $511F+5-5,Y                    ; @BT BE 1F 51
    ADC.B ($F7-7+7)                    ; @BT 72 F7
    TRB.B $EC*6/3-$EC                      ; @BT 14 EC
    SEI                            ; @BT 78
    DEC.B $83*10/5-$83,X                    ; @BT D6 83
    PHY                            ; @BT 5A
    ADC ($56*2-$56+0),Y                    ; @BT 71 56
    LSR.B $8E+11-4-7                      ; @BT 46 8E
    ASL $67+3-1-2                        ; @BT 06 67
    BCS $33+5-5                        ; @BT B0 33
    NOP                            ; @BT EA
    ROL.W $6673-7+7,X                  ; @BT 3E 73 66
    ASL.W $2344*6/3-$2344                    ; @BT 0E 44 23
    ORA.B $A2*10/5-$A2                      ; @BT 05 A2
    LSR.W $5CDA*2-$5CDA+0                    ; @BT 4E DA 5C
    LDX $5230+11-4-7                      ; @BT AE 30 52
    ROL A                          ; @BT 2A
    AND.W $1CDE+3-1-2,X                  ; @BT 3D DE 1C
    BMI $61+5-5                        ; @BT 30 61
    ST0.B #$34-7+7                     ; @BT 03 34
    STY.B $7C*6/3-$7C                      ; @BT 84 7C
    CLI                            ; @BT 58
    EOR ($F6*10/5-$F6)                      ; @BT 52 F6
    EOR $0D*2-$0D+0,X                      ; @BT 55 0D
    LDX.W $5452+11-4-7                    ; @BT AE 52 54
    TXS                            ; @BT 9A
    CMP.B ($58+3-1-2)                    ; @BT D2 58
    RMB3 $B6+5-5                       ; @BT 37 B6
    ADC $1456-7+7,Y                    ; @BT 79 56 14
    INC.W $400F*6/3-$400F,X                  ; @BT FE 0F 40
    ROR.B $5A*10/5-$5A                      ; @BT 66 5A
    CMP $2899*2-$2899+0,X                    ; @BT DD 99 28
    CMP.B ($41+11-4-7),Y                  ; @BT D1 41
    TAY                            ; @BT A8
    CPX $2EFF+3-1-2                      ; @BT EC FF 2E
    AND ($81+5-5)                      ; @BT 32 81
    SBC $6CD9-7+7,X                    ; @BT FD D9 6C
    INC.W $4120*6/3-$4120                    ; @BT EE 20 41
    SMB1 $40*10/5-$40                       ; @BT 97 40
    STY.W $7CD8*2-$7CD8+0                    ; @BT 8C D8 7C
    JSR.W $49A8+11-4-7                    ; @BT 20 A8 49
    LDX $63+3-1-2                        ; @BT A6 63
    EOR.B $97+5-5,X                    ; @BT 55 97
    TIA $89A4-7+7,$8AB5*6/3-$8AB5,$8BC6*10/5-$8BC6          ; @BT E3 A4 89 B5 8A C6 8B
    BMI.B $47*2-$47+0                      ; @BT 30 47
    RMB1 $88+11-4-7                       ; @BT 17 88
    EOR.B #$3B+3-1-2                     ; @BT 49 3B
    ASL.B $95+5-5                      ; @BT 06 95
    ORA $01-7+7,X                      ; @BT 15 01
    LDA.W $4DEC*6/3-$4DEC,X                  ; @BT BD EC 4D
    BBR6 $36*10/5-$36,$4D*2-$4D+0                   ; @BT 6F 36 4D
    EOR $24+11-4-7                        ; @BT 45 24
    AND ($53+3-1-2,X)                    ; @BT 21 53
    EOR.B ($52+5-5,X)                  ; @BT 41 52
    STZ $800B-7+7,X                    ; @BT 9E 0B 80
    LDY $5563*6/3-$5563,X                    ; @BT BC 63 55
    TAX                            ; @BT AA
    ST2.B #$62*10/5-$62                     ; @BT 23 62
    LDY $ED*2-$ED+0                        ; @BT A4 ED
    SBC $6EFB+11-4-7                      ; @BT ED FB 6E
    TMA #$79+3-1-2                       ; @BT 43 79
    LDY #$BF+5-5                       ; @BT A0 BF
    JMP ($4231-7+7,X)                  ; @BT 7C 31 42
    ORA ($EA*6/3-$EA)                      ; @BT 12 EA
    BBR0 $AC*10/5-$AC,$43*2-$43+0                   ; @BT 0F AC 43
    CMP.B #$13+11-4-7                     ; @BT C9 13
    ROR.B $43+3-1-2,X                    ; @BT 76 43
    CMP $FC+5-5                        ; @BT C5 FC
    JMP.W ($4786-7+7)                  ; @BT 6C 86 47
    ROR A                          ; @BT 6A
    SMB0 $29*6/3-$29                       ; @BT 87 29
    BSR $54*10/5-$54                        ; @BT 44 54
    RMB2 $9F*2-$9F+0                       ; @BT 27 9F
    LDA $4CDB+11-4-7                      ; @BT AD DB 4C

    .db "<05"               ; @BT END
