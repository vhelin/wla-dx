
        .EMPTYFILL 0
        
        .MEMORYMAP
           DEFAULTSLOT 0
           SLOTSIZE $2000
           SLOT 0 $0000
           SLOT 1 $A000
           SLOT 2 $E000
        .ENDME

        .ROMBANKMAP
           BANKSTOTAL 16
           BANKSIZE $2000
           BANKS 16
        .ENDRO

        .BANK 0 SLOT 0
        .ORG 0

        
        .MACRO DEEP2
        .DB $aa, $bb
        .ENDM

        .MACRO DEEP1
        .DB 0, 1
        DEEP2
        .DB 2, 3
        .ENDM

        .MACRO HELLO
        .DB 9, 8
        .DB 7, 6
        .ENDM
        
        .SECTION "SectionA" FREE
label_A:.DB 2, 3
        .DB 4, 5
        .ENDS

        .SECTION "SectionX" FREE
        HELLO
        HELLO
        DEEP1

        .REPEAT 4 INDEX COUNT
        .DB COUNT
        .ENDR
        
        .DB 1, 2
        .DB 3, 4
        .ENDS

        .INCLUDE "main_1.inc"
        .INCLUDE "main_2.inc"

        .SECTION "SectionY" FREE
        .DB $0a, $0b
        .ENDS

        .SECTION "SectionZ" FREE
        MAIN_1
        .ENDS


