
        .MEMORYMAP
           DEFAULTSLOT 0
           SLOTSIZE $2000
           SLOT 0 $8000
        .ENDME

        .ROMBANKMAP
           BANKSTOTAL 16
           BANKSIZE $2000
           BANKS 16
        .ENDRO


        .BANK 0 SLOT 0
        .ORG 0
        
        .DEFINE BIG_VALUE = $ABCD

        ; @BT linked.rom
        
        .DB "01>"                                       ; @BT TEST-01 01 START
addr_01:.DB hibyte($1234), hibyte($FF00+$EE)            ; @BT 12 FF
        .DB hibyte(BIG_VALUE), hibyte(BIG_VALUE+$11)    ; @BT AB AB
        .DB hibyte(addr_01), hibyte(addr_01+1)          ; @BT 80 80
        .DB hibyte($A000) + hibyte($0102)               ; @BT A1
        .DB "<01"                                       ; @BT END

        .DB "02>"                                       ; @BT TEST-02 02 START
addr_02:.DB lobyte($1234), lobyte($FF00+$EE)            ; @BT 34 EE
        .DB lobyte(BIG_VALUE), lobyte(BIG_VALUE+$11)    ; @BT CD DE
        .DB lobyte(addr_02), lobyte(addr_02+1)          ; @BT 10 11
        .DB lobyte($A001) + lobyte($0102)               ; @BT 03
        .DB "<02"                                       ; @BT END

        .BANK 1 SLOT 0
        .ORG 0

        .DB "03>"               ; @BT TEST-03 03 START
addr_03:LDX loword(addr_03).w   ; @BT AE 03 80
        LDA #bank(addr_03).b    ; @BT A9 01
        .DB "<03"               ; @BT END
        
        .BANK 0 SLOT 0
        .ORG $40

        .DEFINE BIGGEST_VALUE = $ABCDEF01
        
        .DB "04>"                                                       ; @BT TEST-04 04 START
addr_04:.DW hiword($12345678), hiword($FF000000+$EE0000)                ; @BT 34 12 EE FF
        .DW hiword(BIGGEST_VALUE), hiword(BIGGEST_VALUE+$110000)        ; @BT CD AB DE AB
        .DW hiword(addr_04), hiword(addr_04+$01234567)                  ; @BT 00 00 23 01
        .DW hiword($A0110000) + hiword($01020304)                       ; @BT 13 A1
        .DB "<04"                                                       ; @BT END

        .DB "05>"                                                       ; @BT TEST-05 05 START
addr_05:.DW loword($12345678), loword($FF000201+$EE1111),               ; @BT 78 56 12 13
        .DW loword(BIGGEST_VALUE), loword(BIGGEST_VALUE+$111234)        ; @BT 01 EF 35 01
        .DW loword(addr_04), loword(addr_04+$01234567)                  ; @BT 43 80 AA C5
        .DW loword($A0110102) + loword($01020304)                       ; @BT 06 04
        .DB "<05"                                                       ; @BT END
        
        .DB "06>"                                                       ; @BT TEST-06 06 START
addr_06:LDA #bank(addr_03) + 1 - 1                                      ; @BT A9 01
        LDA #bank(addr_06) + 1                                          ; @BT A9 01
        .DB 1 + BANK(addr_03) + 1                                       ; @BT 03
        .DB bank(addr_03) + (bank(addr_03) * 2) + BANK(addr_02)         ; @BT 03
        .DB "<06"                                                       ; @BT END

        .DB "07>"                                                       ; @BT TEST-07 07 START
        .DB BANK($2001), bank($2000-1), bank($1FFF + 1)                 ; @BT 01 00 01
        .DB "<07"                                                       ; @BT END

        .DB "08>"                                                       ; @BT TEST-08 08 START
        .DB bankbyte($12345678), 1 + bankbyte($12345678 + 1)            ; @BT 34 35
        .DB "<08"                                                       ; @BT END
        
        .DB "09>"                                                       ; @BT TEST-09 09 START
        .DB bank(addr_03), bankbyte(addr_03 + 100)                      ; @BT 01 01
        .DB "<09"                                                       ; @BT END
        
        .BANK 15 SLOT 0
        .ORG 0

        .DB "10>"                               ; @BT TEST-10 10 START
addr_10:.DB bank(addr_10), bankbyte(addr_10)    ; @BT 0F 0F
        .DB "<10"                               ; @BT END

        .DB "11>"                                            ; @BT TEST-11 11 START
        .DB round(1.1), round(2.5), round(-0.1), round(-1.6) ; @BT 01 03 00 FE
        .DB round(256 / 40)                                  ; @BT 06
        .DB round(13.2 / 2)                                  ; @BT 07
        .DB round(256 / 39)                                  ; @BT 07
        .DB 2 + round(256 / 40) + 1                          ; @BT 09
        .DB "<11"                                            ; @BT END

        .DB "12>"                                            ; @BT TEST-12 12 START
        .DB floor(1.1), floor(2.5), floor(-0.1), floor(-1.6) ; @BT 01 02 FF FE
        .DB floor(256 / 40)                                  ; @BT 06
        .DB 2 + floor(256 / 40) + 1                          ; @BT 09
        .DB "<12"                                            ; @BT END
        
        .DB "13>"                                            ; @BT TEST-13 13 START
        .DB ceil(1.1), ceil(2.5), ceil(-0.1), ceil(-1.6)     ; @BT 02 03 00 FF
        .DB ceil(256 / 40)                                   ; @BT 07
        .DB 2 + ceil(256 / 40) + 1                           ; @BT 0A
        .DB "<13"                                            ; @BT END

	.DEFINE VALUE_1 = 1
	
	.DB "14>"		; @BT TEST-14 14 START
	.DB defined(VALUE_1)	; @BT 01
	.DB defined(VALUE_0)+2  ; @BT 02
	.DB defined(VALUE_{%.1d{VALUE_1}}) ; @BT 01
	
	.DB "<14"		; @BT END
	
