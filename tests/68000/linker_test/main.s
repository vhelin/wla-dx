
;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»
; a small test for wla-68000
;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»

.MEMORYMAP
   DEFAULTSLOT     0
   ; ROM area
   SLOTSIZE        $100000
   SLOT            0       $000000
   ; RAM area
   SLOTSIZE        $10000
   SLOT            1       $E00000
.ENDME

.ROMBANKMAP
BANKSTOTAL 1
BANKSIZE $100000
BANKS 1
.ENDRO

        .macro INST_1
        abcd.b d1, \1
        .endm

        .macro INST_2
        abcd.b \1, \2
        .endm

        .macro INST_3
        INST_2 "d1", \1
        .endm

        .macro INST_4
        \1
        .endm

        .macro INST_5
        abcd\1 d1, \2
        .endm

        .macro add_and_store_1
        move.w #(\1+\2),\3
        .endm

        .macro add_and_store_2
        move.l #(\1&\2),\3
        .endm

        .macro add_and_store_3
        move.l #(\1>>\2),\3
        .endm
        
.EMPTYFILL $AA

        .BANK 0 SLOT 0
        .ORG 0

                                ; @BT linked.rom

        .db "00>"               ; @BT TEST-00 00 START
        INST_4 "nop"            ; @BT 4E 71
        INST_1 "d2"             ; @BT C5 01
        INST_2 "d1", "d2"       ; @BT C5 01
        INST_3 "d2"             ; @BT C5 01
        INST_5 ".b", "d2"       ; @BT C5 01
        add_and_store_1 7, 120, "d7" ; @BT 7E 7F
        add_and_store_2 127, 127, "d7" ; @BT 7E 7F
        add_and_store_3 255, 1, "d7" ; @BT 7E 7F
        .db "<00"               ; @BT END

        .db "01>"               ; @BT TEST-01 01 START
        illegal                 ; @BT 4A FC
        nop                     ; @BT 4E 71
        reset                   ; @BT 4E 70
        rte                     ; @BT 4E 73
        rtr                     ; @BT 4E 77
        rts                     ; @BT 4E 75
        trapv                   ; @BT 4E 76
        .db "<01"               ; @BT END
        
        .db "02>"               ; @BT TEST-02 02 START
        abcd d0, d0             ; @BT C1 00
        abcd d1, d2             ; @BT C5 01
        abcd -(a0), -(a0)       ; @BT C1 08
        abcd -(a1), -(a2)       ; @BT C5 09
        abcd.b d0, d0           ; @BT C1 00
        abcd.b d1, d2           ; @BT C5 01
        abcd.b -(a0), -(a0)     ; @BT C1 08
        abcd.b -(a1), -(a2)     ; @BT C5 09
        .db "<02"               ; @BT END

        .db "03>"               ; @BT TEST-03 03 START
        add.b d0,d0 \ add.b d0,d0 ; @BT D0 00 D0 00
        add.w d0,d0             ; @BT D0 40
        add.l d0,d0             ; @BT D0 80
        add.b d1,d2             ; @BT D4 01
        add.w d1,d2             ; @BT D4 41
        add.l d1,d2             ; @BT D4 81
        add.w a0,d0             ; @BT D0 48
        add.l a0,d0             ; @BT D0 88
        add.w a1,d2             ; @BT D4 49
        add.l a1,d2             ; @BT D4 89
        add.b (a1),d2           ; @BT D4 11
        add.w (a1),d2           ; @BT D4 51
        add.l (a1),d2           ; @BT D4 91
        add.b -(a1),d2          ; @BT D4 21
        add.w -(a1),d2          ; @BT D4 61
        add.l -(a1),d2          ; @BT D4 A1
        add.b (a1)+,d2          ; @BT D4 19
        add.w (a1)+,d2          ; @BT D4 59
        add.l (a1)+,d2          ; @BT D4 99
        add.w $1234.w,d1        ; @BT D2 78 12 34
        add.l $1234.w,d1        ; @BT D2 B8 12 34
        add.w $12345678.l,d1    ; @BT D2 79 12 34 56 78
        add.l $12345678.l,d1    ; @BT D2 B9 12 34 56 78
        add.l 305419896.l,d1    ; @BT D2 B9 12 34 56 78
        add.l %00010010001101000101011001111000.l,d1 ; @BT D2 B9 12 34 56 78
        add.w $1234(a1),d2      ; @BT D4 69 12 34
        add.w ($1234,a1),d2     ; @BT D4 69 12 34
        add.b ($12,a2,d3),d4    ; @BT D8 32 30 12
        add.w ($34,a3,d4),d5    ; @BT DA 73 40 34
hello1: add.w hello1(pc),d2     ; @BT D4 7A FF FE
        add.w (yello1,pc),d2    ; @BT D4 7A 00 02
yello1: add.b (mello1,PC,d2),d3 ; @BT D6 3B 20 02
mello1: add.b (cello1,PC,d2),d3 ; @BT D6 3B 20 04
        .db 0, 3                ; @BT 00 03
cello1: add.b (mello1,PC,d2),d3 ; @BT D6 3B 20 F8
        add.w frank1(pc),d2     ; @BT D4 7A 00 04
        .db 1, 2                ; @BT 01 02
frank1: .db "<03"               ; @BT END

        .db "04>"               ; @BT TEST-04 04 START
        add.b d2,(a1)           ; @BT D5 11
        add.w d2,-(a1)          ; @BT D5 61
        add.l d2,(a1)+          ; @BT D5 99
        add.w d1,$1234.w        ; @BT D3 78 12 34
        add.l d1,$12345678.l    ; @BT D3 B9 12 34 56 78
        add.w d2,$1234(a1)      ; @BT D5 69 12 34
        add.b d2,($1234,a1)     ; @BT D5 29 12 34
        add.b d4,($12,a2,d3)    ; @BT D9 32 30 12
        .db "<04"               ; @BT END

        .define D_1234H = $1234
        .define D_12345678H = $12345678
        .define D_1233H = D_1234H - 1
        .define D_12345677H = D_12345678H - 1
        
        .db "05>"               ; @BT TEST-05 05 START
        add.w D_1234H.w,d1      ; @BT D2 78 12 34
        add.l D_12345678H.l,d1  ; @BT D2 B9 12 34 56 78
        add.w d1,D_1234H.w      ; @BT D3 78 12 34
        add.l d1,D_12345678H.l  ; @BT D3 B9 12 34 56 78
        add.w (D_1233H + 1).w,d1  ; @BT D2 78 12 34
        add.l (D_12345677H + 1).l,d1 ; @BT D2 B9 12 34 56 78
        add.w d1,(D_1233H + 1).w  ; @BT D3 78 12 34
        add.l d1,(D_12345677H + 1).l ; @BT D3 B9 12 34 56 78
        .db "<05"               ; @BT END

        .db "06>"               ; @BT TEST-06 06 START
        add.l a3,a7             ; @BT DF CB
        add.l a3, a7            ; @BT DF CB
        add.l a3,sp             ; @BT DF CB
        add.l a3, SP            ; @BT DF CB
        add.w d1, a2            ; @BT D4 C1
        add.l a3, a4            ; @BT D9 CB
        add.w (a5),a6           ; @BT DC D5
        add.w (a1)+,a2          ; @BT D4 D9
        add.l -(a3),a4          ; @BT D9 E3
        add.w $1234.w,a7        ; @BT DE F8 12 34
        add.w $1234.w,sp        ; @BT DE F8 12 34
        add.w $1234.w,SP        ; @BT DE F8 12 34
        add.l $12345678.l,a0    ; @BT D1 F9 12 34 56 78
        add.l $12345678,a0      ; @BT D1 F9 12 34 56 78
        add.l label_ABCE,a2     ; @BT D5 F9 00 00 AB CE
        add.l label_ABCE+1,a2   ; @BT D5 F9 00 00 AB CF
        add.w $1234(a1),a2      ; @BT D4 E9 12 34
        add.l ($1234,a1),a3     ; @BT D7 E9 12 34
        add.l ($12,a2,d3),a4    ; @BT D9 F2 30 12
hello2: add.w hello2(pc),a2     ; @BT D4 FA FF FE
        add.w (yello2,pc),a2    ; @BT D4 FA 00 02
yello2: add.l (mello2,PC,d2),a3 ; @BT D7 FB 20 02
mello2: add.l (cello2,PC,d2),a3 ; @BT D7 FB 20 04
        .db 0, 3                ; @BT 00 03
cello2: add.l (mello2,PC,d2),a3 ; @BT D7 FB 20 F8
        add.w frank2(pc),a6     ; @BT DC FA 00 04
        .db 1, 2                ; @BT 01 02
frank2: .db "<06"               ; @BT END

        .db "07>"               ; @BT TEST-07 07 START
        add.l #$12345678.l,d1   ; @BT 06 81 12 34 56 78
        add.l #$12345678,d1     ; @BT 06 81 12 34 56 78
        add.w #$1234,d2         ; @BT 06 42 12 34
        add.b #$12,d4           ; @BT 06 04 00 12
        add.b #$fe, (a2)        ; @BT 06 12 00 FE
        add.w #$1234, -(a3)     ; @BT 06 63 12 34
        add.l #$12345678, (a4)+ ; @BT 06 9C 12 34 56 78
        add.b #$12, $3456.w     ; @BT 06 38 00 12 34 56
        add.w #$abcd,$12345678.l ; @BT 06 79 AB CD 12 34 56 78
        add.w #$abcd, $12345678 ; @BT 06 79 AB CD 12 34 56 78
        add.b #$12,($3456,a5)   ; @BT 06 2D 00 12 34 56
        add.b #$12,$3456(a5)    ; @BT 06 2D 00 12 34 56
        add.w #$1234,($56,a6,d7) ; @BT 06 76 12 34 70 56
        add.l #$12345678.l,a1   ; @BT D3 FC 12 34 56 78
        add.l #$12345678,a2     ; @BT D5 FC 12 34 56 78
        add.w #$1234,a3         ; @BT D6 FC 12 34
        .db "<07"               ; @BT END

        .db "08>"                ; @BT TEST-08 08 START
        adda.w d1, a2            ; @BT D4 C1
        adda.l a3, a4            ; @BT D9 CB
        adda.w (a5),a6           ; @BT DC D5
        adda.w (a1)+,a2          ; @BT D4 D9
        adda.l -(a3),a4          ; @BT D9 E3
        adda.w $1234.w,a7        ; @BT DE F8 12 34
        adda.l $12345678.l,a0    ; @BT D1 F9 12 34 56 78
        adda.l $12345678,a0      ; @BT D1 F9 12 34 56 78
        adda.l label_ABCE,a2     ; @BT D5 F9 00 00 AB CE
        adda.l label_ABCE+1,a2   ; @BT D5 F9 00 00 AB CF
        adda.w $1234(a1),a2      ; @BT D4 E9 12 34
        adda.l ($1234,a1),a3     ; @BT D7 E9 12 34
        adda.l ($12,a2,d3),a4    ; @BT D9 F2 30 12
hello3: adda.w hello3(pc),a2     ; @BT D4 FA FF FE
        adda.w (yello3,pc),a2    ; @BT D4 FA 00 02
yello3: adda.l (mello3,PC,d2),a3 ; @BT D7 FB 20 02
mello3: adda.l (cello3,PC,d2),a3 ; @BT D7 FB 20 04
        .db 0, 3                 ; @BT 00 03
