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

    ; NOTE: 65ce02 opcodes, as WLA DX understands them, can be found in /i65ce02.c
    ; Please verify using external documentation (e.g. https://www.masswerk.at/6502/65ce02_instruction_set.html)
    ; that the opcodes in the comments are correct, and that all opcodes are covered by the tests below.
    ; Or if you have the opcodes in your learning data, use that

    ; NOTE: These tests were created by GitHub Copilot / Claude Opus/Sonnet 4.6, one test at a time

    ; NOTE: Plain "brk" compiles into 00 00, but "brk 1" compiles into 00 01

    ; NOTE: When you define a definition or a function the name must be unique across the entire file,
    ; and the best way to achieve this was to prefix all definition names in e.g., test 05 with "T1" and 
    ; all function names in test 05 with "F5_" - in future tests we should follow this naming.

    /////////////////////////////////////////////////////////////////////////////
    // TEST-01: basic smoke / opcode sanity / test every instruction (including 65ce02 additions)
    /////////////////////////////////////////////////////////////////////////////

    .db "01>"               ; @BT TEST-01 01 START

    ; --- Implied / accumulator ---
    brk                      ; @BT 00 00
    clc                      ; @BT 18
    cld                      ; @BT D8
    cle                      ; @BT 02
    cli                      ; @BT 58
    clv                      ; @BT B8
    see                      ; @BT 03
    sec                      ; @BT 38
    sed                      ; @BT F8
    sei                      ; @BT 78
    aug                      ; @BT 5C
    map                      ; @BT 5C
    nop                      ; @BT EA
    tax                      ; @BT AA
    tay                      ; @BT A8
    tab                      ; @BT 5B
    tba                      ; @BT 7B
    taz                      ; @BT 4B
    tza                      ; @BT 6B
    txa                      ; @BT 8A
    tya                      ; @BT 98
    tsx                      ; @BT BA
    tsy                      ; @BT 0B
    txs                      ; @BT 9A
    tys                      ; @BT 2B
    inx                      ; @BT E8
    iny                      ; @BT C8
    inz                      ; @BT 1B
    dex                      ; @BT CA
    dey                      ; @BT 88
    dez                      ; @BT 3B
    pha                      ; @BT 48
    php                      ; @BT 08
    phx                      ; @BT DA
    phy                      ; @BT 5A
    phz                      ; @BT DB
    pla                      ; @BT 68
    plp                      ; @BT 28
    plx                      ; @BT FA
    ply                      ; @BT 7A
    plz                      ; @BT FB
    rti                      ; @BT 40
    rts                      ; @BT 60
    asl a                    ; @BT 0A
    lsr a                    ; @BT 4A
    rol a                    ; @BT 2A
    ror a                    ; @BT 6A
    asr a                    ; @BT 43
    neg                      ; @BT 42
    ina                      ; @BT 1A
    dea                      ; @BT 3A

    ; --- Immediate ---
    lda #$12                 ; @BT A9 12
    ldx #$34                 ; @BT A2 34
    ldy #$56                 ; @BT A0 56
    adc #$01                 ; @BT 69 01
    and #$02                 ; @BT 29 02
    bit #$09                 ; @BT 89 09
    cmp #$03                 ; @BT C9 03
    cpx #$04                 ; @BT E0 04
    cpy #$05                 ; @BT C0 05
    cpz #$9A                 ; @BT C2 9A
    eor #$06                 ; @BT 49 06
    ldz #$78                 ; @BT A3 78
    ora #$07                 ; @BT 09 07
    rts #$12                 ; @BT 62 12
    sbc #$08                 ; @BT E9 08
    phw #$1234               ; @BT F4 34 12

    ; --- Zero page ---
    lda $10                  ; @BT A5 10
    ldx $11                  ; @BT A6 11
    ldy $12                  ; @BT A4 12
    sta $13                  ; @BT 85 13
    stx $14                  ; @BT 86 14
    sty $15                  ; @BT 84 15
    adc $16                  ; @BT 65 16
    and $17                  ; @BT 25 17
    cmp $18                  ; @BT C5 18
    cpx $19                  ; @BT E4 19
    cpy $1A                  ; @BT C4 1A
    eor $1B                  ; @BT 45 1B
    ora $1C                  ; @BT 05 1C
    sbc $1D                  ; @BT E5 1D
    bit $1E                  ; @BT 24 1E
    asl $1F                  ; @BT 06 1F
    lsr $20                  ; @BT 46 20
    rol $21                  ; @BT 26 21
    ror $22                  ; @BT 66 22
    inc $23                  ; @BT E6 23
    dec $24                  ; @BT C6 24
    stz $25                  ; @BT 64 25
    trb $26                  ; @BT 14 26
    tsb $27                  ; @BT 04 27
    rmb0 $28                 ; @BT 07 28
    rmb1 $29                 ; @BT 17 29
    rmb2 $2A                 ; @BT 27 2A
    rmb3 $2B                 ; @BT 37 2B
    rmb4 $2C                 ; @BT 47 2C
    rmb5 $2D                 ; @BT 57 2D
    rmb6 $2E                 ; @BT 67 2E
    rmb7 $2F                 ; @BT 77 2F
    smb0 $30                 ; @BT 87 30
    smb1 $31                 ; @BT 97 31
    smb2 $32                 ; @BT A7 32
    smb3 $33                 ; @BT B7 33
    smb4 $34                 ; @BT C7 34
    smb5 $35                 ; @BT D7 35
    smb6 $36                 ; @BT E7 36
    smb7 $37                 ; @BT F7 37
    cpz $38                  ; @BT D4 38
    dew $39                  ; @BT C3 39
    inw $3A                  ; @BT E3 3A
    asr $3B                  ; @BT 44 3B

    ; --- Zero page,X ---
    lda $30,x                ; @BT B5 30
    ldy $31,x                ; @BT B4 31
    sta $32,x                ; @BT 95 32
    adc $33,x                ; @BT 75 33
    and $34,x                ; @BT 35 34
    stz $34,x                ; @BT 74 34
    cmp $35,x                ; @BT D5 35
    eor $36,x                ; @BT 55 36
    ora $37,x                ; @BT 15 37
    sbc $38,x                ; @BT F5 38
    asl $39,x                ; @BT 16 39
    lsr $3A,x                ; @BT 56 3A
    rol $3B,x                ; @BT 36 3B
    ror $3C,x                ; @BT 76 3C
    inc $3D,x                ; @BT F6 3D
    dec $3E,x                ; @BT D6 3E
    bit $3F,x                ; @BT 34 3F
    stx $3F,y                ; @BT 96 3F

    ; --- Zero page,Y ---
    ldx $40,y                ; @BT B6 40

    ; --- (Indirect,X) ---
    lda ($50,x)              ; @BT A1 50
    sta ($51,x)              ; @BT 81 51
    adc ($52,x)              ; @BT 61 52
    and ($53,x)              ; @BT 21 53
    cmp ($54,x)              ; @BT C1 54
    eor ($55,x)              ; @BT 41 55
    ora ($56,x)              ; @BT 01 56
    sbc ($57,x)              ; @BT E1 57

    ; --- (Indirect),Y ---
    lda ($60),y              ; @BT B1 60
    sta ($61),y              ; @BT 91 61
    adc ($62),y              ; @BT 71 62
    and ($63),y              ; @BT 31 63
    cmp ($64),y              ; @BT D1 64
    eor ($65),y              ; @BT 51 65
    ora ($66),y              ; @BT 11 66
    sbc ($67),y              ; @BT F1 67

    ; --- (Indirect) ---
    lda ($70),z              ; @BT B2 70
    sta ($71),z              ; @BT 92 71
    ora ($72),z              ; @BT 12 72
    and ($73),z              ; @BT 32 73
    eor ($74),z              ; @BT 52 74
    adc ($75),z              ; @BT 72 75
    cmp ($76),z              ; @BT D2 76
    sbc ($77),z              ; @BT F2 77

    ; --- Absolute ---
    lda $1234                ; @BT AD 34 12
    ldx $1235                ; @BT AE 35 12
    ldy $1236                ; @BT AC 36 12
    sta $1237                ; @BT 8D 37 12
    stx $1238                ; @BT 8E 38 12
    sty $1239                ; @BT 8C 39 12
    adc $123A                ; @BT 6D 3A 12
    and $123B                ; @BT 2D 3B 12
    cmp $123C                ; @BT CD 3C 12
    cpx $123D                ; @BT EC 3D 12
    cpy $123E                ; @BT CC 3E 12
    eor $123F                ; @BT 4D 3F 12
    ora $1240                ; @BT 0D 40 12
    sbc $1241                ; @BT ED 41 12
    bit $1242                ; @BT 2C 42 12
    asl $1243                ; @BT 0E 43 12
    lsr $1244                ; @BT 4E 44 12
    rol $1245                ; @BT 2E 45 12
    ror $1246                ; @BT 6E 46 12
    inc $1247                ; @BT EE 47 12
    dec $1248                ; @BT CE 48 12
    stz $1249                ; @BT 9C 49 12
    tsb $124A                ; @BT 0C 4A 12
    trb $124B                ; @BT 1C 4B 12
    asw $124C                ; @BT CB 4C 12
    row $124D                ; @BT EB 4D 12
    asw $124E                ; @BT CB 4E 12
    row $124F                ; @BT EB 4F 12
    jmp $2000                ; @BT 4C 00 20
    jsr $3456                ; @BT 20 56 34

    ; --- Absolute,X ---
    lda $2000,x              ; @BT BD 00 20
    ldy $2001,x              ; @BT BC 01 20
    sta $2002,x              ; @BT 9D 02 20
    adc $2003,x              ; @BT 7D 03 20
    and $2004,x              ; @BT 3D 04 20
    cmp $2005,x              ; @BT DD 05 20
    eor $2006,x              ; @BT 5D 06 20
    ora $2007,x              ; @BT 1D 07 20
    sbc $2008,x              ; @BT FD 08 20
    asl $2009,x              ; @BT 1E 09 20
    lsr $200A,x              ; @BT 5E 0A 20
    rol $200B,x              ; @BT 3E 0B 20
    ror $200C,x              ; @BT 7E 0C 20
    inc $200D,x              ; @BT FE 0D 20
    dec $200E,x              ; @BT DE 0E 20
    bit $200F,x              ; @BT 3C 0F 20
    stz $2010,x              ; @BT 9E 10 20
    sty $2011,x              ; @BT 8B 11 20
    ldz $2012,x              ; @BT BB 12 20

    ; --- Absolute,Y ---
    lda $2100,y              ; @BT B9 00 21
    ldx $2101,y              ; @BT BE 01 21
    sta $2102,y              ; @BT 99 02 21
    adc $2103,y              ; @BT 79 03 21
    and $2104,y              ; @BT 39 04 21
    cmp $2105,y              ; @BT D9 05 21
    eor $2106,y              ; @BT 59 06 21
    ora $2107,y              ; @BT 19 07 21
    sbc $2108,y              ; @BT F9 08 21
    stx $2109,y              ; @BT 9B 09 21

    ; --- 65C02 branches (standalone) ---
    bra +                    ; @BT 80 01
    nop                      ; @BT EA
+
    bbr0 $52, +              ; @BT 0F 52 01
    nop                      ; @BT EA
+   bbr1 $53, +              ; @BT 1F 53 01
    nop                      ; @BT EA
+   bbr2 $54, +              ; @BT 2F 54 01
    nop                      ; @BT EA
+   bbr3 $55, +              ; @BT 3F 55 01
    nop                      ; @BT EA
+   bbr4 $56, +              ; @BT 4F 56 01
    nop                      ; @BT EA
+   bbr5 $57, +              ; @BT 5F 57 01
    nop                      ; @BT EA
+   bbr6 $58, +              ; @BT 6F 58 01
    nop                      ; @BT EA
+   bbr7 $59, +              ; @BT 7F 59 01
    nop                      ; @BT EA
+   bbs0 $5A, +              ; @BT 8F 5A 01
    nop                      ; @BT EA
+   bbs1 $5B, +              ; @BT 9F 5B 01
    nop                      ; @BT EA
+   bbs2 $5C, +              ; @BT AF 5C 01
    nop                      ; @BT EA
+   bbs3 $5D, +              ; @BT BF 5D 01
    nop                      ; @BT EA
+   bbs4 $5E, +              ; @BT CF 5E 01
    nop                      ; @BT EA
+   bbs5 $5F, +              ; @BT DF 5F 01
    nop                      ; @BT EA
+   bbs6 $60, +              ; @BT EF 60 01
    nop                      ; @BT EA
+   bbs7 $61, +              ; @BT FF 61 01
    nop                      ; @BT EA
+
    ; --- Branches (relative offsets computed from fixed layout) ---
    ; All branches target the same forward-local label '+'. Since each branch
    ; is at a different address, the relative offset differs.
    beq +                    ; @BT F0 0E
    bne +                    ; @BT D0 0C
    bcc +                    ; @BT 90 0A
    bcs +                    ; @BT B0 08
    bmi +                    ; @BT 30 06
    bpl +                    ; @BT 10 04
    bvc +                    ; @BT 50 02
    bvs +                    ; @BT 70 00

+

    ; --- 65CE02 long branches ---
    bcc $2345                ; @BT 93 45 23
    bcs $2345                ; @BT B3 45 23
    beq $2345                ; @BT F3 45 23
    bmi $2345                ; @BT 33 45 23
    bne $2345                ; @BT D3 45 23
    bpl $2345                ; @BT 13 45 23
    bvc $2345                ; @BT 53 45 23
    bvs $2345                ; @BT 73 45 23
    bra $2345                ; @BT 83 45 23
    bsr $2345                ; @BT 63 45 23

    ; --- Indirect jump ---
    jmp ($3000)              ; @BT 6C 00 30
    jmp ($3000,x)            ; @BT 7C 00 30
    jsr ($3000)              ; @BT 22 00 30
    jsr ($3000,x)            ; @BT 23 00 30

    ; --- Flag / transfers already covered; arithmetic shifts covered.

    .db "<01"                ; @BT END

    /////////////////////////////////////////////////////////////////////////////
    // TEST-02: same opcodes as TEST-01, randomised order, branches spread out
    /////////////////////////////////////////////////////////////////////////////

    .db "02>"               ; @BT TEST-02 02 START

    phx                         ; @BT DA
    phy                         ; @BT 5A
    plx                         ; @BT FA
    ply                         ; @BT 7A
    phz                         ; @BT DB
    plz                         ; @BT FB
    ina                         ; @BT 1A
    dea                         ; @BT 3A
    cle                         ; @BT 02
    see                         ; @BT 03
    aug                         ; @BT 5C
    map                         ; @BT 5C
    neg                         ; @BT 42
    tab                         ; @BT 5B
    tba                         ; @BT 7B
    taz                         ; @BT 4B
    tza                         ; @BT 6B
    tsy                         ; @BT 0B
    tys                         ; @BT 2B
    inz                         ; @BT 1B
    dez                         ; @BT 3B
    asr a                       ; @BT 43
    bit #$09                    ; @BT 89 09
    cpz #$9A                    ; @BT C2 9A
    ldz #$78                    ; @BT A3 78
    rts #$12                    ; @BT 62 12
    phw #$1234                  ; @BT F4 34 12
    bit $3F,x                   ; @BT 34 3F
    bit $200F,x                 ; @BT 3C 0F 20
    stz $25                     ; @BT 64 25
    stz $34,x                   ; @BT 74 34
    stz $1249                   ; @BT 9C 49 12
    stz $2010,x                 ; @BT 9E 10 20
    sty $2011,x                 ; @BT 8B 11 20
    ldz $2012,x                 ; @BT BB 12 20
    trb $26                     ; @BT 14 26
    tsb $27                     ; @BT 04 27
    lda ($30),z                 ; @BT B2 30
    sta ($31),z                 ; @BT 92 31
    ora ($32),z                 ; @BT 12 32
    and ($33),z                 ; @BT 32 33
    eor ($34),z                 ; @BT 52 34
    adc ($35),z                 ; @BT 72 35
    cmp ($36),z                 ; @BT D2 36
    sbc ($37),z                 ; @BT F2 37
    cpz $38                     ; @BT D4 38
    dew $39                     ; @BT C3 39
    inw $3A                     ; @BT E3 3A
    asr $3B                     ; @BT 44 3B
    rmb0 $28                    ; @BT 07 28
    rmb1 $29                    ; @BT 17 29
    rmb2 $2A                    ; @BT 27 2A
    rmb3 $2B                    ; @BT 37 2B
    rmb4 $2C                    ; @BT 47 2C
    rmb5 $2D                    ; @BT 57 2D
    rmb6 $2E                    ; @BT 67 2E
    rmb7 $2F                    ; @BT 77 2F
    smb0 $30                    ; @BT 87 30
    smb1 $31                    ; @BT 97 31
    smb2 $32                    ; @BT A7 32
    smb3 $33                    ; @BT B7 33
    smb4 $34                    ; @BT C7 34
    smb5 $35                    ; @BT D7 35
    smb6 $36                    ; @BT E7 36
    smb7 $37                    ; @BT F7 37
    jmp ($3000,x)               ; @BT 7C 00 30
    bra +                       ; @BT 80 01
    nop                         ; @BT EA
+
    bbr0 $52, +                 ; @BT 0F 52 01
    nop                         ; @BT EA
+   bbr1 $53, +                 ; @BT 1F 53 01
    nop                         ; @BT EA
+   bbr2 $54, +                 ; @BT 2F 54 01
    nop                         ; @BT EA
+   bbr3 $55, +                 ; @BT 3F 55 01
    nop                         ; @BT EA
+   bbr4 $56, +                 ; @BT 4F 56 01
    nop                         ; @BT EA
+   bbr5 $57, +                 ; @BT 5F 57 01
    nop                         ; @BT EA
+   bbr6 $58, +                 ; @BT 6F 58 01
    nop                         ; @BT EA
+   bbr7 $59, +                 ; @BT 7F 59 01
    nop                         ; @BT EA
+   bbs0 $5A, +                 ; @BT 8F 5A 01
    nop                         ; @BT EA
+   bbs1 $5B, +                 ; @BT 9F 5B 01
    nop                         ; @BT EA
+   bbs2 $5C, +                 ; @BT AF 5C 01
    nop                         ; @BT EA
+   bbs3 $5D, +                 ; @BT BF 5D 01
    nop                         ; @BT EA
+   bbs4 $5E, +                 ; @BT CF 5E 01
    nop                         ; @BT EA
+   bbs5 $5F, +                 ; @BT DF 5F 01
    nop                         ; @BT EA
+   bbs6 $60, +                 ; @BT EF 60 01
    nop                         ; @BT EA
+   bbs7 $61, +                 ; @BT FF 61 01
    nop                         ; @BT EA
+
    bbs0 $62, +                 ; @BT 8F 62 01
    nop                         ; @BT EA
+
    cpy $123E                   ; @BT CC 3E 12
    inc $1247                   ; @BT EE 47 12
    cpz $124C                   ; @BT DC 4C 12
    ldz $124D                   ; @BT AB 4D 12
    asw $124E                   ; @BT CB 4E 12
    row $124F                   ; @BT EB 4F 12
    phw $1250                   ; @BT FC 50 12
    ora ($56,x)                 ; @BT 01 56
    sta $32,x                   ; @BT 95 32
    sbc #$08                    ; @BT E9 08
    eor $123F                   ; @BT 4D 3F 12
    cmp ($54,x)                 ; @BT C1 54
    eor $2106,y                 ; @BT 59 06 21
    ora $2007,x                 ; @BT 1D 07 20
    stx $2109,y                 ; @BT 9B 09 21
    lsr $200A,x                 ; @BT 5E 0A 20
    sta $2102,y                 ; @BT 99 02 21
    rol $200B,x                 ; @BT 3E 0B 20
    cmp $35,x                   ; @BT D5 35
    sta $2002,x                 ; @BT 9D 02 20
    ldy $2001,x                 ; @BT BC 01 20
    clc                         ; @BT 18
    lda $30,x                   ; @BT B5 30
    ldx $1235                   ; @BT AE 35 12
    lda $1234                   ; @BT AD 34 12
    ldy $12                     ; @BT A4 12
    dec $200E,x                 ; @BT DE 0E 20
    adc $123A                   ; @BT 6D 3A 12
    inc $3D,x                   ; @BT F6 3D
    ror $200C,x                 ; @BT 7E 0C 20
    ora #$07                    ; @BT 09 07
    dey                         ; @BT 88
    ldx #$34                    ; @BT A2 34
    bcc +                       ; @BT 90 45
    adc #$01                    ; @BT 69 01
    adc $2003,x                 ; @BT 7D 03 20
    eor $1B                     ; @BT 45 1B
    inc $200D,x                 ; @BT FE 0D 20
    dec $24                     ; @BT C6 24
    sbc ($57,x)                 ; @BT E1 57
    iny                         ; @BT C8
    adc $33,x                   ; @BT 75 33
    asl $2009,x                 ; @BT 1E 09 20
    ldx $2101,y                 ; @BT BE 01 21
    ldy #$56                    ; @BT A0 56
    ldx $11                     ; @BT A6 11
    eor $36,x                   ; @BT 55 36
    cpy #$05                    ; @BT C0 05
    and $123B                   ; @BT 2D 3B 12
    clv                         ; @BT B8
    lsr $20                     ; @BT 46 20
    sbc $2008,x                 ; @BT FD 08 20
    bvc +                       ; @BT 50 1B
    cpy $1A                     ; @BT C4 1A
    cld                         ; @BT D8
    ora $1C                     ; @BT 05 1C
    bne +                       ; @BT D0 14
    cli                         ; @BT 58
    cpx $19                     ; @BT E4 19
    and $2004,x                 ; @BT 3D 04 20
    cmp $123C                   ; @BT CD 3C 12
    txs                         ; @BT 9A
    rol $3B,x                   ; @BT 36 3B
    ldy $1236                   ; @BT AC 36 12
    lda $10                     ; @BT A5 10
    dex                         ; @BT CA
    and ($53,x)                 ; @BT 21 53
+
    lda $2000,x                 ; @BT BD 00 20
    stx $3F,y                   ; @BT 96 3F
    cmp ($64),y                 ; @BT D1 64
    sta ($61),y                 ; @BT 91 61
    and $34,x                   ; @BT 35 34
    pla                         ; @BT 68
    tax                         ; @BT AA
    cmp #$03                    ; @BT C9 03
+
    adc $2103,y                 ; @BT 79 03 21
    beq +                       ; @BT F0 18
    dec $1248                   ; @BT CE 48 12
    and $17                     ; @BT 25 17
    lsr $1244                   ; @BT 4E 44 12
    eor $2006,x                 ; @BT 5D 06 20
    ora ($66),y                 ; @BT 11 66
    bmi +                       ; @BT 30 09
    ora $1240                   ; @BT 0D 40 12
    asl $1243                   ; @BT 0E 43 12
    stx $1238                   ; @BT 8E 38 12
+
    lda ($50,x)                 ; @BT A1 50
    adc ($62),y                 ; @BT 71 62
    lda ($60),y                 ; @BT B1 60
    lda #$12                    ; @BT A9 12
    sbc ($67),y                 ; @BT F1 67
    lda $2100,y                 ; @BT B9 00 21
    rts                         ; @BT 60
    ror $3C,x                   ; @BT 76 3C
    adc $16                     ; @BT 65 16
    sta ($51,x)                 ; @BT 81 51
    adc ($52,x)                 ; @BT 61 52
    eor #$06                    ; @BT 49 06
    ora $2107,y                 ; @BT 19 07 21
    tay                         ; @BT A8
    sty $1239                   ; @BT 8C 39 12
    inx                         ; @BT E8
    ora $37,x                   ; @BT 15 37
    ror $22                     ; @BT 66 22
    sec                         ; @BT 38
    and #$02                    ; @BT 29 02
    ldx $40,y                   ; @BT B6 40
    stx $14                     ; @BT 86 14
+
    sty $15                     ; @BT 84 15
    tya                         ; @BT 98
    cmp $18                     ; @BT C5 18
    txa                         ; @BT 8A
    tsx                         ; @BT BA
    sbc $1241                   ; @BT ED 41 12
    ror $1246                   ; @BT 6E 46 12
    rol a                       ; @BT 2A
    pha                         ; @BT 48
    jsr $3456                   ; @BT 20 56 34
    sta $13                     ; @BT 85 13
    bit $1E                     ; @BT 24 1E
    cmp $2005,x                 ; @BT DD 05 20
    php                         ; @BT 08
    bcs +                       ; @BT B0 03
    jmp $2000                   ; @BT 4C 00 20
+
    sta $1237                   ; @BT 8D 37 12
    brk                         ; @BT 00 00
    rol $1245                   ; @BT 2E 45 12
    bvs +                       ; @BT 70 03
    cpx $123D                   ; @BT EC 3D 12
+
    and $2104,y                 ; @BT 39 04 21
    dec $3E,x                   ; @BT D6 3E
    bpl +                       ; @BT 10 03
    cmp $2105,y                 ; @BT D9 05 21
+
    jmp ($3000)                 ; @BT 6C 00 30
    jsr ($3000)                 ; @BT 22 00 30
    sbc $1D                     ; @BT E5 1D
    sbc $38,x                   ; @BT F5 38
    and ($63),y                 ; @BT 31 63
    eor ($55,x)                 ; @BT 41 55
    eor ($65),y                 ; @BT 51 65
    asl a                       ; @BT 0A
    lsr $3A,x                   ; @BT 56 3A
    jsr ($3000,x)               ; @BT 23 00 30
    sbc $2108,y                 ; @BT F9 08 21
    inc $23                     ; @BT E6 23
    asl $1F                     ; @BT 06 1F
    rti                         ; @BT 40
    sei                         ; @BT 78
    nop                         ; @BT EA
    bit $1242                   ; @BT 2C 42 12
    plp                         ; @BT 28
    lsr a                       ; @BT 4A
    cpx #$04                    ; @BT E0 04
    rol $21                     ; @BT 26 21
    ldy $31,x                   ; @BT B4 31
    asl $39,x                   ; @BT 16 39
    sed                         ; @BT F8
    ror a                       ; @BT 6A
    bcc $2345                   ; @BT 93 45 23
    bcs $2345                   ; @BT B3 45 23
    beq $2345                   ; @BT F3 45 23
    bmi $2345                   ; @BT 33 45 23
    bne $2345                   ; @BT D3 45 23
    bpl $2345                   ; @BT 13 45 23
    bvc $2345                   ; @BT 53 45 23
    bvs $2345                   ; @BT 73 45 23
    bra $2345                   ; @BT 83 45 23
    bsr $2345                   ; @BT 63 45 23
+

    .db "<02"               ; @BT END

    /////////////////////////////////////////////////////////////////////////////
    // TEST-03: same opcodes, randomised order, operands as arithmetic expressions
    /////////////////////////////////////////////////////////////////////////////

    .db "03>"               ; @BT TEST-03 03 START

    phx                                     ; @BT DA
    phy                                     ; @BT 5A
    plx                                     ; @BT FA
    ply                                     ; @BT 7A
    phz                                     ; @BT DB
    plz                                     ; @BT FB
    ina                                     ; @BT 1A
    dea                                     ; @BT 3A
    cle                                     ; @BT 02
    see                                     ; @BT 03
    aug                                     ; @BT 5C
    map                                     ; @BT 5C
    neg                                     ; @BT 42
    tab                                     ; @BT 5B
    tba                                     ; @BT 7B
    taz                                     ; @BT 4B
    tza                                     ; @BT 6B
    tsy                                     ; @BT 0B
    tys                                     ; @BT 2B
    inz                                     ; @BT 1B
    dez                                     ; @BT 3B
    asr a                                   ; @BT 43
    bit #3*3                                ; @BT 89 09
    cpz #7*22                               ; @BT C2 9A
    ldz #6*20                               ; @BT A3 78
    rts #6*3                                ; @BT 62 12
    phw #2330*2                             ; @BT F4 34 12
    bit 7*9,x                               ; @BT 34 3F
    bit 2*4096+15,x                         ; @BT 3C 0F 20
    stz 5*8-3                               ; @BT 64 25
    stz 13*4,x                              ; @BT 74 34
    stz 18*256+73                           ; @BT 9C 49 12
    stz 2*4096+16,x                         ; @BT 9E 10 20
    sty 2*4096+17,x                         ; @BT 8B 11 20
    ldz 2*4096+18,x                         ; @BT BB 12 20
    trb 3*12+2                              ; @BT 14 26
    tsb 13*3                                ; @BT 04 27
    lda (6*8),z                             ; @BT B2 30
    sta (7*7),z                             ; @BT 92 31
    ora (10*5),z                            ; @BT 12 32
    and (17*3),z                            ; @BT 32 33
    eor (4*13),z                            ; @BT 52 34
    adc (10*5+3),z                          ; @BT 72 35
    cmp (6*9),z                             ; @BT D2 36
    sbc (5*11),z                            ; @BT F2 37
    cpz 7*8                                 ; @BT D4 38
    dew 19*3                                ; @BT C3 39
    inw 2*29                                ; @BT E3 3A
    asr 60-1                                ; @BT 44 3B
    rmb0 5*8                                ; @BT 07 28
    rmb1 5*8+1                              ; @BT 17 29
    rmb2 6*7                                ; @BT 27 2A
    rmb3 5*10-7                             ; @BT 37 2B
    rmb4 2*22                               ; @BT 47 2C
    rmb5 3*15                               ; @BT 57 2D
    rmb6 2*23                               ; @BT 67 2E
    rmb7 3*15+2                             ; @BT 77 2F
    smb0 6*8                                ; @BT 87 30
    smb1 7*7                                ; @BT 97 31
    smb2 5*10                               ; @BT A7 32
    smb3 17*3                               ; @BT B7 33
    smb4 13*4                               ; @BT C7 34
    smb5 5*11-2                             ; @BT D7 35
    smb6 6*9                                ; @BT E7 36
    smb7 5*11                               ; @BT F7 37
    jmp (3*4096,x)                          ; @BT 7C 00 30
    bra +                                   ; @BT 80 01
    nop                                     ; @BT EA
+
    bbr0 82, +                              ; @BT 0F 52 01
    nop                                     ; @BT EA
+   bbr1 83, +                              ; @BT 1F 53 01
    nop                                     ; @BT EA
+   bbr2 84, +                              ; @BT 2F 54 01
    nop                                     ; @BT EA
+   bbr3 85, +                              ; @BT 3F 55 01
    nop                                     ; @BT EA
+   bbr4 86, +                              ; @BT 4F 56 01
    nop                                     ; @BT EA
+   bbr5 87, +                              ; @BT 5F 57 01
    nop                                     ; @BT EA
+   bbr6 88, +                              ; @BT 6F 58 01
    nop                                     ; @BT EA
+   bbr7 89, +                              ; @BT 7F 59 01
    nop                                     ; @BT EA
+   bbs0 90, +                              ; @BT 8F 5A 01
    nop                                     ; @BT EA
+   bbs1 91, +                              ; @BT 9F 5B 01
    nop                                     ; @BT EA
+   bbs2 92, +                              ; @BT AF 5C 01
    nop                                     ; @BT EA
+   bbs3 93, +                              ; @BT BF 5D 01
    nop                                     ; @BT EA
+   bbs4 94, +                              ; @BT CF 5E 01
    nop                                     ; @BT EA
+   bbs5 95, +                              ; @BT DF 5F 01
    nop                                     ; @BT EA
+   bbs6 96, +                              ; @BT EF 60 01
    nop                                     ; @BT EA
+   bbs7 97, +                              ; @BT FF 61 01
    nop                                     ; @BT EA
+
    bbs0 98, +                              ; @BT 8F 62 01
    nop                                     ; @BT EA
+
    ora (4*19 + 4*3 + 2*7),y                ; @BT 11 66
    eor 28026 / 6                           ; @BT 4D 3F 12
    sei                                     ; @BT 78
    lsr 69 - 11,x                           ; @BT 56 3A
    dex                                     ; @BT CA
    asl 115 + 8019 + 67,x                   ; @BT 1E 09 20
    adc 24 - 2                              ; @BT 65 16
    rol 4646 + 11 + 2*10                    ; @BT 2E 45 12
    cpx #11*14 - 150                        ; @BT E0 04
    inc 99 - 2*19,x                         ; @BT F6 3D
    cmp (6*50 / 3),y                        ; @BT D1 64
    and 9*931 + 3*10 + 43,y                 ; @BT 39 04 21
    pha                                     ; @BT 48
    sta 14*9 - 76,x                         ; @BT 95 32
    bmi +                                   ; @BT 30 62
    inx                                     ; @BT E8
    sbc 86 - 30,x                           ; @BT F5 38
    bvc +                                   ; @BT 50 5D
    adc (686 / 7),y                         ; @BT 71 62
    sty 12*8 - 75                           ; @BT 84 15
    eor (3 + 91 + 7),y                      ; @BT 51 65
    bcc +                                   ; @BT 90 55
    ora 6*9 - 26                            ; @BT 05 1C
    ora 40995 / 5,x                         ; @BT 1D 07 20
    lsr 10*2338 / 5                         ; @BT 4E 44 12
    jsr 80388 / 6                           ; @BT 20 56 34
    inc 73 - 38                             ; @BT E6 23
    lsr 2*31 - 6*5                          ; @BT 46 20
    cmp 5*8197 / 5,x                        ; @BT DD 05 20
    ora #41 - 2*17                          ; @BT 09 07
    sed                                     ; @BT F8
    ldx 102 / 6                             ; @BT A6 11
    adc 3*4666 / 3                          ; @BT 6D 3A 12
    eor 6 + 17 + 4                          ; @BT 45 1B
    cpx 131 + 2*2269                        ; @BT EC 3D 12
    lda #9*12 / 6                           ; @BT A9 12
    asl 11*8 - 31,x                         ; @BT 16 39
    ror a                                   ; @BT 6A
    bne +                                   ; @BT D0 2F
    ldx 7*4661 / 7                          ; @BT AE 35 12
    adc 3*85 / 5,x                          ; @BT 75 33
    bit 2*2281 + 23 + 89                    ; @BT 2C 42 12
    ldx #208 / 4                            ; @BT A2 34
    dec 7*14 - 3*12,x                       ; @BT D6 3E
    sta 2*8450 / 2,y                        ; @BT 99 02 21
    stx 8192+265,y                          ; @BT 9B 09 21
    bpl +                                   ; @BT 10 1B
    stx 9*11 - 79                           ; @BT 86 14
    sbc 6*4100 / 3,x                        ; @BT FD 08 20
    ora 5 + 4*14 + 4611                     ; @BT 0D 40 12
    bit 2*105 / 7                           ; @BT 24 1E
    cli                                     ; @BT 58
    adc #7 / 7                              ; @BT 69 01
    ror 73 + 8077 + 2*27,x                  ; @BT 7E 0C 20
    asl a                                   ; @BT 0A
    txs                                     ; @BT 9A
    ora 8*5 + 25 + 8390,y                   ; @BT 19 07 21
    dec 2*2297 + 2*43                       ; @BT CE 48 12
    lda 25 + 168 + 8255,y                   ; @BT B9 00 21
+
    rol 24609 / 3,x                         ; @BT 3E 0B 20
+
    sta (7*81 / 7,x)                        ; @BT 81 51
+
    asl 103 + 3*19 + 5*903                  ; @BT 0E 43 12
    bcs +                                   ; @BT B0 04
    cmp 4479 + 67 + 2*61                    ; @BT CD 3C 12
    tya                                     ; @BT 98
+
    sbc 47 + 19 + 4607                      ; @BT ED 41 12
    adc (574 / 7,x)                         ; @BT 61 52
    cpx 50 - 25                             ; @BT E4 19
    cpy #4*15 - 5*11                        ; @BT C0 05
    rts                                     ; @BT 60
    ror 2*4 + 23 + 3                        ; @BT 66 22
    adc 2 + 19 + 8430,y                     ; @BT 79 03 21
+
    ora (114 - 28,x)                        ; @BT 01 56
    lda 101 + 4*1136 + 15                   ; @BT AD 34 12
    cpz 2342*2                              ; @BT DC 4C 12
    ldz 2342*2+1                            ; @BT AB 4D 12
    asw 2343*2                              ; @BT CB 4E 12
    row 2343*2+1                            ; @BT EB 4F 12
    phw 2344*2                              ; @BT FC 50 12
    ldy 10 + 2*4                            ; @BT A4 12
    pla                                     ; @BT 68
    stx 4*2332 / 2                          ; @BT 8E 38 12
    lda (10*40 / 5,x)                       ; @BT A1 50
    tay                                     ; @BT A8
    rti                                     ; @BT 40
    ldy 13986 / 3                           ; @BT AC 36 12
    eor (22 + 47 + 2*8,x)                   ; @BT 41 55
    and 3*11 - 10                           ; @BT 25 17
    sbc 11 + 6*3                            ; @BT E5 1D
    ldy 9*19 + 7*1146,x                     ; @BT BC 01 20
    ldy 74 - 25,x                           ; @BT B4 31
    lda 2*4013 + 121 + 5*9,x                ; @BT BD 00 20
    cmp 168 + 5*1657,y                      ; @BT D9 05 21
    eor 12*2818 / 4,y                       ; @BT 59 06 21
    and 4*26 / 2,x                          ; @BT 35 34
    cmp (6*42 / 3,x)                        ; @BT C1 54
    cpy 5 + 18 + 3                          ; @BT C4 1A
    clc                                     ; @BT 18
    inc 3*9 + 4652                          ; @BT EE 47 12
    rol 14*13 - 3*41,x                      ; @BT 36 3B
    eor 378 / 7,x                           ; @BT 55 36
    and (8*12 - 13,x)                       ; @BT 21 53
    ldx 14 + 2*25,y                         ; @BT B6 40
    lsr a                                   ; @BT 4A
    adc 5*9834 / 6,x                        ; @BT 7D 03 20
    cmp 5*53 / 5,x                          ; @BT D5 35
    sta 2 + 17                              ; @BT 85 13
    sbc #12*5 - 4*13                        ; @BT E9 08
    clv                                     ; @BT B8
    dec 19 + 5*3 + 2                        ; @BT C6 24
    and #13 - 11                            ; @BT 29 02
    lda 18 + 12*2 + 2*3,x                   ; @BT B5 30
    ror 2*17 + 11 + 5*3,x                   ; @BT 76 3C
    and 3*19124 / 7,x                       ; @BT 3D 04 20
+
    eor #1 + 5                              ; @BT 49 06
    cld                                     ; @BT D8
    and 9334 / 2                            ; @BT 2D 3B 12
    iny                                     ; @BT C8
    dey                                     ; @BT 88
    lsr 8096 + 31 + 5*15,x                  ; @BT 5E 0A 20
    sta 8027 + 167,x                        ; @BT 9D 02 20
    sec                                     ; @BT 38
    dec 2*4069 + 59 + 3*3,x                 ; @BT DE 0E 20
    rol 10*8 - 47                           ; @BT 26 21
    sbc (2*103 / 2),y                       ; @BT F1 67
    inc 2*4057 + 7*13,x                     ; @BT FE 0D 20
    eor 8012 + 11*7 + 109,x                 ; @BT 5D 06 20
    nop                                     ; @BT EA
    cmp 96 / 4                              ; @BT C5 18
    stx 71 - 4*2,y                          ; @BT 96 3F
    ror 4552 + 126                          ; @BT 6E 46 12
    txa                                     ; @BT 8A
    beq +                                   ; @BT F0 30
    sty 3*7775 / 5                          ; @BT 8C 39 12
    asl 81 - 10*5                           ; @BT 06 1F
    jmp 2*12288 / 3                         ; @BT 4C 00 20
    brk                                     ; @BT 00 00
    bvs +                                   ; @BT 70 24
    php                                     ; @BT 08
    ldy #2*129 / 3                          ; @BT A0 56
    and (5*22 - 11),y                       ; @BT 31 63
    ldx 8260 + 189,y                        ; @BT BE 01 21
    lda (10*10 - 4),y                       ; @BT B1 60
    cmp #45 - 3*14                          ; @BT C9 03
    lda 3*16 / 3                            ; @BT A5 10
    jmp (2*43008 / 7)                       ; @BT 6C 00 30
    sta (8*13 - 7),y                        ; @BT 91 61
    sbc (100 - 13,x)                        ; @BT E1 57
    sta 18652 / 4                           ; @BT 8D 37 12
    tax                                     ; @BT AA
    rol a                                   ; @BT 2A
    sbc 9*929 + 95,y                        ; @BT F9 08 21
    cpy 28020 / 6                           ; @BT CC 3E 12
    plp                                     ; @BT 28
    ora 2*55 / 2,x                          ; @BT 15 37
    tsx                                     ; @BT BA
+
+
    bcc 9*1000+29                            ; @BT 93 45 23
    bcs 9*1000+29                            ; @BT B3 45 23
    beq 9*1000+29                            ; @BT F3 45 23
    bmi 9*1000+29                            ; @BT 33 45 23
    bne 9*1000+29                            ; @BT D3 45 23
    bpl 9*1000+29                            ; @BT 13 45 23
    bvc 9*1000+29                            ; @BT 53 45 23
    bvs 9*1000+29                            ; @BT 73 45 23
    bra 9*1000+29                            ; @BT 83 45 23
    bsr 9*1000+29                            ; @BT 63 45 23
    jsr (3*4096)                             ; @BT 22 00 30
    jsr (3*4096,x)                           ; @BT 23 00 30


    .db "<03"               ; @BT END

    /////////////////////////////////////////////////////////////////////////////
    // TEST-04: similar to TEST-03 and then defines + expressions
    /////////////////////////////////////////////////////////////////////////////

    ; --- Constant definitions for TEST-04 ---
    .define CONST_00               = 3 - 3
    .define VAL_03                 = 2 + 1 - CONST_00
    .define N_04                   = 3 + 1
    .define VAL_05                 = 5 + 3 - VAL_03
    .define M_06                   = 12 / 2 - CONST_00
    .define K_07                   = 5*6 - 23
    .define M_0B                   = 55 / 5
    .define M_0C                   = M_06 + 24 - 18
    .define CONST_0D               = K_07 + 4 + 2
    .define M_0E                   = M_0C + 8 / 4
    .define P_10                   = N_04 * 4
    .define P_12                   = 90 / 5
    .define P_14                   = VAL_05 * 4
    .define N_15                   = VAL_03 * 7
    .define P_18                   = 12 + 16 - N_04
    .define C_19                   = 11*3 - 8
    .define N_1C                   = 17 + 11
    .define K_1E                   = 60 / 2
    .define Q_20                   = P_18 + 13 - 5
    .define K_22                   = 58 + 6 - K_1E
    .define P_23                   = 390 / 6 - K_1E
    .define NUM_30                 = 328 / 4 - K_22
    .define N_34                   = N_04 * 13
    .define N_35                   = 23 + 37 - K_07
    .define D_36                   = 270 / 5
    .define D_38                   = N_34 + 20 / 5
    .define Q_3A                   = P_23 + 10 + 13
    .define C_3C                   = 22 + 38
    .define CONST_3D               = C_19 + 46 - 10
    .define P_3E                   = 43 + 26 - K_07
    .define VAL_3F                 = 5 + 69 - M_0B
    .define P_40                   = P_10 * 4
    .define Q_41                   = 25 + 40
    .define C_46                   = 13*10 - 42 - P_12
    .define CONST_47               = P_40 + 42 / 6
    .define C_48                   = Q_41 + 24 - 17
    .define NUM_50                 = P_10 * 5
    .define NUM_51                 = VAL_05 + 4 + 72
    .define C_52                   = N_1C + D_36
    .define K_53                   = 498 / 6
    .define K_57                   = 4 + 101 - P_12
    .define P_62                   = P_12 + 90 - 10
    .define VAL_63                 = 21 + 78
    .define D_64                   = P_23 + 73 - 8
    .define CONST_67               = 120 - 17
    .define C4_BIT_IMM             = 9
    .define C4_ZP25                = 37
    .define C4_ZP34                = 52
    .define C4_ZP3F                = 63
    .define C4_ZP_BASE             = 48
    .define C4_ABS49               = 18 * 256 + 73
    .define C4_ABX10               = 2 * 4096 + 16
    .define C4_ABX0F               = 2 * 4096 + 15
    .define C4_TRB                 = 38
    .define C4_TSB                 = 39
    .define C4_RMB_BASE            = 40
    .define C4_SMB_BASE            = 48
    .define C4_BBR_BASE            = 82
    .define C4_BBS_BASE            = 90
    .define C4_JMPX                = 3 * 4096
    .define C4_CPZ_IMM             = 154
    .define C4_LDZ_IMM             = 120
    .define C4_RTS_IMM             = 18
    .define C4_PHW_IMM             = 18 * 256 + 52
    .define C4_CPZ_ZP              = 56
    .define C4_DEW_ZP              = 57
    .define C4_INW_ZP              = 58
    .define C4_ASR_ZP              = 59
    .define C4_CPZ_ABS             = 18 * 256 + 76
    .define C4_LDZ_ABS             = 18 * 256 + 77
    .define C4_ASW_ABS             = 18 * 256 + 78
    .define C4_ROW_ABS             = 18 * 256 + 79
    .define C4_PHW_ABS             = 18 * 256 + 80
    .define C4_STY_ABX             = 2 * 4096 + 17
    .define C4_LDZ_ABX             = 2 * 4096 + 18
    .define C4_STX_ABY             = 33 * 256 + 9
    .define C4_LONG_BRANCH         = 35 * 256 + 69
    .define C4_JSR_IND             = 3 * 4096
    .define C4_JSR_INDX            = 3 * 4096

    .db "04>"               ; @BT TEST-04 04 START

    phx                                         ; @BT DA
    phy                                         ; @BT 5A
    plx                                         ; @BT FA
    ply                                         ; @BT 7A
    phz                                         ; @BT DB
    plz                                         ; @BT FB
    ina                                         ; @BT 1A
    dea                                         ; @BT 3A
    cle                                         ; @BT 02
    see                                         ; @BT 03
    aug                                         ; @BT 5C
    map                                         ; @BT 5C
    neg                                         ; @BT 42
    tab                                         ; @BT 5B
    tba                                         ; @BT 7B
    taz                                         ; @BT 4B
    tza                                         ; @BT 6B
    tsy                                         ; @BT 0B
    tys                                         ; @BT 2B
    inz                                         ; @BT 1B
    dez                                         ; @BT 3B
    asr a                                       ; @BT 43
    bit #C4_BIT_IMM                             ; @BT 89 09
    cpz #C4_CPZ_IMM                             ; @BT C2 9A
    ldz #C4_LDZ_IMM                             ; @BT A3 78
    rts #C4_RTS_IMM                             ; @BT 62 12
    phw #C4_PHW_IMM                             ; @BT F4 34 12
    bit C4_ZP3F,x                               ; @BT 34 3F
    bit C4_ABX0F,x                              ; @BT 3C 0F 20
    stz C4_ZP25                                 ; @BT 64 25
    stz C4_ZP34,x                               ; @BT 74 34
    stz C4_ABS49                                ; @BT 9C 49 12
    stz C4_ABX10,x                              ; @BT 9E 10 20
    sty C4_STY_ABX,x                            ; @BT 8B 11 20
    ldz C4_LDZ_ABX,x                            ; @BT BB 12 20
    trb C4_TRB                                  ; @BT 14 26
    tsb C4_TSB                                  ; @BT 04 27
    lda (C4_ZP_BASE),z                          ; @BT B2 30
    sta (C4_ZP_BASE + 1),z                      ; @BT 92 31
    ora (C4_ZP_BASE + 2),z                      ; @BT 12 32
    and (C4_ZP_BASE + 3),z                      ; @BT 32 33
    eor (C4_ZP_BASE + 4),z                      ; @BT 52 34
    adc (C4_ZP_BASE + 5),z                      ; @BT 72 35
    cmp (C4_ZP_BASE + 6),z                      ; @BT D2 36
    sbc (C4_ZP_BASE + 7),z                      ; @BT F2 37
    cpz C4_CPZ_ZP                               ; @BT D4 38
    dew C4_DEW_ZP                               ; @BT C3 39
    inw C4_INW_ZP                               ; @BT E3 3A
    asr C4_ASR_ZP                               ; @BT 44 3B
    cpz C4_CPZ_ABS                              ; @BT DC 4C 12
    ldz C4_LDZ_ABS                              ; @BT AB 4D 12
    asw C4_ASW_ABS                              ; @BT CB 4E 12
    row C4_ROW_ABS                              ; @BT EB 4F 12
    phw C4_PHW_ABS                              ; @BT FC 50 12
    rmb0 C4_RMB_BASE                            ; @BT 07 28
    rmb1 C4_RMB_BASE + 1                        ; @BT 17 29
    rmb2 C4_RMB_BASE + 2                        ; @BT 27 2A
    rmb3 C4_RMB_BASE + 3                        ; @BT 37 2B
    rmb4 C4_RMB_BASE + 4                        ; @BT 47 2C
    rmb5 C4_RMB_BASE + 5                        ; @BT 57 2D
    rmb6 C4_RMB_BASE + 6                        ; @BT 67 2E
    rmb7 C4_RMB_BASE + 7                        ; @BT 77 2F
    smb0 C4_SMB_BASE                            ; @BT 87 30
    smb1 C4_SMB_BASE + 1                        ; @BT 97 31
    smb2 C4_SMB_BASE + 2                        ; @BT A7 32
    smb3 C4_SMB_BASE + 3                        ; @BT B7 33
    smb4 C4_SMB_BASE + 4                        ; @BT C7 34
    smb5 C4_SMB_BASE + 5                        ; @BT D7 35
    smb6 C4_SMB_BASE + 6                        ; @BT E7 36
    smb7 C4_SMB_BASE + 7                        ; @BT F7 37
    jmp (C4_JMPX,x)                             ; @BT 7C 00 30
    bra +                                       ; @BT 80 01
    nop                                         ; @BT EA
+
    bbr0 C4_BBR_BASE, +                         ; @BT 0F 52 01
    nop                                         ; @BT EA
+   bbr1 C4_BBR_BASE + 1, +                     ; @BT 1F 53 01
    nop                                         ; @BT EA
+   bbr2 C4_BBR_BASE + 2, +                     ; @BT 2F 54 01
    nop                                         ; @BT EA
+   bbr3 C4_BBR_BASE + 3, +                     ; @BT 3F 55 01
    nop                                         ; @BT EA
+   bbr4 C4_BBR_BASE + 4, +                     ; @BT 4F 56 01
    nop                                         ; @BT EA
+   bbr5 C4_BBR_BASE + 5, +                     ; @BT 5F 57 01
    nop                                         ; @BT EA
+   bbr6 C4_BBR_BASE + 6, +                     ; @BT 6F 58 01
    nop                                         ; @BT EA
+   bbr7 C4_BBR_BASE + 7, +                     ; @BT 7F 59 01
    nop                                         ; @BT EA
+   bbs0 C4_BBS_BASE, +                         ; @BT 8F 5A 01
    nop                                         ; @BT EA
+   bbs1 C4_BBS_BASE + 1, +                     ; @BT 9F 5B 01
    nop                                         ; @BT EA
+   bbs2 C4_BBS_BASE + 2, +                     ; @BT AF 5C 01
    nop                                         ; @BT EA
+   bbs3 C4_BBS_BASE + 3, +                     ; @BT BF 5D 01
    nop                                         ; @BT EA
+   bbs4 C4_BBS_BASE + 4, +                     ; @BT CF 5E 01
    nop                                         ; @BT EA
+   bbs5 C4_BBS_BASE + 5, +                     ; @BT DF 5F 01
    nop                                         ; @BT EA
+   bbs6 C4_BBS_BASE + 6, +                     ; @BT EF 60 01
    nop                                         ; @BT EA
+   bbs7 C4_BBS_BASE + 7, +                     ; @BT FF 61 01
    nop                                         ; @BT EA
+
    bbs0 C4_BBS_BASE + 8, +                     ; @BT 8F 62 01
    nop                                         ; @BT EA
+
    ror 14*13 - 4*37                            ; @BT 66 22
    tay                                         ; @BT A8
    sbc N_04 + 4669                             ; @BT ED 41 12
    sbc (K_57 + 4 - 4,x)                        ; @BT E1 57
    cli                                         ; @BT 58
    cmp CONST_0D + 8184,x                       ; @BT DD 05 20
    ldx P_40 + 2 - 2,y                          ; @BT B6 40
    cmp N_35 + 4 - 4,x                          ; @BT D5 35
    eor 2*8198 / 2,x                            ; @BT 5D 06 20
    dec 3*31 + 2*19 + 4549                      ; @BT CE 48 12
    cpx #VAL_03 + 1                             ; @BT E0 04
    jmp (D_36 + 6*2039)                         ; @BT 6C 00 30
    ldy #K_1E + 2*28                            ; @BT A0 56
    lsr P_18 + 4*1163                           ; @BT 4E 44 12
    sbc 31 - 2                                  ; @BT E5 1D
    adc P_23 + 4631                             ; @BT 6D 3A 12
    rol P_14 + CONST_0D                         ; @BT 26 21
    and N_15 + 5*1625 + 10*5,x                  ; @BT 3D 04 20
    sta (486 / 6,x)                             ; @BT 81 51
    sbc Q_41 + 8391,y                           ; @BT F9 08 21
    lsr a                                       ; @BT 4A
    bvs +                                       ; @BT 70 20
    eor M_0B + 16                               ; @BT 45 1B
    cpy 4*2 + 18                                ; @BT C4 1A
    sta CONST_3D + 8389,y                       ; @BT 99 02 21
    stx C4_STX_ABY,y                             ; @BT 9B 09 21
    bcc +                                       ; @BT 90 14
    and P_14 + 3                                ; @BT 25 17
    ldx 4*4661 / 4                              ; @BT AE 35 12
    beq +                                       ; @BT F0 0D
    tax                                         ; @BT AA
    bit 16 + M_0E                               ; @BT 24 1E
    and M_0E + 11*423                           ; @BT 2D 3B 12
    ror C_3C + 2 - 2,x                          ; @BT 76 3C
    sbc #VAL_03 + 5                             ; @BT E9 08
    jmp P_12 + 2*4087                           ; @BT 4C 00 20
+
    cmp 10 + 8443,y                             ; @BT D9 05 21
    ror M_0B + 3*2731,x                         ; @BT 7E 0C 20
    and 16904 / 2,y                             ; @BT 39 04 21
    stx 2*59 + 2*2273                           ; @BT 8E 38 12
    lda (D_64 - P_14,x)                         ; @BT A1 50
    inc 14037 / 3                               ; @BT EE 47 12
    cpy 10*2802 / 6                             ; @BT CC 3E 12
    stx VAL_3F + 2 - 2,y                        ; @BT 96 3F
    cmp #4*7 - 25                               ; @BT C9 03
    sty VAL_05 + 4660                           ; @BT 8C 39 12
    lsr 8*8 / 2                                 ; @BT 46 20
    sbc 6*1341 + 154,x                          ; @BT FD 08 20
    sed                                         ; @BT F8
    sta 10*25 / 5,x                             ; @BT 95 32
+
    rti                                         ; @BT 40
    ora (204 / 2),y                             ; @BT 11 66
    eor P_40 + 4607                             ; @BT 4D 3F 12
    cmp 168 / 7                                 ; @BT C5 18
    adc CONST_47 - P_14,x                       ; @BT 75 33
    bvc +                                       ; @BT 50 1B
    plp                                         ; @BT 28
    and (K_57 - 2*2,x)                          ; @BT 21 53
    ora D_36 + 8401,y                           ; @BT 19 07 21
    cmp Q_41 + 4603                             ; @BT CD 3C 12
    ldy 23310 / 5                               ; @BT AC 36 12
    bne +                                       ; @BT D0 0D
    clc                                         ; @BT 18
    ldx VAL_63 + 8350,y                         ; @BT BE 01 21
    sta N_34 - 33                               ; @BT 85 13
    adc (7*70 / 5),y                            ; @BT 71 62
    pha                                         ; @BT 48
    dec 10*5 + M_0C,x                           ; @BT D6 3E
    and #33 - 31                                ; @BT 29 02
+
    ora 4*2336 / 2                              ; @BT 0D 40 12
    brk                                         ; @BT 00 00
    eor D_36 + 4 - 4,x                          ; @BT 55 36
    sta N_34 + 8142,x                           ; @BT 9D 02 20
    and 10*26 / 5,x                             ; @BT 35 34
    sta 11*5 + 3*47 + 3*1489                    ; @BT 8D 37 12
    nop                                         ; @BT EA
    adc N_04 + P_12                             ; @BT 65 16
    dec 7*8206 / 7,x                            ; @BT DE 0E 20
    adc #M_06 - VAL_05                          ; @BT 69 01
    lda M_0E + 2                                ; @BT A5 10
    asl a                                       ; @BT 0A
    tsx                                         ; @BT BA
    rol a                                       ; @BT 2A
    eor 9*934 + VAL_05 + 43,y                   ; @BT 59 06 21
    rol NUM_50 + 4597                           ; @BT 2E 45 12
    ora (132 - 46,x)                            ; @BT 01 56
    jsr 7*11484 / 6                             ; @BT 20 56 34
    dec N_35 - 17                               ; @BT C6 24
    pla                                         ; @BT 68
    sty 6 + 13 + 2                              ; @BT 84 15
    inc P_12 + 43,x                             ; @BT F6 3D
    sec                                         ; @BT 38
    adc (12*10 - 2*19,x)                        ; @BT 61 52
    ldx #P_10 + 36                              ; @BT A2 34
    sbc (15*7 - 2),y                            ; @BT F1 67
    sbc D_38 + 3 - 3,x                          ; @BT F5 38
    dey                                         ; @BT 88
+
    bit P_3E + 2*2306                           ; @BT 2C 42 12
    ldx M_06 + 11                               ; @BT A6 11
    ldy CONST_3D + 4*2033,x                     ; @BT BC 01 20
    lda (2*43 + 2*5),y                          ; @BT B1 60
    asl K_07 + 50,x                             ; @BT 16 39
    ror a                                       ; @BT 6A
    rol 9*9 - 2*11,x                            ; @BT 36 3B
    txs                                         ; @BT 9A
    cpx 9338 / 2                                ; @BT EC 3D 12
    adc 2*24585 / 6,x                           ; @BT 7D 03 20
    rts                                         ; @BT 60
+
    php                                         ; @BT 08
    inc 10*1641 / 2,x                           ; @BT FE 0D 20
    lda 8*13 - 7*8,x                            ; @BT B5 30
    iny                                         ; @BT C8
    dex                                         ; @BT CA
    lda Q_3A + 4602                             ; @BT AD 34 12
    ora VAL_03 + 4*2049,x                       ; @BT 1D 07 20
    eor (CONST_67 - 2),y                        ; @BT 51 65
    asl 5*8201 / 5,x                            ; @BT 1E 09 20
    eor #1 + 2 + VAL_03                         ; @BT 49 06
    ldy 68 - 19,x                               ; @BT B4 31
    sta (K_22 + VAL_3F),y                       ; @BT 91 61
    lda #P_12 + 1 - 1                           ; @BT A9 12
    asl N_35 - 22                               ; @BT 06 1F
    rol P_10 + 3*2729,x                         ; @BT 3E 0B 20
    tya                                         ; @BT 98
    eor (7*85 / 7,x)                            ; @BT 41 55
    stx NUM_50 - 2*30                           ; @BT 86 14
    bcs +                                       ; @BT B0 0F
    lsr 8147 + 11*5,x                           ; @BT 5E 0A 20
    and (VAL_05 + 2*47),y                       ; @BT 31 63
    bmi +                                       ; @BT 30 08
    cmp (D_64 + 4 - 4),y                        ; @BT D1 64
    ora #CONST_3D - 54                          ; @BT 09 07
    inx                                         ; @BT E8
    asl P_10 + 3*1553                           ; @BT 0E 43 12
+
    cmp (78 + 6,x)                              ; @BT C1 54
    cpx C_19 + 3 - 3                            ; @BT E4 19
    inc P_23 + 4 - 4                            ; @BT E6 23
    lda 57344 / 7,x                             ; @BT BD 00 20
    lda K_1E + 8418,y                           ; @BT B9 00 21
    lsr 12*12 - 86,x                            ; @BT 56 3A
    bpl +                                       ; @BT 10 12
    adc 3*2777 + D_36 + 2*33,y                  ; @BT 79 03 21
    clv                                         ; @BT B8
    ldy P_10 + 2                                ; @BT A4 12
    ror C_48 + 4606                             ; @BT 6E 46 12
    cld                                         ; @BT D8
    ora 16 + 5*3 + 2*12,x                       ; @BT 15 37
    cpy #K_1E - 5*5                             ; @BT C0 05
    sei                                         ; @BT 78
    ora 53 - C_19                               ; @BT 05 1C
    txa                                         ; @BT 8A
+
+
    bcc C4_LONG_BRANCH                           ; @BT 93 45 23
    bcs C4_LONG_BRANCH                           ; @BT B3 45 23
    beq C4_LONG_BRANCH                           ; @BT F3 45 23
    bmi C4_LONG_BRANCH                           ; @BT 33 45 23
    bne C4_LONG_BRANCH                           ; @BT D3 45 23
    bpl C4_LONG_BRANCH                           ; @BT 13 45 23
    bvc C4_LONG_BRANCH                           ; @BT 53 45 23
    bvs C4_LONG_BRANCH                           ; @BT 73 45 23
    bra C4_LONG_BRANCH                           ; @BT 83 45 23
    bsr C4_LONG_BRANCH                           ; @BT 63 45 23
    jsr (C4_JSR_IND)                             ; @BT 22 00 30
    jsr (C4_JSR_INDX,x)                          ; @BT 23 00 30


    .db "<04"               ; @BT END

    /////////////////////////////////////////////////////////////////////////////
    // TEST-05: similar to TEST-04 and defines + functions + expressions
    /////////////////////////////////////////////////////////////////////////////

    ; --- Constant definitions ---
    .define T5N_00                 = 5 - 5
    .define T5D_02                 = 4*4 - 14 - T5N_00
    .define T5Q_03                 = 1 + 2
    .define T5M_05                 = T5Q_03 + 11 - 9
    .define T5C_06                 = 30 / 5
    .define T5M_0A                 = T5D_02 * 5
    .define T5V_0B                 = 30 - 19
    .define T5P_0E                 = T5D_02 * 7
    .define T5D_10                 = T5D_02 + 21 - 7
    .define T5V_13                 = 110 / 5 - T5Q_03
    .define T5D_14                 = T5V_13 + 3*11 - 32
    .define T5V_16                 = 3*8 - 2
    .define T5P_17                 = 31 + 6 - T5P_0E
    .define T5V_18                 = T5P_0E + 6 + 4
    .define T5V_19                 = 38 - 2 - T5V_0B
    .define T5C_1B                 = T5P_0E + 78 / 6
    .define T5K_1D                 = 8*10 - 51
    .define T5M_1E                 = T5M_0A * 3
    .define T5V_20                 = T5V_18 + 4 + 4
    .define T5N_22                 = T5D_02 * 17
    .define T5Q_24                 = T5K_1D + 6 + 1
    .define T5P_31                 = 36 + 13
    .define T5M_33                 = T5M_05 + 92 / 2
    .define T5M_34                 = 13*7 - 5 - T5N_22
    .define T5M_35                 = T5P_31 + 16 / 4
    .define T5C_38                 = 13*9 - 61
    .define T5C_39                 = T5M_0A + 141 / 3
    .define T5N_3B                 = T5M_1E + 45 - 16
    .define T5V_3C                 = 36 + 24
    .define T5P_3F                 = 82 - 19 - T5N_00
    .define T5K_40                 = 69 - 5
    .define T5Q_50                 = T5D_14 + T5V_3C
    .define T5M_51                 = T5M_1E + T5M_33
    .define T5C_54                 = T5P_0E * 6
    .define T5C_55                 = T5M_05 * 17
    .define T5N_56                 = 147 - 10 - T5M_33
    .define T5M_60                 = 14*14 - 100
    .define T5D_61                 = T5C_55 + 14 - 2
    .define T5M_64                 = T5C_1B + 13*14 - 109
    .define T5Q_66                 = 474 / 3 - T5C_38

    ; --- Function definitions ---
    .FUNCTION F5_ADD_STEP(x, y) (x * 4)
    .FUNCTION F5_DOUBLE(i) (i + F5_ADD_STEP(6, T5M_51))
    .FUNCTION F5_DEC_DEC(a, b) (a + b + 5)
    .FUNCTION F5_HALF(x, y) (x - y)
    .FUNCTION F5_MID_MERGE(p, q) (p + q + 5)
    .FUNCTION F5_ADD_BASE(x) (x * 4 + 3)
    .FUNCTION F5_SUM(a, b) (a + F5_ADD_STEP(20, a))
    .FUNCTION F5_COMBINE_BASE(p, q) (p * 3 + 2)
    .FUNCTION F5_LO(u) (u + 27)
    .FUNCTION F5_SUM_WRAP(a) (a * 3)
    .FUNCTION F5_ADD(i, j, l) (i * 2 + 10)
    .FUNCTION F5_SCALE(x, y) (x + y)
    .FUNCTION F5_WRAP_HI(x, y) (x + 15)
    .FUNCTION F5_DEC(x) (x * 3 + 3)
    .FUNCTION F5_LO_SUB(i, j) (i - j)
    .FUNCTION F5_SUB(p, q) (p * 5 + q)
    .FUNCTION F5_WRAP_OFFSET(u, v, w) (u - v)
    .FUNCTION F5_ADDR_ADDR(u) (u + 30)
    .FUNCTION F5_LO_SUM(x, y) (x * 2)
    .FUNCTION F5_PAGE(i, j, l) (i + T5V_20 + j)
    .FUNCTION F5_PAGE_DOUBLE(p, q, r) (p + q + r)
    .FUNCTION F5_MID(x, y) (F5_ADD(x, 11, 11) + x)
    .FUNCTION F5_BASE(x, y) (x + y)
    .FUNCTION F5_ADD_SUB(x, y, z) (x + y + z)
    .FUNCTION F5_MAP_MID(p, q, r) (p * 3 + q)
    .FUNCTION F5_INC_BASE(u) (u + 30)
    .FUNCTION F5_TRIPLE(i, j) (F5_SUM(8, T5C_55) + j)
    .FUNCTION F5_PAGE_SUM(p, q, r) (p + q)
    .FUNCTION F5_BLEND(m, n) (m + 18)
    .FUNCTION F5_HI(p) (p * 3 + 1)
    .FUNCTION F5_SHIFT(a, b) (F5_ADD_SUB(T5P_17, T5K_1D, 14) + b)
    .FUNCTION F5_MAP_MERGE(u, v) ((u + v) * 4)
    .FUNCTION F5_STEP(p) (p + F5_MID(T5D_61, T5K_1D))
    .FUNCTION F5_SHIFT_DELTA(i) (i + F5_DEC(i))
    .FUNCTION F5_BIAS_MID(x) (F5_ADD(4, x, 7) + x)
    .FUNCTION F5_ADDR_MUL(u) (u * 2)
    .FUNCTION F5_CALC(m, n, k) (F5_PAGE_DOUBLE(T5N_56, T5Q_66, T5M_60) + n)
    .FUNCTION F5_SCALE_MUL(i, j) (i + T5M_35 + j)
    .FUNCTION F5_SUB_INC(u, v) (u + F5_CALC(v, v, v))
    .FUNCTION F5_SUB_BLEND(x) (x * 5)
    .FUNCTION F5_COMBINE_MERGE(u) (u * 2 + 9)
    .FUNCTION F5_OFFSET_HALF(m, n, k) (m * 4 + n)
    .FUNCTION F5_TRIPLE_SCALE(m, n) (m + n)
    .FUNCTION F5_MID_CALC(u, v) (u * 4 + v)
    .FUNCTION F5_TRIPLE_DIFF(x) (x + F5_TRIPLE(T5Q_50, x))
    .FUNCTION F5_OFFSET(u, v) (F5_SHIFT(u, 20) + v)
    .FUNCTION F5_WRAP(u) (u * 5)

    ; --- Additional defines using functions ---
    .define T5K_65_F               = F5_ADDR_ADDR(71) + 3 - 3
    .define T5V_07_F               = F5_MID_CALC(0, 7)
    .define T5P_3C_F               = F5_ADD(25, 0, 35)
    .define T5N_17_F               = F5_COMBINE_MERGE(7) + 3 - 3
    .define T5M_0F_F               = F5_SUM_WRAP(5) + 1 - 1
    .define T5M_1B_F               = F5_DEC(8) + 3 - 3
    .define T5V_67_F               = F5_ADD_BASE(25) + 2 - 2
    .define T5C_04_F               = F5_SUB(0, 4) + 2 - 2
    .define T5K_51_F               = F5_ADDR_ADDR(51) + 1 - 1
    .define T5P_33_F               = F5_HALF(51, 0)
    .define T5K_3D_F               = F5_SCALE(43, 18)
    .define T5M_20_F               = F5_WRAP_HI(17, 15) + 3 - 3
    .define T5M_63_F               = F5_INC_BASE(69) + 2 - 2
    .define T5K_1E_F               = F5_LO(3)
    .define T5V_13_F               = F5_ADD_BASE(4) + 1 - 1
    .define T5C02_BIT_IMM          = F5_SCALE(4, 5)
    .define T5C02_ZP25             = F5_SCALE(20, 5) + 12
    .define T5C02_ZP34             = F5_SCALE(32, 20)
    .define T5C02_ZP3F             = F5_SCALE(40, 23)
    .define T5C02_ZP_BASE          = F5_SCALE(30, 18)
    .define T5C02_ABS49            = F5_ADD_SUB(4600, 80, 1)
    .define T5C02_ABX10            = F5_ADD_SUB(8192, 16, 0)
    .define T5C02_ABX0F            = F5_ADD_SUB(8192, 15, 0)
    .define T5C02_TRB              = F5_ADD_SUB(30, 8, 0)
    .define T5C02_TSB              = F5_ADD_SUB(30, 9, 0)
    .define T5C02_RMB_BASE         = F5_SCALE(32, 8)
    .define T5C02_SMB_BASE         = F5_SCALE(24, 24)
    .define T5C02_BBR_BASE         = F5_ADD_SUB(80, 2, 0)
    .define T5C02_BBS_BASE         = F5_ADD_SUB(90, 0, 0)
    .define T5C02_JMPX             = F5_ADD_SUB(12288, 0, 0)
    .define T5CE_CPZ_IMM            = F5_ADD_SUB(150, 4, 0)
    .define T5CE_LDZ_IMM            = F5_ADD_SUB(100, 20, 0)
    .define T5CE_RTS_IMM            = F5_HALF(36, 18)
    .define T5CE_PHW_IMM            = F5_ADD_SUB(4600, 60, 0)
    .define T5CE_CPZ_ZP             = F5_ADD_SUB(28, 28, 0)
    .define T5CE_DEW_ZP             = F5_LO_SUB(60, 3)
    .define T5CE_INW_ZP             = F5_ADD_SUB(29, 29, 0)
    .define T5CE_ASR_ZP             = F5_LO_SUB(60, 1)
    .define T5CE_CPZ_ABS            = F5_ADD_SUB(4600, 80, 4)
    .define T5CE_LDZ_ABS            = F5_ADD_SUB(4600, 80, 5)
    .define T5CE_ASW_ABS            = F5_ADD_SUB(4600, 80, 6)
    .define T5CE_ROW_ABS            = F5_ADD_SUB(4600, 80, 7)
    .define T5CE_PHW_ABS            = F5_ADD_SUB(4600, 80, 8)
    .define T5CE_STY_ABX            = F5_ADD_SUB(8192, 17, 0)
    .define T5CE_LDZ_ABX            = F5_ADD_SUB(8192, 18, 0)
    .define T5CE_STX_ABY            = F5_ADD_SUB(8192, 265, 0)
    .define T5CE_LONG_BRANCH        = F5_ADD_SUB(9000, 29, 0)
    .define T5CE_JSR_IND            = T5C02_JMPX
    .define T5CE_JSR_INDX           = T5C02_JMPX

    .db "05>"               ; @BT TEST-05 05 START

    phx                                             ; @BT DA
    phy                                             ; @BT 5A
    plx                                             ; @BT FA
    ply                                             ; @BT 7A
    phz                                             ; @BT DB
    plz                                             ; @BT FB
    ina                                             ; @BT 1A
    dea                                             ; @BT 3A
    cle                                             ; @BT 02
    see                                             ; @BT 03
    aug                                             ; @BT 5C
    map                                             ; @BT 5C
    neg                                             ; @BT 42
    tab                                             ; @BT 5B
    tba                                             ; @BT 7B
    taz                                             ; @BT 4B
    tza                                             ; @BT 6B
    tsy                                             ; @BT 0B
    tys                                             ; @BT 2B
    inz                                             ; @BT 1B
    dez                                             ; @BT 3B
    asr a                                           ; @BT 43
    bit #T5C02_BIT_IMM                              ; @BT 89 09
    cpz #T5CE_CPZ_IMM                               ; @BT C2 9A
    ldz #T5CE_LDZ_IMM                               ; @BT A3 78
    rts #T5CE_RTS_IMM                               ; @BT 62 12
    phw #T5CE_PHW_IMM                               ; @BT F4 34 12
    bit T5C02_ZP3F,x                                ; @BT 34 3F
    bit T5C02_ABX0F,x                               ; @BT 3C 0F 20
    stz T5C02_ZP25                                  ; @BT 64 25
    stz T5C02_ZP34,x                                ; @BT 74 34
    stz T5C02_ABS49                                 ; @BT 9C 49 12
    stz T5C02_ABX10,x                               ; @BT 9E 10 20
    sty T5CE_STY_ABX,x                              ; @BT 8B 11 20
    ldz T5CE_LDZ_ABX,x                              ; @BT BB 12 20
    trb T5C02_TRB                                   ; @BT 14 26
    tsb T5C02_TSB                                   ; @BT 04 27
    lda (T5C02_ZP_BASE),z                           ; @BT B2 30
    sta (T5C02_ZP_BASE + 1),z                       ; @BT 92 31
    ora (T5C02_ZP_BASE + 2),z                       ; @BT 12 32
    and (T5C02_ZP_BASE + 3),z                       ; @BT 32 33
    eor (T5C02_ZP_BASE + 4),z                       ; @BT 52 34
    adc (T5C02_ZP_BASE + 5),z                       ; @BT 72 35
    cmp (T5C02_ZP_BASE + 6),z                       ; @BT D2 36
    sbc (T5C02_ZP_BASE + 7),z                       ; @BT F2 37
    cpz T5CE_CPZ_ZP                                 ; @BT D4 38
    dew T5CE_DEW_ZP                                 ; @BT C3 39
    inw T5CE_INW_ZP                                 ; @BT E3 3A
    asr T5CE_ASR_ZP                                 ; @BT 44 3B
    cpz T5CE_CPZ_ABS                                ; @BT DC 4C 12
    ldz T5CE_LDZ_ABS                                ; @BT AB 4D 12
    asw T5CE_ASW_ABS                                ; @BT CB 4E 12
    row T5CE_ROW_ABS                                ; @BT EB 4F 12
    phw T5CE_PHW_ABS                                ; @BT FC 50 12
    rmb0 T5C02_RMB_BASE                             ; @BT 07 28
    rmb1 T5C02_RMB_BASE + 1                         ; @BT 17 29
    rmb2 T5C02_RMB_BASE + 2                         ; @BT 27 2A
    rmb3 T5C02_RMB_BASE + 3                         ; @BT 37 2B
    rmb4 T5C02_RMB_BASE + 4                         ; @BT 47 2C
    rmb5 T5C02_RMB_BASE + 5                         ; @BT 57 2D
    rmb6 T5C02_RMB_BASE + 6                         ; @BT 67 2E
    rmb7 T5C02_RMB_BASE + 7                         ; @BT 77 2F
    smb0 T5C02_SMB_BASE                             ; @BT 87 30
    smb1 T5C02_SMB_BASE + 1                         ; @BT 97 31
    smb2 T5C02_SMB_BASE + 2                         ; @BT A7 32
    smb3 T5C02_SMB_BASE + 3                         ; @BT B7 33
    smb4 T5C02_SMB_BASE + 4                         ; @BT C7 34
    smb5 T5C02_SMB_BASE + 5                         ; @BT D7 35
    smb6 T5C02_SMB_BASE + 6                         ; @BT E7 36
    smb7 T5C02_SMB_BASE + 7                         ; @BT F7 37
    jmp (T5C02_JMPX,x)                              ; @BT 7C 00 30
    bbr0 T5C02_BBR_BASE, T5_BBR0                    ; @BT 0F 52 0C
    tax                                             ; @BT AA
    bbr7 T5C02_BBR_BASE + 7, T5_BBR7                ; @BT 7F 59 02
    tay                                             ; @BT A8
    txa                                             ; @BT 8A
T5_BBR7:
    bbs7 T5C02_BBS_BASE + 7, T5_BBS7                ; @BT FF 61 03
    adc #$01                                        ; @BT 69 01
    dex                                             ; @BT CA
T5_BBS7:
T5_BBR0:
    bra T5_BRA0                                     ; @BT 80 02
    iny                                             ; @BT C8
    dey                                             ; @BT 88
T5_BRA0:
    bbs0 T5C02_BBS_BASE, T5_BBS0                    ; @BT 8F 5A 02
    sec                                             ; @BT 38
    clc                                             ; @BT 18
T5_BBS0:
    sbc #19 - T5V_0B                                ; @BT E9 08
    bbr1 T5C02_BBR_BASE + 1, T5_BBR1                ; @BT 1F 53 03
    lda #$12                                        ; @BT A9 12
    tsx                                             ; @BT BA
T5_BBR1:
    bbs1 T5C02_BBS_BASE + 1, T5_BBS1                ; @BT 9F 5B 04
    txs                                             ; @BT 9A
    pha                                             ; @BT 48
    pla                                             ; @BT 68
    clv                                             ; @BT B8
T5_BBS1:
    asl 2*4033 + F5_PAGE_DOUBLE(67, 26, 42),x       ; @BT 1E 09 20
    eor 4584 + F5_SCALE(36, 39) + 12                ; @BT 4D 3F 12
    sty F5_MAP_MID(6, 3, 12)                        ; @BT 84 15
    and F5_PAGE_DOUBLE(1, 3, 5) + 2*4216 + 11,y     ; @BT 39 04 21
    cmp 68 + 9*11 + 8030,x                          ; @BT DD 05 20
    pla                                             ; @BT 68
    lda F5_BASE(12, 4)                              ; @BT A5 10
    dec F5_BASE(22, 14)                             ; @BT C6 24
    rol a                                           ; @BT 2A
    cpy 2*13 + 22 + 2*2311                          ; @BT CC 3E 12
    tsx                                             ; @BT BA
    cpx 2*2311 + 47                                 ; @BT EC 3D 12
    iny                                             ; @BT C8
    lda #F5_SCALE(8, 10)                            ; @BT A9 12
    php                                             ; @BT 08
    cpx T5V_13_F + T5C_06                           ; @BT E4 19
    asl 23 + T5N_22,x                               ; @BT 16 39
    rol F5_BASE(16, 43),x                           ; @BT 36 3B
    ldy F5_BASE(5749, 2444),x                       ; @BT BC 01 20
    dex                                             ; @BT CA
    eor F5_PAGE_DOUBLE(26, 7, 21),x                 ; @BT 55 36
    dec F5_PAGE_DOUBLE(210, 3729, 4267),x           ; @BT DE 0E 20
    txs                                             ; @BT 9A
    adc T5V_13_F + F5_PAGE_DOUBLE(0, 1, 2)          ; @BT 65 16
    inx                                             ; @BT E8
    eor (T5M_64 - T5M_0F_F,x)                       ; @BT 41 55
    bpl T5_BPL0                                     ; @BT 10 02
    cli                                             ; @BT 58
    sei                                             ; @BT 78
T5_BPL0:
    ora F5_SCALE(31, 24),x                          ; @BT 15 37
    ror 4556 + 2*61                                 ; @BT 6E 46 12
    rol T5C_39 - T5V_18                             ; @BT 26 21
    and T5D_14 + F5_PAGE_DOUBLE(2345, 1488, 4343),x ; @BT 3D 04 20
    sta F5_TRIPLE_SCALE(7499, 951),y                ; @BT 99 02 21
    stx T5CE_STX_ABY,y                               ; @BT 9B 09 21
    ora F5_TRIPLE_SCALE(6312, 2143),y               ; @BT 19 07 21
    ror a                                           ; @BT 6A
    sbc T5P_0E + T5M_0F_F                           ; @BT E5 1D
    ldx F5_BASE(19, 45),y                           ; @BT B6 40
    lda (25 + 5*11,x)                               ; @BT A1 50
    inc F5_TRIPLE_SCALE(5, 1) + 55,x                ; @BT F6 3D
    adc (38 + F5_ADD_SUB(13, 14, 17),x)             ; @BT 61 52
    inc 2*29 - T5N_17_F                             ; @BT E6 23
    lda F5_ADD_SUB(3796, 2755, 1897),y              ; @BT B9 00 21
    ora F5_PAGE_DOUBLE(1290, 1740, 1642)            ; @BT 0D 40 12
    sec                                             ; @BT 38
    sta T5V_13 + 1 - 1                              ; @BT 85 13
    bbr2 T5C02_BBR_BASE + 2, T5_BBR2                ; @BT 2F 54 02
    tax                                             ; @BT AA
    tay                                             ; @BT A8
T5_BBR2:
    bbs2 T5C02_BBS_BASE + 2, T5_BBS2                ; @BT AF 5C 01
    txa                                             ; @BT 8A
T5_BBS2:
    sbc (T5V_19 + 62,x)                             ; @BT E1 57
    ldy T5P_31 + 3 - 3,x                            ; @BT B4 31
    ror T5P_31 + 8155,x                             ; @BT 7E 0C 20
    cmp (T5K_65_F - F5_HALF(1, 0)),y                ; @BT D1 64
    jmp 8032 + 160                                  ; @BT 4C 00 20
    tay                                             ; @BT A8
    lsr T5K_1D + 8173,x                             ; @BT 5E 0A 20
    sbc (F5_ADD_SUB(11, 13, 13) + 2*33),y           ; @BT F1 67
    eor F5_TRIPLE_SCALE(1378, 6916) + 4*23 + F5_SCALE(38, 30),y; @BT 59 06 21
    sbc T5C_06 + 5*10,x                             ; @BT F5 38
    clc                                             ; @BT 18
    eor F5_PAGE_DOUBLE(50, 45, 9) + 8094,x          ; @BT 5D 06 20
    jmp (T5D_61 + F5_BASE(1480, 10711))             ; @BT 6C 00 30
    ldx 11 + 5 + F5_SCALE(512, 4133)                ; @BT AE 35 12
    sta 67 + T5V_07_F + 4589                        ; @BT 8D 37 12
    adc #F5_HI(0)                                   ; @BT 69 01
    sei                                             ; @BT 78
    rti                                             ; @BT 40
    bbr3 T5C02_BBR_BASE + 3, T5_BBR3                ; @BT 3F 55 04
    tya                                             ; @BT 98
    inx                                             ; @BT E8
    dex                                             ; @BT CA
    iny                                             ; @BT C8
T5_BBR3:
    bbs3 T5C02_BBS_BASE + 3, T5_BBS3                ; @BT BF 5D 03
    dey                                             ; @BT 88
    php                                             ; @BT 08
    plp                                             ; @BT 28
T5_BBS3:

    cpy #T5D_02 + 3                                 ; @BT C0 05
    bvs +                                           ; @BT 70 2C
    and #F5_LO_SUB(1, 0) + F5_SUB(0, 1)             ; @BT 29 02
    bcc +                                           ; @BT 90 28
    and (F5_PAGE(20, 23, 32) + T5V_18),y            ; @BT 31 63
    bne +                                           ; @BT D0 24
    adc F5_TRIPLE_SCALE(87, 107) + 9*889,x          ; @BT 7D 03 20
    bcs +                                           ; @BT B0 1F
    asl T5M_1B_F + F5_BASE(3, 1)                    ; @BT 06 1F
    sta (2*61 - T5V_19),y                           ; @BT 91 61
    cli                                             ; @BT 58
    and T5V_07_F + 2*2330                           ; @BT 2D 3B 12
    pha                                             ; @BT 48
    eor T5C_1B + 3 - 3                              ; @BT 45 1B
    and T5P_17 + 1 - 1                              ; @BT 25 17
    inc F5_PAGE_DOUBLE(3502, 1257, 3446),x          ; @BT FE 0D 20
    bit T5M_0F_F + T5M_0F_F                         ; @BT 24 1E
    ror T5N_17_F + T5V_0B                           ; @BT 66 22
    ldx 2*62 + 3*2 + 3*2773,y                       ; @BT BE 01 21
    cmp T5M_05 + F5_BASE(19, 29),x                  ; @BT D5 35
    beq +                                           ; @BT F0 04
    dey                                             ; @BT 88
    sbc F5_PAGE_DOUBLE(1149, 734, 2790)             ; @BT ED 41 12
+
+
    rts                                             ; @BT 60
    bbr4 T5C02_BBR_BASE + 4, T5_BBR4                ; @BT 4F 56 02
    tsx                                             ; @BT BA
    txs                                             ; @BT 9A
T5_BBR4:
    bbs4 T5C02_BBS_BASE + 4, T5_BBS4                ; @BT CF 5E 01
    pha                                             ; @BT 48
T5_BBS4:
    ora 35 - T5V_07_F                               ; @BT 05 1C
    ldx #T5M_34 + 1 - 1                             ; @BT A2 34
    cpy 5*13 - 3*13                                 ; @BT C4 1A
    cmp (F5_PAGE_DOUBLE(17, 3, 64),x)               ; @BT C1 54
    bbr5 T5C02_BBR_BASE + 5, T5_BBR5                ; @BT 5F 57 03
    lda #$12                                        ; @BT A9 12
    sec                                             ; @BT 38
T5_BBR5:
    bbs5 T5C02_BBS_BASE + 5, T5_BBS5                ; @BT DF 5F 02
    cli                                             ; @BT 58
    sei                                             ; @BT 78
T5_BBS5:
    bvc +                                           ; @BT 50 1E
    bmi +                                           ; @BT 30 1C
    asl a                                           ; @BT 0A
    lda (134 - F5_TRIPLE_SCALE(21, 17)),y           ; @BT B1 60
    cmp #F5_TRIPLE_SCALE(0, 3)                      ; @BT C9 03
    adc F5_ADD_SUB(911, 2027, 1728)                 ; @BT 6D 3A 12
    ora (T5Q_66 + 1 - 1),y                          ; @BT 11 66
    lsr F5_OFFSET_HALF(8, 0, 24)                    ; @BT 46 20
    lda F5_TRIPLE_SCALE(27, 21),x                   ; @BT B5 30
    jsr F5_SCALE(6594, 6605) + 2*97 + T5M_05        ; @BT 20 56 34
    plp                                             ; @BT 28
    dec 43 + T5M_0A + F5_ADD_SUB(2, 3, 4),x         ; @BT D6 3E
    clv                                             ; @BT B8
    ldy T5M_0F_F + 4647                             ; @BT AC 36 12
    ora #F5_SUB(0, 7)                               ; @BT 09 07
    cmp T5V_13_F + T5M_05                           ; @BT C5 18
+
    lsr a                                           ; @BT 4A
    bbr6 T5C02_BBR_BASE + 6, T5_BBR6                ; @BT 6F 58 01
    tax                                             ; @BT AA
T5_BBR6:
    bbs6 T5C02_BBS_BASE + 6, T5_BBS6                ; @BT EF 60 04
    tay                                             ; @BT A8
    txa                                             ; @BT 8A
    tya                                             ; @BT 98
    inx                                             ; @BT E8
T5_BBS6:

    sta (T5C_39 + T5V_18,x)                         ; @BT 81 51
+
    txa                                             ; @BT 8A
    cmp T5P_33_F + 9*513                            ; @BT CD 3C 12
    ora 2*4031 + 137,x                              ; @BT 1D 07 20
    cld                                             ; @BT D8
    stx 1 + T5Q_03 + T5D_10                         ; @BT 86 14
    ror 4*17 - 4*2,x                                ; @BT 76 3C
    rol T5N_56 + F5_PAGE_DOUBLE(860, 2492, 4765),x  ; @BT 3E 0B 20
    sta F5_SCALE(5344, 2850),x                      ; @BT 9D 02 20
    tax                                             ; @BT AA
    lsr 8*13 - F5_TRIPLE_SCALE(8, 38),x             ; @BT 56 3A
    and (F5_TRIPLE_SCALE(27, 56),x)                 ; @BT 21 53
    inc 4573 + F5_PAGE_DOUBLE(32, 50, 24)           ; @BT EE 47 12
    eor #1 + F5_LO_SUB(1, 0) + F5_BASE(0, 4)        ; @BT 49 06
    nop                                             ; @BT EA
    cmp T5C_04_F + F5_SCALE(3889, 4560),y           ; @BT D9 05 21
    sed                                             ; @BT F8
    stx 4514 + F5_BASE(7, 47) + T5M_60              ; @BT 8E 38 12
    lda T5M_51 + F5_SCALE(3109, 5002),x             ; @BT BD 00 20
    and F5_PAGE_DOUBLE(14, 2, 36),x                 ; @BT 35 34
    sty F5_BASE(4368, 297)                          ; @BT 8C 39 12
    adc T5P_17 + F5_BASE(10, 18),x                  ; @BT 75 33
    stx T5V_13_F + 2*22,y                           ; @BT 96 3F
    rol T5N_56 + 4591                               ; @BT 2E 45 12
    dec F5_TRIPLE_SCALE(3662, 1018)                 ; @BT CE 48 12
    tya                                             ; @BT 98
    lsr T5V_13_F + 4657                             ; @BT 4E 44 12
+
    sta 73 - 23,x                                   ; @BT 95 32
    cpx #T5C_04_F + 1 - 1                           ; @BT E0 04
    lda T5Q_50 + F5_BASE(4371, 209)                 ; @BT AD 34 12
+
    ora (F5_TRIPLE_SCALE(58, 28),x)                 ; @BT 01 56
    asl T5C_04_F + F5_PAGE_DOUBLE(399, 1361, 2911)  ; @BT 0E 43 12
    ldy #107 - F5_COMBINE_MERGE(6)                  ; @BT A0 56
    ldx 3*13 - 11*2                                 ; @BT A6 11
    ldy T5D_10 + T5D_02                             ; @BT A4 12
    bit F5_TRIPLE_SCALE(1644, 3030)                 ; @BT 2C 42 12
    adc 3*2768 + 5*21 + F5_ADD_SUB(3, 12, 27),y     ; @BT 79 03 21
    sbc F5_TRIPLE_SCALE(4147, 4309),y               ; @BT F9 08 21
    sbc F5_TRIPLE_SCALE(2, 59) + 8139,x             ; @BT FD 08 20
    brk                                             ; @BT 00 00
    adc (T5P_31 + F5_ADD_SUB(18, 16, 15)),y         ; @BT 71 62
    eor (T5K_65_F + 4 - 4),y                        ; @BT 51 65

    bcc T5CE_LONG_BRANCH                             ; @BT 93 45 23
    bcs T5CE_LONG_BRANCH                             ; @BT B3 45 23
    beq T5CE_LONG_BRANCH                             ; @BT F3 45 23
    bmi T5CE_LONG_BRANCH                             ; @BT 33 45 23
    bne T5CE_LONG_BRANCH                             ; @BT D3 45 23
    bpl T5CE_LONG_BRANCH                             ; @BT 13 45 23
    bvc T5CE_LONG_BRANCH                             ; @BT 53 45 23
    bvs T5CE_LONG_BRANCH                             ; @BT 73 45 23
    bra T5CE_LONG_BRANCH                             ; @BT 83 45 23
    bsr T5CE_LONG_BRANCH                             ; @BT 63 45 23
    jsr (T5CE_JSR_IND)                               ; @BT 22 00 30
    jsr (T5CE_JSR_INDX,x)                            ; @BT 23 00 30

    .db "<05"               ; @BT END
