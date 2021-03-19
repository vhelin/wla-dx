
        .MEMORYMAP
           DEFAULTSLOT 0
           SLOTSIZE $2000
           SLOT 0 $0000
        .ENDME

        .ROMBANKMAP
           BANKSTOTAL 1
           BANKSIZE $2000
           BANKS 1
        .ENDRO

        .BANK 0 SLOT 0
        .ORG 0
        
        .SECTION "Section01" FORCE
        .DB "01>"
        jmp +
--      beq +
        nop
-       beq +
        nop
+       beq _f
        nop
        nop
__      nop
        beq _b
        beq --
        beq -
        jmp -
        beq _b + 1
        .DB "<01"
        .ENDS

        .SECTION "Section02" FORCE
        .DB "02>"
label_01:
@child: nop
        beq __label__
label_02:
        jmp __label__ + 1
        .DB "<02"
        .ENDS
        