cello3: adda.l (mello3,PC,d2),a3 ; @BT D7 FB 20 F8
        adda.w frank3(pc),a6     ; @BT DC FA 00 04
        .db 1, 2                 ; @BT 01 02
frank3: .db "<08"                ; @BT END

        .db "09>"                ; @BT TEST-09 09 START
        addi.l #$12345678.l,d1   ; @BT 06 81 12 34 56 78
        addi.l #$12345678,d1     ; @BT 06 81 12 34 56 78
        addi.w #$1234,d2         ; @BT 06 42 12 34
        addi.b #$12,d4           ; @BT 06 04 00 12
        addi.b #$fe, (a2)        ; @BT 06 12 00 FE
        addi.w #$1234, -(a3)     ; @BT 06 63 12 34
        addi.l #$12345678, (a4)+ ; @BT 06 9C 12 34 56 78
        addi.b #$12, $3456.w     ; @BT 06 38 00 12 34 56
        addi.w #$abcd,$12345678.l ; @BT 06 79 AB CD 12 34 56 78
        addi.w #$abcd, $12345678 ; @BT 06 79 AB CD 12 34 56 78
        addi.b #$12,($3456,a5)   ; @BT 06 2D 00 12 34 56
        addi.b #$12,$3456(a5)    ; @BT 06 2D 00 12 34 56
        addi.w #$1234,($56,a6,d7) ; @BT 06 76 12 34 70 56
        .db "<09"                ; @BT END

        .db "10>"               ; @BT TEST-10 10 START
        add.l #2,d1             ; @BT 54 81
        add.w #3,d2             ; @BT 56 42
        add.b #4,d4             ; @BT 58 04
        add.b #5,(a2)           ; @BT 5A 12
        add.w #6,-(a3)          ; @BT 5C 63
        add.l #7,(a4)+          ; @BT 5E 9C
        add.b #8,$3456.w        ; @BT 50 38 34 56
        add.w #1,$12345678.l    ; @BT 52 79 12 34 56 78
        add.w #2,$12345678      ; @BT 54 79 12 34 56 78
        add.b #3,($3456,a5)     ; @BT 56 2D 34 56
        add.b #4,$3456(a5)      ; @BT 58 2D 34 56
        add.w #5,($56,a6,d7)    ; @BT 5A 76 70 56
        add.l #6,a1             ; @BT 5C 89
        add.l #7,a2             ; @BT 5E 8A
        add.w #8,a3             ; @BT 50 4B
        .db "<10"               ; @BT END

        .db "11>"               ; @BT TEST-11 11 START
        addq.l #2,d1            ; @BT 54 81
        addq.w #3,d2            ; @BT 56 42
        addq.b #4,d4            ; @BT 58 04
        addq.b #5,(a2)          ; @BT 5A 12
        addq.w #6,-(a3)         ; @BT 5C 63
        addq.l #7,(a4)+         ; @BT 5E 9C
        addq.b #8,$3456.w       ; @BT 50 38 34 56
        addq.w #1,$12345678.l   ; @BT 52 79 12 34 56 78
        addq.w #2,$12345678     ; @BT 54 79 12 34 56 78
        addq.b #3,($3456,a5)    ; @BT 56 2D 34 56
        addq.b #4,$3456(a5)     ; @BT 58 2D 34 56
        addq.w #5,($56,a6,d7)   ; @BT 5A 76 70 56
        addq.l #6,a1            ; @BT 5C 89
        addq.l #7,a2            ; @BT 5E 8A
        addq.w #8,a3            ; @BT 50 4B
        .db "<11"               ; @BT END

        .db "12>"               ; @BT TEST-12 12 START
        addx.b d1, d2           ; @BT D5 01
        addx.w d3, d4           ; @BT D9 43
        addx.l d7, d5           ; @BT DB 87
        addx.b -(a1), -(a2)     ; @BT D5 09
        addx.w -(a3), -(a4)     ; @BT D9 4B
        addx.l -(a7), -(a5)     ; @BT DB 8F
        addx.l -(sp), -(a5)     ; @BT DB 8F
        addx.l -(SP), -(a5)     ; @BT DB 8F
        .db "<12"               ; @BT END

        .db "13>"               ; @BT TEST-13 13 START
        and.l d1,d2             ; @BT C4 81
        and.b (a1),d2           ; @BT C4 11
        and.w -(a1),d2          ; @BT C4 61
        and.l (a1)+,d2          ; @BT C4 99
        and.l $1234.w,d1        ; @BT C2 B8 12 34
        and.w $12345678.l,d1    ; @BT C2 79 12 34 56 78
        and.w $1234(a1),d2      ; @BT C4 69 12 34
        and.w ($1234,a1),d2     ; @BT C4 69 12 34
        and.b ($12,a2,d3),d4    ; @BT C8 32 30 12
        and.w ($34,a3,d4),d5    ; @BT CA 73 40 34
hello4: and.w hello4(pc),d2     ; @BT C4 7A FF FE
        and.w (yello4,pc),d2    ; @BT C4 7A 00 02
yello4: and.b (mello4,PC,d2),d3 ; @BT C6 3B 20 02
mello4: and.b (cello4,PC,d2),d3 ; @BT C6 3B 20 04
        .db 0, 3                ; @BT 00 03
cello4: and.b (mello4,PC,d2),d3 ; @BT C6 3B 20 F8
        and.w frank4(pc),d2     ; @BT C4 7A 00 04
        .db 1, 2                ; @BT 01 02
frank4: .db "<13"               ; @BT END

        .db "14>"                ; @BT TEST-14 14 START
        andi.l #$12345678.l,d1   ; @BT 02 81 12 34 56 78
        andi.l #$12345678,d1     ; @BT 02 81 12 34 56 78
        andi.w #$1234,d2         ; @BT 02 42 12 34
        andi.b #$12,d4           ; @BT 02 04 00 12
        andi.b #$fe, (a2)        ; @BT 02 12 00 FE
        andi.w #$1234, -(a3)     ; @BT 02 63 12 34
        andi.l #$12345678, (a4)+ ; @BT 02 9C 12 34 56 78
        andi.b #$12, $3456.w     ; @BT 02 38 00 12 34 56
        andi.w #$abcd,$12345678.l ; @BT 02 79 AB CD 12 34 56 78
        andi.w #$abcd, $12345678 ; @BT 02 79 AB CD 12 34 56 78
        andi.b #$12,($3456,a5)   ; @BT 02 2D 00 12 34 56
        andi.b #$12,$3456(a5)    ; @BT 02 2D 00 12 34 56
        andi.w #$1234,($56,a6,d7) ; @BT 02 76 12 34 70 56
        .db "<14"                ; @BT END

        .db "15>"               ; @BT TEST-15 15 START
        and.l #2,d1             ; @BT 02 81 00 00 00 02
        and.w #3,d2             ; @BT 02 42 00 03
        and.b #4,d4             ; @BT 02 04 00 04
        and.b #5,(a2)           ; @BT 02 12 00 05
        and.w #6,-(a3)          ; @BT 02 63 00 06
        and.l #7,(a4)+          ; @BT 02 9C 00 00 00 07
        and.b #8,$3456.w        ; @BT 02 38 00 08 34 56
        and.w #1,$12345678.l    ; @BT 02 79 00 01 12 34 56 78
        and.w #2,$12345678      ; @BT 02 79 00 02 12 34 56 78
        and.b #3,($3456,a5)     ; @BT 02 2D 00 03 34 56
        and.b #4,$3456(a5)      ; @BT 02 2D 00 04 34 56
        and.w #5,($56,a6,d7)    ; @BT 02 76 00 05 70 56
        .db "<15"               ; @BT END

        .db "16>"              ; @BT TEST-16 16 START
        andi.l #2,d1           ; @BT 02 81 00 00 00 02
        andi.w #3,d2           ; @BT 02 42 00 03
        andi.b #4,d4           ; @BT 02 04 00 04
        andi.b #5,(a2)         ; @BT 02 12 00 05
        andi.w #6,-(a3)        ; @BT 02 63 00 06
        andi.l #7,(a4)+        ; @BT 02 9C 00 00 00 07
        andi.b #8,$3456.w      ; @BT 02 38 00 08 34 56
        andi.w #1,$12345678.l  ; @BT 02 79 00 01 12 34 56 78
        andi.w #2,$12345678    ; @BT 02 79 00 02 12 34 56 78
        andi.b #3,($3456,a5)   ; @BT 02 2D 00 03 34 56
        andi.b #4,$3456(a5)    ; @BT 02 2D 00 04 34 56
        andi.w #5,($56,a6,d7)  ; @BT 02 76 00 05 70 56
        andi.b #10,CCR         ; @BT 02 3C 00 0A
        andi.b #$ff,CCR        ; @BT 02 3C 00 FF
        andi.w #$1234,SR       ; @BT 02 7C 12 34
        andi.w #$FFEE,SR       ; @BT 02 7C FF EE
        .db "<16"              ; @BT END

        .db "17>"         ; @BT TEST-17 17 START
        asr d4,d7         ; @BT E8 67
        asl.b d1,d5       ; @BT E3 25
        asr.w d4,d2       ; @BT E8 62
        asr.l #8,d1       ; @BT E0 81
        asl.l #2,d1       ; @BT E5 81
        asr.w #5,d2       ; @BT EA 42
        asr (a2)          ; @BT E0 D2
        asl.w -(a3)       ; @BT E1 E3
        asr.w (a4)+       ; @BT E0 DC
        asl.w $3456.w     ; @BT E1 F8 34 56
        asl.w $12345678.l ; @BT E1 F9 12 34 56 78
        asr.w ($3456,a5)  ; @BT E0 ED 34 56
        asl.w $3456(a5)   ; @BT E1 ED 34 56
        asr.w ($56,a6,d7) ; @BT E0 F6 70 56
        .db "<17"         ; @BT END

        .db "18>"         ; @BT TEST-18 18 START
        bcc.b bcc_test_1  ; @BT 64 08
        bcs.w bcc_test_1  ; @BT 65 00 00 06
        beq.w bcc_test_1  ; @BT 67 00 00 02
bcc_test_1:
        bge.b bcc_test_1  ; @BT 6C FE
        bgt.w bcc_test_1  ; @BT 6E 00 FF FC
        bhi.b bcc_test_1  ; @BT 62 F8
        ble.w bcc_test_1  ; @BT 6F 00 FF F6
        bls.b bcc_test_1  ; @BT 63 F2
        blt.w bcc_test_1  ; @BT 6D 00 FF F0
        bmi.b bcc_test_1  ; @BT 6B EC
        bne.w bcc_test_1  ; @BT 66 00 FF EA
        bpl.b bcc_test_1  ; @BT 6A E6
        bvc.w bcc_test_1  ; @BT 68 00 FF E4
        bvs.b bcc_test_1  ; @BT 69 E0
        .db "<18"         ; @BT END

        .db "19>"           ; @BT TEST-19 19 START
        bcc.b bcc_test_2+1  ; @BT 64 09
        bcs.w bcc_test_2+1  ; @BT 65 00 00 07
        beq.w bcc_test_2+1  ; @BT 67 00 00 03
