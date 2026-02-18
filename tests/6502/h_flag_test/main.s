
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

        .section "ROM Bank $000" bank 0 slot 0 keep
          .db "01>"             ; @BT TEST-01 01 START
          lda #MyConst + 2      ; @BT A9 04
          lda #MyConst          ; @BT A9 02
          lda (10, X)           ; @BT A1 0A
          lda (10), Y           ; @BT B1 0A
          lda 10, X             ; @BT B5 0A
          lda 256, X            ; @BT BD 00 01
          lda 256, Y            ; @BT B9 00 01
          lda 11                ; @BT A5 0B
          lda 256               ; @BT AD 00 01
          lda.b #MyConst + 2    ; @BT A9 04
          lda.b #MyConst        ; @BT A9 02
          lda.b (10, X)         ; @BT A1 0A
          lda.b (10), Y         ; @BT B1 0A
          lda.b 10, X           ; @BT B5 0A
          lda.w 256, X          ; @BT BD 00 01
          lda.w 256, Y          ; @BT B9 00 01
          lda.b 11              ; @BT A5 0B
          lda.w 256             ; @BT AD 00 01
          .db "<01"             ; @BT END
        .ends

        MyConst2 = $0200

        .section "ROM Bank $000 - II" bank 0 slot 0 keep
          .db "02>"             ; @BT TEST-02 02 START
          ldx MyConst2 + $1     ; @BT AE 01 02
          ldx MyConst2 + $1     ; @BT AE 01 02
          sta MyConst2 + $1     ; @BT 8D 01 02
          sta MyConst2 + $1     ; @BT 8D 01 02
          sty MyConst2 + $1     ; @BT 8C 01 02
          sty MyConst2 + $1     ; @BT 8C 01 02
          stx MyConst2 + $1     ; @BT 8E 01 02
          stx MyConst2 + $1     ; @BT 8E 01 02
          stx 10,Y              ; @BT 96 0A
          stx 10                ; @BT 86 0A
          stx $1234             ; @BT 8E 34 12
          stx.b 10,Y            ; @BT 96 0A
          stx.b 10              ; @BT 86 0A
          stx.w $1234           ; @BT 8E 34 12
          sty 10,X              ; @BT 94 0A
          sty 10                ; @BT 84 0A
          sty $1234             ; @BT 8C 34 12
          sty.b 10,X            ; @BT 94 0A
          sty.b 10              ; @BT 84 0A
          sty.w $1234           ; @BT 8C 34 12
          .db "<02"             ; @BT END
        .ends
        
