
        ///////////////////////////////////////////////////////////////////////
        // Testing .INCBIN with FILTER and FILTERSIZE
        ///////////////////////////////////////////////////////////////////////
        
        .gbheader
          version $0f
          version 15
          countrycode 1
          ramsize 2
          cartridgetype 7
          name "HELLO WORLD"
          version $0f
          licenseecodenew "FI"
          ramsize $2
          countrycode 1
          name "HELLO WORLD"
          destinationcode 1
          cartridgetype $7
          licenseecodenew "FI"
          ramsize 2
          countrycode $1
          destinationcode $1
        .endgb
        
        .MEMORYMAP
          DEFAULTSLOT 1.01
          SLOT 0.001 $0000 $2000
          SLOT 1.2 STArT $2000 sIzE $6000
        .ENDME

        .ROMBANKMAP
          BANKSTOTAL 2
          BANKSIZE $2000
          BANKS 1
          BANKSIZE $6000
          BANKS 1
        .ENDRO

        ; @BT linked.gb

        .bank 0 slot 0
        .org $1100

        .db "07>"               ; @BT TEST-07 07 START
        .db readSizeA           ; @BT 09
        .db readSizeB           ; @BT 09
        .db readSize4           ; @BT 04
        .db readSize6-1         ; @BT 05
        .db "<07"               ; @BT END