bcc_test_2:
        bge.b bcc_test_2+1  ; @BT 6C FF
        bgt.w bcc_test_2+1  ; @BT 6E 00 FF FD
        bhi.b bcc_test_2+1  ; @BT 62 F9
        ble.w bcc_test_2+1  ; @BT 6F 00 FF F7
        bls.b bcc_test_2+1  ; @BT 63 F3
        blt.w bcc_test_2+1  ; @BT 6D 00 FF F1
        bmi.b bcc_test_2+1  ; @BT 6B ED
        bne.w bcc_test_2+1  ; @BT 66 00 FF EB
        bpl.b bcc_test_2+1  ; @BT 6A E7
        bvc.w bcc_test_2+1  ; @BT 68 00 FF E5
        bvs.b bcc_test_2+1  ; @BT 69 E1
        .db "<19"           ; @BT END

        .db "20>"          ; @BT TEST-20 20 START
        bra.b bcc_test_3   ; @BT 60 08
        bra.w bcc_test_3   ; @BT 60 00 00 06
        bra.w bcc_test_3   ; @BT 60 00 00 02
bcc_test_3:
        bsr.b bcc_test_3   ; @BT 61 FE
        bsr.w bcc_test_3   ; @BT 61 00 FF FC
        bsr.b bcc_test_3   ; @BT 61 F8
        bra.b bcc_test_4+1 ; @BT 60 09
        bra.w bcc_test_4+1 ; @BT 60 00 00 07
        bra.w bcc_test_4+1 ; @BT 60 00 00 03
bcc_test_4:
        bsr.b bcc_test_4+1 ; @BT 61 FF
        bsr.w bcc_test_4+1 ; @BT 61 00 FF FD
        bsr.b bcc_test_4+1 ; @BT 61 F9
        .db "<20"          ; @BT END

        .db "21>"           ; @BT TEST-21 21 START
        bchg d5,d6          ; @BT 0B 46
        bchg #30,d3         ; @BT 08 43 00 1E
        bchg d3,(a6)        ; @BT 07 56
        bchg #7,(a4)+       ; @BT 08 5C 00 07
        bchg #0,(a4)+       ; @BT 08 5C 00 00
        bchg d2,$1234(a7)   ; @BT 05 6F 12 34
        bchg d2,$1234(SP)   ; @BT 05 6F 12 34
        bchg d2,$1234(sp)   ; @BT 05 6F 12 34        
        bchg #7,($5678,a6)  ; @BT 08 6E 00 07 56 78
        bchg d1,(8,a5,d2)   ; @BT 03 75 20 08
        bchg d6,-(a1)       ; @BT 0D 61
        bchg #1,$1234.w     ; @BT 08 78 00 01 12 34
        bchg d7,$12345678.l ; @BT 0F 79 12 34 56 78
        .db "<21"           ; @BT END

        .db "22>"           ; @BT TEST-22 22 START
        bclr d5,d6          ; @BT 0B 86
        bclr #30,d3         ; @BT 08 83 00 1E
        bclr d3,(a6)        ; @BT 07 96
        bclr #7,(a4)+       ; @BT 08 9C 00 07
        bclr #0,(a4)+       ; @BT 08 9C 00 00
        bclr d2,$1234(sp)   ; @BT 05 AF 12 34
        bclr d2,$1234(a7)   ; @BT 05 AF 12 34
        bclr d2,$1234(SP)   ; @BT 05 AF 12 34
        bclr #7,($5678,a6)  ; @BT 08 AE 00 07 56 78
        bclr d1,(8,a5,d2)   ; @BT 03 B5 20 08
        bclr d6,-(a1)       ; @BT 0D A1
        bclr #1,$1234.w     ; @BT 08 B8 00 01 12 34
        bclr d7,$12345678.l ; @BT 0F B9 12 34 56 78
        .db "<22"           ; @BT END

        .db "23>"           ; @BT TEST-23 23 START
        bset d5,d6          ; @BT 0B C6
        bset #30,d3         ; @BT 08 C3 00 1E
        bset d3,(a6)        ; @BT 07 D6
        bset #0,(a4)+       ; @BT 08 DC 00 00
        bset #7,(a4)+       ; @BT 08 DC 00 07
        bset d2,$1234(SP)   ; @BT 05 EF 12 34
        bset d2,$1234(a7)   ; @BT 05 EF 12 34
        bset d2,$1234(sp)   ; @BT 05 EF 12 34
        bset #7,($5678,a6)  ; @BT 08 EE 00 07 56 78
        bset d1,(8,a5,d2)   ; @BT 03 F5 20 08
        bset d6,-(a1)       ; @BT 0D E1
        bset #1,label_ABCE.w ; @BT 08 F8 00 01 AB CE
        bset #1,label_ABCE  ; @BT 08 F9 00 01 00 00 AB CE
        bset d7,$12345678.l ; @BT 0F F9 12 34 56 78
        .db "<23"           ; @BT END

        .db "24>"           ; @BT TEST-24 24 START
        btst d5,d6          ; @BT 0B 06
        btst #30,d3         ; @BT 08 03 00 1E
        btst d3,(a6)        ; @BT 07 16
        btst #7,(a4)+       ; @BT 08 1C 00 07
        btst #0,(a4)+       ; @BT 08 1C 00 00
        btst d2,$1234(sp)   ; @BT 05 2F 12 34
        btst d2,$1234(SP)   ; @BT 05 2F 12 34
        btst d2,$1234(a7)   ; @BT 05 2F 12 34
        btst #7,($5678,a6)  ; @BT 08 2E 00 07 56 78
        btst d1,(8,a5,d2)   ; @BT 03 35 20 08
        btst d6,-(a1)       ; @BT 0D 21
        btst #1,$1234.w     ; @BT 08 38 00 01 12 34
        btst d7,$12345678.l ; @BT 0F 39 12 34 56 78
        .db "<24"           ; @BT END

        .db "25>"           ; @BT TEST-25 25 START
        chk d3,d7           ; @BT 4F 83
        chk (a1),d6         ; @BT 4D 91
        chk (a2)+,d5        ; @BT 4B 9A
        chk -(a3),d4        ; @BT 49 A3
        chk ($1234,a4),d3   ; @BT 47 AC 12 34
        chk $1234(a5),d2    ; @BT 45 AD 12 34
        chk ($12,a6,d1),d0  ; @BT 41 B6 10 12
        chk $1234.w,d1      ; @BT 43 B8 12 34
        chk $12345678.l,d2  ; @BT 45 B9 12 34 56 78
        chk #$1234,d3       ; @BT 47 BC 12 34
        chk (end_25,pc),d4  ; @BT 49 BA 00 0D
        chk end_25(pc),d5   ; @BT 4B BA 00 09
        chk (end_25,pc,d6),d7 ; @BT 4F BB 60 05
        .db "<25"           ; @BT END
end_25: 

        .db "26>"           ; @BT TEST-26 26 START
        clr.b d6            ; @BT 42 06
        clr.w (a4)          ; @BT 42 54
        clr.l -(SP)         ; @BT 42 A7
        clr.l -(sp)         ; @BT 42 A7
        clr.l -(a7)         ; @BT 42 A7
        clr.b (a6)+         ; @BT 42 1E
        clr.w $1234(a5)     ; @BT 42 6D 12 34
        clr.l ($1234,a3)    ; @BT 42 AB 12 34
        clr.b ($12,a1,d5)   ; @BT 42 31 50 12
        clr.l $1234.w       ; @BT 42 B8 12 34
        clr.w $12345678.l   ; @BT 42 79 12 34 56 78
        .db "<26"           ; @BT END

        .db "27>"           ; @BT TEST-27 27 START
        cmp.b (a3)+,(sp)+   ; @BT BF 0B
        cmp.b (a3)+,(a7)+   ; @BT BF 0B
        cmp.b (a3)+,(SP)+   ; @BT BF 0B
        cmp.w (a6)+,(a1)+   ; @BT B3 4E
        cmpm.b (a3)+,(SP)+  ; @BT BF 0B
        cmpm.b (a3)+,(a7)+  ; @BT BF 0B
        cmpm.b (a3)+,(sp)+  ; @BT BF 0B
        cmpm.w (a6)+,(a1)+  ; @BT B3 4E
        cmp.l d3,d7         ; @BT BE 83
        cmp.b (a1),d6       ; @BT BC 11
        cmp.w (a2)+,d5      ; @BT BA 5A
        cmp.l -(a3),d4      ; @BT B8 A3
        cmp.b ($1234,a4),d3   ; @BT B6 2C 12 34
        cmp.w $1234(a5),d2    ; @BT B4 6D 12 34
        cmp.l ($12,a6,d1),d0  ; @BT B0 B6 10 12
        cmp.b $1234.w,d1      ; @BT B2 38 12 34
        cmp.w $12345678.l,d2  ; @BT B4 79 12 34 56 78
        cmp.l #$1234,d3       ; @BT 0C 83 00 00 12 34
        cmp.b (end_27,pc),d4  ; @BT B8 3A 00 0A
        cmp.w end_27(pc),d5   ; @BT BA 7A 00 06
        cmp.l (end_27,pc,d6),d7 ; @BT BE BB 60 02
end_27: .db "<27"           ; @BT END

        .db "28>"           ; @BT TEST-28 28 START
        cmp.b #$12,(a1)              ; @BT 0C 11 00 12
        cmp.w #$1234,(a2)+           ; @BT 0C 5A 12 34
        cmp.l #$12345678,-(a3)       ; @BT 0C A3 12 34 56 78
        cmp.b #$56,($1234,a4)        ; @BT 0C 2C 00 56 12 34
        cmp.w #$5678,$1234(a5)       ; @BT 0C 6D 56 78 12 34
        cmp.l #$12345678,($12,a6,d1) ; @BT 0C B6 12 34 56 78 10 12
        cmp.b #$56,$1234.w           ; @BT 0C 38 00 56 12 34
        cmp.w #$1234,$12345678.l     ; @BT 0C 79 12 34 12 34 56 78
        .db "<28"           ; @BT END

        .db "29>"           ; @BT TEST-29 29 START
        cmp.l d3,a1         ; @BT B3 C3
        cmp.l (a1),a6       ; @BT BD D1
        cmp.w (a2)+,a5      ; @BT BA DA
        cmp.l -(a3),a4      ; @BT B9 E3
        cmp.l ($1234,a4),a3   ; @BT B7 EC 12 34
        cmp.w $1234(a5),a2    ; @BT B4 ED 12 34
        cmp.l ($12,a6,d1),a0  ; @BT B1 F6 10 12
        cmp.l $1234.w,a1      ; @BT B3 F8 12 34
        cmp.w $12345678.l,a2  ; @BT B4 F9 12 34 56 78
        cmp.l #$1234,a3       ; @BT B7 FC 00 00 12 34
        cmp.l (end_29,pc),a4  ; @BT B9 FA 00 12
        cmp.w end_29(pc),a5   ; @BT BA FA 00 0E
        cmp.l (end_29,pc,d6),a7 ; @BT BF FB 60 0A
        cmp.l (end_29,pc,d6),sp ; @BT BF FB 60 06
        cmp.l (end_29,pc,d6),SP ; @BT BF FB 60 02
