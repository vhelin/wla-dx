
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
          DEVICESUPPORT "J            "    ; 16 bytes ('J' and the rest are spaces)
          ROMADDRESSRANGE $0, -1           ;  8 bytes (-1 is turned into ROM size minus one)
          RAMADDRESSRANGE $FF0000, $FFFFFF ;  8 bytes
          EXTRAMEMORY "RA", $A0, $20, 0, 0 ; 12 bytes (S and E and start and end, both 0)
          MODEMSUPPORT "            "      ; 12 bytes (all spaces)
          REGIONSUPPORT "JUE"              ;  3 bytes
        .ENDSMD

        .BANK 0 SLOT 0
        .ORG 0

        ; @BT linked.rom

        .db "01>"               ; @BT TEST-01 01 START
        // 3
        .align 8                ; @BT 05 06 07 AA AA
// 05, 06 and 07 come from .SECTION "Tiny"
        // 8
        .db 0                   ; @BT 00
        // 9
        .align 2                ; @BT AA
        // 10
        .db 0, 1, 2, 3, 4, 5    ; @BT 00 01 02 03 04 05
        // 16
        .align 4
        // 16
        .db 4, 5, 6, 7          ; @BT 04 05 06 07
        // 20
        
        .section "ForceSection" FORCE
        .align 16               ; @BT AA AA AA AA AA AA AA AA AA AA AA AA
        // 32
        .db 0, 1, 2             ; @BT 00 01 02
        // 35
        .ends

        .db "<01"               ; @BT END

        .section "Align4-A" ALIGN 8 RETURNORG
        .db "02>"               ; @BT TEST-02 02 START
        // 3
        .align 4                ; @BT AA
        // 4
        .db 0                   ; @BT 00
        // 5
        .align 4                ; @BT AA AA AA
        // 8
        .db 1, 2                ; @BT 01 02
        // 10
        .align 4                ; @BT AA AA
        // 12
        .db 3                   ; @BT 03
        // 13
        .align 2                ; @BT AA
        // 14
        .db 4, 5                ; @BT 04 05
        .db "<02"               ; @BT END
        .ends

        .section "Align4-B" ALIGN 4
        .db "03>"               ; @BT TEST-03 03 START
        // 3
        .align 4                ; @BT AA
        // 4
        .db 0                   ; @BT 00
        // 5
        .align 4                ; @BT AA AA AA
        // 8
        .db 1                   ; @BT 01
        // 9
        .align 2                ; @BT AA
        // 10
        .db 2                   ; @BT 02
        // 11
        .db "<03"               ; @BT END
        .ends

        .ORG $201

        .db "04>"               ; @BT TEST-04 04 START
        // 4
        .align 8                ; @BT AA AA AA AA
        // 8
        .db 0                   ; @BT 00
        // 9
        .align 2                ; @BT AA
        // 10
        .db 0, 1, 2, 3, 4, 5    ; @BT 00 01 02 03 04 05
        // 16
        .align 4
        // 16
        .db 4, 5, 6, 7          ; @BT 04 05 06 07
        // 20
        .db "<04"               ; @BT END

        .section "Tiny" FREE
        .db 5, 6, 7
        .ends

        .section "Big1" FREE ALIGN 16
        .db "05>"               ; @BT TEST-05 05 START
        .db CADDR & 15          ; @BT 03
        .align 8                ; @BT AA AA AA AA
        .db CADDR & 15          ; @BT 08
        .db "<05"               ; @BT END
        .ends

        .section "Big2" FREE ALIGN 16
        .db "06>"               ; @BT TEST-06 06 START
        .db CADDR & 15          ; @BT 03
        .align 8                ; @BT AA AA AA AA
        .db CADDR & 15          ; @BT 08
        .db "<06"               ; @BT END
        .ends
        
