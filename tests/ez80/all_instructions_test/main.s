.MEMORYMAP
        SLOTSIZE $8000
        DEFAULTSLOT 0
        SLOT 0 START $0000
.ENDME

.ROMBANKMAP
        BANKSTOTAL 1
        BANKSIZE $8000
        BANKS 1
.ENDRO

.EMPTYFILL $ff

; @BT linked.rom

    ; Auto-generated from iez80.c and checked byte-for-byte with byte_tester.
    ; Covers 1049 concrete instruction table rows; skips the linker-deferred RST fallback and sentinel rows.
    ; TEST-02 rewrites TEST-01 numeric operands as label/define/function expressions.

    .BANK 0 SLOT 0
    .ORG 0

    .db "01>"               ; @BT TEST-01 01 START


    ; ADC
        adc a,a                         ; @BT 8F
        adc a,b                         ; @BT 88
        adc a,c                         ; @BT 89
        adc a,d                         ; @BT 8A
        adc a,e                         ; @BT 8B
        adc a,h                         ; @BT 8C
        adc a,ixh                       ; @BT DD 8C
        adc a,ixl                       ; @BT DD 8D
        adc a,iyh                       ; @BT FD 8C
        adc a,iyl                       ; @BT FD 8D
        adc a,(hl)                      ; @BT 8E
        adc a,(ix+$2f)                  ; @BT DD 8E 2F
        adc a,(iy+$3c)                  ; @BT FD 8E 3C
        adc a,l                         ; @BT 8D
        adc a,$1e                       ; @BT CE 1E
        adc hl,bc                       ; @BT ED 4A
        adc hl,de                       ; @BT ED 5A
        adc hl,hl                       ; @BT ED 6A
        adc hl,sp                       ; @BT ED 7A
        adc a                           ; @BT 8F
        adc b                           ; @BT 88
        adc c                           ; @BT 89
        adc d                           ; @BT 8A
        adc e                           ; @BT 8B
        adc h                           ; @BT 8C
        adc ixh                         ; @BT DD 8C
        adc ixl                         ; @BT DD 8D
        adc iyh                         ; @BT FD 8C
        adc iyl                         ; @BT FD 8D
        adc (hl)                        ; @BT 8E
        adc (ix+$46)                    ; @BT DD 8E 46
        adc (iy+$53)                    ; @BT FD 8E 53
        adc l                           ; @BT 8D
        adc $11                         ; @BT CE 11

    ; ADD
        add a,a                         ; @BT 87
        add a,b                         ; @BT 80
        add a,c                         ; @BT 81
        add a,d                         ; @BT 82
        add a,e                         ; @BT 83
        add a,h                         ; @BT 84
        add a,ixh                       ; @BT DD 84
        add a,ixl                       ; @BT DD 85
        add a,iyh                       ; @BT FD 84
        add a,iyl                       ; @BT FD 85
        add a,(hl)                      ; @BT 86
        add a,(ix+$29)                  ; @BT DD 86 29
        add a,(iy+$36)                  ; @BT FD 86 36
        add a,l                         ; @BT 85
        add a,$30                       ; @BT C6 30
        add hl,bc                       ; @BT 09
        add hl,de                       ; @BT 19
        add hl,hl                       ; @BT 29
        add hl,sp                       ; @BT 39
        add ix,bc                       ; @BT DD 09
        add ix,de                       ; @BT DD 19
        add ix,ix                       ; @BT DD 29
        add ix,sp                       ; @BT DD 39
        add iy,bc                       ; @BT FD 09
        add iy,de                       ; @BT FD 19
        add iy,iy                       ; @BT FD 29
        add iy,sp                       ; @BT FD 39
        add a                           ; @BT 87
        add b                           ; @BT 80
        add c                           ; @BT 81
        add d                           ; @BT 82
        add e                           ; @BT 83
        add h                           ; @BT 84
        add ixh                         ; @BT DD 84
        add ixl                         ; @BT DD 85
        add iyh                         ; @BT FD 84
        add iyl                         ; @BT FD 85
        add (hl)                        ; @BT 86
        add (ix+$38)                    ; @BT DD 86 38
        add (iy+$45)                    ; @BT FD 86 45
        add l                           ; @BT 85
        add $3b                         ; @BT C6 3B

    ; AND
        and a,a                         ; @BT A7
        and a,b                         ; @BT A0
        and a,c                         ; @BT A1
        and a,d                         ; @BT A2
        and a,e                         ; @BT A3
        and a,h                         ; @BT A4
        and a,ixh                       ; @BT DD A4
        and a,ixl                       ; @BT DD A5
        and a,iyh                       ; @BT FD A4
        and a,iyl                       ; @BT FD A5
        and a,(hl)                      ; @BT A6
        and a,(ix+$1b)                  ; @BT DD A6 1B
        and a,(iy+$28)                  ; @BT FD A6 28
        and a,l                         ; @BT A5
        and a,$5a                       ; @BT E6 5A
        and a                           ; @BT A7
        and b                           ; @BT A0
        and c                           ; @BT A1
        and d                           ; @BT A2
        and e                           ; @BT A3
        and h                           ; @BT A4
        and ixh                         ; @BT DD A4
        and ixl                         ; @BT DD A5
        and iyh                         ; @BT FD A4
        and iyl                         ; @BT FD A5
        and (hl)                        ; @BT A6
        and (ix+$6e)                    ; @BT DD A6 6E
        and (iy+$7b)                    ; @BT FD A6 7B
        and l                           ; @BT A5
        and $79                         ; @BT E6 79

    ; BIT
        bit 0,a                         ; @BT CB 47
        bit 0,b                         ; @BT CB 40
        bit 0,c                         ; @BT CB 41
        bit 0,d                         ; @BT CB 42
        bit 0,e                         ; @BT CB 43
        bit 0,h                         ; @BT CB 44
        bit 0,(hl)                      ; @BT CB 46
        bit 0,(ix+$1d)                  ; @BT DD CB 1D 46
        bit 0,(iy+$2a)                  ; @BT FD CB 2A 46
        bit 0,l                         ; @BT CB 45
        bit 1,a                         ; @BT CB 4F
        bit 1,b                         ; @BT CB 48
        bit 1,c                         ; @BT CB 49
        bit 1,d                         ; @BT CB 4A
        bit 1,e                         ; @BT CB 4B
        bit 1,h                         ; @BT CB 4C
        bit 1,(hl)                      ; @BT CB 4E
        bit 1,(ix+$2f)                  ; @BT DD CB 2F 4E
        bit 1,(iy+$3c)                  ; @BT FD CB 3C 4E
        bit 1,l                         ; @BT CB 4D
        bit 2,a                         ; @BT CB 57
        bit 2,b                         ; @BT CB 50
        bit 2,c                         ; @BT CB 51
        bit 2,d                         ; @BT CB 52
        bit 2,e                         ; @BT CB 53
        bit 2,h                         ; @BT CB 54
        bit 2,(hl)                      ; @BT CB 56
        bit 2,(ix+$41)                  ; @BT DD CB 41 56
        bit 2,(iy+$4e)                  ; @BT FD CB 4E 56
        bit 2,l                         ; @BT CB 55
        bit 3,a                         ; @BT CB 5F
        bit 3,b                         ; @BT CB 58
        bit 3,c                         ; @BT CB 59
        bit 3,d                         ; @BT CB 5A
        bit 3,e                         ; @BT CB 5B
        bit 3,h                         ; @BT CB 5C
        bit 3,(hl)                      ; @BT CB 5E
        bit 3,(ix+$53)                  ; @BT DD CB 53 5E
        bit 3,(iy+$60)                  ; @BT FD CB 60 5E
        bit 3,l                         ; @BT CB 5D
        bit 4,a                         ; @BT CB 67
        bit 4,b                         ; @BT CB 60
        bit 4,c                         ; @BT CB 61
        bit 4,d                         ; @BT CB 62
        bit 4,e                         ; @BT CB 63
        bit 4,h                         ; @BT CB 64
        bit 4,(hl)                      ; @BT CB 66
        bit 4,(ix+$65)                  ; @BT DD CB 65 66
        bit 4,(iy+$72)                  ; @BT FD CB 72 66
        bit 4,l                         ; @BT CB 65
        bit 5,a                         ; @BT CB 6F
        bit 5,b                         ; @BT CB 68
        bit 5,c                         ; @BT CB 69
        bit 5,d                         ; @BT CB 6A
        bit 5,e                         ; @BT CB 6B
        bit 5,h                         ; @BT CB 6C
        bit 5,(hl)                      ; @BT CB 6E
        bit 5,(ix+$77)                  ; @BT DD CB 77 6E
        bit 5,(iy+$14)                  ; @BT FD CB 14 6E
        bit 5,l                         ; @BT CB 6D
        bit 6,a                         ; @BT CB 77
        bit 6,b                         ; @BT CB 70
        bit 6,c                         ; @BT CB 71
        bit 6,d                         ; @BT CB 72
        bit 6,e                         ; @BT CB 73
        bit 6,h                         ; @BT CB 74
        bit 6,(hl)                      ; @BT CB 76
        bit 6,(ix+$19)                  ; @BT DD CB 19 76
        bit 6,(iy+$26)                  ; @BT FD CB 26 76
        bit 6,l                         ; @BT CB 75
        bit 7,a                         ; @BT CB 7F
        bit 7,b                         ; @BT CB 78
        bit 7,c                         ; @BT CB 79
        bit 7,d                         ; @BT CB 7A
        bit 7,e                         ; @BT CB 7B
        bit 7,h                         ; @BT CB 7C
        bit 7,(hl)                      ; @BT CB 7E
        bit 7,(ix+$2b)                  ; @BT DD CB 2B 7E
        bit 7,(iy+$38)                  ; @BT FD CB 38 7E
        bit 7,l                         ; @BT CB 7D

    ; CALL
        call lss,$2ae2                  ; @BT DC E2 2A
        call lst,$2b07                  ; @BT DC 07 2B
        call neg,$2b2c                  ; @BT FC 2C 2B
        call gte,$2b51                  ; @BT D4 51 2B
        call geq,$2b76                  ; @BT D4 76 2B
        call neq,$2b9b                  ; @BT C4 9B 2B
        call pos,$2bc0                  ; @BT F4 C0 2B
        call equ,$2be5                  ; @BT CC E5 2B
        call c,$2c0a                    ; @BT DC 0A 2C
        call m,$2c2f                    ; @BT FC 2F 2C
        call nc,$2c54                   ; @BT D4 54 2C
        call nz,$2c79                   ; @BT C4 79 2C
        call pe,$2c9e                   ; @BT EC 9E 2C
        call po,$2cc3                   ; @BT E4 C3 2C
        call p,$2ce8                    ; @BT F4 E8 2C
        call z,$2d0d                    ; @BT CC 0D 2D
        call $2d32                      ; @BT CD 32 2D

    ; CCF
        ccf                             ; @BT 3F

    ; CP
        cp a,a                          ; @BT BF
        cp a,b                          ; @BT B8
        cp a,c                          ; @BT B9
        cp a,d                          ; @BT BA
        cp a,e                          ; @BT BB
        cp a,h                          ; @BT BC
        cp a,ixh                        ; @BT DD BC
        cp a,ixl                        ; @BT DD BD
        cp a,iyh                        ; @BT FD BC
        cp a,iyl                        ; @BT FD BD
        cp a,(hl)                       ; @BT BE
        cp a,(ix+$7b)                   ; @BT DD BE 7B
        cp a,(iy+$18)                   ; @BT FD BE 18
        cp a,l                          ; @BT BD
        cp a,$1a                        ; @BT FE 1A
        cp a                            ; @BT BF
        cp b                            ; @BT B8
        cp c                            ; @BT B9
        cp d                            ; @BT BA
        cp e                            ; @BT BB
        cp h                            ; @BT BC
        cp ixh                          ; @BT DD BC
        cp ixl                          ; @BT DD BD
        cp iyh                          ; @BT FD BC
        cp iyl                          ; @BT FD BD
        cp (hl)                         ; @BT BE
        cp (ix+$5e)                     ; @BT DD BE 5E
        cp (iy+$6b)                     ; @BT FD BE 6B
        cp l                            ; @BT BD
        cp $39                          ; @BT FE 39

    ; CPDR
        cpdr                            ; @BT ED B9

    ; CPD
        cpd                             ; @BT ED A9

    ; CPIR
        cpir                            ; @BT ED B1

    ; CPI
        cpi                             ; @BT ED A1

    ; CPL
        cpl                             ; @BT 2F

    ; DAA
        daa                             ; @BT 27

    ; DEC
        dec a                           ; @BT 3D
        dec b                           ; @BT 05
        dec bc                          ; @BT 0B
        dec c                           ; @BT 0D
        dec d                           ; @BT 15
        dec de                          ; @BT 1B
        dec e                           ; @BT 1D
        dec h                           ; @BT 25
        dec hl                          ; @BT 2B
        dec (hl)                        ; @BT 35
        dec ix                          ; @BT DD 2B
        dec ixh                         ; @BT DD 25
        dec ixl                         ; @BT DD 2D
        dec (ix+$39)                    ; @BT DD 35 39
        dec iy                          ; @BT FD 2B
        dec iyh                         ; @BT FD 25
        dec iyl                         ; @BT FD 2D
        dec (iy+$6d)                    ; @BT FD 35 6D
        dec l                           ; @BT 2D
        dec sp                          ; @BT 3B

    ; DI
        di                              ; @BT F3

    ; DJNZ
        djnz $55                        ; @BT 10 55

    ; EI
        ei                              ; @BT FB

    ; EX
        ex af',af                       ; @BT 08
        ex af,af'                       ; @BT 08
        ex de,hl                        ; @BT EB
        ex hl,de                        ; @BT EB
        ex (sp),hl                      ; @BT E3
        ex (sp),ix                      ; @BT DD E3
        ex (sp),iy                      ; @BT FD E3
        ex hl,(sp)                      ; @BT E3
        ex ix,(sp)                      ; @BT DD E3
        ex iy,(sp)                      ; @BT FD E3

    ; EXX
        exx                             ; @BT D9

    ; HALT
        halt                            ; @BT 76

    ; IM
        im 0                            ; @BT ED 46
        im 1                            ; @BT ED 56
        im 2                            ; @BT ED 5E

    ; IN
        in a,(c)                        ; @BT ED 78
        in a,($37)                      ; @BT DB 37
        in a,$48                        ; @BT DB 48
        in b,(c)                        ; @BT ED 40
        in c,(c)                        ; @BT ED 48
        in d,(c)                        ; @BT ED 50
        in e,(c)                        ; @BT ED 58
        in f,(c)                        ; @BT ED 70
        in h,(c)                        ; @BT ED 60
        in l,(c)                        ; @BT ED 68
        in (c)                          ; @BT ED 70

    ; IN0
        in0 a,($71)                     ; @BT ED 38 71
        in0 b,($12)                     ; @BT ED 00 12
        in0 c,($23)                     ; @BT ED 08 23
        in0 d,($34)                     ; @BT ED 10 34
        in0 e,($45)                     ; @BT ED 18 45
        in0 h,($56)                     ; @BT ED 20 56
        in0 l,($67)                     ; @BT ED 28 67

    ; INC
        inc a                           ; @BT 3C
        inc b                           ; @BT 04
        inc bc                          ; @BT 03
        inc c                           ; @BT 0C
        inc d                           ; @BT 14
        inc de                          ; @BT 13
        inc e                           ; @BT 1C
        inc h                           ; @BT 24
        inc hl                          ; @BT 23
        inc (hl)                        ; @BT 34
        inc ix                          ; @BT DD 23
        inc ixh                         ; @BT DD 24
        inc ixl                         ; @BT DD 2C
        inc (ix+$71)                    ; @BT DD 34 71
        inc iy                          ; @BT FD 23
        inc iyh                         ; @BT FD 24
        inc iyl                         ; @BT FD 2C
        inc (iy+$35)                    ; @BT FD 34 35
        inc l                           ; @BT 2C
        inc sp                          ; @BT 33

    ; IND
        ind                             ; @BT ED AA

    ; IND2
        ind2                            ; @BT ED 8C

    ; IND2R
        ind2r                           ; @BT ED 9C

    ; INDM
        indm                            ; @BT ED 8A

    ; INDMR
        indmr                           ; @BT ED 9A

    ; INDR
        indr                            ; @BT ED BA

    ; INDRX
        indrx                           ; @BT ED CA

    ; INI
        ini                             ; @BT ED A2

    ; INI2
        ini2                            ; @BT ED 84

    ; INI2R
        ini2r                           ; @BT ED 94

    ; INIM
        inim                            ; @BT ED 82

    ; INIMR
        inimr                           ; @BT ED 92

    ; INIR
        inir                            ; @BT ED B2

    ; INIRX
        inirx                           ; @BT ED C2

    ; JP
        jp (hl)                         ; @BT E9
        jp (ix)                         ; @BT DD E9
        jp (iy)                         ; @BT FD E9
        jp hl                           ; @BT E9
        jp ix                           ; @BT DD E9
        jp iy                           ; @BT FD E9
        jp lss,$4090                    ; @BT DA 90 40
        jp lst,$40b5                    ; @BT DA B5 40
        jp neg,$40da                    ; @BT FA DA 40
        jp gte,$40ff                    ; @BT D2 FF 40
        jp geq,$4124                    ; @BT D2 24 41
        jp neq,$4149                    ; @BT C2 49 41
        jp pos,$416e                    ; @BT F2 6E 41
        jp equ,$4193                    ; @BT CA 93 41
        jp c,$41b8                      ; @BT DA B8 41
        jp m,$41dd                      ; @BT FA DD 41
        jp nc,$4202                     ; @BT D2 02 42
        jp nz,$4227                     ; @BT C2 27 42
        jp pe,$424c                     ; @BT EA 4C 42
        jp po,$4271                     ; @BT E2 71 42
        jp p,$4296                      ; @BT F2 96 42
        jp z,$42bb                      ; @BT CA BB 42
        jp $42e0                        ; @BT C3 E0 42

    ; JR
        jr lss,$51                      ; @BT 38 51
        jr lst,$62                      ; @BT 38 62
        jr gte,$73                      ; @BT 30 73
        jr geq,$14                      ; @BT 30 14
        jr neq,$25                      ; @BT 20 25
        jr equ,$36                      ; @BT 28 36
        jr c,$47                        ; @BT 38 47
        jr nc,$58                       ; @BT 30 58
        jr nz,$69                       ; @BT 20 69
        jr z,$7a                        ; @BT 28 7A
        jr $1b                          ; @BT 18 1B

    ; LEA
        lea bc,ix+$2c                   ; @BT ED 02 2C
        lea bc,iy+$39                   ; @BT ED 03 39
        lea de,ix+$46                   ; @BT ED 12 46
        lea de,iy+$53                   ; @BT ED 13 53
        lea hl,ix+$60                   ; @BT ED 22 60
        lea hl,iy+$6d                   ; @BT ED 23 6D
        lea ix,ix+$7a                   ; @BT ED 32 7A
        lea iy,iy+$17                   ; @BT ED 33 17

    ; LD
        ld a,a                          ; @BT 7F
        ld a,b                          ; @BT 78
        ld a,(bc)                       ; @BT 0A
        ld a,c                          ; @BT 79
        ld a,d                          ; @BT 7A
        ld a,(de)                       ; @BT 1A
        ld a,e                          ; @BT 7B
        ld a,h                          ; @BT 7C
        ld a,(hl)                       ; @BT 7E
        ld a,i                          ; @BT ED 57
        ld a,(ix+$36)                   ; @BT DD 7E 36
        ld a,(iy+$43)                   ; @BT FD 7E 43
        ld a,ixh                        ; @BT DD 7C
        ld a,ixl                        ; @BT DD 7D
        ld a,iyh                        ; @BT FD 7C
        ld a,iyl                        ; @BT FD 7D
        ld a,l                          ; @BT 7D
        ld a,mb                         ; @BT ED 6E
        ld a,r                          ; @BT ED 5F
        ld a,($4883)                    ; @BT 3A 83 48
        ld a,$48                        ; @BT 3E 48
        ld b,a                          ; @BT 47
        ld b,b                          ; @BT 40
        ld b,c                          ; @BT 41
        ld bc,(hl)                      ; @BT ED 07
        ld bc,(ix+$19)                  ; @BT DD 07 19
        ld bc,(iy+$26)                  ; @BT FD 07 26
        ld bc,($49ab)                   ; @BT ED 4B AB 49
        ld bc,$49d0                     ; @BT 01 D0 49
        ld (bc),a                       ; @BT 02
        ld b,d                          ; @BT 42
        ld b,e                          ; @BT 43
        ld b,h                          ; @BT 44
        ld b,(hl)                       ; @BT 46
        ld b,(ix+$1e)                   ; @BT DD 46 1E
        ld b,(iy+$2b)                   ; @BT FD 46 2B
        ld b,ixh                        ; @BT DD 44
        ld b,ixl                        ; @BT DD 45
        ld b,iyh                        ; @BT FD 44
        ld b,iyl                        ; @BT FD 45
        ld b,l                          ; @BT 45
        ld b,$5d                        ; @BT 06 5D
        ld c,a                          ; @BT 4F
        ld c,b                          ; @BT 48
        ld c,c                          ; @BT 49
        ld c,d                          ; @BT 4A
        ld c,e                          ; @BT 4B
        ld c,h                          ; @BT 4C
        ld c,(hl)                       ; @BT 4E
        ld c,(ix+$71)                   ; @BT DD 4E 71
        ld c,(iy+$7e)                   ; @BT FD 4E 7E
        ld c,ixh                        ; @BT DD 4C
        ld c,ixl                        ; @BT DD 4D
        ld c,iyh                        ; @BT FD 4C
        ld c,iyl                        ; @BT FD 4D
        ld c,l                          ; @BT 4D
        ld c,$7c                        ; @BT 0E 7C
        ld d,a                          ; @BT 57
        ld d,b                          ; @BT 50
        ld d,c                          ; @BT 51
        ld d,d                          ; @BT 52
        ld d,e                          ; @BT 53
        ld de,(hl)                      ; @BT ED 17
        ld de,(ix+$47)                  ; @BT DD 17 47
        ld de,(iy+$54)                  ; @BT FD 17 54
        ld de,($4f29)                   ; @BT ED 5B 29 4F
        ld de,$4f4e                     ; @BT 11 4E 4F
        ld (de),a                       ; @BT 12
        ld d,h                          ; @BT 54
        ld d,(hl)                       ; @BT 56
        ld d,(ix+$32)                   ; @BT DD 56 32
        ld d,(iy+$3f)                   ; @BT FD 56 3F
        ld d,ixh                        ; @BT DD 54
        ld d,ixl                        ; @BT DD 55
        ld d,iyh                        ; @BT FD 54
        ld d,iyl                        ; @BT FD 55
        ld d,l                          ; @BT 55
        ld d,$21                        ; @BT 16 21
        ld e,a                          ; @BT 5F
        ld e,b                          ; @BT 58
        ld e,c                          ; @BT 59
        ld e,d                          ; @BT 5A
        ld e,e                          ; @BT 5B
        ld e,h                          ; @BT 5C
        ld e,(hl)                       ; @BT 5E
        ld e,(ix+$15)                   ; @BT DD 5E 15
        ld e,(iy+$22)                   ; @BT FD 5E 22
        ld e,ixh                        ; @BT DD 5C
        ld e,ixl                        ; @BT DD 5D
        ld e,iyh                        ; @BT FD 5C
        ld e,iyl                        ; @BT FD 5D
        ld e,l                          ; @BT 5D
        ld e,$40                        ; @BT 1E 40
        ld h,a                          ; @BT 67
        ld h,b                          ; @BT 60
        ld h,c                          ; @BT 61
        ld h,d                          ; @BT 62
        ld h,e                          ; @BT 63
        ld h,h                          ; @BT 64
        ld h,(hl)                       ; @BT 66
        ld h,(ix+$68)                   ; @BT DD 66 68
        ld h,(iy+$75)                   ; @BT FD 66 75
        ld h,l                          ; @BT 65
        ld h,$1b                        ; @BT 26 1B
        ld hl,(hl)                      ; @BT ED 27
        ld hl,(ix+$39)                  ; @BT DD 27 39
        ld hl,(iy+$46)                  ; @BT FD 27 46
        ld hl,($553b)                   ; @BT 2A 3B 55
        ld hl,i                         ; @BT ED D7
        ld hl,$5585                     ; @BT 21 85 55
        ld (hl),a                       ; @BT 77
        ld (hl),b                       ; @BT 70
        ld (hl),bc                      ; @BT ED 0F
        ld (hl),c                       ; @BT 71
        ld (hl),d                       ; @BT 72
        ld (hl),de                      ; @BT ED 1F
        ld (hl),e                       ; @BT 73
        ld (hl),h                       ; @BT 74
        ld (hl),hl                      ; @BT ED 2F
        ld (hl),ix                      ; @BT ED 3F
        ld (hl),iy                      ; @BT ED 3E
        ld (hl),l                       ; @BT 75
        ld (hl),$7e                     ; @BT 36 7E
        ld l,a                          ; @BT 6F
        ld l,b                          ; @BT 68
        ld l,c                          ; @BT 69
        ld l,d                          ; @BT 6A
        ld l,e                          ; @BT 6B
        ld l,h                          ; @BT 6C
        ld l,(hl)                       ; @BT 6E
        ld l,(ix+$2e)                   ; @BT DD 6E 2E
        ld l,(iy+$3b)                   ; @BT FD 6E 3B
        ld l,l                          ; @BT 6D
        ld l,$59                        ; @BT 2E 59
        ld r,a                          ; @BT ED 4F
        ld sp,hl                        ; @BT F9
        ld sp,ix                        ; @BT DD F9
        ld sp,iy                        ; @BT FD F9
        ld sp,($59b6)                   ; @BT ED 7B B6 59
        ld sp,$59db                     ; @BT 31 DB 59
        ld i,a                          ; @BT ED 47
        ld i,hl                         ; @BT ED C7
        ld ix,(hl)                      ; @BT ED 37
        ld ix,(ix+$67)                  ; @BT DD 37 67
        ld ix,(iy+$74)                  ; @BT FD 31 74
        ld ix,($5ab9)                   ; @BT DD 2A B9 5A
        ld ix,$5ade                     ; @BT DD 21 DE 5A
        ld ixh,a                        ; @BT DD 67
        ld ixh,b                        ; @BT DD 60
        ld ixh,c                        ; @BT DD 61
        ld ixh,d                        ; @BT DD 62
        ld ixh,e                        ; @BT DD 63
        ld ixh,ixh                      ; @BT DD 64
        ld ixh,ixl                      ; @BT DD 65
        ld ixh,$6e                      ; @BT DD 26 6E
        ld ixl,a                        ; @BT DD 6F
        ld ixl,b                        ; @BT DD 68
        ld ixl,c                        ; @BT DD 69
        ld ixl,d                        ; @BT DD 6A
        ld ixl,e                        ; @BT DD 6B
        ld ixl,ixh                      ; @BT DD 6C
        ld ixl,ixl                      ; @BT DD 6D
        ld ixl,$16                      ; @BT DD 2E 16
        ld (ix+$1b),a                   ; @BT DD 77 1B
        ld (ix+$28),b                   ; @BT DD 70 28
        ld (ix+$35),bc                  ; @BT DD 0F 35
        ld (ix+$42),c                   ; @BT DD 71 42
        ld (ix+$4f),d                   ; @BT DD 72 4F
        ld (ix+$5c),de                  ; @BT DD 1F 5C
        ld (ix+$69),e                   ; @BT DD 73 69
        ld (ix+$76),h                   ; @BT DD 74 76
        ld (ix+$13),hl                  ; @BT DD 2F 13
        ld (ix+$20),ix                  ; @BT DD 3F 20
        ld (ix+$2d),iy                  ; @BT DD 3E 2D
        ld (ix+$3a),l                   ; @BT DD 75 3A
        ld (ix+$47),$13                 ; @BT DD 36 47 13
        ld iy,(hl)                      ; @BT ED 31
        ld iy,(ix+$61)                  ; @BT DD 31 61
        ld iy,(iy+$6e)                  ; @BT FD 37 6E
        ld iy,($5fa3)                   ; @BT FD 2A A3 5F
        ld iy,$5fc8                     ; @BT FD 21 C8 5F
        ld iyh,a                        ; @BT FD 67
        ld iyh,b                        ; @BT FD 60
        ld iyh,c                        ; @BT FD 61
        ld iyh,d                        ; @BT FD 62
        ld iyh,e                        ; @BT FD 63
        ld iyh,iyh                      ; @BT FD 64
        ld iyh,iyl                      ; @BT FD 65
        ld iyh,$10                      ; @BT FD 26 10
        ld iyl,a                        ; @BT FD 6F
        ld iyl,b                        ; @BT FD 68
        ld iyl,c                        ; @BT FD 69
        ld iyl,d                        ; @BT FD 6A
        ld iyl,e                        ; @BT FD 6B
        ld iyl,iyh                      ; @BT FD 6C
        ld iyl,iyl                      ; @BT FD 6D
        ld iyl,$28                      ; @BT FD 2E 28
        ld (iy+$15),a                   ; @BT FD 77 15
        ld (iy+$22),b                   ; @BT FD 70 22
        ld (iy+$2f),bc                  ; @BT FD 0F 2F
        ld (iy+$3c),c                   ; @BT FD 71 3C
        ld (iy+$49),d                   ; @BT FD 72 49
        ld (iy+$56),de                  ; @BT FD 1F 56
        ld (iy+$63),e                   ; @BT FD 73 63
        ld (iy+$70),h                   ; @BT FD 74 70
        ld (iy+$7d),hl                  ; @BT FD 2F 7D
        ld (iy+$1a),ix                  ; @BT FD 3E 1A
        ld (iy+$27),iy                  ; @BT FD 3F 27
        ld (iy+$34),l                   ; @BT FD 75 34
        ld (iy+$41),$25                 ; @BT FD 36 41 25
        ld ($641e),a                    ; @BT 32 1E 64
        ld ($6443),bc                   ; @BT ED 43 43 64
        ld ($6468),de                   ; @BT ED 53 68 64
        ld ($648d),hl                   ; @BT 22 8D 64
        ld ($64b2),sp                   ; @BT ED 73 B2 64
        ld ($64d7),iy                   ; @BT FD 22 D7 64
        ld ($64fc),ix                   ; @BT DD 22 FC 64
        ld pc,hl                        ; @BT E9
        ld pc,ix                        ; @BT DD E9
        ld pc,iy                        ; @BT FD E9
        ld mb,a                         ; @BT ED 6D

    ; LDDR
        lddr                            ; @BT ED B8

    ; LDD
        ldd                             ; @BT ED A8

    ; LDIR
        ldir                            ; @BT ED B0

    ; LDI
        ldi                             ; @BT ED A0

    ; MLT
        mlt bc                          ; @BT ED 4C
        mlt de                          ; @BT ED 5C
        mlt hl                          ; @BT ED 6C
        mlt sp                          ; @BT ED 7C

    ; NEG
        neg                             ; @BT ED 44

    ; NOP
        nop                             ; @BT 00

    ; OR
        or a,a                          ; @BT B7
        or a,b                          ; @BT B0
        or a,c                          ; @BT B1
        or a,d                          ; @BT B2
        or a,e                          ; @BT B3
        or a,h                          ; @BT B4
        or a,ixh                        ; @BT DD B4
        or a,ixl                        ; @BT DD B5
        or a,iyh                        ; @BT FD B4
        or a,iyl                        ; @BT FD B5
        or a,(hl)                       ; @BT B6
        or a,(ix+$2e)                   ; @BT DD B6 2E
        or a,(iy+$3b)                   ; @BT FD B6 3B
        or a,l                          ; @BT B5
        or a,$59                        ; @BT F6 59
        or a                            ; @BT B7
        or b                            ; @BT B0
        or c                            ; @BT B1
        or d                            ; @BT B2
        or e                            ; @BT B3
        or h                            ; @BT B4
        or ixh                          ; @BT DD B4
        or ixl                          ; @BT DD B5
        or iyh                          ; @BT FD B4
        or iyl                          ; @BT FD B5
        or (hl)                         ; @BT B6
        or (ix+$11)                     ; @BT DD B6 11
        or (iy+$1e)                     ; @BT FD B6 1E
        or l                            ; @BT B5
        or $78                          ; @BT F6 78

    ; OTD2R
        otd2r                           ; @BT ED BC

    ; OTDM
        otdm                            ; @BT ED 8B

    ; OTDMR
        otdmr                           ; @BT ED 9B

    ; OTDR
        otdr                            ; @BT ED BB

    ; OTDRX
        otdrx                           ; @BT ED CB

    ; OTI2R
        oti2r                           ; @BT ED B4

    ; OTIM
        otim                            ; @BT ED 83

    ; OTIMR
        otimr                           ; @BT ED 93

    ; OTIR
        otir                            ; @BT ED B3

    ; OTIRX
        otirx                           ; @BT ED C3

    ; OUT
        out (c),0                       ; @BT ED 71
        out (c),a                       ; @BT ED 79
        out (c),b                       ; @BT ED 41
        out (c),c                       ; @BT ED 49
        out (c),d                       ; @BT ED 51
        out (c),e                       ; @BT ED 59
        out (c),h                       ; @BT ED 61
        out (c),l                       ; @BT ED 69
        out ($6b),a                     ; @BT D3 6B

    ; OUTD
        outd                            ; @BT ED AB

    ; OUTD2
        outd2                           ; @BT ED AC

    ; OUTI
        outi                            ; @BT ED A3

    ; OUTI2
        outi2                           ; @BT ED A4

    ; OUT0
        out0 ($50),a                    ; @BT ED 39 50
        out0 ($61),b                    ; @BT ED 01 61
        out0 ($72),c                    ; @BT ED 09 72
        out0 ($13),d                    ; @BT ED 11 13
        out0 ($24),e                    ; @BT ED 19 24
        out0 ($35),h                    ; @BT ED 21 35
        out0 ($46),l                    ; @BT ED 29 46

    ; PEA
        pea ix+$7b                      ; @BT ED 65 7B
        pea iy+$18                      ; @BT ED 66 18

    ; POP
        pop af                          ; @BT F1
        pop bc                          ; @BT C1
        pop de                          ; @BT D1
        pop hl                          ; @BT E1
        pop ix                          ; @BT DD E1
        pop iy                          ; @BT FD E1

    ; PUSH
        push af                         ; @BT F5
        push bc                         ; @BT C5
        push de                         ; @BT D5
        push hl                         ; @BT E5
        push ix                         ; @BT DD E5
        push iy                         ; @BT FD E5

    ; RES
        res 0,a                         ; @BT CB 87
        res 0,b                         ; @BT CB 80
        res 0,c                         ; @BT CB 81
        res 0,d                         ; @BT CB 82
        res 0,e                         ; @BT CB 83
        res 0,h                         ; @BT CB 84
        res 0,(hl)                      ; @BT CB 86
        res 0,(ix+$3c)                  ; @BT DD CB 3C 86
        res 0,(iy+$49)                  ; @BT FD CB 49 86
        res 0,l                         ; @BT CB 85
        res 1,a                         ; @BT CB 8F
        res 1,b                         ; @BT CB 88
        res 1,c                         ; @BT CB 89
        res 1,d                         ; @BT CB 8A
        res 1,e                         ; @BT CB 8B
        res 1,h                         ; @BT CB 8C
        res 1,(hl)                      ; @BT CB 8E
        res 1,(ix+$4e)                  ; @BT DD CB 4E 8E
        res 1,(iy+$5b)                  ; @BT FD CB 5B 8E
        res 1,l                         ; @BT CB 8D
        res 2,a                         ; @BT CB 97
        res 2,b                         ; @BT CB 90
        res 2,c                         ; @BT CB 91
        res 2,d                         ; @BT CB 92
        res 2,e                         ; @BT CB 93
        res 2,h                         ; @BT CB 94
        res 2,(hl)                      ; @BT CB 96
        res 2,(ix+$60)                  ; @BT DD CB 60 96
        res 2,(iy+$6d)                  ; @BT FD CB 6D 96
        res 2,l                         ; @BT CB 95
        res 3,a                         ; @BT CB 9F
        res 3,b                         ; @BT CB 98
        res 3,c                         ; @BT CB 99
        res 3,d                         ; @BT CB 9A
        res 3,e                         ; @BT CB 9B
        res 3,h                         ; @BT CB 9C
        res 3,(hl)                      ; @BT CB 9E
        res 3,(ix+$72)                  ; @BT DD CB 72 9E
        res 3,(iy+$7f)                  ; @BT FD CB 7F 9E
        res 3,l                         ; @BT CB 9D
        res 4,a                         ; @BT CB A7
        res 4,b                         ; @BT CB A0
        res 4,c                         ; @BT CB A1
        res 4,d                         ; @BT CB A2
        res 4,e                         ; @BT CB A3
        res 4,h                         ; @BT CB A4
        res 4,(hl)                      ; @BT CB A6
        res 4,(ix+$14)                  ; @BT DD CB 14 A6
        res 4,(iy+$21)                  ; @BT FD CB 21 A6
        res 4,l                         ; @BT CB A5
        res 5,a                         ; @BT CB AF
        res 5,b                         ; @BT CB A8
        res 5,c                         ; @BT CB A9
        res 5,d                         ; @BT CB AA
        res 5,e                         ; @BT CB AB
        res 5,h                         ; @BT CB AC
        res 5,(hl)                      ; @BT CB AE
        res 5,(ix+$26)                  ; @BT DD CB 26 AE
        res 5,(iy+$33)                  ; @BT FD CB 33 AE
        res 5,l                         ; @BT CB AD
        res 6,a                         ; @BT CB B7
        res 6,b                         ; @BT CB B0
        res 6,c                         ; @BT CB B1
        res 6,d                         ; @BT CB B2
        res 6,e                         ; @BT CB B3
        res 6,h                         ; @BT CB B4
        res 6,(hl)                      ; @BT CB B6
        res 6,(ix+$38)                  ; @BT DD CB 38 B6
        res 6,(iy+$45)                  ; @BT FD CB 45 B6
        res 6,l                         ; @BT CB B5
        res 7,a                         ; @BT CB BF
        res 7,b                         ; @BT CB B8
        res 7,c                         ; @BT CB B9
        res 7,d                         ; @BT CB BA
        res 7,e                         ; @BT CB BB
        res 7,h                         ; @BT CB BC
        res 7,(hl)                      ; @BT CB BE
        res 7,(ix+$4a)                  ; @BT DD CB 4A BE
        res 7,(iy+$57)                  ; @BT FD CB 57 BE
        res 7,l                         ; @BT CB BD

    ; RET
        ret lss                         ; @BT D8
        ret lst                         ; @BT D8
        ret neg                         ; @BT F8
        ret gte                         ; @BT D0
        ret geq                         ; @BT D0
        ret neq                         ; @BT C0
        ret pos                         ; @BT F0
        ret equ                         ; @BT C8
        ret c                           ; @BT D8
        ret m                           ; @BT F8
        ret nc                          ; @BT D0
        ret nz                          ; @BT C0
        ret p                           ; @BT F0
        ret pe                          ; @BT E8
        ret po                          ; @BT E0
        ret z                           ; @BT C8
        ret                             ; @BT C9

    ; RETI
        reti                            ; @BT ED 4D

    ; RETN
        retn                            ; @BT ED 45

    ; RL
        rl a                            ; @BT CB 17
        rl b                            ; @BT CB 10
        rl c                            ; @BT CB 11
        rl d                            ; @BT CB 12
        rl e                            ; @BT CB 13
        rl h                            ; @BT CB 14
        rl (hl)                         ; @BT CB 16
        rl (ix+$73)                     ; @BT DD CB 73 16
        rl (iy+$10)                     ; @BT FD CB 10 16
        rl l                            ; @BT CB 15

    ; RLC
        rlc a                           ; @BT CB 07
        rlc b                           ; @BT CB 00
        rlc c                           ; @BT CB 01
        rlc d                           ; @BT CB 02
        rlc e                           ; @BT CB 03
        rlc h                           ; @BT CB 04
        rlc (hl)                        ; @BT CB 06
        rlc (ix+$15)                    ; @BT DD CB 15 06
        rlc (iy+$22)                    ; @BT FD CB 22 06
        rlc l                           ; @BT CB 05

    ; RLCA
        rlca                            ; @BT 07

    ; RLA
        rla                             ; @BT 17

    ; RLD
        rld                             ; @BT ED 6F

    ; RR
        rr a                            ; @BT CB 1F
        rr b                            ; @BT CB 18
        rr c                            ; @BT CB 19
        rr d                            ; @BT CB 1A
        rr e                            ; @BT CB 1B
        rr h                            ; @BT CB 1C
        rr (hl)                         ; @BT CB 1E
        rr (ix+$4e)                     ; @BT DD CB 4E 1E
        rr (iy+$5b)                     ; @BT FD CB 5B 1E
        rr l                            ; @BT CB 1D

    ; RRA
        rra                             ; @BT 1F

    ; RRC
        rrc a                           ; @BT CB 0F
        rrc b                           ; @BT CB 08
        rrc c                           ; @BT CB 09
        rrc d                           ; @BT CB 0A
        rrc e                           ; @BT CB 0B
        rrc h                           ; @BT CB 0C
        rrc (hl)                        ; @BT CB 0E
        rrc (ix+$6d)                    ; @BT DD CB 6D 0E
        rrc (iy+$7a)                    ; @BT FD CB 7A 0E
        rrc l                           ; @BT CB 0D

    ; RRCA
        rrca                            ; @BT 0F

    ; RRD
        rrd                             ; @BT ED 67

    ; RST
        rst $00                         ; @BT C7
        rst $08                         ; @BT CF
        rst $10                         ; @BT D7
        rst $18                         ; @BT DF
        rst $20                         ; @BT E7
        rst $28                         ; @BT EF
        rst $30                         ; @BT F7
        rst $38                         ; @BT FF
        rst                             ; @BT C7

    ; RSMIX
        rsmix                           ; @BT ED 7E

    ; SBC
        sbc a,a                         ; @BT 9F
        sbc a,b                         ; @BT 98
        sbc a,c                         ; @BT 99
        sbc a,d                         ; @BT 9A
        sbc a,e                         ; @BT 9B
        sbc a,h                         ; @BT 9C
        sbc a,ixh                       ; @BT DD 9C
        sbc a,ixl                       ; @BT DD 9D
        sbc a,iyh                       ; @BT FD 9C
        sbc a,iyl                       ; @BT FD 9D
        sbc a,(hl)                      ; @BT 9E
        sbc a,(ix+$6f)                  ; @BT DD 9E 6F
        sbc a,(iy+$7c)                  ; @BT FD 9E 7C
        sbc a,l                         ; @BT 9D
        sbc a,$3e                       ; @BT DE 3E
        sbc hl,bc                       ; @BT ED 42
        sbc hl,de                       ; @BT ED 52
        sbc hl,hl                       ; @BT ED 62
        sbc hl,sp                       ; @BT ED 72
        sbc a                           ; @BT 9F
        sbc b                           ; @BT 98
        sbc c                           ; @BT 99
        sbc d                           ; @BT 9A
        sbc e                           ; @BT 9B
        sbc h                           ; @BT 9C
        sbc ixh                         ; @BT DD 9C
        sbc ixl                         ; @BT DD 9D
        sbc iyh                         ; @BT FD 9C
        sbc iyl                         ; @BT FD 9D
        sbc (hl)                        ; @BT 9E
        sbc (ix+$16)                    ; @BT DD 9E 16
        sbc (iy+$23)                    ; @BT FD 9E 23
        sbc l                           ; @BT 9D
        sbc $31                         ; @BT DE 31

    ; SCF
        scf                             ; @BT 37

    ; SLP
        slp                             ; @BT ED 76

    ; STMIX
        stmix                           ; @BT ED 7D

    ; SET
        set 0,a                         ; @BT CB C7
        set 0,b                         ; @BT CB C0
        set 0,c                         ; @BT CB C1
        set 0,d                         ; @BT CB C2
        set 0,e                         ; @BT CB C3
        set 0,h                         ; @BT CB C4
        set 0,(hl)                      ; @BT CB C6
        set 0,(ix+$5c)                  ; @BT DD CB 5C C6
        set 0,(iy+$69)                  ; @BT FD CB 69 C6
        set 0,l                         ; @BT CB C5
        set 1,a                         ; @BT CB CF
        set 1,b                         ; @BT CB C8
        set 1,c                         ; @BT CB C9
        set 1,d                         ; @BT CB CA
        set 1,e                         ; @BT CB CB
        set 1,h                         ; @BT CB CC
        set 1,(hl)                      ; @BT CB CE
        set 1,(ix+$6e)                  ; @BT DD CB 6E CE
        set 1,(iy+$7b)                  ; @BT FD CB 7B CE
        set 1,l                         ; @BT CB CD
        set 2,a                         ; @BT CB D7
        set 2,b                         ; @BT CB D0
        set 2,c                         ; @BT CB D1
        set 2,d                         ; @BT CB D2
        set 2,e                         ; @BT CB D3
        set 2,h                         ; @BT CB D4
        set 2,(hl)                      ; @BT CB D6
        set 2,(ix+$10)                  ; @BT DD CB 10 D6
        set 2,(iy+$1d)                  ; @BT FD CB 1D D6
        set 2,l                         ; @BT CB D5
        set 3,a                         ; @BT CB DF
        set 3,b                         ; @BT CB D8
        set 3,c                         ; @BT CB D9
        set 3,d                         ; @BT CB DA
        set 3,e                         ; @BT CB DB
        set 3,h                         ; @BT CB DC
        set 3,(hl)                      ; @BT CB DE
        set 3,(ix+$22)                  ; @BT DD CB 22 DE
        set 3,(iy+$2f)                  ; @BT FD CB 2F DE
        set 3,l                         ; @BT CB DD
        set 4,a                         ; @BT CB E7
        set 4,b                         ; @BT CB E0
        set 4,c                         ; @BT CB E1
        set 4,d                         ; @BT CB E2
        set 4,e                         ; @BT CB E3
        set 4,h                         ; @BT CB E4
        set 4,(hl)                      ; @BT CB E6
        set 4,(ix+$34)                  ; @BT DD CB 34 E6
        set 4,(iy+$41)                  ; @BT FD CB 41 E6
        set 4,l                         ; @BT CB E5
        set 5,a                         ; @BT CB EF
        set 5,b                         ; @BT CB E8
        set 5,c                         ; @BT CB E9
        set 5,d                         ; @BT CB EA
        set 5,e                         ; @BT CB EB
        set 5,h                         ; @BT CB EC
        set 5,(hl)                      ; @BT CB EE
        set 5,(ix+$46)                  ; @BT DD CB 46 EE
        set 5,(iy+$53)                  ; @BT FD CB 53 EE
        set 5,l                         ; @BT CB ED
        set 6,a                         ; @BT CB F7
        set 6,b                         ; @BT CB F0
        set 6,c                         ; @BT CB F1
        set 6,d                         ; @BT CB F2
        set 6,e                         ; @BT CB F3
        set 6,h                         ; @BT CB F4
        set 6,(hl)                      ; @BT CB F6
        set 6,(ix+$58)                  ; @BT DD CB 58 F6
        set 6,(iy+$65)                  ; @BT FD CB 65 F6
        set 6,l                         ; @BT CB F5
        set 7,a                         ; @BT CB FF
        set 7,b                         ; @BT CB F8
        set 7,c                         ; @BT CB F9
        set 7,d                         ; @BT CB FA
        set 7,e                         ; @BT CB FB
        set 7,h                         ; @BT CB FC
        set 7,(hl)                      ; @BT CB FE
        set 7,(ix+$6a)                  ; @BT DD CB 6A FE
        set 7,(iy+$77)                  ; @BT FD CB 77 FE
        set 7,l                         ; @BT CB FD

    ; SLA
        sla a                           ; @BT CB 27
        sla b                           ; @BT CB 20
        sla c                           ; @BT CB 21
        sla d                           ; @BT CB 22
        sla e                           ; @BT CB 23
        sla h                           ; @BT CB 24
        sla (hl)                        ; @BT CB 26
        sla (ix+$7c)                    ; @BT DD CB 7C 26
        sla (iy+$19)                    ; @BT FD CB 19 26
        sla l                           ; @BT CB 25

    ; SRA
        sra a                           ; @BT CB 2F
        sra b                           ; @BT CB 28
        sra c                           ; @BT CB 29
        sra d                           ; @BT CB 2A
        sra e                           ; @BT CB 2B
        sra h                           ; @BT CB 2C
        sra (hl)                        ; @BT CB 2E
        sra (ix+$1e)                    ; @BT DD CB 1E 2E
        sra (iy+$2b)                    ; @BT FD CB 2B 2E
        sra l                           ; @BT CB 2D

    ; SRL
        srl a                           ; @BT CB 3F
        srl b                           ; @BT CB 38
        srl c                           ; @BT CB 39
        srl d                           ; @BT CB 3A
        srl e                           ; @BT CB 3B
        srl h                           ; @BT CB 3C
        srl (hl)                        ; @BT CB 3E
        srl (ix+$30)                    ; @BT DD CB 30 3E
        srl (iy+$3d)                    ; @BT FD CB 3D 3E
        srl l                           ; @BT CB 3D

    ; SUB
        sub a,a                         ; @BT 97
        sub a,b                         ; @BT 90
        sub a,c                         ; @BT 91
        sub a,d                         ; @BT 92
        sub a,e                         ; @BT 93
        sub a,h                         ; @BT 94
        sub a,ixh                       ; @BT DD 94
        sub a,ixl                       ; @BT DD 95
        sub a,iyh                       ; @BT FD 94
        sub a,iyl                       ; @BT FD 95
        sub a,l                         ; @BT 95
        sub a,(ix+$76)                  ; @BT DD 96 76
        sub a,(iy+$13)                  ; @BT FD 96 13
        sub a,(hl)                      ; @BT 96
        sub a,$61                       ; @BT D6 61
        sub a                           ; @BT 97
        sub b                           ; @BT 90
        sub c                           ; @BT 91
        sub d                           ; @BT 92
        sub e                           ; @BT 93
        sub h                           ; @BT 94
        sub ixh                         ; @BT DD 94
        sub ixl                         ; @BT DD 95
        sub iyh                         ; @BT FD 94
        sub iyl                         ; @BT FD 95
        sub (hl)                        ; @BT 96
        sub (ix+$59)                    ; @BT DD 96 59
        sub (iy+$66)                    ; @BT FD 96 66
        sub l                           ; @BT 95
        sub $10                         ; @BT D6 10

    ; TST
        tst a,a                         ; @BT ED 3C
        tst a,b                         ; @BT ED 04
        tst a,c                         ; @BT ED 0C
        tst a,d                         ; @BT ED 14
        tst a,e                         ; @BT ED 1C
        tst a,h                         ; @BT ED 24
        tst a,(hl)                      ; @BT ED 34
        tst a,l                         ; @BT ED 2C
        tst a,$39                       ; @BT ED 64 39

    ; TSTIO
        tstio $4a                       ; @BT ED 74 4A

    ; XOR
        xor a,a                         ; @BT AF
        xor a,b                         ; @BT A8
        xor a,c                         ; @BT A9
        xor a,d                         ; @BT AA
        xor a,e                         ; @BT AB
        xor a,h                         ; @BT AC
        xor a,ixh                       ; @BT DD AC
        xor a,ixl                       ; @BT DD AD
        xor a,iyh                       ; @BT FD AC
        xor a,iyl                       ; @BT FD AD
        xor a,(hl)                      ; @BT AE
        xor a,(ix+$4e)                  ; @BT DD AE 4E
        xor a,(iy+$5b)                  ; @BT FD AE 5B
        xor a,l                         ; @BT AD
        xor a,$69                       ; @BT EE 69
        xor a                           ; @BT AF
        xor b                           ; @BT A8
        xor c                           ; @BT A9
        xor d                           ; @BT AA
        xor e                           ; @BT AB
        xor h                           ; @BT AC
        xor ixh                         ; @BT DD AC
        xor ixl                         ; @BT DD AD
        xor iyh                         ; @BT FD AC
        xor iyl                         ; @BT FD AD
        xor (hl)                        ; @BT AE
        xor (ix+$31)                    ; @BT DD AE 31
        xor (iy+$3e)                    ; @BT FD AE 3E
        xor l                           ; @BT AD
        xor $18                         ; @BT EE 18

    .db "<01"               ; @BT END

    /////////////////////////////////////////////////////////////////////////////
    // TEST-02: label/define/function expressions for numeric operands
    /////////////////////////////////////////////////////////////////////////////

    ; TEST-02 generated labels. .ENUM creates symbols without emitting bytes.
    .ENUM $00
