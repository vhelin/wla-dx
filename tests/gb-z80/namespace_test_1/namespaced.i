
      .ramsection "namespacedRAM" bank 0 slot 2
      namespacedRAMVariable1:  db
      namespacedRAMVariable2:  db
      .ends

      .macro callMeSub
      .db $22
      .endm
      
      .macro callMe
      callMeSub
      .db $11
      .endm
      
      .org $400

; @BT linked.gb

namespacedLabel:
      .db "01>"                              ; @BT TEST-01 01 START
      
      .dw namespacedLabel+1                  ; @BT 01 04
      .dw namespacedRAMVariable1+1           ; @BT 01 A0

      ld [namespacedLabel],A                 ; @BT EA 00 04
      ld [namespacedLabel+1],A               ; @BT EA 01 04
        
      ld [namespacedRAMVariable1],A          ; @BT EA 00 A0
      ld [namespacedRAMVariable1+1],A        ; @BT EA 01 A0

      callMe                                 ; @BT 22 11
      
      .db "<01"                              ; @BT END
      
