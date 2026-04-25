        .MEMORYMAP
          SLOTSIZE $8000
          DEFAULTSLOT 0
          SLOT 0 START $0000
        .ENDME

        .ROMBANKSIZE $8000
        .ROMBANKS 1

        ; @BT linked.rom

        .BANK 0 SLOT 0
        .ORG $0020

        .SECTION "Cx4 linker flow" FORCE

Entry:
        ; Real-world-style control flow: page byte and low PC byte come from
        ; external routine labels defined in another object file.
        .DB "01>"               ; @BT TEST-01 01 START
        MOV P,>MathInit          ; @BT 01 67
        JSR.F <MathInit          ; @BT 34 2A
        MOV P,>RenderLoop        ; @BT 02 67
        JMP.F <RenderLoop        ; @BT C0 0A
        JSR <LocalHelper         ; @BT 2F 28
        BRA <Done                ; @BT 35 08

LocalHelper:
        MOV PL,>SoundPatch       ; @BT 03 7C
        LD P,>OverlayTask        ; @BT 04 67
        RTS                      ; @BT 00 3C

Done:
        NOP                      ; @BT 00 00
        .DB "<01"               ; @BT END

        .ENDS