T2_L_000000 db
    .ENDE
    .ENUM $01
T2_L_000001 db
    .ENDE
    .ENUM $02
T2_L_000002 db
    .ENDE
    .ENUM $03
T2_L_000003 db
    .ENDE
    .ENUM $04
T2_L_000004 db
    .ENDE
    .ENUM $05
T2_L_000005 db
    .ENDE
    .ENUM $06
T2_L_000006 db
    .ENDE
    .ENUM $07
T2_L_000007 db
    .ENDE
    .ENUM $08
T2_L_000008 db
    .ENDE
    .ENUM $10
T2_L_000010 db
    .ENDE
    .ENUM $11
T2_L_000011 db
    .ENDE
    .ENUM $12
T2_L_000012 db
    .ENDE
    .ENUM $13
T2_L_000013 db
    .ENDE
    .ENUM $14
T2_L_000014 db
    .ENDE
    .ENUM $15
T2_L_000015 db
    .ENDE
    .ENUM $16
T2_L_000016 db
    .ENDE
    .ENUM $17
T2_L_000017 db
    .ENDE
    .ENUM $18
T2_L_000018 db
    .ENDE
    .ENUM $19
T2_L_000019 db
    .ENDE
    .ENUM $1A
T2_L_00001A db
    .ENDE
    .ENUM $1B
