
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


        .BANK 0 SLOT 0
        .ORG $1000
        
        ; @BT linked.rom
        
        .db "50>"                                       ; @BT TEST-50 50 START
        .db NUM_THREE * NUM_TWO                         ; @BT 06
        .db NUM_THREE                                   ; @BT 03
        .db "<50"                                       ; @BT END

        .BANK 4 SLOT 1
        .ORG 0

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

        .function MUL_AB(varA, varB) varA*varB
        .function MUL_ABC1(varA, varB, varC) MUL_AB(varA, varB*varC)
        .function MUL_ABC2(varA, varB, varC) 1*MUL_ABC1(varA*(2-1), (0+1)*varB, 1*varC*1)
        .define NUM_THREE = NUM_ONE + NUM_TWO
        
        .section "RepeatedCalculations4" free keep

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
        
        .bank 15 slot 0
        .org 0

        .section "RepeatedCalculations5" free keep

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
        
