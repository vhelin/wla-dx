
        .MEMORYMAP
           DEFAULTSLOT 0
           SLOTSIZE $2000
           SLOT 0 $8000
        .ENDME

        .ROMBANKMAP
           BANKSTOTAL 1
           BANKSIZE $2000
           BANKS 1
        .ENDRO

        .BANK 0 SLOT 0
        .ORG 0

        ; @BT linked.rom

        MyConst = 2

        .section "ROM Bank $000" bank 0 slot 0
          .db "01>"             ; @BT TEST-01 01 START
          lda #MyConst + 2      ; @BT A9 04
          lda #MyConst          ; @BT A9 02
          .db "<01"             ; @BT END
        .ends
        