end_29: .db "<29"           ; @BT END

        .db "30>"           ; @BT TEST-30 30 START
        cmpi.b #$12,(a1)              ; @BT 0C 11 00 12
        cmpi.w #$1234,(a2)+           ; @BT 0C 5A 12 34
        cmpi.l #$12345678,-(a3)       ; @BT 0C A3 12 34 56 78
        cmpi.b #$56,($1234,a4)        ; @BT 0C 2C 00 56 12 34
        cmpi.w #$5678,$1234(a5)       ; @BT 0C 6D 56 78 12 34
        cmpi.l #$12345678,($12,a6,d1) ; @BT 0C B6 12 34 56 78 10 12
        cmpi.b #$56,$1234.w           ; @BT 0C 38 00 56 12 34
        cmpi.w #$1234,$12345678.l     ; @BT 0C 79 12 34 12 34 56 78
        .db "<30"           ; @BT END

        .db "31>"           ; @BT TEST-31 31 START
        cmpa.l d3,a1         ; @BT B3 C3
        cmpa.l (a1),a6       ; @BT BD D1
        cmpa.w (a2)+,a5      ; @BT BA DA
        cmpa.l -(a3),a4      ; @BT B9 E3
        cmpa.l ($1234,a4),a3   ; @BT B7 EC 12 34
        cmpa.w $1234(a5),a2    ; @BT B4 ED 12 34
        cmpa.l ($12,a6,d1),a0  ; @BT B1 F6 10 12
        cmpa.l $1234.w,a1      ; @BT B3 F8 12 34
        cmpa.w $12345678.l,a2  ; @BT B4 F9 12 34 56 78
        cmpa.l #$1234,a3       ; @BT B7 FC 00 00 12 34
        cmpa.l (end_31,pc),a4  ; @BT B9 FA 00 12
        cmpa.w end_31(pc),a5   ; @BT BA FA 00 0E
        cmpa.l (end_31,pc,d6),a7 ; @BT BF FB 60 0A
        cmpa.l (end_31,pc,d6),SP ; @BT BF FB 60 06
        cmpa.l (end_31,pc,d6),sp ; @BT BF FB 60 02
end_31: .db "<31"           ; @BT END

        .db "32>"           ; @BT TEST-32 32 START
        dbcc d0,label_32       ; @BT 54 C8 00 0A
        dbcs d1,label_32       ; @BT 55 C9 00 06
        dbeq d2,label_32       ; @BT 57 CA 00 02
label_32:
        dbf d3,label_32        ; @BT 51 CB FF FE
        dbge d4,label_32       ; @BT 5C CC FF FA
        dbgt d5,label_32       ; @BT 5E CD FF F6
        dbhi d6,label_32       ; @BT 52 CE FF F2
        dble d7,label_32       ; @BT 5F CF FF EE
        dbls d0,label_32       ; @BT 53 C8 FF EA
        dblt d1,label_32       ; @BT 5D C9 FF E6
        dbmi d2,label_32       ; @BT 5B CA FF E2
        dbne d3,label_32       ; @BT 56 CB FF DE
        dbpl d4,label_32       ; @BT 5A CC FF DA
        dbt d5,label_32        ; @BT 50 CD FF D6
        dbvc d6,label_32       ; @BT 58 CE FF D2
        dbvs d7,label_32       ; @BT 59 CF FF CE
        dbra d0,label_32       ; @BT 51 C8 FF CA
        .db "<32"           ; @BT END

        .db "33>"            ; @BT TEST-33 33 START
        divs d3,d7           ; @BT 8F C3
        divs (a1),d6         ; @BT 8D D1
        divs (a2)+,d5        ; @BT 8B DA
        divs -(a3),d4        ; @BT 89 E3
        divs ($1234,a4),d3   ; @BT 87 EC 12 34
        divs $1234(a5),d2    ; @BT 85 ED 12 34
        divs ($12,a6,d1),d0  ; @BT 81 F6 10 12
        divs $1234.w,d1      ; @BT 83 F8 12 34
        divs $12345678.l,d2  ; @BT 85 F9 12 34 56 78
        divs #$1234,d3       ; @BT 87 FC 12 34
        divs (end_33,pc),d4  ; @BT 89 FA 00 0A
        divs end_33(pc),d5   ; @BT 8B FA 00 06
        divs (end_33,pc,d6),d7 ; @BT 8F FB 60 02
end_33: .db "<33"            ; @BT END

        .db "34>"            ; @BT TEST-34 34 START
        divs.w d3,d7           ; @BT 8F C3
        divs.w (a1),d6         ; @BT 8D D1
        divs.w (a2)+,d5        ; @BT 8B DA
        divs.w -(a3),d4        ; @BT 89 E3
        divs.w ($1234,a4),d3   ; @BT 87 EC 12 34
        divs.w $1234(a5),d2    ; @BT 85 ED 12 34
        divs.w ($12,a6,d1),d0  ; @BT 81 F6 10 12
        divs.w $1234.w,d1      ; @BT 83 F8 12 34
        divs.w $12345678.l,d2  ; @BT 85 F9 12 34 56 78
        divs.w #$1234,d3       ; @BT 87 FC 12 34
        divs.w (end_34,pc),d4  ; @BT 89 FA 00 0A
        divs.w end_34(pc),d5   ; @BT 8B FA 00 06
        divs.w (end_34,pc,d6),d7 ; @BT 8F FB 60 02
end_34: .db "<34"            ; @BT END

        .db "35>"            ; @BT TEST-35 35 START
        divu d3,d7           ; @BT 8E C3
        divu (a1),d6         ; @BT 8C D1
        divu (a2)+,d5        ; @BT 8A DA
        divu -(a3),d4        ; @BT 88 E3
        divu ($1234,a4),d3   ; @BT 86 EC 12 34
        divu $1234(a5),d2    ; @BT 84 ED 12 34
        divu ($12,a6,d1),d0  ; @BT 80 F6 10 12
        divu $1234.w,d1      ; @BT 82 F8 12 34
        divu $12345678.l,d2  ; @BT 84 F9 12 34 56 78
        divu #$1234,d3       ; @BT 86 FC 12 34
        divu (end_35,pc),d4  ; @BT 88 FA 00 0A
        divu end_35(pc),d5   ; @BT 8A FA 00 06
        divu (end_35,pc,d6),d7 ; @BT 8E FB 60 02
end_35: .db "<35"            ; @BT END

        .db "36>"            ; @BT TEST-36 36 START
        divu.w d3,d7           ; @BT 8E C3
        divu.w (a1),d6         ; @BT 8C D1
        divu.w (a2)+,d5        ; @BT 8A DA
        divu.w -(a3),d4        ; @BT 88 E3
        divu.w ($1234,a4),d3   ; @BT 86 EC 12 34
        divu.w $1234(a5),d2    ; @BT 84 ED 12 34
        divu.w ($12,a6,d1),d0  ; @BT 80 F6 10 12
        divu.w $1234.w,d1      ; @BT 82 F8 12 34
        divu.w $12345678.l,d2  ; @BT 84 F9 12 34 56 78
        divu.w #$1234,d3       ; @BT 86 FC 12 34
        divu.w (end_36,pc),d4  ; @BT 88 FA 00 0A
        divu.w end_36(pc),d5   ; @BT 8A FA 00 06
        divu.w (end_36,pc,d6),d7 ; @BT 8E FB 60 02
end_36: .db "<36"            ; @BT END

        .db "37>"                 ; @BT TEST-37 37 START
        eori.l #$12345678.l,d1    ; @BT 0A 81 12 34 56 78
        eori.l #$12345678,d1      ; @BT 0A 81 12 34 56 78
        eori.w #$1234,d2          ; @BT 0A 42 12 34
        eori.b #$12,d4            ; @BT 0A 04 00 12
        eori.b #$fe, (a2)         ; @BT 0A 12 00 FE
        eori.w #$1234, -(a3)      ; @BT 0A 63 12 34
        eori.l #$12345678, (a4)+  ; @BT 0A 9C 12 34 56 78
        eori.b #$12, $3456.w      ; @BT 0A 38 00 12 34 56
        eori.w #$abcd,$12345678.l ; @BT 0A 79 AB CD 12 34 56 78
        eori.w #$abcd, $12345678  ; @BT 0A 79 AB CD 12 34 56 78
        eori.b #$12,($3456,a5)    ; @BT 0A 2D 00 12 34 56
        eori.b #$12,$3456(a5)     ; @BT 0A 2D 00 12 34 56
        eori.w #$1234,($56,a6,d7) ; @BT 0A 76 12 34 70 56
        .db "<37"                 ; @BT END

        .db "38>"                ; @BT TEST-38 38 START
        eor.l #$12345678.l,d1    ; @BT 0A 81 12 34 56 78
        eor.l #$12345678,d1      ; @BT 0A 81 12 34 56 78
        eor.w #$1234,d2          ; @BT 0A 42 12 34
        eor.b #$12,d4            ; @BT 0A 04 00 12
        eor.b #$fe, (a2)         ; @BT 0A 12 00 FE
        eor.w #$1234, -(a3)      ; @BT 0A 63 12 34
        eor.l #$12345678, (a4)+  ; @BT 0A 9C 12 34 56 78
        eor.b #$12, $3456.w      ; @BT 0A 38 00 12 34 56
        eor.w #$abcd,$12345678.l ; @BT 0A 79 AB CD 12 34 56 78
        eor.w #$abcd, $12345678  ; @BT 0A 79 AB CD 12 34 56 78
        eor.b #$12,($3456,a5)    ; @BT 0A 2D 00 12 34 56
        eor.b #$12,$3456(a5)     ; @BT 0A 2D 00 12 34 56
        eor.w #$1234,($56,a6,d7) ; @BT 0A 76 12 34 70 56
        .db "<38"                ; @BT END

        .db "39>"              ; @BT TEST-39 39 START
        eori.b #10,CCR         ; @BT 0A 3C 00 0A
        eori.b #$ff,CCR        ; @BT 0A 3C 00 FF
        eori.w #$1234,SR       ; @BT 0A 7C 12 34
        eori.w #$FFEE,SR       ; @BT 0A 7C FF EE
        .db "<39"              ; @BT END

        .db "40>"            ; @BT TEST-40 40 START
        eor.l d7,d1          ; @BT BF 81
        eor.l d6,d1          ; @BT BD 81
        eor.w d5,d2          ; @BT BB 42
        eor.b d4,d4          ; @BT B9 04
        eor.b d3,(a2)        ; @BT B7 12
        eor.w d2,-(a3)       ; @BT B5 63
        eor.l d1,(a4)+       ; @BT B3 9C
        eor.b d0,$3456.w     ; @BT B1 38 34 56
        eor.w d1,$12345678.l ; @BT B3 79 12 34 56 78
        eor.w d2,$12345678   ; @BT B5 79 12 34 56 78
        eor.b d3,($3456,a5)  ; @BT B7 2D 34 56
        eor.b d4,$3456(a5)   ; @BT B9 2D 34 56
        eor.w d5,($56,a6,d7) ; @BT BB 76 70 56
        .db "<40"            ; @BT END

        .db "41>"            ; @BT TEST-41 41 START
        exg d2,d7            ; @BT C5 47
        exg d7,d2            ; @BT CF 42
        exg a3,a6            ; @BT C7 4E
        exg a6,a3            ; @BT CD 4B
        exg d1,a5            ; @BT C3 8D
        exg d5,a1            ; @BT CB 89
        .db "<41"            ; @BT END

        .db "42>"            ; @BT TEST-42 42 START
        ext.w d1             ; @BT 48 81
        ext.w d7             ; @BT 48 87
        ext.l d2             ; @BT 48 C2
        ext.l d5             ; @BT 48 C5
        .db "<42"            ; @BT END

        .db "43>"            ; @BT TEST-43 43 START
        jmp (a5)             ; @BT 4E D5
        jmp $1234(sp)        ; @BT 4E EF 12 34
        jmp $1234(SP)        ; @BT 4E EF 12 34
        jmp $1234(a7)        ; @BT 4E EF 12 34
        jmp ($1234,a6)       ; @BT 4E EE 12 34
        jmp ($12,a4,d3)      ; @BT 4E F4 30 12
        jmp label_ABCE.w     ; @BT 4E F8 AB CE
        jmp label_ABCE.l     ; @BT 4E F9 00 00 AB CE
        jmp end_43(pc)       ; @BT 4E FA 00 06
        jmp (end_43,pc,d2)   ; @BT 4E FB 20 02
