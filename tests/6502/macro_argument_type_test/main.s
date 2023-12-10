        .MEMORYMAP
          DEFAULTSLOT 0
          SLOTSIZE $1000
          SLOT 0 $1000
        .ENDME

        .ROMBANKSIZE $1000
        .ROMBANKS 1

        .BANK 0
        .ORG 0

        ; @BT linked.rom

        .macro LOAD_DATA
          .if \?1 == ARG_IMMEDIATE
            lda #\1
          .else
            lda \1
          .endif
        .endm

        .macro COPY_DATA
          LOAD_DATA(\1)
          sta \2
        .endm
        
        .db "01>"            ; @BT TEST-01 01 START
        LOAD_DATA($aa)       ; @BT A5 AA
        LOAD_DATA(#$aa)      ; @BT A9 AA
        COPY_DATA($55, $00)  ; @BT A5 55 85 00
        COPY_DATA(#$55, $00) ; @BT A9 55 85 00
        LOAD_DATA $aa        ; @BT A5 AA
        LOAD_DATA #$aa       ; @BT A9 AA
        COPY_DATA $55, $00   ; @BT A5 55 85 00
        COPY_DATA #$55, $00  ; @BT A9 55 85 00
        .db "<01"            ; @BT END

        .macro LOAD1
          .if \?1 == ARG_IMMEDIATE
            .db $01 \1
          .else
            .db $00 \1
          .endif
        .endm

        .macro LOAD2
          .if \?1 == ARG_IMMEDIATE
            LOAD1 \1
          .else
            .db $02 \1
          .endif
        .endm

        .macro LOAD3
          .if \?1 == ARG_IMMEDIATE
            LOAD2 \1
          .else
            .db $03 \1
          .endif
        .endm
        
        .db "02>"               ; @BT TEST-02 02 START
        LOAD3 #$0A              ; @BT 01 0A
        LOAD3 $0A               ; @BT 03 0A
        .db "<02"               ; @BT END

        .macro NEW1
          .if \?1 == ARG_IMMEDIATE
            .db $01 \1
          .else
            .db $00 \1
          .endif
        .endm

        .macro NEW2
          .if \?1 == ARG_IMMEDIATE
            NEW1 \1
          .else
            .db $02 \1
          .endif
        .endm

        .macro NEW3
          .if \?1 != ARG_IMMEDIATE
            NEW2 \1
          .else
            .db $03 \1
          .endif
        .endm
        
        .db "03>"               ; @BT TEST-03 03 START
        NEW3 #$0A               ; @BT 03 0A
        NEW3 $0A                ; @BT 02 0A
        .db "<03"               ; @BT END
        
