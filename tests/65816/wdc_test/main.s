
.memorymap
        slotsize $8000
        defaultslot 1
        slot 0 $0000
        slot 1 $8000
.endme                    

.lorom

.rombanksize $8000
.rombanks 3

.bank 0 slot 0
.section "Section1" force

        ; @BT result.rom

        .DB "01>"               ; @BT TEST-01 01 START
        AND #10                 ; @BT 29 0A
        AND 10,X                ; @BT 35 0A
        AND 10                  ; @BT 25 0A
        .DB "<01"               ; @BT END

        .WDC
        
        .DB "02>"               ; @BT TEST-02 02 START
        AND #<10                ; @BT 29 0A
        AND <10,X               ; @BT 35 0A
        AND <10                 ; @BT 25 0A
        LSR <10                 ; @BT 46 0A
        LSR                     ; @BT 4A
        .DB "<02"               ; @BT END

        .NOWDC
        
        .DB "03>"               ; @BT TEST-03 03 START
        AND #<$1020             ; @BT 29 20
        AND <$1020,X            ; @BT 35 20
        AND <$1020              ; @BT 25 20
        LSR <$1020              ; @BT 46 20
        LSR                     ; @BT 4A
        .DB "<03"               ; @BT END
.ends

