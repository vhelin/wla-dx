
        .MEMORYMAP
           DEFAULTSLOT 0
           SLOTSIZE $2000
           SLOT 0 $0000
        .ENDME

        .ROMBANKMAP
           BANKSTOTAL 2
           BANKSIZE $2000
           BANKS 2
        .ENDRO

        .BANK 0 SLOT 0
        .ORG 0

        .DEFINE CABBAGE = %110011

        ; @BT linked.rom

        .DB "01>"                       ; @BT TEST-01 01 START
        ; CABBAGE == %110011
        .BITS 6 CABBAGE, %011110        ; @BT CD
        ; 8 + 2 == %1010
        .BITS 4 8+2                     ; @BT EA
        .BITS 4 %1011
        ; writes the final byte in the bitstream and resets the counters
        .ENDBITS                        ; @BT B0
        .DB "<01"                       ; @BT END

        .DB "02>"                       ; @BT TEST-02 02 START
        .BITS 8 $12, $34, $56, $78      ; @BT 12 34 56 78
        .ENDBITS
        .DB "<02"                       ; @BT END

        .DB "03>"                                       ; @BT TEST-03 03 START
        .BITS 4 %0101
        .BITS 32 %10101111000010010110000011111010      ; @BT 5A F0 96 0F
        .BITS 4 %0101                                   ; @BT A5
        .ENDBITS
        .DB "<03"                                       ; @BT END
        
        .DB "04>"                                               ; @BT TEST-04 04 START
        .BITS 6 DEFINE_110010 | 1, DEFINE_010010 | %001100      ; @BT CD
        .BITS 4 DEFINE_8 + DEFINE_2                             ; @BT EA
        .BITS 4 DEFINE_8 + DEFINE_2 + DEFINE_1
        .ENDBITS                                                ; @BT B0
        .DB "<04"                                               ; @BT END
        
        .DB "05>"               ; @BT TEST-05 05 START
        .BITS 1 1, 0, 1, 0
        .BITS 3 2+DEFINE_7-1-1
        .BITS 1 1               ; @BT AF
        .ENDBITS
        .DB "<05"               ; @BT END

        .DB "06>"                               ; @BT TEST-06 06 START
        .BITS 4 DEFINE_4 + 1
        .BITS 32 1 + DEFINE_AF0960FA - 1        ; @BT 5A F0 96 0F
        .BITS 4 DEFINE_7 - 2                    ; @BT A5
        .ENDBITS
        .DB "<06"                               ; @BT END

        .DB "07>"                               ; @BT TEST-07 07 START
        .BITS 6 DEFINE_110010, DEFINE_010010    ; @BT C9
        .BITS 4 DEFINE_8                        ; @BT 28
        .BITS 4 DEFINE_2
        .ENDBITS                                ; @BT 20
        .DB "<07"                               ; @BT END

        .DB "08>"               ; @BT TEST-08 08 START
        .BITS 1 1, 0, 1, 0
        .BITS 3 DEFINE_7
        .BITS 1 1               ; @BT AF
        .ENDBITS
        .DB "<08"               ; @BT END

        .DB "09>"                       ; @BT TEST-09 09 START
        .BITS 4 DEFINE_4
        .BITS 32 DEFINE_AF0960FA        ; @BT 4A F0 96 0F
        .BITS 4 DEFINE_7                ; @BT A7
        .ENDBITS                        ; @BT END
        .DB "<09"

        .DB "10>"                                       ; @BT TEST-10 10 START
        .BITS 6 HERE_DEFINE_110010, HERE_DEFINE_010010  ; @BT C9
        .BITS 4 HERE_DEFINE_8                           ; @BT 28
        .BITS 4 HERE_DEFINE_2
        .ENDBITS                                        ; @BT 20
        .DB "<10"                                       ; @BT END

        .DEFINE HERE_DEFINE_110010 = 0b110010
        .DEFINE HERE_DEFINE_010010 = 0b010010
        .DEFINE HERE_DEFINE_8 = 8
        .DEFINE HERE_DEFINE_2 = 2

        .DB "11>"               ; @BT TEST-11 11 START
        .BITS 4 %1010
        .BITS 7 label_0x7f      ; @BT AF
        .BITS 5 %00100          ; @BT E4
        .BITS 3 %111
        .BITS 9 label_0xff + 1  ; @BT F0
        .BITS 4 %1010           ; @BT 0A
        .BITS 4 DEFINE_8
        .BITS 13 label_0x1fff   ; @BT 8F FF
        .BITS 7 DEFINE_110010   ; @BT B2
        .ENDBITS
        .DB "<11"               ; @BT END

        .ORG $7F
label_0x7f:
        .ORG $FF
label_0xff:
        .ORG $1FFF
label_0x1fff:
        
        .BANK 1 SLOT 0
        .ORG 0

        .SECTION "BANKHEADER"

        .DB "12>"                       ; @BT TEST-12 12 START
        .DW label_0x7f                  ; @BT 7F 00
        ; CABBAGE == %110011
        .BITS 6 CABBAGE, %011110        ; @BT CD
        ; 8 + 2 == %1010
        .BITS 4 8+2                     ; @BT EA
        .BITS 4 %1011
        .BITS 1 1
        .BITS 3 DEFINE_7                ; @BT BF
        .BITS 3 DEFINE_4
        .BITS 1 1
        .ENDBITS                        ; @BT 90
        .DB "<12"                       ; @BT END

        .DB "13>"                       ; @BT TEST-13 13 START
        .BITS 4 DEFINE_4
        .BITS 32 DEFINE_AF0960FA        ; @BT 4A F0 96 0F
        .BITS 4 DEFINE_7                ; @BT A7
        .ENDBITS
        .DB "<13"                       ; @BT END

        .DB "14>"                                               ; @BT TEST-14 14 START
        .BITS 6 DEFINE_110010 | 1, DEFINE_010010 | %001100      ; @BT CD
        .BITS 4 DEFINE_8 + DEFINE_2                             ; @BT EA
        .BITS 4 DEFINE_8 + DEFINE_2 + DEFINE_1
        .BITS 4 HERE_DEFINE_2 | %1000, %1110 & %0110            ; @BT BA
        .ENDBITS                                                ; @BT 60
        .DB "<14"                                               ; @BT END
        
        .ENDS
