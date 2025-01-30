
        .MEMORYMAP
           DEFAULTSLOT 0
           SLOTSIZE $2000
           SLOT 0 $0000
           SLOT 1 $8000
        .ENDME

        .ROMBANKMAP
           BANKSTOTAL 16
           BANKSIZE $2000
           BANKS 16
        .ENDRO

        .BANK 0 SLOT 1
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

        .BANK 1 SLOT 1
        .ORG 0

        .DB "03>"               ; @BT TEST-03 03 START
addr_03:LDX loword(addr_03).w   ; @BT AE 03 80
        LDA #bank(addr_03).b    ; @BT A9 01
        .DB "<03"               ; @BT END
        
        .BANK 0 SLOT 1
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
        .DB bank(addr_03), bankbyte(addr_03), bankbyte(addr_03 + 100)   ; @BT 01 01 01
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
	.DB defined(VALUE_{%.1d{VALUE_1}})   ; @BT 01
	.DB 1+defined(VALUE_{%.1d{VALUE_1}}) ; @BT 02
	.DB 2+defined(VALUE_{%.1d{0}})       ; @BT 02
        .db (1+1)+(1+1)                      ; @BT 04
	.DB "<14"		; @BT END

        .FUNCTION SUM_AB(varA, varB) (varA + varB)
        .FUNCTION SUM_AB2(var1,var2) var1*1+var2*1
        .function SUM_AB3(var1,var2) SUM_AB(SUM_AB(4-2-1-1,var1),SUM_AB(1+var2-1,0))
        .FUNCTION SUM_AB4(var1,var2) var1*1+var2*(2-1)
        .FUNCTION SUB_A_6(varA) varA-6
        .FUNCTION SUM_ABC(varA, varB, varC) (SUM_AB(varA, varB) + varC)
        .function SUM_ABC2(varA,varB,varC) SUM_AB(SUM_AB(varA,varB),varC)
        .function SUM_ABC3(varA,varB,varC) SUM_AB(SUM_AB(varA,varB),varC)
        .function SUM_ABC4(varA,varB,varC) SUM_AB2(SUM_AB2(varA,varB),varC)
        .function SUM_ABC5(varA,varB,varC) SUM_AB3(SUM_AB3(varA,varB),varC)
        .function SUM_ABC6(varA,varB,varC) SUM_AB3(SUM_AB2(varA,varB),SUM_AB2(2-1,varC-1))
        .function SUM_ABC7(varA,varB,varC) SUM_AB2(SUM_AB2(varA,varB),varC) - 0*SUM_AB2(varA+1,varB)
        .function CONSTANT_1() 1
        .DEFINE SUM = 0 + 1 + SUM_AB(2, 3) + 4 + 5

        .db "15>"               ; @BT TEST-15 15 START
        .db SUM_AB(1,2)         ; @BT 03
        .db SUB_A_6(10)         ; @BT 04
        .db SUM_ABC(1,2,3)      ; @BT 06
        .db SUM_ABC(1,2,3)      ; @BT 06
        .db SUM_ABC2(2,3,4)     ; @BT 09
        .db SUM_ABC3(4,5,6)     ; @BT 0F
        .db SUM_ABC4(5,6,7)     ; @BT 12
        .db SUM_ABC2(2,3,4)     ; @BT 09
        .db SUM_ABC3(4,5,6)     ; @BT 0F
        .db SUM_ABC4(5,6,7)     ; @BT 12
        .db SUM_ABC5(6,7,8)     ; @BT 15
        .db SUM_ABC5(6,7,8)     ; @BT 15
        .db SUM_ABC6(0,1,2)     ; @BT 03
        .db SUM_ABC6(0,1,2)     ; @BT 03
        .db SUM_ABC7(0,1,2)     ; @BT 03
        .db SUM_ABC7(0,1,2)     ; @BT 03
        .db SUM                 ; @BT 0F
        .db "<15"               ; @BT END

        .db "16>"               ; @BT TEST-16 16 START
        .db SUM_AB(2-1,2+1)     ; @BT 04
        .db SUB_A_6(13-2)       ; @BT 05
        .db SUM_ABC(1+0,2+0,3+0)      ; @BT 06
        .db SUM_ABC(1+0,2+0,3+0)      ; @BT 06
        .db SUM_ABC2(2+0,3+0,4+0)     ; @BT 09
        .db SUM_ABC3(4+0,5+0,6+0)     ; @BT 0F
        .db SUM_ABC4(5+0,6+0,7+0)     ; @BT 12
        .db SUM_ABC2(2+0,3+0,4+0)     ; @BT 09
        .db SUM_ABC3(4+0,5+0,6+0)     ; @BT 0F
        .db SUM_ABC4(5+0,6+0,7+0)     ; @BT 12
        .db SUM_ABC5(6+0,7+0,8+0)     ; @BT 15
        .db SUM_ABC5(6+0,7+0,8+0)     ; @BT 15
        .db SUM_ABC6(0+0,1+0,2+0)     ; @BT 03
        .db SUM_ABC6(0+0,1+0,2+0)     ; @BT 03
        .db SUM_ABC7(0+0,1+0,2+0)     ; @BT 03
        .db SUM_ABC7(0+0,1+0,2+0)     ; @BT 03
        .db SUM                 ; @BT 0F
        .db "<16"               ; @BT END

        .db "17>"               ; @BT TEST-17 17 START
        .db 1+SUM_AB(2-1,2+1)-1     ; @BT 04
        .db 1+SUB_A_6(13-2)-1       ; @BT 05
        .db 1+SUM_ABC(1+0,2+0,3+0)-1      ; @BT 06
        .db 1+SUM_ABC(1+0,2+0,3+0)-1      ; @BT 06
        .db 1+SUM_ABC2(2+0,3+0,4+0)-1     ; @BT 09
        .db 1+SUM_ABC3(4+0,5+0,6+0)-1     ; @BT 0F
        .db 1+SUM_ABC4(5+0,6+0,7+0)-1     ; @BT 12
        .db 1+SUM_ABC2(2+0,3+0,4+0)-1     ; @BT 09
        .db 1+SUM_ABC3(4+0,5+0,6+0)-1     ; @BT 0F
        .db 1+SUM_ABC4(5+0,6+0,7+0)-1     ; @BT 12
        .db 1+SUM_ABC5(6+0,7+0,8+0)-1     ; @BT 15
        .db 1+SUM_ABC5(6+0,7+0,8+0)-1     ; @BT 15
        .db 1+SUM_ABC6(0+0,1+0,2+0)-1     ; @BT 03
        .db 1+SUM_ABC6(0+0,1+0,2+0)-1     ; @BT 03
        .db 1+SUM_ABC7(0+0,1+0,2+0)-1     ; @BT 03
        .db 1+SUM_ABC7(0+0,1+0,2+0)-1     ; @BT 03
        .db 1+SUM-1             ; @BT 0F
        .db "<17"               ; @BT END

        .db "18>"               ; @BT TEST-18 18 START
        .db 1+SUM_AB(2-1,2+1)-SUM_AB(0-3,2+2)     ; @BT 04
        .db SUM_AB(0+1,2-1-1)+SUB_A_6(13-2)-1     ; @BT 05
        .db SUB_A_6(8-1)+SUM_ABC(1+0,2+0,3+0)-1      ; @BT 06
        .db SUM_ABC(2-2,3-2,-2+1+1)+SUM_ABC(1+0,2+0,3+0)-1      ; @BT 06
        .db SUM_ABC2(2-1,3+1,4+0)-SUM_ABC2(1-2,2+2,3-3+3)+0     ; @BT 03
        .db SUM_ABC3(-7,1-4,10+1)+SUM_ABC3(4+0,5+0,6+0)-1     ; @BT 0F
        .db SUM_ABC4(1-3,2-2,-2+4)+SUM_ABC4(5+0,6+0,7+0)-0     ; @BT 12
        .db SUM_ABC2(-2+2,-1+2,-4+2+2)+SUM_ABC2(2+0,3+0,4+0)-1     ; @BT 09
        .db SUM_ABC3(0+0,1-1,4-3)+SUM_ABC3(4+0,5+0,6+0)-1     ; @BT 0F
        .db SUM_ABC4(1+1-1,3-2-1,-3+2+1)+SUM_ABC4(5+0,6+0,7+0)-1     ; @BT 12
        .db SUM_ABC5(3-1-1,2-1-1,-2+(1+1))+SUM_ABC5(6+0,7+0,8+0)-1     ; @BT 15
        .db SUM_ABC5(2-(1+1),0-1+1,4-1-1-1)+SUM_ABC5(6+0,7+0,8+0)-1     ; @BT 15
        .db 2+SUM_ABC6(0+0,1+0,2+0)-SUM_ABC6(2*1-1,4-2*2,6-2*2-1)+0    ; @BT 03
        .db 6+SUM_ABC6(0+0,1+0,2+0)-SUM_ABC6(1+(2-1)+0-(1-1),-6+1+6,6>>1)-0     ; @BT 03
        .db SUM_ABC7(0+0,0-0,1*1)+SUM_ABC7(0+0,1+0,2+0)-SUM_ABC7(0+0,2<<1,-5+2)     ; @BT 03
        .db SUM+SUM_ABC7(SUM,1+0,2+0)-SUM_ABC7(SUM*0,SUM+SUM,0+0)     ; @BT 03
        .db 1+SUM-1             ; @BT 0F
        .db "<18"               ; @BT END

        .function MUL_AB(varA, varB) varA*varB
        .function MUL_ABC1(varA, varB, varC) MUL_AB(varA, varB*varC)
        .function MUL_ABC2(varA, varB, varC) 1*MUL_ABC1(varA*(2-1), (0+1)*varB, 1*varC*1)
        .define NUM_THREE = NUM_ONE + NUM_TWO
        .export NUM_THREE

        .db "19>"               ; @BT TEST-19 19 START
        .db MUL_AB(1,2)         ; @BT 02
        .db MUL_AB(1+1,1+2)     ; @BT 06
        .db MUL_AB(MUL_AB(0,1), MUL_AB(2,3)) ; @BT 00
        .db MUL_AB(MUL_AB(1,2), MUL_AB(3,2)) ; @BT 0C
        .db MUL_AB(MUL_AB(MUL_AB(1,2), MUL_AB(2,3)), MUL_AB(MUL_AB(3,4), MUL_AB(4,5)))-2879 ; @BT 01
        .db MUL_ABC1(1,2,3)     ; @BT 06
        .db MUL_ABC1(1+0,2+0,3+0)     ; @BT 06
        .db MUL_ABC1(SUM_ABC6(0+0,1+0,1), SUM_ABC7(1,2,3), SUM_ABC5(0-6,-4-1,5+5+1+1)) ; @BT 0C
        .db MUL_ABC2(1,2,3)     ; @BT 06
        .db MUL_ABC2(1+0,2+0,3+0)     ; @BT 06
        .db MUL_ABC2(SUM_ABC6(0+0,1+0,1), SUM_ABC7(1,2,3), SUM_ABC5(0-6,-4-1,5+5+1+1)) ; @BT 0C
        .db MUL_AB(1,2)+0         ; @BT 02
        .db MUL_AB(1+1,1+2)+0     ; @BT 06
        .db MUL_AB(MUL_AB(0,1), MUL_AB(2,3))+0 ; @BT 00
        .db MUL_AB(MUL_AB(1,2), MUL_AB(3,2))+0 ; @BT 0C
        .db MUL_AB(MUL_AB(MUL_AB(1,2), MUL_AB(2,3)), MUL_AB(MUL_AB(3,4), MUL_AB(4,5)))-2879 ; @BT 01
        .db MUL_ABC1(1,2,3)+0     ; @BT 06
        .db MUL_ABC1(1+0,2+0,3+0)+0     ; @BT 06
        .db MUL_ABC1(SUM_ABC6(0+0,1+0,1), SUM_ABC7(1,2,3), SUM_ABC5(0-6,-4-1,5+5+1+1))+0 ; @BT 0C
        .db MUL_ABC2(1,2,3)+0     ; @BT 06
        .db MUL_ABC2(1+0,2+0,3+0)+0     ; @BT 06
        .db MUL_ABC2(SUM_ABC6(0+0,1+0,1), SUM_ABC7(1,2,3), SUM_ABC5(0-6,-4-1,5+5+1+1))+0 ; @BT 0C
        .db MUL_ABC2(0+1, SUM_ABC6(0+0,1+0,1) - SUM_ABC7(1,2,3), SUM_ABC5(1+2+3, 0-4-2-2-2, 1+1))+0 ; @BT 08
        .db MUL_ABC2(0+NUM_ONE, SUM_ABC6(0+0,NUM_ONE+0,1) - SUM_ABC7(NUM_ONE,NUM_TWO,3), SUM_ABC5(NUM_THREE+3, 0-4-2-NUM_TWO-2, 1+1))+0 ; @BT 08
        .db NUM_THREE * NUM_TWO ; @BT 06
        .db NUM_THREE           ; @BT 03
        .db "<19"               ; @BT END
        
        .db "20>"               ; @BT TEST-20 20 START
        .rept MUL_AB(1,2)
        .db 1                   ; @BT 01 01
        .endr
        .db "<20"               ; @BT END

        .db "21>"               ; @BT TEST-21 21 START
        .rept MUL_AB(0+2,1+1)
        .db 2                   ; @BT 02 02 02 02
        .endr
        .db "<21"               ; @BT END

        .db "22>"               ; @BT TEST-22 22 START
        .rept SUM_ABC7(1*1,2*1,3*1)
        .db 3                   ; @BT 03 03 03 03 03 03
        .endr
        .db "<22"               ; @BT END

        .db "23>"               ; @BT TEST-23 23 START
        .rept SUM_ABC7(MUL_ABC1(1+0,0+1,2*1)+0, 1+MUL_ABC2(-1*1,1*(2-1),SUM_ABC3(0+0,4-1-1-1,2-2)*(5-4)), 1+SUM_ABC6(0-2,MUL_ABC2(0+0,1-1,2-2),0))
        .db 4                   ; @BT 04
        .endr
        .rept 0+SUM_ABC7(MUL_ABC1(1+0,0+1,2*1)+0, 1+MUL_ABC2(-1*1,1*(2-1),SUM_ABC3(0+0,4-1-1-1,2-2)*(5-4)), 1+SUM_ABC6(0-2,MUL_ABC2(0+0,1-1,2-2),0))*2
        .db 5                   ; @BT 05 05
        .endr
        .db NUM_THREE-NUM_TWO+(-NUM_TWO-NUM_THREE)+NUM_THREE+NUM_THREE ; @BT 02
        .db NUM_THREE-NUM_TWO-NUM_TWO-NUM_THREE+NUM_THREE+NUM_THREE    ; @BT 02
        .db 3-NUM_TWO-NUM_TWO-NUM_THREE+NUM_THREE+NUM_THREE    ; @BT 02
        .db 3-NUM_TWO-NUM_TWO-NUM_THREE+NUM_THREE*2+NUM_THREE+1-NUM_TWO-NUM_TWO ; @BT 02
        .db "<23"               ; @BT END

        // this is the same as TEST-19, but inside .REPT
        .db "24>"               ; @BT TEST-24 24 START
        .rept 20
        .db MUL_AB(1,2)         ; @BT 02
        .db MUL_AB(1+1,1+2)     ; @BT 06
        .db MUL_AB(MUL_AB(0,1), MUL_AB(2,3)) ; @BT 00
        .db MUL_AB(MUL_AB(1,2), MUL_AB(3,2)) ; @BT 0C
        .db MUL_AB(MUL_AB(MUL_AB(1,2), MUL_AB(2,3)), MUL_AB(MUL_AB(3,4), MUL_AB(4,5)))-2879 ; @BT 01
        .db MUL_ABC1(1,2,3)     ; @BT 06
        .db MUL_ABC1(1+0,2+0,3+0)     ; @BT 06
        .db MUL_ABC1(SUM_ABC6(0+0,1+0,1), SUM_ABC7(1,2,3), SUM_ABC5(0-6,-4-1,5+5+1+1)) ; @BT 0C
        .db MUL_ABC2(1,2,3)     ; @BT 06
        .db MUL_ABC2(1+0,2+0,3+0)     ; @BT 06
        .db MUL_ABC2(SUM_ABC6(0+0,1+0,1), SUM_ABC7(1,2,3), SUM_ABC5(0-6,-4-1,5+5+1+1)) ; @BT 0C
        .db MUL_AB(1,2)+0         ; @BT 02
        .db MUL_AB(1+1,1+2)+0     ; @BT 06
        .db MUL_AB(MUL_AB(0,1), MUL_AB(2,3))+0 ; @BT 00
        .db MUL_AB(MUL_AB(1,2), MUL_AB(3,2))+0 ; @BT 0C
        .db MUL_AB(MUL_AB(MUL_AB(1,2), MUL_AB(2,3)), MUL_AB(MUL_AB(3,4), MUL_AB(4,5)))-2879 ; @BT 01
        .db MUL_ABC1(1,2,3)+0     ; @BT 06
        .db MUL_ABC1(1+0,2+0,3+0)+0     ; @BT 06
        .db MUL_ABC1(SUM_ABC6(0+0,1+0,1), SUM_ABC7(1,-NUM_TWO+NUM_THREE+1,3), SUM_ABC5(0-6,-4-1,5+5+1+1))+0 ; @BT 0C
        .db MUL_ABC1(SUM_ABC6(0+0,1+0,1), SUM_ABC7(1,NUM_THREE-NUM_TWO+1,3), SUM_ABC5(0-6,-4-1,5+5+1+1))+0 ; @BT 0C
        .db MUL_ABC2(1,2,3)+0     ; @BT 06
        .db MUL_ABC2(1+0,2+0,3+0)+0     ; @BT 06
        .db MUL_ABC2(SUM_ABC6(0+0,1+0,1), SUM_ABC7(1,2,3), SUM_ABC5(0-6,-4-1,5+5+1+1))+0 ; @BT 0C
        .db MUL_ABC2(0+1, SUM_ABC6(0+0,1+0,1) - SUM_ABC7(1,NUM_TWO+0,NUM_THREE), SUM_ABC5(1+2+NUM_THREE, 0-4-2-2-2, 1+1))+0 ; @BT 08
        .db MUL_ABC1(SUM_ABC6(0+0,1+0,1), SUM_ABC7(1,NUM_THREE-NUM_TWO+(-NUM_TWO-NUM_THREE)+NUM_THREE+NUM_THREE,3), SUM_ABC5(0-6,-4-1,5+5+1+1))+0 ; @BT 0C
        .endr
                                ; @BT 02 06 00 0C 01 06 06 0C 06 06 0C 02 06 00 0C 01 06 06 0C 0C 06 06 0C 08 0C
                                ; @BT 02 06 00 0C 01 06 06 0C 06 06 0C 02 06 00 0C 01 06 06 0C 0C 06 06 0C 08 0C
                                ; @BT 02 06 00 0C 01 06 06 0C 06 06 0C 02 06 00 0C 01 06 06 0C 0C 06 06 0C 08 0C
                                ; @BT 02 06 00 0C 01 06 06 0C 06 06 0C 02 06 00 0C 01 06 06 0C 0C 06 06 0C 08 0C
                                ; @BT 02 06 00 0C 01 06 06 0C 06 06 0C 02 06 00 0C 01 06 06 0C 0C 06 06 0C 08 0C
                                ; @BT 02 06 00 0C 01 06 06 0C 06 06 0C 02 06 00 0C 01 06 06 0C 0C 06 06 0C 08 0C
                                ; @BT 02 06 00 0C 01 06 06 0C 06 06 0C 02 06 00 0C 01 06 06 0C 0C 06 06 0C 08 0C
                                ; @BT 02 06 00 0C 01 06 06 0C 06 06 0C 02 06 00 0C 01 06 06 0C 0C 06 06 0C 08 0C
                                ; @BT 02 06 00 0C 01 06 06 0C 06 06 0C 02 06 00 0C 01 06 06 0C 0C 06 06 0C 08 0C
                                ; @BT 02 06 00 0C 01 06 06 0C 06 06 0C 02 06 00 0C 01 06 06 0C 0C 06 06 0C 08 0C
                                ; @BT 02 06 00 0C 01 06 06 0C 06 06 0C 02 06 00 0C 01 06 06 0C 0C 06 06 0C 08 0C
                                ; @BT 02 06 00 0C 01 06 06 0C 06 06 0C 02 06 00 0C 01 06 06 0C 0C 06 06 0C 08 0C
                                ; @BT 02 06 00 0C 01 06 06 0C 06 06 0C 02 06 00 0C 01 06 06 0C 0C 06 06 0C 08 0C
                                ; @BT 02 06 00 0C 01 06 06 0C 06 06 0C 02 06 00 0C 01 06 06 0C 0C 06 06 0C 08 0C
                                ; @BT 02 06 00 0C 01 06 06 0C 06 06 0C 02 06 00 0C 01 06 06 0C 0C 06 06 0C 08 0C
                                ; @BT 02 06 00 0C 01 06 06 0C 06 06 0C 02 06 00 0C 01 06 06 0C 0C 06 06 0C 08 0C
                                ; @BT 02 06 00 0C 01 06 06 0C 06 06 0C 02 06 00 0C 01 06 06 0C 0C 06 06 0C 08 0C
                                ; @BT 02 06 00 0C 01 06 06 0C 06 06 0C 02 06 00 0C 01 06 06 0C 0C 06 06 0C 08 0C
                                ; @BT 02 06 00 0C 01 06 06 0C 06 06 0C 02 06 00 0C 01 06 06 0C 0C 06 06 0C 08 0C
        .db "<24"               ; @BT END

        .db "25>"               ; @BT TEST-25 25 START
        .rept 20
        ASL MUL_AB(1,2)         ; @BT 06 02
        ASL MUL_AB(1+1,1+2)     ; @BT 06 06
        ASL MUL_AB(MUL_AB(0,1), MUL_AB(2,3)) ; @BT 06 00
        ASL MUL_AB(MUL_AB(1,2), MUL_AB(3,2)) ; @BT 06 0C
        ASL MUL_AB(MUL_AB(MUL_AB(1,2), MUL_AB(2,3)), MUL_AB(MUL_AB(3,4), MUL_AB(4,5)))-2879 ; @BT 06 01
        ASL MUL_ABC1(1,2,3)     ; @BT 06 06
        ASL MUL_ABC1(1+0,2+0,3+0)     ; @BT 06 06
        ASL MUL_ABC1(SUM_ABC6(0+0,1+0,1), SUM_ABC7(1,2,3), SUM_ABC5(0-6,-4-1,5+5+1+1)) ; @BT 06 0C
        ASL MUL_ABC2(1,2,3)     ; @BT 06 06
        ASL MUL_ABC2(1+0,2+0,3+0)     ; @BT 06 06
        ASL MUL_ABC2(SUM_ABC6(0+0,1+0,1), SUM_ABC7(1,2,3), SUM_ABC5(0-6,-4-1,5+5+1+1)) ; @BT 06 0C
        ASL MUL_AB(1,2)+0         ; @BT 06 02
        ASL MUL_AB(1+1,1+2)+0     ; @BT 06 06
        ASL MUL_AB(MUL_AB(0,1), MUL_AB(2,3))+0 ; @BT 06 00
        ASL MUL_AB(MUL_AB(1,2), MUL_AB(3,2))+0 ; @BT 06 0C
        ASL MUL_AB(MUL_AB(MUL_AB(1,2), MUL_AB(2,3)), MUL_AB(MUL_AB(3,4), MUL_AB(4,5)))-2879 ; @BT 06 01
        ASL MUL_ABC1(1,2,3)+0     ; @BT 06 06
        ASL MUL_ABC1(1+0,2+0,3+0)+0     ; @BT 06 06
        ASL MUL_ABC1(SUM_ABC6(0+0,1+0,1), SUM_ABC7(1,2,3), SUM_ABC5(0-6,-4-1,5+5+1+1))+0 ; @BT 06 0C
        ASL MUL_ABC2(1,2,3)+0     ; @BT 06 06
        ASL MUL_ABC2(1+0,2+0,3+0)+0     ; @BT 06 06
        ASL MUL_ABC2(SUM_ABC6(0+0,1+0,1), SUM_ABC7(1,2,3), SUM_ABC5(0-6,-4-1,5+5+1+1))+0 ; @BT 06 0C
        ASL MUL_ABC2(0+1, SUM_ABC6(0+0,1+0,1) - SUM_ABC7(1,2,3), SUM_ABC5(1+2+3, 0-4-2-2-2, 1+1))+0 ; @BT 06 08
        .endr
                                ; @BT 06 02 06 06 06 00 06 0C 06 01 06 06 06 06 06 0C 06 06 06 06 06 0C 06 02 06 06 06 00 06 0C 06 01 06 06 06 06 06 0C 06 06 06 06 06 0C 06 08
                                ; @BT 06 02 06 06 06 00 06 0C 06 01 06 06 06 06 06 0C 06 06 06 06 06 0C 06 02 06 06 06 00 06 0C 06 01 06 06 06 06 06 0C 06 06 06 06 06 0C 06 08
                                ; @BT 06 02 06 06 06 00 06 0C 06 01 06 06 06 06 06 0C 06 06 06 06 06 0C 06 02 06 06 06 00 06 0C 06 01 06 06 06 06 06 0C 06 06 06 06 06 0C 06 08
                                ; @BT 06 02 06 06 06 00 06 0C 06 01 06 06 06 06 06 0C 06 06 06 06 06 0C 06 02 06 06 06 00 06 0C 06 01 06 06 06 06 06 0C 06 06 06 06 06 0C 06 08
                                ; @BT 06 02 06 06 06 00 06 0C 06 01 06 06 06 06 06 0C 06 06 06 06 06 0C 06 02 06 06 06 00 06 0C 06 01 06 06 06 06 06 0C 06 06 06 06 06 0C 06 08
                                ; @BT 06 02 06 06 06 00 06 0C 06 01 06 06 06 06 06 0C 06 06 06 06 06 0C 06 02 06 06 06 00 06 0C 06 01 06 06 06 06 06 0C 06 06 06 06 06 0C 06 08
                                ; @BT 06 02 06 06 06 00 06 0C 06 01 06 06 06 06 06 0C 06 06 06 06 06 0C 06 02 06 06 06 00 06 0C 06 01 06 06 06 06 06 0C 06 06 06 06 06 0C 06 08
                                ; @BT 06 02 06 06 06 00 06 0C 06 01 06 06 06 06 06 0C 06 06 06 06 06 0C 06 02 06 06 06 00 06 0C 06 01 06 06 06 06 06 0C 06 06 06 06 06 0C 06 08
                                ; @BT 06 02 06 06 06 00 06 0C 06 01 06 06 06 06 06 0C 06 06 06 06 06 0C 06 02 06 06 06 00 06 0C 06 01 06 06 06 06 06 0C 06 06 06 06 06 0C 06 08
                                ; @BT 06 02 06 06 06 00 06 0C 06 01 06 06 06 06 06 0C 06 06 06 06 06 0C 06 02 06 06 06 00 06 0C 06 01 06 06 06 06 06 0C 06 06 06 06 06 0C 06 08
                                ; @BT 06 02 06 06 06 00 06 0C 06 01 06 06 06 06 06 0C 06 06 06 06 06 0C 06 02 06 06 06 00 06 0C 06 01 06 06 06 06 06 0C 06 06 06 06 06 0C 06 08
                                ; @BT 06 02 06 06 06 00 06 0C 06 01 06 06 06 06 06 0C 06 06 06 06 06 0C 06 02 06 06 06 00 06 0C 06 01 06 06 06 06 06 0C 06 06 06 06 06 0C 06 08
                                ; @BT 06 02 06 06 06 00 06 0C 06 01 06 06 06 06 06 0C 06 06 06 06 06 0C 06 02 06 06 06 00 06 0C 06 01 06 06 06 06 06 0C 06 06 06 06 06 0C 06 08
                                ; @BT 06 02 06 06 06 00 06 0C 06 01 06 06 06 06 06 0C 06 06 06 06 06 0C 06 02 06 06 06 00 06 0C 06 01 06 06 06 06 06 0C 06 06 06 06 06 0C 06 08
                                ; @BT 06 02 06 06 06 00 06 0C 06 01 06 06 06 06 06 0C 06 06 06 06 06 0C 06 02 06 06 06 00 06 0C 06 01 06 06 06 06 06 0C 06 06 06 06 06 0C 06 08
                                ; @BT 06 02 06 06 06 00 06 0C 06 01 06 06 06 06 06 0C 06 06 06 06 06 0C 06 02 06 06 06 00 06 0C 06 01 06 06 06 06 06 0C 06 06 06 06 06 0C 06 08
                                ; @BT 06 02 06 06 06 00 06 0C 06 01 06 06 06 06 06 0C 06 06 06 06 06 0C 06 02 06 06 06 00 06 0C 06 01 06 06 06 06 06 0C 06 06 06 06 06 0C 06 08
                                ; @BT 06 02 06 06 06 00 06 0C 06 01 06 06 06 06 06 0C 06 06 06 06 06 0C 06 02 06 06 06 00 06 0C 06 01 06 06 06 06 06 0C 06 06 06 06 06 0C 06 08
                                ; @BT 06 02 06 06 06 00 06 0C 06 01 06 06 06 06 06 0C 06 06 06 06 06 0C 06 02 06 06 06 00 06 0C 06 01 06 06 06 06 06 0C 06 06 06 06 06 0C 06 08
        
        .db "<25"               ; @BT END

        ////////////////////////////////////////////////////////////////////////////////////////////
        // repeat some of the tests inside .SECTIONs
        ////////////////////////////////////////////////////////////////////////////////////////////

        .section "RepeatedCalculations1" free

        .db "15>"               ; @BT TEST-15 15 START
        .db SUM_AB(1,2)         ; @BT 03
        .db SUB_A_6(10)         ; @BT 04
        .db SUM_ABC(1,2,3)      ; @BT 06
        .db SUM_ABC(1,2,3)      ; @BT 06
        .db SUM_ABC2(2,3,4)     ; @BT 09
        .db SUM_ABC3(4,5,6)     ; @BT 0F
        .db SUM_ABC4(5,6,7)     ; @BT 12
        .db SUM_ABC2(2,3,4)     ; @BT 09
        .db SUM_ABC3(4,5,6)     ; @BT 0F
        .db SUM_ABC4(5,6,7)     ; @BT 12
        .db SUM_ABC5(6,7,8)     ; @BT 15
        .db SUM_ABC5(6,7,8)     ; @BT 15
        .db SUM_ABC6(0,1,2)     ; @BT 03
        .db SUM_ABC6(0,1,2)     ; @BT 03
        .db SUM_ABC7(0,1,2)     ; @BT 03
        .db SUM_ABC7(0,1,2)     ; @BT 03
        .db SUM                 ; @BT 0F
        .db "<15"               ; @BT END

        .db "16>"               ; @BT TEST-16 16 START
        .db SUM_AB(2-1,2+1)     ; @BT 04
        .db SUB_A_6(13-2)       ; @BT 05
        .db SUM_ABC(1+0,2+0,3+0)      ; @BT 06
        .db SUM_ABC(1+0,2+0,3+0)      ; @BT 06
        .db SUM_ABC2(2+0,3+0,4+0)     ; @BT 09
        .db SUM_ABC3(4+0,5+0,6+0)     ; @BT 0F
        .db SUM_ABC4(5+0,6+0,7+0)     ; @BT 12
        .db SUM_ABC2(2+0,3+0,4+0)     ; @BT 09
        .db SUM_ABC3(4+0,5+0,6+0)     ; @BT 0F
        .db SUM_ABC4(5+0,6+0,7+0)     ; @BT 12
        .db SUM_ABC5(6+0,7+0,8+0)     ; @BT 15
        .db SUM_ABC5(6+0,7+0,8+0)     ; @BT 15
        .db SUM_ABC6(0+0,1+0,2+0)     ; @BT 03
        .db SUM_ABC6(0+0,1+0,2+0)     ; @BT 03
        .db SUM_ABC7(0+0,1+0,2+0)     ; @BT 03
        .db SUM_ABC7(0+0,1+0,2+0)     ; @BT 03
        .db SUM                 ; @BT 0F
        .db "<16"               ; @BT END

        .db "17>"               ; @BT TEST-17 17 START
        .db 1+SUM_AB(2-1,2+1)-1     ; @BT 04
        .db 1+SUB_A_6(13-2)-1       ; @BT 05
        .db 1+SUM_ABC(1+0,2+0,3+0)-1      ; @BT 06
        .db 1+SUM_ABC(1+0,2+0,3+0)-1      ; @BT 06
        .db 1+SUM_ABC2(2+0,3+0,4+0)-1     ; @BT 09
        .db 1+SUM_ABC3(4+0,5+0,6+0)-1     ; @BT 0F
        .db 1+SUM_ABC4(5+0,6+0,7+0)-1     ; @BT 12
        .db 1+SUM_ABC2(2+0,3+0,4+0)-1     ; @BT 09
        .db 1+SUM_ABC3(4+0,5+0,6+0)-1     ; @BT 0F
        .db 1+SUM_ABC4(5+0,6+0,7+0)-1     ; @BT 12
        .db 1+SUM_ABC5(6+0,7+0,8+0)-1     ; @BT 15
        .db 1+SUM_ABC5(6+0,7+0,8+0)-1     ; @BT 15
        .db 1+SUM_ABC6(0+0,1+0,2+0)-1     ; @BT 03
        .db 1+SUM_ABC6(0+0,1+0,2+0)-1     ; @BT 03
        .db 1+SUM_ABC7(0+0,1+0,2+0)-1     ; @BT 03
        .db 1+SUM_ABC7(0+0,1+0,2+0)-1     ; @BT 03
        .db 1+SUM-1             ; @BT 0F
        .db "<17"               ; @BT END

        .db "18>"               ; @BT TEST-18 18 START
        .db 1+SUM_AB(2-1,2+1)-SUM_AB(0-3,2+2)     ; @BT 04
        .db SUM_AB(0+1,2-1-1)+SUB_A_6(13-2)-1     ; @BT 05
        .db SUB_A_6(8-1)+SUM_ABC(1+0,2+0,3+0)-1      ; @BT 06
        .db SUM_ABC(2-2,3-2,-2+1+1)+SUM_ABC(1+0,2+0,3+0)-1      ; @BT 06
        .db SUM_ABC2(2-1,3+1,4+0)-SUM_ABC2(1-2,2+2,3-3+3)+0     ; @BT 03
        .db SUM_ABC3(-7,1-4,10+1)+SUM_ABC3(4+0,5+0,6+0)-1     ; @BT 0F
        .db SUM_ABC4(1-3,2-2,-2+4)+SUM_ABC4(5+0,6+0,7+0)-0     ; @BT 12
        .db SUM_ABC2(-2+2,-1+2,-4+2+2)+SUM_ABC2(2+0,3+0,4+0)-1     ; @BT 09
        .db SUM_ABC3(0+0,1-1,4-3)+SUM_ABC3(4+0,5+0,6+0)-1     ; @BT 0F
        .db SUM_ABC4(1+1-1,3-2-1,-3+2+1)+SUM_ABC4(5+0,6+0,7+0)-1     ; @BT 12
        .db SUM_ABC5(3-1-1,2-1-1,-2+(1+1))+SUM_ABC5(6+0,7+0,8+0)-1     ; @BT 15
        .db SUM_ABC5(2-(1+1),0-1+1,4-1-1-1)+SUM_ABC5(6+0,7+0,8+0)-1     ; @BT 15
        .db 2+SUM_ABC6(0+0,1+0,2+0)-SUM_ABC6(2*1-1,4-2*2,6-2*2-1)+0    ; @BT 03
        .db 6+SUM_ABC6(0+0,1+0,2+0)-SUM_ABC6(1+(2-1)+0-(1-1),-6+1+6,6>>1)-0     ; @BT 03
        .db SUM_ABC7(0+0,0-0,1*1)+SUM_ABC7(0+0,1+0,2+0)-SUM_ABC7(0+0,2<<1,-5+2)     ; @BT 03
        .db SUM+SUM_ABC7(SUM,1+0,2+0)-SUM_ABC7(SUM*0,SUM+SUM,0+0)     ; @BT 03
        .db 1+SUM-1             ; @BT 0F
        .db "<18"               ; @BT END

        .db "19>"               ; @BT TEST-19 19 START
        .db MUL_AB(1,2)         ; @BT 02
        .db MUL_AB(1+1,1+2)     ; @BT 06
        .db MUL_AB(MUL_AB(0,1), MUL_AB(2,3)) ; @BT 00
        .db MUL_AB(MUL_AB(1,2), MUL_AB(3,2)) ; @BT 0C
        .db MUL_AB(MUL_AB(MUL_AB(1,2), MUL_AB(2,3)), MUL_AB(MUL_AB(3,4), MUL_AB(4,5)))-2879 ; @BT 01
        .db MUL_ABC1(1,2,3)     ; @BT 06
        .db MUL_ABC1(1+0,2+0,3+0)     ; @BT 06
        .db MUL_ABC1(SUM_ABC6(0+0,1+0,1), SUM_ABC7(1,2,3), SUM_ABC5(0-6,-4-1,5+5+1+1)) ; @BT 0C
        .db MUL_ABC2(1,2,3)     ; @BT 06
        .db MUL_ABC2(1+0,2+0,3+0)     ; @BT 06
        .db MUL_ABC2(SUM_ABC6(0+0,1+0,1), SUM_ABC7(1,2,3), SUM_ABC5(0-6,-4-1,5+5+1+1)) ; @BT 0C
        .db MUL_AB(1,2)+0         ; @BT 02
        .db MUL_AB(1+1,1+2)+0     ; @BT 06
        .db MUL_AB(MUL_AB(0,1), MUL_AB(2,3))+0 ; @BT 00
        .db MUL_AB(MUL_AB(1,2), MUL_AB(3,2))+0 ; @BT 0C
        .db MUL_AB(MUL_AB(MUL_AB(1,2), MUL_AB(2,3)), MUL_AB(MUL_AB(3,4), MUL_AB(4,5)))-2879 ; @BT 01
        .db MUL_ABC1(1,2,3)+0     ; @BT 06
        .db MUL_ABC1(1+0,2+0,3+0)+0     ; @BT 06
        .db MUL_ABC1(SUM_ABC6(0+0,1+0,1), SUM_ABC7(1,2,3), SUM_ABC5(0-6,-4-1,5+5+1+1))+0 ; @BT 0C
        .db MUL_ABC2(1,2,3)+0     ; @BT 06
        .db MUL_ABC2(1+0,2+0,3+0)+0     ; @BT 06
        .db MUL_ABC2(SUM_ABC6(0+0,1+0,1), SUM_ABC7(1,2,3), SUM_ABC5(0-6,-4-1,5+5+1+1))+0 ; @BT 0C
        .db MUL_ABC2(0+1, SUM_ABC6(0+0,1+0,1) - SUM_ABC7(1,2,3), SUM_ABC5(1+2+3, 0-4-2-2-2, 1+1))+0 ; @BT 08
        .db MUL_ABC2(0+NUM_ONE, SUM_ABC6(0+0,NUM_ONE+0,1) - SUM_ABC7(NUM_ONE,NUM_TWO,3), SUM_ABC5(NUM_THREE+3, 0-4-2-NUM_TWO-2, 1+1))+0 ; @BT 08
        .db NUM_THREE * NUM_TWO ; @BT 06
        .db NUM_THREE           ; @BT 03
        .db "<19"               ; @BT END
        
        .db "20>"               ; @BT TEST-20 20 START
        .rept MUL_AB(1,2)
        .db 1                   ; @BT 01 01
        .endr
        .db "<20"               ; @BT END

        .db "21>"               ; @BT TEST-21 21 START
        .rept MUL_AB(0+2,1+1)
        .db 2                   ; @BT 02 02 02 02
        .endr
        .db "<21"               ; @BT END

        .db "22>"               ; @BT TEST-22 22 START
        .rept SUM_ABC7(1*1,2*1,3*1)
        .db 3                   ; @BT 03 03 03 03 03 03
        .endr
        .db "<22"               ; @BT END

        .db "23>"               ; @BT TEST-23 23 START
        .rept SUM_ABC7(MUL_ABC1(1+0,0+1,2*1)+0, 1+MUL_ABC2(-1*1,1*(2-1),SUM_ABC3(0+0,4-1-1-1,2-2)*(5-4)), 1+SUM_ABC6(0-2,MUL_ABC2(0+0,1-1,2-2),0))
        .db 4                   ; @BT 04
        .endr
        .rept 0+SUM_ABC7(MUL_ABC1(1+0,0+1,2*1)+0, 1+MUL_ABC2(-1*1,1*(2-1),SUM_ABC3(0+0,4-1-1-1,2-2)*(5-4)), 1+SUM_ABC6(0-2,MUL_ABC2(0+0,1-1,2-2),0))*2
        .db 5                   ; @BT 05 05
        .endr
        .db NUM_THREE-NUM_TWO+(-NUM_TWO-NUM_THREE)+NUM_THREE+NUM_THREE ; @BT 02
        .db NUM_THREE-NUM_TWO-NUM_TWO-NUM_THREE+NUM_THREE+NUM_THREE    ; @BT 02
        .db 3-NUM_TWO-NUM_TWO-NUM_THREE+NUM_THREE+NUM_THREE    ; @BT 02
        .db 3-NUM_TWO-NUM_TWO-NUM_THREE+NUM_THREE*2+NUM_THREE+1-NUM_TWO-NUM_TWO ; @BT 02
        .db "<23"               ; @BT END

        // this is the same as TEST-19, but inside .REPT
        .db "24>"               ; @BT TEST-24 24 START
        .rept 20
        .db MUL_AB(1,2)         ; @BT 02
        .db MUL_AB(1+1,1+2)     ; @BT 06
        .db MUL_AB(MUL_AB(0,1), MUL_AB(2,3)) ; @BT 00
        .db MUL_AB(MUL_AB(1,2), MUL_AB(3,2)) ; @BT 0C
        .db MUL_AB(MUL_AB(MUL_AB(1,2), MUL_AB(2,3)), MUL_AB(MUL_AB(3,4), MUL_AB(4,5)))-2879 ; @BT 01
        .db MUL_ABC1(1,2,3)     ; @BT 06
        .db MUL_ABC1(1+0,2+0,3+0)     ; @BT 06
        .db MUL_ABC1(SUM_ABC6(0+0,1+0,1), SUM_ABC7(1,2,3), SUM_ABC5(0-6,-4-1,5+5+1+1)) ; @BT 0C
        .db MUL_ABC2(1,2,3)     ; @BT 06
        .db MUL_ABC2(1+0,2+0,3+0)     ; @BT 06
        .db MUL_ABC2(SUM_ABC6(0+0,1+0,1), SUM_ABC7(1,2,3), SUM_ABC5(0-6,-4-1,5+5+1+1)) ; @BT 0C
        .db MUL_AB(1,2)+0         ; @BT 02
        .db MUL_AB(1+1,1+2)+0     ; @BT 06
        .db MUL_AB(MUL_AB(0,1), MUL_AB(2,3))+0 ; @BT 00
        .db MUL_AB(MUL_AB(1,2), MUL_AB(3,2))+0 ; @BT 0C
        .db MUL_AB(MUL_AB(MUL_AB(1,2), MUL_AB(2,3)), MUL_AB(MUL_AB(3,4), MUL_AB(4,5)))-2879 ; @BT 01
        .db MUL_ABC1(1,2,3)+0     ; @BT 06
        .db MUL_ABC1(1+0,2+0,3+0)+0     ; @BT 06
        .db MUL_ABC1(SUM_ABC6(0+0,1+0,1), SUM_ABC7(1,-NUM_TWO+NUM_THREE+1,3), SUM_ABC5(0-6,-4-1,5+5+1+1))+0 ; @BT 0C
        .db MUL_ABC1(SUM_ABC6(0+0,1+0,1), SUM_ABC7(1,NUM_THREE-NUM_TWO+1,3), SUM_ABC5(0-6,-4-1,5+5+1+1))+0 ; @BT 0C
        .db MUL_ABC2(1,2,3)+0     ; @BT 06
        .db MUL_ABC2(1+0,2+0,3+0)+0     ; @BT 06
        .db MUL_ABC2(SUM_ABC6(0+0,1+0,1), SUM_ABC7(1,2,3), SUM_ABC5(0-6,-4-1,5+5+1+1))+0 ; @BT 0C
        .db MUL_ABC2(0+1, SUM_ABC6(0+0,1+0,1) - SUM_ABC7(1,NUM_TWO+0,NUM_THREE), SUM_ABC5(1+2+NUM_THREE, 0-4-2-2-2, 1+1))+0 ; @BT 08
        .db MUL_ABC1(SUM_ABC6(0+0,1+0,1), SUM_ABC7(1,NUM_THREE-NUM_TWO+(-NUM_TWO-NUM_THREE)+NUM_THREE+NUM_THREE,3), SUM_ABC5(0-6,-4-1,5+5+1+1))+0 ; @BT 0C
        .endr
                                ; @BT 02 06 00 0C 01 06 06 0C 06 06 0C 02 06 00 0C 01 06 06 0C 0C 06 06 0C 08 0C
                                ; @BT 02 06 00 0C 01 06 06 0C 06 06 0C 02 06 00 0C 01 06 06 0C 0C 06 06 0C 08 0C
                                ; @BT 02 06 00 0C 01 06 06 0C 06 06 0C 02 06 00 0C 01 06 06 0C 0C 06 06 0C 08 0C
                                ; @BT 02 06 00 0C 01 06 06 0C 06 06 0C 02 06 00 0C 01 06 06 0C 0C 06 06 0C 08 0C
                                ; @BT 02 06 00 0C 01 06 06 0C 06 06 0C 02 06 00 0C 01 06 06 0C 0C 06 06 0C 08 0C
                                ; @BT 02 06 00 0C 01 06 06 0C 06 06 0C 02 06 00 0C 01 06 06 0C 0C 06 06 0C 08 0C
                                ; @BT 02 06 00 0C 01 06 06 0C 06 06 0C 02 06 00 0C 01 06 06 0C 0C 06 06 0C 08 0C
                                ; @BT 02 06 00 0C 01 06 06 0C 06 06 0C 02 06 00 0C 01 06 06 0C 0C 06 06 0C 08 0C
                                ; @BT 02 06 00 0C 01 06 06 0C 06 06 0C 02 06 00 0C 01 06 06 0C 0C 06 06 0C 08 0C
                                ; @BT 02 06 00 0C 01 06 06 0C 06 06 0C 02 06 00 0C 01 06 06 0C 0C 06 06 0C 08 0C
                                ; @BT 02 06 00 0C 01 06 06 0C 06 06 0C 02 06 00 0C 01 06 06 0C 0C 06 06 0C 08 0C
                                ; @BT 02 06 00 0C 01 06 06 0C 06 06 0C 02 06 00 0C 01 06 06 0C 0C 06 06 0C 08 0C
                                ; @BT 02 06 00 0C 01 06 06 0C 06 06 0C 02 06 00 0C 01 06 06 0C 0C 06 06 0C 08 0C
                                ; @BT 02 06 00 0C 01 06 06 0C 06 06 0C 02 06 00 0C 01 06 06 0C 0C 06 06 0C 08 0C
                                ; @BT 02 06 00 0C 01 06 06 0C 06 06 0C 02 06 00 0C 01 06 06 0C 0C 06 06 0C 08 0C
                                ; @BT 02 06 00 0C 01 06 06 0C 06 06 0C 02 06 00 0C 01 06 06 0C 0C 06 06 0C 08 0C
                                ; @BT 02 06 00 0C 01 06 06 0C 06 06 0C 02 06 00 0C 01 06 06 0C 0C 06 06 0C 08 0C
                                ; @BT 02 06 00 0C 01 06 06 0C 06 06 0C 02 06 00 0C 01 06 06 0C 0C 06 06 0C 08 0C
                                ; @BT 02 06 00 0C 01 06 06 0C 06 06 0C 02 06 00 0C 01 06 06 0C 0C 06 06 0C 08 0C
        .db "<24"               ; @BT END

        .db "25>"               ; @BT TEST-25 25 START
        .rept 20
        ASL MUL_AB(1,2)         ; @BT 06 02
        ASL MUL_AB(1+1,1+2)     ; @BT 06 06
        ASL MUL_AB(MUL_AB(0,1), MUL_AB(2,3)) ; @BT 06 00
        ASL MUL_AB(MUL_AB(1,2), MUL_AB(3,2)) ; @BT 06 0C
        ASL MUL_AB(MUL_AB(MUL_AB(1,2), MUL_AB(2,3)), MUL_AB(MUL_AB(3,4), MUL_AB(4,5)))-2879 ; @BT 06 01
        ASL MUL_ABC1(1,2,3)     ; @BT 06 06
        ASL MUL_ABC1(1+0,2+0,3+0)     ; @BT 06 06
        ASL MUL_ABC1(SUM_ABC6(0+0,1+0,1), SUM_ABC7(1,2,3), SUM_ABC5(0-6,-4-1,5+5+1+1)) ; @BT 06 0C
        ASL MUL_ABC2(1,2,3)     ; @BT 06 06
        ASL MUL_ABC2(1+0,2+0,3+0)     ; @BT 06 06
        ASL MUL_ABC2(SUM_ABC6(0+0,1+0,1), SUM_ABC7(1,2,3), SUM_ABC5(0-6,-4-1,5+5+1+1)) ; @BT 06 0C
        ASL MUL_AB(1,2)+0         ; @BT 06 02
        ASL MUL_AB(1+1,1+2)+0     ; @BT 06 06
        ASL MUL_AB(MUL_AB(0,1), MUL_AB(2,3))+0 ; @BT 06 00
        ASL MUL_AB(MUL_AB(1,2), MUL_AB(3,2))+0 ; @BT 06 0C
        ASL MUL_AB(MUL_AB(MUL_AB(1,2), MUL_AB(2,3)), MUL_AB(MUL_AB(3,4), MUL_AB(4,5)))-2879 ; @BT 06 01
        ASL MUL_ABC1(1,2,3)+0     ; @BT 06 06
        ASL MUL_ABC1(1+0,2+0,3+0)+0     ; @BT 06 06
        ASL MUL_ABC1(SUM_ABC6(0+0,1+0,1), SUM_ABC7(1,2,3), SUM_ABC5(0-6,-4-1,5+5+1+1))+0 ; @BT 06 0C
        ASL MUL_ABC2(1,2,3)+0     ; @BT 06 06
        ASL MUL_ABC2(1+0,2+0,3+0)+0     ; @BT 06 06
        ASL MUL_ABC2(SUM_ABC6(0+0,1+0,1), SUM_ABC7(1,2,3), SUM_ABC5(0-6,-4-1,5+5+1+1))+0 ; @BT 06 0C
        ASL MUL_ABC2(0+1, SUM_ABC6(0+0,1+0,1) - SUM_ABC7(1,2,3), SUM_ABC5(1+2+3, 0-4-2-2-2, 1+1))+0 ; @BT 06 08
        .endr
                                ; @BT 06 02 06 06 06 00 06 0C 06 01 06 06 06 06 06 0C 06 06 06 06 06 0C 06 02 06 06 06 00 06 0C 06 01 06 06 06 06 06 0C 06 06 06 06 06 0C 06 08
                                ; @BT 06 02 06 06 06 00 06 0C 06 01 06 06 06 06 06 0C 06 06 06 06 06 0C 06 02 06 06 06 00 06 0C 06 01 06 06 06 06 06 0C 06 06 06 06 06 0C 06 08
                                ; @BT 06 02 06 06 06 00 06 0C 06 01 06 06 06 06 06 0C 06 06 06 06 06 0C 06 02 06 06 06 00 06 0C 06 01 06 06 06 06 06 0C 06 06 06 06 06 0C 06 08
                                ; @BT 06 02 06 06 06 00 06 0C 06 01 06 06 06 06 06 0C 06 06 06 06 06 0C 06 02 06 06 06 00 06 0C 06 01 06 06 06 06 06 0C 06 06 06 06 06 0C 06 08
                                ; @BT 06 02 06 06 06 00 06 0C 06 01 06 06 06 06 06 0C 06 06 06 06 06 0C 06 02 06 06 06 00 06 0C 06 01 06 06 06 06 06 0C 06 06 06 06 06 0C 06 08
                                ; @BT 06 02 06 06 06 00 06 0C 06 01 06 06 06 06 06 0C 06 06 06 06 06 0C 06 02 06 06 06 00 06 0C 06 01 06 06 06 06 06 0C 06 06 06 06 06 0C 06 08
                                ; @BT 06 02 06 06 06 00 06 0C 06 01 06 06 06 06 06 0C 06 06 06 06 06 0C 06 02 06 06 06 00 06 0C 06 01 06 06 06 06 06 0C 06 06 06 06 06 0C 06 08
                                ; @BT 06 02 06 06 06 00 06 0C 06 01 06 06 06 06 06 0C 06 06 06 06 06 0C 06 02 06 06 06 00 06 0C 06 01 06 06 06 06 06 0C 06 06 06 06 06 0C 06 08
                                ; @BT 06 02 06 06 06 00 06 0C 06 01 06 06 06 06 06 0C 06 06 06 06 06 0C 06 02 06 06 06 00 06 0C 06 01 06 06 06 06 06 0C 06 06 06 06 06 0C 06 08
                                ; @BT 06 02 06 06 06 00 06 0C 06 01 06 06 06 06 06 0C 06 06 06 06 06 0C 06 02 06 06 06 00 06 0C 06 01 06 06 06 06 06 0C 06 06 06 06 06 0C 06 08
                                ; @BT 06 02 06 06 06 00 06 0C 06 01 06 06 06 06 06 0C 06 06 06 06 06 0C 06 02 06 06 06 00 06 0C 06 01 06 06 06 06 06 0C 06 06 06 06 06 0C 06 08
                                ; @BT 06 02 06 06 06 00 06 0C 06 01 06 06 06 06 06 0C 06 06 06 06 06 0C 06 02 06 06 06 00 06 0C 06 01 06 06 06 06 06 0C 06 06 06 06 06 0C 06 08
                                ; @BT 06 02 06 06 06 00 06 0C 06 01 06 06 06 06 06 0C 06 06 06 06 06 0C 06 02 06 06 06 00 06 0C 06 01 06 06 06 06 06 0C 06 06 06 06 06 0C 06 08
                                ; @BT 06 02 06 06 06 00 06 0C 06 01 06 06 06 06 06 0C 06 06 06 06 06 0C 06 02 06 06 06 00 06 0C 06 01 06 06 06 06 06 0C 06 06 06 06 06 0C 06 08
                                ; @BT 06 02 06 06 06 00 06 0C 06 01 06 06 06 06 06 0C 06 06 06 06 06 0C 06 02 06 06 06 00 06 0C 06 01 06 06 06 06 06 0C 06 06 06 06 06 0C 06 08
                                ; @BT 06 02 06 06 06 00 06 0C 06 01 06 06 06 06 06 0C 06 06 06 06 06 0C 06 02 06 06 06 00 06 0C 06 01 06 06 06 06 06 0C 06 06 06 06 06 0C 06 08
                                ; @BT 06 02 06 06 06 00 06 0C 06 01 06 06 06 06 06 0C 06 06 06 06 06 0C 06 02 06 06 06 00 06 0C 06 01 06 06 06 06 06 0C 06 06 06 06 06 0C 06 08
                                ; @BT 06 02 06 06 06 00 06 0C 06 01 06 06 06 06 06 0C 06 06 06 06 06 0C 06 02 06 06 06 00 06 0C 06 01 06 06 06 06 06 0C 06 06 06 06 06 0C 06 08
                                ; @BT 06 02 06 06 06 00 06 0C 06 01 06 06 06 06 06 0C 06 06 06 06 06 0C 06 02 06 06 06 00 06 0C 06 01 06 06 06 06 06 0C 06 06 06 06 06 0C 06 08
        
        .db "<25"               ; @BT END
        .ends

        .macro apples1 CHILDLABELS
