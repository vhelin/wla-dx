
.memorymap
        slotsize $8000
        defaultslot 1
        slot 0 $0000
        slot 1 $8000
.endme

.lorom

.rombanksize $8000
.rombanks 3

.define CALCULATION_1 :test_data << 12
.define CALCULATION_2 (test_data - $8000) >> 3
.define CALCULATION_3 test_data+1
.define ONE 1

; @BT result.rom

.bank 0 slot 1
.section "Bank0" force returnorg
main:
        .db "01>"                                               ; @BT TEST-01 01 START
        .dw :test_data << 12                                    ; @BT 00 20
        .dw (test_data - $8000) >> 3                            ; @BT 03 01
        .dw (:test_data << 12) + ((test_data - $8000) >> 3)     ; @BT 03 21
        .dw ((test_data - $8000) >> 3) + (:test_data << 12)     ; @BT 03 21
        .db "<01"                                               ; @BT END
.ends

.bank 2 slot 1
.section "Bank2" force returnorg
        .dsb $818, $00
test_data:
        .db $FF

        .db "02>"                               ; @BT TEST-02 02 START
        .dw CALCULATION_1 + CALCULATION_2       ; @BT 03 21
        .dw CALCULATION_2 + CALCULATION_1       ; @BT 03 21
        .db "<02"                               ; @BT END

        .db "03>"               ; @BT TEST-03 03 START
        .db DIVME/HERE+ONE      ; @BT 81
        .db "<03"               ; @BT END

        .db "04>"               ; @BT TEST-04 04 START
        .dw CALCULATION_3       ; @BT 19 88
        .db bank(CALCULATION_3) ; @BT 02
        .db :CALCULATION_3      ; @BT 02
        .db bank(CALCULATION_3)+1 ; @BT 03
        .db :CALCULATION_3+1      ; @BT 03
        .dw CALCULATION_3+1     ; @BT 1A 88
        .db "<04"               ; @BT END
.ends

.base $20       

.ramsection "LowRAM" bank $7E slot 0 returnorg
    test_var db
.ends

.bank 1 slot 1
.section "Bank1" force returnorg
more_data:
        .db "05>"                                                               ; @BT TEST-05 05 START
        .db (:more_data)+1                                                      ; @BT 22
        .db 1+(:(more_data+1))                                                  ; @BT 22
        .db ((:more_data)+1) + (1+(:(more_data+1))) + (more_data - $8000)       ; @BT 44
        .db "<05"                                                               ; @BT END
        
        .db "06>"                               ; @BT TEST-06 06 START
        .db :test_var                           ; @BT 9E
        .db :(test_var + $8000)                 ; @BT 9F
        .db :test_var + (more_data - $8000)     ; @BT 9E
        .db "<06"                               ; @BT END

        .db "07>"                               ; @BT TEST-07 07 START
        .db bank(more_data),                    ; @BT 21
        .db bankbyte(more_data),                ; @BT 21
        .db :more_data,                         ; @BT 21
        .db bankbyte(more_data + $10),          ; @BT 21
        .db bankbyte($11223344 + $010101)       ; @BT 23
        .db "<07"                               ; @BT END
        
.ends
