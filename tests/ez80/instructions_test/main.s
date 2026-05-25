
.MEMORYMAP
DEFAULTSLOT 0
SLOT 0 $0000 $2000
.ENDME

.ROMBANKMAP
BANKSTOTAL 1
BANKSIZE $2000
BANKS 1
.ENDRO

.EMPTYFILL $ff

.BANK 0 SLOT 0
.ORGA 0

; @BT linked.rom

        .db "01>"               ; @BT TEST-01 01 START
        in0 b,($12)             ; @BT ED 00 12
        in0 a,($34)             ; @BT ED 38 34
        out0 ($56),l            ; @BT ED 29 56
        out0 ($78),a            ; @BT ED 39 78
        .db "<01"               ; @BT END

        .db "02>"               ; @BT TEST-02 02 START
        tst a,b                 ; @BT ED 04
        tst a,(hl)              ; @BT ED 34
        tst a,$9a               ; @BT ED 64 9A
        tstio $bc               ; @BT ED 74 BC
        .db "<02"               ; @BT END

        .db "03>"               ; @BT TEST-03 03 START
        lea bc,ix+1             ; @BT ED 02 01
        lea hl,iy+2             ; @BT ED 23 02
        lea ix,ix-3             ; @BT ED 32 FD
        lea iy,iy-4             ; @BT ED 33 FC
        pea ix+5                ; @BT ED 65 05
        pea iy-6                ; @BT ED 66 FA
        .db "<03"               ; @BT END

        .db "04>"               ; @BT TEST-04 04 START
        nop.sis                 ; @BT 40 00
        nop.lis                 ; @BT 49 00
        nop.sil                 ; @BT 52 00
        nop.lil                 ; @BT 5B 00
        jp.s hl                 ; @BT 40 E9
        jp.l ix                 ; @BT 5B DD E9
        mlt.lil bc              ; @BT 5B ED 4C
        .db "<04"               ; @BT END

        .db "05>"               ; @BT TEST-05 05 START
        .adl on
        jp $123456              ; @BT C3 56 34 12
        ld bc,$123456           ; @BT 01 56 34 12
        ld ($123456),hl         ; @BT 22 56 34 12
        ld bc,($123456)         ; @BT ED 4B 56 34 12
        jp.sis $1234            ; @BT 40 C3 34 12
        jp.lis $1234            ; @BT 49 C3 34 12
        .adl off
        jp.sil $123456          ; @BT 52 C3 56 34 12
        jp.lil $123456          ; @BT 5B C3 56 34 12
        jp $1234                ; @BT C3 34 12
        .db "<05"               ; @BT END

        .db "06>"               ; @BT TEST-06 06 START
        add a,(ix+1)            ; @BT DD 86 01
        ld (iy+2),a             ; @BT FD 77 02
        .db "<06"               ; @BT END

        .db "07>"               ; @BT TEST-07 07 START
        ind                     ; @BT ED AA
        ind2                    ; @BT ED 8C
        ind2r                   ; @BT ED 9C
        indm                    ; @BT ED 8A
        indmr                   ; @BT ED 9A
        indr                    ; @BT ED BA
        indrx                   ; @BT ED CA
        ini                     ; @BT ED A2
        ini2                    ; @BT ED 84
        ini2r                   ; @BT ED 94
        inim                    ; @BT ED 82
        inimr                   ; @BT ED 92
        inir                    ; @BT ED B2
        inirx                   ; @BT ED C2
        outd                    ; @BT ED AB
        outd2                   ; @BT ED AC
        otd2r                   ; @BT ED BC
        otdm                    ; @BT ED 8B
        otdmr                   ; @BT ED 9B
        otdr                    ; @BT ED BB
        otdrx                   ; @BT ED CB
        outi                    ; @BT ED A3
        outi2                   ; @BT ED A4
        oti2r                   ; @BT ED B4
        otim                    ; @BT ED 83
        otimr                   ; @BT ED 93
        otir                    ; @BT ED B3
        otirx                   ; @BT ED C3
        .db "<07"               ; @BT END

        .db "08>"               ; @BT TEST-08 08 START
        ld hl,i                 ; @BT ED D7
        ld i,hl                 ; @BT ED C7
        ld bc,(hl)              ; @BT ED 07
        ld de,(hl)              ; @BT ED 17
        ld hl,(hl)              ; @BT ED 27
        ld (hl),bc              ; @BT ED 0F
        ld (hl),de              ; @BT ED 1F
        ld (hl),hl              ; @BT ED 2F
        ld ix,(hl)              ; @BT ED 37
        ld iy,(hl)              ; @BT ED 31
        ld (hl),ix              ; @BT ED 3F
        ld (hl),iy              ; @BT ED 3E
        ld bc,(ix+1)            ; @BT DD 07 01
        ld de,(iy+2)            ; @BT FD 17 02
        ld hl,(ix-3)            ; @BT DD 27 FD
        ld (iy-4),hl            ; @BT FD 2F FC
        ld ix,(iy+5)            ; @BT FD 37 05
        ld iy,(ix-6)            ; @BT DD 31 FA
        ld (ix+7),iy            ; @BT DD 3E 07
        ld (iy-8),ix            ; @BT FD 3F F8
        .db "<08"               ; @BT END

        .db "09>"               ; @BT TEST-09 09 START
        ld a,ixh                ; @BT DD 7C
        ld b,ixl                ; @BT DD 45
        ld ixh,a                ; @BT DD 67
        ld ixl,$12              ; @BT DD 2E 12
        ld a,iyh                ; @BT FD 7C
        ld e,iyl                ; @BT FD 5D
        ld iyh,b                ; @BT FD 60
        ld iyl,$34              ; @BT FD 2E 34
        inc ixh                 ; @BT DD 24
        dec ixl                 ; @BT DD 2D
        inc iyh                 ; @BT FD 24
        dec iyl                 ; @BT FD 2D
        add a,ixh               ; @BT DD 84
        adc ixl                 ; @BT DD 8D
        sub a,iyh               ; @BT FD 94
        sbc iyl                 ; @BT FD 9D
        and ixh                 ; @BT DD A4
        xor a,ixl               ; @BT DD AD
        or iyh                  ; @BT FD B4
        cp a,iyl                ; @BT FD BD
        .db "<09"               ; @BT END

        .db "10>"               ; @BT TEST-10 10 START
        call $1234              ; @BT CD 34 12
        call c,$2345            ; @BT DC 45 23
        call nc,$3456           ; @BT D4 56 34
        call z,$4567            ; @BT CC 67 45
        call nz,$5678           ; @BT C4 78 56
        call m,$6789            ; @BT FC 89 67
        call p,$789a            ; @BT F4 9A 78
        call pe,$89ab           ; @BT EC AB 89
        call po,$9abc           ; @BT E4 BC 9A
        jp (hl)                 ; @BT E9
        jp (ix)                 ; @BT DD E9
        jp (iy)                 ; @BT FD E9
        jp c,$0abc              ; @BT DA BC 0A
        jp nc,$1bcd             ; @BT D2 CD 1B
        jp z,$2cde              ; @BT CA DE 2C
        jp nz,$3def             ; @BT C2 EF 3D
        jr c,$11                ; @BT 38 11
        jr nc,$22               ; @BT 30 22
        jr z,$33                ; @BT 28 33
        jr nz,$44               ; @BT 20 44
        jr $55                  ; @BT 18 55
        djnz $66                ; @BT 10 66
        ret c                   ; @BT D8
        ret nc                  ; @BT D0
        ret z                   ; @BT C8
        ret nz                  ; @BT C0
        ret m                   ; @BT F8
        ret p                   ; @BT F0
        ret pe                  ; @BT E8
        ret po                  ; @BT E0
        ret                     ; @BT C9
        reti                    ; @BT ED 4D
        retn                    ; @BT ED 45
        rst $00                 ; @BT C7
        rst $18                 ; @BT DF
        rst $38                 ; @BT FF
        ex af',af               ; @BT 08
        ex de,hl                ; @BT EB
        ex (sp),hl              ; @BT E3
        ex (sp),ix              ; @BT DD E3
        ex iy,(sp)              ; @BT FD E3
        im 0                    ; @BT ED 46
        im 1                    ; @BT ED 56
        im 2                    ; @BT ED 5E
        in a,(c)                ; @BT ED 78
        in a,($12)              ; @BT DB 12
        in (c)                  ; @BT ED 70
        out (c),0               ; @BT ED 71
        out (c),a               ; @BT ED 79
        out ($34),a             ; @BT D3 34
        push af                 ; @BT F5
        push ix                 ; @BT DD E5
        push iy                 ; @BT FD E5
        pop af                  ; @BT F1
        pop ix                  ; @BT DD E1
        pop iy                  ; @BT FD E1
        .db "<10"               ; @BT END

        .db "11>"               ; @BT TEST-11 11 START
        bit 0,a                 ; @BT CB 47
        bit 7,l                 ; @BT CB 7D
        bit 4,(hl)              ; @BT CB 66
        bit 2,(ix+$5c)          ; @BT DD CB 5C 56
        bit 6,(iy-$01)          ; @BT FD CB FF 76
        res 0,a                 ; @BT CB 87
        res 3,(hl)              ; @BT CB 9E
        res 5,(ix+$20)          ; @BT DD CB 20 AE
        res 7,(iy-$22)          ; @BT FD CB DE BE
        set 1,b                 ; @BT CB C8
        set 4,(hl)              ; @BT CB E6
        set 6,(ix+$7f)          ; @BT DD CB 7F F6
        set 7,(iy-$80)          ; @BT FD CB 80 FE
        rl a                    ; @BT CB 17
        rl (ix+$01)             ; @BT DD CB 01 16
        rlc b                   ; @BT CB 00
        rlc (iy-$02)            ; @BT FD CB FE 06
        rr c                    ; @BT CB 19
        rr (ix+$03)             ; @BT DD CB 03 1E
        rrc d                   ; @BT CB 0A
        rrc (iy-$04)            ; @BT FD CB FC 0E
        sla e                   ; @BT CB 23
        sla (ix+$05)            ; @BT DD CB 05 26
        sra h                   ; @BT CB 2C
        sra (iy-$06)            ; @BT FD CB FA 2E
        srl l                   ; @BT CB 3D
        srl (ix+$07)            ; @BT DD CB 07 3E
        rla                     ; @BT 17
        rlca                    ; @BT 07
        rra                     ; @BT 1F
        rrca                    ; @BT 0F
        rld                     ; @BT ED 6F
        rrd                     ; @BT ED 67
        .db "<11"               ; @BT END

        .db "12>"               ; @BT TEST-12 12 START
        .adl on
        call $123456            ; @BT CD 56 34 12
        call nz,$234567         ; @BT C4 67 45 23
        jp z,$345678            ; @BT CA 78 56 34
        ld a,($456789)          ; @BT 3A 89 67 45
        ld ($56789a),a          ; @BT 32 9A 78 56
        ld hl,($6789ab)         ; @BT 2A AB 89 67
        ld sp,($789abc)         ; @BT ED 7B BC 9A 78
        ld ix,($89abcd)         ; @BT DD 2A CD AB 89
        ld iy,($9abcde)         ; @BT FD 2A DE BC 9A
        ld ($abcdef),ix         ; @BT DD 22 EF CD AB
        ld ($bcdef0),iy         ; @BT FD 22 F0 DE BC
        ld ix,$0f1234           ; @BT DD 21 34 12 0F
        ld iy,$102345           ; @BT FD 21 45 23 10
        ld sp,$112233           ; @BT 31 33 22 11
        .adl off
        call.s $4567            ; @BT 40 CD 67 45
        call.l $012345          ; @BT 5B CD 45 23 01
        ld.sis a,($2468)        ; @BT 40 3A 68 24
        ld.lil a,($024680)      ; @BT 5B 3A 80 46 02
        .adl on
        ld.sis bc,$5678         ; @BT 40 01 78 56
        ld.lil bc,$654321       ; @BT 5B 01 21 43 65
        .adl off
        .db "<12"               ; @BT END

        .define T13_ADDR16 $4567
        .define T13_ADDR24 $234567
        .define T13_DISP $20
        .define T13_BIT 7
        .function F13_ADDR(a,b,c) (a+b+c)
        .function F13_NEG(a,b) (a-b)
        .function F13_RST(a,b) (a+b)

        .db "13>"               ; @BT TEST-13 13 START
        .adl on
        jp F13_ADDR($120000,$003400,$56) ; @BT C3 56 34 12
        ld de,T13_ADDR24-$111111 ; @BT 11 56 34 12
        ld (T13_ADDR24+$1000),sp ; @BT ED 73 67 55 23
        .adl off
        ld a,(T13_ADDR16+(($20-4)+4)) ; @BT 3A 87 45
        lea hl,ix+T13_DISP      ; @BT ED 22 20
        pea iy-F13_RST(4,4)     ; @BT ED 66 F8
        bit T13_BIT,(ix+T13_DISP) ; @BT DD CB 20 7E
        set (T13_BIT-1),(iy-(T13_DISP/2)) ; @BT FD CB F0 F6
        rst F13_RST($10,$18)    ; @BT EF
        in a,(F13_NEG($20,$0e)) ; @BT DB 12
        out (F13_RST($30,$04)),a ; @BT D3 34
        .db "<13"               ; @BT END

        .ORGA $1000
        .db "14>"               ; @BT TEST-14 14 START
dotted.S:
        .dw dotted.S             ; @BT 03 10
dotted.LIL:
        .dw dotted.LIL           ; @BT 05 10
        .db "<14"               ; @BT END
