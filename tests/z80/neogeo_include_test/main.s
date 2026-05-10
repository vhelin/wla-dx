    .MEMORYMAP
        SLOTSIZE $8000
        DEFAULTSLOT 0
        SLOT 0 START $0000
    .ENDME

    .ROMBANKSIZE $8000
    .ROMBANKS 1

    .INCLUDE "../../../include/neogeo/z80.inc"

    .BANK 0 SLOT 0
    .ORG 0

    ; @BT linked.rom

    .db "01>"               ; @BT TEST_NEO_GEO_NMI 01 START
    NG_Z80_ENABLE_NMI        ; @BT AF D3 08
    NG_Z80_DISABLE_NMI       ; @BT AF D3 18
    .db "<01"               ; @BT END
