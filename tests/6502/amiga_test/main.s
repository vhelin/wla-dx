
        .MEMORYMAP
           DEFAULTSLOT 0
           SLOTSIZE $2000
           SLOT 0 $8000
        .ENDME

        .ROMBANKMAP
           BANKSTOTAL 2
           BANKSIZE $2000
           BANKS 2
        .ENDRO

        .BANK 1 SLOT 0
        .ORG 0

        // here we test things that have caused problems on Amiga before
        // (hopefully not any more)
        
        .db "01>"
label_01:
        .db <$1234, >$5678, :label_01, bank(label_01)
        .db "<01"

        .db "02>"
        .dw loword($12345678), hiword($9abcdef0)
        .db "<02"
        