end_43: .db "<43"            ; @BT END

        .db "44>"            ; @BT TEST-44 44 START
        jsr (a5)             ; @BT 4E 95
        jsr $1234(SP)        ; @BT 4E AF 12 34
        jsr $1234(sp)        ; @BT 4E AF 12 34
        jsr $1234(a7)        ; @BT 4E AF 12 34
        jsr ($1234,a6)       ; @BT 4E AE 12 34
        jsr ($12,a4,d3)      ; @BT 4E B4 30 12
        jsr label_ABCE.w     ; @BT 4E B8 AB CE
        jsr label_ABCE.l     ; @BT 4E B9 00 00 AB CE
        jsr end_44(pc)       ; @BT 4E BA 00 06
        jsr (end_44,pc,d2)   ; @BT 4E BB 20 02
end_44: .db "<44"            ; @BT END

        .db "45>"             ; @BT TEST-45 45 START
        lea (a5),a1           ; @BT 43 D5
        lea $1234(sp),a2      ; @BT 45 EF 12 34
        lea $1234(SP),a2      ; @BT 45 EF 12 34
        lea $1234(a7),a2      ; @BT 45 EF 12 34
        lea ($1234,a6),a3     ; @BT 47 EE 12 34
        lea ($12,a4,d3),a4    ; @BT 49 F4 30 12
        lea label_ABCE.w,a5   ; @BT 4B F8 AB CE
        lea label_ABCE.l,a6   ; @BT 4D F9 00 00 AB CE
        lea end_45(pc),sp     ; @BT 4F FA 00 0E
        lea end_45(pc),SP     ; @BT 4F FA 00 0A
        lea end_45(pc),a7     ; @BT 4F FA 00 06
        lea (end_45,pc,d2),a1 ; @BT 43 FB 20 02
end_45: .db "<45"             ; @BT END
        
        .db "46>"             ; @BT TEST-46 46 START
        link a4, #$1234       ; @BT 4E 54 12 34
        link SP,#$5678        ; @BT 4E 57 56 78
        link a7,#$5678        ; @BT 4E 57 56 78
        link sp,#$5678        ; @BT 4E 57 56 78
        .db "<46"             ; @BT END

        .db "47>"         ; @BT TEST-47 47 START
        lsr d4,d7         ; @BT E8 6F
        lsl.b d1,d5       ; @BT E3 2D
        lsr.w d4,d2       ; @BT E8 6A
        lsr.l #8,d1       ; @BT E0 89
        lsl.l #2,d1       ; @BT E5 89
        lsr.w #5,d2       ; @BT EA 4A
        lsr.w (a4)+       ; @BT E2 DC
        lsl.w $3456.w     ; @BT E3 F8 34 56
        lsl.w $12345678.l ; @BT E3 F9 12 34 56 78
        lsr.w ($3456,a5)  ; @BT E2 ED 34 56
        lsl.w $3456(a5)   ; @BT E3 ED 34 56
        lsr.w ($56,a6,d7) ; @BT E2 F6 70 56
        lsr (a2)          ; @BT E2 D2
        lsr.w (a2)        ; @BT E2 D2
        lsl.w -(a3)       ; @BT E3 E3
        .db "<47"         ; @BT END

        .db "48>"              ; @BT TEST-48 48 START
        muls d3,d7             ; @BT CF C3
        muls (a1),d6           ; @BT CD D1
        muls.w (a2)+,d5        ; @BT CB DA
        muls -(a3),d4          ; @BT C9 E3
        muls ($1234,a4),d3     ; @BT C7 EC 12 34
        muls.w $1234(a5),d2    ; @BT C5 ED 12 34
        muls ($12,a6,d1),d0    ; @BT C1 F6 10 12
        muls $1234.w,d1        ; @BT C3 F8 12 34
        muls.w $12345678.l,d2  ; @BT C5 F9 12 34 56 78
        muls #$1234,d3         ; @BT C7 FC 12 34
        muls.w (end_48,pc),d4  ; @BT C9 FA 00 0A
        muls end_48(pc),d5     ; @BT CB FA 00 06
        muls (end_48,pc,d6),d7 ; @BT CF FB 60 02
end_48: .db "<48"              ; @BT END

        .db "49>"              ; @BT TEST-49 49 START
        mulu d3,d7             ; @BT CE C3
        mulu (a1),d6           ; @BT CC D1
        mulu.w (a2)+,d5        ; @BT CA DA
        mulu -(a3),d4          ; @BT C8 E3
        mulu ($1234,a4),d3     ; @BT C6 EC 12 34
        mulu.w $1234(a5),d2    ; @BT C4 ED 12 34
        mulu ($12,a6,d1),d0    ; @BT C0 F6 10 12
        mulu $1234.w,d1        ; @BT C2 F8 12 34
        mulu.w $12345678.l,d2  ; @BT C4 F9 12 34 56 78
        mulu #$1234,d3         ; @BT C6 FC 12 34
        mulu.w (end_49,pc),d4  ; @BT C8 FA 00 0A
        mulu end_49(pc),d5     ; @BT CA FA 00 06
        mulu (end_49,pc,d6),d7 ; @BT CE FB 60 02
end_49: .db "<49"              ; @BT END

        .db "50>"              ; @BT TEST-50 50 START
        nbcd d3                ; @BT 48 03
        nbcd (a5)              ; @BT 48 15
        nbcd (a6)+             ; @BT 48 1E
        nbcd -(a7)             ; @BT 48 27
        nbcd -(SP)             ; @BT 48 27
        nbcd -(sp)             ; @BT 48 27
        nbcd $1234(a1)         ; @BT 48 29 12 34
        nbcd ($12,a2,d3)       ; @BT 48 32 30 12
        nbcd label_ABCE.w      ; @BT 48 38 AB CE
        nbcd label_ABCE.l      ; @BT 48 39 00 00 AB CE
end_50: .db "<50"              ; @BT END

        .db "51>"           ; @BT TEST-51 51 START
        neg.b d6            ; @BT 44 06
        neg.w (a4)          ; @BT 44 54
        neg.l -(a7)         ; @BT 44 A7
        neg.l -(sp)         ; @BT 44 A7
        neg.l -(SP)         ; @BT 44 A7
        neg.b (a6)+         ; @BT 44 1E
        neg.w $1234(a5)     ; @BT 44 6D 12 34
        neg.l ($1234,a3)    ; @BT 44 AB 12 34
        neg.b ($12,a1,d5)   ; @BT 44 31 50 12
        neg.l $1234.w       ; @BT 44 B8 12 34
        neg.w $12345678.l   ; @BT 44 79 12 34 56 78
        .db "<51"           ; @BT END

        .db "52>"            ; @BT TEST-52 52 START
        negx.b d6            ; @BT 40 06
        negx.w (a4)          ; @BT 40 54
        negx.l -(sp)         ; @BT 40 A7
        negx.l -(SP)         ; @BT 40 A7
        negx.l -(a7)         ; @BT 40 A7
        negx.b (a6)+         ; @BT 40 1E
        negx.w $1234(a5)     ; @BT 40 6D 12 34
        negx.l ($1234,a3)    ; @BT 40 AB 12 34
        negx.b ($12,a1,d5)   ; @BT 40 31 50 12
        negx.l $1234.w       ; @BT 40 B8 12 34
        negx.w $12345678.l   ; @BT 40 79 12 34 56 78
        .db "<52"            ; @BT END

        .db "53>"           ; @BT TEST-53 53 START
        not.b d6            ; @BT 46 06
        not.w (a4)          ; @BT 46 54
        not.l -(SP)         ; @BT 46 A7
        not.l -(sp)         ; @BT 46 A7
        not.l -(a7)         ; @BT 46 A7
        not.b (a6)+         ; @BT 46 1E
        not.w $1234(a5)     ; @BT 46 6D 12 34
        not.l ($1234,a3)    ; @BT 46 AB 12 34
        not.b ($12,a1,d5)   ; @BT 46 31 50 12
        not.l $1234.w       ; @BT 46 B8 12 34
        not.w $12345678.l   ; @BT 46 79 12 34 56 78
        .db "<53"           ; @BT END

        .db "54>"              ; @BT TEST-54 54 START
        or.l d1,d2             ; @BT 84 81
        or.b (a1),d2           ; @BT 84 11
        or.w -(a1),d2          ; @BT 84 61
        or.l (a1)+,d2          ; @BT 84 99
        or.l $1234.w,d1        ; @BT 82 B8 12 34
        or.w $12345678.l,d1    ; @BT 82 79 12 34 56 78
        or.w $1234(a1),d2      ; @BT 84 69 12 34
        or.w ($1234,a1),d2     ; @BT 84 69 12 34
        or.b ($12,a2,d3),d4    ; @BT 88 32 30 12
        or.w ($34,a3,d4),d5    ; @BT 8A 73 40 34
hello5: or.w hello5(pc),d2     ; @BT 84 7A FF FE
        or.w (yello5,pc),d2    ; @BT 84 7A 00 02
yello5: or.b (mello5,PC,d2),d3 ; @BT 86 3B 20 02
mello5: or.b (cello5,PC,d2),d3 ; @BT 86 3B 20 04
        .db 0, 3               ; @BT 00 03
cello5: or.b (mello5,PC,d2),d3 ; @BT 86 3B 20 F8
        or.w frank5(pc),d2     ; @BT 84 7A 00 04
        .db 1, 2               ; @BT 01 02
frank5: .db "<54"              ; @BT END

        .db "55>"               ; @BT TEST-55 55 START
        or.l #$12345678.l,d1    ; @BT 00 81 12 34 56 78
        or.l #$12345678,d1      ; @BT 00 81 12 34 56 78
        or.w #$1234,d2          ; @BT 00 42 12 34
        or.b #$12,d4            ; @BT 00 04 00 12
        or.b #$fe, (a2)         ; @BT 00 12 00 FE
        or.w #$1234, -(a3)      ; @BT 00 63 12 34
        or.l #$12345678, (a4)+  ; @BT 00 9C 12 34 56 78
        or.b #$12, $3456.w      ; @BT 00 38 00 12 34 56
        or.w #$abcd,$12345678.l ; @BT 00 79 AB CD 12 34 56 78
        or.w #$abcd, $12345678  ; @BT 00 79 AB CD 12 34 56 78
        or.b #$12,($3456,a5)    ; @BT 00 2D 00 12 34 56
        or.b #$12,$3456(a5)     ; @BT 00 2D 00 12 34 56
        or.w #$1234,($56,a6,d7) ; @BT 00 76 12 34 70 56
        .db "<55"               ; @BT END

        .db "56>"                ; @BT TEST-56 56 START
        ori.l #$12345678.l,d1    ; @BT 00 81 12 34 56 78
        ori.l #$12345678,d1      ; @BT 00 81 12 34 56 78
        ori.w #$1234,d2          ; @BT 00 42 12 34
        ori.b #$12,d4            ; @BT 00 04 00 12
        ori.b #$fe, (a2)         ; @BT 00 12 00 FE
        ori.w #$1234, -(a3)      ; @BT 00 63 12 34
        ori.l #$12345678, (a4)+  ; @BT 00 9C 12 34 56 78
        ori.b #$12, $3456.w      ; @BT 00 38 00 12 34 56
        ori.w #$abcd,$12345678.l ; @BT 00 79 AB CD 12 34 56 78
        ori.w #$abcd, $12345678  ; @BT 00 79 AB CD 12 34 56 78
        ori.b #$12,($3456,a5)    ; @BT 00 2D 00 12 34 56
        ori.b #$12,$3456(a5)     ; @BT 00 2D 00 12 34 56
        ori.w #$1234,($56,a6,d7) ; @BT 00 76 12 34 70 56
        ori.b #10,CCR            ; @BT 00 3C 00 0A
        ori.b #$ff,CCR           ; @BT 00 3C 00 FF
        ori.w #$1234,SR          ; @BT 00 7C 12 34
        ori.w #$FFEE,SR          ; @BT 00 7C FF EE
        .db "<56"                ; @BT END

        .db "57>"            ; @BT TEST-57 57 START
        pea (a5)             ; @BT 48 55
        pea $1234(sp)        ; @BT 48 6F 12 34
        pea $1234(a7)        ; @BT 48 6F 12 34
        pea $1234(SP)        ; @BT 48 6F 12 34
        pea ($1234,a6)       ; @BT 48 6E 12 34
        pea ($12,a4,d3)      ; @BT 48 74 30 12
        pea label_ABCE.w     ; @BT 48 78 AB CE
        pea label_ABCE.l     ; @BT 48 79 00 00 AB CE
        pea end_57(pc)       ; @BT 48 7A 00 06
        pea (end_57,pc,d2)   ; @BT 48 7B 20 02
