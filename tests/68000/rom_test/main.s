
;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»
; a small test for wla-68000
;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»

        .MEMORYMAP
          DEFAULTSLOT 0
          ; ROM area
          SLOTSIZE    $400000
          SLOT        0       $000000
          ; RAM area
          SLOTSIZE    $10000
          SLOT        1       $FF0000
        .ENDME

        .ROMBANKMAP
          BANKSTOTAL 1
          BANKSIZE $400000
          BANKS 1
        .ENDRO

        .EMPTYFILL $AA

        .SMDHEADER
          SYSTEMTYPE "SEGA MEGA DRIVE "    ; 16 bytes
          COPYRIGHT  "                "    ; 16 bytes
          TITLEDOMESTIC "             "    ; 48 bytes (all spaces)
          TITLEOVERSEAS "             "    ; 48 bytes (all spaces)
          SERIALNUMBER  "             "    ; 14 bytes (all spaces)
          DEVICESUPPORT "J            "    ; 16 bytes (all spaces)
          ROMADDRESSRANGE $0, -1           ;  8 bytes (-1 is turned into ROM size minus one)
          RAMADDRESSRANGE $FF0000, $FFFFFF ;  8 bytes
          EXTRAMEMORY "RA", $A0, $20, 0, 0 ; 12 bytes (S and E and start and end, both 0)
          MODEMSUPPORT "            "      ; 12 bytes (all spaces)
          REGIONSUPPORT "JUE"              ;  3 bytes
        .ENDSMD

        .BANK 0 SLOT 0
        .ORG 0

        ; @BT linked.rom

        // TODO: add real tests
        
        .db "01>"               ; @BT TEST-01 01 START
        .db 0                   ; @BT 00
        .db "<01"               ; @BT END
        
