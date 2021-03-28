
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

        .DB "01>"
        .BITS 6 CABBAGE, %011110    ; CABBAGE == %110011
        .BITS 4 8+2                 ; 8 + 2 == %1010
        .BITS 4 %1011
        .ENDBITS                    ; writes the final byte in the bitstream
        .DB "<01"                   ; and resets the counters
        
        .DB "02>"
        .BITS 8 $12, $34, $56, $78
        .ENDBITS
        .DB "<02"

        .DB "03>"
        .BITS 4 %0101
        .BITS 32 %10101111000010010110000011111010
        .BITS 4 %0101
        .ENDBITS
        .DB "<03"
        
        .DB "04>"
        .BITS 6 DEFINE_110010 | 1, DEFINE_010010 | %001100
        .BITS 4 DEFINE_8 + DEFINE_2
        .BITS 4 DEFINE_8 + DEFINE_2 + DEFINE_1
        .ENDBITS
        .DB "<04"
        
        .DB "05>"
        .BITS 1 1, 0, 1, 0
        .BITS 3 2+DEFINE_7-1-1
        .BITS 1 1
        .ENDBITS
        .DB "<05"

        .DB "06>"
        .BITS 4 DEFINE_4 + 1
        .BITS 32 1 + DEFINE_AF0960FA - 1
        .BITS 4 DEFINE_7 - 2
        .ENDBITS
        .DB "<06"
        
        .DB "07>"
        .BITS 6 DEFINE_110010, DEFINE_010010
        .BITS 4 DEFINE_8
        .BITS 4 DEFINE_2
        .ENDBITS
        .DB "<07"
        
        .DB "08>"
        .BITS 1 1, 0, 1, 0
        .BITS 3 DEFINE_7
        .BITS 1 1
        .ENDBITS
        .DB "<08"

        .DB "09>"
        .BITS 4 DEFINE_4
        .BITS 32 DEFINE_AF0960FA
        .BITS 4 DEFINE_7
        .ENDBITS
        .DB "<09"

        .DB "10>"
        .BITS 6 HERE_DEFINE_110010, HERE_DEFINE_010010
        .BITS 4 HERE_DEFINE_8
        .BITS 4 HERE_DEFINE_2
        .ENDBITS        
        .DB "<10"

        .DEFINE HERE_DEFINE_110010 = 0b110010
        .DEFINE HERE_DEFINE_010010 = 0b010010
        .DEFINE HERE_DEFINE_8 = 8
        .DEFINE HERE_DEFINE_2 = 2
        
        .DB "11>"
        .BITS 4 %1010
        .BITS 7 label_0x7f
        .BITS 5 %00100
        .BITS 3 %111
        .BITS 9 label_0xff + 1
        .BITS 4 %1010
        .BITS 4 DEFINE_8
        .BITS 13 label_0x1fff
        .BITS 7 DEFINE_110010
        .ENDBITS
        .DB "<11"

        .ORG $7F
label_0x7f:
        .ORG $FF
label_0xff:
        .ORG $1FFF
label_0x1fff:
        
        .BANK 1 SLOT 0
        .ORG 0

        .SECTION "BANKHEADER"

        .DB "12>"
        .DW label_0x7f
        .BITS 6 CABBAGE, %011110    ; CABBAGE == %110011
        .BITS 4 8+2                 ; 8 + 2 == %1010
        .BITS 4 %1011
        .BITS 1 1
        .BITS 3 DEFINE_7
        .BITS 3 DEFINE_4
        .BITS 1 1
        .ENDBITS
        .DB "<12"

        .DB "13>"
        .BITS 4 DEFINE_4
        .BITS 32 DEFINE_AF0960FA
        .BITS 4 DEFINE_7
        .ENDBITS        
        .DB "<13"

        .DB "14>"
        .BITS 6 DEFINE_110010 | 1, DEFINE_010010 | %001100
        .BITS 4 DEFINE_8 + DEFINE_2
        .BITS 4 DEFINE_8 + DEFINE_2 + DEFINE_1
        .BITS 4 HERE_DEFINE_2 | %1000, %1110 & %0110
        .ENDBITS
        .DB "<14"
        
        .ENDS
