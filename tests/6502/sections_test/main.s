        .MEMORYMAP
        DEFAULTSLOT 0
        SLOTSIZE $1000
        SLOT 0 $8000
        .ENDME

        .ROMBANKMAP
        BANKSTOTAL 2
        BANKSIZE $1000
        BANKS 2
        .ENDRO

        .EMPTYFILL $00

        ; @BT linked.rom

        .BANK 1 SLOT 0
        .ORG $00

        .SECTION "Refs" FORCE
        .db "01>"               ; @BT TEST-01 01 START
        .DW force_label         ; @BT 10 80
        .DW overwrite_label     ; @BT 11 80
        .DW semisuper_label     ; @BT 12 80
        .DW semisub_label       ; @BT 00 80
        .DW semifree_label      ; @BT 02 80
        .DW free_label          ; @BT 04 80
        .DW superfree_label     ; @BT 06 80
        .db "<01"               ; @BT END
        .ENDS

        .BANK 0 SLOT 0
        .ORG $10

        .SECTION "ForceSection" FORCE
force_label:
        .DB $F1, $F2
        .ENDS

        .ORG $11

        .SECTION "OverwriteSection" OVERWRITE
overwrite_label:
        .DB $F3, $F4, 2
        .ENDS

        .ORG $30

        .SECTION "SemiSuperSection" SEMISUPERFREE BANKS 0
semisuper_label:
        .rept $10
        .DB $A1, $A2
        .endr
        .ENDS

        .ORG $50

        .SECTION "SuperFreeSection" SUPERFREE
superfree_label:
        .DB $E1, $E2
        .ENDS

        .ORG $50

        .SECTION "SemiSubSection" SEMISUBFREE
semisub_label:
        .DB $B1, $B2
        .ENDS

        .ORG $40

        .SECTION "FreeSection" FREE
free_label:
        .DB $D1, $D2
        .ENDS

        .ORG $0

        .SECTION "SemiFreeSection" SEMIFREE
semifree_label:
        .DB $C1, $C2
        .ENDS

        .ORG $00

        .SECTION "BANKHEADER"
        .DB $B0, $B1
        .ENDS
