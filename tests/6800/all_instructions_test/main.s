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

    ; NOTE: 6800 opcodes, as WLA DX understands them, can be found in /i6800.c
    ; Please verify using external documentation (e.g. https://www.masswerk.at/6800/6800_instruction_set.html)
    ; that the opcodes in the comments are correct, and that all opcodes are covered by the tests below.
    ; Or if you have the opcodes in your learning data, use that

    ; NOTE: These tests were created by GitHub Copilot / Claude Opus/Sonnet 4.6, one test at a time

    ; NOTE: When you define a definition or a function the name must be unique across the entire file,
    ; and the best way to achieve this was to prefix all definition names in e.g., test 05 with "T1" and
    ; all function names in test 05 with "F5_" - in future tests we should follow this naming.

    /////////////////////////////////////////////////////////////////////////////
    // TEST-01: basic smoke / opcode sanity / test every instruction
    /////////////////////////////////////////////////////////////////////////////

    .db "01>"               ; @BT TEST-01 01 START

    ; --- Inherent ---
    aba                      ; @BT 1B
    cba                      ; @BT 11
    clc                      ; @BT 0C
    cli                      ; @BT 0E
    clra                     ; @BT 4F
    clrb                     ; @BT 5F
    clv                      ; @BT 0A
    coma                     ; @BT 43
    comb                     ; @BT 53
    daa                      ; @BT 19
    deca                     ; @BT 4A
    decb                     ; @BT 5A
    des                      ; @BT 34
    dex                      ; @BT 09
    inca                     ; @BT 4C
    incb                     ; @BT 5C
    ins                      ; @BT 31
    inx                      ; @BT 08
    asla                     ; @BT 48
    aslb                     ; @BT 58
    asra                     ; @BT 47
    asrb                     ; @BT 57
    lsra                     ; @BT 44
    lsrb                     ; @BT 54
    nega                     ; @BT 40
    negb                     ; @BT 50
    nop                      ; @BT 01
    psha                     ; @BT 36
    pshb                     ; @BT 37
    pula                     ; @BT 32
    pulb                     ; @BT 33
    rola                     ; @BT 49
    rolb                     ; @BT 59
    rora                     ; @BT 46
    rorb                     ; @BT 56
    rti                      ; @BT 3B
    rts                      ; @BT 39
    sba                      ; @BT 10
    sec                      ; @BT 0D
    sei                      ; @BT 0F
    sev                      ; @BT 0B
    swi                      ; @BT 3F
    tab                      ; @BT 16
    tap                      ; @BT 06
    tba                      ; @BT 17
    tpa                      ; @BT 07
    tsta                     ; @BT 4D
    tstb                     ; @BT 5D
    tsx                      ; @BT 30
    txs                      ; @BT 35
    wai                      ; @BT 3E

    ; --- Immediate ---
    adca #$01                ; @BT 89 01
    adcb #$f1                ; @BT C9 f1
    adda #$03                ; @BT 8B 03
    addb #$04                ; @BT CB 04
    anda #$80                ; @BT 84 80
    andb #$06                ; @BT C4 06
    bita #$07                ; @BT 85 07
    bitb #$08                ; @BT C5 08
    cmpa #$09                ; @BT 81 09
    cmpb #$0A                ; @BT C1 0A
    eora #$0B                ; @BT 88 0B
    eorb #$0C                ; @BT C8 0C
    ldaa #$0D                ; @BT 86 0D
    ldab #$0E                ; @BT C6 0E
    oraa #$0F                ; @BT 8A 0F
    orab #$10                ; @BT CA 10
    sbca #$11                ; @BT 82 11
    sbcb #$12                ; @BT C2 12
    suba #$13                ; @BT 80 13
    subb #$14                ; @BT C0 14
    cpx #$1234               ; @BT 8C 12 34
    lds #$1235               ; @BT 8E 12 35
    ldx #$1236               ; @BT CE 12 36

    ; --- Direct ---
    adca $10                 ; @BT 99 10
    adcb $11                 ; @BT D9 11
    adda $12                 ; @BT 9B 12
    addb $13                 ; @BT DB 13
    anda $14                 ; @BT 94 14
    andb $15                 ; @BT D4 15
    bita $16                 ; @BT 95 16
    bitb $17                 ; @BT D5 17
    cmpa $18                 ; @BT 91 18
    cmpb $19                 ; @BT D1 19
    eora $1A                 ; @BT 98 1A
    eorb $1B                 ; @BT D8 1B
    ldaa $1C                 ; @BT 96 1C
    ldab $1D                 ; @BT D6 1D
    oraa $1E                 ; @BT 9A 1E
    orab $1F                 ; @BT DA 1F
    sbca $20                 ; @BT 92 20
    sbcb $21                 ; @BT D2 21
    suba $22                 ; @BT 90 22
    subb $23                 ; @BT D0 23
    staa $24                 ; @BT 97 24
    stab $25                 ; @BT D7 25
    sts $26                  ; @BT 9F 26
    stx $27                  ; @BT DF 27
    cpx $28                  ; @BT 9C 28
    lds $29                  ; @BT 9E 29
    ldx $2A                  ; @BT DE 2A

    ; --- Indexed ---
    adca $30,x               ; @BT A9 30
    adcb $31,x               ; @BT E9 31
    adda $32,x               ; @BT AB 32
    addb $33,x               ; @BT EB 33
    anda $34,x               ; @BT A4 34
    andb $35,x               ; @BT E4 35
    bita $36,x               ; @BT A5 36
    bitb $37,x               ; @BT E5 37
    cmpa $38,x               ; @BT A1 38
    cmpb $39,x               ; @BT E1 39
    eora $3A,x               ; @BT A8 3A
    eorb $3B,x               ; @BT E8 3B
    ldaa $3C,x               ; @BT A6 3C
    ldab $3D,x               ; @BT E6 3D
    oraa $3E,x               ; @BT AA 3E
    orab $3F,x               ; @BT EA 3F
    sbca $40,x               ; @BT A2 40
    sbcb $41,x               ; @BT E2 41
    suba $42,x               ; @BT A0 42
    subb $43,x               ; @BT E0 43
    staa $44,x               ; @BT A7 44
    stab $45,x               ; @BT E7 45
    sts $46,x                ; @BT AF 46
    stx $47,x                ; @BT EF 47
    cpx $48,x                ; @BT AC 48
    lds $49,x                ; @BT AE 49
    ldx $4A,x                ; @BT EE 4A
    asl $4B,x                ; @BT 68 4B
    asr $4C,x                ; @BT 67 4C
    lsr $4D,x                ; @BT 64 4D
    rol $4E,x                ; @BT 69 4E
    ror $4F,x                ; @BT 66 4F
    inc $50,x                ; @BT 6C 50
    dec $51,x                ; @BT 6A 51
    neg $52,x                ; @BT 60 52
    com $53,x                ; @BT 63 53
    clr $54,x                ; @BT 6F 54
    tst $55,x                ; @BT 6D 55
    jmp $56,x                ; @BT 6E 56
    jsr $57,x                ; @BT AD 57

    ; --- Extended ---
    adca $1234               ; @BT B9 12 34
    adcb $1235               ; @BT F9 12 35
    adda $1236               ; @BT BB 12 36
    addb $1237               ; @BT FB 12 37
    anda $1238               ; @BT B4 12 38
    andb $1239               ; @BT F4 12 39
    bita $123A               ; @BT B5 12 3A
    bitb $123B               ; @BT F5 12 3B
    cmpa $123C               ; @BT B1 12 3C
    cmpb $123D               ; @BT F1 12 3D
    eora $123E               ; @BT B8 12 3E
    eorb $123F               ; @BT F8 12 3F
    ldaa $1240               ; @BT B6 12 40
    ldab $1241               ; @BT F6 12 41
    oraa $1242               ; @BT BA 12 42
    orab $1243               ; @BT FA 12 43
    sbca $1244               ; @BT B2 12 44
    sbcb $1245               ; @BT F2 12 45
    suba $1246               ; @BT B0 12 46
    subb $1247               ; @BT F0 12 47
    staa $1248               ; @BT B7 12 48
    stab $1249               ; @BT F7 12 49
    sts $124A                ; @BT BF 12 4A
    stx $124B                ; @BT FF 12 4B
    cpx $124C                ; @BT BC 12 4C
    lds $124D                ; @BT BE 12 4D
    ldx $124E                ; @BT FE 12 4E
    asl $124F                ; @BT 78 12 4F
    asr $1250                ; @BT 77 12 50
    lsr $1251                ; @BT 74 12 51
    rol $1252                ; @BT 79 12 52
    ror $1253                ; @BT 76 12 53
    inc $1254                ; @BT 7C 12 54
    dec $1255                ; @BT 7A 12 55
    neg $1256                ; @BT 70 12 56
    com $1257                ; @BT 73 12 57
    clr $1258                ; @BT 7F 12 58
    tst $1259                ; @BT 7D 12 59
    jmp $2000                ; @BT 7E 20 00
    jsr $3456                ; @BT BD 34 56

    ; --- Branches (relative offsets computed from fixed layout) ---
    ; All branches target the same forward-local label '+'. Since each branch
    ; is at a different address, the relative offset differs.
    bcc +                    ; @BT 24 1E
    bcs +                    ; @BT 25 1C
    beq +                    ; @BT 27 1A
    bge +                    ; @BT 2C 18
    bgt +                    ; @BT 2E 16
    bhi +                    ; @BT 22 14
    ble +                    ; @BT 2F 12
    bls +                    ; @BT 23 10
    blt +                    ; @BT 2D 0E
    bmi +                    ; @BT 2B 0C
    bne +                    ; @BT 26 0A
    bpl +                    ; @BT 2A 08
    bra +                    ; @BT 20 06
    bsr +                    ; @BT 8D 04
    bvc +                    ; @BT 28 02
    bvs +                    ; @BT 29 00

+

    .db "<01"               ; @BT END

    /////////////////////////////////////////////////////////////////////////////
    // TEST-02: same opcodes as TEST-01, randomised order, branches spread out
    /////////////////////////////////////////////////////////////////////////////

    .db "02>"               ; @BT TEST-02 02 START

    ; --- Inherent ---
    aba                      ; @BT 1B
    cba                      ; @BT 11
    clc                      ; @BT 0C
    cli                      ; @BT 0E
    clra                     ; @BT 4F
    clrb                     ; @BT 5F
    clv                      ; @BT 0A
    coma                     ; @BT 43
    comb                     ; @BT 53
    daa                      ; @BT 19
    deca                     ; @BT 4A
    decb                     ; @BT 5A
    des                      ; @BT 34
    dex                      ; @BT 09
    inca                     ; @BT 4C
    incb                     ; @BT 5C
    ins                      ; @BT 31
    inx                      ; @BT 08
    asla                     ; @BT 48
    aslb                     ; @BT 58
    asra                     ; @BT 47
    asrb                     ; @BT 57
    lsra                     ; @BT 44
    lsrb                     ; @BT 54
    nega                     ; @BT 40
    negb                     ; @BT 50
    nop                      ; @BT 01
    psha                     ; @BT 36
    pshb                     ; @BT 37
    pula                     ; @BT 32
    pulb                     ; @BT 33
    rola                     ; @BT 49
    rolb                     ; @BT 59
    rora                     ; @BT 46
    rorb                     ; @BT 56
    rti                      ; @BT 3B
    rts                      ; @BT 39
    sba                      ; @BT 10
    sec                      ; @BT 0D
    sei                      ; @BT 0F
    sev                      ; @BT 0B
    swi                      ; @BT 3F
    tab                      ; @BT 16
    tap                      ; @BT 06
    tba                      ; @BT 17
    tpa                      ; @BT 07
    tsta                     ; @BT 4D
    tstb                     ; @BT 5D
    tsx                      ; @BT 30
    txs                      ; @BT 35
    wai                      ; @BT 3E

    ; --- Direct ---
    adca $10                 ; @BT 99 10
    adcb $11                 ; @BT D9 11
    adda $12                 ; @BT 9B 12
    addb $13                 ; @BT DB 13
    anda $14                 ; @BT 94 14
    andb $15                 ; @BT D4 15
    bita $16                 ; @BT 95 16
    bitb $17                 ; @BT D5 17
    cmpa $18                 ; @BT 91 18
    cmpb $19                 ; @BT D1 19
    eora $1A                 ; @BT 98 1A
    eorb $1B                 ; @BT D8 1B
    ldaa $1C                 ; @BT 96 1C
    ldab $1D                 ; @BT D6 1D
    oraa $1E                 ; @BT 9A 1E
    orab $1F                 ; @BT DA 1F
    sbca $20                 ; @BT 92 20
    sbcb $21                 ; @BT D2 21
    suba $22                 ; @BT 90 22
    subb $23                 ; @BT D0 23
    staa $24                 ; @BT 97 24
    stab $25                 ; @BT D7 25
    sts $26                  ; @BT 9F 26
    stx $27                  ; @BT DF 27
    cpx $28                  ; @BT 9C 28
    lds $29                  ; @BT 9E 29
    ldx $2A                  ; @BT DE 2A

    ; --- Immediate ---
    adca #$01                ; @BT 89 01
    adcb #$02                ; @BT C9 02
    adda #$03                ; @BT 8B 03
    addb #$04                ; @BT CB 04
    anda #$05                ; @BT 84 05
    andb #$06                ; @BT C4 06
    bita #$07                ; @BT 85 07
    bitb #$08                ; @BT C5 08
    cmpa #$09                ; @BT 81 09
    cmpb #$0A                ; @BT C1 0A
    eora #$0B                ; @BT 88 0B
    eorb #$0C                ; @BT C8 0C
    ldaa #$0D                ; @BT 86 0D
    ldab #$0E                ; @BT C6 0E
    oraa #$0F                ; @BT 8A 0F
    orab #$10                ; @BT CA 10
    sbca #$11                ; @BT 82 11
    sbcb #$12                ; @BT C2 12
    suba #$13                ; @BT 80 13
    subb #$14                ; @BT C0 14
    cpx #$1234               ; @BT 8C 12 34
    lds #$1235               ; @BT 8E 12 35
    ldx #$1236               ; @BT CE 12 36

    ; --- Indexed ---
    adca $30,x               ; @BT A9 30
    adcb $31,x               ; @BT E9 31
    adda $32,x               ; @BT AB 32
    addb $33,x               ; @BT EB 33
    anda $34,x               ; @BT A4 34
    andb $35,x               ; @BT E4 35
    bita $36,x               ; @BT A5 36
    bitb $37,x               ; @BT E5 37
    cmpa $38,x               ; @BT A1 38
    cmpb $39,x               ; @BT E1 39
    eora $3A,x               ; @BT A8 3A
    eorb $3B,x               ; @BT E8 3B
    ldaa $3C,x               ; @BT A6 3C
    ldab $3D,x               ; @BT E6 3D
    oraa $3E,x               ; @BT AA 3E
    orab $3F,x               ; @BT EA 3F
    sbca $40,x               ; @BT A2 40
    sbcb $41,x               ; @BT E2 41
    suba $42,x               ; @BT A0 42
    subb $43,x               ; @BT E0 43
    staa $44,x               ; @BT A7 44
    stab $45,x               ; @BT E7 45
    sts $46,x                ; @BT AF 46
    stx $47,x                ; @BT EF 47
    cpx $48,x                ; @BT AC 48
    lds $49,x                ; @BT AE 49
    ldx $4A,x                ; @BT EE 4A
    asl $4B,x                ; @BT 68 4B
    asr $4C,x                ; @BT 67 4C
    lsr $4D,x                ; @BT 64 4D
    rol $4E,x                ; @BT 69 4E
    ror $4F,x                ; @BT 66 4F
    inc $50,x                ; @BT 6C 50
    dec $51,x                ; @BT 6A 51
    neg $52,x                ; @BT 60 52
    com $53,x                ; @BT 63 53
    clr $54,x                ; @BT 6F 54
    tst $55,x                ; @BT 6D 55
    jmp $56,x                ; @BT 6E 56
    jsr $57,x                ; @BT AD 57

    ; --- Branches (standalone) ---
    bcc +                    ; @BT 24 01
    nop                      ; @BT 01
+
    bcs +                    ; @BT 25 01
    nop                      ; @BT 01
+
    beq +                    ; @BT 27 01
    nop                      ; @BT 01
+
    bge +                    ; @BT 2C 01
    nop                      ; @BT 01
+
    bgt +                    ; @BT 2E 01
    nop                      ; @BT 01
+
    bhi +                    ; @BT 22 01
    nop                      ; @BT 01
+
    ble +                    ; @BT 2F 01
    nop                      ; @BT 01
+
    bls +                    ; @BT 23 01
    nop                      ; @BT 01
+
    blt +                    ; @BT 2D 01
    nop                      ; @BT 01
+
    bmi +                    ; @BT 2B 01
    nop                      ; @BT 01
+
    bne +                    ; @BT 26 01
    nop                      ; @BT 01
+
    bpl +                    ; @BT 2A 01
    nop                      ; @BT 01
+
    bra +                    ; @BT 20 01
    nop                      ; @BT 01
+
    bsr +                    ; @BT 8D 01
    nop                      ; @BT 01
+
    bvc +                    ; @BT 28 01
    nop                      ; @BT 01
+
    bvs +                    ; @BT 29 01
    nop                      ; @BT 01
+

    ; --- Extended ---
    adca $1234               ; @BT B9 12 34
    adcb $1235               ; @BT F9 12 35
    adda $1236               ; @BT BB 12 36
    addb $1237               ; @BT FB 12 37
    anda $1238               ; @BT B4 12 38
    andb $1239               ; @BT F4 12 39
    bita $123A               ; @BT B5 12 3A
    bitb $123B               ; @BT F5 12 3B
    cmpa $123C               ; @BT B1 12 3C
    cmpb $123D               ; @BT F1 12 3D
    eora $123E               ; @BT B8 12 3E
    eorb $123F               ; @BT F8 12 3F
    ldaa $1240               ; @BT B6 12 40
    ldab $1241               ; @BT F6 12 41
    oraa $1242               ; @BT BA 12 42
    orab $1243               ; @BT FA 12 43
    sbca $1244               ; @BT B2 12 44
    sbcb $1245               ; @BT F2 12 45
    suba $1246               ; @BT B0 12 46
    subb $1247               ; @BT F0 12 47
    staa $1248               ; @BT B7 12 48
    stab $1249               ; @BT F7 12 49
    sts $124A                ; @BT BF 12 4A
    stx $124B                ; @BT FF 12 4B
    cpx $124C                ; @BT BC 12 4C
    lds $124D                ; @BT BE 12 4D
    ldx $124E                ; @BT FE 12 4E
    asl $124F                ; @BT 78 12 4F
    asr $1250                ; @BT 77 12 50
    lsr $1251                ; @BT 74 12 51
    rol $1252                ; @BT 79 12 52
    ror $1253                ; @BT 76 12 53
    inc $1254                ; @BT 7C 12 54
    dec $1255                ; @BT 7A 12 55
    neg $1256                ; @BT 70 12 56
    com $1257                ; @BT 73 12 57
    clr $1258                ; @BT 7F 12 58
    tst $1259                ; @BT 7D 12 59
    jmp $2000                ; @BT 7E 20 00
    jsr $3456                ; @BT BD 34 56

    .db "<02"               ; @BT END

    /////////////////////////////////////////////////////////////////////////////
    // TEST-03: same opcodes, randomised order, operands as arithmetic expressions
    /////////////////////////////////////////////////////////////////////////////

    .db "03>"               ; @BT TEST-03 03 START

    ; --- Indexed ---
    adca 6*8,x               ; @BT A9 30
    adcb $30+1,x             ; @BT E9 31
    adda 25*2,x              ; @BT AB 32
    addb $30+3,x             ; @BT EB 33
    anda 26*2,x              ; @BT A4 34
    andb $36-1,x             ; @BT E4 35
    bita 27*2,x              ; @BT A5 36
    bitb $30+7,x             ; @BT E5 37
    cmpa 28*2,x              ; @BT A1 38
    cmpb $3A-1,x             ; @BT E1 39
    eora 29*2,x              ; @BT A8 3A
    eorb $3A+1,x             ; @BT E8 3B
    ldaa 30*2,x              ; @BT A6 3C
    ldab $3C+1,x             ; @BT E6 3D
    oraa 31*2,x              ; @BT AA 3E
    orab $40-1,x             ; @BT EA 3F
    sbca 32*2,x              ; @BT A2 40
    sbcb $40+1,x             ; @BT E2 41
    suba 33*2,x              ; @BT A0 42
    subb $40+3,x             ; @BT E0 43
    staa 34*2,x              ; @BT A7 44
    stab $44+1,x             ; @BT E7 45
    sts 35*2,x               ; @BT AF 46
    stx $44+3,x              ; @BT EF 47
    cpx 36*2,x               ; @BT AC 48
    lds $48+1,x              ; @BT AE 49
    ldx 37*2,x               ; @BT EE 4A
    asl $4A+1,x              ; @BT 68 4B
    asr 38*2,x               ; @BT 67 4C
    lsr $4C+1,x              ; @BT 64 4D
    rol 39*2,x               ; @BT 69 4E
    ror $50-1,x              ; @BT 66 4F
    inc 40*2,x               ; @BT 6C 50
    dec $50+1,x              ; @BT 6A 51
    neg 41*2,x               ; @BT 60 52
    com $52+1,x              ; @BT 63 53
    clr 42*2,x               ; @BT 6F 54
    tst $54+1,x              ; @BT 6D 55
    jmp 43*2,x               ; @BT 6E 56
    jsr $56+1,x              ; @BT AD 57

    ; --- Immediate ---
    adca #1+0                ; @BT 89 01
    adcb #1+1                ; @BT C9 02
    adda #6/2                ; @BT 8B 03
    addb #8/2                ; @BT CB 04
    anda #10/2               ; @BT 84 05
    andb #3*2                ; @BT C4 06
    bita #14/2               ; @BT 85 07
    bitb #4*2                ; @BT C5 08
    cmpa #3*3                ; @BT 81 09
    cmpb #5*2                ; @BT C1 0A
    eora #22/2               ; @BT 88 0B
    eorb #3*4                ; @BT C8 0C
    ldaa #26/2               ; @BT 86 0D
    ldab #7*2                ; @BT C6 0E
    oraa #3*5                ; @BT 8A 0F
    orab #8*2                ; @BT CA 10
    sbca #34/2               ; @BT 82 11
    sbcb #6*3                ; @BT C2 12
    suba #20-1               ; @BT 80 13
    subb #5*4                ; @BT C0 14
    cpx #$1200 + $34         ; @BT 8C 12 34
    lds #$1200 + $35         ; @BT 8E 12 35
    ldx #$1200 + $36         ; @BT CE 12 36

    ; --- Direct ---
    adca 8*2                 ; @BT 99 10
    adcb $10+1               ; @BT D9 11
    adda 9*2                 ; @BT 9B 12
    addb $10+3               ; @BT DB 13
    anda 4*5                 ; @BT 94 14
    andb 7*3                 ; @BT D4 15
    bita 11*2                ; @BT 95 16
    bitb 23                  ; @BT D5 17
    cmpa 6*4                 ; @BT 91 18
    cmpb $10+9               ; @BT D1 19
    eora $0D+13              ; @BT 98 1A
    eorb $20-5               ; @BT D8 1B
    ldaa 7*4                 ; @BT 96 1C
    ldab $1E-1               ; @BT D6 1D
    oraa 10*3                ; @BT 9A 1E
    orab $20-1               ; @BT DA 1F
    sbca 4*8                 ; @BT 92 20
    sbcb $20+1               ; @BT D2 21
    suba 17*2                ; @BT 90 22
    subb $24-1               ; @BT D0 23
    staa 6*6                 ; @BT 97 24
    stab 5*7+2               ; @BT D7 25
    sts 19*2                 ; @BT 9F 26
    stx 13*3                 ; @BT DF 27
    cpx 20*2                 ; @BT 9C 28
    lds $2A-1                ; @BT 9E 29
    ldx 14*3                 ; @BT DE 2A

    ; --- Inherent ---
    aba                      ; @BT 1B
    cba                      ; @BT 11
    clc                      ; @BT 0C
    cli                      ; @BT 0E
    clra                     ; @BT 4F
    clrb                     ; @BT 5F
    clv                      ; @BT 0A
    coma                     ; @BT 43
    comb                     ; @BT 53
    daa                      ; @BT 19
    deca                     ; @BT 4A
    decb                     ; @BT 5A
    des                      ; @BT 34
    dex                      ; @BT 09
    inca                     ; @BT 4C
    incb                     ; @BT 5C
    ins                      ; @BT 31
    inx                      ; @BT 08
    asla                     ; @BT 48
    aslb                     ; @BT 58
    asra                     ; @BT 47
    asrb                     ; @BT 57
    lsra                     ; @BT 44
    lsrb                     ; @BT 54
    nega                     ; @BT 40
    negb                     ; @BT 50
    nop                      ; @BT 01
    psha                     ; @BT 36
    pshb                     ; @BT 37
    pula                     ; @BT 32
    pulb                     ; @BT 33
    rola                     ; @BT 49
    rolb                     ; @BT 59
    rora                     ; @BT 46
    rorb                     ; @BT 56
    rti                      ; @BT 3B
    rts                      ; @BT 39
    sba                      ; @BT 10
    sec                      ; @BT 0D
    sei                      ; @BT 0F
    sev                      ; @BT 0B
    swi                      ; @BT 3F
    tab                      ; @BT 16
    tap                      ; @BT 06
    tba                      ; @BT 17
    tpa                      ; @BT 07
    tsta                     ; @BT 4D
    tstb                     ; @BT 5D
    tsx                      ; @BT 30
    txs                      ; @BT 35
    wai                      ; @BT 3E

    ; --- Extended ---
    adca $1200 + $34         ; @BT B9 12 34
    adcb $1200 + $35         ; @BT F9 12 35
    adda $1200 + $36         ; @BT BB 12 36
    addb $1200 + $37         ; @BT FB 12 37
    anda $1200 + $38         ; @BT B4 12 38
    andb $1200 + $39         ; @BT F4 12 39
    bita $1200 + $3A         ; @BT B5 12 3A
    bitb $1200 + $3B         ; @BT F5 12 3B
    cmpa $1200 + $3C         ; @BT B1 12 3C
    cmpb $1200 + $3D         ; @BT F1 12 3D
    eora $1200 + $3E         ; @BT B8 12 3E
    eorb $1200 + $3F         ; @BT F8 12 3F
    ldaa $1200 + $40         ; @BT B6 12 40
    ldab $1200 + $41         ; @BT F6 12 41
    oraa $1200 + $42         ; @BT BA 12 42
    orab $1200 + $43         ; @BT FA 12 43
    sbca $1200 + $44         ; @BT B2 12 44
    sbcb $1200 + $45         ; @BT F2 12 45
    suba $1200 + $46         ; @BT B0 12 46
    subb $1200 + $47         ; @BT F0 12 47
    staa $1200 + $48         ; @BT B7 12 48
    stab $1200 + $49         ; @BT F7 12 49
    sts $1200 + $4A          ; @BT BF 12 4A
    stx $1200 + $4B          ; @BT FF 12 4B
    cpx $1200 + $4C          ; @BT BC 12 4C
    lds $1200 + $4D          ; @BT BE 12 4D
    ldx $1200 + $4E          ; @BT FE 12 4E
    asl $1200 + $4F          ; @BT 78 12 4F
    asr $1200 + $50          ; @BT 77 12 50
    lsr $1200 + $51          ; @BT 74 12 51
    rol $1200 + $52          ; @BT 79 12 52
    ror $1200 + $53          ; @BT 76 12 53
    inc $1200 + $54          ; @BT 7C 12 54
    dec $1200 + $55          ; @BT 7A 12 55
    neg $1200 + $56          ; @BT 70 12 56
    com $1200 + $57          ; @BT 73 12 57
    clr $1200 + $58          ; @BT 7F 12 58
    tst $1200 + $59          ; @BT 7D 12 59
    jmp $1000 * 2            ; @BT 7E 20 00
    jsr $3400 + $56          ; @BT BD 34 56

    ; --- Branches (relative offsets computed from fixed layout) ---
    ; All branches target the same forward-local label '+'. Since each branch
    ; is at a different address, the relative offset differs.
    bcc +                    ; @BT 24 1E
    bcs +                    ; @BT 25 1C
    beq +                    ; @BT 27 1A
    bge +                    ; @BT 2C 18
    bgt +                    ; @BT 2E 16
    bhi +                    ; @BT 22 14
    ble +                    ; @BT 2F 12
    bls +                    ; @BT 23 10
    blt +                    ; @BT 2D 0E
    bmi +                    ; @BT 2B 0C
    bne +                    ; @BT 26 0A
    bpl +                    ; @BT 2A 08
    bra +                    ; @BT 20 06
    bsr +                    ; @BT 8D 04
    bvc +                    ; @BT 28 02
    bvs +                    ; @BT 29 00

+

    .db "<03"               ; @BT END

    /////////////////////////////////////////////////////////////////////////////
    // TEST-04: similar to TEST-03 and then defines + expressions
    /////////////////////////////////////////////////////////////////////////////

    ; --- Constant definitions for TEST-04 ---
    .define T4_CONST_00           = 3 - 3
    .define T4_IMM_01             = 1 + T4_CONST_00
    .define T4_IMM_02             = 1 + 1
    .define T4_IMM_03             = 9 / 3
    .define T4_IMM_04             = 2 * 2
    .define T4_IMM_05             = 10 - 5
    .define T4_IMM_06             = 3 * 2
    .define T4_IMM_07             = 21 / 3
    .define T4_IMM_08             = 4 * 2
    .define T4_IMM_09             = 3 * 3
    .define T4_IMM_0A             = 5 * 2
    .define T4_IMM_0B             = 22 / 2
    .define T4_IMM_0C             = 3 * 4
    .define T4_IMM_0D             = 26 / 2
    .define T4_IMM_0E             = 7 * 2
    .define T4_IMM_0F             = 3 * 5
    .define T4_IMM_10             = 8 * 2
    .define T4_IMM_11             = 34 / 2
    .define T4_IMM_12             = 6 * 3
    .define T4_IMM_13             = 20 - 1
    .define T4_IMM_14             = 5 * 4
    .define T4_DIR_BASE           = $10
    .define T4_DIR_10             = T4_DIR_BASE
    .define T4_DIR_11             = T4_DIR_BASE + 1
    .define T4_DIR_12             = 18
    .define T4_DIR_13             = 19
    .define T4_DIR_14             = 20
    .define T4_DIR_15             = 21
    .define T4_DIR_16             = 22
    .define T4_DIR_17             = 23
    .define T4_DIR_18             = 24
    .define T4_DIR_19             = 25
    .define T4_DIR_1A             = 26
    .define T4_DIR_1B             = 27
    .define T4_DIR_1C             = 28
    .define T4_DIR_1D             = 29
    .define T4_DIR_1E             = 30
    .define T4_DIR_1F             = 31
    .define T4_DIR_20             = 32
    .define T4_DIR_21             = 33
    .define T4_DIR_22             = 34
    .define T4_DIR_23             = 35
    .define T4_DIR_24             = 36
    .define T4_DIR_25             = 37
    .define T4_DIR_26             = 38
    .define T4_DIR_27             = 39
    .define T4_DIR_28             = 40
    .define T4_DIR_29             = 41
    .define T4_DIR_2A             = 42
    .define T4_IDX_BASE           = $30
    .define T4_IDX_30             = T4_IDX_BASE
    .define T4_IDX_31             = T4_IDX_BASE + 1
    .define T4_IDX_32             = T4_IDX_BASE + 2
    .define T4_IDX_33             = T4_IDX_BASE + 3
    .define T4_IDX_34             = T4_IDX_BASE + 4
    .define T4_IDX_35             = T4_IDX_BASE + 5
    .define T4_IDX_36             = T4_IDX_BASE + 6
    .define T4_IDX_37             = T4_IDX_BASE + 7
    .define T4_IDX_38             = T4_IDX_BASE + 8
    .define T4_IDX_39             = T4_IDX_BASE + 9
    .define T4_IDX_3A             = T4_IDX_BASE + 10
    .define T4_IDX_3B             = T4_IDX_BASE + 11
    .define T4_IDX_3C             = T4_IDX_BASE + 12
    .define T4_IDX_3D             = T4_IDX_BASE + 13
    .define T4_IDX_3E             = T4_IDX_BASE + 14
    .define T4_IDX_3F             = T4_IDX_BASE + 15
    .define T4_IDX_40             = T4_IDX_BASE + 16
    .define T4_IDX_41             = T4_IDX_BASE + 17
    .define T4_IDX_42             = T4_IDX_BASE + 18
    .define T4_IDX_43             = T4_IDX_BASE + 19
    .define T4_IDX_44             = T4_IDX_BASE + 20
    .define T4_IDX_45             = T4_IDX_BASE + 21
    .define T4_IDX_46             = T4_IDX_BASE + 22
    .define T4_IDX_47             = T4_IDX_BASE + 23
    .define T4_IDX_48             = T4_IDX_BASE + 24
    .define T4_IDX_49             = T4_IDX_BASE + 25
    .define T4_IDX_4A             = T4_IDX_BASE + 26
    .define T4_IDX_4B             = T4_IDX_BASE + 27
    .define T4_IDX_4C             = T4_IDX_BASE + 28
    .define T4_IDX_4D             = T4_IDX_BASE + 29
    .define T4_IDX_4E             = T4_IDX_BASE + 30
    .define T4_IDX_4F             = T4_IDX_BASE + 31
    .define T4_IDX_50             = T4_IDX_BASE + 32
    .define T4_IDX_51             = T4_IDX_BASE + 33
    .define T4_IDX_52             = T4_IDX_BASE + 34
    .define T4_IDX_53             = T4_IDX_BASE + 35
    .define T4_IDX_54             = T4_IDX_BASE + 36
    .define T4_IDX_55             = T4_IDX_BASE + 37
    .define T4_IDX_56             = T4_IDX_BASE + 38
    .define T4_IDX_57             = T4_IDX_BASE + 39
    .define T4_EXT_BASE           = $1200
    .define T4_EXT_34             = T4_EXT_BASE + $34
    .define T4_EXT_35             = T4_EXT_BASE + $35
    .define T4_EXT_36             = T4_EXT_BASE + $36
    .define T4_EXT_37             = T4_EXT_BASE + $37
    .define T4_EXT_38             = T4_EXT_BASE + $38
    .define T4_EXT_39             = T4_EXT_BASE + $39
    .define T4_EXT_3A             = T4_EXT_BASE + $3A
    .define T4_EXT_3B             = T4_EXT_BASE + $3B
    .define T4_EXT_3C             = T4_EXT_BASE + $3C
    .define T4_EXT_3D             = T4_EXT_BASE + $3D
    .define T4_EXT_3E             = T4_EXT_BASE + $3E
    .define T4_EXT_3F             = T4_EXT_BASE + $3F
    .define T4_EXT_40             = T4_EXT_BASE + $40
    .define T4_EXT_41             = T4_EXT_BASE + $41
    .define T4_EXT_42             = T4_EXT_BASE + $42
    .define T4_EXT_43             = T4_EXT_BASE + $43
    .define T4_EXT_44             = T4_EXT_BASE + $44
    .define T4_EXT_45             = T4_EXT_BASE + $45
    .define T4_EXT_46             = T4_EXT_BASE + $46
    .define T4_EXT_47             = T4_EXT_BASE + $47
    .define T4_EXT_48             = T4_EXT_BASE + $48
    .define T4_EXT_49             = T4_EXT_BASE + $49
    .define T4_EXT_4A             = T4_EXT_BASE + $4A
    .define T4_EXT_4B             = T4_EXT_BASE + $4B
    .define T4_EXT_4C             = T4_EXT_BASE + $4C
    .define T4_EXT_4D             = T4_EXT_BASE + $4D
    .define T4_EXT_4E             = T4_EXT_BASE + $4E
    .define T4_EXT_4F             = T4_EXT_BASE + $4F
    .define T4_EXT_50             = T4_EXT_BASE + $50
    .define T4_EXT_51             = T4_EXT_BASE + $51
    .define T4_EXT_52             = T4_EXT_BASE + $52
    .define T4_EXT_53             = T4_EXT_BASE + $53
    .define T4_EXT_54             = T4_EXT_BASE + $54
    .define T4_EXT_55             = T4_EXT_BASE + $55
    .define T4_EXT_56             = T4_EXT_BASE + $56
    .define T4_EXT_57             = T4_EXT_BASE + $57
    .define T4_EXT_58             = T4_EXT_BASE + $58
    .define T4_EXT_59             = T4_EXT_BASE + $59
    .define T4_JMP_ADDR           = $2000
    .define T4_JSR_ADDR           = $3456

    .db "04>"               ; @BT TEST-04 04 START

    ; --- Immediate ---
    adca #T4_IMM_01                ; @BT 89 01
    adcb #T4_IMM_02                ; @BT C9 02
    adda #T4_IMM_03                ; @BT 8B 03
    addb #T4_IMM_04                ; @BT CB 04
    anda #T4_IMM_05                ; @BT 84 05
    andb #T4_IMM_06                ; @BT C4 06
    bita #T4_IMM_07                ; @BT 85 07
    bitb #T4_IMM_08                ; @BT C5 08
    cmpa #T4_IMM_09                ; @BT 81 09
    cmpb #T4_IMM_0A                ; @BT C1 0A
    eora #T4_IMM_0B                ; @BT 88 0B
    eorb #T4_IMM_0C                ; @BT C8 0C
    ldaa #T4_IMM_0D                ; @BT 86 0D
    ldab #T4_IMM_0E                ; @BT C6 0E
    oraa #T4_IMM_0F                ; @BT 8A 0F
    orab #T4_IMM_10                ; @BT CA 10
    sbca #T4_IMM_11                ; @BT 82 11
    sbcb #T4_IMM_12                ; @BT C2 12
    suba #T4_IMM_13                ; @BT 80 13
    subb #T4_IMM_14                ; @BT C0 14
    cpx #T4_EXT_34                 ; @BT 8C 12 34
    lds #T4_EXT_35                 ; @BT 8E 12 35
    ldx #T4_EXT_36                 ; @BT CE 12 36

    ; --- Inherent ---
    aba                      ; @BT 1B
    cba                      ; @BT 11
    clc                      ; @BT 0C
    cli                      ; @BT 0E
    clra                     ; @BT 4F
    clrb                     ; @BT 5F
    clv                      ; @BT 0A
    coma                     ; @BT 43
    comb                     ; @BT 53
    daa                      ; @BT 19
    deca                     ; @BT 4A
    decb                     ; @BT 5A
    des                      ; @BT 34
    dex                      ; @BT 09
    inca                     ; @BT 4C
    incb                     ; @BT 5C
    ins                      ; @BT 31
    inx                      ; @BT 08
    asla                     ; @BT 48
    aslb                     ; @BT 58
    asra                     ; @BT 47
    asrb                     ; @BT 57
    lsra                     ; @BT 44
    lsrb                     ; @BT 54
    nega                     ; @BT 40
    negb                     ; @BT 50
    nop                      ; @BT 01
    psha                     ; @BT 36
    pshb                     ; @BT 37
    pula                     ; @BT 32
    pulb                     ; @BT 33
    rola                     ; @BT 49
    rolb                     ; @BT 59
    rora                     ; @BT 46
    rorb                     ; @BT 56
    rti                      ; @BT 3B
    rts                      ; @BT 39
    sba                      ; @BT 10
    sec                      ; @BT 0D
    sei                      ; @BT 0F
    sev                      ; @BT 0B
    swi                      ; @BT 3F
    tab                      ; @BT 16
    tap                      ; @BT 06
    tba                      ; @BT 17
    tpa                      ; @BT 07
    tsta                     ; @BT 4D
    tstb                     ; @BT 5D
    tsx                      ; @BT 30
    txs                      ; @BT 35
    wai                      ; @BT 3E

    ; --- Indexed ---
    adca T4_IDX_30,x               ; @BT A9 30
    adcb T4_IDX_31,x               ; @BT E9 31
    adda T4_IDX_32,x               ; @BT AB 32
    addb T4_IDX_33,x               ; @BT EB 33
    anda T4_IDX_34,x               ; @BT A4 34
    andb T4_IDX_35,x               ; @BT E4 35
    bita T4_IDX_36,x               ; @BT A5 36
    bitb T4_IDX_37,x               ; @BT E5 37
    cmpa T4_IDX_38,x               ; @BT A1 38
    cmpb T4_IDX_39,x               ; @BT E1 39
    eora T4_IDX_3A,x               ; @BT A8 3A
    eorb T4_IDX_3B,x               ; @BT E8 3B
    ldaa T4_IDX_3C,x               ; @BT A6 3C
    ldab T4_IDX_3D,x               ; @BT E6 3D
    oraa T4_IDX_3E,x               ; @BT AA 3E
    orab T4_IDX_3F,x               ; @BT EA 3F
    sbca T4_IDX_40,x               ; @BT A2 40
    sbcb T4_IDX_41,x               ; @BT E2 41
    suba T4_IDX_42,x               ; @BT A0 42
    subb T4_IDX_43,x               ; @BT E0 43
    staa T4_IDX_44,x               ; @BT A7 44
    stab T4_IDX_45,x               ; @BT E7 45
    sts T4_IDX_46,x                ; @BT AF 46
    stx T4_IDX_47,x                ; @BT EF 47
    cpx T4_IDX_48,x                ; @BT AC 48
    lds T4_IDX_49,x                ; @BT AE 49
    ldx T4_IDX_4A,x                ; @BT EE 4A
    asl T4_IDX_4B,x                ; @BT 68 4B
    asr T4_IDX_4C,x                ; @BT 67 4C
    lsr T4_IDX_4D,x                ; @BT 64 4D
    rol T4_IDX_4E,x                ; @BT 69 4E
    ror T4_IDX_4F,x                ; @BT 66 4F
    inc T4_IDX_50,x                ; @BT 6C 50
    dec T4_IDX_51,x                ; @BT 6A 51
    neg T4_IDX_52,x                ; @BT 60 52
    com T4_IDX_53,x                ; @BT 63 53
    clr T4_IDX_54,x                ; @BT 6F 54
    tst T4_IDX_55,x                ; @BT 6D 55
    jmp T4_IDX_56,x                ; @BT 6E 56
    jsr T4_IDX_57,x                ; @BT AD 57

    ; --- Direct ---
    adca T4_DIR_10                 ; @BT 99 10
    adcb T4_DIR_11                 ; @BT D9 11
    adda T4_DIR_12                 ; @BT 9B 12
    addb T4_DIR_13                 ; @BT DB 13
    anda T4_DIR_14                 ; @BT 94 14
    andb T4_DIR_15                 ; @BT D4 15
    bita T4_DIR_16                 ; @BT 95 16
    bitb T4_DIR_17                 ; @BT D5 17
    cmpa T4_DIR_18                 ; @BT 91 18
    cmpb T4_DIR_19                 ; @BT D1 19
    eora T4_DIR_1A                 ; @BT 98 1A
    eorb T4_DIR_1B                 ; @BT D8 1B
    ldaa T4_DIR_1C                 ; @BT 96 1C
    ldab T4_DIR_1D                 ; @BT D6 1D
    oraa T4_DIR_1E                 ; @BT 9A 1E
    orab T4_DIR_1F                 ; @BT DA 1F
    sbca T4_DIR_20                 ; @BT 92 20
    sbcb T4_DIR_21                 ; @BT D2 21
    suba T4_DIR_22                 ; @BT 90 22
    subb T4_DIR_23                 ; @BT D0 23
    staa T4_DIR_24                 ; @BT 97 24
    stab T4_DIR_25                 ; @BT D7 25
    sts T4_DIR_26                  ; @BT 9F 26
    stx T4_DIR_27                  ; @BT DF 27
    cpx T4_DIR_28                  ; @BT 9C 28
    lds T4_DIR_29                  ; @BT 9E 29
    ldx T4_DIR_2A                  ; @BT DE 2A

    ; --- Extended ---
    adca T4_EXT_34                 ; @BT B9 12 34
    adcb T4_EXT_35                 ; @BT F9 12 35
    adda T4_EXT_36                 ; @BT BB 12 36
    addb T4_EXT_37                 ; @BT FB 12 37
    anda T4_EXT_38                 ; @BT B4 12 38
    andb T4_EXT_39                 ; @BT F4 12 39
    bita T4_EXT_3A                 ; @BT B5 12 3A
    bitb T4_EXT_3B                 ; @BT F5 12 3B
    cmpa T4_EXT_3C                 ; @BT B1 12 3C
    cmpb T4_EXT_3D                 ; @BT F1 12 3D
    eora T4_EXT_3E                 ; @BT B8 12 3E
    eorb T4_EXT_3F                 ; @BT F8 12 3F
    ldaa T4_EXT_40                 ; @BT B6 12 40
    ldab T4_EXT_41                 ; @BT F6 12 41
    oraa T4_EXT_42                 ; @BT BA 12 42
    orab T4_EXT_43                 ; @BT FA 12 43
    sbca T4_EXT_44                 ; @BT B2 12 44
    sbcb T4_EXT_45                 ; @BT F2 12 45
    suba T4_EXT_46                 ; @BT B0 12 46
    subb T4_EXT_47                 ; @BT F0 12 47
    staa T4_EXT_48                 ; @BT B7 12 48
    stab T4_EXT_49                 ; @BT F7 12 49
    sts T4_EXT_4A                  ; @BT BF 12 4A
    stx T4_EXT_4B                  ; @BT FF 12 4B
    cpx T4_EXT_4C                  ; @BT BC 12 4C
    lds T4_EXT_4D                  ; @BT BE 12 4D
    ldx T4_EXT_4E                  ; @BT FE 12 4E
    asl T4_EXT_4F                  ; @BT 78 12 4F
    asr T4_EXT_50                  ; @BT 77 12 50
    lsr T4_EXT_51                  ; @BT 74 12 51
    rol T4_EXT_52                  ; @BT 79 12 52
    ror T4_EXT_53                  ; @BT 76 12 53
    inc T4_EXT_54                  ; @BT 7C 12 54
    dec T4_EXT_55                  ; @BT 7A 12 55
    neg T4_EXT_56                  ; @BT 70 12 56
    com T4_EXT_57                  ; @BT 73 12 57
    clr T4_EXT_58                  ; @BT 7F 12 58
    tst T4_EXT_59                  ; @BT 7D 12 59
    jmp T4_JMP_ADDR                ; @BT 7E 20 00
    jsr T4_JSR_ADDR                ; @BT BD 34 56

    ; --- Branches (relative offsets computed from fixed layout) ---
    ; All branches target the same forward-local label '+'. Since each branch
    ; is at a different address, the relative offset differs.
    bcc +                    ; @BT 24 1E
    bcs +                    ; @BT 25 1C
    beq +                    ; @BT 27 1A
    bge +                    ; @BT 2C 18
    bgt +                    ; @BT 2E 16
    bhi +                    ; @BT 22 14
    ble +                    ; @BT 2F 12
    bls +                    ; @BT 23 10
    blt +                    ; @BT 2D 0E
    bmi +                    ; @BT 2B 0C
    bne +                    ; @BT 26 0A
    bpl +                    ; @BT 2A 08
    bra +                    ; @BT 20 06
    bsr +                    ; @BT 8D 04
    bvc +                    ; @BT 28 02
    bvs +                    ; @BT 29 00

+

    .db "<04"               ; @BT END

    /////////////////////////////////////////////////////////////////////////////
    // TEST-05: similar to TEST-04 and defines + functions + expressions
    /////////////////////////////////////////////////////////////////////////////

    ; --- Constant definitions and functions for TEST-05 ---
    .define T5_IMM_BASE            = 1
    .define T5_DIR_BASE            = $10
    .define T5_IDX_BASE            = $30
    .define T5_EXT_BASE            = $1200
    .define T5_JMP_ADDR            = $2000
    .define T5_JSR_ADDR            = $3456

    .FUNCTION F5_ADD(p, q) (p + q)
    .FUNCTION F5_SUB(p, q) (p - q)
    .FUNCTION F5_MUL(p, q) (p * q)
    .FUNCTION F5_ZERO(p) (F5_SUB(p, p))
    .FUNCTION F5_ONE(p) (F5_ADD(F5_ZERO(p), 1))
    .FUNCTION F5_TWO(p) (F5_ADD(F5_ONE(p), F5_ONE(p)))
    .FUNCTION F5_THREE(p) (F5_ADD(F5_TWO(p), F5_ONE(p)))
    .FUNCTION F5_FOUR(p) (F5_ADD(F5_TWO(p), F5_TWO(p)))
    .FUNCTION F5_FIVE(p) (F5_ADD(F5_FOUR(p), F5_ONE(p)))
    .FUNCTION F5_SIX(p) (F5_ADD(F5_THREE(p), F5_THREE(p)))
    .FUNCTION F5_SEVEN(p) (F5_ADD(F5_SIX(p), F5_ONE(p)))
    .FUNCTION F5_EIGHT(p) (F5_ADD(F5_FOUR(p), F5_FOUR(p)))
    .FUNCTION F5_NINE(p) (F5_ADD(F5_EIGHT(p), F5_ONE(p)))
    .FUNCTION F5_TEN(p) (F5_ADD(F5_FIVE(p), F5_FIVE(p)))
    .FUNCTION F5_INC(p) (F5_ADD(p, F5_ONE(p)))
    .FUNCTION F5_DEC(p) (F5_SUB(p, F5_ONE(p)))
    .FUNCTION F5_BASE(p, q) (F5_ADD(p, q))
    .FUNCTION F5_OFF(p, q) (F5_ADD(F5_BASE(p, q), F5_ZERO(p)))
    .FUNCTION F5_SCALE(p, q) (F5_ADD(F5_MUL(p, q), F5_ZERO(q)))
    .FUNCTION F5_BLEND(p, q) (F5_ADD(F5_OFF(p, q), F5_ONE(p)))
    .FUNCTION F5_SHIFT(p, q) (F5_ADD(p, F5_ADD(q, F5_ZERO(q))))
    .FUNCTION F5_STEP(p, q) (F5_ADD(F5_SHIFT(p, q), F5_ZERO(p)))
    .FUNCTION F5_BIAS(p, q) (F5_ADD(F5_STEP(p, q), F5_ZERO(q)))
    .FUNCTION F5_PAGE(p, q) (F5_ADD(F5_BIAS(p, q), F5_ZERO(p)))
    .FUNCTION F5_WRAP(p, q) (F5_ADD(F5_PAGE(p, q), F5_ZERO(q)))
    .FUNCTION F5_ID(p) (F5_ADD(p, F5_ZERO(p)))
    .FUNCTION F5_OFFSET(p, q) (F5_ADD(F5_ID(p), F5_ID(q)))
    .FUNCTION F5_OFFSET2(p, q) (F5_ADD(F5_OFFSET(p, q), F5_ZERO(p)))
    .FUNCTION F5_OFFSET3(p, q) (F5_ADD(F5_OFFSET2(p, q), F5_ZERO(q)))
    .FUNCTION F5_CONST(p, q) (F5_ADD(F5_ZERO(p), q))
    .FUNCTION F5_TUNE(p, q) (F5_ADD(F5_OFFSET3(p, q), F5_ZERO(q)))
    .FUNCTION F5_TRIM(p, q) (F5_SUB(F5_OFFSET3(p, q), F5_ZERO(p)))
    .FUNCTION F5_PAD(p) (F5_SUB(p, p))
    .FUNCTION F5_VAL(p, q) (F5_ADD(F5_TUNE(p, q), F5_PAD(q)))
    .FUNCTION F5_OUT(p, q) (F5_ADD(F5_VAL(p, q), F5_PAD(p)))

    .define T5_IMM_01               = F5_ADD(F5_OUT(T5_IMM_BASE, 0), F5_PAD(1))
    .define T5_IMM_02               = F5_ADD(F5_OUT(T5_IMM_BASE, 1), F5_PAD(1))
    .define T5_IMM_03               = F5_ADD(F5_OUT(T5_IMM_BASE, 2), F5_PAD(1))
    .define T5_IMM_04               = F5_ADD(F5_OUT(T5_IMM_BASE, 3), F5_PAD(1))
    .define T5_IMM_05               = F5_ADD(F5_OUT(T5_IMM_BASE, 4), F5_PAD(1))
    .define T5_IMM_06               = F5_ADD(F5_OUT(T5_IMM_BASE, 5), F5_PAD(1))
    .define T5_IMM_07               = F5_ADD(F5_OUT(T5_IMM_BASE, 6), F5_PAD(1))
    .define T5_IMM_08               = F5_ADD(F5_OUT(T5_IMM_BASE, 7), F5_PAD(1))
    .define T5_IMM_09               = F5_ADD(F5_OUT(T5_IMM_BASE, 8), F5_PAD(1))
    .define T5_IMM_0A               = F5_ADD(F5_OUT(T5_IMM_BASE, 9), F5_PAD(1))
    .define T5_IMM_0B               = F5_ADD(F5_OUT(T5_IMM_BASE, 10), F5_PAD(1))
    .define T5_IMM_0C               = F5_ADD(F5_OUT(T5_IMM_BASE, 11), F5_PAD(1))
    .define T5_IMM_0D               = F5_ADD(F5_OUT(T5_IMM_BASE, 12), F5_PAD(1))
    .define T5_IMM_0E               = F5_ADD(F5_OUT(T5_IMM_BASE, 13), F5_PAD(1))
    .define T5_IMM_0F               = F5_ADD(F5_OUT(T5_IMM_BASE, 14), F5_PAD(1))
    .define T5_IMM_10               = F5_ADD(F5_OUT(T5_IMM_BASE, 15), F5_PAD(1))
    .define T5_IMM_11               = F5_ADD(F5_OUT(T5_IMM_BASE, 16), F5_PAD(1))
    .define T5_IMM_12               = F5_ADD(F5_OUT(T5_IMM_BASE, 17), F5_PAD(1))
    .define T5_IMM_13               = F5_ADD(F5_OUT(T5_IMM_BASE, 18), F5_PAD(1))
    .define T5_IMM_14               = F5_ADD(F5_OUT(T5_IMM_BASE, 19), F5_PAD(1))
    .define T5_DIR_10               = F5_ADD(F5_OUT(T5_DIR_BASE, 0), F5_PAD(1))
    .define T5_DIR_11               = F5_ADD(F5_OUT(T5_DIR_BASE, 1), F5_PAD(1))
    .define T5_DIR_12               = F5_ADD(F5_OUT(T5_DIR_BASE, 2), F5_PAD(1))
    .define T5_DIR_13               = F5_ADD(F5_OUT(T5_DIR_BASE, 3), F5_PAD(1))
    .define T5_DIR_14               = F5_ADD(F5_OUT(T5_DIR_BASE, 4), F5_PAD(1))
    .define T5_DIR_15               = F5_ADD(F5_OUT(T5_DIR_BASE, 5), F5_PAD(1))
    .define T5_DIR_16               = F5_ADD(F5_OUT(T5_DIR_BASE, 6), F5_PAD(1))
    .define T5_DIR_17               = F5_ADD(F5_OUT(T5_DIR_BASE, 7), F5_PAD(1))
    .define T5_DIR_18               = F5_ADD(F5_OUT(T5_DIR_BASE, 8), F5_PAD(1))
    .define T5_DIR_19               = F5_ADD(F5_OUT(T5_DIR_BASE, 9), F5_PAD(1))
    .define T5_DIR_1A               = F5_ADD(F5_OUT(T5_DIR_BASE, 10), F5_PAD(1))
    .define T5_DIR_1B               = F5_ADD(F5_OUT(T5_DIR_BASE, 11), F5_PAD(1))
    .define T5_DIR_1C               = F5_ADD(F5_OUT(T5_DIR_BASE, 12), F5_PAD(1))
    .define T5_DIR_1D               = F5_ADD(F5_OUT(T5_DIR_BASE, 13), F5_PAD(1))
    .define T5_DIR_1E               = F5_ADD(F5_OUT(T5_DIR_BASE, 14), F5_PAD(1))
    .define T5_DIR_1F               = F5_ADD(F5_OUT(T5_DIR_BASE, 15), F5_PAD(1))
    .define T5_DIR_20               = F5_ADD(F5_OUT(T5_DIR_BASE, 16), F5_PAD(1))
    .define T5_DIR_21               = F5_ADD(F5_OUT(T5_DIR_BASE, 17), F5_PAD(1))
    .define T5_DIR_22               = F5_ADD(F5_OUT(T5_DIR_BASE, 18), F5_PAD(1))
    .define T5_DIR_23               = F5_ADD(F5_OUT(T5_DIR_BASE, 19), F5_PAD(1))
    .define T5_DIR_24               = F5_ADD(F5_OUT(T5_DIR_BASE, 20), F5_PAD(1))
    .define T5_DIR_25               = F5_ADD(F5_OUT(T5_DIR_BASE, 21), F5_PAD(1))
    .define T5_DIR_26               = F5_ADD(F5_OUT(T5_DIR_BASE, 22), F5_PAD(1))
    .define T5_DIR_27               = F5_ADD(F5_OUT(T5_DIR_BASE, 23), F5_PAD(1))
    .define T5_DIR_28               = F5_ADD(F5_OUT(T5_DIR_BASE, 24), F5_PAD(1))
    .define T5_DIR_29               = F5_ADD(F5_OUT(T5_DIR_BASE, 25), F5_PAD(1))
    .define T5_DIR_2A               = F5_ADD(F5_OUT(T5_DIR_BASE, 26), F5_PAD(1))
    .define T5_IDX_30               = F5_ADD(F5_OUT(T5_IDX_BASE, 0), F5_PAD(1))
    .define T5_IDX_31               = F5_ADD(F5_OUT(T5_IDX_BASE, 1), F5_PAD(1))
    .define T5_IDX_32               = F5_ADD(F5_OUT(T5_IDX_BASE, 2), F5_PAD(1))
    .define T5_IDX_33               = F5_ADD(F5_OUT(T5_IDX_BASE, 3), F5_PAD(1))
    .define T5_IDX_34               = F5_ADD(F5_OUT(T5_IDX_BASE, 4), F5_PAD(1))
    .define T5_IDX_35               = F5_ADD(F5_OUT(T5_IDX_BASE, 5), F5_PAD(1))
    .define T5_IDX_36               = F5_ADD(F5_OUT(T5_IDX_BASE, 6), F5_PAD(1))
    .define T5_IDX_37               = F5_ADD(F5_OUT(T5_IDX_BASE, 7), F5_PAD(1))
    .define T5_IDX_38               = F5_ADD(F5_OUT(T5_IDX_BASE, 8), F5_PAD(1))
    .define T5_IDX_39               = F5_ADD(F5_OUT(T5_IDX_BASE, 9), F5_PAD(1))
    .define T5_IDX_3A               = F5_ADD(F5_OUT(T5_IDX_BASE, 10), F5_PAD(1))
    .define T5_IDX_3B               = F5_ADD(F5_OUT(T5_IDX_BASE, 11), F5_PAD(1))
    .define T5_IDX_3C               = F5_ADD(F5_OUT(T5_IDX_BASE, 12), F5_PAD(1))
    .define T5_IDX_3D               = F5_ADD(F5_OUT(T5_IDX_BASE, 13), F5_PAD(1))
    .define T5_IDX_3E               = F5_ADD(F5_OUT(T5_IDX_BASE, 14), F5_PAD(1))
    .define T5_IDX_3F               = F5_ADD(F5_OUT(T5_IDX_BASE, 15), F5_PAD(1))
    .define T5_IDX_40               = F5_ADD(F5_OUT(T5_IDX_BASE, 16), F5_PAD(1))
    .define T5_IDX_41               = F5_ADD(F5_OUT(T5_IDX_BASE, 17), F5_PAD(1))
    .define T5_IDX_42               = F5_ADD(F5_OUT(T5_IDX_BASE, 18), F5_PAD(1))
    .define T5_IDX_43               = F5_ADD(F5_OUT(T5_IDX_BASE, 19), F5_PAD(1))
    .define T5_IDX_44               = F5_ADD(F5_OUT(T5_IDX_BASE, 20), F5_PAD(1))
    .define T5_IDX_45               = F5_ADD(F5_OUT(T5_IDX_BASE, 21), F5_PAD(1))
    .define T5_IDX_46               = F5_ADD(F5_OUT(T5_IDX_BASE, 22), F5_PAD(1))
    .define T5_IDX_47               = F5_ADD(F5_OUT(T5_IDX_BASE, 23), F5_PAD(1))
    .define T5_IDX_48               = F5_ADD(F5_OUT(T5_IDX_BASE, 24), F5_PAD(1))
    .define T5_IDX_49               = F5_ADD(F5_OUT(T5_IDX_BASE, 25), F5_PAD(1))
    .define T5_IDX_4A               = F5_ADD(F5_OUT(T5_IDX_BASE, 26), F5_PAD(1))
    .define T5_IDX_4B               = F5_ADD(F5_OUT(T5_IDX_BASE, 27), F5_PAD(1))
    .define T5_IDX_4C               = F5_ADD(F5_OUT(T5_IDX_BASE, 28), F5_PAD(1))
    .define T5_IDX_4D               = F5_ADD(F5_OUT(T5_IDX_BASE, 29), F5_PAD(1))
    .define T5_IDX_4E               = F5_ADD(F5_OUT(T5_IDX_BASE, 30), F5_PAD(1))
    .define T5_IDX_4F               = F5_ADD(F5_OUT(T5_IDX_BASE, 31), F5_PAD(1))
    .define T5_IDX_50               = F5_ADD(F5_OUT(T5_IDX_BASE, 32), F5_PAD(1))
    .define T5_IDX_51               = F5_ADD(F5_OUT(T5_IDX_BASE, 33), F5_PAD(1))
    .define T5_IDX_52               = F5_ADD(F5_OUT(T5_IDX_BASE, 34), F5_PAD(1))
    .define T5_IDX_53               = F5_ADD(F5_OUT(T5_IDX_BASE, 35), F5_PAD(1))
    .define T5_IDX_54               = F5_ADD(F5_OUT(T5_IDX_BASE, 36), F5_PAD(1))
    .define T5_IDX_55               = F5_ADD(F5_OUT(T5_IDX_BASE, 37), F5_PAD(1))
    .define T5_IDX_56               = F5_ADD(F5_OUT(T5_IDX_BASE, 38), F5_PAD(1))
    .define T5_IDX_57               = F5_ADD(F5_OUT(T5_IDX_BASE, 39), F5_PAD(1))
    .define T5_EXT_34               = F5_ADD(F5_OUT(T5_EXT_BASE, $34), F5_PAD(1))
    .define T5_EXT_35               = F5_ADD(F5_OUT(T5_EXT_BASE, $35), F5_PAD(1))
    .define T5_EXT_36               = F5_ADD(F5_OUT(T5_EXT_BASE, $36), F5_PAD(1))
    .define T5_EXT_37               = F5_ADD(F5_OUT(T5_EXT_BASE, $37), F5_PAD(1))
    .define T5_EXT_38               = F5_ADD(F5_OUT(T5_EXT_BASE, $38), F5_PAD(1))
    .define T5_EXT_39               = F5_ADD(F5_OUT(T5_EXT_BASE, $39), F5_PAD(1))
    .define T5_EXT_3A               = F5_ADD(F5_OUT(T5_EXT_BASE, $3A), F5_PAD(1))
    .define T5_EXT_3B               = F5_ADD(F5_OUT(T5_EXT_BASE, $3B), F5_PAD(1))
    .define T5_EXT_3C               = F5_ADD(F5_OUT(T5_EXT_BASE, $3C), F5_PAD(1))
    .define T5_EXT_3D               = F5_ADD(F5_OUT(T5_EXT_BASE, $3D), F5_PAD(1))
    .define T5_EXT_3E               = F5_ADD(F5_OUT(T5_EXT_BASE, $3E), F5_PAD(1))
    .define T5_EXT_3F               = F5_ADD(F5_OUT(T5_EXT_BASE, $3F), F5_PAD(1))
    .define T5_EXT_40               = F5_ADD(F5_OUT(T5_EXT_BASE, $40), F5_PAD(1))
    .define T5_EXT_41               = F5_ADD(F5_OUT(T5_EXT_BASE, $41), F5_PAD(1))
    .define T5_EXT_42               = F5_ADD(F5_OUT(T5_EXT_BASE, $42), F5_PAD(1))
    .define T5_EXT_43               = F5_ADD(F5_OUT(T5_EXT_BASE, $43), F5_PAD(1))
    .define T5_EXT_44               = F5_ADD(F5_OUT(T5_EXT_BASE, $44), F5_PAD(1))
    .define T5_EXT_45               = F5_ADD(F5_OUT(T5_EXT_BASE, $45), F5_PAD(1))
    .define T5_EXT_46               = F5_ADD(F5_OUT(T5_EXT_BASE, $46), F5_PAD(1))
    .define T5_EXT_47               = F5_ADD(F5_OUT(T5_EXT_BASE, $47), F5_PAD(1))
    .define T5_EXT_48               = F5_ADD(F5_OUT(T5_EXT_BASE, $48), F5_PAD(1))
    .define T5_EXT_49               = F5_ADD(F5_OUT(T5_EXT_BASE, $49), F5_PAD(1))
    .define T5_EXT_4A               = F5_ADD(F5_OUT(T5_EXT_BASE, $4A), F5_PAD(1))
    .define T5_EXT_4B               = F5_ADD(F5_OUT(T5_EXT_BASE, $4B), F5_PAD(1))
    .define T5_EXT_4C               = F5_ADD(F5_OUT(T5_EXT_BASE, $4C), F5_PAD(1))
    .define T5_EXT_4D               = F5_ADD(F5_OUT(T5_EXT_BASE, $4D), F5_PAD(1))
    .define T5_EXT_4E               = F5_ADD(F5_OUT(T5_EXT_BASE, $4E), F5_PAD(1))
    .define T5_EXT_4F               = F5_ADD(F5_OUT(T5_EXT_BASE, $4F), F5_PAD(1))
    .define T5_EXT_50               = F5_ADD(F5_OUT(T5_EXT_BASE, $50), F5_PAD(1))
    .define T5_EXT_51               = F5_ADD(F5_OUT(T5_EXT_BASE, $51), F5_PAD(1))
    .define T5_EXT_52               = F5_ADD(F5_OUT(T5_EXT_BASE, $52), F5_PAD(1))
    .define T5_EXT_53               = F5_ADD(F5_OUT(T5_EXT_BASE, $53), F5_PAD(1))
    .define T5_EXT_54               = F5_ADD(F5_OUT(T5_EXT_BASE, $54), F5_PAD(1))
    .define T5_EXT_55               = F5_ADD(F5_OUT(T5_EXT_BASE, $55), F5_PAD(1))
    .define T5_EXT_56               = F5_ADD(F5_OUT(T5_EXT_BASE, $56), F5_PAD(1))
    .define T5_EXT_57               = F5_ADD(F5_OUT(T5_EXT_BASE, $57), F5_PAD(1))
    .define T5_EXT_58               = F5_ADD(F5_OUT(T5_EXT_BASE, $58), F5_PAD(1))
    .define T5_EXT_59               = F5_ADD(F5_OUT(T5_EXT_BASE, $59), F5_PAD(1))

    .db "99>"               ; @BT TEST-99 99 START

    ; --- Immediate ---
    adca #T5_IMM_01               ; @BT 89 01
    adcb #T5_IMM_02               ; @BT C9 02
    adda #T5_IMM_03               ; @BT 8B 03
    addb #T5_IMM_04               ; @BT CB 04
    anda #T5_IMM_05               ; @BT 84 05
    andb #T5_IMM_06               ; @BT C4 06
    bita #T5_IMM_07               ; @BT 85 07
    bitb #T5_IMM_08               ; @BT C5 08
    cmpa #T5_IMM_09               ; @BT 81 09
    cmpb #T5_IMM_0A               ; @BT C1 0A
    eora #T5_IMM_0B               ; @BT 88 0B
    eorb #T5_IMM_0C               ; @BT C8 0C
    ldaa #T5_IMM_0D               ; @BT 86 0D
    ldab #T5_IMM_0E               ; @BT C6 0E
    oraa #T5_IMM_0F               ; @BT 8A 0F
    orab #T5_IMM_10               ; @BT CA 10
    sbca #T5_IMM_11               ; @BT 82 11
    sbcb #T5_IMM_12               ; @BT C2 12
    suba #T5_IMM_13               ; @BT 80 13
    subb #T5_IMM_14               ; @BT C0 14
    cpx #T5_EXT_34                ; @BT 8C 12 34
    lds #T5_EXT_35                ; @BT 8E 12 35
    ldx #T5_EXT_36                ; @BT CE 12 36

    ; --- Inherent ---
    aba                      ; @BT 1B
    cba                      ; @BT 11
    clc                      ; @BT 0C
    cli                      ; @BT 0E
    clra                     ; @BT 4F
    clrb                     ; @BT 5F
    clv                      ; @BT 0A
    coma                     ; @BT 43
    comb                     ; @BT 53
    daa                      ; @BT 19
    deca                     ; @BT 4A
    decb                     ; @BT 5A
    des                      ; @BT 34
    dex                      ; @BT 09
    inca                     ; @BT 4C
    incb                     ; @BT 5C
    ins                      ; @BT 31
    inx                      ; @BT 08
    asla                     ; @BT 48
    aslb                     ; @BT 58
    asra                     ; @BT 47
    asrb                     ; @BT 57
    lsra                     ; @BT 44
    lsrb                     ; @BT 54
    nega                     ; @BT 40
    negb                     ; @BT 50
    nop                      ; @BT 01
    psha                     ; @BT 36
    pshb                     ; @BT 37
    pula                     ; @BT 32
    pulb                     ; @BT 33
    rola                     ; @BT 49
    rolb                     ; @BT 59
    rora                     ; @BT 46
    rorb                     ; @BT 56
    rti                      ; @BT 3B
    rts                      ; @BT 39
    sba                      ; @BT 10
    sec                      ; @BT 0D
    sei                      ; @BT 0F
    sev                      ; @BT 0B
    swi                      ; @BT 3F
    tab                      ; @BT 16
    tap                      ; @BT 06
    tba                      ; @BT 17
    tpa                      ; @BT 07
    tsta                     ; @BT 4D
    tstb                     ; @BT 5D
    tsx                      ; @BT 30
    txs                      ; @BT 35
    wai                      ; @BT 3E

    ; --- Indexed ---
    adca T5_IDX_30,x               ; @BT A9 30
    adcb T5_IDX_31,x               ; @BT E9 31
    adda T5_IDX_32,x               ; @BT AB 32
    addb T5_IDX_33,x               ; @BT EB 33
    anda T5_IDX_34,x               ; @BT A4 34
    andb T5_IDX_35,x               ; @BT E4 35
    bita T5_IDX_36,x               ; @BT A5 36
    bitb T5_IDX_37,x               ; @BT E5 37
    cmpa T5_IDX_38,x               ; @BT A1 38
    cmpb T5_IDX_39,x               ; @BT E1 39
    eora T5_IDX_3A,x               ; @BT A8 3A
    eorb T5_IDX_3B,x               ; @BT E8 3B
    ldaa T5_IDX_3C,x               ; @BT A6 3C
    ldab T5_IDX_3D,x               ; @BT E6 3D
    oraa T5_IDX_3E,x               ; @BT AA 3E
    orab T5_IDX_3F,x               ; @BT EA 3F
    sbca T5_IDX_40,x               ; @BT A2 40
    sbcb T5_IDX_41,x               ; @BT E2 41
    suba T5_IDX_42,x               ; @BT A0 42
    subb T5_IDX_43,x               ; @BT E0 43
    staa T5_IDX_44,x               ; @BT A7 44
    stab T5_IDX_45,x               ; @BT E7 45
    sts T5_IDX_46,x                ; @BT AF 46
    stx T5_IDX_47,x                ; @BT EF 47
    cpx T5_IDX_48,x                ; @BT AC 48
    lds T5_IDX_49,x                ; @BT AE 49
    ldx T5_IDX_4A,x                ; @BT EE 4A
    asl T5_IDX_4B,x                ; @BT 68 4B
    asr T5_IDX_4C,x                ; @BT 67 4C
    lsr T5_IDX_4D,x                ; @BT 64 4D
    rol T5_IDX_4E,x                ; @BT 69 4E
    ror T5_IDX_4F,x                ; @BT 66 4F
    inc T5_IDX_50,x                ; @BT 6C 50
    dec 1,x                ; @BT 6A 01
    neg 2,x                ; @BT 60 02
    com 3,x                ; @BT 63 03
    clr 4,x                ; @BT 6F 04
    tst 5,x                ; @BT 6D 05
    jmp 6,x                ; @BT 6E 06
    jsr 7,x                ; @BT AD 07

    ; --- Direct ---
    adca T5_DIR_10                 ; @BT 99 10
    adcb T5_DIR_11                 ; @BT D9 11
    adda T5_DIR_12                 ; @BT 9B 12
    addb T5_DIR_13                 ; @BT DB 13
    anda T5_DIR_14                 ; @BT 94 14
    andb T5_DIR_15                 ; @BT D4 15
    bita T5_DIR_16                 ; @BT 95 16
    bitb T5_DIR_17                 ; @BT D5 17
    cmpa T5_DIR_18                 ; @BT 91 18
    cmpb T5_DIR_19                 ; @BT D1 19
    eora T5_DIR_1A                 ; @BT 98 1A
    eorb T5_DIR_1B                 ; @BT D8 1B
    ldaa T5_DIR_1C                 ; @BT 96 1C
    ldab T5_DIR_1D                 ; @BT D6 1D
    oraa T5_DIR_1E                 ; @BT 9A 1E
    orab T5_DIR_1F                 ; @BT DA 1F
    sbca T5_DIR_20                 ; @BT 92 20
    sbcb T5_DIR_21                 ; @BT D2 21
    suba T5_DIR_22                 ; @BT 90 22
    subb T5_DIR_23                 ; @BT D0 23
    staa T5_DIR_24                 ; @BT 97 24
    stab T5_DIR_25                 ; @BT D7 25
    sts T5_DIR_26                  ; @BT 9F 26
    stx T5_DIR_27                  ; @BT DF 27
    cpx T5_DIR_28                  ; @BT 9C 28
    lds T5_DIR_29                  ; @BT 9E 29
    ldx T5_DIR_2A                  ; @BT DE 2A

    ; --- Extended ---
    adca T5_EXT_34                 ; @BT B9 12 34
    adcb T5_EXT_35                 ; @BT F9 12 35
    adda T5_EXT_36                 ; @BT BB 12 36
    addb T5_EXT_37                 ; @BT FB 12 37
    anda T5_EXT_38                 ; @BT B4 12 38
    andb T5_EXT_39                 ; @BT F4 12 39
    bita T5_EXT_3A                 ; @BT B5 12 3A
    bitb T5_EXT_3B                 ; @BT F5 12 3B
    cmpa T5_EXT_3C                 ; @BT B1 12 3C
    cmpb T5_EXT_3D                 ; @BT F1 12 3D
    eora T5_EXT_3E                 ; @BT B8 12 3E
    eorb T5_EXT_3F                 ; @BT F8 12 3F
    ldaa T5_EXT_40                 ; @BT B6 12 40
    ldab T5_EXT_41                 ; @BT F6 12 41
    oraa T5_EXT_42                 ; @BT BA 12 42
    orab T5_EXT_43                 ; @BT FA 12 43
    sbca T5_EXT_44                 ; @BT B2 12 44
    sbcb T5_EXT_45                 ; @BT F2 12 45
    suba T5_EXT_46                 ; @BT B0 12 46
    subb T5_EXT_47                 ; @BT F0 12 47
    staa T5_EXT_48                 ; @BT B7 12 48
    stab T5_EXT_49                 ; @BT F7 12 49
    sts T5_EXT_4A                  ; @BT BF 12 4A
    stx T5_EXT_4B                  ; @BT FF 12 4B
    cpx T5_EXT_4C                  ; @BT BC 12 4C
    lds T5_EXT_4D                  ; @BT BE 12 4D
    ldx T5_EXT_4E                  ; @BT FE 12 4E
    asl T5_EXT_4F                  ; @BT 78 12 4F
    asr T5_EXT_50                  ; @BT 77 12 50
    lsr T5_EXT_51                  ; @BT 74 12 51
    rol T5_EXT_52                  ; @BT 79 12 52
    ror T5_EXT_53                  ; @BT 76 12 53
    inc T5_EXT_54                  ; @BT 7C 12 54
    dec T5_EXT_55                  ; @BT 7A 12 55
    neg T5_EXT_56                  ; @BT 70 12 56
    com T5_EXT_57                  ; @BT 73 12 57
    clr T5_EXT_58                  ; @BT 7F 12 58
    tst T5_EXT_59                  ; @BT 7D 12 59
    jmp T5_JMP_ADDR                ; @BT 7E 20 00
    jsr T5_JSR_ADDR                ; @BT BD 34 56

    ; --- Branches (relative offsets computed from fixed layout) ---
    ; All branches target the same forward-local label '+'. Since each branch
    ; is at a different address, the relative offset differs.
    bcc +                    ; @BT 24 1E
    bcs +                    ; @BT 25 1C
    beq +                    ; @BT 27 1A
    bge +                    ; @BT 2C 18
    bgt +                    ; @BT 2E 16
    bhi +                    ; @BT 22 14
    ble +                    ; @BT 2F 12
    bls +                    ; @BT 23 10
    blt +                    ; @BT 2D 0E
    bmi +                    ; @BT 2B 0C
    bne +                    ; @BT 26 0A
    bpl +                    ; @BT 2A 08
    bra +                    ; @BT 20 06
    bsr +                    ; @BT 8D 04
    bvc +                    ; @BT 28 02
    bvs +                    ; @BT 29 00

+

    .db "<99"               ; @BT END
