
        .MEMORYMAP
          DEFAULTSLOT 1
          SLOT 0 $0000 $4000
          SLOT 1 STArT $4000 sIzE $4000
        .ENDME

        .ROMBANKMAP
          BANKSTOTAL 8
          BANKSIZE $4000
          BANKS 8
        .ENDRO

        .EMPTYFILL $ff
        
        .BANK 0 SLOT 0
        .ORGA 0

        ; @BT linked.gb
        
        .GBHEADER
          NAME "TANKBOMBPANIC"  ; identical to a freestanding .NAME.
          LICENSEECODEOLD $34   ; identical to a freestanding .LICENSEECODEOLD.
          CARTRIDGETYPE $00     ; identical to a freestanding .CARTRIDGETYPE.
          RAMSIZE $09           ; identical to a freestanding .RAMSIZE.
          ROMSIZE               ; identical to a freestanding .ROMSIZE.
          COUNTRYCODE $01       ; identical to a freestanding .COUNTRYCODE/DESTINATIONCODE.
          DESTINATIONCODE $01   ; identical to a freestanding .DESTINATIONCODE/COUNTRYCODE.
          NINTENDOLOGO          ; identical to a freestanding .NINTENDOLOGO.
          VERSION $02           ; identical to a freestanding .VERSION.
          ROMDMG                ; identical to a freestanding .ROMDMG.
                                ; Alternatively, ROMGBC or ROMGBCONLY can be used
        .ENDGB

        .ORG $0147-3
        .DB "01>"               ; @BT TEST-01 01 START
        //.DB $00               ; @BT 00
        //.DB $03               ; @BT 02
        //.DB $09               ; @BT 09
        //.DB $01               ; @BT 01
        //.DB $34               ; @BT 34
        //,DB $02               ; @BT 02
        .ORG $014D
        .DB $4D, $CC, $19       ; @BT 4D CC 19
        .DB "<01"               ; @BT END
        