T2_L_00001B db
    .ENDE
    .ENUM $1D
T2_L_00001D db
    .ENDE
    .ENUM $1E
T2_L_00001E db
    .ENDE
    .ENUM $20
T2_L_000020 db
    .ENDE
    .ENUM $21
T2_L_000021 db
    .ENDE
    .ENUM $22
T2_L_000022 db
    .ENDE
    .ENUM $23
T2_L_000023 db
    .ENDE
    .ENUM $24
T2_L_000024 db
    .ENDE
    .ENUM $25
T2_L_000025 db
    .ENDE
    .ENUM $26
T2_L_000026 db
    .ENDE
    .ENUM $27
T2_L_000027 db
    .ENDE
    .ENUM $28
T2_L_000028 db
    .ENDE
    .ENUM $29
T2_L_000029 db
    .ENDE
    .ENUM $2A
T2_L_00002A db
    .ENDE
    .ENUM $2B
T2_L_00002B db
    .ENDE
    .ENUM $2C
T2_L_00002C db
    .ENDE
    .ENUM $2D
T2_L_00002D db
    .ENDE
    .ENUM $2E
T2_L_00002E db
    .ENDE
    .ENUM $2F
T2_L_00002F db
    .ENDE
    .ENUM $30
T2_L_000030 db
    .ENDE
    .ENUM $31
