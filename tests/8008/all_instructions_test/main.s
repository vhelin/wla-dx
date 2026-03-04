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

    ; NOTE: 8008 opcodes, as WLA DX understands them, can be found in /i8008.c
    ; Please verify using external documentation (e.g. https://www.masswerk.at/8008/8008_instruction_set.html)
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
    ; Auto-generated from i8008.c + verified against 8008 ISA references

    ACA                      ; @BT 88
    ACB                      ; @BT 89
    ACC                      ; @BT 8A
    ACD                      ; @BT 8B
    ACE                      ; @BT 8C
    ACH                      ; @BT 8D
    ACI $28                  ; @BT 0C 28
    ACL                      ; @BT 8E
    ACM                      ; @BT 8F
    ADA                      ; @BT 80
    ADB                      ; @BT 81
    ADC                      ; @BT 82
    ADD                      ; @BT 83
    ADE                      ; @BT 84
    ADH                      ; @BT 85
    ADI $3F                  ; @BT 04 3F
    ADL                      ; @BT 86
    ADM                      ; @BT 87
    CAL $1345                ; @BT 46 45 13
    CFC $1456                ; @BT 42 56 14
    CFP $1567                ; @BT 5A 67 15
    CFS $1678                ; @BT 52 78 16
    CFZ $1789                ; @BT 4A 89 17
    CPA                      ; @BT B8
    CPB                      ; @BT B9
    CPC                      ; @BT BA
    CPD                      ; @BT BB
    CPE                      ; @BT BC
    CPH                      ; @BT BD
    CPI $56                  ; @BT 3C 56
    CPL                      ; @BT BE
    CPM                      ; @BT BF
    CTC $189A                ; @BT 62 9A 18
    CTP $19AB                ; @BT 7A AB 19
    CTS $1ABC                ; @BT 72 BC 1A
    CTZ $1BCD                ; @BT 6A CD 1B
    DCB                      ; @BT 09
    DCC                      ; @BT 11
    DCD                      ; @BT 19
    DCE                      ; @BT 21
    DCH                      ; @BT 29
    DCL                      ; @BT 31
    HLT                      ; @BT FF
    INB                      ; @BT 08
    INC                      ; @BT 10
    IND                      ; @BT 18
    INE                      ; @BT 20
    INH                      ; @BT 28
    INL                      ; @BT 30
    INP 0                    ; @BT 41
    INP 1                    ; @BT 43
    INP 2                    ; @BT 45
    INP 3                    ; @BT 47
    INP 4                    ; @BT 49
    INP 5                    ; @BT 4B
    INP 6                    ; @BT 4D
    INP 7                    ; @BT 4F
    JFC $1CDE                ; @BT 40 DE 1C
    JFP $1DEF                ; @BT 58 EF 1D
    JFS $1F00                ; @BT 50 00 1F
    JFZ $2011                ; @BT 48 11 20
    JMP $2122                ; @BT 44 22 21
    JTC $2233                ; @BT 60 33 22
    JTP $2344                ; @BT 78 44 23
    JTS $2455                ; @BT 70 55 24
    JTZ $2566                ; @BT 68 66 25
    LAA                      ; @BT C0
    LAB                      ; @BT C1
    LAC                      ; @BT C2
    LAD                      ; @BT C3
    LAE                      ; @BT C4
    LAH                      ; @BT C5
    LAI $6D                  ; @BT 06 6D
    LAL                      ; @BT C6
    LAM                      ; @BT C7
    LBA                      ; @BT C8
    LBB                      ; @BT C9
    LBC                      ; @BT CA
    LBD                      ; @BT CB
    LBE                      ; @BT CC
    LBH                      ; @BT CD
    LBI $84                  ; @BT 0E 84
    LBL                      ; @BT CE
    LBM                      ; @BT CF
    LCA                      ; @BT D0
    LCB                      ; @BT D1
    LCC                      ; @BT D2
    LCD                      ; @BT D3
    LCE                      ; @BT D4
    LCH                      ; @BT D5
    LCI $9B                  ; @BT 16 9B
    LCL                      ; @BT D6
    LCM                      ; @BT D7
    LDA                      ; @BT D8
    LDB                      ; @BT D9
    LDC                      ; @BT DA
    LDD                      ; @BT DB
    LDE                      ; @BT DC
    LDH                      ; @BT DD
    LDI $B2                  ; @BT 1E B2
    LDL                      ; @BT DE
    LDM                      ; @BT DF
    LEA                      ; @BT E0
    LEB                      ; @BT E1
    LEC                      ; @BT E2
    LED                      ; @BT E3
    LEE                      ; @BT E4
    LEH                      ; @BT E5
    LEI $C9                  ; @BT 26 C9
    LEL                      ; @BT E6
    LEM                      ; @BT E7
    LHA                      ; @BT E8
    LHB                      ; @BT E9
    LHC                      ; @BT EA
    LHD                      ; @BT EB
    LHE                      ; @BT EC
    LHH                      ; @BT ED
    LHI $E0                  ; @BT 2E E0
    LHL                      ; @BT EE
    LHM                      ; @BT EF
    LLA                      ; @BT F0
    LLB                      ; @BT F1
    LLC                      ; @BT F2
    LLD                      ; @BT F3
    LLE                      ; @BT F4
    LLH                      ; @BT F5
    LLI $F7                  ; @BT 36 F7
    LLL                      ; @BT F6
    LLM                      ; @BT F7
    LMA                      ; @BT F8
    LMB                      ; @BT F9
    LMC                      ; @BT FA
    LMD                      ; @BT FB
    LME                      ; @BT FC
    LMH                      ; @BT FD
    LMI $0E                  ; @BT 3E 0E
    LML                      ; @BT FE
    MVI A,$25                ; @BT 06 25
    MOV A,A                  ; @BT C0
    MOV A, B                 ; @BT C1
    MOV A,  C                ; @BT C2
    MOV A, D                 ; @BT C3
    MOV  A,E                 ; @BT C4
    MOV A,H                  ; @BT C5
    MOV A,L                  ; @BT C6
    MOV A,M                  ; @BT C7
    MOV B,A                  ; @BT C8
    MOV B,B                  ; @BT C9
    MOV B,C                  ; @BT CA
    MOV B,D                  ; @BT CB
    MOV B,E                  ; @BT CC
    MOV B,H                  ; @BT CD
    MVI B,$3C                ; @BT 0E 3C
    MOV B,L                  ; @BT CE
    MOV B,M                  ; @BT CF
    MOV C,A                  ; @BT D0
    MOV C,B                  ; @BT D1
    MOV C,C                  ; @BT D2
    MOV C,D                  ; @BT D3
    MOV C,E                  ; @BT D4
    MOV C,H                  ; @BT D5
    MVI C,$53                  ; @BT 16 53
    MOV C,L                  ; @BT D6
    MOV C,M                  ; @BT D7
    MOV D,A                  ; @BT D8
    MOV D,B                  ; @BT D9
    MOV D,C                  ; @BT DA
    MOV D,D                  ; @BT DB
    MOV D,E                  ; @BT DC
    MOV D,H                  ; @BT DD
    MVI D,$6A                ; @BT 1E 6A
    MOV D,L                  ; @BT DE
    MOV D,M                  ; @BT DF
    MOV E,A                  ; @BT E0
    MOV E,B                  ; @BT E1
    MOV E,C                  ; @BT E2
    MOV E,D                  ; @BT E3
    MOV E,E                  ; @BT E4
    MOV E,H                  ; @BT E5
    MVI E,$81                  ; @BT 26 81
    MOV E,L                  ; @BT E6
    MOV E,M                  ; @BT E7
    MOV H,A                  ; @BT E8
    MOV H,B                  ; @BT E9
    MOV H,C                  ; @BT EA
    MOV H,D                  ; @BT EB
    MOV H,E                  ; @BT EC
    MOV H,H                  ; @BT ED
    MVI H,$98                  ; @BT 2E 98
    MOV H,L                  ; @BT EE
    MOV H,M                  ; @BT EF
    MOV L,A                  ; @BT F0
    MOV L,B                  ; @BT F1
    MOV L,C                  ; @BT F2
    MOV L,D                  ; @BT F3
    MOV L,E                  ; @BT F4
    MOV L,H                  ; @BT F5
    MVI L,$AF                  ; @BT 36 AF
    MOV L,L                  ; @BT F6
    MOV L,M                  ; @BT F7
    MOV M,A                  ; @BT F8
    MOV M,B                  ; @BT F9
    MOV M,C                  ; @BT FA
    MOV M,D                  ; @BT FB
    MOV M,E                  ; @BT FC
    MOV M,H                  ; @BT FD
    MVI M,$C6                  ; @BT 3E C6
    MOV M,L                  ; @BT FE
    NDA                      ; @BT A0
    NDB                      ; @BT A1
    NDC                      ; @BT A2
    NDD                      ; @BT A3
    NDE                      ; @BT A4
    NDH                      ; @BT A5
    NDI $DD                  ; @BT 24 DD
    NDL                      ; @BT A6
    NDM                      ; @BT A7
    NOP                      ; @BT C0
    ORA                      ; @BT B0
    ORB                      ; @BT B1
    ORC                      ; @BT B2
    ORD                      ; @BT B3
    ORE                      ; @BT B4
    ORH                      ; @BT B5
    ORI $F4                  ; @BT 34 F4
    ORL                      ; @BT B6
    ORM                      ; @BT B7
    OUT 8                    ; @BT 51
    OUT 9                    ; @BT 53
    OUT 10                   ; @BT 55
    OUT 11                   ; @BT 57
    OUT 12                   ; @BT 59
    OUT 13                   ; @BT 5B
    OUT 14                   ; @BT 5D
    OUT 15                   ; @BT 5F
    OUT 16                   ; @BT 61
    OUT 17                   ; @BT 63
    OUT 18                   ; @BT 65
    OUT 19                   ; @BT 67
    OUT 20                   ; @BT 69
    OUT 21                   ; @BT 6B
    OUT 22                   ; @BT 6D
    OUT 23                   ; @BT 6F
    OUT 24                   ; @BT 71
    OUT 25                   ; @BT 73
    OUT 26                   ; @BT 75
    OUT 27                   ; @BT 77
    OUT 28                   ; @BT 79
    OUT 29                   ; @BT 7B
    OUT 30                   ; @BT 7D
    OUT 31                   ; @BT 7F
    RAL                      ; @BT 12
    RAR                      ; @BT 1A
    RET                      ; @BT 07
    RFC                      ; @BT 03
    RFP                      ; @BT 1B
    RFS                      ; @BT 13
    RFZ                      ; @BT 0B
    RLC                      ; @BT 02
    RRC                      ; @BT 0A
    RST 0                    ; @BT 05
    RST 1                    ; @BT 0D
    RST 2                    ; @BT 15
    RST 3                    ; @BT 1D
    RST 4                    ; @BT 25
    RST 5                    ; @BT 2D
    RST 6                    ; @BT 35
    RST 7                    ; @BT 3D
    RTC                      ; @BT 23
    RTP                      ; @BT 3B
    RTS                      ; @BT 33
    RTZ                      ; @BT 2B
    SBA                      ; @BT 98
    SBB                      ; @BT 99
    SBC                      ; @BT 9A
    SBD                      ; @BT 9B
    SBE                      ; @BT 9C
    SBH                      ; @BT 9D
    SBI $0B                  ; @BT 1C 0B
    SBL                      ; @BT 9E
    SBM                      ; @BT 9F
    SUA                      ; @BT 90
    SUB                      ; @BT 91
    SUC                      ; @BT 92
    SUD                      ; @BT 93
    SUE                      ; @BT 94
    SUH                      ; @BT 95
    SUI $22                  ; @BT 14 22
    SUL                      ; @BT 96
    SUM                      ; @BT 97
    XRA                      ; @BT A8
    XRB                      ; @BT A9
    XRC                      ; @BT AA
    XRD                      ; @BT AB
    XRE                      ; @BT AC
    XRH                      ; @BT AD
    XRI $39                  ; @BT 2C 39
    XRL                      ; @BT AE
    XRM                      ; @BT AF
    .db "<01"               ; @BT END

    /////////////////////////////////////////////////////////////////////////////
    // TEST-02: same opcodes as TEST-01, randomised order, branches spread out
    /////////////////////////////////////////////////////////////////////////////

    .db "02>"               ; @BT TEST-02 02 START

    ; GENERATED: copied from TEST-01 with full TEST-02 randomization
    ; GENERATED: seed = 2007978089

    CPA                      ; @BT B8
    LDI $B2                  ; @BT 1E B2
    LLM                      ; @BT F7
    LME                      ; @BT FC
    LBC                      ; @BT CA
    OUT 24                   ; @BT 71
    ADA                      ; @BT 80
    INC                      ; @BT 10
    OUT 22                   ; @BT 6D
    CFP $1567                ; @BT 5A 67 15
    LHM                      ; @BT EF
    ADD                      ; @BT 83
    JTC $2233                ; @BT 60 33 22
    NDA                      ; @BT A0
    SUM                      ; @BT 97
    CTS $1ABC                ; @BT 72 BC 1A
    LHH                      ; @BT ED
    LEB                      ; @BT E1
    OUT 19                   ; @BT 67
    LEM                      ; @BT E7
    INP 4                    ; @BT 49
    SUB                      ; @BT 91
    RTS                      ; @BT 33
    ORM                      ; @BT B7
    OUT 11                   ; @BT 57
    OUT 14                   ; @BT 5D
    OUT 20                   ; @BT 69
    JFZ $2011                ; @BT 48 11 20
    OUT 23                   ; @BT 6F
    ORI $3C                  ; @BT 34 3C
    ACD                      ; @BT 8B
    LDA                      ; @BT D8
    LMC                      ; @BT FA
    RFC                      ; @BT 03
    OUT 29                   ; @BT 7B
    XRH                      ; @BT AD
    NDE                      ; @BT A4
    LEL                      ; @BT E6
    SUL                      ; @BT 96
    LAI $6D                  ; @BT 06 6D
    CPD                      ; @BT BB
    DCH                      ; @BT 29
    LDB                      ; @BT D9
    RFP                      ; @BT 1B
    JFS $1F00                ; @BT 50 00 1F
    SUC                      ; @BT 92
    SBB                      ; @BT 99
    OUT 15                   ; @BT 5F
    LMH                      ; @BT FD
    SUE                      ; @BT 94
    XRL                      ; @BT AE
    ACA                      ; @BT 88
    CTC $189A                ; @BT 62 9A 18
    INP 7                    ; @BT 4F
    LCD                      ; @BT D3
    LMA                      ; @BT F8
    RAL                      ; @BT 12
    LEI $C9                  ; @BT 26 C9
    CPB                      ; @BT B9
    OUT 25                   ; @BT 73
    LCB                      ; @BT D1
    CPL                      ; @BT BE
    OUT 30                   ; @BT 7D
    RLC                      ; @BT 02
    LLC                      ; @BT F2
    ADH                      ; @BT 85
    XRA                      ; @BT A8
    SBI $53                  ; @BT 1C 53
    LEC                      ; @BT E2
    LLA                      ; @BT F0
    IND                      ; @BT 18
    LLI $F7                  ; @BT 36 F7
    ACB                      ; @BT 89
    ORE                      ; @BT B4
    ACI $28                  ; @BT 0C 28
    CPI $56                  ; @BT 3C 56
    INP 6                    ; @BT 4D
    RST 4                    ; @BT 25
    OUT 26                   ; @BT 75
    ADE                      ; @BT 84
    OUT 12                   ; @BT 59
    DCC                      ; @BT 11
    INP 3                    ; @BT 47
    LDM                      ; @BT DF
    ACL                      ; @BT 8E
    XRD                      ; @BT AB
    DCD                      ; @BT 19
    LMI $0E                  ; @BT 3E 0E
    RTP                      ; @BT 3B
    LLH                      ; @BT F5
    LLB                      ; @BT F1
    CFS $1678                ; @BT 52 78 16
    NDM                      ; @BT A7
    INP 2                    ; @BT 45
    LBI $84                  ; @BT 0E 84
    LCM                      ; @BT D7
    INL                      ; @BT 30
    OUT 31                   ; @BT 7F
    INP 0                    ; @BT 41
    SBC                      ; @BT 9A
    OUT 9                    ; @BT 53
    CPM                      ; @BT BF
    OUT 13                   ; @BT 5B
    DCL                      ; @BT 31
    LHE                      ; @BT EC
    RET                      ; @BT 07
    NDL                      ; @BT A6
    LCA                      ; @BT D0
    LBA                      ; @BT C8
    JTZ $2566                ; @BT 68 66 25
    SBM                      ; @BT 9F
    CFZ $1789                ; @BT 4A 89 17
    LEH                      ; @BT E5
    LBL                      ; @BT CE
    ADB                      ; @BT 81
    ADM                      ; @BT 87
    OUT 18                   ; @BT 65
    SUI $6A                  ; @BT 14 6A
    XRM                      ; @BT AF
    DCE                      ; @BT 21
    SBH                      ; @BT 9D
    XRE                      ; @BT AC
    LCH                      ; @BT D5
    INB                      ; @BT 08
    RST 6                    ; @BT 35
    SBL                      ; @BT 9E
    RST 3                    ; @BT 1D
    LHD                      ; @BT EB
    OUT 21                   ; @BT 6B
    LAE                      ; @BT C4
    LBH                      ; @BT CD
    ADL                      ; @BT 86
    JFP $1DEF                ; @BT 58 EF 1D
    JTP $2344                ; @BT 78 44 23
    RST 7                    ; @BT 3D
    RST 1                    ; @BT 0D
    LBE                      ; @BT CC
    NDH                      ; @BT A5
    CTP $19AB                ; @BT 7A AB 19
    ORA                      ; @BT B0
    LEA                      ; @BT E0
    NDB                      ; @BT A1
    LHB                      ; @BT E9
    XRC                      ; @BT AA
    LED                      ; @BT E3
    RST 0                    ; @BT 05
    LHC                      ; @BT EA
    LHA                      ; @BT E8
    RST 2                    ; @BT 15
    LDH                      ; @BT DD
    RFS                      ; @BT 13
    RTZ                      ; @BT 2B
    ORB                      ; @BT B1
    ORH                      ; @BT B5
    LCL                      ; @BT D6
    LCE                      ; @BT D4
    NDI $25                  ; @BT 24 25
    XRI $81                  ; @BT 2C 81
    RFZ                      ; @BT 0B
    RST 5                    ; @BT 2D
    OUT 16                   ; @BT 61
    INP 5                    ; @BT 4B
    LAH                      ; @BT C5
    CPC                      ; @BT BA
    NDC                      ; @BT A2
    DCB                      ; @BT 09
    CTZ $1BCD                ; @BT 6A CD 1B
    INP 1                    ; @BT 43
    LLD                      ; @BT F3
    XRB                      ; @BT A9
    RRC                      ; @BT 0A
    ACC                      ; @BT 8A
    SUH                      ; @BT 95
    LHL                      ; @BT EE
    NOP                      ; @BT C0
    CFC $1456                ; @BT 42 56 14
    ADC                      ; @BT 82
    NDD                      ; @BT A3
    SBA                      ; @BT 98
    LDD                      ; @BT DB
    ORL                      ; @BT B6
    LMD                      ; @BT FB
    SBE                      ; @BT 9C
    LMB                      ; @BT F9
    INE                      ; @BT 20
    HLT                      ; @BT FF
    LCI $9B                  ; @BT 16 9B
    JFC $1CDE                ; @BT 40 DE 1C
    INH                      ; @BT 28
    LAC                      ; @BT C2
    LAL                      ; @BT C6
    SUD                      ; @BT 93
    RTC                      ; @BT 23
    LEE                      ; @BT E4
    ADI $3F                  ; @BT 04 3F
    LCC                      ; @BT D2
    OUT 10                   ; @BT 55
    ORD                      ; @BT B3
    SBD                      ; @BT 9B
    LAD                      ; @BT C3
    ORC                      ; @BT B2
    LBM                      ; @BT CF
    LDC                      ; @BT DA
    ACM                      ; @BT 8F
    JTS $2455                ; @BT 70 55 24
    LLE                      ; @BT F4
    LLL                      ; @BT F6
    CPH                      ; @BT BD
    LBB                      ; @BT C9
    OUT 28                   ; @BT 79
    SUA                      ; @BT 90
    OUT 8                    ; @BT 51
    JMP $2122                ; @BT 44 22 21
    LDE                      ; @BT DC
    ACE                      ; @BT 8C
    LBD                      ; @BT CB
    RAR                      ; @BT 1A
    LML                      ; @BT FE
    CAL $1345                ; @BT 46 45 13
    ACH                      ; @BT 8D
    OUT 27                   ; @BT 77
    OUT 17                   ; @BT 63
    LDL                      ; @BT DE
    CPE                      ; @BT BC
    LHI $E0                  ; @BT 2E E0
    LAM                      ; @BT C7
    LAB                      ; @BT C1

    MVI A,$25                ; @BT 06 25
    MOV A,A                  ; @BT C0
    MOV A, B                 ; @BT C1
    MOV A,  C                ; @BT C2
    MOV A, D                 ; @BT C3
    MOV  A,E                 ; @BT C4
    MOV A,H                  ; @BT C5
    MOV A,L                  ; @BT C6
    MOV A,M                  ; @BT C7
    MOV B,A                  ; @BT C8
    MOV B,B                  ; @BT C9
    MOV B,C                  ; @BT CA
    MOV B,D                  ; @BT CB
    MOV B,E                  ; @BT CC
    MOV B,H                  ; @BT CD
    MVI B,$3C                ; @BT 0E 3C
    MOV B,L                  ; @BT CE
    MOV B,M                  ; @BT CF
    MOV C,A                  ; @BT D0
    MOV C,B                  ; @BT D1
    MOV C,C                  ; @BT D2
    MOV C,D                  ; @BT D3
    MOV C,E                  ; @BT D4
    MOV C,H                  ; @BT D5
    MVI C,$53                  ; @BT 16 53
    MOV C,L                  ; @BT D6
    MOV C,M                  ; @BT D7
    MOV D,A                  ; @BT D8
    MOV D,B                  ; @BT D9
    MOV D,C                  ; @BT DA
    MOV D,D                  ; @BT DB
    MOV D,E                  ; @BT DC
    MOV D,H                  ; @BT DD
    MVI D,$6A                ; @BT 1E 6A
    MOV D,L                  ; @BT DE
    MOV D,M                  ; @BT DF
    MOV E,A                  ; @BT E0
    MOV E,B                  ; @BT E1
    MOV E,C                  ; @BT E2
    MOV E,D                  ; @BT E3
    MOV E,E                  ; @BT E4
    MOV E,H                  ; @BT E5
    MVI E,$81                  ; @BT 26 81
    MOV E,L                  ; @BT E6
    MOV E,M                  ; @BT E7
    MOV H,A                  ; @BT E8
    MOV H,B                  ; @BT E9
    MOV H,C                  ; @BT EA
    MOV H,D                  ; @BT EB
    MOV H,E                  ; @BT EC
    MOV H,H                  ; @BT ED
    MVI H,$98                  ; @BT 2E 98
    MOV H,L                  ; @BT EE
    MOV H,M                  ; @BT EF
    MOV L,A                  ; @BT F0
    MOV L,B                  ; @BT F1
    MOV L,C                  ; @BT F2
    MOV L,D                  ; @BT F3
    MOV L,E                  ; @BT F4
    MOV L,H                  ; @BT F5
    MVI L,$AF                  ; @BT 36 AF
    MOV L,L                  ; @BT F6
    MOV L,M                  ; @BT F7
    MOV M,A                  ; @BT F8
    MOV M,B                  ; @BT F9
    MOV M,C                  ; @BT FA
    MOV M,D                  ; @BT FB
    MOV M,E                  ; @BT FC
    MOV M,H                  ; @BT FD
    MVI M,$C6                  ; @BT 3E C6
    MOV M,L                  ; @BT FE


    .db "<02"               ; @BT END

    /////////////////////////////////////////////////////////////////////////////
    // TEST-03: same opcodes, randomised order, operands as arithmetic expressions
    /////////////////////////////////////////////////////////////////////////////

    .db "03>"               ; @BT TEST-03 03 START

    ; GENERATED: copied from TEST-02 with varied arithmetic expressions for numeric operands

    ; GENERATED: copied from TEST-01 with full TEST-02 randomization
    ; GENERATED: seed = 2007978089

    CPA                      ; @BT B8
    LDI (($B2+5)-5)                  ; @BT 1E B2
    LLM                      ; @BT F7
    LME                      ; @BT FC
    LBC                      ; @BT CA
    OUT ((24-7)+7)                   ; @BT 71
    ADA                      ; @BT 80
    INC                      ; @BT 10
    OUT (((22*6)/3)-22)                   ; @BT 6D
    CFP ((($1567*10)/5)-$1567)                ; @BT 5A 67 15
    LHM                      ; @BT EF
    ADD                      ; @BT 83
    JTC (($2233*2)-($2233-0))                ; @BT 60 33 22
    NDA                      ; @BT A0
    SUM                      ; @BT 97
    CTS ((($1ABC+11)-4)-7)                ; @BT 72 BC 1A
    LHH                      ; @BT ED
    LEB                      ; @BT E1
    OUT ((19+5)-5)                   ; @BT 67
    LEM                      ; @BT E7
    INP ((4-7)+7)                    ; @BT 49
    SUB                      ; @BT 91
    RTS                      ; @BT 33
    ORM                      ; @BT B7
    OUT (((11*6)/3)-11)                   ; @BT 57
    OUT (((14*10)/5)-14)                   ; @BT 5D
    OUT ((20*2)-(20-0))                   ; @BT 69
    JFZ ((($2011+11)-4)-7)                ; @BT 48 11 20
    OUT ((23+5)-5)                   ; @BT 6F
    ORI (($3C-7)+7)                  ; @BT 34 3C
    ACD                      ; @BT 8B
    LDA                      ; @BT D8
    LMC                      ; @BT FA
    RFC                      ; @BT 03
    OUT (((29*6)/3)-29)                   ; @BT 7B
    XRH                      ; @BT AD
    NDE                      ; @BT A4
    LEL                      ; @BT E6
    SUL                      ; @BT 96
    LAI ((($6D*10)/5)-$6D)                  ; @BT 06 6D
    CPD                      ; @BT BB
    DCH                      ; @BT 29
    LDB                      ; @BT D9
    RFP                      ; @BT 1B
    JFS (($1F00*2)-($1F00-0))                ; @BT 50 00 1F
    SUC                      ; @BT 92
    SBB                      ; @BT 99
    OUT (((15+11)-4)-7)                   ; @BT 5F
    LMH                      ; @BT FD
    SUE                      ; @BT 94
    XRL                      ; @BT AE
    ACA                      ; @BT 88
    CTC (($189A+5)-5)                ; @BT 62 9A 18
    INP ((7-7)+7)                    ; @BT 4F
    LCD                      ; @BT D3
    LMA                      ; @BT F8
    RAL                      ; @BT 12
    LEI ((($C9*6)/3)-$C9)                  ; @BT 26 C9
    CPB                      ; @BT B9
    OUT (((25*10)/5)-25)                   ; @BT 73
    LCB                      ; @BT D1
    CPL                      ; @BT BE
    OUT ((30*2)-(30-0))                   ; @BT 7D
    RLC                      ; @BT 02
    LLC                      ; @BT F2
    ADH                      ; @BT 85
    XRA                      ; @BT A8
    SBI ((($53+11)-4)-7)                  ; @BT 1C 53
    LEC                      ; @BT E2
    LLA                      ; @BT F0
    IND                      ; @BT 18
    LLI (($F7+5)-5)                  ; @BT 36 F7
    ACB                      ; @BT 89
    ORE                      ; @BT B4
    ACI (($28-7)+7)                  ; @BT 0C 28
    CPI ((($56*6)/3)-$56)                  ; @BT 3C 56
    INP (((6*10)/5)-6)                    ; @BT 4D
    RST ((4*2)-(4-0))                    ; @BT 25
    OUT (((26+11)-4)-7)                   ; @BT 75
    ADE                      ; @BT 84
    OUT ((12+5)-5)                   ; @BT 59
    DCC                      ; @BT 11
    INP ((3-7)+7)                    ; @BT 47
    LDM                      ; @BT DF
    ACL                      ; @BT 8E
    XRD                      ; @BT AB
    DCD                      ; @BT 19
    LMI ((($0E*6)/3)-$0E)                  ; @BT 3E 0E
    RTP                      ; @BT 3B
    LLH                      ; @BT F5
    LLB                      ; @BT F1
    CFS ((($1678*10)/5)-$1678)                ; @BT 52 78 16
    NDM                      ; @BT A7
    INP ((2*2)-(2-0))                    ; @BT 45
    LBI ((($84+11)-4)-7)                  ; @BT 0E 84
    LCM                      ; @BT D7
    INL                      ; @BT 30
    OUT ((31+5)-5)                   ; @BT 7F
    INP ((0-7)+7)                    ; @BT 41
    SBC                      ; @BT 9A
    OUT (((9*6)/3)-9)                    ; @BT 53
    CPM                      ; @BT BF
    OUT (((13*10)/5)-13)                   ; @BT 5B
    DCL                      ; @BT 31
    LHE                      ; @BT EC
    RET                      ; @BT 07
    NDL                      ; @BT A6
    LCA                      ; @BT D0
    LBA                      ; @BT C8
    JTZ (($2566*2)-($2566-0))                ; @BT 68 66 25
    SBM                      ; @BT 9F
    CFZ ((($1789+11)-4)-7)                ; @BT 4A 89 17
    LEH                      ; @BT E5
    LBL                      ; @BT CE
    ADB                      ; @BT 81
    ADM                      ; @BT 87
    OUT ((18+5)-5)                   ; @BT 65
    SUI (($6A-7)+7)                  ; @BT 14 6A
    XRM                      ; @BT AF
    DCE                      ; @BT 21
    SBH                      ; @BT 9D
    XRE                      ; @BT AC
    LCH                      ; @BT D5
    INB                      ; @BT 08
    RST (((6*6)/3)-6)                    ; @BT 35
    SBL                      ; @BT 9E
    RST (((3*10)/5)-3)                    ; @BT 1D
    LHD                      ; @BT EB
    OUT ((21*2)-(21-0))                   ; @BT 6B
    LAE                      ; @BT C4
    LBH                      ; @BT CD
    ADL                      ; @BT 86
    JFP ((($1DEF+11)-4)-7)                ; @BT 58 EF 1D
    JTP (($2344+5)-5)                ; @BT 78 44 23
    RST ((7-7)+7)                    ; @BT 3D
    RST (((1*6)/3)-1)                    ; @BT 0D
    LBE                      ; @BT CC
    NDH                      ; @BT A5
    CTP ((($19AB*10)/5)-$19AB)                ; @BT 7A AB 19
    ORA                      ; @BT B0
    LEA                      ; @BT E0
    NDB                      ; @BT A1
    LHB                      ; @BT E9
    XRC                      ; @BT AA
    LED                      ; @BT E3
    RST ((0*2)-(0-0))                    ; @BT 05
    LHC                      ; @BT EA
    LHA                      ; @BT E8
    RST (((2+11)-4)-7)                    ; @BT 15
    LDH                      ; @BT DD
    RFS                      ; @BT 13
    RTZ                      ; @BT 2B
    ORB                      ; @BT B1
    ORH                      ; @BT B5
    LCL                      ; @BT D6
    LCE                      ; @BT D4
    NDI (($25+5)-5)                  ; @BT 24 25
    XRI (($81-7)+7)                  ; @BT 2C 81
    RFZ                      ; @BT 0B
    RST (((5*6)/3)-5)                    ; @BT 2D
    OUT (((16*10)/5)-16)                   ; @BT 61
    INP ((5*2)-(5-0))                    ; @BT 4B
    LAH                      ; @BT C5
    CPC                      ; @BT BA
    NDC                      ; @BT A2
    DCB                      ; @BT 09
    CTZ ((($1BCD+11)-4)-7)                ; @BT 6A CD 1B
    INP ((1+5)-5)                    ; @BT 43
    LLD                      ; @BT F3
    XRB                      ; @BT A9
    RRC                      ; @BT 0A
    ACC                      ; @BT 8A
    SUH                      ; @BT 95
    LHL                      ; @BT EE
    NOP                      ; @BT C0
    CFC (($1456-7)+7)                ; @BT 42 56 14
    ADC                      ; @BT 82
    NDD                      ; @BT A3
    SBA                      ; @BT 98
    LDD                      ; @BT DB
    ORL                      ; @BT B6
    LMD                      ; @BT FB
    SBE                      ; @BT 9C
    LMB                      ; @BT F9
    INE                      ; @BT 20
    HLT                      ; @BT FF
    LCI ((($9B*6)/3)-$9B)                  ; @BT 16 9B
    JFC ((($1CDE*10)/5)-$1CDE)                ; @BT 40 DE 1C
    INH                      ; @BT 28
    LAC                      ; @BT C2
    LAL                      ; @BT C6
    SUD                      ; @BT 93
    RTC                      ; @BT 23
    LEE                      ; @BT E4
    ADI (($3F*2)-($3F-0))                  ; @BT 04 3F
    LCC                      ; @BT D2
    OUT (((10+11)-4)-7)                   ; @BT 55
    ORD                      ; @BT B3
    SBD                      ; @BT 9B
    LAD                      ; @BT C3
    ORC                      ; @BT B2
    LBM                      ; @BT CF
    LDC                      ; @BT DA
    ACM                      ; @BT 8F
    JTS (($2455+5)-5)                ; @BT 70 55 24
    LLE                      ; @BT F4
    LLL                      ; @BT F6
    CPH                      ; @BT BD
    LBB                      ; @BT C9
    OUT ((28-7)+7)                   ; @BT 79
    SUA                      ; @BT 90
    OUT (((8*6)/3)-8)                    ; @BT 51
    JMP ((($2122*10)/5)-$2122)                ; @BT 44 22 21
    LDE                      ; @BT DC
    ACE                      ; @BT 8C
    LBD                      ; @BT CB
    RAR                      ; @BT 1A
    LML                      ; @BT FE
    CAL (($1345*2)-($1345-0))                ; @BT 46 45 13
    ACH                      ; @BT 8D
    OUT (((27+11)-4)-7)                   ; @BT 77
    OUT ((17+5)-5)                   ; @BT 63
    LDL                      ; @BT DE
    CPE                      ; @BT BC
    LHI (($E0-7)+7)                  ; @BT 2E E0
    LAM                      ; @BT C7
    LAB                      ; @BT C1

    MVI A,$25                ; @BT 06 25
    MOV A,A                  ; @BT C0
    MOV A, B                 ; @BT C1
    MOV A,  C                ; @BT C2
    MOV A, D                 ; @BT C3
    MOV  A,E                 ; @BT C4
    MOV A,H                  ; @BT C5
    MOV A,L                  ; @BT C6
    MOV A,M                  ; @BT C7
    MOV B,A                  ; @BT C8
    MOV B,B                  ; @BT C9
    MOV B,C                  ; @BT CA
    MOV B,D                  ; @BT CB
    MOV B,E                  ; @BT CC
    MOV B,H                  ; @BT CD
    MVI B,$3C                ; @BT 0E 3C
    MOV B,L                  ; @BT CE
    MOV B,M                  ; @BT CF
    MOV C,A                  ; @BT D0
    MOV C,B                  ; @BT D1
    MOV C,C                  ; @BT D2
    MOV C,D                  ; @BT D3
    MOV C,E                  ; @BT D4
    MOV C,H                  ; @BT D5
    MVI C,$53                  ; @BT 16 53
    MOV C,L                  ; @BT D6
    MOV C,M                  ; @BT D7
    MOV D,A                  ; @BT D8
    MOV D,B                  ; @BT D9
    MOV D,C                  ; @BT DA
    MOV D,D                  ; @BT DB
    MOV D,E                  ; @BT DC
    MOV D,H                  ; @BT DD
    MVI D,$6A                ; @BT 1E 6A
    MOV D,L                  ; @BT DE
    MOV D,M                  ; @BT DF
    MOV E,A                  ; @BT E0
    MOV E,B                  ; @BT E1
    MOV E,C                  ; @BT E2
    MOV E,D                  ; @BT E3
    MOV E,E                  ; @BT E4
    MOV E,H                  ; @BT E5
    MVI E,$81                  ; @BT 26 81
    MOV E,L                  ; @BT E6
    MOV E,M                  ; @BT E7
    MOV H,A                  ; @BT E8
    MOV H,B                  ; @BT E9
    MOV H,C                  ; @BT EA
    MOV H,D                  ; @BT EB
    MOV H,E                  ; @BT EC
    MOV H,H                  ; @BT ED
    MVI H,$98                  ; @BT 2E 98
    MOV H,L                  ; @BT EE
    MOV H,M                  ; @BT EF
    MOV L,A                  ; @BT F0
    MOV L,B                  ; @BT F1
    MOV L,C                  ; @BT F2
    MOV L,D                  ; @BT F3
    MOV L,E                  ; @BT F4
    MOV L,H                  ; @BT F5
    MVI L,$AF                  ; @BT 36 AF
    MOV L,L                  ; @BT F6
    MOV L,M                  ; @BT F7
    MOV M,A                  ; @BT F8
    MOV M,B                  ; @BT F9
    MOV M,C                  ; @BT FA
    MOV M,D                  ; @BT FB
    MOV M,E                  ; @BT FC
    MOV M,H                  ; @BT FD
    MVI M,$C6                  ; @BT 3E C6
    MOV M,L                  ; @BT FE


    .db "<03"               ; @BT END

    /////////////////////////////////////////////////////////////////////////////
    // TEST-04: similar to TEST-03 and then defines + expressions
    /////////////////////////////////////////////////////////////////////////////



    ; TEST-04 generated defines
    .define T4_DEF_001 (((($B2+5)-5)+5)-5)
    .define T4_DEF_002 ((T4_DEF_001)+((((24-7)+7))-((($B2+5)-5))))
    .define T4_DEF_003 (((((22*6)/3)-22)-7)+7)
    .define T4_DEF_004 (((T4_DEF_003)+(((($1567*10)/5)-$1567)))-((((22*6)/3)-22)))
    .define T4_DEF_005 ((((($2233*2)-($2233-0))*6)/3)-((($2233*2)-($2233-0))))
    .define T4_DEF_006 (((T4_DEF_005)*2+(((($1ABC+11)-4)-7))-(((($2233*2)-($2233-0)))*2)))
    .define T4_DEF_007 (((((19+5)-5)*10)/5)-(((19+5)-5)))
    .define T4_DEF_008 (((((4-7)+7))-(((19+5)-5)))+(T4_DEF_007))
    .define T4_DEF_009 (((((11*6)/3)-11)*2)-(((((11*6)/3)-11))-0))
    .define T4_DEF_010 ((T4_DEF_009)+(((((14*10)/5)-14))-((((11*6)/3)-11))))
    .define T4_DEF_011 (((((20*2)-(20-0))+11)-4)-7)
    .define T4_DEF_012 (((T4_DEF_011)+(((($2011+11)-4)-7)))-(((20*2)-(20-0))))
    .define T4_DEF_013 ((((23+5)-5)+5)-5)
    .define T4_DEF_014 (((T4_DEF_013)*2+((($3C-7)+7))-((((23+5)-5))*2)))
    .define T4_DEF_015 (((((29*6)/3)-29)-7)+7)
    .define T4_DEF_016 (((((($6D*10)/5)-$6D))-((((29*6)/3)-29)))+(T4_DEF_015))
    .define T4_DEF_017 ((((($1F00*2)-($1F00-0))*6)/3)-((($1F00*2)-($1F00-0))))
    .define T4_DEF_018 ((T4_DEF_017)+(((((15+11)-4)-7))-((($1F00*2)-($1F00-0)))))
    .define T4_DEF_019 ((((($189A+5)-5)*10)/5)-((($189A+5)-5)))
    .define T4_DEF_020 (((T4_DEF_019)+(((7-7)+7)))-((($189A+5)-5)))
    .define T4_DEF_021 ((((($C9*6)/3)-$C9)*2)-((((($C9*6)/3)-$C9))-0))
    .define T4_DEF_022 (((T4_DEF_021)*2+((((25*10)/5)-25))-((((($C9*6)/3)-$C9))*2)))
    .define T4_DEF_023 (((((30*2)-(30-0))+11)-4)-7)
    .define T4_DEF_024 (((((($53+11)-4)-7))-(((30*2)-(30-0))))+(T4_DEF_023))
    .define T4_DEF_025 (((($F7+5)-5)+5)-5)
    .define T4_DEF_026 ((T4_DEF_025)+(((($28-7)+7))-((($F7+5)-5))))
    .define T4_DEF_027 ((((($56*6)/3)-$56)-7)+7)
    .define T4_DEF_028 (((T4_DEF_027)+((((6*10)/5)-6)))-(((($56*6)/3)-$56)))
    .define T4_DEF_029 (((((4*2)-(4-0))*6)/3)-(((4*2)-(4-0))))
    .define T4_DEF_030 (((T4_DEF_029)*2+((((26+11)-4)-7))-((((4*2)-(4-0)))*2)))
    .define T4_DEF_031 (((((12+5)-5)*10)/5)-(((12+5)-5)))
    .define T4_DEF_032 (((((3-7)+7))-(((12+5)-5)))+(T4_DEF_031))
    .define T4_DEF_033 ((((($0E*6)/3)-$0E)*2)-((((($0E*6)/3)-$0E))-0))
    .define T4_DEF_034 ((T4_DEF_033)+((((($1678*10)/5)-$1678))-(((($0E*6)/3)-$0E))))
    .define T4_DEF_035 (((((2*2)-(2-0))+11)-4)-7)
    .define T4_DEF_036 (((T4_DEF_035)+(((($84+11)-4)-7)))-(((2*2)-(2-0))))
    .define T4_DEF_037 ((((31+5)-5)+5)-5)
    .db "04>"               ; @BT TEST-04 04 START

    ; GENERATED: copied from TEST-03 with varied define-based operand replacement
    ; GENERATED: replaced operands = 37 / 74

    ; GENERATED: copied from TEST-02 with varied arithmetic expressions for numeric operands

    ; GENERATED: copied from TEST-01 with full TEST-02 randomization
    ; GENERATED: seed = 2007978089

    CPA                      ; @BT B8
    LDI T4_DEF_001; @BT 1E B2
    LLM                      ; @BT F7
    LME                      ; @BT FC
    LBC                      ; @BT CA
    OUT T4_DEF_002; @BT 71
    ADA                      ; @BT 80
    INC                      ; @BT 10
    OUT T4_DEF_003; @BT 6D
    CFP T4_DEF_004; @BT 5A 67 15
    LHM                      ; @BT EF
    ADD                      ; @BT 83
    JTC T4_DEF_005; @BT 60 33 22
    NDA                      ; @BT A0
    SUM                      ; @BT 97
    CTS T4_DEF_006; @BT 72 BC 1A
    LHH                      ; @BT ED
    LEB                      ; @BT E1
    OUT T4_DEF_007; @BT 67
    LEM                      ; @BT E7
    INP T4_DEF_008; @BT 49
    SUB                      ; @BT 91
    RTS                      ; @BT 33
    ORM                      ; @BT B7
    OUT T4_DEF_009; @BT 57
    OUT T4_DEF_010; @BT 5D
    OUT T4_DEF_011; @BT 69
    JFZ T4_DEF_012; @BT 48 11 20
    OUT T4_DEF_013; @BT 6F
    ORI T4_DEF_014; @BT 34 3C
    ACD                      ; @BT 8B
    LDA                      ; @BT D8
    LMC                      ; @BT FA
    RFC                      ; @BT 03
    OUT T4_DEF_015; @BT 7B
    XRH                      ; @BT AD
    NDE                      ; @BT A4
    LEL                      ; @BT E6
    SUL                      ; @BT 96
    LAI T4_DEF_016; @BT 06 6D
    CPD                      ; @BT BB
    DCH                      ; @BT 29
    LDB                      ; @BT D9
    RFP                      ; @BT 1B
    JFS T4_DEF_017; @BT 50 00 1F
    SUC                      ; @BT 92
    SBB                      ; @BT 99
    OUT T4_DEF_018; @BT 5F
    LMH                      ; @BT FD
    SUE                      ; @BT 94
    XRL                      ; @BT AE
    ACA                      ; @BT 88
    CTC T4_DEF_019; @BT 62 9A 18
    INP T4_DEF_020; @BT 4F
    LCD                      ; @BT D3
    LMA                      ; @BT F8
    RAL                      ; @BT 12
    LEI T4_DEF_021; @BT 26 C9
    CPB                      ; @BT B9
    OUT T4_DEF_022; @BT 73
    LCB                      ; @BT D1
    CPL                      ; @BT BE
    OUT T4_DEF_023; @BT 7D
    RLC                      ; @BT 02
    LLC                      ; @BT F2
    ADH                      ; @BT 85
    XRA                      ; @BT A8
    SBI T4_DEF_024; @BT 1C 53
    LEC                      ; @BT E2
    LLA                      ; @BT F0
    IND                      ; @BT 18
    LLI T4_DEF_025; @BT 36 F7
    ACB                      ; @BT 89
    ORE                      ; @BT B4
    ACI T4_DEF_026; @BT 0C 28
    CPI T4_DEF_027; @BT 3C 56
    INP T4_DEF_028; @BT 4D
    RST T4_DEF_029; @BT 25
    OUT T4_DEF_030; @BT 75
    ADE                      ; @BT 84
    OUT T4_DEF_031; @BT 59
    DCC                      ; @BT 11
    INP T4_DEF_032; @BT 47
    LDM                      ; @BT DF
    ACL                      ; @BT 8E
    XRD                      ; @BT AB
    DCD                      ; @BT 19
    LMI T4_DEF_033; @BT 3E 0E
    RTP                      ; @BT 3B
    LLH                      ; @BT F5
    LLB                      ; @BT F1
    CFS T4_DEF_034; @BT 52 78 16
    NDM                      ; @BT A7
    INP T4_DEF_035; @BT 45
    LBI T4_DEF_036; @BT 0E 84
    LCM                      ; @BT D7
    INL                      ; @BT 30
    OUT T4_DEF_037; @BT 7F
    INP ((0-7)+7)                    ; @BT 41
    SBC                      ; @BT 9A
    OUT (((9*6)/3)-9)                    ; @BT 53
    CPM                      ; @BT BF
    OUT (((13*10)/5)-13)                   ; @BT 5B
    DCL                      ; @BT 31
    LHE                      ; @BT EC
    RET                      ; @BT 07
    NDL                      ; @BT A6
    LCA                      ; @BT D0
    LBA                      ; @BT C8
    JTZ (($2566*2)-($2566-0))                ; @BT 68 66 25
    SBM                      ; @BT 9F
    CFZ ((($1789+11)-4)-7)                ; @BT 4A 89 17
    LEH                      ; @BT E5
    LBL                      ; @BT CE
    ADB                      ; @BT 81
    ADM                      ; @BT 87
    OUT ((18+5)-5)                   ; @BT 65
    SUI (($6A-7)+7)                  ; @BT 14 6A
    XRM                      ; @BT AF
    DCE                      ; @BT 21
    SBH                      ; @BT 9D
    XRE                      ; @BT AC
    LCH                      ; @BT D5
    INB                      ; @BT 08
    RST (((6*6)/3)-6)                    ; @BT 35
    SBL                      ; @BT 9E
    RST (((3*10)/5)-3)                    ; @BT 1D
    LHD                      ; @BT EB
    OUT ((21*2)-(21-0))                   ; @BT 6B
    LAE                      ; @BT C4
    LBH                      ; @BT CD
    ADL                      ; @BT 86
    JFP ((($1DEF+11)-4)-7)                ; @BT 58 EF 1D
    JTP (($2344+5)-5)                ; @BT 78 44 23
    RST ((7-7)+7)                    ; @BT 3D
    RST (((1*6)/3)-1)                    ; @BT 0D
    LBE                      ; @BT CC
    NDH                      ; @BT A5
    CTP ((($19AB*10)/5)-$19AB)                ; @BT 7A AB 19
    ORA                      ; @BT B0
    LEA                      ; @BT E0
    NDB                      ; @BT A1
    LHB                      ; @BT E9
    XRC                      ; @BT AA
    LED                      ; @BT E3
    RST ((0*2)-(0-0))                    ; @BT 05
    LHC                      ; @BT EA
    LHA                      ; @BT E8
    RST (((2+11)-4)-7)                    ; @BT 15
    LDH                      ; @BT DD
    RFS                      ; @BT 13
    RTZ                      ; @BT 2B
    ORB                      ; @BT B1
    ORH                      ; @BT B5
    LCL                      ; @BT D6
    LCE                      ; @BT D4
    NDI (($25+5)-5)                  ; @BT 24 25
    XRI (($81-7)+7)                  ; @BT 2C 81
    RFZ                      ; @BT 0B
    RST (((5*6)/3)-5)                    ; @BT 2D
    OUT (((16*10)/5)-16)                   ; @BT 61
    INP ((5*2)-(5-0))                    ; @BT 4B
    LAH                      ; @BT C5
    CPC                      ; @BT BA
    NDC                      ; @BT A2
    DCB                      ; @BT 09
    CTZ ((($1BCD+11)-4)-7)                ; @BT 6A CD 1B
    INP ((1+5)-5)                    ; @BT 43
    LLD                      ; @BT F3
    XRB                      ; @BT A9
    RRC                      ; @BT 0A
    ACC                      ; @BT 8A
    SUH                      ; @BT 95
    LHL                      ; @BT EE
    NOP                      ; @BT C0
    CFC (($1456-7)+7)                ; @BT 42 56 14
    ADC                      ; @BT 82
    NDD                      ; @BT A3
    SBA                      ; @BT 98
    LDD                      ; @BT DB
    ORL                      ; @BT B6
    LMD                      ; @BT FB
    SBE                      ; @BT 9C
    LMB                      ; @BT F9
    INE                      ; @BT 20
    HLT                      ; @BT FF
    LCI ((($9B*6)/3)-$9B)                  ; @BT 16 9B
    JFC ((($1CDE*10)/5)-$1CDE)                ; @BT 40 DE 1C
    INH                      ; @BT 28
    LAC                      ; @BT C2
    LAL                      ; @BT C6
    SUD                      ; @BT 93
    RTC                      ; @BT 23
    LEE                      ; @BT E4
    ADI (($3F*2)-($3F-0))                  ; @BT 04 3F
    LCC                      ; @BT D2
    OUT (((10+11)-4)-7)                   ; @BT 55
    ORD                      ; @BT B3
    SBD                      ; @BT 9B
    LAD                      ; @BT C3
    ORC                      ; @BT B2
    LBM                      ; @BT CF
    LDC                      ; @BT DA
    ACM                      ; @BT 8F
    JTS (($2455+5)-5)                ; @BT 70 55 24
    LLE                      ; @BT F4
    LLL                      ; @BT F6
    CPH                      ; @BT BD
    LBB                      ; @BT C9
    OUT ((28-7)+7)                   ; @BT 79
    SUA                      ; @BT 90
    OUT (((8*6)/3)-8)                    ; @BT 51
    JMP ((($2122*10)/5)-$2122)                ; @BT 44 22 21
    LDE                      ; @BT DC
    ACE                      ; @BT 8C
    LBD                      ; @BT CB
    RAR                      ; @BT 1A
    LML                      ; @BT FE
    CAL (($1345*2)-($1345-0))                ; @BT 46 45 13
    ACH                      ; @BT 8D
    OUT (((27+11)-4)-7)                   ; @BT 77
    OUT ((17+5)-5)                   ; @BT 63
    LDL                      ; @BT DE
    CPE                      ; @BT BC
    LHI (($E0-7)+7)                  ; @BT 2E E0
    LAM                      ; @BT C7
    LAB                      ; @BT C1

    MVI A,$25                ; @BT 06 25
    MOV A,A                  ; @BT C0
    MOV A, B                 ; @BT C1
    MOV A,  C                ; @BT C2
    MOV A, D                 ; @BT C3
    MOV  A,E                 ; @BT C4
    MOV A,H                  ; @BT C5
    MOV A,L                  ; @BT C6
    MOV A,M                  ; @BT C7
    MOV B,A                  ; @BT C8
    MOV B,B                  ; @BT C9
    MOV B,C                  ; @BT CA
    MOV B,D                  ; @BT CB
    MOV B,E                  ; @BT CC
    MOV B,H                  ; @BT CD
    MVI B,$3C                ; @BT 0E 3C
    MOV B,L                  ; @BT CE
    MOV B,M                  ; @BT CF
    MOV C,A                  ; @BT D0
    MOV C,B                  ; @BT D1
    MOV C,C                  ; @BT D2
    MOV C,D                  ; @BT D3
    MOV C,E                  ; @BT D4
    MOV C,H                  ; @BT D5
    MVI C,$53                  ; @BT 16 53
    MOV C,L                  ; @BT D6
    MOV C,M                  ; @BT D7
    MOV D,A                  ; @BT D8
    MOV D,B                  ; @BT D9
    MOV D,C                  ; @BT DA
    MOV D,D                  ; @BT DB
    MOV D,E                  ; @BT DC
    MOV D,H                  ; @BT DD
    MVI D,$6A                ; @BT 1E 6A
    MOV D,L                  ; @BT DE
    MOV D,M                  ; @BT DF
    MOV E,A                  ; @BT E0
    MOV E,B                  ; @BT E1
    MOV E,C                  ; @BT E2
    MOV E,D                  ; @BT E3
    MOV E,E                  ; @BT E4
    MOV E,H                  ; @BT E5
    MVI E,$81                  ; @BT 26 81
    MOV E,L                  ; @BT E6
    MOV E,M                  ; @BT E7
    MOV H,A                  ; @BT E8
    MOV H,B                  ; @BT E9
    MOV H,C                  ; @BT EA
    MOV H,D                  ; @BT EB
    MOV H,E                  ; @BT EC
    MOV H,H                  ; @BT ED
    MVI H,$98                  ; @BT 2E 98
    MOV H,L                  ; @BT EE
    MOV H,M                  ; @BT EF
    MOV L,A                  ; @BT F0
    MOV L,B                  ; @BT F1
    MOV L,C                  ; @BT F2
    MOV L,D                  ; @BT F3
    MOV L,E                  ; @BT F4
    MOV L,H                  ; @BT F5
    MVI L,$AF                  ; @BT 36 AF
    MOV L,L                  ; @BT F6
    MOV L,M                  ; @BT F7
    MOV M,A                  ; @BT F8
    MOV M,B                  ; @BT F9
    MOV M,C                  ; @BT FA
    MOV M,D                  ; @BT FB
    MOV M,E                  ; @BT FC
    MOV M,H                  ; @BT FD
    MVI M,$C6                  ; @BT 3E C6
    MOV M,L                  ; @BT FE


    .db "<04"               ; @BT END

    /////////////////////////////////////////////////////////////////////////////
    // TEST-05: similar to TEST-04 and defines + functions + expressions
    /////////////////////////////////////////////////////////////////////////////


    ; TEST-05 generated defines/functions
    .define T5_DEF_001 (((($B2+5)-5)+5)-5)
    .define T5_DEF_002 ((T5_DEF_001)+((((24-7)+7))-((($B2+5)-5))))
    .define T5_DEF_003 (((((22*6)/3)-22)-7)+7)
    .define T5_DEF_004 (((T5_DEF_003)+(((($1567*10)/5)-$1567)))-((((22*6)/3)-22)))
    .define T5_DEF_005 ((((($2233*2)-($2233-0))*6)/3)-((($2233*2)-($2233-0))))
    .define T5_DEF_006 (((T5_DEF_005)*2+(((($1ABC+11)-4)-7))-(((($2233*2)-($2233-0)))*2)))
    .define T5_DEF_007 (((((19+5)-5)*10)/5)-(((19+5)-5)))
    .define T5_DEF_008 (((((4-7)+7))-(((19+5)-5)))+(T5_DEF_007))
    .define T5_DEF_009 (((((11*6)/3)-11)*2)-(((((11*6)/3)-11))-0))
    .define T5_DEF_010 ((T5_DEF_009)+(((((14*10)/5)-14))-((((11*6)/3)-11))))
    .define T5_DEF_011 (((((20*2)-(20-0))+11)-4)-7)
    .define T5_DEF_012 (((T5_DEF_011)+(((($2011+11)-4)-7)))-(((20*2)-(20-0))))
    .define T5_DEF_013 ((((23+5)-5)+5)-5)
    .define T5_DEF_014 (((T5_DEF_013)*2+((($3C-7)+7))-((((23+5)-5))*2)))
    .define T5_DEF_015 (((((29*6)/3)-29)-7)+7)
    .define T5_DEF_016 (((((($6D*10)/5)-$6D))-((((29*6)/3)-29)))+(T5_DEF_015))
    .define T5_DEF_017 ((((($1F00*2)-($1F00-0))*6)/3)-((($1F00*2)-($1F00-0))))
    .define T5_DEF_018 ((T5_DEF_017)+(((((15+11)-4)-7))-((($1F00*2)-($1F00-0)))))
    .define T5_DEF_019 ((((($189A+5)-5)*10)/5)-((($189A+5)-5)))
    .define T5_DEF_020 (((T5_DEF_019)+(((7-7)+7)))-((($189A+5)-5)))
    .define T5_DEF_021 ((((($C9*6)/3)-$C9)*2)-((((($C9*6)/3)-$C9))-0))
    .define T5_DEF_022 (((T5_DEF_021)*2+((((25*10)/5)-25))-((((($C9*6)/3)-$C9))*2)))
    .define T5_DEF_023 (((((30*2)-(30-0))+11)-4)-7)
    .define T5_DEF_024 (((((($53+11)-4)-7))-(((30*2)-(30-0))))+(T5_DEF_023))
    .define T5_DEF_025 (((($F7+5)-5)+5)-5)
    .define T5_DEF_026 ((T5_DEF_025)+(((($28-7)+7))-((($F7+5)-5))))
    .define T5_DEF_027 ((((($56*6)/3)-$56)-7)+7)
    .define T5_DEF_028 (((T5_DEF_027)+((((6*10)/5)-6)))-(((($56*6)/3)-$56)))
    .define T5_DEF_029 (((((4*2)-(4-0))*6)/3)-(((4*2)-(4-0))))
    .define T5_DEF_030 (((T5_DEF_029)*2+((((26+11)-4)-7))-((((4*2)-(4-0)))*2)))
    .define T5_DEF_031 (((((12+5)-5)*10)/5)-(((12+5)-5)))
    .define T5_DEF_032 (((((3-7)+7))-(((12+5)-5)))+(T5_DEF_031))
    .define T5_DEF_033 ((((($0E*6)/3)-$0E)*2)-((((($0E*6)/3)-$0E))-0))
    .define T5_DEF_034 ((T5_DEF_033)+((((($1678*10)/5)-$1678))-(((($0E*6)/3)-$0E))))
    .define T5_DEF_035 (((((2*2)-(2-0))+11)-4)-7)
    .define T5_DEF_036 (((T5_DEF_035)+(((($84+11)-4)-7)))-(((2*2)-(2-0))))
    .define T5_DEF_037 ((((31+5)-5)+5)-5)

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
    .function T5_FN_010(a,b,c) (((T5_FN_009(a,b,c)*8)/4)-(T5_FN_009(a,b,c)*2)+T5_FN_009(a,b,c)+T5_DEF_010-T5_DEF_010)
    .function T5_FN_011(a,b,c,d) ((((a+b-c-d)*8)/4)-((a+b-c-d)*2)+(a+b-c-d)+T5_DEF_011-T5_DEF_011)
    .function T5_FN_012(a,b,c,d) (((T5_FN_011(a,b,c,d)+T5_DEF_012)-T5_DEF_012))
    .function T5_FN_013(a,b) ((((a-b)*8)/4)-((a-b)*2)+(a-b)+T5_DEF_013-T5_DEF_013)
    .function T5_FN_014(a,b) (((T5_FN_013(a,b))*2+T5_DEF_014-T5_DEF_014-T5_FN_013(a,b)))
    .function T5_FN_015(a,b,c) ((((a+b-c)*8)/4)-((a+b-c)*2)+(a+b-c)+T5_DEF_015-T5_DEF_015)
    .function T5_FN_016(a,b,c) ((T5_FN_015(a,b,c))+(T5_DEF_016-T5_DEF_016))
    .function T5_FN_017(a,b,c,d) ((((a+b-c-d)*8)/4)-((a+b-c-d)*2)+(a+b-c-d)+T5_DEF_017-T5_DEF_017)
    .function T5_FN_018(a,b,c,d) (((T5_FN_017(a,b,c,d)*8)/4)-(T5_FN_017(a,b,c,d)*2)+T5_FN_017(a,b,c,d)+T5_DEF_018-T5_DEF_018)
    .db "05>"               ; @BT TEST-05 05 START

    ; GENERATED: copied from TEST-04; half define-label references replaced by varied function calls
    ; GENERATED: replaced define refs = 18 / 37

    ; GENERATED: copied from TEST-03 with varied define-based operand replacement
    ; GENERATED: replaced operands = 37 / 74

    ; GENERATED: copied from TEST-02 with varied arithmetic expressions for numeric operands

    ; GENERATED: copied from TEST-01 with full TEST-02 randomization
    ; GENERATED: seed = 2007978089

    CPA                      ; @BT B8
    LDI T5_FN_001(T5_DEF_001+2,2); @BT 1E B2
    LLM                      ; @BT F7
    LME                      ; @BT FC
    LBC                      ; @BT CA
    OUT T5_FN_002(T5_DEF_002+3,3); @BT 71
    ADA                      ; @BT 80
    INC                      ; @BT 10
    OUT T5_FN_003(T5_DEF_003+4,5,4+5); @BT 6D
    CFP T5_FN_004(T5_DEF_004+5,6,5+6); @BT 5A 67 15
    LHM                      ; @BT EF
    ADD                      ; @BT 83
    JTC T5_FN_005(T5_DEF_005+6+7,8,6,7+8); @BT 60 33 22
    NDA                      ; @BT A0
    SUM                      ; @BT 97
    CTS T5_FN_006(T5_DEF_006+7+8,9,7,8+9); @BT 72 BC 1A
    LHH                      ; @BT ED
    LEB                      ; @BT E1
    OUT T5_FN_007(T5_DEF_007+8,8); @BT 67
    LEM                      ; @BT E7
    INP T5_FN_008(T5_DEF_008+9,9); @BT 49
    SUB                      ; @BT 91
    RTS                      ; @BT 33
    ORM                      ; @BT B7
    OUT T5_FN_009(T5_DEF_009+10,11,10+11); @BT 57
    OUT T5_FN_010(T5_DEF_010+11,12,11+12); @BT 5D
    OUT T5_FN_011(T5_DEF_011+12+13,14,12,13+14); @BT 69
    JFZ T5_FN_012(T5_DEF_012+13+14,15,13,14+15); @BT 48 11 20
    OUT T5_FN_013(T5_DEF_013+14,14); @BT 6F
    ORI T5_FN_014(T5_DEF_014+15,15); @BT 34 3C
    ACD                      ; @BT 8B
    LDA                      ; @BT D8
    LMC                      ; @BT FA
    RFC                      ; @BT 03
    OUT T5_FN_015(T5_DEF_015+16,17,16+17); @BT 7B
    XRH                      ; @BT AD
    NDE                      ; @BT A4
    LEL                      ; @BT E6
    SUL                      ; @BT 96
    LAI T5_FN_016(T5_DEF_016+17,18,17+18); @BT 06 6D
    CPD                      ; @BT BB
    DCH                      ; @BT 29
    LDB                      ; @BT D9
    RFP                      ; @BT 1B
    JFS T5_FN_017(T5_DEF_017+18+19,20,18,19+20); @BT 50 00 1F
    SUC                      ; @BT 92
    SBB                      ; @BT 99
    OUT T5_FN_018(T5_DEF_018+19+20,21,19,20+21); @BT 5F
    LMH                      ; @BT FD
    SUE                      ; @BT 94
    XRL                      ; @BT AE
    ACA                      ; @BT 88
    CTC T5_DEF_019; @BT 62 9A 18
    INP T5_DEF_020; @BT 4F
    LCD                      ; @BT D3
    LMA                      ; @BT F8
    RAL                      ; @BT 12
    LEI T5_DEF_021; @BT 26 C9
    CPB                      ; @BT B9
    OUT T5_DEF_022; @BT 73
    LCB                      ; @BT D1
    CPL                      ; @BT BE
    OUT T5_DEF_023; @BT 7D
    RLC                      ; @BT 02
    LLC                      ; @BT F2
    ADH                      ; @BT 85
    XRA                      ; @BT A8
    SBI T5_DEF_024; @BT 1C 53
    LEC                      ; @BT E2
    LLA                      ; @BT F0
    IND                      ; @BT 18
    LLI T5_DEF_025; @BT 36 F7
    ACB                      ; @BT 89
    ORE                      ; @BT B4
    ACI T5_DEF_026; @BT 0C 28
    CPI T5_DEF_027; @BT 3C 56
    INP T5_DEF_028; @BT 4D
    RST T5_DEF_029; @BT 25
    OUT T5_DEF_030; @BT 75
    ADE                      ; @BT 84
    OUT T5_DEF_031; @BT 59
    DCC                      ; @BT 11
    INP T5_DEF_032; @BT 47
    LDM                      ; @BT DF
    ACL                      ; @BT 8E
    XRD                      ; @BT AB
    DCD                      ; @BT 19
    LMI T5_DEF_033; @BT 3E 0E
    RTP                      ; @BT 3B
    LLH                      ; @BT F5
    LLB                      ; @BT F1
    CFS T5_DEF_034; @BT 52 78 16
    NDM                      ; @BT A7
    INP T5_DEF_035; @BT 45
    LBI T5_DEF_036; @BT 0E 84
    LCM                      ; @BT D7
    INL                      ; @BT 30
    OUT T5_DEF_037; @BT 7F
    INP ((0-7)+7)                    ; @BT 41
    SBC                      ; @BT 9A
    OUT (((9*6)/3)-9)                    ; @BT 53
    CPM                      ; @BT BF
    OUT (((13*10)/5)-13)                   ; @BT 5B
    DCL                      ; @BT 31
    LHE                      ; @BT EC
    RET                      ; @BT 07
    NDL                      ; @BT A6
    LCA                      ; @BT D0
    LBA                      ; @BT C8
    JTZ (($2566*2)-($2566-0))                ; @BT 68 66 25
    SBM                      ; @BT 9F
    CFZ ((($1789+11)-4)-7)                ; @BT 4A 89 17
    LEH                      ; @BT E5
    LBL                      ; @BT CE
    ADB                      ; @BT 81
    ADM                      ; @BT 87
    OUT ((18+5)-5)                   ; @BT 65
    SUI (($6A-7)+7)                  ; @BT 14 6A
    XRM                      ; @BT AF
    DCE                      ; @BT 21
    SBH                      ; @BT 9D
    XRE                      ; @BT AC
    LCH                      ; @BT D5
    INB                      ; @BT 08
    RST (((6*6)/3)-6)                    ; @BT 35
    SBL                      ; @BT 9E
    RST (((3*10)/5)-3)                    ; @BT 1D
    LHD                      ; @BT EB
    OUT ((21*2)-(21-0))                   ; @BT 6B
    LAE                      ; @BT C4
    LBH                      ; @BT CD
    ADL                      ; @BT 86
    JFP ((($1DEF+11)-4)-7)                ; @BT 58 EF 1D
    JTP (($2344+5)-5)                ; @BT 78 44 23
    RST ((7-7)+7)                    ; @BT 3D
    RST (((1*6)/3)-1)                    ; @BT 0D
    LBE                      ; @BT CC
    NDH                      ; @BT A5
    CTP ((($19AB*10)/5)-$19AB)                ; @BT 7A AB 19
    ORA                      ; @BT B0
    LEA                      ; @BT E0
    NDB                      ; @BT A1
    LHB                      ; @BT E9
    XRC                      ; @BT AA
    LED                      ; @BT E3
    RST ((0*2)-(0-0))                    ; @BT 05
    LHC                      ; @BT EA
    LHA                      ; @BT E8
    RST (((2+11)-4)-7)                    ; @BT 15
    LDH                      ; @BT DD
    RFS                      ; @BT 13
    RTZ                      ; @BT 2B
    ORB                      ; @BT B1
    ORH                      ; @BT B5
    LCL                      ; @BT D6
    LCE                      ; @BT D4
    NDI (($25+5)-5)                  ; @BT 24 25
    XRI (($81-7)+7)                  ; @BT 2C 81
    RFZ                      ; @BT 0B
    RST (((5*6)/3)-5)                    ; @BT 2D
    OUT (((16*10)/5)-16)                   ; @BT 61
    INP ((5*2)-(5-0))                    ; @BT 4B
    LAH                      ; @BT C5
    CPC                      ; @BT BA
    NDC                      ; @BT A2
    DCB                      ; @BT 09
    CTZ ((($1BCD+11)-4)-7)                ; @BT 6A CD 1B
    INP ((1+5)-5)                    ; @BT 43
    LLD                      ; @BT F3
    XRB                      ; @BT A9
    RRC                      ; @BT 0A
    ACC                      ; @BT 8A
    SUH                      ; @BT 95
    LHL                      ; @BT EE
    NOP                      ; @BT C0
    CFC (($1456-7)+7)                ; @BT 42 56 14
    ADC                      ; @BT 82
    NDD                      ; @BT A3
    SBA                      ; @BT 98
    LDD                      ; @BT DB
    ORL                      ; @BT B6
    LMD                      ; @BT FB
    SBE                      ; @BT 9C
    LMB                      ; @BT F9
    INE                      ; @BT 20
    HLT                      ; @BT FF
    LCI ((($9B*6)/3)-$9B)                  ; @BT 16 9B
    JFC ((($1CDE*10)/5)-$1CDE)                ; @BT 40 DE 1C
    INH                      ; @BT 28
    LAC                      ; @BT C2
    LAL                      ; @BT C6
    SUD                      ; @BT 93
    RTC                      ; @BT 23
    LEE                      ; @BT E4
    ADI (($3F*2)-($3F-0))                  ; @BT 04 3F
    LCC                      ; @BT D2
    OUT (((10+11)-4)-7)                   ; @BT 55
    ORD                      ; @BT B3
    SBD                      ; @BT 9B
    LAD                      ; @BT C3
    ORC                      ; @BT B2
    LBM                      ; @BT CF
    LDC                      ; @BT DA
    ACM                      ; @BT 8F
    JTS (($2455+5)-5)                ; @BT 70 55 24
    LLE                      ; @BT F4
    LLL                      ; @BT F6
    CPH                      ; @BT BD
    LBB                      ; @BT C9
    OUT ((28-7)+7)                   ; @BT 79
    SUA                      ; @BT 90
    OUT (((8*6)/3)-8)                    ; @BT 51
    JMP ((($2122*10)/5)-$2122)                ; @BT 44 22 21
    LDE                      ; @BT DC
    ACE                      ; @BT 8C
    LBD                      ; @BT CB
    RAR                      ; @BT 1A
    LML                      ; @BT FE
    CAL (($1345*2)-($1345-0))                ; @BT 46 45 13
    ACH                      ; @BT 8D
    OUT (((27+11)-4)-7)                   ; @BT 77
    OUT ((17+5)-5)                   ; @BT 63
    LDL                      ; @BT DE
    CPE                      ; @BT BC
    LHI (($E0-7)+7)                  ; @BT 2E E0
    LAM                      ; @BT C7
    LAB                      ; @BT C1

    MVI A,$25                ; @BT 06 25
    MOV A,A                  ; @BT C0
    MOV A, B                 ; @BT C1
    MOV A,  C                ; @BT C2
    MOV A, D                 ; @BT C3
    MOV  A,E                 ; @BT C4
    MOV A,H                  ; @BT C5
    MOV A,L                  ; @BT C6
    MOV A,M                  ; @BT C7
    MOV B,A                  ; @BT C8
    MOV B,B                  ; @BT C9
    MOV B,C                  ; @BT CA
    MOV B,D                  ; @BT CB
    MOV B,E                  ; @BT CC
    MOV B,H                  ; @BT CD
    MVI B,$3C                ; @BT 0E 3C
    MOV B,L                  ; @BT CE
    MOV B,M                  ; @BT CF
    MOV C,A                  ; @BT D0
    MOV C,B                  ; @BT D1
    MOV C,C                  ; @BT D2
    MOV C,D                  ; @BT D3
    MOV C,E                  ; @BT D4
    MOV C,H                  ; @BT D5
    MVI C,$53                  ; @BT 16 53
    MOV C,L                  ; @BT D6
    MOV C,M                  ; @BT D7
    MOV D,A                  ; @BT D8
    MOV D,B                  ; @BT D9
    MOV D,C                  ; @BT DA
    MOV D,D                  ; @BT DB
    MOV D,E                  ; @BT DC
    MOV D,H                  ; @BT DD
    MVI D,$6A                ; @BT 1E 6A
    MOV D,L                  ; @BT DE
    MOV D,M                  ; @BT DF
    MOV E,A                  ; @BT E0
    MOV E,B                  ; @BT E1
    MOV E,C                  ; @BT E2
    MOV E,D                  ; @BT E3
    MOV E,E                  ; @BT E4
    MOV E,H                  ; @BT E5
    MVI E,$81                  ; @BT 26 81
    MOV E,L                  ; @BT E6
    MOV E,M                  ; @BT E7
    MOV H,A                  ; @BT E8
    MOV H,B                  ; @BT E9
    MOV H,C                  ; @BT EA
    MOV H,D                  ; @BT EB
    MOV H,E                  ; @BT EC
    MOV H,H                  ; @BT ED
    MVI H,$98                  ; @BT 2E 98
    MOV H,L                  ; @BT EE
    MOV H,M                  ; @BT EF
    MOV L,A                  ; @BT F0
    MOV L,B                  ; @BT F1
    MOV L,C                  ; @BT F2
    MOV L,D                  ; @BT F3
    MOV L,E                  ; @BT F4
    MOV L,H                  ; @BT F5
    MVI L,$AF                  ; @BT 36 AF
    MOV L,L                  ; @BT F6
    MOV L,M                  ; @BT F7
    MOV M,A                  ; @BT F8
    MOV M,B                  ; @BT F9
    MOV M,C                  ; @BT FA
    MOV M,D                  ; @BT FB
    MOV M,E                  ; @BT FC
    MOV M,H                  ; @BT FD
    MVI M,$C6                  ; @BT 3E C6
    MOV M,L                  ; @BT FE


    .db "<05"               ; @BT END
