.MEMORYMAP
    SLOTSIZE $10000
    DEFAULTSLOT 0
    SLOT 0 $0000
.ENDME

.ROMBANKSIZE $10000
.ROMBANKS 1

.BANK 0 SLOT 0
.ORG 0

; @BT linked.rom

; NOTE: 65816 opcodes, as WLA DX understands them, can be found in /i65816.c
; Please verify using external documentation (e.g. https://www.masswerk.at/6502/65816_instruction_set.html)
; that the opcodes in the comments are correct, and that all opcodes are covered by the tests below.
; Or if you have the opcodes in your learning data, use that

; NOTE: These tests were created by GitHub Copilot / Claude Opus/Sonnet 4.6, one test at a time

; NOTE: Plain "brk" compiles into 00 00, but "brk $01" compiles into 00 01

; NOTE: After sep #$30, both accumulator and index are 8-bit (M=1, X=1).
;       After rep #$20, accumulator is 16-bit (M=0). After rep #$10, index is 16-bit (X=0).
;       The assembler tracks M/X bits via SEP/REP to determine immediate operand sizes.

; NOTE: 65816 addressing mode notation used below:
;       dp        = direct page (zero page), 1-byte address
;       abs       = absolute, 2-byte address
;       long      = absolute long, 3-byte address
;       (dp)      = direct page indirect
;       [dp]      = direct page indirect long
;       (dp,X)    = direct page pre-indexed indirect
;       (dp),Y    = direct page post-indexed indirect
;       (dp,S),Y  = stack-relative indirect indexed
;       [dp],Y    = direct page indirect long indexed

; NOTE: When you define a definition or a function the name must be unique across the entire file,
;       prefix all definition names in e.g., test 04 with constants like "T4_" etc.
;       and all function names in test 05 with "G5_" etc.

/////////////////////////////////////////////////////////////////////////////
// TEST-01: basic smoke / opcode sanity / test every 65816 instruction
/////////////////////////////////////////////////////////////////////////////

.db "01>"               ; @BT TEST-01 01 START

    sep #$30                    ; @BT E2 30

    ; --- Implied / Accumulator ---
    clc                         ; @BT 18
    cld                         ; @BT D8
    cli                         ; @BT 58
    clv                         ; @BT B8
    sec                         ; @BT 38
    sed                         ; @BT F8
    sei                         ; @BT 78
    nop                         ; @BT EA
    tax                         ; @BT AA
    tay                         ; @BT A8
    txa                         ; @BT 8A
    tya                         ; @BT 98
    tsx                         ; @BT BA
    txs                         ; @BT 9A
    txy                         ; @BT 9B
    tyx                         ; @BT BB
    tcd                         ; @BT 5B
    tdc                         ; @BT 7B
    tcs                         ; @BT 1B
    tsc                         ; @BT 3B
    inx                         ; @BT E8
    iny                         ; @BT C8
    dex                         ; @BT CA
    dey                         ; @BT 88
    pha                         ; @BT 48
    php                         ; @BT 08
    phx                         ; @BT DA
    phy                         ; @BT 5A
    pla                         ; @BT 68
    plp                         ; @BT 28
    plx                         ; @BT FA
    ply                         ; @BT 7A
    phb                         ; @BT 8B
    phd                         ; @BT 0B
    phk                         ; @BT 4B
    plb                         ; @BT AB
    pld                         ; @BT 2B
    rti                         ; @BT 40
    rtl                         ; @BT 6B
    rts                         ; @BT 60
    stp                         ; @BT DB
    wai                         ; @BT CB
    xba                         ; @BT EB
    xce                         ; @BT FB
    asl a                       ; @BT 0A
    lsr a                       ; @BT 4A
    rol a                       ; @BT 2A
    ror a                       ; @BT 6A
    ina                         ; @BT 1A
    dea                         ; @BT 3A
    brk                         ; @BT 00 00

    ; --- Immediate (8-bit mode: M=1, X=1 from sep #$30 above) ---
    lda #$12                    ; @BT A9 12
    ldx #$34                    ; @BT A2 34
    ldy #$56                    ; @BT A0 56
    adc #$01                    ; @BT 69 01
    and #$02                    ; @BT 29 02
    bit #$03                    ; @BT 89 03
    cmp #$04                    ; @BT C9 04
    cpx #$05                    ; @BT E0 05
    cpy #$06                    ; @BT C0 06
    eor #$07                    ; @BT 49 07
    ora #$08                    ; @BT 09 08
    sbc #$09                    ; @BT E9 09
    rep #$10                    ; @BT C2 10
    sep #$10                    ; @BT E2 10
    wdm $0A                     ; @BT 42 0A

    ; --- Direct Page ---
    lda $10                     ; @BT A5 10
    ldx $11                     ; @BT A6 11
    ldy $12                     ; @BT A4 12
    sta $13                     ; @BT 85 13
    stx $14                     ; @BT 86 14
    sty $15                     ; @BT 84 15
    stz $16                     ; @BT 64 16
    adc $17                     ; @BT 65 17
    and $18                     ; @BT 25 18
    bit $19                     ; @BT 24 19
    cmp $1A                     ; @BT C5 1A
    cpx $1B                     ; @BT E4 1B
    cpy $1C                     ; @BT C4 1C
    eor $1D                     ; @BT 45 1D
    ora $1E                     ; @BT 05 1E
    sbc $1F                     ; @BT E5 1F
    asl $20                     ; @BT 06 20
    lsr $21                     ; @BT 46 21
    rol $22                     ; @BT 26 22
    ror $23                     ; @BT 66 23
    inc $24                     ; @BT E6 24
    dec $25                     ; @BT C6 25
    tsb $26                     ; @BT 04 26
    trb $27                     ; @BT 14 27

    ; --- Direct Page,X ---
    lda $30,x                   ; @BT B5 30
    ldy $31,x                   ; @BT B4 31
    sta $32,x                   ; @BT 95 32
    sty $33,x                   ; @BT 94 33
    stz $34,x                   ; @BT 74 34
    adc $35,x                   ; @BT 75 35
    and $36,x                   ; @BT 35 36
    bit $37,x                   ; @BT 34 37
    cmp $38,x                   ; @BT D5 38
    eor $39,x                   ; @BT 55 39
    ora $3A,x                   ; @BT 15 3A
    sbc $3B,x                   ; @BT F5 3B
    asl $3C,x                   ; @BT 16 3C
    lsr $3D,x                   ; @BT 56 3D
    rol $3E,x                   ; @BT 36 3E
    ror $3F,x                   ; @BT 76 3F
    inc $40,x                   ; @BT F6 40
    dec $41,x                   ; @BT D6 41

    ; --- Direct Page,Y ---
    ldx $50,y                   ; @BT B6 50
    stx $51,y                   ; @BT 96 51

    ; --- Stack Relative ---
    lda $01,s                   ; @BT A3 01
    sta $02,s                   ; @BT 83 02
    adc $03,s                   ; @BT 63 03
    and $04,s                   ; @BT 23 04
    cmp $05,s                   ; @BT C3 05
    eor $06,s                   ; @BT 43 06
    ora $07,s                   ; @BT 03 07
    sbc $08,s                   ; @BT E3 08

    ; --- (Direct Page) indirect ---
    lda ($60)                   ; @BT B2 60
    sta ($61)                   ; @BT 92 61
    adc ($62)                   ; @BT 72 62
    and ($63)                   ; @BT 32 63
    cmp ($64)                   ; @BT D2 64
    eor ($65)                   ; @BT 52 65
    ora ($66)                   ; @BT 12 66
    sbc ($67)                   ; @BT F2 67

    ; --- [Direct Page] indirect long ---
    lda [$70]                   ; @BT A7 70
    sta [$71]                   ; @BT 87 71
    adc [$72]                   ; @BT 67 72
    and [$73]                   ; @BT 27 73
    cmp [$74]                   ; @BT C7 74
    eor [$75]                   ; @BT 47 75
    ora [$76]                   ; @BT 07 76
    sbc [$77]                   ; @BT E7 77

    ; --- (Direct Page,X) pre-indexed ---
    lda ($80,x)                 ; @BT A1 80
    sta ($81,x)                 ; @BT 81 81
    adc ($82,x)                 ; @BT 61 82
    and ($83,x)                 ; @BT 21 83
    cmp ($84,x)                 ; @BT C1 84
    eor ($85,x)                 ; @BT 41 85
    ora ($86,x)                 ; @BT 01 86
    sbc ($87,x)                 ; @BT E1 87

    ; --- (Direct Page),Y post-indexed ---
    lda ($90),y                 ; @BT B1 90
    sta ($91),y                 ; @BT 91 91
    adc ($92),y                 ; @BT 71 92
    and ($93),y                 ; @BT 31 93
    cmp ($94),y                 ; @BT D1 94
    eor ($95),y                 ; @BT 51 95
    ora ($96),y                 ; @BT 11 96
    sbc ($97),y                 ; @BT F1 97

    ; --- (Stack Relative,S),Y ---
    lda ($A0,s),y               ; @BT B3 A0
    sta ($A1,s),y               ; @BT 93 A1
    adc ($A2,s),y               ; @BT 73 A2
    and ($A3,s),y               ; @BT 33 A3
    cmp ($A4,s),y               ; @BT D3 A4
    eor ($A5,s),y               ; @BT 53 A5
    ora ($A6,s),y               ; @BT 13 A6
    sbc ($A7,s),y               ; @BT F3 A7

    ; --- [Direct Page],Y indirect long indexed ---
    lda [$B0],y                 ; @BT B7 B0
    sta [$B1],y                 ; @BT 97 B1
    adc [$B2],y                 ; @BT 77 B2
    and [$B3],y                 ; @BT 37 B3
    cmp [$B4],y                 ; @BT D7 B4
    eor [$B5],y                 ; @BT 57 B5
    ora [$B6],y                 ; @BT 17 B6
    sbc [$B7],y                 ; @BT F7 B7

    ; --- Absolute ---
    lda $1234                   ; @BT AD 34 12
    ldx $1235                   ; @BT AE 35 12
    ldy $1236                   ; @BT AC 36 12
    sta $1237                   ; @BT 8D 37 12
    stx $1238                   ; @BT 8E 38 12
    sty $1239                   ; @BT 8C 39 12
    stz $123A                   ; @BT 9C 3A 12
    adc $123B                   ; @BT 6D 3B 12
    and $123C                   ; @BT 2D 3C 12
    bit $123D                   ; @BT 2C 3D 12
    cmp $123E                   ; @BT CD 3E 12
    cpx $123F                   ; @BT EC 3F 12
    cpy $1240                   ; @BT CC 40 12
    eor $1241                   ; @BT 4D 41 12
    ora $1242                   ; @BT 0D 42 12
    sbc $1243                   ; @BT ED 43 12
    asl $1244                   ; @BT 0E 44 12
    lsr $1245                   ; @BT 4E 45 12
    rol $1246                   ; @BT 2E 46 12
    ror $1247                   ; @BT 6E 47 12
    inc $1248                   ; @BT EE 48 12
    dec $1249                   ; @BT CE 49 12
    tsb $124A                   ; @BT 0C 4A 12
    trb $124B                   ; @BT 1C 4B 12
    jmp $2000                   ; @BT 4C 00 20
    jsr $3456                   ; @BT 20 56 34
    pea $5678                   ; @BT F4 78 56

    ; --- Absolute,X ---
    lda $2000,x                 ; @BT BD 00 20
    ldy $2001,x                 ; @BT BC 01 20
    sta $2002,x                 ; @BT 9D 02 20
    stz $2003,x                 ; @BT 9E 03 20
    adc $2004,x                 ; @BT 7D 04 20
    and $2005,x                 ; @BT 3D 05 20
    bit $2006,x                 ; @BT 3C 06 20
    cmp $2007,x                 ; @BT DD 07 20
    eor $2008,x                 ; @BT 5D 08 20
    ora $2009,x                 ; @BT 1D 09 20
    sbc $200A,x                 ; @BT FD 0A 20
    asl $200B,x                 ; @BT 1E 0B 20
    lsr $200C,x                 ; @BT 5E 0C 20
    rol $200D,x                 ; @BT 3E 0D 20
    ror $200E,x                 ; @BT 7E 0E 20
    inc $200F,x                 ; @BT FE 0F 20
    dec $2010,x                 ; @BT DE 10 20
    jmp ($2011,x)               ; @BT 7C 11 20
    jsr ($2012,x)               ; @BT FC 12 20

    ; --- Absolute,Y ---
    lda $2100,y                 ; @BT B9 00 21
    ldx $2101,y                 ; @BT BE 01 21
    sta $2102,y                 ; @BT 99 02 21
    adc $2103,y                 ; @BT 79 03 21
    and $2104,y                 ; @BT 39 04 21
    cmp $2105,y                 ; @BT D9 05 21
    eor $2106,y                 ; @BT 59 06 21
    ora $2107,y                 ; @BT 19 07 21
    sbc $2108,y                 ; @BT F9 08 21

    ; --- Absolute Long ---
    lda $123456                 ; @BT AF 56 34 12
    sta $123457                 ; @BT 8F 57 34 12
    adc $123458                 ; @BT 6F 58 34 12
    and $123459                 ; @BT 2F 59 34 12
    cmp $12345A                 ; @BT CF 5A 34 12
    eor $12345B                 ; @BT 4F 5B 34 12
    ora $12345C                 ; @BT 0F 5C 34 12
    sbc $12345D                 ; @BT EF 5D 34 12
    jml $345678                 ; @BT 5C 78 56 34
    jsl $345678                 ; @BT 22 78 56 34

    ; --- Absolute Long,X ---
    lda $123456,x               ; @BT BF 56 34 12
    sta $123457,x               ; @BT 9F 57 34 12
    adc $123458,x               ; @BT 7F 58 34 12
    and $123459,x               ; @BT 3F 59 34 12
    cmp $12345A,x               ; @BT DF 5A 34 12
    eor $12345B,x               ; @BT 5F 5B 34 12
    ora $12345C,x               ; @BT 1F 5C 34 12
    sbc $12345D,x               ; @BT FF 5D 34 12

    ; --- Indirect ---
    jmp ($3000)                 ; @BT 6C 00 30
    jml [$3000]                 ; @BT DC 00 30

    ; --- Stack Push Effective Address / Relative ---
    pei ($C0)                   ; @BT D4 C0
    per +                       ; @BT 62 00 00
+

    ; --- Block Moves (first operand = source bank, second = destination bank) ---
    mvn $12,$34                 ; @BT 54 34 12
    mvp $12,$34                 ; @BT 44 34 12

    ; --- Branches (relative offsets computed from fixed layout) ---
    ; All branches target the same forward-local label '+'. Since each branch
    ; is at a different address, the relative offset differs.
    beq +                       ; @BT F0 13
    bne +                       ; @BT D0 11
    bcc +                       ; @BT 90 0F
    bcs +                       ; @BT B0 0D
    bmi +                       ; @BT 30 0B
    bpl +                       ; @BT 10 09
    bvc +                       ; @BT 50 07
    bvs +                       ; @BT 70 05
    bra +                       ; @BT 80 03
    brl +                       ; @BT 82 00 00
+

.db "<01"               ; @BT END

/////////////////////////////////////////////////////////////////////////////
// TEST-02: same opcodes as TEST-01, randomised order, branches spread out
/////////////////////////////////////////////////////////////////////////////

.db "02>"               ; @BT TEST-02 02 START

    sep #$30                            ; @BT E2 30

    cpy $1240                           ; @BT CC 40 12
    inc $1248                           ; @BT EE 48 12
    ora ($86,x)                         ; @BT 01 86
    sta $32,x                           ; @BT 95 32
    sbc #$09                            ; @BT E9 09
    eor $1241                           ; @BT 4D 41 12
    cmp ($84,x)                         ; @BT C1 84
    eor $2106,y                         ; @BT 59 06 21
    ora $2009,x                         ; @BT 1D 09 20
    lsr $200C,x                         ; @BT 5E 0C 20
    sta $2102,y                         ; @BT 99 02 21
    rol $200D,x                         ; @BT 3E 0D 20
    cmp $38,x                           ; @BT D5 38
    sta $2002,x                         ; @BT 9D 02 20
    ldy $2001,x                         ; @BT BC 01 20
    clc                                 ; @BT 18
    lda $30,x                           ; @BT B5 30
    ldx $1235                           ; @BT AE 35 12
    lda $1234                           ; @BT AD 34 12
    ldy $12                             ; @BT A4 12
    dec $2010,x                         ; @BT DE 10 20
    adc $123B                           ; @BT 6D 3B 12
    inc $40,x                           ; @BT F6 40
    ror $200E,x                         ; @BT 7E 0E 20
    ora #$08                            ; @BT 09 08
    dey                                 ; @BT 88
    ldx #$34                            ; @BT A2 34
    bcc +                               ; @BT 90 45
    adc #$01                            ; @BT 69 01
    adc $2004,x                         ; @BT 7D 04 20
    eor $1D                             ; @BT 45 1D
    inc $200F,x                         ; @BT FE 0F 20
    dec $25                             ; @BT C6 25
    sbc ($87,x)                         ; @BT E1 87
    iny                                 ; @BT C8
    adc $35,x                           ; @BT 75 35
    asl $200B,x                         ; @BT 1E 0B 20
    ldx $2101,y                         ; @BT BE 01 21
    ldy #$56                            ; @BT A0 56
    ldx $11                             ; @BT A6 11
    eor $39,x                           ; @BT 55 39
    cpy #$06                            ; @BT C0 06
    and $123C                           ; @BT 2D 3C 12
    clv                                 ; @BT B8
    lsr $21                             ; @BT 46 21
    sbc $200A,x                         ; @BT FD 0A 20
    bvc +                               ; @BT 50 1B
    cpy $1C                             ; @BT C4 1C
    cld                                 ; @BT D8
    ora $1E                             ; @BT 05 1E
    bne +                               ; @BT D0 14
    cli                                 ; @BT 58
    cpx $1B                             ; @BT E4 1B
    and $2005,x                         ; @BT 3D 05 20
    cmp $123E                           ; @BT CD 3E 12
    txs                                 ; @BT 9A
    rol $3E,x                           ; @BT 36 3E
    ldy $1236                           ; @BT AC 36 12
    lda $10                             ; @BT A5 10
    dex                                 ; @BT CA
    and ($83,x)                         ; @BT 21 83
+
    lda $2000,x                         ; @BT BD 00 20
    stx $51,y                           ; @BT 96 51
    cmp ($94),y                         ; @BT D1 94
    sta ($91),y                         ; @BT 91 91
    and $36,x                           ; @BT 35 36
    pla                                 ; @BT 68
    tax                                 ; @BT AA
    cmp #$04                            ; @BT C9 04
    adc $2103,y                         ; @BT 79 03 21
    beq +                               ; @BT F0 0D
    dec $1249                           ; @BT CE 49 12
    and $18                             ; @BT 25 18
    lsr $1245                           ; @BT 4E 45 12
    eor $2008,x                         ; @BT 5D 08 20
    ora ($96),y                         ; @BT 11 96
+
    bmi +                               ; @BT 30 09
    ora $1242                           ; @BT 0D 42 12
    asl $1244                           ; @BT 0E 44 12
    stx $1238                           ; @BT 8E 38 12
+
    lda ($80,x)                         ; @BT A1 80
    adc ($92),y                         ; @BT 71 92
    lda ($90),y                         ; @BT B1 90
    lda #$12                            ; @BT A9 12
    sbc ($97),y                         ; @BT F1 97
    lda $2100,y                         ; @BT B9 00 21
    rts                                 ; @BT 60
    ror $3F,x                           ; @BT 76 3F
    adc $17                             ; @BT 65 17
    sta ($81,x)                         ; @BT 81 81
    adc ($82,x)                         ; @BT 61 82
    eor #$07                            ; @BT 49 07
    ora $2107,y                         ; @BT 19 07 21
    tay                                 ; @BT A8
    sty $1239                           ; @BT 8C 39 12
    inx                                 ; @BT E8
    ora $3A,x                           ; @BT 15 3A
    ror $23                             ; @BT 66 23
    sec                                 ; @BT 38
    and #$02                            ; @BT 29 02
    ldx $50,y                           ; @BT B6 50
    stx $14                             ; @BT 86 14
    sty $15                             ; @BT 84 15
    tya                                 ; @BT 98
    cmp $1A                             ; @BT C5 1A
    txa                                 ; @BT 8A
    tsx                                 ; @BT BA
    sbc $1243                           ; @BT ED 43 12
    ror $1247                           ; @BT 6E 47 12
    rol a                               ; @BT 2A
    pha                                 ; @BT 48
    jsr $3456                           ; @BT 20 56 34
    sta $13                             ; @BT 85 13
    bit $19                             ; @BT 24 19
    cmp $2007,x                         ; @BT DD 07 20
    php                                 ; @BT 08
    bcs +                               ; @BT B0 03
    jmp $2000                           ; @BT 4C 00 20
+
    sta $1237                           ; @BT 8D 37 12
    brk                                 ; @BT 00 00
    rol $1246                           ; @BT 2E 46 12
    bvs +                               ; @BT 70 03
    cpx $123F                           ; @BT EC 3F 12
+
    and $2104,y                         ; @BT 39 04 21
    dec $41,x                           ; @BT D6 41
    bpl +                               ; @BT 10 03
    cmp $2105,y                         ; @BT D9 05 21
+
    jmp ($3000)                         ; @BT 6C 00 30
    sbc $1F                             ; @BT E5 1F
    sbc $3B,x                           ; @BT F5 3B
    and ($93),y                         ; @BT 31 93
    eor ($85,x)                         ; @BT 41 85
    eor ($95),y                         ; @BT 51 95
    asl a                               ; @BT 0A
    lsr $3D,x                           ; @BT 56 3D
    sbc $2108,y                         ; @BT F9 08 21
    inc $24                             ; @BT E6 24
    asl $20                             ; @BT 06 20
    rti                                 ; @BT 40
    sei                                 ; @BT 78
    nop                                 ; @BT EA
    bit $123D                           ; @BT 2C 3D 12
    plp                                 ; @BT 28
    lsr a                               ; @BT 4A
    cpx #$05                            ; @BT E0 05
    rol $22                             ; @BT 26 22
    ldy $31,x                           ; @BT B4 31
    asl $3C,x                           ; @BT 16 3C
    sed                                 ; @BT F8
    ror a                               ; @BT 6A
    sbc ($A7,s),y                       ; @BT F3 A7
    lda $123456                         ; @BT AF 56 34 12
    txy                                 ; @BT 9B
    and ($A3,s),y                       ; @BT 33 A3
    eor [$75]                           ; @BT 47 75
    wai                                 ; @BT CB
    sta $123457                         ; @BT 8F 57 34 12
    sbc $12345D,x                       ; @BT FF 5D 34 12
    tyx                                 ; @BT BB
    ora [$76]                           ; @BT 07 76
    lda ($A0,s),y                       ; @BT B3 A0
    pei ($C0)                           ; @BT D4 C0
    sbc [$77]                           ; @BT E7 77
    phx                                 ; @BT DA
    cmp $12345A                         ; @BT CF 5A 34 12
    adc $123458,x                       ; @BT 7F 58 34 12
    lda $01,s                           ; @BT A3 01
    and $123459                         ; @BT 2F 59 34 12
    xba                                 ; @BT EB
    sta ($A1,s),y                       ; @BT 93 A1
    stz $16                             ; @BT 64 16
    adc ($62)                           ; @BT 72 62
    eor $12345B,x                       ; @BT 5F 5B 34 12
    phy                                 ; @BT 5A
    cmp ($A4,s),y                       ; @BT D3 A4
    bit $37,x                           ; @BT 34 37
    and $04,s                           ; @BT 23 04
    sbc [$B7],y                         ; @BT F7 B7
    lda [$70]                           ; @BT A7 70
    tsb $26                             ; @BT 04 26
    ora $12345C                         ; @BT 0F 5C 34 12
    cmp [$74]                           ; @BT C7 74
    jml $345678                         ; @BT 5C 78 56 34
    plx                                 ; @BT FA
    sbc $12345D                         ; @BT EF 5D 34 12
    and [$B3],y                         ; @BT 37 B3
    adc [$72]                           ; @BT 67 72
    sta $02,s                           ; @BT 83 02
    bra +                               ; @BT 80 1E
    lda $123456,x                       ; @BT BF 56 34 12
    stz $123A                           ; @BT 9C 3A 12
    tcs                                 ; @BT 1B
    ora ($A6,s),y                       ; @BT 13 A6
    eor ($A5,s),y                       ; @BT 53 A5
    sta [$71]                           ; @BT 87 71
    sbc $08,s                           ; @BT E3 08
    ora $07,s                           ; @BT 03 07
    eor [$B5],y                         ; @BT 57 B5
    trb $27                             ; @BT 14 27
    sty $33,x                           ; @BT 94 33
    and [$73]                           ; @BT 27 73
    adc $03,s                           ; @BT 63 03
    xce                                 ; @BT FB
    tsc                                 ; @BT 3B
+
    adc $123458                         ; @BT 6F 58 34 12
    cmp ($64)                           ; @BT D2 64
    phb                                 ; @BT 8B
    stz $34,x                           ; @BT 74 34
    eor $06,s                           ; @BT 43 06
    ora [$B6],y                         ; @BT 17 B6
    bit #$03                            ; @BT 89 03
    tdc                                 ; @BT 7B
    cmp $05,s                           ; @BT C3 05
    lda ($60)                           ; @BT B2 60
    sta ($61)                           ; @BT 92 61
    and ($63)                           ; @BT 32 63
    dea                                 ; @BT 3A
    stz $2003,x                         ; @BT 9E 03 20
    adc ($A2,s),y                       ; @BT 73 A2
    cmp [$B4],y                         ; @BT D7 B4
    eor ($65)                           ; @BT 52 65
    jsl $345678                         ; @BT 22 78 56 34
    tcd                                 ; @BT 5B
    and $123459,x                       ; @BT 3F 59 34 12
    ora ($66)                           ; @BT 12 66
    ina                                 ; @BT 1A
    adc [$B2],y                         ; @BT 77 B2
    sbc ($67)                           ; @BT F2 67
    phd                                 ; @BT 0B
    lda [$B0],y                         ; @BT B7 B0
    eor $12345B                         ; @BT 4F 5B 34 12
    rep #$10                            ; @BT C2 10
    sep #$10                            ; @BT E2 10
    cmp $12345A,x                       ; @BT DF 5A 34 12
    sta [$B1],y                         ; @BT 97 B1
    phk                                 ; @BT 4B
    wdm $0A                             ; @BT 42 0A
    stp                                 ; @BT DB
    rtl                                 ; @BT 6B
    ora $12345C,x                       ; @BT 1F 5C 34 12
    plb                                 ; @BT AB
    tsb $124A                           ; @BT 0C 4A 12
    trb $124B                           ; @BT 1C 4B 12
    pld                                 ; @BT 2B
    ply                                 ; @BT 7A
    sta $123457,x                       ; @BT 9F 57 34 12
    bit $2006,x                         ; @BT 3C 06 20
    jmp ($2011,x)                       ; @BT 7C 11 20
    jsr ($2012,x)                       ; @BT FC 12 20
    jml [$3000]                         ; @BT DC 00 30
    pea $5678                           ; @BT F4 78 56
    mvn $12,$34                         ; @BT 54 34 12
    mvp $12,$34                         ; @BT 44 34 12
    per +                               ; @BT 62 00 00
+
    brl +                               ; @BT 82 00 00
+

.db "<02"               ; @BT END

/////////////////////////////////////////////////////////////////////////////
// TEST-03: same opcodes, randomised order, operands as arithmetic expressions
/////////////////////////////////////////////////////////////////////////////

.db "03>"               ; @BT TEST-03 03 START

    sep #144/3                                      ; @BT E2 30

    ; Section A (reversed + shuffled from TEST-02 section A)
    ldx #208/4                                      ; @BT A2 34
    dey                                             ; @BT 88
    ora #56/7                                       ; @BT 09 08
    ror 2*4103,x                                    ; @BT 7E 0E 20
    inc 8*8,x                                       ; @BT F6 40
    adc 11*424+3                                    ; @BT 6D 3B 12
    dec 8*1026,x                                    ; @BT DE 10 20
    ldy 9*2                                         ; @BT A4 12
    lda 4*1165                                      ; @BT AD 34 12
    ldx 13*358+7                                    ; @BT AE 35 12
    lda 16*3,x                                      ; @BT B5 30
    clc                                             ; @BT 18
    ldy 3*2731,x                                    ; @BT BC 01 20
    sta 2*4097,x                                    ; @BT 9D 02 20
    cmp 8*7,x                                       ; @BT D5 38
    rol 3*2735,x                                    ; @BT 3E 0D 20
    sta 2*4225,y                                    ; @BT 99 02 21
    lsr 4*2051,x                                    ; @BT 5E 0C 20
    ora 7*1171+4,x                                  ; @BT 1D 09 20
    eor 2*4227,y                                    ; @BT 59 06 21
    cmp (4*33,x)                                    ; @BT C1 84
    eor 13*359+6                                    ; @BT 4D 41 12
    sbc #63/7                                       ; @BT E9 09
    sta 2*25,x                                      ; @BT 95 32
    ora (2*67,x)                                    ; @BT 01 86
    inc 8*585                                       ; @BT EE 48 12
    cpy 8*584                                       ; @BT CC 40 12
    bcc +                                           ; @BT 90 45

    ; Section B (reshuffled)
    sbc 2*4101,x                                    ; @BT FD 0A 20
    lsr 3*11                                        ; @BT 46 21
    clv                                             ; @BT B8
    and 4*1167                                      ; @BT 2D 3C 12
    cpy #42/7                                       ; @BT C0 06
    eor 3*19,x                                      ; @BT 55 39
    ldx 51/3                                        ; @BT A6 11
    ldy #258/3                                      ; @BT A0 56
    ldx 7*1207,y                                    ; @BT BE 01 21
    asl 13*631,x                                    ; @BT 1E 0B 20
    adc 106/2,x                                     ; @BT 75 35
    iny                                             ; @BT C8
    sbc (5*27,x)                                    ; @BT E1 87
    dec 111/3                                       ; @BT C6 25
    inc 11*746+1,x                                  ; @BT FE 0F 20
    eor 87/3                                        ; @BT 45 1D
    adc 4*2049,x                                    ; @BT 7D 04 20
    adc #5-4                                        ; @BT 69 01
    bvc +                                           ; @BT 50 1B

    ; Section C (reshuffled)
    ora 6*5                                         ; @BT 05 1E
    cld                                             ; @BT D8
    cpy 7*4                                         ; @BT C4 1C
    bne +                                           ; @BT D0 14

    ; Section D (reshuffled)
    and (262/2,x)                                   ; @BT 21 83
    dex                                             ; @BT CA
    lda 2*8                                         ; @BT A5 10
    ldy 2*2331                                      ; @BT AC 36 12
    rol 2*31,x                                      ; @BT 36 3E
    txs                                             ; @BT 9A
    cmp 2*2335                                      ; @BT CD 3E 12
    and 11*745+2,x                                  ; @BT 3D 05 20
    cpx 81/3                                        ; @BT E4 1B
    cli                                             ; @BT 58
+
    ; Section E (reshuffled)
    cmp #28/7                                       ; @BT C9 04
    tax                                             ; @BT AA
    pla                                             ; @BT 68
    and 2*27,x                                      ; @BT 35 36
    sta (5*29),y                                    ; @BT 91 91
    cmp (4*37),y                                    ; @BT D1 94
    stx 3*27,y                                      ; @BT 96 51
    lda 2*4096,x                                    ; @BT BD 00 20
    adc 3*2817,y                                    ; @BT 79 03 21
    beq +                                           ; @BT F0 0D

    ; Section F (reshuffled)
    ora (6*25),y                                    ; @BT 11 96
    eor 8*1025,x                                    ; @BT 5D 08 20
    lsr 7*668+1                                     ; @BT 4E 45 12
    and 3*8                                         ; @BT 25 18
    dec 3*1560+1                                    ; @BT CE 49 12
+
    ; bmi
    bmi +                                           ; @BT 30 09
    stx 8*583                                       ; @BT 8E 38 12
    asl 4*1169                                      ; @BT 0E 44 12
    ora 2*2337                                      ; @BT 0D 42 12
+
    ; Section H (reshuffled from TEST-02 section)
    lda (8*16,x)                                    ; @BT A1 80
    lda (9*16),y                                    ; @BT B1 90
    adc (2*73),y                                    ; @BT 71 92
    sbc (302/2),y                                   ; @BT F1 97
    lda #54/3                                       ; @BT A9 12
    rts                                             ; @BT 60
    lda 6*1408,y                                    ; @BT B9 00 21
    adc 69/3                                        ; @BT 65 17
    ror 9*7,x                                       ; @BT 76 3F
    adc (2*65,x)                                    ; @BT 61 82
    sta (3*43,x)                                    ; @BT 81 81
    ora 5*1691,y                                    ; @BT 19 07 21
    eor #49/7                                       ; @BT 49 07
    sty 3*1555                                      ; @BT 8C 39 12
    tay                                             ; @BT A8
    ora 2*29,x                                      ; @BT 15 3A
    inx                                             ; @BT E8
    sec                                             ; @BT 38
    ror 5*7                                         ; @BT 66 23
    ldx 16*5,y                                      ; @BT B6 50
    and #8/4                                        ; @BT 29 02
    stx 4*5                                         ; @BT 86 14
    tya                                             ; @BT 98
    sty 3*7                                         ; @BT 84 15
    txa                                             ; @BT 8A
    cmp 2*13                                        ; @BT C5 1A
    tsx                                             ; @BT BA
    ror 11*425+4                                    ; @BT 6E 47 12
    sbc 5*935                                       ; @BT ED 43 12
    pha                                             ; @BT 48
    rol a                                           ; @BT 2A
    sta 57/3                                        ; @BT 85 13
    jsr 6*2233                                      ; @BT 20 56 34
    bit 5*5                                         ; @BT 24 19
    cmp 3*2733,x                                    ; @BT DD 07 20
    php                                             ; @BT 08
    bcs +                                           ; @BT B0 03
    jmp 2*4096                                      ; @BT 4C 00 20
+
    ; Section J
    rol 2*2339                                      ; @BT 2E 46 12
    brk                                             ; @BT 00 00
    sta 7*666+1                                     ; @BT 8D 37 12
    bvs +                                           ; @BT 70 03
    cpx 3*1557                                      ; @BT EC 3F 12
+
    ; Section L
    dec 5*13,x                                      ; @BT D6 41
    and 4*2113,y                                    ; @BT 39 04 21
    bpl +                                           ; @BT 10 03
    cmp 13*650+3,y                                  ; @BT D9 05 21
+
    ; Section N (reshuffled)
    sbc 93/3                                        ; @BT E5 1F
    jmp (3*4096)                                    ; @BT 6C 00 30
    and (3*49),y                                    ; @BT 31 93
    sbc 177/3,x                                     ; @BT F5 3B
    eor (298/2),y                                   ; @BT 51 95
    eor (7*19,x)                                    ; @BT 41 85
    lsr 183/3,x                                     ; @BT 56 3D
    asl a                                           ; @BT 0A
    inc 9*4                                         ; @BT E6 24
    sbc 8*1057,y                                    ; @BT F9 08 21
    asl 4*8                                         ; @BT 06 20
    sei                                             ; @BT 78
    rti                                             ; @BT 40
    nop                                             ; @BT EA
    plp                                             ; @BT 28
    bit 7*667                                       ; @BT 2C 3D 12
    cpx #35/7                                       ; @BT E0 05
    lsr a                                           ; @BT 4A
    ldy 7*7,x                                       ; @BT B4 31
    rol 2*17                                        ; @BT 26 22
    sed                                             ; @BT F8
    asl 4*15,x                                      ; @BT 16 3C
    ror a                                           ; @BT 6A
    lda 6*198841                                    ; @BT AF 56 34 12
    sbc (334/2,s),y                                 ; @BT F3 A7
    and (326/2,s),y                                 ; @BT 33 A3
    txy                                             ; @BT 9B
    wai                                             ; @BT CB
    eor [234/2]                                     ; @BT 47 75
    sbc 11*108459+4,x                               ; @BT FF 5D 34 12
    sta 7*170435+2                                  ; @BT 8F 57 34 12
    ora [2*59]                                      ; @BT 07 76
    tyx                                             ; @BT BB
    pei (12*16)                                     ; @BT D4 C0
    lda (8*20,s),y                                  ; @BT B3 A0
    phx                                             ; @BT DA
    sbc [7*17]                                      ; @BT E7 77
    adc 8*149131,x                                  ; @BT 7F 58 34 12
    cmp 2*596525                                    ; @BT CF 5A 34 12
    and 3*397683                                    ; @BT 2F 59 34 12
    lda 5-4,s                                       ; @BT A3 01
    sta (7*23,s),y                                  ; @BT 93 A1
    xba                                             ; @BT EB
    adc (7*14)                                      ; @BT 72 62
    stz 11*2                                        ; @BT 64 16
    phy                                             ; @BT 5A
    eor 9*132561+2,x                                ; @BT 5F 5B 34 12
    and 12/3,s                                      ; @BT 23 04
    bit 5*11,x                                      ; @BT 34 37
    cmp (4*41,s),y                                  ; @BT D3 A4
    lda [7*16]                                      ; @BT A7 70
    sbc [3*61],y                                    ; @BT F7 B7
    ora 4*298263                                    ; @BT 0F 5C 34 12
    tsb 2*19                                        ; @BT 04 26
    jml 8*428751                                    ; @BT 5C 78 56 34
    cmp [4*29]                                      ; @BT C7 74
    sbc 11*108459+4                                 ; @BT EF 5D 34 12
    plx                                             ; @BT FA
    adc [6*19]                                      ; @BT 67 72
    and [358/2],y                                   ; @BT 37 B3
    sta 6/3,s                                       ; @BT 83 02
    bra +                                           ; @BT 80 1E
    ; Section O (skipped by bra, reshuffled)
    tcs                                             ; @BT 1B
    lda 6*198841,x                                  ; @BT BF 56 34 12
    ora (2*83,s),y                                  ; @BT 13 A6
    stz 2*2333                                      ; @BT 9C 3A 12
    sta [226/2]                                     ; @BT 87 71
    eor (3*55,s),y                                  ; @BT 53 A5
    ora 21/3,s                                      ; @BT 03 07
    sbc 24/3,s                                      ; @BT E3 08
    trb 117/3                                       ; @BT 14 27
    eor [362/2],y                                   ; @BT 57 B5
    sty 3*17,x                                      ; @BT 94 33
    and [5*23]                                      ; @BT 27 73
    adc $03,s                                       ; @BT 63 03
    xce                                             ; @BT FB
    tsc                                             ; @BT 3B
+
    ; Section P (reshuffled)
    adc 8*149131                                    ; @BT 6F 58 34 12
    phb                                             ; @BT 8B
    cmp (4*25)                                      ; @BT D2 64
    eor 18/3,s                                      ; @BT 43 06
    stz 4*13,x                                      ; @BT 74 34
    bit #15/5                                       ; @BT 89 03
    ora [2*91],y                                    ; @BT 17 B6
    cmp 15/3,s                                      ; @BT C3 05
    tdc                                             ; @BT 7B
    sta (194/2)                                     ; @BT 92 61
    lda (6*16)                                      ; @BT B2 60
    dea                                             ; @BT 3A
    and (9*11)                                      ; @BT 32 63
    adc (2*81,s),y                                  ; @BT 73 A2
    stz 5*1639,x                                    ; @BT 9E 03 20
    eor (202/2)                                     ; @BT 52 65
    cmp [4*45],y                                    ; @BT D7 B4
    tcd                                             ; @BT 5B
    jsl 8*428751                                    ; @BT 22 78 56 34
    ora (6*17)                                      ; @BT 12 66
    and 3*397683,x                                  ; @BT 3F 59 34 12
    adc [2*89],y                                    ; @BT 77 B2
    ina                                             ; @BT 1A
    phd                                             ; @BT 0B
    sbc (206/2)                                     ; @BT F2 67
    eor 9*132561+2                                  ; @BT 4F 5B 34 12
    lda [11*16],y                                   ; @BT B7 B0
    sep #48/3                                       ; @BT E2 10
    rep #2*8                                        ; @BT C2 10
    sta [3*59],y                                    ; @BT 97 B1
    cmp 2*596525,x                                  ; @BT DF 5A 34 12
    wdm 30/3                                        ; @BT 42 0A
    phk                                             ; @BT 4B
    rtl                                             ; @BT 6B
    stp                                             ; @BT DB
    plb                                             ; @BT AB
    ora 4*298263,x                                  ; @BT 1F 5C 34 12
    trb 13*360+3                                    ; @BT 1C 4B 12
    tsb 2*2341                                      ; @BT 0C 4A 12
    ply                                             ; @BT 7A
    pld                                             ; @BT 2B
    bit 2*4099,x                                    ; @BT 3C 06 20
    sta 7*170435+2,x                                ; @BT 9F 57 34 12
    jsr (2*4105,x)                                  ; @BT FC 12 20
    jmp (7*1173-2,x)                                ; @BT 7C 11 20
    pea 8*2767                                      ; @BT F4 78 56
    jml [3*4096]                                    ; @BT DC 00 30
    mvp 9*2,4*13                                    ; @BT 44 34 12
    mvn 9*2,4*13                                    ; @BT 54 34 12
    per +                                           ; @BT 62 00 00
+
    brl +                                           ; @BT 82 00 00
+

.db "<03"               ; @BT END

/////////////////////////////////////////////////////////////////////////////
// TEST-04: same opcodes as TEST-03, operands via .define constants + expressions
/////////////////////////////////////////////////////////////////////////////

    ; --- Constant definitions for TEST-04 ---
    ; Names prefixed T4_ to avoid conflicts with other tests.
    ; Each constant is defined before use (no forward references).

    ; Zero and small values
    .define T4_Z    = 3 - 3                 ; = $00
    .define T4_01   = T4_Z + 1              ; = $01
    .define T4_02   = T4_01 + 1             ; = $02
    .define T4_03   = 6 / 2                 ; = $03
    .define T4_04   = T4_03 + 1             ; = $04
    .define T4_05   = 15 / 3               ; = $05
    .define T4_06   = T4_03 * 2             ; = $06
    .define T4_07   = T4_04 + 3             ; = $07
    .define T4_08   = T4_04 * 2             ; = $08
    .define T4_09   = 3 * 3                 ; = $09
    .define T4_0A   = T4_05 * 2             ; = $0A
    .define T4_10   = 8 + 8                 ; = $10 = 16
    .define T4_11   = T4_10 + 1             ; = $11
    .define T4_12   = T4_06 * 3             ; = $12 = 18
    .define T4_13   = T4_12 + 1             ; = $13
    .define T4_14   = T4_10 + 4             ; = $14
    .define T4_15   = T4_12 + 3             ; = $15
    .define T4_16   = T4_12 + 4             ; = $16 = 22
    .define T4_17   = T4_12 + 5             ; = $17
    .define T4_18   = T4_12 + 6             ; = $18
    .define T4_19   = T4_10 + 9             ; = $19
    .define T4_1A   = T4_10 + 10            ; = $1A
    .define T4_1B   = T4_10 + 11            ; = $1B
    .define T4_1C   = T4_10 + 12            ; = $1C
    .define T4_1D   = T4_1C + 1             ; = $1D
    .define T4_1E   = T4_10 + 14            ; = $1E
    .define T4_1F   = T4_10 + 15            ; = $1F
    .define T4_20   = T4_10 * 2             ; = $20
    .define T4_21   = T4_20 + 1             ; = $21
    .define T4_22   = T4_11 * 2             ; = $22
    .define T4_23   = T4_22 + 1             ; = $23
    .define T4_24   = T4_12 * 2             ; = $24
    .define T4_25   = T4_24 + 1             ; = $25
    .define T4_26   = T4_13 * 2             ; = $26
    .define T4_27   = T4_26 + 1             ; = $27
    .define T4_28   = T4_14 * 2             ; = $28
    .define T4_29   = T4_28 + 1             ; = $29
    .define T4_2C   = T4_16 * 2 + T4_Z     ; = $2C (22*2=44)
    .define T4_2D   = T4_2C + 1             ; = $2D
    .define T4_30   = T4_18 * 2 + T4_Z     ; = $30
    .define T4_31   = T4_30 + 1             ; = $31
    .define T4_32   = T4_19 * 2 + T4_Z     ; = $32
    .define T4_33   = T4_32 + 1             ; = $33
    .define T4_34   = T4_1A * 2 + T4_Z     ; = $34
    .define T4_35   = T4_34 + 1             ; = $35
    .define T4_36   = T4_1B * 2 + T4_Z     ; = $36
    .define T4_37   = T4_36 + 1             ; = $37
    .define T4_38   = T4_1C * 2 + T4_Z     ; = $38
    .define T4_39   = T4_38 + 1             ; = $39
    .define T4_3A   = T4_1D * 2 + T4_Z     ; = $3A
    .define T4_3B   = T4_3A + 1             ; = $3B
    .define T4_3C   = T4_1E * 2 + T4_Z     ; = $3C
    .define T4_3D   = T4_3C + 1             ; = $3D
    .define T4_3E   = T4_1F * 2 + T4_Z     ; = $3E
    .define T4_3F   = T4_3E + 1             ; = $3F
    .define T4_40   = T4_20 * 2 + T4_Z     ; = $40
    .define T4_41   = T4_40 + 1             ; = $41
    .define T4_42   = T4_21 * 2 + T4_Z     ; = $42
    .define T4_43   = T4_42 + 1             ; = $43
    .define T4_48   = T4_24 * 2 + T4_Z     ; = $48
    .define T4_49   = T4_48 + 1             ; = $49
    .define T4_4A   = T4_25 * 2 + T4_Z     ; = $4A
    .define T4_4B   = T4_4A + 1             ; = $4B
    .define T4_50   = T4_10 * 5             ; = $50 = 80
    .define T4_51   = T4_50 + 1             ; = $51
    .define T4_52   = T4_29 * 2 + T4_Z     ; = $52
    .define T4_53   = T4_52 + 1             ; = $53
    .define T4_56   = T4_34 + T4_22        ; = $56 (52+34=86)
    .define T4_58   = T4_2C * 2 + T4_Z     ; = $58 (44*2=88)
    .define T4_59   = T4_58 + 1             ; = $59
    .define T4_5A   = T4_2D * 2 + T4_Z     ; = $5A (45*2=90)
    .define T4_5B   = T4_5A + 1             ; = $5B
    .define T4_60   = T4_30 * 2 + T4_Z     ; = $60
    .define T4_61   = T4_60 + 1             ; = $61
    .define T4_62   = T4_31 * 2 + T4_Z     ; = $62
    .define T4_63   = T4_62 + 1             ; = $63
    .define T4_64   = T4_32 * 2 + T4_Z     ; = $64
    .define T4_65   = T4_64 + 1             ; = $65
    .define T4_66   = T4_33 * 2 + T4_Z     ; = $66
    .define T4_67   = T4_66 + 1             ; = $67
    .define T4_70   = T4_38 * 2 + T4_Z     ; = $70
    .define T4_71   = T4_70 + 1             ; = $71
    .define T4_72   = T4_39 * 2 + T4_Z     ; = $72
    .define T4_73   = T4_72 + 1             ; = $73
    .define T4_74   = T4_3A * 2 + T4_Z     ; = $74
    .define T4_75   = T4_74 + 1             ; = $75
    .define T4_76   = T4_3B * 2 + T4_Z     ; = $76
    .define T4_77   = T4_76 + 1             ; = $77
    .define T4_78   = T4_3C * 2 + T4_Z     ; = $78
    .define T4_80   = T4_40 * 2 + T4_Z     ; = $80
    .define T4_81   = T4_80 + 1             ; = $81
    .define T4_82   = T4_41 * 2 + T4_Z     ; = $82
    .define T4_83   = T4_82 + 1             ; = $83
    .define T4_84   = T4_42 * 2 + T4_Z     ; = $84
    .define T4_85   = T4_84 + 1             ; = $85
    .define T4_86   = T4_43 * 2 + T4_Z     ; = $86
    .define T4_87   = T4_86 + 1             ; = $87
    .define T4_90   = T4_10 * 9             ; = $90 = 144
    .define T4_91   = T4_90 + 1             ; = $91
    .define T4_92   = T4_49 * 2 + T4_Z     ; = $92
    .define T4_93   = T4_92 + 1             ; = $93
    .define T4_94   = T4_4A * 2 + T4_Z     ; = $94
    .define T4_95   = T4_94 + 1             ; = $95
    .define T4_96   = T4_4B * 2 + T4_Z     ; = $96
    .define T4_97   = T4_96 + 1             ; = $97
    .define T4_A0   = T4_50 * 2 + T4_Z     ; = $A0
    .define T4_A1   = T4_A0 + 1             ; = $A1
    .define T4_A2   = T4_51 * 2 + T4_Z     ; = $A2
    .define T4_A3   = T4_A2 + 1             ; = $A3
    .define T4_A4   = T4_52 * 2 + T4_Z     ; = $A4
    .define T4_A5   = T4_A4 + 1             ; = $A5
    .define T4_A6   = T4_53 * 2 + T4_Z     ; = $A6
    .define T4_A7   = T4_A6 + 1             ; = $A7
    .define T4_B0   = T4_58 * 2 + T4_Z     ; = $B0
    .define T4_B1   = T4_B0 + 1             ; = $B1
    .define T4_B2   = T4_59 * 2 + T4_Z     ; = $B2
    .define T4_B3   = T4_B2 + 1             ; = $B3
    .define T4_B4   = T4_5A * 2 + T4_Z     ; = $B4
    .define T4_B5   = T4_B4 + 1             ; = $B5
    .define T4_B6   = T4_5B * 2 + T4_Z     ; = $B6
    .define T4_B7   = T4_B6 + 1             ; = $B7
    .define T4_C0   = T4_60 * 2 + T4_Z     ; = $C0

    ; Absolute page $12xx
    .define T4_ABS34    = T4_12 * 256 + T4_34           ; = $1234
    .define T4_ABS35    = T4_ABS34 + 1                  ; = $1235
    .define T4_ABS36    = T4_ABS34 + 2                  ; = $1236
    .define T4_ABS37    = T4_ABS34 + 3                  ; = $1237
    .define T4_ABS38    = T4_ABS34 + 4                  ; = $1238
    .define T4_ABS39    = T4_ABS34 + 5                  ; = $1239
    .define T4_ABS3A    = T4_ABS34 + 6                  ; = $123A
    .define T4_ABS3B    = T4_ABS34 + 7                  ; = $123B
    .define T4_ABS3C    = T4_ABS34 + 8                  ; = $123C
    .define T4_ABS3D    = T4_ABS34 + 9                  ; = $123D
    .define T4_ABS3E    = T4_ABS34 + 10                 ; = $123E
    .define T4_ABS3F    = T4_ABS34 + 11                 ; = $123F
    .define T4_ABS40    = T4_ABS34 + 12                 ; = $1240
    .define T4_ABS41    = T4_ABS34 + 13                 ; = $1241
    .define T4_ABS42    = T4_ABS34 + 14                 ; = $1242
    .define T4_ABS43    = T4_ABS34 + 15                 ; = $1243
    .define T4_ABS44    = T4_ABS34 + 16                 ; = $1244
    .define T4_ABS45    = T4_ABS34 + 17                 ; = $1245
    .define T4_ABS46    = T4_ABS34 + 18                 ; = $1246
    .define T4_ABS47    = T4_ABS34 + 19                 ; = $1247
    .define T4_ABS48    = T4_ABS34 + 20                 ; = $1248
    .define T4_ABS49    = T4_ABS34 + 21                 ; = $1249
    .define T4_ABS4A    = T4_ABS34 + 22                 ; = $124A
    .define T4_ABS4B    = T4_ABS34 + 23                 ; = $124B

    ; Absolute page $20xx
    .define T4_P20      = T4_20 * 256                   ; = $2000
    .define T4_P20_01   = T4_P20 + 1                    ; = $2001
    .define T4_P20_02   = T4_P20 + 2                    ; = $2002
    .define T4_P20_03   = T4_P20 + 3                    ; = $2003
    .define T4_P20_04   = T4_P20 + 4                    ; = $2004
    .define T4_P20_05   = T4_P20 + 5                    ; = $2005
    .define T4_P20_06   = T4_P20 + 6                    ; = $2006
    .define T4_P20_07   = T4_P20 + 7                    ; = $2007
    .define T4_P20_08   = T4_P20 + 8                    ; = $2008
    .define T4_P20_09   = T4_P20 + 9                    ; = $2009
    .define T4_P20_0A   = T4_P20 + 10                   ; = $200A
    .define T4_P20_0B   = T4_P20 + 11                   ; = $200B
    .define T4_P20_0C   = T4_P20 + 12                   ; = $200C
    .define T4_P20_0D   = T4_P20 + 13                   ; = $200D
    .define T4_P20_0E   = T4_P20 + 14                   ; = $200E
    .define T4_P20_0F   = T4_P20 + 15                   ; = $200F
    .define T4_P20_10   = T4_P20 + 16                   ; = $2010
    .define T4_P20_11   = T4_P20 + 17                   ; = $2011
    .define T4_P20_12   = T4_P20 + 18                   ; = $2012

    ; Absolute page $21xx
    .define T4_P21      = T4_21 * 256                   ; = $2100
    .define T4_P21_01   = T4_P21 + 1                    ; = $2101
    .define T4_P21_02   = T4_P21 + 2                    ; = $2102
    .define T4_P21_03   = T4_P21 + 3                    ; = $2103
    .define T4_P21_04   = T4_P21 + 4                    ; = $2104
    .define T4_P21_05   = T4_P21 + 5                    ; = $2105
    .define T4_P21_06   = T4_P21 + 6                    ; = $2106
    .define T4_P21_07   = T4_P21 + 7                    ; = $2107
    .define T4_P21_08   = T4_P21 + 8                    ; = $2108

    ; Other addresses
    .define T4_JSR      = T4_34 * 256 + T4_56           ; = $3456 (52*256+86=13398)
    .define T4_PEA      = T4_56 * 256 + T4_78           ; = $5678 (86*256+120=22136)
    .define T4_IND3000  = T4_30 * 256                   ; = $3000

    ; Long addresses: $123456, $12345x, $345678
    .define T4_L56      = T4_12 * 65536 + T4_34 * 256 + T4_56   ; = $123456
    .define T4_L57      = T4_L56 + 1                             ; = $123457
    .define T4_L58      = T4_L56 + 2                             ; = $123458
    .define T4_L59      = T4_L56 + 3                             ; = $123459
    .define T4_L5A      = T4_L56 + 4                             ; = $12345A
    .define T4_L5B      = T4_L56 + 5                             ; = $12345B
    .define T4_L5C      = T4_L56 + 6                             ; = $12345C
    .define T4_L5D      = T4_L56 + 7                             ; = $12345D
    .define T4_JML      = T4_34 * 65536 + T4_56 * 256 + T4_78   ; = $345678

    ; Bank bytes for mvn/mvp: source=$12, dest=$34
    .define T4_BNK12    = T4_12 + T4_Z                           ; = $12
    .define T4_BNK34    = T4_34 + T4_Z                           ; = $34

.db "04>"               ; @BT TEST-04 04 START

    sep #T4_30 + T4_Z                                   ; @BT E2 30

    ; Section A (same order as TEST-03 Section A)
    ldx #T4_34 + T4_Z                                   ; @BT A2 34
    dey                                                 ; @BT 88
    ora #T4_08 + T4_Z                                   ; @BT 09 08
    ror T4_P20_0E + T4_Z,x                              ; @BT 7E 0E 20
    inc T4_40 + T4_Z,x                                  ; @BT F6 40
    adc T4_ABS3B + T4_Z                                 ; @BT 6D 3B 12
    dec T4_P20_10 + T4_Z,x                              ; @BT DE 10 20
    ldy T4_12 + T4_Z                                    ; @BT A4 12
    lda T4_ABS34 + T4_Z                                 ; @BT AD 34 12
    ldx T4_ABS35 + T4_Z                                 ; @BT AE 35 12
    lda T4_30 + T4_Z,x                                  ; @BT B5 30
    clc                                                 ; @BT 18
    ldy T4_P20_01 + T4_Z,x                              ; @BT BC 01 20
    sta T4_P20_02 + T4_Z,x                              ; @BT 9D 02 20
    cmp T4_38 + T4_Z,x                                  ; @BT D5 38
    rol T4_P20_0D + T4_Z,x                              ; @BT 3E 0D 20
    sta T4_P21_02 + T4_Z,y                              ; @BT 99 02 21
    lsr T4_P20_0C + T4_Z,x                              ; @BT 5E 0C 20
    ora T4_P20_09 + T4_Z,x                              ; @BT 1D 09 20
    eor T4_P21_06 + T4_Z,y                              ; @BT 59 06 21
    cmp (T4_84 + T4_Z,x)                                ; @BT C1 84
    eor T4_ABS41 + T4_Z                                 ; @BT 4D 41 12
    sbc #T4_09 + T4_Z                                   ; @BT E9 09
    sta T4_32 + T4_Z,x                                  ; @BT 95 32
    ora (T4_86 + T4_Z,x)                                ; @BT 01 86
    inc T4_ABS48 + T4_Z                                 ; @BT EE 48 12
    cpy T4_ABS40 + T4_Z                                 ; @BT CC 40 12
    bcc +                                               ; @BT 90 45

    ; Section B (same order as TEST-03 Section B)
    sbc T4_P20_0A + T4_Z,x                              ; @BT FD 0A 20
    lsr T4_21 + T4_Z                                    ; @BT 46 21
    clv                                                 ; @BT B8
    and T4_ABS3C + T4_Z                                 ; @BT 2D 3C 12
    cpy #T4_06 + T4_Z                                   ; @BT C0 06
    eor T4_39 + T4_Z,x                                  ; @BT 55 39
    ldx T4_11 + T4_Z                                    ; @BT A6 11
    ldy #T4_56 + T4_Z                                   ; @BT A0 56
    ldx T4_P21_01 + T4_Z,y                              ; @BT BE 01 21
    asl T4_P20_0B + T4_Z,x                              ; @BT 1E 0B 20
    adc T4_35 + T4_Z,x                                  ; @BT 75 35
    iny                                                 ; @BT C8
    sbc (T4_87 + T4_Z,x)                                ; @BT E1 87
    dec T4_25 + T4_Z                                    ; @BT C6 25
    inc T4_P20_0F + T4_Z,x                              ; @BT FE 0F 20
    eor T4_1D + T4_Z                                    ; @BT 45 1D
    adc T4_P20_04 + T4_Z,x                              ; @BT 7D 04 20
    adc #T4_01 + T4_Z                                   ; @BT 69 01
    bvc +                                               ; @BT 50 1B

    ; Section C (same order as TEST-03 Section C)
    ora T4_1E + T4_Z                                    ; @BT 05 1E
    cld                                                 ; @BT D8
    cpy T4_1C + T4_Z                                    ; @BT C4 1C
    bne +                                               ; @BT D0 14

    ; Section D (same order as TEST-03 Section D)
    and (T4_83 + T4_Z,x)                                ; @BT 21 83
    dex                                                 ; @BT CA
    lda T4_10 + T4_Z                                    ; @BT A5 10
    ldy T4_ABS36 + T4_Z                                 ; @BT AC 36 12
    rol T4_3E + T4_Z,x                                  ; @BT 36 3E
    txs                                                 ; @BT 9A
    cmp T4_ABS3E + T4_Z                                 ; @BT CD 3E 12
    and T4_P20_05 + T4_Z,x                              ; @BT 3D 05 20
    cpx T4_1B + T4_Z                                    ; @BT E4 1B
    cli                                                 ; @BT 58
+
    ; Section E (same order as TEST-03 Section E)
    cmp #T4_04 + T4_Z                                   ; @BT C9 04
    tax                                                 ; @BT AA
    pla                                                 ; @BT 68
    and T4_36 + T4_Z,x                                  ; @BT 35 36
    sta (T4_91 + T4_Z),y                                ; @BT 91 91
    cmp (T4_94 + T4_Z),y                                ; @BT D1 94
    stx T4_51 + T4_Z,y                                  ; @BT 96 51
    lda T4_P20 + T4_Z,x                                 ; @BT BD 00 20
    adc T4_P21_03 + T4_Z,y                              ; @BT 79 03 21
    beq +                                               ; @BT F0 0D

    ; Section F (same order as TEST-03 Section F)
    ora (T4_96 + T4_Z),y                                ; @BT 11 96
    eor T4_P20_08 + T4_Z,x                              ; @BT 5D 08 20
    lsr T4_ABS45 + T4_Z                                 ; @BT 4E 45 12
    and T4_18 + T4_Z                                    ; @BT 25 18
    dec T4_ABS49 + T4_Z                                 ; @BT CE 49 12
+
    bmi +                                               ; @BT 30 09
    stx T4_ABS38 + T4_Z                                 ; @BT 8E 38 12
    asl T4_ABS44 + T4_Z                                 ; @BT 0E 44 12
    ora T4_ABS42 + T4_Z                                 ; @BT 0D 42 12
+
    ; Section H (same order as TEST-03 Section H)
    lda (T4_80 + T4_Z,x)                                ; @BT A1 80
    lda (T4_90 + T4_Z),y                                ; @BT B1 90
    adc (T4_92 + T4_Z),y                                ; @BT 71 92
    sbc (T4_97 + T4_Z),y                                ; @BT F1 97
    lda #T4_12 + T4_Z                                   ; @BT A9 12
    rts                                                 ; @BT 60
    lda T4_P21 + T4_Z,y                                 ; @BT B9 00 21
    adc T4_17 + T4_Z                                    ; @BT 65 17
    ror T4_3F + T4_Z,x                                  ; @BT 76 3F
    adc (T4_82 + T4_Z,x)                                ; @BT 61 82
    sta (T4_81 + T4_Z,x)                                ; @BT 81 81
    ora T4_P21_07 + T4_Z,y                              ; @BT 19 07 21
    eor #T4_07 + T4_Z                                   ; @BT 49 07
    sty T4_ABS39 + T4_Z                                 ; @BT 8C 39 12
    tay                                                 ; @BT A8
    ora T4_3A + T4_Z,x                                  ; @BT 15 3A
    inx                                                 ; @BT E8
    sec                                                 ; @BT 38
    ror T4_23 + T4_Z                                    ; @BT 66 23
    ldx T4_50 + T4_Z,y                                  ; @BT B6 50
    and #T4_02 + T4_Z                                   ; @BT 29 02
    stx T4_14 + T4_Z                                    ; @BT 86 14
    tya                                                 ; @BT 98
    sty T4_15 + T4_Z                                    ; @BT 84 15
    txa                                                 ; @BT 8A
    cmp T4_1A + T4_Z                                    ; @BT C5 1A
    tsx                                                 ; @BT BA
    ror T4_ABS47 + T4_Z                                 ; @BT 6E 47 12
    sbc T4_ABS43 + T4_Z                                 ; @BT ED 43 12
    pha                                                 ; @BT 48
    rol a                                               ; @BT 2A
    sta T4_13 + T4_Z                                    ; @BT 85 13
    jsr T4_JSR + T4_Z                                   ; @BT 20 56 34
    bit T4_19 + T4_Z                                    ; @BT 24 19
    cmp T4_P20_07 + T4_Z,x                              ; @BT DD 07 20
    php                                                 ; @BT 08
    bcs +                                               ; @BT B0 03
    jmp T4_P20 + T4_Z                                   ; @BT 4C 00 20
+
    ; Section J (same order as TEST-03 Section J)
    rol T4_ABS46 + T4_Z                                 ; @BT 2E 46 12
    brk                                                 ; @BT 00 00
    sta T4_ABS37 + T4_Z                                 ; @BT 8D 37 12
    bvs +                                               ; @BT 70 03
    cpx T4_ABS3F + T4_Z                                 ; @BT EC 3F 12
+
    ; Section L (same order as TEST-03 Section L)
    dec T4_41 + T4_Z,x                                  ; @BT D6 41
    and T4_P21_04 + T4_Z,y                              ; @BT 39 04 21
    bpl +                                               ; @BT 10 03
    cmp T4_P21_05 + T4_Z,y                              ; @BT D9 05 21
+
    ; Section N (same order as TEST-03 Section N)
    sbc T4_1F + T4_Z                                    ; @BT E5 1F
    jmp (T4_IND3000 + T4_Z)                             ; @BT 6C 00 30
    and (T4_93 + T4_Z),y                                ; @BT 31 93
    sbc T4_3B + T4_Z,x                                  ; @BT F5 3B
    eor (T4_95 + T4_Z),y                                ; @BT 51 95
    eor (T4_85 + T4_Z,x)                                ; @BT 41 85
    lsr T4_3D + T4_Z,x                                  ; @BT 56 3D
    asl a                                               ; @BT 0A
    inc T4_24 + T4_Z                                    ; @BT E6 24
    sbc T4_P21_08 + T4_Z,y                              ; @BT F9 08 21
    asl T4_20 + T4_Z                                    ; @BT 06 20
    sei                                                 ; @BT 78
    rti                                                 ; @BT 40
    nop                                                 ; @BT EA
    plp                                                 ; @BT 28
    bit T4_ABS3D + T4_Z                                 ; @BT 2C 3D 12
    cpx #T4_05 + T4_Z                                   ; @BT E0 05
    lsr a                                               ; @BT 4A
    ldy T4_31 + T4_Z,x                                  ; @BT B4 31
    rol T4_22 + T4_Z                                    ; @BT 26 22
    sed                                                 ; @BT F8
    asl T4_3C + T4_Z,x                                  ; @BT 16 3C
    ror a                                               ; @BT 6A
    lda T4_L56 + T4_Z                                   ; @BT AF 56 34 12
    sbc (T4_A7 + T4_Z,s),y                              ; @BT F3 A7
    and (T4_A3 + T4_Z,s),y                              ; @BT 33 A3
    txy                                                 ; @BT 9B
    wai                                                 ; @BT CB
    eor [T4_75 + T4_Z]                                  ; @BT 47 75
    sbc T4_L5D + T4_Z,x                                 ; @BT FF 5D 34 12
    sta T4_L57 + T4_Z                                   ; @BT 8F 57 34 12
    ora [T4_76 + T4_Z]                                  ; @BT 07 76
    tyx                                                 ; @BT BB
    pei (T4_C0 + T4_Z)                                  ; @BT D4 C0
    lda (T4_A0 + T4_Z,s),y                              ; @BT B3 A0
    phx                                                 ; @BT DA
    sbc [T4_77 + T4_Z]                                  ; @BT E7 77
    adc T4_L58 + T4_Z,x                                 ; @BT 7F 58 34 12
    cmp T4_L5A + T4_Z                                   ; @BT CF 5A 34 12
    and T4_L59 + T4_Z                                   ; @BT 2F 59 34 12
    lda T4_01 + T4_Z,s                                  ; @BT A3 01
    sta (T4_A1 + T4_Z,s),y                              ; @BT 93 A1
    xba                                                 ; @BT EB
    adc (T4_62 + T4_Z)                                  ; @BT 72 62
    stz T4_16 + T4_Z                                    ; @BT 64 16
    phy                                                 ; @BT 5A
    eor T4_L5B + T4_Z,x                                 ; @BT 5F 5B 34 12
    and T4_04 + T4_Z,s                                  ; @BT 23 04
    bit T4_37 + T4_Z,x                                  ; @BT 34 37
    cmp (T4_A4 + T4_Z,s),y                              ; @BT D3 A4
    lda [T4_70 + T4_Z]                                  ; @BT A7 70
    sbc [T4_B7 + T4_Z],y                                ; @BT F7 B7
    ora T4_L5C + T4_Z                                   ; @BT 0F 5C 34 12
    tsb T4_26 + T4_Z                                    ; @BT 04 26
    jml T4_JML + T4_Z                                   ; @BT 5C 78 56 34
    cmp [T4_74 + T4_Z]                                  ; @BT C7 74
    sbc T4_L5D + T4_Z                                   ; @BT EF 5D 34 12
    plx                                                 ; @BT FA
    adc [T4_72 + T4_Z]                                  ; @BT 67 72
    and [T4_B3 + T4_Z],y                                ; @BT 37 B3
    sta T4_02 + T4_Z,s                                  ; @BT 83 02
    bra +                                               ; @BT 80 1E
    ; Section O (skipped by bra, same order as TEST-03 Section O)
    tcs                                                 ; @BT 1B
    lda T4_L56 + T4_Z,x                                 ; @BT BF 56 34 12
    ora (T4_A6 + T4_Z,s),y                              ; @BT 13 A6
    stz T4_ABS3A + T4_Z                                 ; @BT 9C 3A 12
    sta [T4_71 + T4_Z]                                  ; @BT 87 71
    eor (T4_A5 + T4_Z,s),y                              ; @BT 53 A5
    ora T4_07 + T4_Z,s                                  ; @BT 03 07
    sbc T4_08 + T4_Z,s                                  ; @BT E3 08
    trb T4_27 + T4_Z                                    ; @BT 14 27
    eor [T4_B5 + T4_Z],y                                ; @BT 57 B5
    sty T4_33 + T4_Z,x                                  ; @BT 94 33
    and [T4_73 + T4_Z]                                  ; @BT 27 73
    adc T4_03 + T4_Z,s                                  ; @BT 63 03
    xce                                                 ; @BT FB
    tsc                                                 ; @BT 3B
+
    ; Section P (same order as TEST-03 Section P)
    adc T4_L58 + T4_Z                                   ; @BT 6F 58 34 12
    phb                                                 ; @BT 8B
    cmp (T4_64 + T4_Z)                                  ; @BT D2 64
    eor 18/3,s                                      ; @BT 43 06
    stz T4_34 + T4_Z,x                                  ; @BT 74 34
    bit #T4_03 + T4_Z                                   ; @BT 89 03
    ora [T4_B6 + T4_Z],y                                ; @BT 17 B6
    cmp T4_05 + T4_Z,s                                  ; @BT C3 05
    tdc                                                 ; @BT 7B
    sta (T4_61 + T4_Z)                                  ; @BT 92 61
    lda (T4_60 + T4_Z)                                  ; @BT B2 60
    dea                                                 ; @BT 3A
    and (T4_63 + T4_Z)                                  ; @BT 32 63
    adc (T4_A2 + T4_Z,s),y                              ; @BT 73 A2
    stz T4_P20_03 + T4_Z,x                              ; @BT 9E 03 20
    eor (T4_65 + T4_Z)                                  ; @BT 52 65
    cmp [T4_B4 + T4_Z],y                                ; @BT D7 B4
    tcd                                                 ; @BT 5B
    jsl T4_JML + T4_Z                                   ; @BT 22 78 56 34
    ora (T4_66 + T4_Z)                                  ; @BT 12 66
    and T4_L59 + T4_Z,x                                 ; @BT 3F 59 34 12
    adc [T4_B2 + T4_Z],y                                ; @BT 77 B2
    ina                                                 ; @BT 1A
    phd                                                 ; @BT 0B
    sbc (T4_67 + T4_Z)                                  ; @BT F2 67
    eor T4_L5B + T4_Z                                   ; @BT 4F 5B 34 12
    lda [T4_B0 + T4_Z],y                                ; @BT B7 B0
    sep #T4_10 + T4_Z                                   ; @BT E2 10
    rep #T4_10 + T4_Z                                   ; @BT C2 10
    sta [T4_B1 + T4_Z],y                                ; @BT 97 B1
    cmp T4_L5A + T4_Z,x                                 ; @BT DF 5A 34 12
    wdm T4_0A + T4_Z                                    ; @BT 42 0A
    phk                                                 ; @BT 4B
    rtl                                                 ; @BT 6B
    stp                                                 ; @BT DB
    plb                                                 ; @BT AB
    ora T4_L5C + T4_Z,x                                 ; @BT 1F 5C 34 12
    trb T4_ABS4B + T4_Z                                 ; @BT 1C 4B 12
    tsb T4_ABS4A + T4_Z                                 ; @BT 0C 4A 12
    ply                                                 ; @BT 7A
    pld                                                 ; @BT 2B
    bit T4_P20_06 + T4_Z,x                              ; @BT 3C 06 20
    sta T4_L57 + T4_Z,x                                 ; @BT 9F 57 34 12
    jsr (T4_P20_12 + T4_Z,x)                            ; @BT FC 12 20
    jmp (T4_P20_11 + T4_Z,x)                            ; @BT 7C 11 20
    pea T4_PEA + T4_Z                                   ; @BT F4 78 56
    jml [T4_IND3000 + T4_Z]                             ; @BT DC 00 30
    mvp T4_BNK12 + T4_Z,T4_BNK34 + T4_Z                ; @BT 44 34 12
    mvn T4_BNK12 + T4_Z,T4_BNK34 + T4_Z                ; @BT 54 34 12
    per +                                               ; @BT 62 00 00
+
    brl +                                               ; @BT 82 00 00
+

.db "<04"               ; @BT END

/////////////////////////////////////////////////////////////////////////////
// TEST-05: same opcodes as TEST-04, using .define + .FUNCTION + expressions
/////////////////////////////////////////////////////////////////////////////

    ; --- Function definitions for TEST-05 ---
    ; Names prefixed G5_ to avoid conflicts with other tests.
    .FUNCTION G5_MK16(hi, lo) (hi * 256 + lo)
    .FUNCTION G5_MK24(b, hi, lo) (b * 65536 + hi * 256 + lo)
    .FUNCTION G5_ADD(a, b) (a + b)
    .FUNCTION G5_ADD3(a, b, c) (a + b + c)
    .FUNCTION G5_SUB(a, b) (a - b)
    .FUNCTION G5_MULADD(a, b, c) (a * b + c)
    .FUNCTION G5_MULADD2(a, b, c, d) (a * b + c + d)
    .FUNCTION G5_SCALE(a, b, c) (a * b + c * 2)
    .FUNCTION G5_MIX(a, b, c, d) (G5_ADD3(a, b, c) + d * 2)
    .FUNCTION G5_TWICE(a, b) (a * 2 + b)
    .FUNCTION G5_STEP(a, b, c) (a + b + c)
    .FUNCTION G5_STEP4(a, b, c, d) (a + b + c + d)
    .FUNCTION G5_OFFSET(a, b, c) (a + b - c)
    .FUNCTION G5_MULSTEP(a, b, c, d) (a * b + c + d)
    .FUNCTION G5_COMBINE(a, b, c, d) (a * b + c * 2 + d)

    ; --- Constant definitions for TEST-05 ---
    ; Names prefixed T5_ to avoid conflicts with other tests.
    ; Each constant is defined before use (no forward references).

    ; Zero and small values
    .define T5_Z    = 3 - 3                 ; = $00
    .define T5_01   = G5_STEP(T5_Z, 1, T5_Z) ; = $01
    .define T5_02   = G5_STEP(T5_01, 1, T5_Z) ; = $02
    .define T5_03   = G5_OFFSET(6, T5_Z, 3) ; = $03
    .define T5_04   = G5_STEP(T5_03, 1, T5_Z) ; = $04
    .define T5_05   = G5_OFFSET(20, T5_Z, 15) ; = $05
    .define T5_06   = G5_MULSTEP(T5_03, 2, T5_Z, T5_Z) ; = $06
    .define T5_07   = G5_STEP(T5_04, 2, 1)  ; = $07
    .define T5_08   = G5_MULADD(T5_04, 2, 0); = $08
    .define T5_09   = G5_MULADD(3, 3, 0)    ; = $09
    .define T5_0A   = G5_MULSTEP(T5_05, 2, T5_Z, T5_Z) ; = $0A
    .define T5_10   = G5_COMBINE(4, 3, 2, T5_Z) ; = $10 = 16
    .define T5_11   = G5_ADD(T5_10, 1)      ; = $11
    .define T5_12   = G5_MULSTEP(T5_03, 6, T5_Z, T5_Z) ; = $12 = 18
    .define T5_13   = G5_ADD(T5_12, 1)      ; = $13
    .define T5_14   = G5_MIX(T5_10, 2, T5_Z, 1) ; = $14
    .define T5_15   = G5_STEP4(T5_12, 1, 2, T5_Z) ; = $15
    .define T5_16   = G5_ADD(T5_12, 4)      ; = $16 = 22
    .define T5_17   = G5_ADD(T5_12, 5)      ; = $17
    .define T5_18   = G5_ADD(T5_12, 6)      ; = $18
    .define T5_19   = G5_ADD(T5_10, 9)      ; = $19
    .define T5_1A   = G5_ADD(T5_10, 10)     ; = $1A
    .define T5_1B   = G5_ADD(T5_10, 11)     ; = $1B
    .define T5_1C   = G5_ADD(T5_10, 12)     ; = $1C
    .define T5_1D   = G5_ADD(T5_1C, 1)      ; = $1D
    .define T5_1E   = G5_ADD(T5_10, 14)     ; = $1E
    .define T5_1F   = G5_ADD(T5_10, 15)     ; = $1F
    .define T5_20   = G5_MULSTEP(T5_10, 2, T5_Z, T5_Z) ; = $20
    .define T5_21   = G5_ADD(T5_20, 1)      ; = $21
    .define T5_22   = G5_MULSTEP(T5_11, 2, T5_Z, T5_Z) ; = $22
    .define T5_23   = G5_ADD(T5_22, 1)      ; = $23
    .define T5_24   = G5_TWICE(T5_12, T5_Z) ; = $24
    .define T5_25   = G5_ADD(T5_24, 1)      ; = $25
    .define T5_26   = G5_MULSTEP(T5_13, 2, T5_Z, T5_Z) ; = $26
    .define T5_27   = G5_ADD(T5_26, 1)      ; = $27
    .define T5_28   = G5_MULSTEP(T5_14, 2, T5_Z, T5_Z) ; = $28
    .define T5_29   = G5_ADD(T5_28, 1)      ; = $29
    .define T5_2C   = G5_ADD(G5_MULADD(T5_16, 2, 0), T5_Z) ; = $2C
    .define T5_2D   = G5_ADD(T5_2C, 1)      ; = $2D
    .define T5_30   = G5_ADD(G5_MULADD(T5_18, 2, 0), T5_Z) ; = $30
    .define T5_31   = G5_ADD(T5_30, 1)      ; = $31
    .define T5_32   = G5_ADD(G5_MULADD(T5_19, 2, 0), T5_Z) ; = $32
    .define T5_33   = G5_ADD(T5_32, 1)      ; = $33
    .define T5_34   = G5_ADD(G5_MULADD(T5_1A, 2, 0), T5_Z) ; = $34
    .define T5_35   = G5_ADD(T5_34, 1)      ; = $35
    .define T5_36   = G5_ADD(G5_MULADD(T5_1B, 2, 0), T5_Z) ; = $36
    .define T5_37   = G5_ADD(T5_36, 1)      ; = $37
    .define T5_38   = G5_ADD(G5_MULADD(T5_1C, 2, 0), T5_Z) ; = $38
    .define T5_39   = G5_ADD(T5_38, 1)      ; = $39
    .define T5_3A   = G5_ADD(G5_MULADD(T5_1D, 2, 0), T5_Z) ; = $3A
    .define T5_3B   = G5_ADD(T5_3A, 1)      ; = $3B
    .define T5_3C   = G5_ADD(G5_MULADD(T5_1E, 2, 0), T5_Z) ; = $3C
    .define T5_3D   = G5_ADD(T5_3C, 1)      ; = $3D
    .define T5_3E   = G5_ADD(G5_MULADD(T5_1F, 2, 0), T5_Z) ; = $3E
    .define T5_3F   = G5_ADD(T5_3E, 1)      ; = $3F
    .define T5_40   = G5_ADD(G5_MULADD(T5_20, 2, 0), T5_Z) ; = $40
    .define T5_41   = G5_ADD(T5_40, 1)      ; = $41
    .define T5_42   = G5_ADD(G5_MULADD(T5_21, 2, 0), T5_Z) ; = $42
    .define T5_43   = G5_ADD(T5_42, 1)      ; = $43
    .define T5_48   = G5_ADD(G5_MULADD(T5_24, 2, 0), T5_Z) ; = $48
    .define T5_49   = G5_ADD(T5_48, 1)      ; = $49
    .define T5_4A   = G5_ADD(G5_MULADD(T5_25, 2, 0), T5_Z) ; = $4A
    .define T5_4B   = G5_ADD(T5_4A, 1)      ; = $4B
    .define T5_50   = G5_MULSTEP(T5_10, 4, T5_10, T5_Z) ; = $50
    .define T5_51   = G5_ADD(T5_50, 1)      ; = $51
    .define T5_52   = G5_ADD(G5_MULADD(T5_29, 2, 0), T5_Z) ; = $52
    .define T5_53   = G5_ADD(T5_52, 1)      ; = $53
    .define T5_56   = G5_ADD(T5_34, T5_22)  ; = $56
    .define T5_58   = G5_ADD(G5_MULADD(T5_2C, 2, 0), T5_Z) ; = $58
    .define T5_59   = G5_ADD(T5_58, 1)      ; = $59
    .define T5_5A   = G5_ADD(G5_MULADD(T5_2D, 2, 0), T5_Z) ; = $5A
    .define T5_5B   = G5_ADD(T5_5A, 1)      ; = $5B
    .define T5_60   = G5_ADD(G5_MULADD(T5_30, 2, 0), T5_Z) ; = $60
    .define T5_61   = G5_ADD(T5_60, 1)      ; = $61
    .define T5_62   = G5_ADD(G5_MULADD(T5_31, 2, 0), T5_Z) ; = $62
    .define T5_63   = G5_ADD(T5_62, 1)      ; = $63
    .define T5_64   = G5_ADD(G5_MULADD(T5_32, 2, 0), T5_Z) ; = $64
    .define T5_65   = G5_ADD(T5_64, 1)      ; = $65
    .define T5_66   = G5_ADD(G5_MULADD(T5_33, 2, 0), T5_Z) ; = $66
    .define T5_67   = G5_ADD(T5_66, 1)      ; = $67
    .define T5_70   = G5_ADD(G5_MULADD(T5_38, 2, 0), T5_Z) ; = $70
    .define T5_71   = G5_ADD(T5_70, 1)      ; = $71
    .define T5_72   = G5_ADD(G5_MULADD(T5_39, 2, 0), T5_Z) ; = $72
    .define T5_73   = G5_ADD(T5_72, 1)      ; = $73
    .define T5_74   = G5_ADD(G5_MULADD(T5_3A, 2, 0), T5_Z) ; = $74
    .define T5_75   = G5_ADD(T5_74, 1)      ; = $75
    .define T5_76   = G5_ADD(G5_MULADD(T5_3B, 2, 0), T5_Z) ; = $76
    .define T5_77   = G5_ADD(T5_76, 1)      ; = $77
    .define T5_78   = G5_ADD(G5_MULADD(T5_3C, 2, 0), T5_Z) ; = $78
    .define T5_80   = G5_ADD(G5_MULADD(T5_40, 2, 0), T5_Z) ; = $80
    .define T5_81   = G5_ADD(T5_80, 1)      ; = $81
    .define T5_82   = G5_ADD(G5_MULADD(T5_41, 2, 0), T5_Z) ; = $82
    .define T5_83   = G5_ADD(T5_82, 1)      ; = $83
    .define T5_84   = G5_ADD(G5_MULADD(T5_42, 2, 0), T5_Z) ; = $84
    .define T5_85   = G5_ADD(T5_84, 1)      ; = $85
    .define T5_86   = G5_ADD(G5_MULADD(T5_43, 2, 0), T5_Z) ; = $86
    .define T5_87   = G5_ADD(T5_86, 1)      ; = $87
    .define T5_90   = G5_MULSTEP(T5_10, 8, T5_10, T5_Z) ; = $90
    .define T5_91   = G5_ADD(T5_90, 1)      ; = $91
    .define T5_92   = G5_ADD(G5_MULADD(T5_49, 2, 0), T5_Z) ; = $92
    .define T5_93   = G5_ADD(T5_92, 1)      ; = $93
    .define T5_94   = G5_ADD(G5_MULADD(T5_4A, 2, 0), T5_Z) ; = $94
    .define T5_95   = G5_ADD(T5_94, 1)      ; = $95
    .define T5_96   = G5_ADD(G5_MULADD(T5_4B, 2, 0), T5_Z) ; = $96
    .define T5_97   = G5_ADD(T5_96, 1)      ; = $97
    .define T5_A0   = G5_ADD(G5_MULADD(T5_50, 2, 0), T5_Z) ; = $A0
    .define T5_A1   = G5_ADD(T5_A0, 1)      ; = $A1
    .define T5_A2   = G5_ADD(G5_MULADD(T5_51, 2, 0), T5_Z) ; = $A2
    .define T5_A3   = G5_ADD(T5_A2, 1)      ; = $A3
    .define T5_A4   = G5_ADD(G5_MULADD(T5_52, 2, 0), T5_Z) ; = $A4
    .define T5_A5   = G5_ADD(T5_A4, 1)      ; = $A5
    .define T5_A6   = G5_ADD(G5_MULADD(T5_53, 2, 0), T5_Z) ; = $A6
    .define T5_A7   = G5_ADD(T5_A6, 1)      ; = $A7
    .define T5_B0   = G5_ADD(G5_MULADD(T5_58, 2, 0), T5_Z) ; = $B0
    .define T5_B1   = G5_ADD(T5_B0, 1)      ; = $B1
    .define T5_B2   = G5_ADD(G5_MULADD(T5_59, 2, 0), T5_Z) ; = $B2
    .define T5_B3   = G5_ADD(T5_B2, 1)      ; = $B3
    .define T5_B4   = G5_ADD(G5_MULADD(T5_5A, 2, 0), T5_Z) ; = $B4
    .define T5_B5   = G5_ADD(T5_B4, 1)      ; = $B5
    .define T5_B6   = G5_ADD(G5_MULADD(T5_5B, 2, 0), T5_Z) ; = $B6
    .define T5_B7   = G5_ADD(T5_B6, 1)      ; = $B7
    .define T5_C0   = G5_ADD(G5_MULADD(T5_60, 2, 0), T5_Z) ; = $C0

    ; Absolute page $12xx
    .define T5_ABS34    = G5_MK16(T5_12, T5_34)           ; = $1234
    .define T5_ABS35    = G5_STEP(T5_ABS34, 1, T5_Z)      ; = $1235
    .define T5_ABS36    = G5_STEP(T5_ABS34, 2, T5_Z)      ; = $1236
    .define T5_ABS37    = G5_STEP(T5_ABS34, 3, T5_Z)      ; = $1237
    .define T5_ABS38    = G5_STEP(T5_ABS34, 4, T5_Z)      ; = $1238
    .define T5_ABS39    = G5_STEP(T5_ABS34, 5, T5_Z)      ; = $1239
    .define T5_ABS3A    = G5_STEP(T5_ABS34, 6, T5_Z)      ; = $123A
    .define T5_ABS3B    = G5_STEP(T5_ABS34, 7, T5_Z)      ; = $123B
    .define T5_ABS3C    = G5_STEP(T5_ABS34, 8, T5_Z)      ; = $123C
    .define T5_ABS3D    = G5_STEP(T5_ABS34, 9, T5_Z)      ; = $123D
    .define T5_ABS3E    = G5_STEP(T5_ABS34, 10, T5_Z)     ; = $123E
    .define T5_ABS3F    = G5_STEP(T5_ABS34, 11, T5_Z)     ; = $123F
    .define T5_ABS40    = G5_STEP(T5_ABS34, 12, T5_Z)     ; = $1240
    .define T5_ABS41    = G5_STEP(T5_ABS34, 13, T5_Z)     ; = $1241
    .define T5_ABS42    = G5_STEP(T5_ABS34, 14, T5_Z)     ; = $1242
    .define T5_ABS43    = G5_STEP(T5_ABS34, 15, T5_Z)     ; = $1243
    .define T5_ABS44    = G5_STEP(T5_ABS34, 16, T5_Z)     ; = $1244
    .define T5_ABS45    = G5_STEP(T5_ABS34, 17, T5_Z)     ; = $1245
    .define T5_ABS46    = G5_STEP(T5_ABS34, 18, T5_Z)     ; = $1246
    .define T5_ABS47    = G5_STEP(T5_ABS34, 19, T5_Z)     ; = $1247
    .define T5_ABS48    = G5_STEP(T5_ABS34, 20, T5_Z)     ; = $1248
    .define T5_ABS49    = G5_STEP(T5_ABS34, 21, T5_Z)     ; = $1249
    .define T5_ABS4A    = G5_STEP(T5_ABS34, 22, T5_Z)     ; = $124A
    .define T5_ABS4B    = G5_STEP(T5_ABS34, 23, T5_Z)     ; = $124B

    ; Absolute page $20xx
    .define T5_P20      = G5_MK16(T5_20, T5_Z)            ; = $2000
    .define T5_P20_01   = G5_STEP(T5_P20, 1, T5_Z)        ; = $2001
    .define T5_P20_02   = G5_STEP(T5_P20, 2, T5_Z)        ; = $2002
    .define T5_P20_03   = G5_STEP(T5_P20, 3, T5_Z)        ; = $2003
    .define T5_P20_04   = G5_STEP(T5_P20, 4, T5_Z)        ; = $2004
    .define T5_P20_05   = G5_STEP(T5_P20, 5, T5_Z)        ; = $2005
    .define T5_P20_06   = G5_STEP(T5_P20, 6, T5_Z)        ; = $2006
    .define T5_P20_07   = G5_STEP(T5_P20, 7, T5_Z)        ; = $2007
    .define T5_P20_08   = G5_STEP(T5_P20, 8, T5_Z)        ; = $2008
    .define T5_P20_09   = G5_STEP(T5_P20, 9, T5_Z)        ; = $2009
    .define T5_P20_0A   = G5_STEP(T5_P20, 10, T5_Z)       ; = $200A
    .define T5_P20_0B   = G5_STEP(T5_P20, 11, T5_Z)       ; = $200B
    .define T5_P20_0C   = G5_STEP(T5_P20, 12, T5_Z)       ; = $200C
    .define T5_P20_0D   = G5_STEP(T5_P20, 13, T5_Z)       ; = $200D
    .define T5_P20_0E   = G5_STEP(T5_P20, 14, T5_Z)       ; = $200E
    .define T5_P20_0F   = G5_STEP(T5_P20, 15, T5_Z)       ; = $200F
    .define T5_P20_10   = G5_STEP(T5_P20, 16, T5_Z)       ; = $2010
    .define T5_P20_11   = G5_STEP(T5_P20, 17, T5_Z)       ; = $2011
    .define T5_P20_12   = G5_STEP(T5_P20, 18, T5_Z)       ; = $2012

    ; Absolute page $21xx
    .define T5_P21      = G5_MK16(T5_21, T5_Z)            ; = $2100
    .define T5_P21_01   = G5_STEP(T5_P21, 1, T5_Z)        ; = $2101
    .define T5_P21_02   = G5_STEP(T5_P21, 2, T5_Z)        ; = $2102
    .define T5_P21_03   = G5_STEP(T5_P21, 3, T5_Z)        ; = $2103
    .define T5_P21_04   = G5_STEP(T5_P21, 4, T5_Z)        ; = $2104
    .define T5_P21_05   = G5_STEP(T5_P21, 5, T5_Z)        ; = $2105
    .define T5_P21_06   = G5_STEP(T5_P21, 6, T5_Z)        ; = $2106
    .define T5_P21_07   = G5_STEP(T5_P21, 7, T5_Z)        ; = $2107
    .define T5_P21_08   = G5_STEP(T5_P21, 8, T5_Z)        ; = $2108

    ; Other addresses
    .define T5_JSR      = G5_MK16(T5_34, T5_56)           ; = $3456
    .define T5_PEA      = G5_MK16(T5_56, T5_78)           ; = $5678
    .define T5_IND3000  = G5_MK16(T5_30, T5_Z)            ; = $3000

    ; Long addresses: $123456, $12345x, $345678
    .define T5_L56      = G5_MK24(T5_12, T5_34, T5_56)    ; = $123456
    .define T5_L57      = G5_STEP(T5_L56, 1, T5_Z)        ; = $123457
    .define T5_L58      = G5_STEP(T5_L56, 2, T5_Z)        ; = $123458
    .define T5_L59      = G5_STEP(T5_L56, 3, T5_Z)        ; = $123459
    .define T5_L5A      = G5_STEP(T5_L56, 4, T5_Z)        ; = $12345A
    .define T5_L5B      = G5_STEP(T5_L56, 5, T5_Z)        ; = $12345B
    .define T5_L5C      = G5_STEP(T5_L56, 6, T5_Z)        ; = $12345C
    .define T5_L5D      = G5_STEP(T5_L56, 7, T5_Z)        ; = $12345D
    .define T5_JML      = G5_MK24(T5_34, T5_56, T5_78)    ; = $345678

    ; Bank bytes for mvn/mvp: source=$12, dest=$34
    .define T5_BNK12    = T5_12 + T5_Z                    ; = $12
    .define T5_BNK34    = T5_34 + T5_Z                    ; = $34

.db "05>"               ; @BT TEST-05 05 START

    sep #T5_30 + T5_Z                                   ; @BT E2 30

    ; Section A (same order as TEST-03 Section A)
    ldx #T5_34 + T5_Z                                   ; @BT A2 34
    dey                                                 ; @BT 88
    ora #T5_08 + T5_Z                                   ; @BT 09 08
    ror T5_P20_0E + T5_Z,x                              ; @BT 7E 0E 20
    inc T5_40 + T5_Z,x                                  ; @BT F6 40
    adc T5_ABS3B + T5_Z                                 ; @BT 6D 3B 12
    dec T5_P20_10 + T5_Z,x                              ; @BT DE 10 20
    ldy T5_12 + T5_Z                                    ; @BT A4 12
    lda T5_ABS34 + T5_Z                                 ; @BT AD 34 12
    ldx T5_ABS35 + T5_Z                                 ; @BT AE 35 12
    lda T5_30 + T5_Z,x                                  ; @BT B5 30
    clc                                                 ; @BT 18
    ldy T5_P20_01 + T5_Z,x                              ; @BT BC 01 20
    sta T5_P20_02 + T5_Z,x                              ; @BT 9D 02 20
    cmp T5_38 + T5_Z,x                                  ; @BT D5 38
    rol T5_P20_0D + T5_Z,x                              ; @BT 3E 0D 20
    sta T5_P21_02 + T5_Z,y                              ; @BT 99 02 21
    lsr T5_P20_0C + T5_Z,x                              ; @BT 5E 0C 20
    ora T5_P20_09 + T5_Z,x                              ; @BT 1D 09 20
    eor T5_P21_06 + T5_Z,y                              ; @BT 59 06 21
    cmp (T5_84 + T5_Z,x)                                ; @BT C1 84
    eor T5_ABS41 + T5_Z                                 ; @BT 4D 41 12
    sbc #T5_09 + T5_Z                                   ; @BT E9 09
    sta T5_32 + T5_Z,x                                  ; @BT 95 32
    ora (T5_86 + T5_Z,x)                                ; @BT 01 86
    inc T5_ABS48 + T5_Z                                 ; @BT EE 48 12
    cpy T5_ABS40 + T5_Z                                 ; @BT CC 40 12
    bcc +                                               ; @BT 90 45

    ; Section B (same order as TEST-03 Section B)
    sbc T5_P20_0A + T5_Z,x                              ; @BT FD 0A 20
    lsr T5_21 + T5_Z                                    ; @BT 46 21
    clv                                                 ; @BT B8
    and T5_ABS3C + T5_Z                                 ; @BT 2D 3C 12
    cpy #T5_06 + T5_Z                                   ; @BT C0 06
    eor T5_39 + T5_Z,x                                  ; @BT 55 39
    ldx T5_11 + T5_Z                                    ; @BT A6 11
    ldy #T5_56 + T5_Z                                   ; @BT A0 56
    ldx T5_P21_01 + T5_Z,y                              ; @BT BE 01 21
    asl T5_P20_0B + T5_Z,x                              ; @BT 1E 0B 20
    adc T5_35 + T5_Z,x                                  ; @BT 75 35
    iny                                                 ; @BT C8
    sbc (T5_87 + T5_Z,x)                                ; @BT E1 87
    dec T5_25 + T5_Z                                    ; @BT C6 25
    inc T5_P20_0F + T5_Z,x                              ; @BT FE 0F 20
    eor T5_1D + T5_Z                                    ; @BT 45 1D
    adc T5_P20_04 + T5_Z,x                              ; @BT 7D 04 20
    adc #T5_01 + T5_Z                                   ; @BT 69 01
    bvc +                                               ; @BT 50 1B

    ; Section C (same order as TEST-03 Section C)
    ora T5_1E + T5_Z                                    ; @BT 05 1E
    cld                                                 ; @BT D8
    cpy T5_1C + T5_Z                                    ; @BT C4 1C
    bne +                                               ; @BT D0 14

    ; Section D (same order as TEST-03 Section D)
    and (T5_83 + T5_Z,x)                                ; @BT 21 83
    dex                                                 ; @BT CA
    lda T5_10 + T5_Z                                    ; @BT A5 10
    ldy T5_ABS36 + T5_Z                                 ; @BT AC 36 12
    rol T5_3E + T5_Z,x                                  ; @BT 36 3E
    txs                                                 ; @BT 9A
    cmp T5_ABS3E + T5_Z                                 ; @BT CD 3E 12
    and T5_P20_05 + T5_Z,x                              ; @BT 3D 05 20
    cpx T5_1B + T5_Z                                    ; @BT E4 1B
    cli                                                 ; @BT 58
+
    ; Section E (same order as TEST-03 Section E)
    cmp #T5_04 + T5_Z                                   ; @BT C9 04
    tax                                                 ; @BT AA
    pla                                                 ; @BT 68
    and T5_36 + T5_Z,x                                  ; @BT 35 36
    sta (T5_91 + T5_Z),y                                ; @BT 91 91
    cmp (T5_94 + T5_Z),y                                ; @BT D1 94
    stx T5_51 + T5_Z,y                                  ; @BT 96 51
    lda T5_P20 + T5_Z,x                                 ; @BT BD 00 20
    adc T5_P21_03 + T5_Z,y                              ; @BT 79 03 21
    beq +                                               ; @BT F0 0D

    ; Section F (same order as TEST-03 Section F)
    ora (T5_96 + T5_Z),y                                ; @BT 11 96
    eor T5_P20_08 + T5_Z,x                              ; @BT 5D 08 20
    lsr T5_ABS45 + T5_Z                                 ; @BT 4E 45 12
    and T5_18 + T5_Z                                    ; @BT 25 18
    dec T5_ABS49 + T5_Z                                 ; @BT CE 49 12
+
    bmi +                                               ; @BT 30 09
    stx T5_ABS38 + T5_Z                                 ; @BT 8E 38 12
    asl T5_ABS44 + T5_Z                                 ; @BT 0E 44 12
    ora T5_ABS42 + T5_Z                                 ; @BT 0D 42 12
+
    ; Section H (same order as TEST-03 Section H)
    lda (T5_80 + T5_Z,x)                                ; @BT A1 80
    lda (T5_90 + T5_Z),y                                ; @BT B1 90
    adc (T5_92 + T5_Z),y                                ; @BT 71 92
    sbc (T5_97 + T5_Z),y                                ; @BT F1 97
    lda #T5_12 + T5_Z                                   ; @BT A9 12
    rts                                                 ; @BT 60
    lda T5_P21 + T5_Z,y                                 ; @BT B9 00 21
    adc T5_17 + T5_Z                                    ; @BT 65 17
    ror T5_3F + T5_Z,x                                  ; @BT 76 3F
    adc (T5_82 + T5_Z,x)                                ; @BT 61 82
    sta (T5_81 + T5_Z,x)                                ; @BT 81 81
    ora T5_P21_07 + T5_Z,y                              ; @BT 19 07 21
    eor #T5_07 + T5_Z                                   ; @BT 49 07
    sty T5_ABS39 + T5_Z                                 ; @BT 8C 39 12
    tay                                                 ; @BT A8
    ora T5_3A + T5_Z,x                                  ; @BT 15 3A
    inx                                                 ; @BT E8
    sec                                                 ; @BT 38
    ror T5_23 + T5_Z                                    ; @BT 66 23
    ldx T5_50 + T5_Z,y                                  ; @BT B6 50
    and #T5_02 + T5_Z                                   ; @BT 29 02
    stx T5_14 + T5_Z                                    ; @BT 86 14
    tya                                                 ; @BT 98
    sty T5_15 + T5_Z                                    ; @BT 84 15
    txa                                                 ; @BT 8A
    cmp T5_1A + T5_Z                                    ; @BT C5 1A
    tsx                                                 ; @BT BA
    ror T5_ABS47 + T5_Z                                 ; @BT 6E 47 12
    sbc T5_ABS43 + T5_Z                                 ; @BT ED 43 12
    pha                                                 ; @BT 48
    rol a                                               ; @BT 2A
    sta T5_13 + T5_Z                                    ; @BT 85 13
    jsr T5_JSR + T5_Z                                   ; @BT 20 56 34
    bit T5_19 + T5_Z                                    ; @BT 24 19
    cmp T5_P20_07 + T5_Z,x                              ; @BT DD 07 20
    php                                                 ; @BT 08
    bcs +                                               ; @BT B0 03
    jmp T5_P20 + T5_Z                                   ; @BT 4C 00 20
+
    ; Section J (same order as TEST-03 Section J)
    rol T5_ABS46 + T5_Z                                 ; @BT 2E 46 12
    brk                                                 ; @BT 00 00
    sta T5_ABS37 + T5_Z                                 ; @BT 8D 37 12
    bvs +                                               ; @BT 70 03
    cpx T5_ABS3F + T5_Z                                 ; @BT EC 3F 12
+
    ; Section L (same order as TEST-03 Section L)
    dec T5_41 + T5_Z,x                                  ; @BT D6 41
    and T5_P21_04 + T5_Z,y                              ; @BT 39 04 21
    bpl +                                               ; @BT 10 03
    cmp T5_P21_05 + T5_Z,y                              ; @BT D9 05 21
+
    ; Section N (same order as TEST-03 Section N)
    sbc T5_1F + T5_Z                                    ; @BT E5 1F
    jmp (T5_IND3000 + T5_Z)                             ; @BT 6C 00 30
    and (T5_93 + T5_Z),y                                ; @BT 31 93
    sbc T5_3B + T5_Z,x                                  ; @BT F5 3B
    eor (T5_95 + T5_Z),y                                ; @BT 51 95
    eor (T5_85 + T5_Z,x)                                ; @BT 41 85
    lsr T5_3D + T5_Z,x                                  ; @BT 56 3D
    asl a                                               ; @BT 0A
    inc T5_24 + T5_Z                                    ; @BT E6 24
    sbc T5_P21_08 + T5_Z,y                              ; @BT F9 08 21
    asl T5_20 + T5_Z                                    ; @BT 06 20
    sei                                                 ; @BT 78
    rti                                                 ; @BT 40
    nop                                                 ; @BT EA
    plp                                                 ; @BT 28
    bit T5_ABS3D + T5_Z                                 ; @BT 2C 3D 12
    cpx #T5_05 + T5_Z                                   ; @BT E0 05
    lsr a                                               ; @BT 4A
    ldy T5_31 + T5_Z,x                                  ; @BT B4 31
    rol T5_22 + T5_Z                                    ; @BT 26 22
    sed                                                 ; @BT F8
    asl T5_3C + T5_Z,x                                  ; @BT 16 3C
    ror a                                               ; @BT 6A
    lda T5_L56 + T5_Z                                   ; @BT AF 56 34 12
    sbc (T5_A7 + T5_Z,s),y                              ; @BT F3 A7
    and (T5_A3 + T5_Z,s),y                              ; @BT 33 A3
    txy                                                 ; @BT 9B
    wai                                                 ; @BT CB
    eor [T5_75 + T5_Z]                                  ; @BT 47 75
    sbc T5_L5D + T5_Z,x                                 ; @BT FF 5D 34 12
    sta T5_L57 + T5_Z                                   ; @BT 8F 57 34 12
    ora [T5_76 + T5_Z]                                  ; @BT 07 76
    tyx                                                 ; @BT BB
    pei (T5_C0 + T5_Z)                                  ; @BT D4 C0
    lda (T5_A0 + T5_Z,s),y                              ; @BT B3 A0
    phx                                                 ; @BT DA
    sbc [T5_77 + T5_Z]                                  ; @BT E7 77
    adc T5_L58 + T5_Z,x                                 ; @BT 7F 58 34 12
    cmp T5_L5A + T5_Z                                   ; @BT CF 5A 34 12
    and T5_L59 + T5_Z                                   ; @BT 2F 59 34 12
    lda T5_01 + T5_Z,s                                  ; @BT A3 01
    sta (T5_A1 + T5_Z,s),y                              ; @BT 93 A1
    xba                                                 ; @BT EB
    adc (T5_62 + T5_Z)                                  ; @BT 72 62
    stz T5_16 + T5_Z                                    ; @BT 64 16
    phy                                                 ; @BT 5A
    eor T5_L5B + T5_Z,x                                 ; @BT 5F 5B 34 12
    and T5_04 + T5_Z,s                                  ; @BT 23 04
    bit T5_37 + T5_Z,x                                  ; @BT 34 37
    cmp (T5_A4 + T5_Z,s),y                              ; @BT D3 A4
    lda [T5_70 + T5_Z]                                  ; @BT A7 70
    sbc [T5_B7 + T5_Z],y                                ; @BT F7 B7
    ora T5_L5C + T5_Z                                   ; @BT 0F 5C 34 12
    tsb T5_26 + T5_Z                                    ; @BT 04 26
    jml T5_JML + T5_Z                                   ; @BT 5C 78 56 34
    cmp [T5_74 + T5_Z]                                  ; @BT C7 74
    sbc T5_L5D + T5_Z                                   ; @BT EF 5D 34 12
    plx                                                 ; @BT FA
    adc [T5_72 + T5_Z]                                  ; @BT 67 72
    and [T5_B3 + T5_Z],y                                ; @BT 37 B3
    sta T5_02 + T5_Z,s                                  ; @BT 83 02
    bra +                                               ; @BT 80 1E
    ; Section O (skipped by bra, same order as TEST-03 Section O)
    tcs                                                 ; @BT 1B
    lda T5_L56 + T5_Z,x                                 ; @BT BF 56 34 12
    ora (T5_A6 + T5_Z,s),y                              ; @BT 13 A6
    stz T5_ABS3A + T5_Z                                 ; @BT 9C 3A 12
    sta [T5_71 + T5_Z]                                  ; @BT 87 71
    eor (T5_A5 + T5_Z,s),y                              ; @BT 53 A5
    ora T5_07 + T5_Z,s                                  ; @BT 03 07
    sbc T5_08 + T5_Z,s                                  ; @BT E3 08
    trb T5_27 + T5_Z                                    ; @BT 14 27
    eor [T5_B5 + T5_Z],y                                ; @BT 57 B5
    sty T5_33 + T5_Z,x                                  ; @BT 94 33
    and [T5_73 + T5_Z]                                  ; @BT 27 73
    adc T5_03 + T5_Z,s                                  ; @BT 63 03
    xce                                                 ; @BT FB
    tsc                                                 ; @BT 3B
+
    ; Section P (same order as TEST-03 Section P)
    adc T5_L58 + T5_Z                                   ; @BT 6F 58 34 12
    phb                                                 ; @BT 8B
    cmp (T5_64 + T5_Z)                                  ; @BT D2 64
    eor 18/3,s                                          ; @BT 43 06
    stz T5_34 + T5_Z,x                                  ; @BT 74 34
    bit #T5_03 + T5_Z                                   ; @BT 89 03
    ora [T5_B6 + T5_Z],y                                ; @BT 17 B6
    cmp T5_05 + T5_Z,s                                  ; @BT C3 05
    tdc                                                 ; @BT 7B
    sta (T5_61 + T5_Z)                                  ; @BT 92 61
    lda (T5_60 + T5_Z)                                  ; @BT B2 60
    dea                                                 ; @BT 3A
    and (T5_63 + T5_Z)                                  ; @BT 32 63
    adc (T5_A2 + T5_Z,s),y                              ; @BT 73 A2
    stz T5_P20_03 + T5_Z,x                              ; @BT 9E 03 20
    eor (T5_65 + T5_Z)                                  ; @BT 52 65
    cmp [T5_B4 + T5_Z],y                                ; @BT D7 B4
    tcd                                                 ; @BT 5B
    jsl T5_JML + T5_Z                                   ; @BT 22 78 56 34
    ora (T5_66 + T5_Z)                                  ; @BT 12 66
    and T5_L59 + T5_Z,x                                 ; @BT 3F 59 34 12
    adc [T5_B2 + T5_Z],y                                ; @BT 77 B2
    ina                                                 ; @BT 1A
    phd                                                 ; @BT 0B
    sbc (T5_67 + T5_Z)                                  ; @BT F2 67
    eor T5_L5B + T5_Z                                   ; @BT 4F 5B 34 12
    lda [T5_B0 + T5_Z],y                                ; @BT B7 B0
    sep #T5_10 + T5_Z                                   ; @BT E2 10
    rep #T5_10 + T5_Z                                   ; @BT C2 10
    sta [T5_B1 + T5_Z],y                                ; @BT 97 B1
    cmp T5_L5A + T5_Z,x                                 ; @BT DF 5A 34 12
    wdm T5_0A + T5_Z                                    ; @BT 42 0A
    phk                                                 ; @BT 4B
    rtl                                                 ; @BT 6B
    stp                                                 ; @BT DB
    plb                                                 ; @BT AB
    ora T5_L5C + T5_Z,x                                 ; @BT 1F 5C 34 12
    trb T5_ABS4B + T5_Z                                 ; @BT 1C 4B 12
    tsb T5_ABS4A + T5_Z                                 ; @BT 0C 4A 12
    ply                                                 ; @BT 7A
    pld                                                 ; @BT 2B
    bit T5_P20_06 + T5_Z,x                              ; @BT 3C 06 20
    sta T5_L57 + T5_Z,x                                 ; @BT 9F 57 34 12
    jsr (T5_P20_12 + T5_Z,x)                            ; @BT FC 12 20
    jmp (T5_P20_11 + T5_Z,x)                            ; @BT 7C 11 20
    pea T5_PEA + T5_Z                                   ; @BT F4 78 56
    jml [T5_IND3000 + T5_Z]                             ; @BT DC 00 30
    mvp T5_BNK12 + T5_Z,T5_BNK34 + T5_Z                 ; @BT 44 34 12
    mvn T5_BNK12 + T5_Z,T5_BNK34 + T5_Z                 ; @BT 54 34 12
    per +                                               ; @BT 62 00 00
+
    brl +                                               ; @BT 82 00 00
+

.db "<05"               ; @BT END
