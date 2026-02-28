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
    ADCB #$02                ; @BT C9 02
    ADDA #$03                ; @BT 8B 03
    ADDB #$04                ; @BT CB 04
    ANDA #$06                ; @BT 84 06
    ANDB #$07                ; @BT C4 07
    BITA #$09                ; @BT 85 09
    BITB #$0A                ; @BT C5 0A
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
    ; Indexed: zero-offset (,R) — postbyte only
    ; NOTE: WLA-DX syntax has NO space before comma: "LDA,X" not "LDA ,X"
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

    .db "<01"               ; @BT END

    /////////////////////////////////////////////////////////////////////////////
    // TEST-02: same opcodes as TEST-01, randomised order, branches spread out
    /////////////////////////////////////////////////////////////////////////////

    .db "02>"               ; @BT TEST-02 02 START

    .db "<02"               ; @BT END

    /////////////////////////////////////////////////////////////////////////////
    // TEST-03: same opcodes, randomised order, operands as arithmetic expressions
    /////////////////////////////////////////////////////////////////////////////

    .db "03>"               ; @BT TEST-03 03 START

    .db "<03"               ; @BT END

    /////////////////////////////////////////////////////////////////////////////
    // TEST-04: similar to TEST-03 and then defines + expressions
    /////////////////////////////////////////////////////////////////////////////

    .db "04>"               ; @BT TEST-04 04 START

    .db "<04"               ; @BT END

    /////////////////////////////////////////////////////////////////////////////
    // TEST-05: similar to TEST-04 and defines + functions + expressions
    /////////////////////////////////////////////////////////////////////////////

    .db "05>"               ; @BT TEST-05 05 START

    .db "<05"               ; @BT END
