        .memorymap
          slotsize $8000
          defaultslot 0
          slot 0 $0000
        .endme

        .rombanksize $8000
        .rombanks 1

        .bank 0 slot 0

        ; @BT result.rom

        .org $0020
start:
        ; Cx4 jump/call operands are page-local low 8-bit targets.
        .db "01>"               ; @BT TEST-01 01 START
        jmp <target              ; @BT 34 08
        jmp.f <target            ; @BT 34 0A
        jsr <target              ; @BT 34 28
        jsr.f <target            ; @BT 34 2A
        .db "<01"               ; @BT END

        .org $0134
target:
        nop