T2_L_000031 db
    .ENDE
    .ENUM $32
T2_L_000032 db
    .ENDE
    .ENUM $33
T2_L_000033 db
    .ENDE
    .ENUM $34
T2_L_000034 db
    .ENDE
    .ENUM $35
T2_L_000035 db
    .ENDE
    .ENUM $36
T2_L_000036 db
    .ENDE
    .ENUM $37
T2_L_000037 db
    .ENDE
    .ENUM $38
T2_L_000038 db
    .ENDE
    .ENUM $39
T2_L_000039 db
    .ENDE
    .ENUM $3A
T2_L_00003A db
    .ENDE
    .ENUM $3B
T2_L_00003B db
    .ENDE
    .ENUM $3C
T2_L_00003C db
    .ENDE
    .ENUM $3D
T2_L_00003D db
    .ENDE
    .ENUM $3E
T2_L_00003E db
    .ENDE
    .ENUM $3F
T2_L_00003F db
    .ENDE
    .ENUM $40
T2_L_000040 db
    .ENDE
    .ENUM $41
T2_L_000041 db
    .ENDE
    .ENUM $42
T2_L_000042 db
    .ENDE
    .ENUM $43
T2_L_000043 db
    .ENDE
    .ENUM $45
T2_L_000045 db
    .ENDE
    .ENUM $46
T2_L_000046 db
    .ENDE
    .ENUM $47
T2_L_000047 db
    .ENDE
    .ENUM $48
T2_L_000048 db
    .ENDE
    .ENUM $49
T2_L_000049 db
    .ENDE
    .ENUM $4A
T2_L_00004A db
    .ENDE
    .ENUM $4E
T2_L_00004E db
    .ENDE
    .ENUM $4F
T2_L_00004F db
    .ENDE
    .ENUM $50
T2_L_000050 db
    .ENDE
    .ENUM $51
T2_L_000051 db
    .ENDE
    .ENUM $53
T2_L_000053 db
    .ENDE
    .ENUM $54
T2_L_000054 db
    .ENDE
    .ENUM $55
T2_L_000055 db
    .ENDE
    .ENUM $56
T2_L_000056 db
    .ENDE
    .ENUM $57
T2_L_000057 db
    .ENDE
    .ENUM $58
T2_L_000058 db
    .ENDE
    .ENUM $59
T2_L_000059 db
    .ENDE
    .ENUM $5A
T2_L_00005A db
    .ENDE
    .ENUM $5B
T2_L_00005B db
    .ENDE
    .ENUM $5C
T2_L_00005C db
    .ENDE
    .ENUM $5D
T2_L_00005D db
    .ENDE
    .ENUM $5E
T2_L_00005E db
    .ENDE
    .ENUM $60
T2_L_000060 db
    .ENDE
    .ENUM $61
T2_L_000061 db
    .ENDE
    .ENUM $62
T2_L_000062 db
    .ENDE
    .ENUM $63
T2_L_000063 db
    .ENDE
    .ENUM $65
T2_L_000065 db
    .ENDE
    .ENUM $66