end_57: .db "<57"            ; @BT END

        .db "58>"         ; @BT TEST-58 58 START
        ror d4,d7         ; @BT E8 7F
        rol.b d1,d5       ; @BT E3 3D
        ror.w d4,d2       ; @BT E8 7A
        ror.l #8,d1       ; @BT E0 99
        rol.l #2,d1       ; @BT E5 99
        ror.w #5,d2       ; @BT EA 5A
        ror.w (a4)+       ; @BT E6 DC
        rol.w $3456.w     ; @BT E7 F8 34 56
        rol.w $12345678.l ; @BT E7 F9 12 34 56 78
        ror.w ($3456,a5)  ; @BT E6 ED 34 56
        rol.w $3456(a5)   ; @BT E7 ED 34 56
        ror.w ($56,a6,d7) ; @BT E6 F6 70 56
        ror (a2)          ; @BT E6 D2
        ror.w (a2)        ; @BT E6 D2
        rol.w -(a3)       ; @BT E7 E3
        .db "<58"         ; @BT END

        .db "59>"          ; @BT TEST-59 59 START
        roxr d4,d7         ; @BT E8 77
        roxl.b d1,d5       ; @BT E3 35
        roxr.w d4,d2       ; @BT E8 72
        roxr.l #8,d1       ; @BT E0 91
        roxl.l #2,d1       ; @BT E5 91
        roxr.w #5,d2       ; @BT EA 52
        roxr.w (a4)+       ; @BT E4 DC
        roxl.w $3456.w     ; @BT E5 F8 34 56
        roxl.w $12345678.l ; @BT E5 F9 12 34 56 78
        roxr.w ($3456,a5)  ; @BT E4 ED 34 56
        roxl.w $3456(a5)   ; @BT E5 ED 34 56
        roxr.w ($56,a6,d7) ; @BT E4 F6 70 56
        roxr (a2)          ; @BT E4 D2
        roxr.w (a2)        ; @BT E4 D2
        roxl.w -(a3)       ; @BT E5 E3
        .db "<59"          ; @BT END

        .db "60>"               ; @BT TEST-60 60 START
        sbcd d0, d0             ; @BT 81 00
        sbcd d1, d2             ; @BT 85 01
        sbcd -(a0), -(a0)       ; @BT 81 08
        sbcd -(a1), -(a2)       ; @BT 85 09
        sbcd.b d0, d0           ; @BT 81 00
        sbcd.b d1, d2           ; @BT 85 01
        sbcd.b -(a0), -(a0)     ; @BT 81 08
        sbcd.b -(a1), -(a2)     ; @BT 85 09
        .db "<60"               ; @BT END

        .db "61>"         ; @BT TEST-61 61 START
        scc d6            ; @BT 54 C6
        scs (a4)          ; @BT 55 D4
        seq -(SP)         ; @BT 57 E7
        seq -(a7)         ; @BT 57 E7
        seq -(sp)         ; @BT 57 E7
        sf (a6)+          ; @BT 51 DE
        sge $1234(a5)     ; @BT 5C ED 12 34
        sgt ($1234,a3)    ; @BT 5E EB 12 34
        shi ($12,a1,d5)   ; @BT 52 F1 50 12
        sle $1234.w       ; @BT 5F F8 12 34
        sls $12345678.l   ; @BT 53 F9 12 34 56 78
        slt label_ABCE    ; @BT 5D F9 00 00 AB CE
        smi d7            ; @BT 5B C7
        sne (a2)          ; @BT 56 D2
        spl -(a3)         ; @BT 5A E3
        st $1234(a4)      ; @BT 50 EC 12 34
        svc ($34,a2,d6)   ; @BT 58 F2 60 34
        svs label_ABCE.w  ; @BT 59 F8 AB CE
        .db "<61"         ; @BT END

        .db "62>"         ; @BT TEST-62 62 START
        stop #$1234       ; @BT 4E 72 12 34
        stop #$0102       ; @BT 4E 72 01 02
        .db "<62"         ; @BT END

        .db "63>"               ; @BT TEST-63 63 START
        sub.l d1,d2             ; @BT 94 81
        sub.l a1,d2             ; @BT 94 89
        sub.l (a1),d2           ; @BT 94 91
        sub.l -(a1),d2          ; @BT 94 A1
        sub.l (a1)+,d2          ; @BT 94 99
        sub.l $1234.w,d1        ; @BT 92 B8 12 34
        sub.l 305419896.l,d1    ; @BT 92 B9 12 34 56 78
        sub.w $1234(a1),d2      ; @BT 94 69 12 34
        sub.w ($1234,a1),d2     ; @BT 94 69 12 34
        sub.b ($12,a2,d3),d4    ; @BT 98 32 30 12
hello6: sub.w hello6(pc),d2     ; @BT 94 7A FF FE
        sub.w (yello6,pc),d2    ; @BT 94 7A 00 02
yello6: sub.b (mello6,PC,d2),d3 ; @BT 96 3B 20 02
mello6: sub.b (mello6,PC,d2),d3 ; @BT 96 3B 20 FE
        sub.w frank6(pc),d2     ; @BT 94 7A 00 04
        .db 1, 2                ; @BT 01 02
frank6: .db "<63"               ; @BT END

        .db "64>"               ; @BT TEST-64 64 START
        sub.b d2,(a1)           ; @BT 95 11
        sub.w d2,-(a1)          ; @BT 95 61
        sub.l d2,(a1)+          ; @BT 95 99
        sub.w d1,$1234.w        ; @BT 93 78 12 34
        sub.l d1,$12345678.l    ; @BT 93 B9 12 34 56 78
        sub.w d2,$1234(a1)      ; @BT 95 69 12 34
        sub.b d2,($1234,a1)     ; @BT 95 29 12 34
        sub.b d4,($12,a2,d3)    ; @BT 99 32 30 12
        .db "<64"               ; @BT END

        .db "66>"               ; @BT TEST-66 66 START
        sub.w d1, a2            ; @BT 94 C1
        sub.l a3, a4            ; @BT 99 CB
        sub.w (a5),a6           ; @BT 9C D5
        sub.w (a1)+,a2          ; @BT 94 D9
        sub.l -(a3),a4          ; @BT 99 E3
        sub.w $1234.w,a7        ; @BT 9E F8 12 34
        sub.w $1234.w,SP        ; @BT 9E F8 12 34
        sub.w $1234.w,sp        ; @BT 9E F8 12 34
        sub.l $12345678.l,a0    ; @BT 91 F9 12 34 56 78
        sub.l $12345678,a0      ; @BT 91 F9 12 34 56 78
        sub.l label_ABCE,a2     ; @BT 95 F9 00 00 AB CE
        sub.l label_ABCE+1,a2   ; @BT 95 F9 00 00 AB CF
        sub.w $1234(a1),a2      ; @BT 94 E9 12 34
        sub.l ($1234,a1),a3     ; @BT 97 E9 12 34
        sub.l ($12,a2,d3),a4    ; @BT 99 F2 30 12
hello7: sub.w hello7(pc),a2     ; @BT 94 FA FF FE
        sub.w (yello7,pc),a2    ; @BT 94 FA 00 02
yello7: sub.l (mello7,PC,d2),a3 ; @BT 97 FB 20 02
mello7: sub.l (cello7,PC,d2),a3 ; @BT 97 FB 20 04
        .db 0, 3                ; @BT 00 03
cello7: sub.l (mello7,PC,d2),a3 ; @BT 97 FB 20 F8
        sub.w frank7(pc),a6     ; @BT 9C FA 00 04
        .db 1, 2                ; @BT 01 02
frank7: .db "<66"               ; @BT END

        .db "67>"               ; @BT TEST-67 67 START
        sub.l #$12345678.l,d1   ; @BT 04 81 12 34 56 78
        sub.l #$12345678,d1     ; @BT 04 81 12 34 56 78
        sub.w #$1234,d2         ; @BT 04 42 12 34
        sub.b #$12,d4           ; @BT 04 04 00 12
        sub.b #$fe, (a2)        ; @BT 04 12 00 FE
        sub.w #$1234, -(a3)     ; @BT 04 63 12 34
        sub.l #$12345678, (a4)+ ; @BT 04 9C 12 34 56 78
        sub.b #$12, $3456.w     ; @BT 04 38 00 12 34 56
        sub.w #$abcd,$12345678.l ; @BT 04 79 AB CD 12 34 56 78
        sub.w #$abcd, $12345678 ; @BT 04 79 AB CD 12 34 56 78
        sub.b #$12,($3456,a5)   ; @BT 04 2D 00 12 34 56
        sub.b #$12,$3456(a5)    ; @BT 04 2D 00 12 34 56
        sub.w #$1234,($56,a6,d7) ; @BT 04 76 12 34 70 56
        sub.l #$12345678.l,a1   ; @BT 93 FC 12 34 56 78
        sub.l #$12345678,a2     ; @BT 95 FC 12 34 56 78
        sub.w #$1234,a3         ; @BT 96 FC 12 34
        .db "<67"               ; @BT END

        .db "68>"                ; @BT TEST-68 68 START
        suba.w d1, a2            ; @BT 94 C1
        suba.l a3, a4            ; @BT 99 CB
        suba.w (a5),a6           ; @BT 9C D5
        suba.w (a1)+,a2          ; @BT 94 D9
        suba.l -(a3),a4          ; @BT 99 E3
        suba.w $1234.w,a7        ; @BT 9E F8 12 34
        suba.w $1234.w,sp        ; @BT 9E F8 12 34
        suba.w $1234.w,SP        ; @BT 9E F8 12 34
        suba.l $12345678.l,a0    ; @BT 91 F9 12 34 56 78
        suba.l $12345678,a0      ; @BT 91 F9 12 34 56 78
        suba.l label_ABCE,a2     ; @BT 95 F9 00 00 AB CE
        suba.l label_ABCE+1,a2   ; @BT 95 F9 00 00 AB CF
        suba.w $1234(a1),a2      ; @BT 94 E9 12 34
        suba.l ($1234,a1),a3     ; @BT 97 E9 12 34
        suba.l ($12,a2,d3),a4    ; @BT 99 F2 30 12
