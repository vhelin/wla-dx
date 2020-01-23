; C64 memory map:
;
.MEMORYMAP
        DEFAULTSLOT     2
        SLOTSIZE        $00FD
        SLOT 0          $0003   ; zero-page (variable space)
        SLOT 1          $0100   ; 6502-stack (variable space)
        SLOTSIZE        $FE00
        SLOT 2          $0200   ; main program RAM ("ROM")
.ENDME

.ROMBANKSIZE            $FE00
.ROMBANKS               1

.SLOT 2
.ORGA $0801

LoadAddress:
	lda $00
        beq LoadAddress

.RAMSECTION "bug" SLOT 0 RETURNORG
        bug BYTE
.ENDS

Loop:   lda $00
        beq Loop