T2_L_000066 db
    .ENDE
    .ENUM $67
T2_L_000067 db
    .ENDE
    .ENUM $68
T2_L_000068 db
    .ENDE
    .ENUM $69
T2_L_000069 db
    .ENDE
    .ENUM $6A
T2_L_00006A db
    .ENDE
    .ENUM $6B
T2_L_00006B db
    .ENDE
    .ENUM $6D
T2_L_00006D db
    .ENDE
    .ENUM $6E
T2_L_00006E db
    .ENDE
    .ENUM $6F
T2_L_00006F db
    .ENDE
    .ENUM $70
T2_L_000070 db
    .ENDE
    .ENUM $71
T2_L_000071 db
    .ENDE
    .ENUM $72
T2_L_000072 db
    .ENDE
    .ENUM $73
T2_L_000073 db
    .ENDE
    .ENUM $74
T2_L_000074 db
    .ENDE
    .ENUM $75
T2_L_000075 db
    .ENDE
    .ENUM $76
T2_L_000076 db
    .ENDE
    .ENUM $77
T2_L_000077 db
    .ENDE
    .ENUM $78
T2_L_000078 db
    .ENDE
    .ENUM $79
T2_L_000079 db
    .ENDE
    .ENUM $7A
T2_L_00007A db
    .ENDE
    .ENUM $7B
T2_L_00007B db
    .ENDE
    .ENUM $7C
T2_L_00007C db
    .ENDE
    .ENUM $7D
T2_L_00007D db
    .ENDE
    .ENUM $7E
T2_L_00007E db
    .ENDE
    .ENUM $7F
T2_L_00007F db
    .ENDE
    .ENUM $2AE2
T2_L_002AE2 db
    .ENDE
    .ENUM $2B07
T2_L_002B07 db
    .ENDE
    .ENUM $2B2C
T2_L_002B2C db
    .ENDE
    .ENUM $2B51
T2_L_002B51 db
    .ENDE
    .ENUM $2B76
T2_L_002B76 db
    .ENDE
    .ENUM $2B9B
T2_L_002B9B db
    .ENDE
    .ENUM $2BC0
T2_L_002BC0 db
    .ENDE
    .ENUM $2BE5
T2_L_002BE5 db
    .ENDE
    .ENUM $2C0A
T2_L_002C0A db
    .ENDE
    .ENUM $2C2F
T2_L_002C2F db
    .ENDE
    .ENUM $2C54
T2_L_002C54 db
    .ENDE
    .ENUM $2C79
T2_L_002C79 db
    .ENDE
    .ENUM $2C9E
T2_L_002C9E db
    .ENDE
    .ENUM $2CC3
T2_L_002CC3 db
    .ENDE
    .ENUM $2CE8
T2_L_002CE8 db
    .ENDE
    .ENUM $2D0D
T2_L_002D0D db
    .ENDE
    .ENUM $2D32
T2_L_002D32 db
    .ENDE
    .ENUM $4090
T2_L_004090 db
    .ENDE
    .ENUM $40B5
T2_L_0040B5 db
    .ENDE
    .ENUM $40DA
T2_L_0040DA db
    .ENDE
    .ENUM $40FF
T2_L_0040FF db
    .ENDE
    .ENUM $4124
T2_L_004124 db
    .ENDE
    .ENUM $4149
T2_L_004149 db
    .ENDE
    .ENUM $416E
T2_L_00416E db
    .ENDE
    .ENUM $4193
T2_L_004193 db
    .ENDE
    .ENUM $41B8
T2_L_0041B8 db
    .ENDE
    .ENUM $41DD
T2_L_0041DD db
    .ENDE
    .ENUM $4202
T2_L_004202 db
    .ENDE
    .ENUM $4227
T2_L_004227 db
    .ENDE
    .ENUM $424C
T2_L_00424C db
    .ENDE
    .ENUM $4271
T2_L_004271 db
    .ENDE
    .ENUM $4296
T2_L_004296 db
    .ENDE
    .ENUM $42BB
T2_L_0042BB db
    .ENDE
    .ENUM $42E0
T2_L_0042E0 db
    .ENDE
    .ENUM $4883
T2_L_004883 db
    .ENDE
    .ENUM $49AB
T2_L_0049AB db
    .ENDE
    .ENUM $49D0
T2_L_0049D0 db
    .ENDE
    .ENUM $4F29
T2_L_004F29 db
    .ENDE
    .ENUM $4F4E
T2_L_004F4E db
    .ENDE
    .ENUM $553B
T2_L_00553B db
    .ENDE
    .ENUM $5585
T2_L_005585 db
    .ENDE
    .ENUM $59B6
T2_L_0059B6 db
    .ENDE
    .ENUM $59DB
T2_L_0059DB db
    .ENDE
    .ENUM $5AB9
T2_L_005AB9 db
    .ENDE
    .ENUM $5ADE
T2_L_005ADE db
    .ENDE
    .ENUM $5FA3
T2_L_005FA3 db
    .ENDE
    .ENUM $5FC8
T2_L_005FC8 db
    .ENDE
    .ENUM $641E
T2_L_00641E db
    .ENDE
    .ENUM $6443
T2_L_006443 db
    .ENDE
    .ENUM $6468
T2_L_006468 db
    .ENDE
    .ENUM $648D
T2_L_00648D db
    .ENDE
    .ENUM $64B2
T2_L_0064B2 db
    .ENDE
    .ENUM $64D7
T2_L_0064D7 db
    .ENDE
    .ENUM $64FC