child1_1   nop
@child1_2  .dw ?child1_1
@@child1_3 .dw ?@child1_2+1
        .endm

        .macro apples2 CHILDLABELS args shift
child2_1:   .db 1 << shift
@child2_2:  nop
@@child2_3: .dw ?child2_1
            apples1
        .endm

        .macro apples3 args shift
child3_1:  .db (1 << shift)
@child3_2  nop
@@child3_3 .dw @@child3_3
           apples1
        .endm
        
parent  apples2 2
        apples1
        apples3 1
@child:

        .dw parent@child1_1
        .dw parent@child2_1@child2_2@child2_3@child1_1
        .dw child3_1@child3_2@child3_3@child1_1@child1_2
        
        .db "26>"               ; @BT TEST-26 26 START
        .db is("insidesection") ; @BT 00
        .db is("insidesection") == 0 ; @BT 01
        .section "INSIDE" FORCE PRIORITY 7 OFFSET 2 ALIGN 4
        .db is("InsideSection") ; @BT 01
        .db is("InsideSection") == 0    ; @BT 00
        .db get("section.priority")     ; @BT 07
        .db 1+get("section.priority")+1 ; @BT 09
        .db get("section.name") == "OUTSIDE" ; @BT 00
        .db get("section.name") == "INSIDE"  ; @BT 01
        .db get("section.offset")            ; @BT 02
        .db get("section.alignment")         ; @BT 04
        .ends
        .db "<26"               ; @BT END