hello8: suba.w hello8(pc),a2     ; @BT 94 FA FF FE
        suba.w (yello8,pc),a2    ; @BT 94 FA 00 02
yello8: suba.l (mello8,PC,d2),a3 ; @BT 97 FB 20 02
mello8: suba.l (cello8,PC,d2),a3 ; @BT 97 FB 20 04
        .db 0, 3                 ; @BT 00 03
cello8: suba.l (mello8,PC,d2),a3 ; @BT 97 FB 20 F8
        suba.w frank8(pc),a6     ; @BT 9C FA 00 04
        .db 1, 2                 ; @BT 01 02
frank8: .db "<68"                ; @BT END

        .db "69>"                 ; @BT TEST-69 69 START
        subi.l #$12345678.l,d1    ; @BT 04 81 12 34 56 78
        subi.l #$12345678,d1      ; @BT 04 81 12 34 56 78
        subi.w #$1234,d2          ; @BT 04 42 12 34
        subi.b #$12,d4            ; @BT 04 04 00 12
        subi.b #$fe, (a2)         ; @BT 04 12 00 FE
        subi.w #$1234, -(a3)      ; @BT 04 63 12 34
        subi.l #$12345678, (a4)+  ; @BT 04 9C 12 34 56 78
        subi.b #$12, $3456.w      ; @BT 04 38 00 12 34 56
        subi.w #$abcd,$12345678.l ; @BT 04 79 AB CD 12 34 56 78
        subi.w #$abcd, $12345678  ; @BT 04 79 AB CD 12 34 56 78
        subi.b #$12,($3456,a5)    ; @BT 04 2D 00 12 34 56
        subi.b #$12,$3456(a5)     ; @BT 04 2D 00 12 34 56
        subi.w #$1234,($56,a6,d7) ; @BT 04 76 12 34 70 56
        .db "<69"                 ; @BT END

        .db "70>"               ; @BT TEST-70 70 START
        sub.l #2,d1             ; @BT 55 81
        sub.w #3,d2             ; @BT 57 42
        sub.b #4,d4             ; @BT 59 04
        sub.b #5,(a2)           ; @BT 5B 12
        sub.w #6,-(a3)          ; @BT 5D 63
        sub.l #7,(a4)+          ; @BT 5F 9C
        sub.b #8,$3456.w        ; @BT 51 38 34 56
        sub.w #1,$12345678.l    ; @BT 53 79 12 34 56 78
        sub.w #2,$12345678      ; @BT 55 79 12 34 56 78
        sub.b #3,($3456,a5)     ; @BT 57 2D 34 56
        sub.b #4,$3456(a5)      ; @BT 59 2D 34 56
        sub.w #5,($56,a6,d7)    ; @BT 5B 76 70 56
        sub.l #6,a1             ; @BT 5D 89
        sub.l #7,a2             ; @BT 5F 8A
        sub.w #8,a3             ; @BT 51 4B
        .db "<70"               ; @BT END

        .db "71>"               ; @BT TEST-71 71 START
        subq.l #2,d1            ; @BT 55 81
        subq.w #3,d2            ; @BT 57 42
        subq.b #4,d4            ; @BT 59 04
        subq.b #5,(a2)          ; @BT 5B 12
        subq.w #6,-(a3)         ; @BT 5D 63
        subq.l #7,(a4)+         ; @BT 5F 9C
        subq.b #8,$3456.w       ; @BT 51 38 34 56
        subq.w #1,$12345678.l   ; @BT 53 79 12 34 56 78
        subq.w #2,$12345678     ; @BT 55 79 12 34 56 78
        subq.b #3,($3456,a5)    ; @BT 57 2D 34 56
        subq.b #4,$3456(a5)     ; @BT 59 2D 34 56
        subq.w #5,($56,a6,d7)   ; @BT 5B 76 70 56
        subq.l #6,a1            ; @BT 5D 89
        subq.l #7,a2            ; @BT 5F 8A
        subq.w #8,a3            ; @BT 51 4B
        .db "<71"               ; @BT END

        .db "72>"               ; @BT TEST-72 72 START
        subx.b d1, d2           ; @BT 95 01
        subx.w d3, d4           ; @BT 99 43
        subx.l d7, d5           ; @BT 9B 87
        subx.b -(a1), -(a2)     ; @BT 95 09
        subx.w -(a3), -(a4)     ; @BT 99 4B
        subx.l -(sp), -(a5)     ; @BT 9B 8F
        subx.l -(SP), -(a5)     ; @BT 9B 8F
        subx.l -(a7), -(a5)     ; @BT 9B 8F
        .db "<72"               ; @BT END

        .db "73>"  ; @BT TEST-73 73 START
        swap d3    ; @BT 48 43
        swap d7    ; @BT 48 47
        swap d0    ; @BT 48 40
        .db "<73"  ; @BT END

        .db "74>"         ; @BT TEST-74 74 START
        tas d6            ; @BT 4A C6
        tas (a4)          ; @BT 4A D4
        tas -(SP)         ; @BT 4A E7
        tas -(sp)         ; @BT 4A E7
        tas -(a7)         ; @BT 4A E7
        tas (a6)+         ; @BT 4A DE
        tas $1234(a5)     ; @BT 4A ED 12 34
        tas ($1234,a3)    ; @BT 4A EB 12 34
        tas ($12,a1,d5)   ; @BT 4A F1 50 12
        tas $1234.w       ; @BT 4A F8 12 34
        tas $12345678.l   ; @BT 4A F9 12 34 56 78
        .db "<74"         ; @BT END

        .db "75>"     ; @BT TEST-75 75 START
        trap #0       ; @BT 4E 40
        trap #7       ; @BT 4E 47
        trap #15      ; @BT 4E 4F
        .db "<75"     ; @BT END

        .db "76>"         ; @BT TEST-76 76 START
        tst d6            ; @BT 4A 46
        tst (a4)          ; @BT 4A 54
        tst -(a7)         ; @BT 4A 67
        tst -(sp)         ; @BT 4A 67
        tst -(SP)         ; @BT 4A 67
        tst (a6)+         ; @BT 4A 5E
        tst $1234(a5)     ; @BT 4A 6D 12 34
        tst ($1234,a3)    ; @BT 4A 6B 12 34
        tst ($12,a1,d5)   ; @BT 4A 71 50 12
        tst $1234.w       ; @BT 4A 78 12 34
        tst $12345678.l   ; @BT 4A 79 12 34 56 78
        .db "<76"         ; @BT END

        .db "77>"   ; @BT TEST-77 77 START
        unlk a0     ; @BT 4E 58
        unlk a7     ; @BT 4E 5F
        unlk SP     ; @BT 4E 5F
        unlk sp     ; @BT 4E 5F
        unlk a4     ; @BT 4E 5C
        .db "<77"   ; @BT END

        .db "78>"             ; @BT TEST-78 78 START
        move d3,CCR           ; @BT 44 C3
        move (a4),CCR         ; @BT 44 D4
        move -(a5),CCR        ; @BT 44 E5
        move (a6)+,CCR        ; @BT 44 DE
        move $1234(sp),CCR    ; @BT 44 EF 12 34
        move $1234(SP),CCR    ; @BT 44 EF 12 34
        move $1234(a7),CCR    ; @BT 44 EF 12 34
        move ($12,a1,d2),CCR  ; @BT 44 F1 20 12
        move $1234.w,CCR      ; @BT 44 F8 12 34
        move $12345678.l,CCR  ; @BT 44 F9 12 34 56 78
        move #$1234,CCR       ; @BT 44 FC 12 34
        move (ccr1,pc),ccr    ; @BT 44 FA 00 06
        move (ccr1,pc,d3),ccr ; @BT 44 FB 30 02
ccr1:   .db "<78"             ; @BT END

        .db "79>"            ; @BT TEST-79 79 START
        move d3,SR           ; @BT 46 C3
        move (a4),SR         ; @BT 46 D4
        move -(a5),SR        ; @BT 46 E5
        move (a6)+,SR        ; @BT 46 DE
        move $1234(SP),SR    ; @BT 46 EF 12 34
        move $1234(sp),SR    ; @BT 46 EF 12 34
        move $1234(a7),SR    ; @BT 46 EF 12 34
        move ($12,a1,d2),SR  ; @BT 46 F1 20 12
        move $1234.w,SR      ; @BT 46 F8 12 34
        move $12345678.l,SR  ; @BT 46 F9 12 34 56 78
        move #$1234,SR       ; @BT 46 FC 12 34
        move (ccr2,pc),sr    ; @BT 46 FA 00 06
        move (ccr2,pc,d3),sr ; @BT 46 FB 30 02
ccr2:   .db "<79"            ; @BT END

        .db "80>"            ; @BT TEST-80 80 START
        move SR,d3           ; @BT 40 C3
        move sr,(a4)         ; @BT 40 D4
        move sr,-(a5)        ; @BT 40 E5
        move sr,(a6)+        ; @BT 40 DE
        move sr,$1234(a7)    ; @BT 40 EF 12 34
        move sr,$1234(sp)    ; @BT 40 EF 12 34
        move sr,$1234(SP)    ; @BT 40 EF 12 34
        move sr,($12,a1,d2)  ; @BT 40 F1 20 12
        move sr,$1234.w      ; @BT 40 F8 12 34
        move sr,$12345678.l  ; @BT 40 F9 12 34 56 78
        .db "<80"            ; @BT END

        .db "81>"            ; @BT TEST-81 81 START
        move a3,usp          ; @BT 4E 63
        move a7,USP          ; @BT 4E 67
        move SP,USP          ; @BT 4E 67
        move sp,USP          ; @BT 4E 67
        move usp,a2          ; @BT 4E 6A
        move USP,a6          ; @BT 4E 6E
        .db "<81"            ; @BT END

        .db "82>"            ; @BT TEST-82 82 START
        moveq #-128,d3       ; @BT 76 80
        moveq.l #127,d7      ; @BT 7E 7F
        moveq #0,d6          ; @BT 7C 00
        moveq.l #1,d1        ; @BT 72 01
        move #-128,d3        ; @BT 76 80
        move.l #127,d7       ; @BT 7E 7F
        move #0,d6           ; @BT 7C 00
        move.l #1,d1         ; @BT 72 01
        .db "<82"            ; @BT END

        .db "83>"                 ; @BT TEST-83 83 START
        movea.w a1,a5             ; @BT 3A 49
        movea.l sp,a2             ; @BT 24 4F
        movea.l SP,a2             ; @BT 24 4F
        movea.l a7,a2             ; @BT 24 4F
        movea.w d3,a4             ; @BT 38 43
        movea.l (a1),a0           ; @BT 20 51
        movea.w -(a2),SP          ; @BT 3E 62
        movea.w -(a2),sp          ; @BT 3E 62
        movea.w -(a2),a7          ; @BT 3E 62
        movea.l (a3)+,a4          ; @BT 28 5B
        movea.w $1234(a5),a1      ; @BT 32 6D 12 34
        movea.l ($12,a6,d7),a2    ; @BT 24 76 70 12
        movea.w $1234.w,a3        ; @BT 36 78 12 34
        movea.w $12345678,a4      ; @BT 38 79 12 34 56 78
        movea.w #$1234,a5         ; @BT 3A 7C 12 34
        movea.l #$12345678,a6     ; @BT 2C 7C 12 34 56 78
        movea.w end_83(pc),a7     ; @BT 3E 7A 00 0E
        movea.w end_83(pc),sp     ; @BT 3E 7A 00 0A
        movea.w end_83(pc),SP     ; @BT 3E 7A 00 06
        movea.l (end_83,pc,d2),a1 ; @BT 22 7B 20 02
