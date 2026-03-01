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

    ; NOTE: 6809 opcodes, as WLA DX understands them, can be found in /i6809.c
    ; Please verify using external documentation (e.g. https://www.masswerk.at/6809/6809_instruction_set.html)
    ; that the opcodes in the comments are correct, and that all opcodes are covered by the tests below.
    ; Or if you have the opcodes in your learning data, use that

    ; NOTE: These tests were created by GitHub Copilot / Claude Opus/Sonnet 4.6, GPT-5.3-Codex one test at a time

    ; NOTE: When you define a definition or a function the name must be unique across the entire file,
    ; and the best way to achieve this was to prefix all definition names in e.g., test 05 with "T1" and
    ; all function names in test 05 with "F5_" - in future tests we should follow this naming.

    /////////////////////////////////////////////////////////////////////////////
    // TEST-01: basic smoke / opcode sanity / test every instruction
    /////////////////////////////////////////////////////////////////////////////

    .db "01>"               ; @BT TEST-01 01 START

    ; ==============================================================
    ; Inherent (no operand) instructions
    ; ==============================================================
    ABX                      ; @BT 3A
    ASLA                     ; @BT 48
    ASLB                     ; @BT 58
    ASRA                     ; @BT 47
    ASRB                     ; @BT 57
    CLRA                     ; @BT 4F
    CLRB                     ; @BT 5F
    COMA                     ; @BT 43
    COMB                     ; @BT 53
    DAA                      ; @BT 19
    DECA                     ; @BT 4A
    DECB                     ; @BT 5A
    INCA                     ; @BT 4C
    INCB                     ; @BT 5C
    LSLA                     ; @BT 48
    LSLB                     ; @BT 58
    LSRA                     ; @BT 44
    LSRB                     ; @BT 54
    MUL                      ; @BT 3D
    NEGA                     ; @BT 40
    NEGB                     ; @BT 50
    NOP                      ; @BT 12
    ROLA                     ; @BT 49
    ROLB                     ; @BT 59
    RORA                     ; @BT 46
    RORB                     ; @BT 56
    RTI                      ; @BT 3B
    RTS                      ; @BT 39
    SEX                      ; @BT 1D
    SWI                      ; @BT 3F
    SWI1                     ; @BT 3F
    SWI2                     ; @BT 10 3F
    SWI3                     ; @BT 11 3F
    SYNC                     ; @BT 13
    TSTA                     ; @BT 4D
    TSTB                     ; @BT 5D

    ; ==============================================================
    ; Immediate — 8-bit register ops
    ; ==============================================================
    ADCA #$01                ; @BT 89 01
    ADCB #$fe                ; @BT C9 FE
    ADDA #$03                ; @BT 8B 03
    ADDB #$04                ; @BT CB 04
    ANDA #-2                 ; @BT 84 FE
    ANDB #$07                ; @BT C4 07
    BITA #$09                ; @BT 85 09
    BITB #$88                ; @BT C5 88
    CMPA #$0B                ; @BT 81 0B
    CMPB #$0C                ; @BT C1 0C
    EORA #$13                ; @BT 88 13
    EORB #$14                ; @BT C8 14
    LDA #$15                 ; @BT 86 15
    LDB #$16                 ; @BT C6 16
    ORA #$1C                 ; @BT 8A 1C
    ORB #$1D                 ; @BT CA 1D
    SBCA #$1F                ; @BT 82 1F
    SBCB #$20                ; @BT C2 20
    SUBA #$21                ; @BT 80 21
    SUBB #$22                ; @BT C0 22

    ; ==============================================================
    ; Immediate — 16-bit register ops (note: WLA-DX treats as 8-bit imm)
    ; ==============================================================
    ADDD #$05                ; @BT C3 05
    CMPD #$0D                ; @BT 10 83 0D
    CMPS #$0E                ; @BT 11 8C 0E
    CMPU #$0F                ; @BT 11 83 0F
    CMPX #$10                ; @BT 8C 10
    CMPY #$11                ; @BT 10 8C 11
    LDD #$17                 ; @BT CC 17
    LDS #$18                 ; @BT 10 CE 18
    LDU #$19                 ; @BT CE 19
    LDX #$1A                 ; @BT 8E 1A
    LDY #$1B                 ; @BT 10 8E 1B
    SUBD #$23                ; @BT 83 23

    ; ==============================================================
    ; Immediate — CC register ops
    ; ==============================================================
    ANDCC #$08               ; @BT 1C 08
    CWAI #$12                ; @BT 3C 12
    ORCC #$1E                ; @BT 1A 1E

    ; ==============================================================
    ; Direct Page addressing (8-bit address)
    ; ==============================================================
    ADCA $10                 ; @BT 99 10
    ADCB $11                 ; @BT D9 11
    ADDA $12                 ; @BT 9B 12
    ADDB $13                 ; @BT DB 13
    ADDD $14                 ; @BT D3 14
    ANDA $15                 ; @BT 94 15
    ANDB $16                 ; @BT D4 16
    BITA $17                 ; @BT 95 17
    BITB $18                 ; @BT D5 18
    CMPA $19                 ; @BT 91 19
    CMPB $1A                 ; @BT D1 1A
    CMPD $1B                 ; @BT 10 93 1B
    CMPS $1C                 ; @BT 11 9C 1C
    CMPU $1D                 ; @BT 11 93 1D
    CMPX $1E                 ; @BT 9C 1E
    CMPY $1F                 ; @BT 10 9C 1F
    EORA $20                 ; @BT 98 20
    EORB $21                 ; @BT D8 21
    LDA $22                  ; @BT 96 22
    LDB $23                  ; @BT D6 23
    LDD $24                  ; @BT DC 24
    LDS $25                  ; @BT 10 DE 25
    LDU $26                  ; @BT DE 26
    LDX $27                  ; @BT 9E 27
    LDY $28                  ; @BT 10 9E 28
    ORA $29                  ; @BT 9A 29
    ORB $2A                  ; @BT DA 2A
    SBCA $2B                 ; @BT 92 2B
    SBCB $2C                 ; @BT D2 2C
    STA $2D                  ; @BT 97 2D
    STB $2E                  ; @BT D7 2E
    STD $2F                  ; @BT DD 2F
    STS $30                  ; @BT 10 DF 30
    STU $31                  ; @BT DF 31
    STX $32                  ; @BT 9F 32
    STY $33                  ; @BT 10 9F 33
    SUBA $34                 ; @BT 90 34
    SUBB $35                 ; @BT D0 35
    SUBD $36                 ; @BT 93 36

    ; --- Direct Page: memory-only ops ---
    NEG $37                  ; @BT 00 37
    COM $38                  ; @BT 03 38
    LSR $39                  ; @BT 04 39
    ROR $3A                  ; @BT 06 3A
    ASR $3B                  ; @BT 07 3B
    ASL $3C                  ; @BT 08 3C
    LSL $3D                  ; @BT 08 3D
    ROL $3E                  ; @BT 09 3E
    DEC $3F                  ; @BT 0A 3F
    INC $40                  ; @BT 0C 40
    TST $41                  ; @BT 0D 41
    JMP $42                  ; @BT 0E 42
    JSR $43                  ; @BT 9D 43
    CLR $44                  ; @BT 0F 44

    ; ==============================================================
    ; Extended addressing (16-bit address)
    ; ==============================================================
    ADCA $1234               ; @BT B9 12 34
    ADCB $1235               ; @BT F9 12 35
    ADDA $1236               ; @BT BB 12 36
    ADDB $1237               ; @BT FB 12 37
    ADDD $1238               ; @BT F3 12 38
    ANDA $1239               ; @BT B4 12 39
    ANDB $123A               ; @BT F4 12 3A
    BITA $123B               ; @BT B5 12 3B
    BITB $123C               ; @BT F5 12 3C
    CMPA $123D               ; @BT B1 12 3D
    CMPB $123E               ; @BT F1 12 3E
    CMPD $123F               ; @BT 10 B3 12 3F
    CMPS $1240               ; @BT 11 BC 12 40
    CMPU $1241               ; @BT 11 B3 12 41
    CMPX $1242               ; @BT BC 12 42
    CMPY $1243               ; @BT 10 BC 12 43
    EORA $1244               ; @BT B8 12 44
    EORB $1245               ; @BT F8 12 45
    LDA $1246                ; @BT B6 12 46
    LDB $1247                ; @BT F6 12 47
    LDD $1248                ; @BT FC 12 48
    LDS $1249                ; @BT 10 FE 12 49
    LDU $124A                ; @BT FE 12 4A
    LDX $124B                ; @BT BE 12 4B
    LDY $124C                ; @BT 10 BE 12 4C
    ORA $124D                ; @BT BA 12 4D
    ORB $124E                ; @BT FA 12 4E
    SBCA $124F               ; @BT B2 12 4F
    SBCB $1250               ; @BT F2 12 50
    STA $1251                ; @BT B7 12 51
    STB $1252                ; @BT F7 12 52
    STD $1253                ; @BT FD 12 53
    STS $1254                ; @BT 10 FF 12 54
    STU $1255                ; @BT FF 12 55
    STX $1256                ; @BT BF 12 56
    STY $1257                ; @BT 10 BF 12 57
    SUBA $1258               ; @BT B0 12 58
    SUBB $1259               ; @BT F0 12 59
    SUBD $125A               ; @BT B3 12 5A

    ; --- Extended: memory-only ops ---
    NEG $1260                ; @BT 70 12 60
    COM $1261                ; @BT 73 12 61
    LSR $1262                ; @BT 74 12 62
    ROR $1263                ; @BT 76 12 63
    ASR $1264                ; @BT 77 12 64
    ASL $1265                ; @BT 78 12 65
    LSL $1266                ; @BT 78 12 66
    ROL $1267                ; @BT 79 12 67
    DEC $1268                ; @BT 7A 12 68
    INC $1269                ; @BT 7C 12 69
    TST $126A                ; @BT 7D 12 6A
    JMP $2000                ; @BT 7E 20 00
    JSR $3456                ; @BT BD 34 56
    CLR $126B                ; @BT 7F 12 6B

    ; ==============================================================
    ; Indexed: zero-offset (,R) — postbyte only (no space before comma)
    ; ==============================================================
    LDA,X                    ; @BT A6 84
    LDA,Y                    ; @BT A6 A4
    LDA,U                    ; @BT A6 C4
    LDA,S                    ; @BT A6 E4
    STA,X                    ; @BT A7 84
    STD,Y                    ; @BT ED A4

    ; ==============================================================
    ; Indexed: register offset (A/B/D,R)
    ; ==============================================================
    LDA A,X                  ; @BT A6 86
    LDA A,Y                  ; @BT A6 A6
    LDA A,U                  ; @BT A6 C6
    LDA A,S                  ; @BT A6 E6
    LDA B,X                  ; @BT A6 85
    LDA B,Y                  ; @BT A6 A5
    LDA B,U                  ; @BT A6 C5
    LDA B,S                  ; @BT A6 E5
    LDA D,X                  ; @BT A6 8B
    LDA D,Y                  ; @BT A6 AB
    LDA D,U                  ; @BT A6 CB
    LDA D,S                  ; @BT A6 EB

    ; ==============================================================
    ; Indexed: auto-increment / auto-decrement
    ; ==============================================================
    LDA,X+                   ; @BT A6 80
    LDA,X++                  ; @BT A6 81
    LDA,-X                   ; @BT A6 82
    LDA,--X                  ; @BT A6 83
    LDA,Y+                   ; @BT A6 A0
    LDA,Y++                  ; @BT A6 A1
    LDA,-Y                   ; @BT A6 A2
    LDA,--Y                  ; @BT A6 A3
    LDA,U+                   ; @BT A6 C0
    LDA,U++                  ; @BT A6 C1
    LDA,-U                   ; @BT A6 C2
    LDA,--U                  ; @BT A6 C3
    LDA,S+                   ; @BT A6 E0
    LDA,S++                  ; @BT A6 E1
    LDA,-S                   ; @BT A6 E2
    LDA,--S                  ; @BT A6 E3

    ; ==============================================================
    ; Indexed: 5-bit constant offset (s,R) — range -16..+15
    ; postbyte = (reg_bits) | (offset & $1F)
    ;   X=$00 Y=$20 U=$40 S=$60
    ; ==============================================================
    LDA 0,X                  ; @BT A6 00
    LDA 5,X                  ; @BT A6 05
    LDA 15,X                 ; @BT A6 0F
    LDA -1,X                 ; @BT A6 1F
    LDA -16,X                ; @BT A6 10
    LDA 0,Y                  ; @BT A6 20
    LDA 5,Y                  ; @BT A6 25
    LDA 15,Y                 ; @BT A6 2F
    LDA 0,U                  ; @BT A6 40
    LDA 5,U                  ; @BT A6 45
    LDA 0,S                  ; @BT A6 60
    LDA -2,S                 ; @BT A6 7E
    LDA -16,S                ; @BT A6 70

    ; ==============================================================
    ; Indexed: 8-bit constant offset (x,R) — range -128..+127
    ; postbyte = $88/$A8/$C8/$E8 + 1-byte signed offset
    ; ==============================================================
    LDA $20,X                ; @BT A6 88 20
    LDA $7F,X                ; @BT A6 88 7F
    LDA $20,Y                ; @BT A6 A8 20
    LDA $7F,Y                ; @BT A6 A8 7F
    LDA $20,U                ; @BT A6 C8 20
    LDA $30,S                ; @BT A6 E8 30

    ; ==============================================================
    ; Indexed: 16-bit constant offset (?,R)
    ; postbyte = $89/$A9/$C9/$E9 + 2-byte offset
    ; ==============================================================
    LDA $1234,X              ; @BT A6 89 12 34
    LDA $5678,Y              ; @BT A6 A9 56 78
    LDA $1234,U              ; @BT A6 C9 12 34
    LDA $1234,S              ; @BT A6 E9 12 34

    ; ==============================================================
    ; Indexed: indirect zero-offset (,R)
    ; ==============================================================
    LDA (,X)                 ; @BT A6 94
    LDA (,Y)                 ; @BT A6 B4
    LDA (,U)                 ; @BT A6 D4
    LDA (,S)                 ; @BT A6 F4

    ; ==============================================================
    ; Indexed: indirect register offset (A/B/D,R)
    ; ==============================================================
    LDA (A,X)                ; @BT A6 96
    LDA (A,Y)                ; @BT A6 B6
    LDA (A,U)                ; @BT A6 D6
    LDA (A,S)                ; @BT A6 F6
    LDA (B,X)                ; @BT A6 95
    LDA (B,Y)                ; @BT A6 B5
    LDA (B,U)                ; @BT A6 D5
    LDA (B,S)                ; @BT A6 F5
    LDA (D,X)                ; @BT A6 9B
    LDA (D,Y)                ; @BT A6 BB
    LDA (D,U)                ; @BT A6 DB
    LDA (D,S)                ; @BT A6 FB

    ; ==============================================================
    ; Indexed: indirect auto-increment/decrement (only ++ and --)
    ; ==============================================================
    LDA (,X++)               ; @BT A6 91
    LDA (,Y++)               ; @BT A6 B1
    LDA (,U++)               ; @BT A6 D1
    LDA (,S++)               ; @BT A6 F1
    LDA (,--X)               ; @BT A6 93
    LDA (,--Y)               ; @BT A6 B3
    LDA (,--U)               ; @BT A6 D3
    LDA (,--S)               ; @BT A6 F3

    ; ==============================================================
    ; Indexed: indirect 8-bit offset (x,R)
    ; ==============================================================
    LDA ($20,X)              ; @BT A6 98 20
    LDA ($30,Y)              ; @BT A6 B8 30
    LDA ($40,U)              ; @BT A6 D8 40
    LDA ($50,S)              ; @BT A6 F8 50

    ; ==============================================================
    ; Indexed: indirect 16-bit offset (?,R)
    ; ==============================================================
    LDA ($1234,X)            ; @BT A6 99 12 34
    LDA ($5678,Y)            ; @BT A6 B9 56 78
    LDA ($1234,U)            ; @BT A6 D9 12 34
    LDA ($1234,S)            ; @BT A6 F9 12 34

    ; ==============================================================
    ; Indexed: PC-relative offset
    ; ==============================================================
    LDA 5,PCR                ; @BT A6 8C 05
    LDA $1234,PCR            ; @BT A6 8D 12 34

    ; ==============================================================
    ; Indexed: indirect PC-relative offset
    ; ==============================================================
    LDA ($20,PCR)            ; @BT A6 9C 20
    LDA ($1234,PCR)          ; @BT A6 9D 12 34

    ; ==============================================================
    ; Indexed: extended indirect
    ; ==============================================================
    LDA ($1234)              ; @BT A6 9F 12 34

    ; ==============================================================
    ; Indexed: memory-only ops in indexed mode
    ; ==============================================================
    CLR,X                    ; @BT 6F 84
    NEG,Y                    ; @BT 60 A4
    COM,X                    ; @BT 63 84
    LSR,U                    ; @BT 64 C4
    ROR,S                    ; @BT 66 E4
    ASR,X                    ; @BT 67 84
    ASL,X                    ; @BT 68 84
    LSL,Y                    ; @BT 68 A4
    ROL,X                    ; @BT 69 84
    DEC,X                    ; @BT 6A 84
    INC 5,X                  ; @BT 6C 05
    TST,S                    ; @BT 6D E4
    JMP,X                    ; @BT 6E 84
    JSR 5,Y                  ; @BT AD 25

    ; ==============================================================
    ; Indexed: page-2 ops in indexed mode
    ; ==============================================================
    CMPD,X                   ; @BT 10 A3 84
    CMPY A,S                 ; @BT 10 AC E6
    LDY,X++                  ; @BT 10 AE 81
    STY,X                    ; @BT 10 AF 84
    STS $20,X                ; @BT 10 EF 88 20
    LDS,--Y                  ; @BT 10 EE A3
    CMPS,X                   ; @BT 11 AC 84
    CMPU,Y                   ; @BT 11 A3 A4

    ; ==============================================================
    ; Branches — short (8-bit relative offset)
    ; 17 branches to T1_BR1: each 2 bytes, offsets count down by 2
    ; ==============================================================
    BRA T1_BR1               ; @BT 20 20
    BRN T1_BR1               ; @BT 21 1E
    BHI T1_BR1               ; @BT 22 1C
    BLS T1_BR1               ; @BT 23 1A
    BCC T1_BR1               ; @BT 24 18
    BHS T1_BR1               ; @BT 24 16
    BCS T1_BR1               ; @BT 25 14
    BLO T1_BR1               ; @BT 25 12
    BNE T1_BR1               ; @BT 26 10
    BEQ T1_BR1               ; @BT 27 0E
    BVC T1_BR1               ; @BT 28 0C
    BVS T1_BR1               ; @BT 29 0A
    BPL T1_BR1               ; @BT 2A 08
    BMI T1_BR1               ; @BT 2B 06
    BGE T1_BR1               ; @BT 2C 04
    BLT T1_BR1               ; @BT 2D 02
    BGT T1_BR1               ; @BT 2E 00
T1_BR1:
    BLE T1_BR2               ; @BT 2F 02
    BSR T1_BR2               ; @BT 8D 00
T1_BR2:

    ; ==============================================================
    ; Branches — long (16-bit relative offset)
    ; LBRA/LBSR = 3 bytes; all other LBxx = 4 bytes (page-2 prefix)
    ; 17 branches to T1_LBR1:
    ;   LBRA(3) + 16×LBxx(4) = 67 = $43 bytes before target
    ; ==============================================================
    LBRA T1_LBR1             ; @BT 16 00 40
    LBRN T1_LBR1             ; @BT 10 21 00 3C
    LBHI T1_LBR1             ; @BT 10 22 00 38
    LBLS T1_LBR1             ; @BT 10 23 00 34
    LBCC T1_LBR1             ; @BT 10 24 00 30
    LBHS T1_LBR1             ; @BT 10 24 00 2C
    LBCS T1_LBR1             ; @BT 10 25 00 28
    LBLO T1_LBR1             ; @BT 10 25 00 24
    LBNE T1_LBR1             ; @BT 10 26 00 20
    LBEQ T1_LBR1             ; @BT 10 27 00 1C
    LBVC T1_LBR1             ; @BT 10 28 00 18
    LBVS T1_LBR1             ; @BT 10 29 00 14
    LBPL T1_LBR1             ; @BT 10 2A 00 10
    LBMI T1_LBR1             ; @BT 10 2B 00 0C
    LBGE T1_LBR1             ; @BT 10 2C 00 08
    LBLT T1_LBR1             ; @BT 10 2D 00 04
    LBGT T1_LBR1             ; @BT 10 2E 00 00
T1_LBR1:
    LBLE T1_LBR2             ; @BT 10 2F 00 03
    LBSR T1_LBR2             ; @BT 17 00 00
T1_LBR2:

    ; ==============================================================
    ; EXG / TFR — postbyte = (src<<4)|dst
    ; 16-bit regs: D=0 X=1 Y=2 U=3 S=4 PC=5
    ;  8-bit regs: A=8 B=9 CC=A DP=B
    ; ==============================================================
    EXG A,B                  ; @BT 1E 89
    EXG D,X                  ; @BT 1E 01
    EXG X,Y                  ; @BT 1E 12
    EXG U,S                  ; @BT 1E 34
    EXG CC,DP                ; @BT 1E AB
    TFR A,B                  ; @BT 1F 89
    TFR D,X                  ; @BT 1F 01
    TFR X,Y                  ; @BT 1F 12

    ; ==============================================================
    ; PSHS/PULS — bitmask: CC=$01 A=$02 B=$04 DP=$08 X=$10 Y=$20 U=$40 PC=$80
    ; PSHU/PULU — same bitmask but bit6 = S instead of U
    ; ==============================================================
    PSHS CC                  ; @BT 34 01
    PSHS A                   ; @BT 34 02
    PSHS B                   ; @BT 34 04
    PSHS DP                  ; @BT 34 08
    PSHS X                   ; @BT 34 10
    PSHS Y                   ; @BT 34 20
    PSHS U                   ; @BT 34 40
    PSHS PC                  ; @BT 34 80
    PSHS CC,A,B,DP,X,Y,U,PC ; @BT 34 FF
    PULS A                   ; @BT 35 02
    PULS CC,A,B,DP,X,Y,U,PC ; @BT 35 FF
    PSHU A                   ; @BT 36 02
    PSHU CC,A,B,DP,X,Y,S,PC ; @BT 36 FF
    PULU A                   ; @BT 37 02
    PULU CC,A,B,DP,X,Y,S,PC ; @BT 37 FF

    ; ==============================================================
    ; LEA — load effective address
    ; ==============================================================
    LEAX 5,X                 ; @BT 30 05
    LEAX,X++                 ; @BT 30 81
    LEAX $1234,X             ; @BT 30 89 12 34
    LEAY 5,Y                 ; @BT 31 25
    LEAY,X++                 ; @BT 31 81
    LEAS -2,S                ; @BT 32 7E
    LEAS,S++                 ; @BT 32 E1
    LEAU,U                   ; @BT 33 C4
    LEAU 5,U                 ; @BT 33 45

    ; ==============================================================
    ; Indexed: space-before-comma zero-offset ( ,R with space)
    ; Verifies that "MNEMONIC ,X" parses the same as "MNEMONIC,X"
    ; Also provides indexed opcode coverage for all indexed mnemonics
    ; ==============================================================
    ADCA ,X                  ; @BT A9 84
    ADCB ,X                  ; @BT E9 84
    ADDA ,X                  ; @BT AB 84
    ADDB ,X                  ; @BT EB 84
    ADDD ,X                  ; @BT E3 84
    ANDA ,X                  ; @BT A4 84
    ANDB ,X                  ; @BT E4 84
    ASL ,X                   ; @BT 68 84
    ASR ,X                   ; @BT 67 84
    BITA ,X                  ; @BT A5 84
    BITB ,X                  ; @BT E5 84
    CLR ,X                   ; @BT 6F 84
    CMPA ,X                  ; @BT A1 84
    CMPB ,X                  ; @BT E1 84
    CMPD ,X                  ; @BT 10 A3 84
    CMPS ,X                  ; @BT 11 AC 84
    CMPU ,X                  ; @BT 11 A3 84
    CMPX ,X                  ; @BT AC 84
    CMPY ,X                  ; @BT 10 AC 84
    COM ,X                   ; @BT 63 84
    DEC ,X                   ; @BT 6A 84
    EORA ,X                  ; @BT A8 84
    EORB ,X                  ; @BT E8 84
    INC ,X                   ; @BT 6C 84
    JMP ,X                   ; @BT 6E 84
    JSR ,X                   ; @BT AD 84
    LDA ,X                   ; @BT A6 84
    LDA ,Y                   ; @BT A6 A4
    LDA ,U                   ; @BT A6 C4
    LDA ,S                   ; @BT A6 E4
    LDB ,X                   ; @BT E6 84
    LDD ,X                   ; @BT EC 84
    LDS ,X                   ; @BT 10 EE 84
    LDU ,X                   ; @BT EE 84
    LDX ,X                   ; @BT AE 84
    LDY ,X                   ; @BT 10 AE 84
    LEAS ,X                  ; @BT 32 84
    LEAU ,X                  ; @BT 33 84
    LEAX ,X                  ; @BT 30 84
    LEAY ,X                  ; @BT 31 84
    LSL ,X                   ; @BT 68 84
    LSR ,X                   ; @BT 64 84
    NEG ,X                   ; @BT 60 84
    ORA ,X                   ; @BT AA 84
    ORB ,X                   ; @BT EA 84
    ROL ,X                   ; @BT 69 84
    ROR ,X                   ; @BT 66 84
    SBCA ,X                  ; @BT A2 84
    SBCB ,X                  ; @BT E2 84
    STA ,X                   ; @BT A7 84
    STB ,X                   ; @BT E7 84
    STD ,X                   ; @BT ED 84
    STS ,X                   ; @BT 10 EF 84
    STU ,X                   ; @BT EF 84
    STX ,X                   ; @BT AF 84
    STY ,X                   ; @BT 10 AF 84
    SUBA ,X                  ; @BT A0 84
    SUBB ,X                  ; @BT E0 84
    SUBD ,X                  ; @BT A3 84
    TST ,X                   ; @BT 6D 84

    .db "<01"               ; @BT END

    /////////////////////////////////////////////////////////////////////////////
    // TEST-02: same opcodes as TEST-01, randomised order, branches spread out
    /////////////////////////////////////////////////////////////////////////////

    .db "02>"               ; @BT TEST-02 02 START

    ; GENERATED: copied from TEST-01, randomized command order
    ; GENERATED: seed = 6809

    ASL $1265                ; @BT 78 12 65
    LDA $20,X                ; @BT A6 88 20
    STX ,X                   ; @BT AF 84
    ADCA #$01                ; @BT 89 01
    LDY #$1B                 ; @BT 10 8E 1B
    LDA A,X                  ; @BT A6 86
    ANDB $123A               ; @BT F4 12 3A
    LDA (B,X)                ; @BT A6 95
    MUL                      ; @BT 3D
    LDA (A,S)                ; @BT A6 F6
    LDA 15,X                 ; @BT A6 0F
    LDA -2,S                 ; @BT A6 7E
    LDB $23                  ; @BT D6 23
    LDA #$15                 ; @BT 86 15
    CLR $126B                ; @BT 7F 12 6B
    LSR $1262                ; @BT 74 12 62
    PSHS Y                   ; @BT 34 20
    LEAS,S++                 ; @BT 32 E1
    CLRB                     ; @BT 5F
    SBCB $2C                 ; @BT D2 2C
    EXG CC,DP                ; @BT 1E AB
    CMPD $123F               ; @BT 10 B3 12 3F
    LDA (D,U)                ; @BT A6 DB
    LDA (,--U)               ; @BT A6 D3
    CWAI #$12                ; @BT 3C 12
    LSR $39                  ; @BT 04 39
    LDA ($20,PCR)            ; @BT A6 9C 20
    RORB                     ; @BT 56
    PSHS U                   ; @BT 34 40
    CMPS $1240               ; @BT 11 BC 12 40
    ASRA                     ; @BT 47
    BITB #$0A                ; @BT C5 0A
    CMPA #$0B                ; @BT 81 0B
    LEAS -2,S                ; @BT 32 7E
    LDA (,--S)               ; @BT A6 F3
    LDA (D,Y)                ; @BT A6 BB
    TFR X,Y                  ; @BT 1F 12
    CMPX #$10                ; @BT 8C 10
    LDX $27                  ; @BT 9E 27
    LDY ,X                   ; @BT 10 AE 84
    LDA A,Y                  ; @BT A6 A6
    JSR $43                  ; @BT 9D 43
    LDA B,X                  ; @BT A6 85
    SBCA $124F               ; @BT B2 12 4F
    LDA 5,X                  ; @BT A6 05
    ORA $124D                ; @BT BA 12 4D
    CMPY #$11                ; @BT 10 8C 11
    PSHS CC,A,B,DP,X,Y,U,PC ; @BT 34 FF
    ORA $29                  ; @BT 9A 29
    LDU $26                  ; @BT DE 26
    LDX ,X                   ; @BT AE 84
    LSL $1266                ; @BT 78 12 66
    ORB #$1D                 ; @BT CA 1D
    LDA ,U                   ; @BT A6 C4
    TSTA                     ; @BT 4D
    SUBB #$22                ; @BT C0 22
    ROL ,X                   ; @BT 69 84
    CMPY A,S                 ; @BT 10 AC E6
    ANDA ,X                  ; @BT A4 84
    LEAY,X++                 ; @BT 31 81
    ROL $3E                  ; @BT 09 3E
    LSLA                     ; @BT 48
    LDA ($30,Y)              ; @BT A6 B8 30
    LDB #$16                 ; @BT C6 16
    SWI3                     ; @BT 11 3F
    DEC ,X                   ; @BT 6A 84
    BITB $18                 ; @BT D5 18
    BRA T2_BR1               ; @BT 20 20
    BRN T2_BR1               ; @BT 21 1E
    BHI T2_BR1               ; @BT 22 1C
    BLS T2_BR1               ; @BT 23 1A
    BCC T2_BR1               ; @BT 24 18
    BHS T2_BR1               ; @BT 24 16
    BCS T2_BR1               ; @BT 25 14
    BLO T2_BR1               ; @BT 25 12
    BNE T2_BR1               ; @BT 26 10
    BEQ T2_BR1               ; @BT 27 0E
    BVC T2_BR1               ; @BT 28 0C
    BVS T2_BR1               ; @BT 29 0A
    BPL T2_BR1               ; @BT 2A 08
    BMI T2_BR1               ; @BT 2B 06
    BGE T2_BR1               ; @BT 2C 04
    BLT T2_BR1               ; @BT 2D 02
    BGT T2_BR1               ; @BT 2E 00
T2_BR1:
    BLE T2_BR2               ; @BT 2F 02
    BSR T2_BR2               ; @BT 8D 00
T2_BR2:
    CMPB $123E               ; @BT F1 12 3E
    ASR,X                    ; @BT 67 84
    EORA #$13                ; @BT 88 13
    SBCA $2B                 ; @BT 92 2B
    LSL $3D                  ; @BT 08 3D
    LDA ($1234,X)            ; @BT A6 99 12 34
    LDA ($1234,U)            ; @BT A6 D9 12 34
    ROR $1263                ; @BT 76 12 63
    STX $32                  ; @BT 9F 32
    STD $2F                  ; @BT DD 2F
    EORA ,X                  ; @BT A8 84
    ADDB $13                 ; @BT DB 13
    NEG $1260                ; @BT 70 12 60
    LDA,-Y                   ; @BT A6 A2
    JMP $42                  ; @BT 0E 42
    LDU #$19                 ; @BT CE 19
    NEGB                     ; @BT 50
    PSHS B                   ; @BT 34 04
    SUBB ,X                  ; @BT E0 84
    LDA D,Y                  ; @BT A6 AB
    LDA (,Y++)               ; @BT A6 B1
    PSHS PC                  ; @BT 34 80
    LDA B,Y                  ; @BT A6 A5
    ANDA #$06                ; @BT 84 06
    BITA #$09                ; @BT 85 09
    ASR $3B                  ; @BT 07 3B
    ANDB #$07                ; @BT C4 07
    LSL ,X                   ; @BT 68 84
    LDA (B,Y)                ; @BT A6 B5
    INC $40                  ; @BT 0C 40
    ADDD ,X                  ; @BT E3 84
    BITB ,X                  ; @BT E5 84
    ROR ,X                   ; @BT 66 84
    LDA,Y                    ; @BT A6 A4
    CMPB #$0C                ; @BT C1 0C
    ADCA ,X                  ; @BT A9 84
    STY ,X                   ; @BT 10 AF 84
    COM $38                  ; @BT 03 38
    CMPD #$0D                ; @BT 10 83 0D
    LDA A,S                  ; @BT A6 E6
    LDA (D,X)                ; @BT A6 9B
    LDA 0,S                  ; @BT A6 60
    LDA (,X)                 ; @BT A6 94
    SUBD #$23                ; @BT 83 23
    EXG D,X                  ; @BT 1E 01
    LDA $7F,Y                ; @BT A6 A8 7F
    PSHU CC,A,B,DP,X,Y,S,PC ; @BT 36 FF
    ORB ,X                   ; @BT EA 84
    ROL,X                    ; @BT 69 84
    LDA ($1234)              ; @BT A6 9F 12 34
    ASL $3C                  ; @BT 08 3C
    LDA,S                    ; @BT A6 E4
    SUBB $1259               ; @BT F0 12 59
    LDA,X                    ; @BT A6 84
    ADCB $11                 ; @BT D9 11
    LDA $1246                ; @BT B6 12 46
    RTS                      ; @BT 39
    PULS A                   ; @BT 35 02
    CMPS ,X                  ; @BT 11 AC 84
    LSLB                     ; @BT 58
    LDA (,S)                 ; @BT A6 F4
    TST ,X                   ; @BT 6D 84
    EXG A,B                  ; @BT 1E 89
    EORB #$14                ; @BT C8 14
    CMPS #$0E                ; @BT 11 8C 0E
    STB $2E                  ; @BT D7 2E
    ADCA $1234               ; @BT B9 12 34
    LDU $124A                ; @BT FE 12 4A
    STS $20,X                ; @BT 10 EF 88 20
    STD,Y                    ; @BT ED A4
    ROLB                     ; @BT 59
    SBCB ,X                  ; @BT E2 84
    LDA,U++                  ; @BT A6 C1
    LDA $20,Y                ; @BT A6 A8 20
    RORA                     ; @BT 46
    CMPX $1E                 ; @BT 9C 1E
    NEG,Y                    ; @BT 60 A4
    LDA (,U)                 ; @BT A6 D4
    EORA $1244               ; @BT B8 12 44
    LDA 0,Y                  ; @BT A6 20
    ADCB #$02                ; @BT C9 02
    CMPA ,X                  ; @BT A1 84
    ADDA $1236               ; @BT BB 12 36
    EXG U,S                  ; @BT 1E 34
    LDA $20,U                ; @BT A6 C8 20
    LDA,U                    ; @BT A6 C4
    LDA -16,X                ; @BT A6 10
    LDA (,Y)                 ; @BT A6 B4
    BITA $123B               ; @BT B5 12 3B
    STY $33                  ; @BT 10 9F 33
    LDA D,S                  ; @BT A6 EB
    SWI1                     ; @BT 3F
    ASR ,X                   ; @BT 67 84
    COMA                     ; @BT 43
    LDB $1247                ; @BT F6 12 47
    LDX #$1A                 ; @BT 8E 1A
    ANDA $15                 ; @BT 94 15
    LDA ($50,S)              ; @BT A6 F8 50
    LSL,Y                    ; @BT 68 A4
    COM ,X                   ; @BT 63 84
    SUBD $36                 ; @BT 93 36
    TST $126A                ; @BT 7D 12 6A
    BITA ,X                  ; @BT A5 84
    STS ,X                   ; @BT 10 EF 84
    CMPU,Y                   ; @BT 11 A3 A4
    STU ,X                   ; @BT EF 84
    LDA (,X++)               ; @BT A6 91
    DAA                      ; @BT 19
    LDA $7F,X                ; @BT A6 88 7F
    LDA,--X                  ; @BT A6 83
    CLRA                     ; @BT 4F
    LDA 5,PCR                ; @BT A6 8C 05
    DEC $1268                ; @BT 7A 12 68
    EXG X,Y                  ; @BT 1E 12
    LDS ,X                   ; @BT 10 EE 84
    INC $1269                ; @BT 7C 12 69
    SYNC                     ; @BT 13
    DECB                     ; @BT 5A
    LDA,X++                  ; @BT A6 81
    LDD #$17                 ; @BT CC 17
    LDA (,--X)               ; @BT A6 93
    PSHS DP                  ; @BT 34 08
    PSHS A                   ; @BT 34 02
    LEAX 5,X                 ; @BT 30 05
    LDA 5,U                  ; @BT A6 45
    LDA A,U                  ; @BT A6 C6
    JSR 5,Y                  ; @BT AD 25
    BITB $123C               ; @BT F5 12 3C
    ORA ,X                   ; @BT AA 84
    ADDD #$05                ; @BT C3 05
    COM $1261                ; @BT 73 12 61
    CMPB $1A                 ; @BT D1 1A
    ABX                      ; @BT 3A
    LSR,U                    ; @BT 64 C4
    LSRB                     ; @BT 54
    CMPU $1D                 ; @BT 11 93 1D
    ASLA                     ; @BT 48
    STX $1256                ; @BT BF 12 56
    LDY $124C                ; @BT 10 BE 12 4C
    CMPU $1241               ; @BT 11 B3 12 41
    LDD $24                  ; @BT DC 24
    LDA,X+                   ; @BT A6 80
    SUBA ,X                  ; @BT A0 84
    LDS $1249                ; @BT 10 FE 12 49
    LDA,-X                   ; @BT A6 82
    LEAX $1234,X             ; @BT 30 89 12 34
    STB ,X                   ; @BT E7 84
    PSHU A                   ; @BT 36 02
    PSHS X                   ; @BT 34 10
    LDA ,S                   ; @BT A6 E4
    LEAS ,X                  ; @BT 32 84
    LDA (D,S)                ; @BT A6 FB
    LDA $30,S                ; @BT A6 E8 30
    NEG ,X                   ; @BT 60 84
    ADDD $1238               ; @BT F3 12 38
    JMP,X                    ; @BT 6E 84
    LDA B,U                  ; @BT A6 C5
    SBCA #$1F                ; @BT 82 1F
    STD $1253                ; @BT FD 12 53
    ASR $1264                ; @BT 77 12 64
    LDA (A,Y)                ; @BT A6 B6
    CMPY $1243               ; @BT 10 BC 12 43
    NEGA                     ; @BT 40
    CMPA $123D               ; @BT B1 12 3D
    LDA D,X                  ; @BT A6 8B
    CLR ,X                   ; @BT 6F 84
    LEAU ,X                  ; @BT 33 84
    LDA ($20,X)              ; @BT A6 98 20
    LEAX,X++                 ; @BT 30 81
    LDA,--U                  ; @BT A6 C3
    CMPD $1B                 ; @BT 10 93 1B
    LDA ($1234,PCR)          ; @BT A6 9D 12 34
    STS $1254                ; @BT 10 FF 12 54
    SBCA ,X                  ; @BT A2 84
    STY,X                    ; @BT 10 AF 84
    CMPX $1242               ; @BT BC 12 42
    CMPU ,X                  ; @BT 11 A3 84
    ORB $2A                  ; @BT DA 2A
    JMP $2000                ; @BT 7E 20 00
    SUBA $1258               ; @BT B0 12 58
    COMB                     ; @BT 53
    ROR $3A                  ; @BT 06 3A
    LDU ,X                   ; @BT EE 84
    STS $30                  ; @BT 10 DF 30
    LDA,--S                  ; @BT A6 E3
    LDA,Y+                   ; @BT A6 A0
    LDA (,U++)               ; @BT A6 D1
    LDA 5,Y                  ; @BT A6 25
    CMPY $1F                 ; @BT 10 9C 1F
    ADCB $1235               ; @BT F9 12 35
    PULU CC,A,B,DP,X,Y,S,PC ; @BT 37 FF
    LDA ($40,U)              ; @BT A6 D8 40
    LEAY ,X                  ; @BT 31 84
    STU $1255                ; @BT FF 12 55
    SBCB #$20                ; @BT C2 20
    JMP ,X                   ; @BT 6E 84
    INC ,X                   ; @BT 6C 84
    STA,X                    ; @BT A7 84
    LDA,S++                  ; @BT A6 E1
    CMPS,X                   ; @BT 11 AC 84
    TSTB                     ; @BT 5D
    INC 5,X                  ; @BT 6C 05
    SUBD ,X                  ; @BT A3 84
    ANDB ,X                  ; @BT E4 84
    CLR $44                  ; @BT 0F 44
    SWI2                     ; @BT 10 3F
    ADCA $10                 ; @BT 99 10
    DECA                     ; @BT 4A
    LDA (,S++)               ; @BT A6 F1
    BITA $17                 ; @BT 95 17
    CMPU #$0F                ; @BT 11 83 0F
    LDS #$18                 ; @BT 10 CE 18
    ADCB ,X                  ; @BT E9 84
    TST $41                  ; @BT 0D 41
    LDA,Y++                  ; @BT A6 A1
    ORA #$1C                 ; @BT 8A 1C
    LEAX ,X                  ; @BT 30 84
    LDA (A,U)                ; @BT A6 D6
    EORB $1245               ; @BT F8 12 45
    SEX                      ; @BT 1D
    LDA $1234,S              ; @BT A6 E9 12 34
    INCB                     ; @BT 5C
    ANDA $1239               ; @BT B4 12 39
    LDY $28                  ; @BT 10 9E 28
    LDA (A,X)                ; @BT A6 96
    ROLA                     ; @BT 49
    STU $31                  ; @BT DF 31
    LDA (,--Y)               ; @BT A6 B3
    STA $1251                ; @BT B7 12 51
    TST,S                    ; @BT 6D E4
    LDY,X++                  ; @BT 10 AE 81
    LDA D,U                  ; @BT A6 CB
    CMPX ,X                  ; @BT AC 84
    EORB ,X                  ; @BT E8 84
    INCA                     ; @BT 4C
    ASL ,X                   ; @BT 68 84
    STY $1257                ; @BT 10 BF 12 57
    ASL,X                    ; @BT 68 84
    LDA,U+                   ; @BT A6 C0
    LDA 0,X                  ; @BT A6 00
    ANDCC #$08               ; @BT 1C 08
    COM,X                    ; @BT 63 84
    STA $2D                  ; @BT 97 2D
    PULS CC,A,B,DP,X,Y,U,PC ; @BT 35 FF
    LDA 15,Y                 ; @BT A6 2F
    ADDD $14                 ; @BT D3 14
    TFR A,B                  ; @BT 1F 89
    PULU A                   ; @BT 37 02
    SBCB $1250               ; @BT F2 12 50
    RTI                      ; @BT 3B
    ORB $124E                ; @BT FA 12 4E
    LDA ($5678,Y)            ; @BT A6 B9 56 78
    CMPD ,X                  ; @BT 10 A3 84
    LDA,-U                   ; @BT A6 C2
    DEC,X                    ; @BT 6A 84
    LDA 0,U                  ; @BT A6 40
    CMPB ,X                  ; @BT E1 84
    LDA $1234,X              ; @BT A6 89 12 34
    ADDB #$04                ; @BT CB 04
    ADDB $1237               ; @BT FB 12 37
    LDA,S+                   ; @BT A6 E0
    LDA B,S                  ; @BT A6 E5
    JSR ,X                   ; @BT AD 84
    STB $1252                ; @BT F7 12 52
    ASRB                     ; @BT 57
    ROL $1267                ; @BT 79 12 67
    SUBB $35                 ; @BT D0 35
    LDS,--Y                  ; @BT 10 EE A3
    LEAU,U                   ; @BT 33 C4
    CMPD,X                   ; @BT 10 A3 84
    LBRA T2_LBR1             ; @BT 16 00 40
    LBRN T2_LBR1             ; @BT 10 21 00 3C
    LBHI T2_LBR1             ; @BT 10 22 00 38
    LBLS T2_LBR1             ; @BT 10 23 00 34
    LBCC T2_LBR1             ; @BT 10 24 00 30
    LBHS T2_LBR1             ; @BT 10 24 00 2C
    LBCS T2_LBR1             ; @BT 10 25 00 28
    LBLO T2_LBR1             ; @BT 10 25 00 24
    LBNE T2_LBR1             ; @BT 10 26 00 20
    LBEQ T2_LBR1             ; @BT 10 27 00 1C
    LBVC T2_LBR1             ; @BT 10 28 00 18
    LBVS T2_LBR1             ; @BT 10 29 00 14
    LBPL T2_LBR1             ; @BT 10 2A 00 10
    LBMI T2_LBR1             ; @BT 10 2B 00 0C
    LBGE T2_LBR1             ; @BT 10 2C 00 08
    LBLT T2_LBR1             ; @BT 10 2D 00 04
    LBGT T2_LBR1             ; @BT 10 2E 00 00
T2_LBR1:
    LBLE T2_LBR2             ; @BT 10 2F 00 03
    LBSR T2_LBR2             ; @BT 17 00 00
T2_LBR2:
    LDA ,X                   ; @BT A6 84
    LDA (B,S)                ; @BT A6 F5
    CLR,X                    ; @BT 6F 84
    SUBD $125A               ; @BT B3 12 5A
    LDX $124B                ; @BT BE 12 4B
    LEAU 5,U                 ; @BT 33 45
    LDA ($1234,S)            ; @BT A6 F9 12 34
    ADDA $12                 ; @BT 9B 12
    STD ,X                   ; @BT ED 84
    ASLB                     ; @BT 58
    LDA -16,S                ; @BT A6 70
    ROR,S                    ; @BT 66 E4
    CMPA $19                 ; @BT 91 19
    CMPS $1C                 ; @BT 11 9C 1C
    NEG $37                  ; @BT 00 37
    LDA -1,X                 ; @BT A6 1F
    LDA $22                  ; @BT 96 22
    LDA (B,U)                ; @BT A6 D5
    LDS $25                  ; @BT 10 DE 25
    LDA $1234,U              ; @BT A6 C9 12 34
    EORB $21                 ; @BT D8 21
    LDA $5678,Y              ; @BT A6 A9 56 78
    LEAY 5,Y                 ; @BT 31 25
    SUBA #$21                ; @BT 80 21
    LDD $1248                ; @BT FC 12 48
    NOP                      ; @BT 12
    JSR $3456                ; @BT BD 34 56
    LSR ,X                   ; @BT 64 84
    LDA,--Y                  ; @BT A6 A3
    ADDA #$03                ; @BT 8B 03
    DEC $3F                  ; @BT 0A 3F
    ADDB ,X                  ; @BT EB 84
    PSHS CC                  ; @BT 34 01
    LDA,-S                   ; @BT A6 E2
    ORCC #$1E                ; @BT 1A 1E
    LDA $1234,PCR            ; @BT A6 8D 12 34
    LDD ,X                   ; @BT EC 84
    TFR D,X                  ; @BT 1F 01
    SUBA $34                 ; @BT 90 34
    STA ,X                   ; @BT A7 84
    EORA $20                 ; @BT 98 20
    LDB ,X                   ; @BT E6 84
    LDA ,Y                   ; @BT A6 A4
    ADDA ,X                  ; @BT AB 84
    SWI                      ; @BT 3F
    CMPY ,X                  ; @BT 10 AC 84
    ANDB $16                 ; @BT D4 16
    LSRA                     ; @BT 44

    .db "<02"               ; @BT END

    /////////////////////////////////////////////////////////////////////////////
    // TEST-03: same opcodes, randomised order, operands as arithmetic expressions
    /////////////////////////////////////////////////////////////////////////////

    .db "03>"               ; @BT TEST-03 03 START

    ; GENERATED: copied from TEST-02 with expressionized immediate operands

    ; GENERATED: copied from TEST-01, randomized command order
    ; GENERATED: seed = 6809

    ASL $1265                ; @BT 78 12 65
    LDA $20,X                ; @BT A6 88 20
    STX ,X                   ; @BT AF 84
    ADCA #((($01*8)/4)-($01*2)+$01)                ; @BT 89 01
    LDY #((($1B*8)/4)-($1B*2)+$1B)                 ; @BT 10 8E 1B
    LDA A,X                  ; @BT A6 86
    ANDB $123A               ; @BT F4 12 3A
    LDA (B,X)                ; @BT A6 95
    MUL                      ; @BT 3D
    LDA (A,S)                ; @BT A6 F6
    LDA 15,X                 ; @BT A6 0F
    LDA -2,S                 ; @BT A6 7E
    LDB $23                  ; @BT D6 23
    LDA #((($15*8)/4)-($15*2)+$15)                 ; @BT 86 15
    CLR $126B                ; @BT 7F 12 6B
    LSR $1262                ; @BT 74 12 62
    PSHS Y                   ; @BT 34 20
    LEAS,S++                 ; @BT 32 E1
    CLRB                     ; @BT 5F
    SBCB $2C                 ; @BT D2 2C
    EXG CC,DP                ; @BT 1E AB
    CMPD $123F               ; @BT 10 B3 12 3F
    LDA (D,U)                ; @BT A6 DB
    LDA (,--U)               ; @BT A6 D3
    CWAI #((($12*8)/4)-($12*2)+$12)                ; @BT 3C 12
    LSR $39                  ; @BT 04 39
    LDA ($20,PCR)            ; @BT A6 9C 20
    RORB                     ; @BT 56
    PSHS U                   ; @BT 34 40
    CMPS $1240               ; @BT 11 BC 12 40
    ASRA                     ; @BT 47
    BITB #((($0A*8)/4)-($0A*2)+$0A)                ; @BT C5 0A
    CMPA #((($0B*8)/4)-($0B*2)+$0B)                ; @BT 81 0B
    LEAS -2,S                ; @BT 32 7E
    LDA (,--S)               ; @BT A6 F3
    LDA (D,Y)                ; @BT A6 BB
    TFR X,Y                  ; @BT 1F 12
    CMPX #((($10*8)/4)-($10*2)+$10)                ; @BT 8C 10
    LDX $27                  ; @BT 9E 27
    LDY ,X                   ; @BT 10 AE 84
    LDA A,Y                  ; @BT A6 A6
    JSR $43                  ; @BT 9D 43
    LDA B,X                  ; @BT A6 85
    SBCA $124F               ; @BT B2 12 4F
    LDA 5,X                  ; @BT A6 05
    ORA $124D                ; @BT BA 12 4D
    CMPY #((($11*8)/4)-($11*2)+$11)                ; @BT 10 8C 11
    PSHS CC,A,B,DP,X,Y,U,PC ; @BT 34 FF
    ORA $29                  ; @BT 9A 29
    LDU $26                  ; @BT DE 26
    LDX ,X                   ; @BT AE 84
    LSL $1266                ; @BT 78 12 66
    ORB #((($1D*8)/4)-($1D*2)+$1D)                 ; @BT CA 1D
    LDA ,U                   ; @BT A6 C4
    TSTA                     ; @BT 4D
    SUBB #((($22*8)/4)-($22*2)+$22)                ; @BT C0 22
    ROL ,X                   ; @BT 69 84
    CMPY A,S                 ; @BT 10 AC E6
    ANDA ,X                  ; @BT A4 84
    LEAY,X++                 ; @BT 31 81
    ROL $3E                  ; @BT 09 3E
    LSLA                     ; @BT 48
    LDA ($30,Y)              ; @BT A6 B8 30
    LDB #((($16*8)/4)-($16*2)+$16)                 ; @BT C6 16
    SWI3                     ; @BT 11 3F
    DEC ,X                   ; @BT 6A 84
    BITB $18                 ; @BT D5 18
    BRA T3_BR1               ; @BT 20 20
    BRN T3_BR1               ; @BT 21 1E
    BHI T3_BR1               ; @BT 22 1C
    BLS T3_BR1               ; @BT 23 1A
    BCC T3_BR1               ; @BT 24 18
    BHS T3_BR1               ; @BT 24 16
    BCS T3_BR1               ; @BT 25 14
    BLO T3_BR1               ; @BT 25 12
    BNE T3_BR1               ; @BT 26 10
    BEQ T3_BR1               ; @BT 27 0E
    BVC T3_BR1               ; @BT 28 0C
    BVS T3_BR1               ; @BT 29 0A
    BPL T3_BR1               ; @BT 2A 08
    BMI T3_BR1               ; @BT 2B 06
    BGE T3_BR1               ; @BT 2C 04
    BLT T3_BR1               ; @BT 2D 02
    BGT T3_BR1               ; @BT 2E 00
T3_BR1:
    BLE T3_BR2               ; @BT 2F 02
    BSR T3_BR2               ; @BT 8D 00
T3_BR2:
    CMPB $123E               ; @BT F1 12 3E
    ASR,X                    ; @BT 67 84
    EORA #((($13*8)/4)-($13*2)+$13)                ; @BT 88 13
    SBCA $2B                 ; @BT 92 2B
    LSL $3D                  ; @BT 08 3D
    LDA ($1234,X)            ; @BT A6 99 12 34
    LDA ($1234,U)            ; @BT A6 D9 12 34
    ROR $1263                ; @BT 76 12 63
    STX $32                  ; @BT 9F 32
    STD $2F                  ; @BT DD 2F
    EORA ,X                  ; @BT A8 84
    ADDB $13                 ; @BT DB 13
    NEG $1260                ; @BT 70 12 60
    LDA,-Y                   ; @BT A6 A2
    JMP $42                  ; @BT 0E 42
    LDU #((($19*8)/4)-($19*2)+$19)                 ; @BT CE 19
    NEGB                     ; @BT 50
    PSHS B                   ; @BT 34 04
    SUBB ,X                  ; @BT E0 84
    LDA D,Y                  ; @BT A6 AB
    LDA (,Y++)               ; @BT A6 B1
    PSHS PC                  ; @BT 34 80
    LDA B,Y                  ; @BT A6 A5
    ANDA #((($06*8)/4)-($06*2)+$06)                ; @BT 84 06
    BITA #((($09*8)/4)-($09*2)+$09)                ; @BT 85 09
    ASR $3B                  ; @BT 07 3B
    ANDB #((($07*8)/4)-($07*2)+$07)                ; @BT C4 07
    LSL ,X                   ; @BT 68 84
    LDA (B,Y)                ; @BT A6 B5
    INC $40                  ; @BT 0C 40
    ADDD ,X                  ; @BT E3 84
    BITB ,X                  ; @BT E5 84
    ROR ,X                   ; @BT 66 84
    LDA,Y                    ; @BT A6 A4
    CMPB #((($0C*8)/4)-($0C*2)+$0C)                ; @BT C1 0C
    ADCA ,X                  ; @BT A9 84
    STY ,X                   ; @BT 10 AF 84
    COM $38                  ; @BT 03 38
    CMPD #((($0D*8)/4)-($0D*2)+$0D)                ; @BT 10 83 0D
    LDA A,S                  ; @BT A6 E6
    LDA (D,X)                ; @BT A6 9B
    LDA 0,S                  ; @BT A6 60
    LDA (,X)                 ; @BT A6 94
    SUBD #((($23*8)/4)-($23*2)+$23)                ; @BT 83 23
    EXG D,X                  ; @BT 1E 01
    LDA $7F,Y                ; @BT A6 A8 7F
    PSHU CC,A,B,DP,X,Y,S,PC ; @BT 36 FF
    ORB ,X                   ; @BT EA 84
    ROL,X                    ; @BT 69 84
    LDA ($1234)              ; @BT A6 9F 12 34
    ASL $3C                  ; @BT 08 3C
    LDA,S                    ; @BT A6 E4
    SUBB $1259               ; @BT F0 12 59
    LDA,X                    ; @BT A6 84
    ADCB $11                 ; @BT D9 11
    LDA $1246                ; @BT B6 12 46
    RTS                      ; @BT 39
    PULS A                   ; @BT 35 02
    CMPS ,X                  ; @BT 11 AC 84
    LSLB                     ; @BT 58
    LDA (,S)                 ; @BT A6 F4
    TST ,X                   ; @BT 6D 84
    EXG A,B                  ; @BT 1E 89
    EORB #((($14*8)/4)-($14*2)+$14)                ; @BT C8 14
    CMPS #((($0E*8)/4)-($0E*2)+$0E)                ; @BT 11 8C 0E
    STB $2E                  ; @BT D7 2E
    ADCA $1234               ; @BT B9 12 34
    LDU $124A                ; @BT FE 12 4A
    STS $20,X                ; @BT 10 EF 88 20
    STD,Y                    ; @BT ED A4
    ROLB                     ; @BT 59
    SBCB ,X                  ; @BT E2 84
    LDA,U++                  ; @BT A6 C1
    LDA $20,Y                ; @BT A6 A8 20
    RORA                     ; @BT 46
    CMPX $1E                 ; @BT 9C 1E
    NEG,Y                    ; @BT 60 A4
    LDA (,U)                 ; @BT A6 D4
    EORA $1244               ; @BT B8 12 44
    LDA 0,Y                  ; @BT A6 20
    ADCB #((($02*8)/4)-($02*2)+$02)                ; @BT C9 02
    CMPA ,X                  ; @BT A1 84
    ADDA $1236               ; @BT BB 12 36
    EXG U,S                  ; @BT 1E 34
    LDA $20,U                ; @BT A6 C8 20
    LDA,U                    ; @BT A6 C4
    LDA -16,X                ; @BT A6 10
    LDA (,Y)                 ; @BT A6 B4
    BITA $123B               ; @BT B5 12 3B
    STY $33                  ; @BT 10 9F 33
    LDA D,S                  ; @BT A6 EB
    SWI1                     ; @BT 3F
    ASR ,X                   ; @BT 67 84
    COMA                     ; @BT 43
    LDB $1247                ; @BT F6 12 47
    LDX #((($1A*8)/4)-($1A*2)+$1A)                 ; @BT 8E 1A
    ANDA $15                 ; @BT 94 15
    LDA ($50,S)              ; @BT A6 F8 50
    LSL,Y                    ; @BT 68 A4
    COM ,X                   ; @BT 63 84
    SUBD $36                 ; @BT 93 36
    TST $126A                ; @BT 7D 12 6A
    BITA ,X                  ; @BT A5 84
    STS ,X                   ; @BT 10 EF 84
    CMPU,Y                   ; @BT 11 A3 A4
    STU ,X                   ; @BT EF 84
    LDA (,X++)               ; @BT A6 91
    DAA                      ; @BT 19
    LDA $7F,X                ; @BT A6 88 7F
    LDA,--X                  ; @BT A6 83
    CLRA                     ; @BT 4F
    LDA 5,PCR                ; @BT A6 8C 05
    DEC $1268                ; @BT 7A 12 68
    EXG X,Y                  ; @BT 1E 12
    LDS ,X                   ; @BT 10 EE 84
    INC $1269                ; @BT 7C 12 69
    SYNC                     ; @BT 13
    DECB                     ; @BT 5A
    LDA,X++                  ; @BT A6 81
    LDD #((($17*8)/4)-($17*2)+$17)                 ; @BT CC 17
    LDA (,--X)               ; @BT A6 93
    PSHS DP                  ; @BT 34 08
    PSHS A                   ; @BT 34 02
    LEAX 5,X                 ; @BT 30 05
    LDA 5,U                  ; @BT A6 45
    LDA A,U                  ; @BT A6 C6
    JSR 5,Y                  ; @BT AD 25
    BITB $123C               ; @BT F5 12 3C
    ORA ,X                   ; @BT AA 84
    ADDD #((($05*8)/4)-($05*2)+$05)                ; @BT C3 05
    COM $1261                ; @BT 73 12 61
    CMPB $1A                 ; @BT D1 1A
    ABX                      ; @BT 3A
    LSR,U                    ; @BT 64 C4
    LSRB                     ; @BT 54
    CMPU $1D                 ; @BT 11 93 1D
    ASLA                     ; @BT 48
    STX $1256                ; @BT BF 12 56
    LDY $124C                ; @BT 10 BE 12 4C
    CMPU $1241               ; @BT 11 B3 12 41
    LDD $24                  ; @BT DC 24
    LDA,X+                   ; @BT A6 80
    SUBA ,X                  ; @BT A0 84
    LDS $1249                ; @BT 10 FE 12 49
    LDA,-X                   ; @BT A6 82
    LEAX $1234,X             ; @BT 30 89 12 34
    STB ,X                   ; @BT E7 84
    PSHU A                   ; @BT 36 02
    PSHS X                   ; @BT 34 10
    LDA ,S                   ; @BT A6 E4
    LEAS ,X                  ; @BT 32 84
    LDA (D,S)                ; @BT A6 FB
    LDA $30,S                ; @BT A6 E8 30
    NEG ,X                   ; @BT 60 84
    ADDD $1238               ; @BT F3 12 38
    JMP,X                    ; @BT 6E 84
    LDA B,U                  ; @BT A6 C5
    SBCA #((($1F*8)/4)-($1F*2)+$1F)                ; @BT 82 1F
    STD $1253                ; @BT FD 12 53
    ASR $1264                ; @BT 77 12 64
    LDA (A,Y)                ; @BT A6 B6
    CMPY $1243               ; @BT 10 BC 12 43
    NEGA                     ; @BT 40
    CMPA $123D               ; @BT B1 12 3D
    LDA D,X                  ; @BT A6 8B
    CLR ,X                   ; @BT 6F 84
    LEAU ,X                  ; @BT 33 84
    LDA ($20,X)              ; @BT A6 98 20
    LEAX,X++                 ; @BT 30 81
    LDA,--U                  ; @BT A6 C3
    CMPD $1B                 ; @BT 10 93 1B
    LDA ($1234,PCR)          ; @BT A6 9D 12 34
    STS $1254                ; @BT 10 FF 12 54
    SBCA ,X                  ; @BT A2 84
    STY,X                    ; @BT 10 AF 84
    CMPX $1242               ; @BT BC 12 42
    CMPU ,X                  ; @BT 11 A3 84
    ORB $2A                  ; @BT DA 2A
    JMP $2000                ; @BT 7E 20 00
    SUBA $1258               ; @BT B0 12 58
    COMB                     ; @BT 53
    ROR $3A                  ; @BT 06 3A
    LDU ,X                   ; @BT EE 84
    STS $30                  ; @BT 10 DF 30
    LDA,--S                  ; @BT A6 E3
    LDA,Y+                   ; @BT A6 A0
    LDA (,U++)               ; @BT A6 D1
    LDA 5,Y                  ; @BT A6 25
    CMPY $1F                 ; @BT 10 9C 1F
    ADCB $1235               ; @BT F9 12 35
    PULU CC,A,B,DP,X,Y,S,PC ; @BT 37 FF
    LDA ($40,U)              ; @BT A6 D8 40
    LEAY ,X                  ; @BT 31 84
    STU $1255                ; @BT FF 12 55
    SBCB #((($20*8)/4)-($20*2)+$20)                ; @BT C2 20
    JMP ,X                   ; @BT 6E 84
    INC ,X                   ; @BT 6C 84
    STA,X                    ; @BT A7 84
    LDA,S++                  ; @BT A6 E1
    CMPS,X                   ; @BT 11 AC 84
    TSTB                     ; @BT 5D
    INC 5,X                  ; @BT 6C 05
    SUBD ,X                  ; @BT A3 84
    ANDB ,X                  ; @BT E4 84
    CLR $44                  ; @BT 0F 44
    SWI2                     ; @BT 10 3F
    ADCA $10                 ; @BT 99 10
    DECA                     ; @BT 4A
    LDA (,S++)               ; @BT A6 F1
    BITA $17                 ; @BT 95 17
    CMPU #((($0F*8)/4)-($0F*2)+$0F)                ; @BT 11 83 0F
    LDS #((($18*8)/4)-($18*2)+$18)                 ; @BT 10 CE 18
    ADCB ,X                  ; @BT E9 84
    TST $41                  ; @BT 0D 41
    LDA,Y++                  ; @BT A6 A1
    ORA #((($1C*8)/4)-($1C*2)+$1C)                 ; @BT 8A 1C
    LEAX ,X                  ; @BT 30 84
    LDA (A,U)                ; @BT A6 D6
    EORB $1245               ; @BT F8 12 45
    SEX                      ; @BT 1D
    LDA $1234,S              ; @BT A6 E9 12 34
    INCB                     ; @BT 5C
    ANDA $1239               ; @BT B4 12 39
    LDY $28                  ; @BT 10 9E 28
    LDA (A,X)                ; @BT A6 96
    ROLA                     ; @BT 49
    STU $31                  ; @BT DF 31
    LDA (,--Y)               ; @BT A6 B3
    STA $1251                ; @BT B7 12 51
    TST,S                    ; @BT 6D E4
    LDY,X++                  ; @BT 10 AE 81
    LDA D,U                  ; @BT A6 CB
    CMPX ,X                  ; @BT AC 84
    EORB ,X                  ; @BT E8 84
    INCA                     ; @BT 4C
    ASL ,X                   ; @BT 68 84
    STY $1257                ; @BT 10 BF 12 57
    ASL,X                    ; @BT 68 84
    LDA,U+                   ; @BT A6 C0
    LDA 0,X                  ; @BT A6 00
    ANDCC #((($08*8)/4)-($08*2)+$08)               ; @BT 1C 08
    COM,X                    ; @BT 63 84
    STA $2D                  ; @BT 97 2D
    PULS CC,A,B,DP,X,Y,U,PC ; @BT 35 FF
    LDA 15,Y                 ; @BT A6 2F
    ADDD $14                 ; @BT D3 14
    TFR A,B                  ; @BT 1F 89
    PULU A                   ; @BT 37 02
    SBCB $1250               ; @BT F2 12 50
    RTI                      ; @BT 3B
    ORB $124E                ; @BT FA 12 4E
    LDA ($5678,Y)            ; @BT A6 B9 56 78
    CMPD ,X                  ; @BT 10 A3 84
    LDA,-U                   ; @BT A6 C2
    DEC,X                    ; @BT 6A 84
    LDA 0,U                  ; @BT A6 40
    CMPB ,X                  ; @BT E1 84
    LDA $1234,X              ; @BT A6 89 12 34
    ADDB #((($04*8)/4)-($04*2)+$04)                ; @BT CB 04
    ADDB $1237               ; @BT FB 12 37
    LDA,S+                   ; @BT A6 E0
    LDA B,S                  ; @BT A6 E5
    JSR ,X                   ; @BT AD 84
    STB $1252                ; @BT F7 12 52
    ASRB                     ; @BT 57
    ROL $1267                ; @BT 79 12 67
    SUBB $35                 ; @BT D0 35
    LDS,--Y                  ; @BT 10 EE A3
    LEAU,U                   ; @BT 33 C4
    CMPD,X                   ; @BT 10 A3 84
    LBRA T3_LBR1             ; @BT 16 00 40
    LBRN T3_LBR1             ; @BT 10 21 00 3C
    LBHI T3_LBR1             ; @BT 10 22 00 38
    LBLS T3_LBR1             ; @BT 10 23 00 34
    LBCC T3_LBR1             ; @BT 10 24 00 30
    LBHS T3_LBR1             ; @BT 10 24 00 2C
    LBCS T3_LBR1             ; @BT 10 25 00 28
    LBLO T3_LBR1             ; @BT 10 25 00 24
    LBNE T3_LBR1             ; @BT 10 26 00 20
    LBEQ T3_LBR1             ; @BT 10 27 00 1C
    LBVC T3_LBR1             ; @BT 10 28 00 18
    LBVS T3_LBR1             ; @BT 10 29 00 14
    LBPL T3_LBR1             ; @BT 10 2A 00 10
    LBMI T3_LBR1             ; @BT 10 2B 00 0C
    LBGE T3_LBR1             ; @BT 10 2C 00 08
    LBLT T3_LBR1             ; @BT 10 2D 00 04
    LBGT T3_LBR1             ; @BT 10 2E 00 00
T3_LBR1:
    LBLE T3_LBR2             ; @BT 10 2F 00 03
    LBSR T3_LBR2             ; @BT 17 00 00
T3_LBR2:
    LDA ,X                   ; @BT A6 84
    LDA (B,S)                ; @BT A6 F5
    CLR,X                    ; @BT 6F 84
    SUBD $125A               ; @BT B3 12 5A
    LDX $124B                ; @BT BE 12 4B
    LEAU 5,U                 ; @BT 33 45
    LDA ($1234,S)            ; @BT A6 F9 12 34
    ADDA $12                 ; @BT 9B 12
    STD ,X                   ; @BT ED 84
    ASLB                     ; @BT 58
    LDA -16,S                ; @BT A6 70
    ROR,S                    ; @BT 66 E4
    CMPA $19                 ; @BT 91 19
    CMPS $1C                 ; @BT 11 9C 1C
    NEG $37                  ; @BT 00 37
    LDA -1,X                 ; @BT A6 1F
    LDA $22                  ; @BT 96 22
    LDA (B,U)                ; @BT A6 D5
    LDS $25                  ; @BT 10 DE 25
    LDA $1234,U              ; @BT A6 C9 12 34
    EORB $21                 ; @BT D8 21
    LDA $5678,Y              ; @BT A6 A9 56 78
    LEAY 5,Y                 ; @BT 31 25
    SUBA #((($21*8)/4)-($21*2)+$21)                ; @BT 80 21
    LDD $1248                ; @BT FC 12 48
    NOP                      ; @BT 12
    JSR $3456                ; @BT BD 34 56
    LSR ,X                   ; @BT 64 84
    LDA,--Y                  ; @BT A6 A3
    ADDA #((($03*8)/4)-($03*2)+$03)                ; @BT 8B 03
    DEC $3F                  ; @BT 0A 3F
    ADDB ,X                  ; @BT EB 84
    PSHS CC                  ; @BT 34 01
    LDA,-S                   ; @BT A6 E2
    ORCC #((($1E*8)/4)-($1E*2)+$1E)                ; @BT 1A 1E
    LDA $1234,PCR            ; @BT A6 8D 12 34
    LDD ,X                   ; @BT EC 84
    TFR D,X                  ; @BT 1F 01
    SUBA $34                 ; @BT 90 34
    STA ,X                   ; @BT A7 84
    EORA $20                 ; @BT 98 20
    LDB ,X                   ; @BT E6 84
    LDA ,Y                   ; @BT A6 A4
    ADDA ,X                  ; @BT AB 84
    SWI                      ; @BT 3F
    CMPY ,X                  ; @BT 10 AC 84
    ANDB $16                 ; @BT D4 16
    LSRA                     ; @BT 44

    .db "<03"               ; @BT END

    /////////////////////////////////////////////////////////////////////////////
    // TEST-04: similar to TEST-03 and then defines + expressions
    /////////////////////////////////////////////////////////////////////////////


    ; TEST-04 generated defines
    .define T4_DEF_001 (((((($01*8)/4)-($01*2)+$01))*8)/4-((((($01*8)/4)-($01*2)+$01))*2)+(((($01*8)/4)-($01*2)+$01)))
    .define T4_DEF_002 ((T4_DEF_001)+((((($1B*8)/4)-($1B*2)+$1B))-(((($01*8)/4)-($01*2)+$01))))
    .define T4_DEF_003 (((((($15*8)/4)-($15*2)+$15))*8)/4-((((($15*8)/4)-($15*2)+$15))*2)+(((($15*8)/4)-($15*2)+$15)))
    .define T4_DEF_004 ((T4_DEF_003)+((((($12*8)/4)-($12*2)+$12))-(((($15*8)/4)-($15*2)+$15))))
    .define T4_DEF_005 (((((($0A*8)/4)-($0A*2)+$0A))*8)/4-((((($0A*8)/4)-($0A*2)+$0A))*2)+(((($0A*8)/4)-($0A*2)+$0A)))
    .define T4_DEF_006 ((T4_DEF_005)+((((($0B*8)/4)-($0B*2)+$0B))-(((($0A*8)/4)-($0A*2)+$0A))))
    .define T4_DEF_007 (((((($10*8)/4)-($10*2)+$10))*8)/4-((((($10*8)/4)-($10*2)+$10))*2)+(((($10*8)/4)-($10*2)+$10)))
    .define T4_DEF_008 ((T4_DEF_007)+((((($11*8)/4)-($11*2)+$11))-(((($10*8)/4)-($10*2)+$10))))
    .define T4_DEF_009 (((((($1D*8)/4)-($1D*2)+$1D))*8)/4-((((($1D*8)/4)-($1D*2)+$1D))*2)+(((($1D*8)/4)-($1D*2)+$1D)))
    .define T4_DEF_010 ((T4_DEF_009)+((((($22*8)/4)-($22*2)+$22))-(((($1D*8)/4)-($1D*2)+$1D))))
    .define T4_DEF_011 (((((($16*8)/4)-($16*2)+$16))*8)/4-((((($16*8)/4)-($16*2)+$16))*2)+(((($16*8)/4)-($16*2)+$16)))
    .define T4_DEF_012 ((T4_DEF_011)+((((($13*8)/4)-($13*2)+$13))-(((($16*8)/4)-($16*2)+$16))))
    .define T4_DEF_013 (((((($19*8)/4)-($19*2)+$19))*8)/4-((((($19*8)/4)-($19*2)+$19))*2)+(((($19*8)/4)-($19*2)+$19)))
    .define T4_DEF_014 ((T4_DEF_013)+((((($06*8)/4)-($06*2)+$06))-(((($19*8)/4)-($19*2)+$19))))
    .define T4_DEF_015 (((((($09*8)/4)-($09*2)+$09))*8)/4-((((($09*8)/4)-($09*2)+$09))*2)+(((($09*8)/4)-($09*2)+$09)))
    .define T4_DEF_016 ((T4_DEF_015)+((((($07*8)/4)-($07*2)+$07))-(((($09*8)/4)-($09*2)+$09))))
    .define T4_DEF_017 (((((($0C*8)/4)-($0C*2)+$0C))*8)/4-((((($0C*8)/4)-($0C*2)+$0C))*2)+(((($0C*8)/4)-($0C*2)+$0C)))
    .db "04>"               ; @BT TEST-04 04 START

    ; GENERATED: copied from TEST-03; half immediate operands replaced by defines
    ; GENERATED: replaced immediates = 17 / 35

    ; GENERATED: copied from TEST-02 with expressionized immediate operands

    ; GENERATED: copied from TEST-01, randomized command order
    ; GENERATED: seed = 6809

    ASL $1265                ; @BT 78 12 65
    LDA $20,X                ; @BT A6 88 20
    STX ,X                   ; @BT AF 84
    ADCA #T4_DEF_001                ; @BT 89 01
    LDY #T4_DEF_002                 ; @BT 10 8E 1B
    LDA A,X                  ; @BT A6 86
    ANDB $123A               ; @BT F4 12 3A
    LDA (B,X)                ; @BT A6 95
    MUL                      ; @BT 3D
    LDA (A,S)                ; @BT A6 F6
    LDA 15,X                 ; @BT A6 0F
    LDA -2,S                 ; @BT A6 7E
    LDB $23                  ; @BT D6 23
    LDA #T4_DEF_003                 ; @BT 86 15
    CLR $126B                ; @BT 7F 12 6B
    LSR $1262                ; @BT 74 12 62
    PSHS Y                   ; @BT 34 20
    LEAS,S++                 ; @BT 32 E1
    CLRB                     ; @BT 5F
    SBCB $2C                 ; @BT D2 2C
    EXG CC,DP                ; @BT 1E AB
    CMPD $123F               ; @BT 10 B3 12 3F
    LDA (D,U)                ; @BT A6 DB
    LDA (,--U)               ; @BT A6 D3
    CWAI #T4_DEF_004                ; @BT 3C 12
    LSR $39                  ; @BT 04 39
    LDA ($20,PCR)            ; @BT A6 9C 20
    RORB                     ; @BT 56
    PSHS U                   ; @BT 34 40
    CMPS $1240               ; @BT 11 BC 12 40
    ASRA                     ; @BT 47
    BITB #T4_DEF_005                ; @BT C5 0A
    CMPA #T4_DEF_006                ; @BT 81 0B
    LEAS -2,S                ; @BT 32 7E
    LDA (,--S)               ; @BT A6 F3
    LDA (D,Y)                ; @BT A6 BB
    TFR X,Y                  ; @BT 1F 12
    CMPX #T4_DEF_007                ; @BT 8C 10
    LDX $27                  ; @BT 9E 27
    LDY ,X                   ; @BT 10 AE 84
    LDA A,Y                  ; @BT A6 A6
    JSR $43                  ; @BT 9D 43
    LDA B,X                  ; @BT A6 85
    SBCA $124F               ; @BT B2 12 4F
    LDA 5,X                  ; @BT A6 05
    ORA $124D                ; @BT BA 12 4D
    CMPY #T4_DEF_008                ; @BT 10 8C 11
    PSHS CC,A,B,DP,X,Y,U,PC ; @BT 34 FF
    ORA $29                  ; @BT 9A 29
    LDU $26                  ; @BT DE 26
    LDX ,X                   ; @BT AE 84
    LSL $1266                ; @BT 78 12 66
    ORB #T4_DEF_009                 ; @BT CA 1D
    LDA ,U                   ; @BT A6 C4
    TSTA                     ; @BT 4D
    SUBB #T4_DEF_010                ; @BT C0 22
    ROL ,X                   ; @BT 69 84
    CMPY A,S                 ; @BT 10 AC E6
    ANDA ,X                  ; @BT A4 84
    LEAY,X++                 ; @BT 31 81
    ROL $3E                  ; @BT 09 3E
    LSLA                     ; @BT 48
    LDA ($30,Y)              ; @BT A6 B8 30
    LDB #T4_DEF_011                 ; @BT C6 16
    SWI3                     ; @BT 11 3F
    DEC ,X                   ; @BT 6A 84
    BITB $18                 ; @BT D5 18
    BRA T4_BR1               ; @BT 20 20
    BRN T4_BR1               ; @BT 21 1E
    BHI T4_BR1               ; @BT 22 1C
    BLS T4_BR1               ; @BT 23 1A
    BCC T4_BR1               ; @BT 24 18
    BHS T4_BR1               ; @BT 24 16
    BCS T4_BR1               ; @BT 25 14
    BLO T4_BR1               ; @BT 25 12
    BNE T4_BR1               ; @BT 26 10
    BEQ T4_BR1               ; @BT 27 0E
    BVC T4_BR1               ; @BT 28 0C
    BVS T4_BR1               ; @BT 29 0A
    BPL T4_BR1               ; @BT 2A 08
    BMI T4_BR1               ; @BT 2B 06
    BGE T4_BR1               ; @BT 2C 04
    BLT T4_BR1               ; @BT 2D 02
    BGT T4_BR1               ; @BT 2E 00
T4_BR1:
    BLE T4_BR2               ; @BT 2F 02
    BSR T4_BR2               ; @BT 8D 00
T4_BR2:
    CMPB $123E               ; @BT F1 12 3E
    ASR,X                    ; @BT 67 84
    EORA #T4_DEF_012                ; @BT 88 13
    SBCA $2B                 ; @BT 92 2B
    LSL $3D                  ; @BT 08 3D
    LDA ($1234,X)            ; @BT A6 99 12 34
    LDA ($1234,U)            ; @BT A6 D9 12 34
    ROR $1263                ; @BT 76 12 63
    STX $32                  ; @BT 9F 32
    STD $2F                  ; @BT DD 2F
    EORA ,X                  ; @BT A8 84
    ADDB $13                 ; @BT DB 13
    NEG $1260                ; @BT 70 12 60
    LDA,-Y                   ; @BT A6 A2
    JMP $42                  ; @BT 0E 42
    LDU #T4_DEF_013                 ; @BT CE 19
    NEGB                     ; @BT 50
    PSHS B                   ; @BT 34 04
    SUBB ,X                  ; @BT E0 84
    LDA D,Y                  ; @BT A6 AB
    LDA (,Y++)               ; @BT A6 B1
    PSHS PC                  ; @BT 34 80
    LDA B,Y                  ; @BT A6 A5
    ANDA #T4_DEF_014                ; @BT 84 06
    BITA #T4_DEF_015                ; @BT 85 09
    ASR $3B                  ; @BT 07 3B
    ANDB #T4_DEF_016                ; @BT C4 07
    LSL ,X                   ; @BT 68 84
    LDA (B,Y)                ; @BT A6 B5
    INC $40                  ; @BT 0C 40
    ADDD ,X                  ; @BT E3 84
    BITB ,X                  ; @BT E5 84
    ROR ,X                   ; @BT 66 84
    LDA,Y                    ; @BT A6 A4
    CMPB #T4_DEF_017                ; @BT C1 0C
    ADCA ,X                  ; @BT A9 84
    STY ,X                   ; @BT 10 AF 84
    COM $38                  ; @BT 03 38
    CMPD #((($0D*8)/4)-($0D*2)+$0D)                ; @BT 10 83 0D
    LDA A,S                  ; @BT A6 E6
    LDA (D,X)                ; @BT A6 9B
    LDA 0,S                  ; @BT A6 60
    LDA (,X)                 ; @BT A6 94
    SUBD #((($23*8)/4)-($23*2)+$23)                ; @BT 83 23
    EXG D,X                  ; @BT 1E 01
    LDA $7F,Y                ; @BT A6 A8 7F
    PSHU CC,A,B,DP,X,Y,S,PC ; @BT 36 FF
    ORB ,X                   ; @BT EA 84
    ROL,X                    ; @BT 69 84
    LDA ($1234)              ; @BT A6 9F 12 34
    ASL $3C                  ; @BT 08 3C
    LDA,S                    ; @BT A6 E4
    SUBB $1259               ; @BT F0 12 59
    LDA,X                    ; @BT A6 84
    ADCB $11                 ; @BT D9 11
    LDA $1246                ; @BT B6 12 46
    RTS                      ; @BT 39
    PULS A                   ; @BT 35 02
    CMPS ,X                  ; @BT 11 AC 84
    LSLB                     ; @BT 58
    LDA (,S)                 ; @BT A6 F4
    TST ,X                   ; @BT 6D 84
    EXG A,B                  ; @BT 1E 89
    EORB #((($14*8)/4)-($14*2)+$14)                ; @BT C8 14
    CMPS #((($0E*8)/4)-($0E*2)+$0E)                ; @BT 11 8C 0E
    STB $2E                  ; @BT D7 2E
    ADCA $1234               ; @BT B9 12 34
    LDU $124A                ; @BT FE 12 4A
    STS $20,X                ; @BT 10 EF 88 20
    STD,Y                    ; @BT ED A4
    ROLB                     ; @BT 59
    SBCB ,X                  ; @BT E2 84
    LDA,U++                  ; @BT A6 C1
    LDA $20,Y                ; @BT A6 A8 20
    RORA                     ; @BT 46
    CMPX $1E                 ; @BT 9C 1E
    NEG,Y                    ; @BT 60 A4
    LDA (,U)                 ; @BT A6 D4
    EORA $1244               ; @BT B8 12 44
    LDA 0,Y                  ; @BT A6 20
    ADCB #((($02*8)/4)-($02*2)+$02)                ; @BT C9 02
    CMPA ,X                  ; @BT A1 84
    ADDA $1236               ; @BT BB 12 36
    EXG U,S                  ; @BT 1E 34
    LDA $20,U                ; @BT A6 C8 20
    LDA,U                    ; @BT A6 C4
    LDA -16,X                ; @BT A6 10
    LDA (,Y)                 ; @BT A6 B4
    BITA $123B               ; @BT B5 12 3B
    STY $33                  ; @BT 10 9F 33
    LDA D,S                  ; @BT A6 EB
    SWI1                     ; @BT 3F
    ASR ,X                   ; @BT 67 84
    COMA                     ; @BT 43
    LDB $1247                ; @BT F6 12 47
    LDX #((($1A*8)/4)-($1A*2)+$1A)                 ; @BT 8E 1A
    ANDA $15                 ; @BT 94 15
    LDA ($50,S)              ; @BT A6 F8 50
    LSL,Y                    ; @BT 68 A4
    COM ,X                   ; @BT 63 84
    SUBD $36                 ; @BT 93 36
    TST $126A                ; @BT 7D 12 6A
    BITA ,X                  ; @BT A5 84
    STS ,X                   ; @BT 10 EF 84
    CMPU,Y                   ; @BT 11 A3 A4
    STU ,X                   ; @BT EF 84
    LDA (,X++)               ; @BT A6 91
    DAA                      ; @BT 19
    LDA $7F,X                ; @BT A6 88 7F
    LDA,--X                  ; @BT A6 83
    CLRA                     ; @BT 4F
    LDA 5,PCR                ; @BT A6 8C 05
    DEC $1268                ; @BT 7A 12 68
    EXG X,Y                  ; @BT 1E 12
    LDS ,X                   ; @BT 10 EE 84
    INC $1269                ; @BT 7C 12 69
    SYNC                     ; @BT 13
    DECB                     ; @BT 5A
    LDA,X++                  ; @BT A6 81
    LDD #((($17*8)/4)-($17*2)+$17)                 ; @BT CC 17
    LDA (,--X)               ; @BT A6 93
    PSHS DP                  ; @BT 34 08
    PSHS A                   ; @BT 34 02
    LEAX 5,X                 ; @BT 30 05
    LDA 5,U                  ; @BT A6 45
    LDA A,U                  ; @BT A6 C6
    JSR 5,Y                  ; @BT AD 25
    BITB $123C               ; @BT F5 12 3C
    ORA ,X                   ; @BT AA 84
    ADDD #((($05*8)/4)-($05*2)+$05)                ; @BT C3 05
    COM $1261                ; @BT 73 12 61
    CMPB $1A                 ; @BT D1 1A
    ABX                      ; @BT 3A
    LSR,U                    ; @BT 64 C4
    LSRB                     ; @BT 54
    CMPU $1D                 ; @BT 11 93 1D
    ASLA                     ; @BT 48
    STX $1256                ; @BT BF 12 56
    LDY $124C                ; @BT 10 BE 12 4C
    CMPU $1241               ; @BT 11 B3 12 41
    LDD $24                  ; @BT DC 24
    LDA,X+                   ; @BT A6 80
    SUBA ,X                  ; @BT A0 84
    LDS $1249                ; @BT 10 FE 12 49
    LDA,-X                   ; @BT A6 82
    LEAX $1234,X             ; @BT 30 89 12 34
    STB ,X                   ; @BT E7 84
    PSHU A                   ; @BT 36 02
    PSHS X                   ; @BT 34 10
    LDA ,S                   ; @BT A6 E4
    LEAS ,X                  ; @BT 32 84
    LDA (D,S)                ; @BT A6 FB
    LDA $30,S                ; @BT A6 E8 30
    NEG ,X                   ; @BT 60 84
    ADDD $1238               ; @BT F3 12 38
    JMP,X                    ; @BT 6E 84
    LDA B,U                  ; @BT A6 C5
    SBCA #((($1F*8)/4)-($1F*2)+$1F)                ; @BT 82 1F
    STD $1253                ; @BT FD 12 53
    ASR $1264                ; @BT 77 12 64
    LDA (A,Y)                ; @BT A6 B6
    CMPY $1243               ; @BT 10 BC 12 43
    NEGA                     ; @BT 40
    CMPA $123D               ; @BT B1 12 3D
    LDA D,X                  ; @BT A6 8B
    CLR ,X                   ; @BT 6F 84
    LEAU ,X                  ; @BT 33 84
    LDA ($20,X)              ; @BT A6 98 20
    LEAX,X++                 ; @BT 30 81
    LDA,--U                  ; @BT A6 C3
    CMPD $1B                 ; @BT 10 93 1B
    LDA ($1234,PCR)          ; @BT A6 9D 12 34
    STS $1254                ; @BT 10 FF 12 54
    SBCA ,X                  ; @BT A2 84
    STY,X                    ; @BT 10 AF 84
    CMPX $1242               ; @BT BC 12 42
    CMPU ,X                  ; @BT 11 A3 84
    ORB $2A                  ; @BT DA 2A
    JMP $2000                ; @BT 7E 20 00
    SUBA $1258               ; @BT B0 12 58
    COMB                     ; @BT 53
    ROR $3A                  ; @BT 06 3A
    LDU ,X                   ; @BT EE 84
    STS $30                  ; @BT 10 DF 30
    LDA,--S                  ; @BT A6 E3
    LDA,Y+                   ; @BT A6 A0
    LDA (,U++)               ; @BT A6 D1
    LDA 5,Y                  ; @BT A6 25
    CMPY $1F                 ; @BT 10 9C 1F
    ADCB $1235               ; @BT F9 12 35
    PULU CC,A,B,DP,X,Y,S,PC ; @BT 37 FF
    LDA ($40,U)              ; @BT A6 D8 40
    LEAY ,X                  ; @BT 31 84
    STU $1255                ; @BT FF 12 55
    SBCB #((($20*8)/4)-($20*2)+$20)                ; @BT C2 20
    JMP ,X                   ; @BT 6E 84
    INC ,X                   ; @BT 6C 84
    STA,X                    ; @BT A7 84
    LDA,S++                  ; @BT A6 E1
    CMPS,X                   ; @BT 11 AC 84
    TSTB                     ; @BT 5D
    INC 5,X                  ; @BT 6C 05
    SUBD ,X                  ; @BT A3 84
    ANDB ,X                  ; @BT E4 84
    CLR $44                  ; @BT 0F 44
    SWI2                     ; @BT 10 3F
    ADCA $10                 ; @BT 99 10
    DECA                     ; @BT 4A
    LDA (,S++)               ; @BT A6 F1
    BITA $17                 ; @BT 95 17
    CMPU #((($0F*8)/4)-($0F*2)+$0F)                ; @BT 11 83 0F
    LDS #((($18*8)/4)-($18*2)+$18)                 ; @BT 10 CE 18
    ADCB ,X                  ; @BT E9 84
    TST $41                  ; @BT 0D 41
    LDA,Y++                  ; @BT A6 A1
    ORA #((($1C*8)/4)-($1C*2)+$1C)                 ; @BT 8A 1C
    LEAX ,X                  ; @BT 30 84
    LDA (A,U)                ; @BT A6 D6
    EORB $1245               ; @BT F8 12 45
    SEX                      ; @BT 1D
    LDA $1234,S              ; @BT A6 E9 12 34
    INCB                     ; @BT 5C
    ANDA $1239               ; @BT B4 12 39
    LDY $28                  ; @BT 10 9E 28
    LDA (A,X)                ; @BT A6 96
    ROLA                     ; @BT 49
    STU $31                  ; @BT DF 31
    LDA (,--Y)               ; @BT A6 B3
    STA $1251                ; @BT B7 12 51
    TST,S                    ; @BT 6D E4
    LDY,X++                  ; @BT 10 AE 81
    LDA D,U                  ; @BT A6 CB
    CMPX ,X                  ; @BT AC 84
    EORB ,X                  ; @BT E8 84
    INCA                     ; @BT 4C
    ASL ,X                   ; @BT 68 84
    STY $1257                ; @BT 10 BF 12 57
    ASL,X                    ; @BT 68 84
    LDA,U+                   ; @BT A6 C0
    LDA 0,X                  ; @BT A6 00
    ANDCC #((($08*8)/4)-($08*2)+$08)               ; @BT 1C 08
    COM,X                    ; @BT 63 84
    STA $2D                  ; @BT 97 2D
    PULS CC,A,B,DP,X,Y,U,PC ; @BT 35 FF
    LDA 15,Y                 ; @BT A6 2F
    ADDD $14                 ; @BT D3 14
    TFR A,B                  ; @BT 1F 89
    PULU A                   ; @BT 37 02
    SBCB $1250               ; @BT F2 12 50
    RTI                      ; @BT 3B
    ORB $124E                ; @BT FA 12 4E
    LDA ($5678,Y)            ; @BT A6 B9 56 78
    CMPD ,X                  ; @BT 10 A3 84
    LDA,-U                   ; @BT A6 C2
    DEC,X                    ; @BT 6A 84
    LDA 0,U                  ; @BT A6 40
    CMPB ,X                  ; @BT E1 84
    LDA $1234,X              ; @BT A6 89 12 34
    ADDB #((($04*8)/4)-($04*2)+$04)                ; @BT CB 04
    ADDB $1237               ; @BT FB 12 37
    LDA,S+                   ; @BT A6 E0
    LDA B,S                  ; @BT A6 E5
    JSR ,X                   ; @BT AD 84
    STB $1252                ; @BT F7 12 52
    ASRB                     ; @BT 57
    ROL $1267                ; @BT 79 12 67
    SUBB $35                 ; @BT D0 35
    LDS,--Y                  ; @BT 10 EE A3
    LEAU,U                   ; @BT 33 C4
    CMPD,X                   ; @BT 10 A3 84
    LBRA T4_LBR1             ; @BT 16 00 40
    LBRN T4_LBR1             ; @BT 10 21 00 3C
    LBHI T4_LBR1             ; @BT 10 22 00 38
    LBLS T4_LBR1             ; @BT 10 23 00 34
    LBCC T4_LBR1             ; @BT 10 24 00 30
    LBHS T4_LBR1             ; @BT 10 24 00 2C
    LBCS T4_LBR1             ; @BT 10 25 00 28
    LBLO T4_LBR1             ; @BT 10 25 00 24
    LBNE T4_LBR1             ; @BT 10 26 00 20
    LBEQ T4_LBR1             ; @BT 10 27 00 1C
    LBVC T4_LBR1             ; @BT 10 28 00 18
    LBVS T4_LBR1             ; @BT 10 29 00 14
    LBPL T4_LBR1             ; @BT 10 2A 00 10
    LBMI T4_LBR1             ; @BT 10 2B 00 0C
    LBGE T4_LBR1             ; @BT 10 2C 00 08
    LBLT T4_LBR1             ; @BT 10 2D 00 04
    LBGT T4_LBR1             ; @BT 10 2E 00 00
T4_LBR1:
    LBLE T4_LBR2             ; @BT 10 2F 00 03
    LBSR T4_LBR2             ; @BT 17 00 00
T4_LBR2:
    LDA ,X                   ; @BT A6 84
    LDA (B,S)                ; @BT A6 F5
    CLR,X                    ; @BT 6F 84
    SUBD $125A               ; @BT B3 12 5A
    LDX $124B                ; @BT BE 12 4B
    LEAU 5,U                 ; @BT 33 45
    LDA ($1234,S)            ; @BT A6 F9 12 34
    ADDA $12                 ; @BT 9B 12
    STD ,X                   ; @BT ED 84
    ASLB                     ; @BT 58
    LDA -16,S                ; @BT A6 70
    ROR,S                    ; @BT 66 E4
    CMPA $19                 ; @BT 91 19
    CMPS $1C                 ; @BT 11 9C 1C
    NEG $37                  ; @BT 00 37
    LDA -1,X                 ; @BT A6 1F
    LDA $22                  ; @BT 96 22
    LDA (B,U)                ; @BT A6 D5
    LDS $25                  ; @BT 10 DE 25
    LDA $1234,U              ; @BT A6 C9 12 34
    EORB $21                 ; @BT D8 21
    LDA $5678,Y              ; @BT A6 A9 56 78
    LEAY 5,Y                 ; @BT 31 25
    SUBA #((($21*8)/4)-($21*2)+$21)                ; @BT 80 21
    LDD $1248                ; @BT FC 12 48
    NOP                      ; @BT 12
    JSR $3456                ; @BT BD 34 56
    LSR ,X                   ; @BT 64 84
    LDA,--Y                  ; @BT A6 A3
    ADDA #((($03*8)/4)-($03*2)+$03)                ; @BT 8B 03
    DEC $3F                  ; @BT 0A 3F
    ADDB ,X                  ; @BT EB 84
    PSHS CC                  ; @BT 34 01
    LDA,-S                   ; @BT A6 E2
    ORCC #((($1E*8)/4)-($1E*2)+$1E)                ; @BT 1A 1E
    LDA $1234,PCR            ; @BT A6 8D 12 34
    LDD ,X                   ; @BT EC 84
    TFR D,X                  ; @BT 1F 01
    SUBA $34                 ; @BT 90 34
    STA ,X                   ; @BT A7 84
    EORA $20                 ; @BT 98 20
    LDB ,X                   ; @BT E6 84
    LDA ,Y                   ; @BT A6 A4
    ADDA ,X                  ; @BT AB 84
    SWI                      ; @BT 3F
    CMPY ,X                  ; @BT 10 AC 84
    ANDB $16                 ; @BT D4 16
    LSRA                     ; @BT 44

    .db "<04"               ; @BT END

    /////////////////////////////////////////////////////////////////////////////
    // TEST-05: similar to TEST-04 and defines + functions + expressions
    /////////////////////////////////////////////////////////////////////////////



    ; TEST-05 generated defines/functions
    .define T5_DEF_001 (((((($01*8)/4)-($01*2)+$01))*8)/4-((((($01*8)/4)-($01*2)+$01))*2)+(((($01*8)/4)-($01*2)+$01)))
    .define T5_DEF_002 ((T5_DEF_001)+((((($1B*8)/4)-($1B*2)+$1B))-(((($01*8)/4)-($01*2)+$01))))
    .define T5_DEF_003 (((((($15*8)/4)-($15*2)+$15))*8)/4-((((($15*8)/4)-($15*2)+$15))*2)+(((($15*8)/4)-($15*2)+$15)))
    .define T5_DEF_004 ((T5_DEF_003)+((((($12*8)/4)-($12*2)+$12))-(((($15*8)/4)-($15*2)+$15))))
    .define T5_DEF_005 (((((($0A*8)/4)-($0A*2)+$0A))*8)/4-((((($0A*8)/4)-($0A*2)+$0A))*2)+(((($0A*8)/4)-($0A*2)+$0A)))
    .define T5_DEF_006 ((T5_DEF_005)+((((($0B*8)/4)-($0B*2)+$0B))-(((($0A*8)/4)-($0A*2)+$0A))))
    .define T5_DEF_007 (((((($10*8)/4)-($10*2)+$10))*8)/4-((((($10*8)/4)-($10*2)+$10))*2)+(((($10*8)/4)-($10*2)+$10)))
    .define T5_DEF_008 ((T5_DEF_007)+((((($11*8)/4)-($11*2)+$11))-(((($10*8)/4)-($10*2)+$10))))
    .define T5_DEF_009 (((((($1D*8)/4)-($1D*2)+$1D))*8)/4-((((($1D*8)/4)-($1D*2)+$1D))*2)+(((($1D*8)/4)-($1D*2)+$1D)))
    .define T5_DEF_010 ((T5_DEF_009)+((((($22*8)/4)-($22*2)+$22))-(((($1D*8)/4)-($1D*2)+$1D))))
    .define T5_DEF_011 (((((($16*8)/4)-($16*2)+$16))*8)/4-((((($16*8)/4)-($16*2)+$16))*2)+(((($16*8)/4)-($16*2)+$16)))
    .define T5_DEF_012 ((T5_DEF_011)+((((($13*8)/4)-($13*2)+$13))-(((($16*8)/4)-($16*2)+$16))))
    .define T5_DEF_013 (((((($19*8)/4)-($19*2)+$19))*8)/4-((((($19*8)/4)-($19*2)+$19))*2)+(((($19*8)/4)-($19*2)+$19)))
    .define T5_DEF_014 ((T5_DEF_013)+((((($06*8)/4)-($06*2)+$06))-(((($19*8)/4)-($19*2)+$19))))
    .define T5_DEF_015 (((((($09*8)/4)-($09*2)+$09))*8)/4-((((($09*8)/4)-($09*2)+$09))*2)+(((($09*8)/4)-($09*2)+$09)))
    .define T5_DEF_016 ((T5_DEF_015)+((((($07*8)/4)-($07*2)+$07))-(((($09*8)/4)-($09*2)+$09))))
    .define T5_DEF_017 (((((($0C*8)/4)-($0C*2)+$0C))*8)/4-((((($0C*8)/4)-($0C*2)+$0C))*2)+(((($0C*8)/4)-($0C*2)+$0C)))

    ; TEST-05 generated functions
    .function T5_FN_001(a,b,c) ((((a+b-c)*8)/4)-((a+b-c)*2)+(a+b-c)+T5_DEF_001-T5_DEF_001)
    .function T5_FN_002(a,b,c) (((T5_FN_001(a,b,c)*8)/4)-(T5_FN_001(a,b,c)*2)+T5_FN_001(a,b,c)+T5_DEF_002-T5_DEF_002)
    .function T5_FN_003(a,b,c) ((((a+b-c)*8)/4)-((a+b-c)*2)+(a+b-c)+T5_DEF_003-T5_DEF_003)
    .function T5_FN_004(a,b,c) (((T5_FN_003(a,b,c)*8)/4)-(T5_FN_003(a,b,c)*2)+T5_FN_003(a,b,c)+T5_DEF_004-T5_DEF_004)
    .function T5_FN_005(a,b,c) ((((a+b-c)*8)/4)-((a+b-c)*2)+(a+b-c)+T5_DEF_005-T5_DEF_005)
    .function T5_FN_006(a,b,c) (((T5_FN_005(a,b,c)*8)/4)-(T5_FN_005(a,b,c)*2)+T5_FN_005(a,b,c)+T5_DEF_006-T5_DEF_006)
    .function T5_FN_007(a,b,c) ((((a+b-c)*8)/4)-((a+b-c)*2)+(a+b-c)+T5_DEF_007-T5_DEF_007)
    .function T5_FN_008(a,b,c) (((T5_FN_007(a,b,c)*8)/4)-(T5_FN_007(a,b,c)*2)+T5_FN_007(a,b,c)+T5_DEF_008-T5_DEF_008)
    .db "05>"               ; @BT TEST-05 05 START

    ; GENERATED: copied from TEST-04; half define-label immediates replaced by varied function calls
    ; GENERATED: replaced define refs = 8 / 17

    ; GENERATED: copied from TEST-03; half immediate operands replaced by defines
    ; GENERATED: replaced immediates = 17 / 35

    ; GENERATED: copied from TEST-02 with expressionized immediate operands

    ; GENERATED: copied from TEST-01, randomized command order
    ; GENERATED: seed = 6809

    ASL $1265                ; @BT 78 12 65
    LDA $20,X                ; @BT A6 88 20
    STX ,X                   ; @BT AF 84
    ADCA #T5_FN_001(T5_DEF_001+2,3,2+3)                ; @BT 89 01
    LDY #T5_FN_002(T5_DEF_002+3,4,3+4)                 ; @BT 10 8E 1B
    LDA A,X                  ; @BT A6 86
    ANDB $123A               ; @BT F4 12 3A
    LDA (B,X)                ; @BT A6 95
    MUL                      ; @BT 3D
    LDA (A,S)                ; @BT A6 F6
    LDA 15,X                 ; @BT A6 0F
    LDA -2,S                 ; @BT A6 7E
    LDB $23                  ; @BT D6 23
    LDA #T5_FN_003(T5_DEF_003+4,5,4+5)                 ; @BT 86 15
    CLR $126B                ; @BT 7F 12 6B
    LSR $1262                ; @BT 74 12 62
    PSHS Y                   ; @BT 34 20
    LEAS,S++                 ; @BT 32 E1
    CLRB                     ; @BT 5F
    SBCB $2C                 ; @BT D2 2C
    EXG CC,DP                ; @BT 1E AB
    CMPD $123F               ; @BT 10 B3 12 3F
    LDA (D,U)                ; @BT A6 DB
    LDA (,--U)               ; @BT A6 D3
    CWAI #T5_FN_004(T5_DEF_004+5,6,5+6)                ; @BT 3C 12
    LSR $39                  ; @BT 04 39
    LDA ($20,PCR)            ; @BT A6 9C 20
    RORB                     ; @BT 56
    PSHS U                   ; @BT 34 40
    CMPS $1240               ; @BT 11 BC 12 40
    ASRA                     ; @BT 47
    BITB #T5_FN_005(T5_DEF_005+6,7,6+7)                ; @BT C5 0A
    CMPA #T5_FN_006(T5_DEF_006+7,8,7+8)                ; @BT 81 0B
    LEAS -2,S                ; @BT 32 7E
    LDA (,--S)               ; @BT A6 F3
    LDA (D,Y)                ; @BT A6 BB
    TFR X,Y                  ; @BT 1F 12
    CMPX #T5_FN_007(T5_DEF_007+8,9,8+9)                ; @BT 8C 10
    LDX $27                  ; @BT 9E 27
    LDY ,X                   ; @BT 10 AE 84
    LDA A,Y                  ; @BT A6 A6
    JSR $43                  ; @BT 9D 43
    LDA B,X                  ; @BT A6 85
    SBCA $124F               ; @BT B2 12 4F
    LDA 5,X                  ; @BT A6 05
    ORA $124D                ; @BT BA 12 4D
    CMPY #T5_FN_008(T5_DEF_008+9,10,9+10)                ; @BT 10 8C 11
    PSHS CC,A,B,DP,X,Y,U,PC ; @BT 34 FF
    ORA $29                  ; @BT 9A 29
    LDU $26                  ; @BT DE 26
    LDX ,X                   ; @BT AE 84
    LSL $1266                ; @BT 78 12 66
    ORB #T5_DEF_009                 ; @BT CA 1D
    LDA ,U                   ; @BT A6 C4
    TSTA                     ; @BT 4D
    SUBB #T5_DEF_010                ; @BT C0 22
    ROL ,X                   ; @BT 69 84
    CMPY A,S                 ; @BT 10 AC E6
    ANDA ,X                  ; @BT A4 84
    LEAY,X++                 ; @BT 31 81
    ROL $3E                  ; @BT 09 3E
    LSLA                     ; @BT 48
    LDA ($30,Y)              ; @BT A6 B8 30
    LDB #T5_DEF_011                 ; @BT C6 16
    SWI3                     ; @BT 11 3F
    DEC ,X                   ; @BT 6A 84
    BITB $18                 ; @BT D5 18
    BRA T5_BR1               ; @BT 20 20
    BRN T5_BR1               ; @BT 21 1E
    BHI T5_BR1               ; @BT 22 1C
    BLS T5_BR1               ; @BT 23 1A
    BCC T5_BR1               ; @BT 24 18
    BHS T5_BR1               ; @BT 24 16
    BCS T5_BR1               ; @BT 25 14
    BLO T5_BR1               ; @BT 25 12
    BNE T5_BR1               ; @BT 26 10
    BEQ T5_BR1               ; @BT 27 0E
    BVC T5_BR1               ; @BT 28 0C
    BVS T5_BR1               ; @BT 29 0A
    BPL T5_BR1               ; @BT 2A 08
    BMI T5_BR1               ; @BT 2B 06
    BGE T5_BR1               ; @BT 2C 04
    BLT T5_BR1               ; @BT 2D 02
    BGT T5_BR1               ; @BT 2E 00
T5_BR1:
    BLE T5_BR2               ; @BT 2F 02
    BSR T5_BR2               ; @BT 8D 00
T5_BR2:
    CMPB $123E               ; @BT F1 12 3E
    ASR,X                    ; @BT 67 84
    EORA #T5_DEF_012                ; @BT 88 13
    SBCA $2B                 ; @BT 92 2B
    LSL $3D                  ; @BT 08 3D
    LDA ($1234,X)            ; @BT A6 99 12 34
    LDA ($1234,U)            ; @BT A6 D9 12 34
    ROR $1263                ; @BT 76 12 63
    STX $32                  ; @BT 9F 32
    STD $2F                  ; @BT DD 2F
    EORA ,X                  ; @BT A8 84
    ADDB $13                 ; @BT DB 13
    NEG $1260                ; @BT 70 12 60
    LDA,-Y                   ; @BT A6 A2
    JMP $42                  ; @BT 0E 42
    LDU #T5_DEF_013                 ; @BT CE 19
    NEGB                     ; @BT 50
    PSHS B                   ; @BT 34 04
    SUBB ,X                  ; @BT E0 84
    LDA D,Y                  ; @BT A6 AB
    LDA (,Y++)               ; @BT A6 B1
    PSHS PC                  ; @BT 34 80
    LDA B,Y                  ; @BT A6 A5
    ANDA #T5_DEF_014                ; @BT 84 06
    BITA #T5_DEF_015                ; @BT 85 09
    ASR $3B                  ; @BT 07 3B
    ANDB #T5_DEF_016                ; @BT C4 07
    LSL ,X                   ; @BT 68 84
    LDA (B,Y)                ; @BT A6 B5
    INC $40                  ; @BT 0C 40
    ADDD ,X                  ; @BT E3 84
    BITB ,X                  ; @BT E5 84
    ROR ,X                   ; @BT 66 84
    LDA,Y                    ; @BT A6 A4
    CMPB #T5_DEF_017                ; @BT C1 0C
    ADCA ,X                  ; @BT A9 84
    STY ,X                   ; @BT 10 AF 84
    COM $38                  ; @BT 03 38
    CMPD #((($0D*8)/4)-($0D*2)+$0D)                ; @BT 10 83 0D
    LDA A,S                  ; @BT A6 E6
    LDA (D,X)                ; @BT A6 9B
    LDA 0,S                  ; @BT A6 60
    LDA (,X)                 ; @BT A6 94
    SUBD #((($23*8)/4)-($23*2)+$23)                ; @BT 83 23
    EXG D,X                  ; @BT 1E 01
    LDA $7F,Y                ; @BT A6 A8 7F
    PSHU CC,A,B,DP,X,Y,S,PC ; @BT 36 FF
    ORB ,X                   ; @BT EA 84
    ROL,X                    ; @BT 69 84
    LDA ($1234)              ; @BT A6 9F 12 34
    ASL $3C                  ; @BT 08 3C
    LDA,S                    ; @BT A6 E4
    SUBB $1259               ; @BT F0 12 59
    LDA,X                    ; @BT A6 84
    ADCB $11                 ; @BT D9 11
    LDA $1246                ; @BT B6 12 46
    RTS                      ; @BT 39
    PULS A                   ; @BT 35 02
    CMPS ,X                  ; @BT 11 AC 84
    LSLB                     ; @BT 58
    LDA (,S)                 ; @BT A6 F4
    TST ,X                   ; @BT 6D 84
    EXG A,B                  ; @BT 1E 89
    EORB #((($14*8)/4)-($14*2)+$14)                ; @BT C8 14
    CMPS #((($0E*8)/4)-($0E*2)+$0E)                ; @BT 11 8C 0E
    STB $2E                  ; @BT D7 2E
    ADCA $1234               ; @BT B9 12 34
    LDU $124A                ; @BT FE 12 4A
    STS $20,X                ; @BT 10 EF 88 20
    STD,Y                    ; @BT ED A4
    ROLB                     ; @BT 59
    SBCB ,X                  ; @BT E2 84
    LDA,U++                  ; @BT A6 C1
    LDA $20,Y                ; @BT A6 A8 20
    RORA                     ; @BT 46
    CMPX $1E                 ; @BT 9C 1E
    NEG,Y                    ; @BT 60 A4
    LDA (,U)                 ; @BT A6 D4
    EORA $1244               ; @BT B8 12 44
    LDA 0,Y                  ; @BT A6 20
    ADCB #((($02*8)/4)-($02*2)+$02)                ; @BT C9 02
    CMPA ,X                  ; @BT A1 84
    ADDA $1236               ; @BT BB 12 36
    EXG U,S                  ; @BT 1E 34
    LDA $20,U                ; @BT A6 C8 20
    LDA,U                    ; @BT A6 C4
    LDA -16,X                ; @BT A6 10
    LDA (,Y)                 ; @BT A6 B4
    BITA $123B               ; @BT B5 12 3B
    STY $33                  ; @BT 10 9F 33
    LDA D,S                  ; @BT A6 EB
    SWI1                     ; @BT 3F
    ASR ,X                   ; @BT 67 84
    COMA                     ; @BT 43
    LDB $1247                ; @BT F6 12 47
    LDX #((($1A*8)/4)-($1A*2)+$1A)                 ; @BT 8E 1A
    ANDA $15                 ; @BT 94 15
    LDA ($50,S)              ; @BT A6 F8 50
    LSL,Y                    ; @BT 68 A4
    COM ,X                   ; @BT 63 84
    SUBD $36                 ; @BT 93 36
    TST $126A                ; @BT 7D 12 6A
    BITA ,X                  ; @BT A5 84
    STS ,X                   ; @BT 10 EF 84
    CMPU,Y                   ; @BT 11 A3 A4
    STU ,X                   ; @BT EF 84
    LDA (,X++)               ; @BT A6 91
    DAA                      ; @BT 19
    LDA $7F,X                ; @BT A6 88 7F
    LDA,--X                  ; @BT A6 83
    CLRA                     ; @BT 4F
    LDA 5,PCR                ; @BT A6 8C 05
    DEC $1268                ; @BT 7A 12 68
    EXG X,Y                  ; @BT 1E 12
    LDS ,X                   ; @BT 10 EE 84
    INC $1269                ; @BT 7C 12 69
    SYNC                     ; @BT 13
    DECB                     ; @BT 5A
    LDA,X++                  ; @BT A6 81
    LDD #((($17*8)/4)-($17*2)+$17)                 ; @BT CC 17
    LDA (,--X)               ; @BT A6 93
    PSHS DP                  ; @BT 34 08
    PSHS A                   ; @BT 34 02
    LEAX 5,X                 ; @BT 30 05
    LDA 5,U                  ; @BT A6 45
    LDA A,U                  ; @BT A6 C6
    JSR 5,Y                  ; @BT AD 25
    BITB $123C               ; @BT F5 12 3C
    ORA ,X                   ; @BT AA 84
    ADDD #((($05*8)/4)-($05*2)+$05)                ; @BT C3 05
    COM $1261                ; @BT 73 12 61
    CMPB $1A                 ; @BT D1 1A
    ABX                      ; @BT 3A
    LSR,U                    ; @BT 64 C4
    LSRB                     ; @BT 54
    CMPU $1D                 ; @BT 11 93 1D
    ASLA                     ; @BT 48
    STX $1256                ; @BT BF 12 56
    LDY $124C                ; @BT 10 BE 12 4C
    CMPU $1241               ; @BT 11 B3 12 41
    LDD $24                  ; @BT DC 24
    LDA,X+                   ; @BT A6 80
    SUBA ,X                  ; @BT A0 84
    LDS $1249                ; @BT 10 FE 12 49
    LDA,-X                   ; @BT A6 82
    LEAX $1234,X             ; @BT 30 89 12 34
    STB ,X                   ; @BT E7 84
    PSHU A                   ; @BT 36 02
    PSHS X                   ; @BT 34 10
    LDA ,S                   ; @BT A6 E4
    LEAS ,X                  ; @BT 32 84
    LDA (D,S)                ; @BT A6 FB
    LDA $30,S                ; @BT A6 E8 30
    NEG ,X                   ; @BT 60 84
    ADDD $1238               ; @BT F3 12 38
    JMP,X                    ; @BT 6E 84
    LDA B,U                  ; @BT A6 C5
    SBCA #((($1F*8)/4)-($1F*2)+$1F)                ; @BT 82 1F
    STD $1253                ; @BT FD 12 53
    ASR $1264                ; @BT 77 12 64
    LDA (A,Y)                ; @BT A6 B6
    CMPY $1243               ; @BT 10 BC 12 43
    NEGA                     ; @BT 40
    CMPA $123D               ; @BT B1 12 3D
    LDA D,X                  ; @BT A6 8B
    CLR ,X                   ; @BT 6F 84
    LEAU ,X                  ; @BT 33 84
    LDA ($20,X)              ; @BT A6 98 20
    LEAX,X++                 ; @BT 30 81
    LDA,--U                  ; @BT A6 C3
    CMPD $1B                 ; @BT 10 93 1B
    LDA ($1234,PCR)          ; @BT A6 9D 12 34
    STS $1254                ; @BT 10 FF 12 54
    SBCA ,X                  ; @BT A2 84
    STY,X                    ; @BT 10 AF 84
    CMPX $1242               ; @BT BC 12 42
    CMPU ,X                  ; @BT 11 A3 84
    ORB $2A                  ; @BT DA 2A
    JMP $2000                ; @BT 7E 20 00
    SUBA $1258               ; @BT B0 12 58
    COMB                     ; @BT 53
    ROR $3A                  ; @BT 06 3A
    LDU ,X                   ; @BT EE 84
    STS $30                  ; @BT 10 DF 30
    LDA,--S                  ; @BT A6 E3
    LDA,Y+                   ; @BT A6 A0
    LDA (,U++)               ; @BT A6 D1
    LDA 5,Y                  ; @BT A6 25
    CMPY $1F                 ; @BT 10 9C 1F
    ADCB $1235               ; @BT F9 12 35
    PULU CC,A,B,DP,X,Y,S,PC ; @BT 37 FF
    LDA ($40,U)              ; @BT A6 D8 40
    LEAY ,X                  ; @BT 31 84
    STU $1255                ; @BT FF 12 55
    SBCB #((($20*8)/4)-($20*2)+$20)                ; @BT C2 20
    JMP ,X                   ; @BT 6E 84
    INC ,X                   ; @BT 6C 84
    STA,X                    ; @BT A7 84
    LDA,S++                  ; @BT A6 E1
    CMPS,X                   ; @BT 11 AC 84
    TSTB                     ; @BT 5D
    INC 5,X                  ; @BT 6C 05
    SUBD ,X                  ; @BT A3 84
    ANDB ,X                  ; @BT E4 84
    CLR $44                  ; @BT 0F 44
    SWI2                     ; @BT 10 3F
    ADCA $10                 ; @BT 99 10
    DECA                     ; @BT 4A
    LDA (,S++)               ; @BT A6 F1
    BITA $17                 ; @BT 95 17
    CMPU #((($0F*8)/4)-($0F*2)+$0F)                ; @BT 11 83 0F
    LDS #((($18*8)/4)-($18*2)+$18)                 ; @BT 10 CE 18
    ADCB ,X                  ; @BT E9 84
    TST $41                  ; @BT 0D 41
    LDA,Y++                  ; @BT A6 A1
    ORA #((($1C*8)/4)-($1C*2)+$1C)                 ; @BT 8A 1C
    LEAX ,X                  ; @BT 30 84
    LDA (A,U)                ; @BT A6 D6
    EORB $1245               ; @BT F8 12 45
    SEX                      ; @BT 1D
    LDA $1234,S              ; @BT A6 E9 12 34
    INCB                     ; @BT 5C
    ANDA $1239               ; @BT B4 12 39
    LDY $28                  ; @BT 10 9E 28
    LDA (A,X)                ; @BT A6 96
    ROLA                     ; @BT 49
    STU $31                  ; @BT DF 31
    LDA (,--Y)               ; @BT A6 B3
    STA $1251                ; @BT B7 12 51
    TST,S                    ; @BT 6D E4
    LDY,X++                  ; @BT 10 AE 81
    LDA D,U                  ; @BT A6 CB
    CMPX ,X                  ; @BT AC 84
    EORB ,X                  ; @BT E8 84
    INCA                     ; @BT 4C
    ASL ,X                   ; @BT 68 84
    STY $1257                ; @BT 10 BF 12 57
    ASL,X                    ; @BT 68 84
    LDA,U+                   ; @BT A6 C0
    LDA 0,X                  ; @BT A6 00
    ANDCC #((($08*8)/4)-($08*2)+$08)               ; @BT 1C 08
    COM,X                    ; @BT 63 84
    STA $2D                  ; @BT 97 2D
    PULS CC,A,B,DP,X,Y,U,PC ; @BT 35 FF
    LDA 15,Y                 ; @BT A6 2F
    ADDD $14                 ; @BT D3 14
    TFR A,B                  ; @BT 1F 89
    PULU A                   ; @BT 37 02
    SBCB $1250               ; @BT F2 12 50
    RTI                      ; @BT 3B
    ORB $124E                ; @BT FA 12 4E
    LDA ($5678,Y)            ; @BT A6 B9 56 78
    CMPD ,X                  ; @BT 10 A3 84
    LDA,-U                   ; @BT A6 C2
    DEC,X                    ; @BT 6A 84
    LDA 0,U                  ; @BT A6 40
    CMPB ,X                  ; @BT E1 84
    LDA $1234,X              ; @BT A6 89 12 34
    ADDB #((($04*8)/4)-($04*2)+$04)                ; @BT CB 04
    ADDB $1237               ; @BT FB 12 37
    LDA,S+                   ; @BT A6 E0
    LDA B,S                  ; @BT A6 E5
    JSR ,X                   ; @BT AD 84
    STB $1252                ; @BT F7 12 52
    ASRB                     ; @BT 57
    ROL $1267                ; @BT 79 12 67
    SUBB $35                 ; @BT D0 35
    LDS,--Y                  ; @BT 10 EE A3
    LEAU,U                   ; @BT 33 C4
    CMPD,X                   ; @BT 10 A3 84
    LBRA T5_LBR1             ; @BT 16 00 40
    LBRN T5_LBR1             ; @BT 10 21 00 3C
    LBHI T5_LBR1             ; @BT 10 22 00 38
    LBLS T5_LBR1             ; @BT 10 23 00 34
    LBCC T5_LBR1             ; @BT 10 24 00 30
    LBHS T5_LBR1             ; @BT 10 24 00 2C
    LBCS T5_LBR1             ; @BT 10 25 00 28
    LBLO T5_LBR1             ; @BT 10 25 00 24
    LBNE T5_LBR1             ; @BT 10 26 00 20
    LBEQ T5_LBR1             ; @BT 10 27 00 1C
    LBVC T5_LBR1             ; @BT 10 28 00 18
    LBVS T5_LBR1             ; @BT 10 29 00 14
    LBPL T5_LBR1             ; @BT 10 2A 00 10
    LBMI T5_LBR1             ; @BT 10 2B 00 0C
    LBGE T5_LBR1             ; @BT 10 2C 00 08
    LBLT T5_LBR1             ; @BT 10 2D 00 04
    LBGT T5_LBR1             ; @BT 10 2E 00 00
T5_LBR1:
    LBLE T5_LBR2             ; @BT 10 2F 00 03
    LBSR T5_LBR2             ; @BT 17 00 00
T5_LBR2:
    LDA ,X                   ; @BT A6 84
    LDA (B,S)                ; @BT A6 F5
    CLR,X                    ; @BT 6F 84
    SUBD $125A               ; @BT B3 12 5A
    LDX $124B                ; @BT BE 12 4B
    LEAU 5,U                 ; @BT 33 45
    LDA ($1234,S)            ; @BT A6 F9 12 34
    ADDA $12                 ; @BT 9B 12
    STD ,X                   ; @BT ED 84
    ASLB                     ; @BT 58
    LDA -16,S                ; @BT A6 70
    ROR,S                    ; @BT 66 E4
    CMPA $19                 ; @BT 91 19
    CMPS $1C                 ; @BT 11 9C 1C
    NEG $37                  ; @BT 00 37
    LDA -1,X                 ; @BT A6 1F
    LDA $22                  ; @BT 96 22
    LDA (B,U)                ; @BT A6 D5
    LDS $25                  ; @BT 10 DE 25
    LDA $1234,U              ; @BT A6 C9 12 34
    EORB $21                 ; @BT D8 21
    LDA $5678,Y              ; @BT A6 A9 56 78
    LEAY 5,Y                 ; @BT 31 25
    SUBA #((($21*8)/4)-($21*2)+$21)                ; @BT 80 21
    LDD $1248                ; @BT FC 12 48
    NOP                      ; @BT 12
    JSR $3456                ; @BT BD 34 56
    LSR ,X                   ; @BT 64 84
    LDA,--Y                  ; @BT A6 A3
    ADDA #((($03*8)/4)-($03*2)+$03)                ; @BT 8B 03
    DEC $3F                  ; @BT 0A 3F
    ADDB ,X                  ; @BT EB 84
    PSHS CC                  ; @BT 34 01
    LDA,-S                   ; @BT A6 E2
    ORCC #((($1E*8)/4)-($1E*2)+$1E)                ; @BT 1A 1E
    LDA $1234,PCR            ; @BT A6 8D 12 34
    LDD ,X                   ; @BT EC 84
    TFR D,X                  ; @BT 1F 01
    SUBA $34                 ; @BT 90 34
    STA ,X                   ; @BT A7 84
    EORA $20                 ; @BT 98 20
    LDB ,X                   ; @BT E6 84
    LDA ,Y                   ; @BT A6 A4
    ADDA ,X                  ; @BT AB 84
    SWI                      ; @BT 3F
    CMPY ,X                  ; @BT 10 AC 84
    ANDB $16                 ; @BT D4 16
    LSRA                     ; @BT 44

    .db "<05"               ; @BT END