T2_L_0064FC db
    .ENDE

    ; TEST-02 generated defines/functions.
    .define T2_D_000000 $00
    .define T2_D_000001 $01
    .define T2_D_000002 $02
    .define T2_D_000003 $03
    .define T2_D_000004 $04
    .define T2_D_000005 $05
    .define T2_D_000006 $06
    .define T2_D_000007 $07
    .define T2_D_000008 $08
    .define T2_D_000010 $10
    .define T2_D_000011 $11
    .define T2_D_000012 $12
    .define T2_D_000013 $13
    .define T2_D_000014 $14
    .define T2_D_000015 $15
    .define T2_D_000016 $16
    .define T2_D_000017 $17
    .define T2_D_000018 $18
    .define T2_D_000019 $19
    .define T2_D_00001A $1A
    .define T2_D_00001B $1B
    .define T2_D_00001D $1D
    .define T2_D_00001E $1E
    .define T2_D_000020 $20
    .define T2_D_000021 $21
    .define T2_D_000022 $22
    .define T2_D_000023 $23
    .define T2_D_000024 $24
    .define T2_D_000025 $25
    .define T2_D_000026 $26
    .define T2_D_000027 $27
    .define T2_D_000028 $28
    .define T2_D_000029 $29
    .define T2_D_00002A $2A
    .define T2_D_00002B $2B
    .define T2_D_00002C $2C
    .define T2_D_00002D $2D
    .define T2_D_00002E $2E
    .define T2_D_00002F $2F
    .define T2_D_000030 $30
    .define T2_D_000031 $31
    .define T2_D_000032 $32
    .define T2_D_000033 $33
    .define T2_D_000034 $34
    .define T2_D_000035 $35
    .define T2_D_000036 $36
    .define T2_D_000037 $37
    .define T2_D_000038 $38
    .define T2_D_000039 $39
    .define T2_D_00003A $3A
    .define T2_D_00003B $3B
    .define T2_D_00003C $3C
    .define T2_D_00003D $3D
    .define T2_D_00003E $3E
    .define T2_D_00003F $3F
    .define T2_D_000040 $40
    .define T2_D_000041 $41
    .define T2_D_000042 $42
    .define T2_D_000043 $43
    .define T2_D_000045 $45
    .define T2_D_000046 $46
    .define T2_D_000047 $47
    .define T2_D_000048 $48
    .define T2_D_000049 $49
    .define T2_D_00004A $4A
    .define T2_D_00004E $4E
    .define T2_D_00004F $4F
    .define T2_D_000050 $50
    .define T2_D_000051 $51
    .define T2_D_000053 $53
    .define T2_D_000054 $54
    .define T2_D_000055 $55
    .define T2_D_000056 $56
    .define T2_D_000057 $57
    .define T2_D_000058 $58
    .define T2_D_000059 $59
    .define T2_D_00005A $5A
    .define T2_D_00005B $5B
    .define T2_D_00005C $5C
    .define T2_D_00005D $5D
    .define T2_D_00005E $5E
    .define T2_D_000060 $60
    .define T2_D_000061 $61
    .define T2_D_000062 $62
    .define T2_D_000063 $63
    .define T2_D_000065 $65
    .define T2_D_000066 $66
    .define T2_D_000067 $67
    .define T2_D_000068 $68
    .define T2_D_000069 $69
    .define T2_D_00006A $6A
    .define T2_D_00006B $6B
    .define T2_D_00006D $6D
    .define T2_D_00006E $6E
    .define T2_D_00006F $6F
    .define T2_D_000070 $70
    .define T2_D_000071 $71
    .define T2_D_000072 $72
    .define T2_D_000073 $73
    .define T2_D_000074 $74
    .define T2_D_000075 $75
    .define T2_D_000076 $76
    .define T2_D_000077 $77
    .define T2_D_000078 $78
    .define T2_D_000079 $79
    .define T2_D_00007A $7A
    .define T2_D_00007B $7B
    .define T2_D_00007C $7C
    .define T2_D_00007D $7D
    .define T2_D_00007E $7E
    .define T2_D_00007F $7F
    .define T2_D_002AE2 $2AE2
    .define T2_D_002B07 $2B07
    .define T2_D_002B2C $2B2C
    .define T2_D_002B51 $2B51
    .define T2_D_002B76 $2B76
    .define T2_D_002B9B $2B9B
    .define T2_D_002BC0 $2BC0
    .define T2_D_002BE5 $2BE5
    .define T2_D_002C0A $2C0A
    .define T2_D_002C2F $2C2F
    .define T2_D_002C54 $2C54
    .define T2_D_002C79 $2C79
    .define T2_D_002C9E $2C9E
    .define T2_D_002CC3 $2CC3
    .define T2_D_002CE8 $2CE8
    .define T2_D_002D0D $2D0D
    .define T2_D_002D32 $2D32
    .define T2_D_004090 $4090
    .define T2_D_0040B5 $40B5
    .define T2_D_0040DA $40DA
    .define T2_D_0040FF $40FF
    .define T2_D_004124 $4124
    .define T2_D_004149 $4149
    .define T2_D_00416E $416E
    .define T2_D_004193 $4193
    .define T2_D_0041B8 $41B8
    .define T2_D_0041DD $41DD
    .define T2_D_004202 $4202
    .define T2_D_004227 $4227
    .define T2_D_00424C $424C
    .define T2_D_004271 $4271
    .define T2_D_004296 $4296
    .define T2_D_0042BB $42BB
    .define T2_D_0042E0 $42E0
    .define T2_D_004883 $4883
    .define T2_D_0049AB $49AB
    .define T2_D_0049D0 $49D0
    .define T2_D_004F29 $4F29
    .define T2_D_004F4E $4F4E
    .define T2_D_00553B $553B
    .define T2_D_005585 $5585
    .define T2_D_0059B6 $59B6
    .define T2_D_0059DB $59DB
    .define T2_D_005AB9 $5AB9
    .define T2_D_005ADE $5ADE
    .define T2_D_005FA3 $5FA3
    .define T2_D_005FC8 $5FC8
    .define T2_D_00641E $641E
    .define T2_D_006443 $6443
    .define T2_D_006468 $6468
    .define T2_D_00648D $648D
    .define T2_D_0064B2 $64B2
    .define T2_D_0064D7 $64D7
    .define T2_D_0064FC $64FC
    .function F2_ID(a) (a)
    .function F2_ADD(a,b) (a+b)
    .function F2_SUB(a,b) (a-b)
    .function F2_MIX(a,b,c) ((a+b)-c)

    .db "02>"               ; @BT TEST-02 02 START

    ; GENERATED: copied from TEST-01 numeric operand rows with label/define/function replacement.
    ; GENERATED: rows = 474, unique values = 165

    ; ADC
        adc a,(ix+T2_L_00002F)          ; @BT DD 8E 2F
        adc a,(iy+T2_D_00003C)          ; @BT FD 8E 3C
        adc a,F2_ID(T2_L_00001E)        ; @BT CE 1E
        adc (ix+F2_ADD(T2_D_000046,T2_L_000000)) ; @BT DD 8E 46
        adc (iy+F2_SUB(T2_L_000053,T2_D_000000)) ; @BT FD 8E 53
        adc F2_MIX(T2_L_000011,T2_D_000011,T2_D_000011) ; @BT CE 11

    ; ADD
        add a,(ix+F2_MIX(T2_D_000029,T2_L_000000,T2_L_000000)) ; @BT DD 86 29
        add a,(iy+T2_L_000036)          ; @BT FD 86 36
        add a,T2_D_000030               ; @BT C6 30
        add (ix+F2_ID(T2_L_000038))     ; @BT DD 86 38
        add (iy+F2_ADD(T2_D_000045,T2_L_000000)) ; @BT FD 86 45
        add F2_SUB(T2_L_00003B,T2_D_000000) ; @BT C6 3B

    ; AND
        and a,(ix+F2_MIX(T2_L_00001B,T2_D_00001B,T2_D_00001B)) ; @BT DD A6 1B
        and a,(iy+F2_MIX(T2_D_000028,T2_L_000000,T2_L_000000)) ; @BT FD A6 28
        and a,T2_L_00005A               ; @BT E6 5A
        and (ix+T2_D_00006E)            ; @BT DD A6 6E
        and (iy+F2_ID(T2_L_00007B))     ; @BT FD A6 7B
        and F2_ADD(T2_D_000079,T2_L_000000) ; @BT E6 79

    ; BIT
        bit F2_SUB(T2_L_000000,T2_D_000000),a ; @BT CB 47
        bit F2_MIX(T2_L_000000,T2_D_000000,T2_D_000000),b ; @BT CB 40
        bit F2_MIX(T2_D_000000,T2_L_000000,T2_L_000000),c ; @BT CB 41
        bit T2_L_000000,d               ; @BT CB 42
        bit T2_D_000000,e               ; @BT CB 43
        bit F2_ID(T2_L_000000),h        ; @BT CB 44
        bit F2_ADD(T2_D_000000,T2_L_000000),(hl) ; @BT CB 46
        bit F2_MIX(T2_L_000000,T2_D_000000,T2_D_000000),(ix+F2_SUB(T2_L_00001D,T2_D_000000)) ; @BT DD CB 1D 46
        bit T2_L_000000,(iy+F2_MIX(T2_D_00002A,T2_L_000000,T2_L_000000)) ; @BT FD CB 2A 46
        bit T2_D_000000,l               ; @BT CB 45
        bit F2_ID(T2_L_000001),a        ; @BT CB 4F
        bit F2_ADD(T2_D_000001,T2_L_000000),b ; @BT CB 48
        bit F2_SUB(T2_L_000001,T2_D_000000),c ; @BT CB 49
        bit F2_MIX(T2_L_000001,T2_D_000001,T2_D_000001),d ; @BT CB 4A
        bit F2_MIX(T2_D_000001,T2_L_000000,T2_L_000000),e ; @BT CB 4B
        bit T2_L_000001,h               ; @BT CB 4C
        bit T2_D_000001,(hl)            ; @BT CB 4E
        bit F2_ADD(T2_D_000001,T2_L_000000),(ix+F2_ID(T2_L_00002F)) ; @BT DD CB 2F 4E
        bit F2_MIX(T2_L_000001,T2_D_000001,T2_D_000001),(iy+F2_SUB(T2_L_00003C,T2_D_000000)) ; @BT FD CB 3C 4E
        bit F2_MIX(T2_D_000001,T2_L_000000,T2_L_000000),l ; @BT CB 4D
        bit T2_L_000002,a               ; @BT CB 57
        bit T2_D_000002,b               ; @BT CB 50
        bit F2_ID(T2_L_000002),c        ; @BT CB 51
        bit F2_ADD(T2_D_000002,T2_L_000000),d ; @BT CB 52
        bit F2_SUB(T2_L_000002,T2_D_000000),e ; @BT CB 53
        bit F2_MIX(T2_L_000002,T2_D_000002,T2_D_000002),h ; @BT CB 54
        bit F2_MIX(T2_D_000002,T2_L_000000,T2_L_000000),(hl) ; @BT CB 56
        bit T2_D_000002,(ix+T2_L_000041) ; @BT DD CB 41 56
        bit F2_ADD(T2_D_000002,T2_L_000000),(iy+F2_ID(T2_L_00004E)) ; @BT FD CB 4E 56
        bit F2_SUB(T2_L_000002,T2_D_000000),l ; @BT CB 55
        bit F2_MIX(T2_L_000003,T2_D_000003,T2_D_000003),a ; @BT CB 5F
        bit F2_MIX(T2_D_000003,T2_L_000000,T2_L_000000),b ; @BT CB 58
        bit T2_L_000003,c               ; @BT CB 59
        bit T2_D_000003,d               ; @BT CB 5A
        bit F2_ID(T2_L_000003),e        ; @BT CB 5B
        bit F2_ADD(T2_D_000003,T2_L_000000),h ; @BT CB 5C
        bit F2_SUB(T2_L_000003,T2_D_000000),(hl) ; @BT CB 5E
        bit F2_MIX(T2_D_000003,T2_L_000000,T2_L_000000),(ix+F2_MIX(T2_L_000053,T2_D_000053,T2_D_000053)) ; @BT DD CB 53 5E
        bit T2_D_000003,(iy+T2_L_000060) ; @BT FD CB 60 5E
        bit F2_ID(T2_L_000003),l        ; @BT CB 5D
        bit F2_ADD(T2_D_000004,T2_L_000000),a ; @BT CB 67
        bit F2_SUB(T2_L_000004,T2_D_000000),b ; @BT CB 60
        bit F2_MIX(T2_L_000004,T2_D_000004,T2_D_000004),c ; @BT CB 61
        bit F2_MIX(T2_D_000004,T2_L_000000,T2_L_000000),d ; @BT CB 62
        bit T2_L_000004,e               ; @BT CB 63
        bit T2_D_000004,h               ; @BT CB 64
        bit F2_ID(T2_L_000004),(hl)     ; @BT CB 66
        bit F2_SUB(T2_L_000004,T2_D_000000),(ix+F2_ADD(T2_D_000065,T2_L_000000)) ; @BT DD CB 65 66
        bit F2_MIX(T2_D_000004,T2_L_000000,T2_L_000000),(iy+F2_MIX(T2_L_000072,T2_D_000072,T2_D_000072)) ; @BT FD CB 72 66
        bit T2_L_000004,l               ; @BT CB 65
        bit T2_D_000005,a               ; @BT CB 6F
        bit F2_ID(T2_L_000005),b        ; @BT CB 68
        bit F2_ADD(T2_D_000005,T2_L_000000),c ; @BT CB 69
        bit F2_SUB(T2_L_000005,T2_D_000000),d ; @BT CB 6A
        bit F2_MIX(T2_L_000005,T2_D_000005,T2_D_000005),e ; @BT CB 6B
        bit F2_MIX(T2_D_000005,T2_L_000000,T2_L_000000),h ; @BT CB 6C
        bit T2_L_000005,(hl)            ; @BT CB 6E
        bit F2_ID(T2_L_000005),(ix+T2_D_000077) ; @BT DD CB 77 6E
        bit F2_SUB(T2_L_000005,T2_D_000000),(iy+F2_ADD(T2_D_000014,T2_L_000000)) ; @BT FD CB 14 6E
        bit F2_MIX(T2_L_000005,T2_D_000005,T2_D_000005),l ; @BT CB 6D
        bit F2_MIX(T2_D_000006,T2_L_000000,T2_L_000000),a ; @BT CB 77
        bit T2_L_000006,b               ; @BT CB 70
        bit T2_D_000006,c               ; @BT CB 71
        bit F2_ID(T2_L_000006),d        ; @BT CB 72
        bit F2_ADD(T2_D_000006,T2_L_000000),e ; @BT CB 73
        bit F2_SUB(T2_L_000006,T2_D_000000),h ; @BT CB 74
        bit F2_MIX(T2_L_000006,T2_D_000006,T2_D_000006),(hl) ; @BT CB 76
        bit T2_L_000006,(ix+F2_MIX(T2_D_000019,T2_L_000000,T2_L_000000)) ; @BT DD CB 19 76
        bit F2_ID(T2_L_000006),(iy+T2_D_000026) ; @BT FD CB 26 76
        bit F2_ADD(T2_D_000006,T2_L_000000),l ; @BT CB 75
        bit F2_SUB(T2_L_000007,T2_D_000000),a ; @BT CB 7F
        bit F2_MIX(T2_L_000007,T2_D_000007,T2_D_000007),b ; @BT CB 78
        bit F2_MIX(T2_D_000007,T2_L_000000,T2_L_000000),c ; @BT CB 79
        bit T2_L_000007,d               ; @BT CB 7A
        bit T2_D_000007,e               ; @BT CB 7B
        bit F2_ID(T2_L_000007),h        ; @BT CB 7C
        bit F2_ADD(T2_D_000007,T2_L_000000),(hl) ; @BT CB 7E
        bit F2_MIX(T2_L_000007,T2_D_000007,T2_D_000007),(ix+F2_SUB(T2_L_00002B,T2_D_000000)) ; @BT DD CB 2B 7E
        bit T2_L_000007,(iy+F2_MIX(T2_D_000038,T2_L_000000,T2_L_000000)) ; @BT FD CB 38 7E
        bit T2_D_000007,l               ; @BT CB 7D

    ; CALL
        call lss,F2_ID(T2_L_002AE2)     ; @BT DC E2 2A
        call lst,F2_ADD(T2_D_002B07,T2_L_000000) ; @BT DC 07 2B
        call neg,F2_SUB(T2_L_002B2C,T2_D_000000) ; @BT FC 2C 2B
        call gte,F2_MIX(T2_L_002B51,T2_D_002B51,T2_D_002B51) ; @BT D4 51 2B
        call geq,F2_MIX(T2_D_002B76,T2_L_000000,T2_L_000000) ; @BT D4 76 2B
        call neq,T2_L_002B9B            ; @BT C4 9B 2B
        call pos,T2_D_002BC0            ; @BT F4 C0 2B
        call equ,F2_ID(T2_L_002BE5)     ; @BT CC E5 2B
        call c,F2_ADD(T2_D_002C0A,T2_L_000000) ; @BT DC 0A 2C
        call m,F2_SUB(T2_L_002C2F,T2_D_000000) ; @BT FC 2F 2C
        call nc,F2_MIX(T2_L_002C54,T2_D_002C54,T2_D_002C54) ; @BT D4 54 2C
        call nz,F2_MIX(T2_D_002C79,T2_L_000000,T2_L_000000) ; @BT C4 79 2C
        call pe,T2_L_002C9E             ; @BT EC 9E 2C
        call po,T2_D_002CC3             ; @BT E4 C3 2C
        call p,F2_ID(T2_L_002CE8)       ; @BT F4 E8 2C
        call z,F2_ADD(T2_D_002D0D,T2_L_000000) ; @BT CC 0D 2D
        call F2_SUB(T2_L_002D32,T2_D_000000) ; @BT CD 32 2D

    ; CP
        cp a,(ix+F2_MIX(T2_L_00007B,T2_D_00007B,T2_D_00007B)) ; @BT DD BE 7B
        cp a,(iy+F2_MIX(T2_D_000018,T2_L_000000,T2_L_000000)) ; @BT FD BE 18
        cp a,T2_L_00001A                ; @BT FE 1A
        cp (ix+T2_D_00005E)             ; @BT DD BE 5E
        cp (iy+F2_ID(T2_L_00006B))      ; @BT FD BE 6B
        cp F2_ADD(T2_D_000039,T2_L_000000) ; @BT FE 39

    ; DEC
        dec (ix+F2_SUB(T2_L_000039,T2_D_000000)) ; @BT DD 35 39
        dec (iy+F2_MIX(T2_L_00006D,T2_D_00006D,T2_D_00006D)) ; @BT FD 35 6D

    ; DJNZ
        djnz F2_SUB(T2_D_000055,T2_D_000000) ; @BT 10 55

    ; IM
        im T2_L_000000                  ; @BT ED 46
        im T2_D_000001                  ; @BT ED 56
        im F2_ID(T2_L_000002)           ; @BT ED 5E

    ; IN
        in a,(F2_ADD(T2_D_000037,T2_L_000000)) ; @BT DB 37
        in a,F2_SUB(T2_L_000048,T2_D_000000) ; @BT DB 48

    ; IN0
        in0 a,(F2_MIX(T2_L_000071,T2_D_000071,T2_D_000071)) ; @BT ED 38 71
        in0 b,(F2_MIX(T2_D_000012,T2_L_000000,T2_L_000000)) ; @BT ED 00 12
        in0 c,(T2_L_000023)             ; @BT ED 08 23
        in0 d,(T2_D_000034)             ; @BT ED 10 34
        in0 e,(F2_ID(T2_L_000045))      ; @BT ED 18 45
        in0 h,(F2_ADD(T2_D_000056,T2_L_000000)) ; @BT ED 20 56
        in0 l,(F2_SUB(T2_L_000067,T2_D_000000)) ; @BT ED 28 67

    ; INC
        inc (ix+F2_MIX(T2_L_000071,T2_D_000071,T2_D_000071)) ; @BT DD 34 71
        inc (iy+F2_MIX(T2_D_000035,T2_L_000000,T2_L_000000)) ; @BT FD 34 35

    ; JP
        jp lss,T2_L_004090              ; @BT DA 90 40
        jp lst,T2_D_0040B5              ; @BT DA B5 40
        jp neg,F2_ID(T2_L_0040DA)       ; @BT FA DA 40
        jp gte,F2_ADD(T2_D_0040FF,T2_L_000000) ; @BT D2 FF 40
        jp geq,F2_SUB(T2_L_004124,T2_D_000000) ; @BT D2 24 41
        jp neq,F2_MIX(T2_L_004149,T2_D_004149,T2_D_004149) ; @BT C2 49 41
        jp pos,F2_MIX(T2_D_00416E,T2_L_000000,T2_L_000000) ; @BT F2 6E 41
        jp equ,T2_L_004193              ; @BT CA 93 41
        jp c,T2_D_0041B8                ; @BT DA B8 41
        jp m,F2_ID(T2_L_0041DD)         ; @BT FA DD 41
        jp nc,F2_ADD(T2_D_004202,T2_L_000000) ; @BT D2 02 42
        jp nz,F2_SUB(T2_L_004227,T2_D_000000) ; @BT C2 27 42
        jp pe,F2_MIX(T2_L_00424C,T2_D_00424C,T2_D_00424C) ; @BT EA 4C 42
        jp po,F2_MIX(T2_D_004271,T2_L_000000,T2_L_000000) ; @BT E2 71 42
        jp p,T2_L_004296                ; @BT F2 96 42
        jp z,T2_D_0042BB                ; @BT CA BB 42
        jp F2_ID(T2_L_0042E0)           ; @BT C3 E0 42

    ; JR
        jr lss,F2_SUB(T2_D_000051,T2_D_000000) ; @BT 38 51
        jr lst,T2_D_000062              ; @BT 38 62
        jr gte,F2_ID(T2_D_000073)       ; @BT 30 73
        jr geq,F2_ADD(T2_D_000014,T2_D_000000) ; @BT 30 14
        jr neq,F2_SUB(T2_D_000025,T2_D_000000) ; @BT 20 25
        jr equ,T2_D_000036              ; @BT 28 36
        jr c,F2_ID(T2_D_000047)         ; @BT 38 47
        jr nc,F2_ADD(T2_D_000058,T2_D_000000) ; @BT 30 58
        jr nz,F2_SUB(T2_D_000069,T2_D_000000) ; @BT 20 69
        jr z,T2_D_00007A                ; @BT 28 7A
        jr F2_ID(T2_D_00001B)           ; @BT 18 1B

    ; LEA
        lea bc,ix+T2_L_00002C           ; @BT ED 02 2C
        lea bc,iy+T2_D_000039           ; @BT ED 03 39
        lea de,ix+F2_ID(T2_L_000046)    ; @BT ED 12 46
        lea de,iy+F2_ADD(T2_D_000053,T2_L_000000) ; @BT ED 13 53
        lea hl,ix+F2_SUB(T2_L_000060,T2_D_000000) ; @BT ED 22 60
        lea hl,iy+F2_MIX(T2_L_00006D,T2_D_00006D,T2_D_00006D) ; @BT ED 23 6D
        lea ix,ix+F2_MIX(T2_D_00007A,T2_L_000000,T2_L_000000) ; @BT ED 32 7A
        lea iy,iy+T2_L_000017           ; @BT ED 33 17

    ; LD
        ld a,(ix+T2_D_000036)           ; @BT DD 7E 36
        ld a,(iy+F2_ID(T2_L_000043))    ; @BT FD 7E 43
        ld a,(F2_ADD(T2_D_004883,T2_L_000000)) ; @BT 3A 83 48
        ld a,F2_SUB(T2_L_000048,T2_D_000000) ; @BT 3E 48
        ld bc,(ix+F2_MIX(T2_L_000019,T2_D_000019,T2_D_000019)) ; @BT DD 07 19
        ld bc,(iy+F2_MIX(T2_D_000026,T2_L_000000,T2_L_000000)) ; @BT FD 07 26
        ld bc,(T2_L_0049AB)             ; @BT ED 4B AB 49
        ld bc,T2_D_0049D0               ; @BT 01 D0 49
        ld b,(ix+F2_ID(T2_L_00001E))    ; @BT DD 46 1E
        ld b,(iy+F2_ADD(T2_D_00002B,T2_L_000000)) ; @BT FD 46 2B
        ld b,F2_SUB(T2_L_00005D,T2_D_000000) ; @BT 06 5D
        ld c,(ix+F2_MIX(T2_L_000071,T2_D_000071,T2_D_000071)) ; @BT DD 4E 71
        ld c,(iy+F2_MIX(T2_D_00007E,T2_L_000000,T2_L_000000)) ; @BT FD 4E 7E
        ld c,T2_L_00007C                ; @BT 0E 7C
        ld de,(ix+T2_D_000047)          ; @BT DD 17 47
        ld de,(iy+F2_ID(T2_L_000054))   ; @BT FD 17 54
        ld de,(F2_ADD(T2_D_004F29,T2_L_000000)) ; @BT ED 5B 29 4F
        ld de,F2_SUB(T2_L_004F4E,T2_D_000000) ; @BT 11 4E 4F
        ld d,(ix+F2_MIX(T2_L_000032,T2_D_000032,T2_D_000032)) ; @BT DD 56 32
        ld d,(iy+F2_MIX(T2_D_00003F,T2_L_000000,T2_L_000000)) ; @BT FD 56 3F
        ld d,T2_L_000021                ; @BT 16 21
        ld e,(ix+T2_D_000015)           ; @BT DD 5E 15
        ld e,(iy+F2_ID(T2_L_000022))    ; @BT FD 5E 22
        ld e,F2_ADD(T2_D_000040,T2_L_000000) ; @BT 1E 40
        ld h,(ix+F2_SUB(T2_L_000068,T2_D_000000)) ; @BT DD 66 68
        ld h,(iy+F2_MIX(T2_L_000075,T2_D_000075,T2_D_000075)) ; @BT FD 66 75
        ld h,F2_MIX(T2_D_00001B,T2_L_000000,T2_L_000000) ; @BT 26 1B
        ld hl,(ix+T2_L_000039)          ; @BT DD 27 39
        ld hl,(iy+T2_D_000046)          ; @BT FD 27 46
        ld hl,(F2_ID(T2_L_00553B))      ; @BT 2A 3B 55
        ld hl,F2_ADD(T2_D_005585,T2_L_000000) ; @BT 21 85 55
        ld (hl),F2_SUB(T2_L_00007E,T2_D_000000) ; @BT 36 7E
        ld l,(ix+F2_MIX(T2_L_00002E,T2_D_00002E,T2_D_00002E)) ; @BT DD 6E 2E
        ld l,(iy+F2_MIX(T2_D_00003B,T2_L_000000,T2_L_000000)) ; @BT FD 6E 3B
        ld l,T2_L_000059                ; @BT 2E 59
        ld sp,(T2_D_0059B6)             ; @BT ED 7B B6 59
        ld sp,F2_ID(T2_L_0059DB)        ; @BT 31 DB 59
        ld ix,(ix+F2_ADD(T2_D_000067,T2_L_000000)) ; @BT DD 37 67
        ld ix,(iy+F2_SUB(T2_L_000074,T2_D_000000)) ; @BT FD 31 74
        ld ix,(F2_MIX(T2_L_005AB9,T2_D_005AB9,T2_D_005AB9)) ; @BT DD 2A B9 5A
        ld ix,F2_MIX(T2_D_005ADE,T2_L_000000,T2_L_000000) ; @BT DD 21 DE 5A
        ld ixh,T2_L_00006E              ; @BT DD 26 6E
        ld ixl,T2_D_000016              ; @BT DD 2E 16
        ld (ix+F2_ID(T2_L_00001B)),a    ; @BT DD 77 1B
        ld (ix+F2_ADD(T2_D_000028,T2_L_000000)),b ; @BT DD 70 28
        ld (ix+F2_SUB(T2_L_000035,T2_D_000000)),bc ; @BT DD 0F 35
        ld (ix+F2_MIX(T2_L_000042,T2_D_000042,T2_D_000042)),c ; @BT DD 71 42
        ld (ix+F2_MIX(T2_D_00004F,T2_L_000000,T2_L_000000)),d ; @BT DD 72 4F
        ld (ix+T2_L_00005C),de          ; @BT DD 1F 5C
        ld (ix+T2_D_000069),e           ; @BT DD 73 69
        ld (ix+F2_ID(T2_L_000076)),h    ; @BT DD 74 76
        ld (ix+F2_ADD(T2_D_000013,T2_L_000000)),hl ; @BT DD 2F 13
        ld (ix+F2_SUB(T2_L_000020,T2_D_000000)),ix ; @BT DD 3F 20
        ld (ix+F2_MIX(T2_L_00002D,T2_D_00002D,T2_D_00002D)),iy ; @BT DD 3E 2D
        ld (ix+F2_MIX(T2_D_00003A,T2_L_000000,T2_L_000000)),l ; @BT DD 75 3A
        ld (ix+T2_L_000047),T2_D_000013 ; @BT DD 36 47 13
        ld iy,(ix+F2_ID(T2_L_000061))   ; @BT DD 31 61
        ld iy,(iy+F2_ADD(T2_D_00006E,T2_L_000000)) ; @BT FD 37 6E
        ld iy,(F2_SUB(T2_L_005FA3,T2_D_000000)) ; @BT FD 2A A3 5F
        ld iy,F2_MIX(T2_L_005FC8,T2_D_005FC8,T2_D_005FC8) ; @BT FD 21 C8 5F
        ld iyh,F2_MIX(T2_D_000010,T2_L_000000,T2_L_000000) ; @BT FD 26 10
        ld iyl,T2_L_000028              ; @BT FD 2E 28
        ld (iy+T2_D_000015),a           ; @BT FD 77 15
        ld (iy+F2_ID(T2_L_000022)),b    ; @BT FD 70 22
        ld (iy+F2_ADD(T2_D_00002F,T2_L_000000)),bc ; @BT FD 0F 2F
        ld (iy+F2_SUB(T2_L_00003C,T2_D_000000)),c ; @BT FD 71 3C
        ld (iy+F2_MIX(T2_L_000049,T2_D_000049,T2_D_000049)),d ; @BT FD 72 49
        ld (iy+F2_MIX(T2_D_000056,T2_L_000000,T2_L_000000)),de ; @BT FD 1F 56
        ld (iy+T2_L_000063),e           ; @BT FD 73 63
        ld (iy+T2_D_000070),h           ; @BT FD 74 70
        ld (iy+F2_ID(T2_L_00007D)),hl   ; @BT FD 2F 7D
        ld (iy+F2_ADD(T2_D_00001A,T2_L_000000)),ix ; @BT FD 3E 1A
        ld (iy+F2_SUB(T2_L_000027,T2_D_000000)),iy ; @BT FD 3F 27
        ld (iy+F2_MIX(T2_L_000034,T2_D_000034,T2_D_000034)),l ; @BT FD 75 34
        ld (iy+F2_MIX(T2_D_000041,T2_L_000000,T2_L_000000)),T2_L_000025 ; @BT FD 36 41 25
        ld (T2_D_00641E),a              ; @BT 32 1E 64
        ld (F2_ID(T2_L_006443)),bc      ; @BT ED 43 43 64
        ld (F2_ADD(T2_D_006468,T2_L_000000)),de ; @BT ED 53 68 64
        ld (F2_SUB(T2_L_00648D,T2_D_000000)),hl ; @BT 22 8D 64
        ld (F2_MIX(T2_L_0064B2,T2_D_0064B2,T2_D_0064B2)),sp ; @BT ED 73 B2 64
        ld (F2_MIX(T2_D_0064D7,T2_L_000000,T2_L_000000)),iy ; @BT FD 22 D7 64
        ld (T2_L_0064FC),ix             ; @BT DD 22 FC 64

    ; OR
        or a,(ix+T2_D_00002E)           ; @BT DD B6 2E
        or a,(iy+F2_ID(T2_L_00003B))    ; @BT FD B6 3B
        or a,F2_ADD(T2_D_000059,T2_L_000000) ; @BT F6 59
        or (ix+F2_SUB(T2_L_000011,T2_D_000000)) ; @BT DD B6 11
        or (iy+F2_MIX(T2_L_00001E,T2_D_00001E,T2_D_00001E)) ; @BT FD B6 1E
        or F2_MIX(T2_D_000078,T2_L_000000,T2_L_000000) ; @BT F6 78

    ; OUT
        out (T2_L_00006B),a             ; @BT D3 6B

    ; OUT0
        out0 (T2_D_000050),a            ; @BT ED 39 50
        out0 (F2_ID(T2_L_000061)),b     ; @BT ED 01 61
        out0 (F2_ADD(T2_D_000072,T2_L_000000)),c ; @BT ED 09 72
        out0 (F2_SUB(T2_L_000013,T2_D_000000)),d ; @BT ED 11 13
        out0 (F2_MIX(T2_L_000024,T2_D_000024,T2_D_000024)),e ; @BT ED 19 24
        out0 (F2_MIX(T2_D_000035,T2_L_000000,T2_L_000000)),h ; @BT ED 21 35
        out0 (T2_L_000046),l            ; @BT ED 29 46

    ; PEA
        pea ix+T2_D_00007B              ; @BT ED 65 7B
        pea iy+F2_ID(T2_L_000018)       ; @BT ED 66 18

    ; RES
        res F2_ADD(T2_D_000000,T2_L_000000),a ; @BT CB 87
        res F2_SUB(T2_L_000000,T2_D_000000),b ; @BT CB 80
        res F2_MIX(T2_L_000000,T2_D_000000,T2_D_000000),c ; @BT CB 81
        res F2_MIX(T2_D_000000,T2_L_000000,T2_L_000000),d ; @BT CB 82
        res T2_L_000000,e               ; @BT CB 83
        res T2_D_000000,h               ; @BT CB 84
        res F2_ID(T2_L_000000),(hl)     ; @BT CB 86
        res F2_SUB(T2_L_000000,T2_D_000000),(ix+F2_ADD(T2_D_00003C,T2_L_000000)) ; @BT DD CB 3C 86
        res F2_MIX(T2_D_000000,T2_L_000000,T2_L_000000),(iy+F2_MIX(T2_L_000049,T2_D_000049,T2_D_000049)) ; @BT FD CB 49 86
        res T2_L_000000,l               ; @BT CB 85
        res T2_D_000001,a               ; @BT CB 8F
        res F2_ID(T2_L_000001),b        ; @BT CB 88
        res F2_ADD(T2_D_000001,T2_L_000000),c ; @BT CB 89
        res F2_SUB(T2_L_000001,T2_D_000000),d ; @BT CB 8A
        res F2_MIX(T2_L_000001,T2_D_000001,T2_D_000001),e ; @BT CB 8B
        res F2_MIX(T2_D_000001,T2_L_000000,T2_L_000000),h ; @BT CB 8C
        res T2_L_000001,(hl)            ; @BT CB 8E
        res F2_ID(T2_L_000001),(ix+T2_D_00004E) ; @BT DD CB 4E 8E
        res F2_SUB(T2_L_000001,T2_D_000000),(iy+F2_ADD(T2_D_00005B,T2_L_000000)) ; @BT FD CB 5B 8E
        res F2_MIX(T2_L_000001,T2_D_000001,T2_D_000001),l ; @BT CB 8D
        res F2_MIX(T2_D_000002,T2_L_000000,T2_L_000000),a ; @BT CB 97
        res T2_L_000002,b               ; @BT CB 90
        res T2_D_000002,c               ; @BT CB 91
        res F2_ID(T2_L_000002),d        ; @BT CB 92
        res F2_ADD(T2_D_000002,T2_L_000000),e ; @BT CB 93
        res F2_SUB(T2_L_000002,T2_D_000000),h ; @BT CB 94
        res F2_MIX(T2_L_000002,T2_D_000002,T2_D_000002),(hl) ; @BT CB 96
        res T2_L_000002,(ix+F2_MIX(T2_D_000060,T2_L_000000,T2_L_000000)) ; @BT DD CB 60 96
        res F2_ID(T2_L_000002),(iy+T2_D_00006D) ; @BT FD CB 6D 96
        res F2_ADD(T2_D_000002,T2_L_000000),l ; @BT CB 95
        res F2_SUB(T2_L_000003,T2_D_000000),a ; @BT CB 9F
        res F2_MIX(T2_L_000003,T2_D_000003,T2_D_000003),b ; @BT CB 98
        res F2_MIX(T2_D_000003,T2_L_000000,T2_L_000000),c ; @BT CB 99
        res T2_L_000003,d               ; @BT CB 9A
        res T2_D_000003,e               ; @BT CB 9B
        res F2_ID(T2_L_000003),h        ; @BT CB 9C
        res F2_ADD(T2_D_000003,T2_L_000000),(hl) ; @BT CB 9E
        res F2_MIX(T2_L_000003,T2_D_000003,T2_D_000003),(ix+F2_SUB(T2_L_000072,T2_D_000000)) ; @BT DD CB 72 9E
        res T2_L_000003,(iy+F2_MIX(T2_D_00007F,T2_L_000000,T2_L_000000)) ; @BT FD CB 7F 9E
        res T2_D_000003,l               ; @BT CB 9D
        res F2_ID(T2_L_000004),a        ; @BT CB A7
        res F2_ADD(T2_D_000004,T2_L_000000),b ; @BT CB A0
        res F2_SUB(T2_L_000004,T2_D_000000),c ; @BT CB A1
        res F2_MIX(T2_L_000004,T2_D_000004,T2_D_000004),d ; @BT CB A2
        res F2_MIX(T2_D_000004,T2_L_000000,T2_L_000000),e ; @BT CB A3
        res T2_L_000004,h               ; @BT CB A4
        res T2_D_000004,(hl)            ; @BT CB A6
        res F2_ADD(T2_D_000004,T2_L_000000),(ix+F2_ID(T2_L_000014)) ; @BT DD CB 14 A6
        res F2_MIX(T2_L_000004,T2_D_000004,T2_D_000004),(iy+F2_SUB(T2_L_000021,T2_D_000000)) ; @BT FD CB 21 A6
        res F2_MIX(T2_D_000004,T2_L_000000,T2_L_000000),l ; @BT CB A5
        res T2_L_000005,a               ; @BT CB AF
        res T2_D_000005,b               ; @BT CB A8
        res F2_ID(T2_L_000005),c        ; @BT CB A9
        res F2_ADD(T2_D_000005,T2_L_000000),d ; @BT CB AA
        res F2_SUB(T2_L_000005,T2_D_000000),e ; @BT CB AB
        res F2_MIX(T2_L_000005,T2_D_000005,T2_D_000005),h ; @BT CB AC
        res F2_MIX(T2_D_000005,T2_L_000000,T2_L_000000),(hl) ; @BT CB AE
        res T2_D_000005,(ix+T2_L_000026) ; @BT DD CB 26 AE
        res F2_ADD(T2_D_000005,T2_L_000000),(iy+F2_ID(T2_L_000033)) ; @BT FD CB 33 AE
        res F2_SUB(T2_L_000005,T2_D_000000),l ; @BT CB AD
        res F2_MIX(T2_L_000006,T2_D_000006,T2_D_000006),a ; @BT CB B7
        res F2_MIX(T2_D_000006,T2_L_000000,T2_L_000000),b ; @BT CB B0
        res T2_L_000006,c               ; @BT CB B1
        res T2_D_000006,d               ; @BT CB B2
        res F2_ID(T2_L_000006),e        ; @BT CB B3
        res F2_ADD(T2_D_000006,T2_L_000000),h ; @BT CB B4
        res F2_SUB(T2_L_000006,T2_D_000000),(hl) ; @BT CB B6
        res F2_MIX(T2_D_000006,T2_L_000000,T2_L_000000),(ix+F2_MIX(T2_L_000038,T2_D_000038,T2_D_000038)) ; @BT DD CB 38 B6
        res T2_D_000006,(iy+T2_L_000045) ; @BT FD CB 45 B6
        res F2_ID(T2_L_000006),l        ; @BT CB B5
        res F2_ADD(T2_D_000007,T2_L_000000),a ; @BT CB BF
        res F2_SUB(T2_L_000007,T2_D_000000),b ; @BT CB B8
        res F2_MIX(T2_L_000007,T2_D_000007,T2_D_000007),c ; @BT CB B9
        res F2_MIX(T2_D_000007,T2_L_000000,T2_L_000000),d ; @BT CB BA
        res T2_L_000007,e               ; @BT CB BB
        res T2_D_000007,h               ; @BT CB BC
        res F2_ID(T2_L_000007),(hl)     ; @BT CB BE
        res F2_SUB(T2_L_000007,T2_D_000000),(ix+F2_ADD(T2_D_00004A,T2_L_000000)) ; @BT DD CB 4A BE
        res F2_MIX(T2_D_000007,T2_L_000000,T2_L_000000),(iy+F2_MIX(T2_L_000057,T2_D_000057,T2_D_000057)) ; @BT FD CB 57 BE
        res T2_L_000007,l               ; @BT CB BD

    ; RL
        rl (ix+T2_D_000073)             ; @BT DD CB 73 16
        rl (iy+F2_ID(T2_L_000010))      ; @BT FD CB 10 16

    ; RLC
        rlc (ix+F2_ADD(T2_D_000015,T2_L_000000)) ; @BT DD CB 15 06
        rlc (iy+F2_SUB(T2_L_000022,T2_D_000000)) ; @BT FD CB 22 06

    ; RR
        rr (ix+F2_MIX(T2_L_00004E,T2_D_00004E,T2_D_00004E)) ; @BT DD CB 4E 1E
        rr (iy+F2_MIX(T2_D_00005B,T2_L_000000,T2_L_000000)) ; @BT FD CB 5B 1E

    ; RRC
        rrc (ix+T2_L_00006D)            ; @BT DD CB 6D 0E
        rrc (iy+T2_D_00007A)            ; @BT FD CB 7A 0E

    ; RST
        rst F2_ID(T2_L_000000)          ; @BT C7
        rst F2_ADD(T2_D_000008,T2_L_000000) ; @BT CF
        rst F2_SUB(T2_L_000010,T2_D_000000) ; @BT D7
        rst F2_MIX(T2_L_000018,T2_D_000018,T2_D_000018) ; @BT DF
        rst F2_MIX(T2_D_000020,T2_L_000000,T2_L_000000) ; @BT E7
        rst T2_L_000028                 ; @BT EF
        rst T2_D_000030                 ; @BT F7
        rst F2_ID(T2_L_000038)          ; @BT FF

    ; SBC
        sbc a,(ix+F2_ADD(T2_D_00006F,T2_L_000000)) ; @BT DD 9E 6F
        sbc a,(iy+F2_SUB(T2_L_00007C,T2_D_000000)) ; @BT FD 9E 7C
        sbc a,F2_MIX(T2_L_00003E,T2_D_00003E,T2_D_00003E) ; @BT DE 3E
        sbc (ix+F2_MIX(T2_D_000016,T2_L_000000,T2_L_000000)) ; @BT DD 9E 16
        sbc (iy+T2_L_000023)            ; @BT FD 9E 23
        sbc T2_D_000031                 ; @BT DE 31

    ; SET
        set F2_ID(T2_L_000000),a        ; @BT CB C7
        set F2_ADD(T2_D_000000,T2_L_000000),b ; @BT CB C0
        set F2_SUB(T2_L_000000,T2_D_000000),c ; @BT CB C1
        set F2_MIX(T2_L_000000,T2_D_000000,T2_D_000000),d ; @BT CB C2
        set F2_MIX(T2_D_000000,T2_L_000000,T2_L_000000),e ; @BT CB C3
        set T2_L_000000,h               ; @BT CB C4
        set T2_D_000000,(hl)            ; @BT CB C6
        set F2_ADD(T2_D_000000,T2_L_000000),(ix+F2_ID(T2_L_00005C)) ; @BT DD CB 5C C6
        set F2_MIX(T2_L_000000,T2_D_000000,T2_D_000000),(iy+F2_SUB(T2_L_000069,T2_D_000000)) ; @BT FD CB 69 C6
        set F2_MIX(T2_D_000000,T2_L_000000,T2_L_000000),l ; @BT CB C5
        set T2_L_000001,a               ; @BT CB CF
        set T2_D_000001,b               ; @BT CB C8
        set F2_ID(T2_L_000001),c        ; @BT CB C9
        set F2_ADD(T2_D_000001,T2_L_000000),d ; @BT CB CA
        set F2_SUB(T2_L_000001,T2_D_000000),e ; @BT CB CB
        set F2_MIX(T2_L_000001,T2_D_000001,T2_D_000001),h ; @BT CB CC
        set F2_MIX(T2_D_000001,T2_L_000000,T2_L_000000),(hl) ; @BT CB CE
        set T2_D_000001,(ix+T2_L_00006E) ; @BT DD CB 6E CE
        set F2_ADD(T2_D_000001,T2_L_000000),(iy+F2_ID(T2_L_00007B)) ; @BT FD CB 7B CE
        set F2_SUB(T2_L_000001,T2_D_000000),l ; @BT CB CD
        set F2_MIX(T2_L_000002,T2_D_000002,T2_D_000002),a ; @BT CB D7
        set F2_MIX(T2_D_000002,T2_L_000000,T2_L_000000),b ; @BT CB D0
        set T2_L_000002,c               ; @BT CB D1
        set T2_D_000002,d               ; @BT CB D2
        set F2_ID(T2_L_000002),e        ; @BT CB D3
        set F2_ADD(T2_D_000002,T2_L_000000),h ; @BT CB D4
        set F2_SUB(T2_L_000002,T2_D_000000),(hl) ; @BT CB D6
        set F2_MIX(T2_D_000002,T2_L_000000,T2_L_000000),(ix+F2_MIX(T2_L_000010,T2_D_000010,T2_D_000010)) ; @BT DD CB 10 D6
        set T2_D_000002,(iy+T2_L_00001D) ; @BT FD CB 1D D6
        set F2_ID(T2_L_000002),l        ; @BT CB D5
        set F2_ADD(T2_D_000003,T2_L_000000),a ; @BT CB DF
        set F2_SUB(T2_L_000003,T2_D_000000),b ; @BT CB D8
        set F2_MIX(T2_L_000003,T2_D_000003,T2_D_000003),c ; @BT CB D9
        set F2_MIX(T2_D_000003,T2_L_000000,T2_L_000000),d ; @BT CB DA
        set T2_L_000003,e               ; @BT CB DB
        set T2_D_000003,h               ; @BT CB DC
        set F2_ID(T2_L_000003),(hl)     ; @BT CB DE
        set F2_SUB(T2_L_000003,T2_D_000000),(ix+F2_ADD(T2_D_000022,T2_L_000000)) ; @BT DD CB 22 DE
        set F2_MIX(T2_D_000003,T2_L_000000,T2_L_000000),(iy+F2_MIX(T2_L_00002F,T2_D_00002F,T2_D_00002F)) ; @BT FD CB 2F DE
        set T2_L_000003,l               ; @BT CB DD
        set T2_D_000004,a               ; @BT CB E7
        set F2_ID(T2_L_000004),b        ; @BT CB E0
        set F2_ADD(T2_D_000004,T2_L_000000),c ; @BT CB E1
        set F2_SUB(T2_L_000004,T2_D_000000),d ; @BT CB E2
        set F2_MIX(T2_L_000004,T2_D_000004,T2_D_000004),e ; @BT CB E3
        set F2_MIX(T2_D_000004,T2_L_000000,T2_L_000000),h ; @BT CB E4
        set T2_L_000004,(hl)            ; @BT CB E6
        set F2_ID(T2_L_000004),(ix+T2_D_000034) ; @BT DD CB 34 E6
        set F2_SUB(T2_L_000004,T2_D_000000),(iy+F2_ADD(T2_D_000041,T2_L_000000)) ; @BT FD CB 41 E6
        set F2_MIX(T2_L_000004,T2_D_000004,T2_D_000004),l ; @BT CB E5
        set F2_MIX(T2_D_000005,T2_L_000000,T2_L_000000),a ; @BT CB EF
        set T2_L_000005,b               ; @BT CB E8
        set T2_D_000005,c               ; @BT CB E9
        set F2_ID(T2_L_000005),d        ; @BT CB EA
        set F2_ADD(T2_D_000005,T2_L_000000),e ; @BT CB EB
        set F2_SUB(T2_L_000005,T2_D_000000),h ; @BT CB EC
        set F2_MIX(T2_L_000005,T2_D_000005,T2_D_000005),(hl) ; @BT CB EE
        set T2_L_000005,(ix+F2_MIX(T2_D_000046,T2_L_000000,T2_L_000000)) ; @BT DD CB 46 EE
        set F2_ID(T2_L_000005),(iy+T2_D_000053) ; @BT FD CB 53 EE
        set F2_ADD(T2_D_000005,T2_L_000000),l ; @BT CB ED
        set F2_SUB(T2_L_000006,T2_D_000000),a ; @BT CB F7
        set F2_MIX(T2_L_000006,T2_D_000006,T2_D_000006),b ; @BT CB F0
        set F2_MIX(T2_D_000006,T2_L_000000,T2_L_000000),c ; @BT CB F1
        set T2_L_000006,d               ; @BT CB F2
        set T2_D_000006,e               ; @BT CB F3
        set F2_ID(T2_L_000006),h        ; @BT CB F4
        set F2_ADD(T2_D_000006,T2_L_000000),(hl) ; @BT CB F6
        set F2_MIX(T2_L_000006,T2_D_000006,T2_D_000006),(ix+F2_SUB(T2_L_000058,T2_D_000000)) ; @BT DD CB 58 F6
        set T2_L_000006,(iy+F2_MIX(T2_D_000065,T2_L_000000,T2_L_000000)) ; @BT FD CB 65 F6
        set T2_D_000006,l               ; @BT CB F5
        set F2_ID(T2_L_000007),a        ; @BT CB FF
        set F2_ADD(T2_D_000007,T2_L_000000),b ; @BT CB F8
        set F2_SUB(T2_L_000007,T2_D_000000),c ; @BT CB F9
        set F2_MIX(T2_L_000007,T2_D_000007,T2_D_000007),d ; @BT CB FA
        set F2_MIX(T2_D_000007,T2_L_000000,T2_L_000000),e ; @BT CB FB
        set T2_L_000007,h               ; @BT CB FC
        set T2_D_000007,(hl)            ; @BT CB FE
        set F2_ADD(T2_D_000007,T2_L_000000),(ix+F2_ID(T2_L_00006A)) ; @BT DD CB 6A FE
        set F2_MIX(T2_L_000007,T2_D_000007,T2_D_000007),(iy+F2_SUB(T2_L_000077,T2_D_000000)) ; @BT FD CB 77 FE
        set F2_MIX(T2_D_000007,T2_L_000000,T2_L_000000),l ; @BT CB FD

    ; SLA
        sla (ix+T2_L_00007C)            ; @BT DD CB 7C 26
        sla (iy+T2_D_000019)            ; @BT FD CB 19 26

    ; SRA
        sra (ix+F2_ID(T2_L_00001E))     ; @BT DD CB 1E 2E
        sra (iy+F2_ADD(T2_D_00002B,T2_L_000000)) ; @BT FD CB 2B 2E

    ; SRL
        srl (ix+F2_SUB(T2_L_000030,T2_D_000000)) ; @BT DD CB 30 3E
        srl (iy+F2_MIX(T2_L_00003D,T2_D_00003D,T2_D_00003D)) ; @BT FD CB 3D 3E

    ; SUB
        sub a,(ix+F2_MIX(T2_D_000076,T2_L_000000,T2_L_000000)) ; @BT DD 96 76
        sub a,(iy+T2_L_000013)          ; @BT FD 96 13
        sub a,T2_D_000061               ; @BT D6 61
        sub (ix+F2_ID(T2_L_000059))     ; @BT DD 96 59
        sub (iy+F2_ADD(T2_D_000066,T2_L_000000)) ; @BT FD 96 66
        sub F2_SUB(T2_L_000010,T2_D_000000) ; @BT D6 10

    ; TST
        tst a,F2_MIX(T2_L_000039,T2_D_000039,T2_D_000039) ; @BT ED 64 39

    ; TSTIO
        tstio F2_MIX(T2_D_00004A,T2_L_000000,T2_L_000000) ; @BT ED 74 4A

    ; XOR
        xor a,(ix+T2_L_00004E)          ; @BT DD AE 4E
        xor a,(iy+T2_D_00005B)          ; @BT FD AE 5B
        xor a,F2_ID(T2_L_000069)        ; @BT EE 69
        xor (ix+F2_ADD(T2_D_000031,T2_L_000000)) ; @BT DD AE 31
        xor (iy+F2_SUB(T2_L_00003E,T2_D_000000)) ; @BT FD AE 3E
        xor F2_MIX(T2_L_000018,T2_D_000018,T2_D_000018) ; @BT EE 18

    .db "<02"               ; @BT END