end_83: .db "<83"                 ; @BT END

        .db "84>"                  ; @BT TEST-84 84 START
        move.w a1,d5               ; @BT 3A 09
        move.w a2,$12345678        ; @BT 33 CA 12 34 56 78
        move.b d3,$1234(a4)        ; @BT 19 43 12 34
        move.l (a1),(a2)           ; @BT 24 91
        move.b -(a2),(a3)+         ; @BT 16 E2
        move.l (a3)+,-(a4)         ; @BT 29 1B
        move.w $1234(a5),d1        ; @BT 32 2D 12 34
        move.l ($12,a6,d7),$1234.w ; @BT 21 F6 70 12 12 34
        move.b $1234.w,d3          ; @BT 16 38 12 34
        move.w $12345678,(a4)      ; @BT 38 B9 12 34 56 78
        move.w #$1234,(a5)+        ; @BT 3A FC 12 34
        move.l #$12345678,-(a6)    ; @BT 2D 3C 12 34 56 78
        move.b end_84(pc),(a7)     ; @BT 1E BA 00 1C
        move.b end_84(pc),(SP)     ; @BT 1E BA 00 18
        move.b end_84(pc),(sp)     ; @BT 1E BA 00 14
        move.l (end_84,pc,d2),$1234(sp) ; @BT 2F 7B 20 10 12 34
        move.l (end_84,pc,d2),$1234(a7) ; @BT 2F 7B 20 0A 12 34
        move.l (end_84,pc,d2),$1234(SP) ; @BT 2F 7B 20 04 12 34
end_84: .db "<84"                  ; @BT END

        .db "85>"                ; @BT TEST-85 85 START
        move.w a1,a5             ; @BT 3A 49
        move.l SP,a2             ; @BT 24 4F
        move.l a7,a2             ; @BT 24 4F
        move.l sp,a2             ; @BT 24 4F
        move.w d3,a4             ; @BT 38 43
        move.l (a1),a0           ; @BT 20 51
        move.w -(a2),a7          ; @BT 3E 62
        move.w -(a2),sp          ; @BT 3E 62
        move.w -(a2),SP          ; @BT 3E 62
        move.l (a3)+,a4          ; @BT 28 5B
        move.w $1234(a5),a1      ; @BT 32 6D 12 34
        move.l ($12,a6,d7),a2    ; @BT 24 76 70 12
        move.w $1234.w,a3        ; @BT 36 78 12 34
        move.w $12345678,a4      ; @BT 38 79 12 34 56 78
        move.w #$1234,a5         ; @BT 3A 7C 12 34
        move.l #$12345678,a6     ; @BT 2C 7C 12 34 56 78
        move.w end_85(pc),a7     ; @BT 3E 7A 00 0E
        move.w end_85(pc),SP     ; @BT 3E 7A 00 0A
        move.w end_85(pc),sp     ; @BT 3E 7A 00 06
        move.l (end_85,pc,d2),a1 ; @BT 22 7B 20 02
end_85: .db "<85"                ; @BT END

        .db "86>"                ; @BT TEST-86 86 START
        movep d3,$1234(a5)       ; @BT 07 8D 12 34
        movep.w d7,($5678,a2)    ; @BT 0F 8A 56 78
        movep $1234(a7),d4       ; @BT 09 0F 12 34
        movep $1234(sp),d4       ; @BT 09 0F 12 34
        movep $1234(SP),d4       ; @BT 09 0F 12 34
        movep.w ($5678,a2),d1    ; @BT 03 0A 56 78
        movep.l d3,$1234(a5)     ; @BT 07 CD 12 34
        movep.l d7,($5678,a2)    ; @BT 0F CA 56 78
        movep.l $1234(a7),d4     ; @BT 09 4F 12 34
        movep.l $1234(SP),d4     ; @BT 09 4F 12 34
        movep.l $1234(sp),d4     ; @BT 09 4F 12 34
        movep.l ($5678,a2),d1    ; @BT 03 4A 56 78
        .db "<86"                ; @BT END

        .db "87>"                ; @BT TEST-87 87 START
        movem.w d0-d7/a0-sp,(a1) ; @BT 48 91 FF FF
        movem.w d0-d7/a0-SP,(a1) ; @BT 48 91 FF FF
        movem.w d0-d7/a0-a7,(a1) ; @BT 48 91 FF FF
        movem.l d0-d3/a4-SP,(a1) ; @BT 48 D1 F0 0F
        movem.l d0-d3/a4-sp,(a1) ; @BT 48 D1 F0 0F
        movem.l d0-d3/a4-a7,(a1) ; @BT 48 D1 F0 0F
        movem.w d1/d3/d6-d7/a0/a2/a4-a6,-(sp) ; @BT 48 A7 53 AE
        movem.w d1/d3/d6-d7/a0/a2/a4-a6,-(a7) ; @BT 48 A7 53 AE
        movem.w d1/d3/d6-d7/a0/a2/a4-a6,-(SP) ; @BT 48 A7 53 AE
        movem.w d0-d7/a0-SP,$1234(a3) ; @BT 48 AB FF FF 12 34
        movem.w d0-d7/a0-a7,$1234(a3) ; @BT 48 AB FF FF 12 34
        movem.w d0-d7/a0-sp,$1234(a3) ; @BT 48 AB FF FF 12 34
        movem.l d0-d3/a4-a7,($12,a3,d4) ; @BT 48 F3 F0 0F 40 12
        movem.l d0-d3/a4-sp,($12,a3,d4) ; @BT 48 F3 F0 0F 40 12
        movem.l d0-d3/a4-SP,($12,a3,d4) ; @BT 48 F3 F0 0F 40 12
        movem.w d1/d3/d6-d7/a0/a2/a4-a6,$1234.w ; @BT 48 B8 75 CA 12 34
        movem.w d0-d3/d5/a0-a6,$12345678 ; @BT 48 B9 7F 2F 12 34 56 78
        movem d0-d7/a0-a7,(a1) ; @BT 48 91 FF FF
        movem d0-d7/a0-SP,(a1) ; @BT 48 91 FF FF
        movem d0-d7/a0-sp,(a1) ; @BT 48 91 FF FF
        movem d1/d3/d6-d7/a0/a2/a4-a6,-(sp) ; @BT 48 A7 53 AE
        movem d1/d3/d6-d7/a0/a2/a4-a6,-(SP) ; @BT 48 A7 53 AE
        movem d1/d3/d6-d7/a0/a2/a4-a6,-(a7) ; @BT 48 A7 53 AE
        movem d0-d7/a0-SP,$1234(a3) ; @BT 48 AB FF FF 12 34
        movem d0-d7/a0-sp,$1234(a3) ; @BT 48 AB FF FF 12 34
        movem d0-d7/a0-a7,$1234(a3) ; @BT 48 AB FF FF 12 34
        movem d1/d3/d6-d7/a0/a2/a4-a6,$1234.w ; @BT 48 B8 75 CA 12 34
        movem d0-d3/d5/a0-a6,$12345678 ; @BT 48 B9 7F 2F 12 34 56 78
        .db "<87"                ; @BT END

        .db "88>"                ; @BT TEST-88 88 START
        movem.l (a3),a0/a2/a4-a6/d3-d5/d7 ; @BT 4C D3 75 B8
        movem.w (a4)+,a0-a5/d0-d4 ; @BT 4C 9C 3F 1F
        movem.l $5678(sp),d0-d7   ; @BT 4C EF 00 FF 56 78
        movem.l $5678(a7),d0-d7   ; @BT 4C EF 00 FF 56 78
        movem.l $5678(SP),d0-d7   ; @BT 4C EF 00 FF 56 78
        movem.l ($78,a1,d2),a0-a5/d0-d1 ; @BT 4C F1 3F 03 20 78
        movem.w $3456.w,a0/a2/a4/a6/d0/d2/d4/d6 ; @BT 4C B8 55 55 34 56
        movem.l $12345678,a0-a2/d0-d2 ; @BT 4C F9 07 07 12 34 56 78
        movem.w end_88(pc),a0/a5/d1 ; @BT 4C BA 21 02 00 08
        movem.l (end_88,pc,d0),d1-d4/a0-a2/d5/a4 ; @BT 4C FB 17 3E 00 02
end_88: .db "<88"                ; @BT END

        .db "89>"               ; @BT TEST-89 89 START
        movem.l (a3),a0/a2/a4-a6/d3-d5/d7 \ movep.l d7,($5678,a2) ; @BT 4C D3 75 B8 0F CA 56 78
        add.w d1,$1234.w \ add.w d1,$1234.w ; @BT D3 78 12 34 D3 78 12 34
        move.l ($12,a6,d7),$1234.w \ movep $1234(SP),d4 ; @BT 21 F6 70 12 12 34 09 0F 12 34
        move.l ($12,a6,d7),$1234.w \ movep $1234(a7),d4 ; @BT 21 F6 70 12 12 34 09 0F 12 34
        move.l ($12,a6,d7),$1234.w \ movep $1234(sp),d4 ; @BT 21 F6 70 12 12 34 09 0F 12 34
        movem.l (a3),a0/a2/a4-a6/d3-d5/d7 \ movep.l d7,($5678,a2) \ add.w d1,$1234.w \ add.w d1,$1234.w \ move.l ($12,a6,d7),$1234.w \ movep $1234(a7),d4 ; @BT 4C D3 75 B8 0F CA 56 78 D3 78 12 34 D3 78 12 34 21 F6 70 12 12 34 09 0F 12 34
        neg.b d6 \ neg.w (a4) \ neg.l -(a7) \ neg.b (a6)+ \ neg.w $1234(a5) \ neg.l ($1234,a3) \ neg.b ($12,a1,d5) \ neg.l $1234.w \ neg.w $12345678.l ; @BT  44 06 44 54 44 A7 44 1E 44 6D 12 34 44 AB 12 34 44 31 50 12 44 B8 12 34 44 79 12 34 56 78
        neg.b d6 \ neg.w (a4) \ neg.l -(sp) \ neg.b (a6)+ \ neg.w $1234(a5) \ neg.l ($1234,a3) \ neg.b ($12,a1,d5) \ neg.l $1234.w \ neg.w $12345678.l ; @BT  44 06 44 54 44 A7 44 1E 44 6D 12 34 44 AB 12 34 44 31 50 12 44 B8 12 34 44 79 12 34 56 78
        neg.b d6 \ neg.w (a4) \ neg.l -(SP) \ neg.b (a6)+ \ neg.w $1234(a5) \ neg.l ($1234,a3) \ neg.b ($12,a1,d5) \ neg.l $1234.w \ neg.w $12345678.l ; @BT  44 06 44 54 44 A7 44 1E 44 6D 12 34 44 AB 12 34 44 31 50 12 44 B8 12 34 44 79 12 34 56 78
        stop #$1234 \ stop #$0102 ; @BT 4E 72 12 34 4E 72 01 02
        .db "<89"               ; @BT END
        
        .org $ABCE
label_ABCE:

        // just testing that end-of-file in-macro replacements have enough space
        
        .macro X
        \1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1\1 nop
        .endm

        X "                                                       "
