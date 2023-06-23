
        .MEMORYMAP
           DEFAULTSLOT 0
           SLOTSIZE $2000
           SLOT 0 $8000
        .ENDME

        .ROMBANKMAP
           BANKSTOTAL 1
           BANKSIZE $2000
           BANKS 1
        .ENDRO

        .BANK 0 SLOT 0
        .ORG 0

        ////////////////////////////////////////////////////////////////////////

        ; @BT linked.rom

        .db "01>"               ; @BT TEST-01 01 START

        .if 0 > 1
        .db $01
        .else
        .db $02                 ; @BT 02
        .endif

        .if 1 || 0
        .db 9                   ; @BT 09
        .else
        .db 8
        .endif

        .if (0 == 1) || (1 != 2)
        .db $03                 ; @BT 03
        .else
        .db $04
        .endif

        .if 0 > 1 || 0 < 2
        .db $12                 ; @BT 12
        .else
        .db $23
        .endif

        .if (0 > 1) || (0 < 2)
        .db $12                 ; @BT 12
        .else
        .db $23
        .endif
        
        .if 0 > 1 && 0 < 2
        .db $22
        .else
        .db $24                 ; @BT 24
        .endif

        .if (0 > 1) || (2 < 1) || (1 != 2) || (1 == 2)
        .db $13                 ; @BT 13
        .else
        .db $25
        .endif

        .if !((0 > 1) || (2 < 1) || (1 != 2) || (1 == 2))
        .db $14
        .else
        .db $26                 ; @BT 26
        .endif

        .db "<01"               ; @BT END

        ////////////////////////////////////////////////////////////////////////

        .define ZERO = 0
        .define ONE  = 1

        ////////////////////////////////////////////////////////////////////////
        
        .db "02>"               ; @BT TEST-02 02 START

        .if !ZERO && ONE && !ZERO && (ZERO+ZERO*2+ONE)
        .db $34                 ; @BT 34
        .else
        .db $45
        .endif

        .if ZERO == 1-1 && ONE == 1 && !ZERO && !(ZERO+ZERO*2)
        .db $34                 ; @BT 34
        .else
        .db $45
        .endif

        .if !(!(!(ZERO == 1-1 && ONE == 1 && !ZERO && !(ZERO+ZERO*2))))
        .db $34
        .else
        .db $45                 ; @BT 45
        .endif

        .if 1 & 3 && 1 | 0 && 7 & 1
        .db $34                 ; @BT 34
        .else
        .db $45
        .endif

        .if !1 + 3 && 1 << 0 && !(!1)
        .db $34                 ; @BT 34
        .else
        .db $45
        .endif

        .if 1
        .db $01                 ; @BT 01
        .else
        .db $00
        .endif
        
        .db "<02"               ; @BT END

        ////////////////////////////////////////////////////////////////////////
        
        .db "03>"               ; @BT TEST-03 03 START

        .if defined(ZERO) && defined(ONE) && !defined(TWO)
        .db $01                 ; @BT 01
        .else
        .db $00
        .endif

        .if defined(TWO) || defined(THREE) || !defined(ZERO)
        .db $01
        .else
        .db $00                 ; @BT 00
        .endif
        
        .if defined(ONE)
        .db $01                 ; @BT 01
        .else
        .db $00
        .endif

        .if !defined(ONE)
        .db $01
        .else
        .db $00                 ; @BT 00
        .endif

        .if defined(THREE) == 0
        .db $01                 ; @BT 01
        .else
        .db $00
        .endif

        .if defined(ZERO) == 1
        .db $02                 ; @BT 02
        .else
        .db $00
        .endif
        
        .db "<03"               ; @BT END

        ////////////////////////////////////////////////////////////////////////
        
        .db "04>"               ; @BT TEST-04 04 START

        .if exists("main.s")
        .db $11                 ; @BT 11
        .else
        .db $22
        .endif

        .if !exists("main.s")
        .db $11
        .else
        .db $22                 ; @BT 22
        .endif

        .define TESTSFILE "testsfile"

        .if !exists("main.txt") && 1 && exists("linkfile")
        .db $33                 ; @BT 33
        .else
        .db $44
        .endif

        .if exists("main.txt") || 0 & 1 || exists("linkfile")
        .db $44                 ; @BT 44
        .else
        .db $55
        .endif

        .if !defined(HELLO) && defined(ONE) && (ONE > 0)
        .db $55                 ; @BT 55
        .else
        .db $66
        .endif
        
        .db "<04"               ; @BT END

        ////////////////////////////////////////////////////////////////////////
        
        .db "05>"               ; @BT TEST-05 05 START
        .dw ~1+2*3-5            ; @BT FF FF
        .db 1+1~0+1             ; @BT 03
        .db ~0xfe~1+1           ; @BT 03
        .db 1+(0xfe~0xfe+1)     ; @BT 02
        .db 1+1+1+1-1-1         ; @BT 02
        .db "<05"               ; @BT END

        ////////////////////////////////////////////////////////////////////////
        
        .db "06>"               ; @BT TEST-06 06 START

        .if "ABCD" < "ABCE"
        .db $99                 ; @BT 99
        .else
        .db $00
        .endif

        .if "ABCE" < "ABCD"
        .db $99
        .else
        .db $00                 ; @BT 00
        .endif

        .if "ABCE" > "ABCD"
        .db $78                 ; @BT 78
        .else
        .db $89
        .endif

        .if "ABCD" > "ABCE"
        .db $78
        .else
        .db $89                 ; @BT 89
        .endif

        .if "ABCD" <= "ABCD"
        .db $01                 ; @BT 01
        .else
        .db $02
        .endif

        .if "ABCD" <= "ABCE"
        .db $03                 ; @BT 03
        .else
        .db $04
        .endif

        .if "ABCD" >= "ABCD"
        .db $05                 ; @BT 05
        .else
        .db $06
        .endif

        .if "ABCE" >= "ABCD"
        .db $07                 ; @BT 07
        .else
        .db $08
        .endif

        .if "ABCD" == "ABCD"
        .db $09                 ; @BT 09
        .else
        .db $0A
        .endif

        .if "ABCE" != "ABCD"
        .db $0B                 ; @BT 0B
        .else
        .db $0C
        .endif

        .if "HELLO" == "ABCD"
        .db $0D
        .else
        .db $0E                 ; @BT 0E
        .endif

        .if "ABCE" != "ABCE"
        .db $0F
        .else
        .db $10                 ; @BT 10
        .endif

        .db "<06"               ; @BT END

        ////////////////////////////////////////////////////////////////////////

        .define MONSTER "terrrmurrrrr"
        .define DUMMY "testing", 1, 2, 3
        .define FILE "main.s"
        .define VERSION 105
        
        ////////////////////////////////////////////////////////////////////////
        
        .db "07>"               ; @BT TEST-07 07 START

        .if MONSTER >= DUMMY
        .db $00
        .else
        .db $01                 ; @BT 01
        .endif

        .if "terrrmurrrrr" != MONSTER || MONSTER < DUMMY && "ABC" != "ABX" || "XXX" <= "ABC"
        .db $02                 ; @BT 02
        .else
        .db $03
        .endif

        .if FILE == "main.s" && VERSION > 102
        .db $04                 ; @BT 04
        .else
        .db $05
        .endif

        .if "HELLO" != MONSTER && "1234" < "5678" && VERSION == VERSION || !1
        .db $06                 ; @BT 06
        .else
        .db $07
        .endif

        .if !("main.o" != FILE)
        .db $08
        .else
        .db $09                 ; @BT 09
        .endif
        
        .db "<07"               ; @BT END

        ////////////////////////////////////////////////////////////////////////
        
        .db "08>"               ; @BT TEST-08 08 START
        .db 1+1+1+1-1-1*2+1+1-1 ; @BT 02
        .db 1*2+3-4*2+2*3-1-1*2 ; @BT 00
        .db 1 & 1 + 2 * 3       ; @BT 01
        .db ((1 & 1) + 2) * 3   ; @BT 09
        .db 1 + <$1122 & 3      ; @BT 03
        .db 1 + >$1122 & 3      ; @BT 02
        .db "<08"               ; @BT END
        
        ////////////////////////////////////////////////////////////////////////

        .db "09>"               ; @BT TEST-09 09 START

        .if 1 > 0 && exists("main.s")
          .db $00               ; @BT 00
          .if 1 < 0 || "hello" == "moto"
          .db $01
          .else
          .db $02               ; @BT 02
          .endif
        .else
          .db $03
        .endif

        .if 1 > 0
          .db $04               ; @BT 04
        .elif 2 > 1 && 3 != 2
          .db $05
        .endif

        .if 1 < 0
          .db $06
        .elif 2 > 1
          .db $07               ; @BT 07
        .endif

        .if 1 < 0
          .db $08
        .elif 2 > 3
          .db $09
        .else
          .db $0A               ; @BT 0A
        .endif

        .if 1 < 0
          .db $0B
        .elif 2 > 3
          .db $0C
        .elif 4 > 2
          .db $0D               ; @BT 0D
        .elif 5 > 1
          .db $0E
        .else
          .db $0F
        .endif

        .if 1 > 2
          .db $10
        .elif 2 > 1
          .db $11               ; @BT 11
          .if 1 < 0
          .db $12
          .else
          .db $13               ; @BT 13
          .endif
        .elif 3 > 2
          .db $14
        .else
          .db $15
        .endif
        
        .db "<09"               ; @BT END

        ////////////////////////////////////////////////////////////////////////

        .db "10>"               ; @BT TEST-10 10 START

        .if 2 > 1
        .db $00                 ; @BT 00
        .elif 4 > 1
        .db $01
        .elif 3 > 1
        .db $02
        .elif 5 > 2
          .db $03
          .if 1 > 0
          .db $04
          .elif 2 > 1
          .db $05
          .elif 4 > 3
          .db $06
          .else
            .if 3 > 0
            .db $07
            .elif 1 < 2
            .db $08
            .else
            .db $09
            .endif
          .endif
        .elif 8 > 0
        .db $0A
        .elif 222 < 111
        .db $0B
        .else
        .db $0C
        .endif

        .if 1 > 1
          .db $10
          .if 3 > 0
          .db $40
          .endif
        .elif 4 < 1
        .db $11
        .elif 3 < 1
        .db $12
        .elif 5 > 2
          .db $13               ; @BT 13
          .if 1 < 0
          .db $14
          .elif 2 > 1
            .db $15             ; @BT 15
            .if 0 != 0
            .db $20
            .elif 100 > 0
            .db $21             ; @BT 21
            .elif 1 == 1
            .db $22
            .endif
          .elif 4 > 3
          .db $16
          .else
            .if 3 > 0
              .db $17
              .if 3 > 0
              .db $30
              .endif
            .elif 1 < 2
            .db $18
            .else
            .db $19
            .endif
          .endif
        .elif 8 > 0
        .db $1A
        .elif 222 < 111
        .db $1B
        .else
        .db $1C
        .endif
        
        .db "<10"              ; @BT END
        
        ////////////////////////////////////////////////////////////////////////

        .db "11>"                 ; @BT TEST-11 11 START

        .if 0 == 1
        .db $00
        .else
          .if 1 == 0
          .db $01
          .elif 4 == 1+1+1+1+1+1-1-1
          .db $02                 ; @BT 02
          .else
            .if 1+1+1+1 == 6-1-1
            .db $03
            .else
            .db $04
            .endif
          .endif
        .endif

        .if "aaa" == "bbb"
        .db $10
        .elif "aaa" == "ccc"
        .db $11
        .elif "eee" == "eee"
          .if 2 < 0
          .db $12
          .elif 3 < 0
          .db $13
          .elif 1 > 1+1-1-1-1
          .db $14               ; @BT 14
          .else
          .db $15
          .endif
        .elif 2+2-1-1 == 1+1+1-1-1+1
          .db $16
          .if 0 > 1
          .db $17
          .elif 2 > 2
          .db $18
          .endif
        .else
          .if 9 == 9
          .db $19
          .endif
        .endif
        .db "<11"               ; @BT END
        
        ////////////////////////////////////////////////////////////////////////

        .define MINUS_ONE -1
        .if MINUS_ONE != -1
          .fail "MINUS_ONE != -1 ---> FAILS!"
        .endif

        .if -1 == -2
          .fail "-1 == -2 ---> FAILS!"
        .endif
        
        .if -(1-2) < -2
          .fail "-(1-2) < -2 ---> FAILS!"
        .endif
        
        .if -(-(2-4)) != -(4-2)
          .fail "-(-(2-4)) != -(4-2) ---> FAILS"
        .endif
        
        ////////////////////////////////////////////////////////////////////////

        .define LUT_SIZE 64
        .define VALUE_0 0
        .org $200
        .section "HURLUMHEI" free align $300
some_label:
        .rept 64
        .db 0
        .endr
some_label_end:
        .ends

        .db "12>"               ; @BT TEST-12 12 START
        .dw some_label          ; @BT 00 83
        .dw LUT_SIZE-LUT_SIZE+some_label ; @BT 00 83
        .dw some_label - VALUE_0         ; @BT 00 83
        .db some_label_end - some_label  ; @BT 40
        .db 1 + some_label_end - some_label  ; @BT 41
        .db 1 + (some_label_end - some_label)  ; @BT 41
        .db 1 + some_label_end - some_label + 1  ; @BT 42
        .db (some_label_end - some_label)*2 - (some_label_end - some_label) ; @BT 40
        .dw (some_label_end - some_label)*2 + some_label - (some_label_end - some_label)*(3-1) ; @BT 00 83
        .rept (some_label_end - some_label)*2 + 4 - (some_label_end - some_label)*(5-3)
        .db 3                   ; @BT 03 03 03 03
        .endr
        .db "<12"                        ; @BT END

        .if LUT_SIZE != LUT_SIZE+0
          .fail "Calculations are broken..."
        .endif
        .if (some_label_end-some_label) != LUT_SIZE
          .fail "LUT should have 64 entries (1)!"
        .endif
        .if (some_label_end-some_label) != LUT_SIZE-LUT_SIZE+64
          .fail "LUT should have 64 entries (2)!"
        .endif
        .if some_label_end-some_label != LUT_SIZE
          .fail "LUT should have 64 entries (3)!"
        .endif
        .if (some_label_end - some_label)*2 - (LUT_SIZE-LUT_SIZE) - (some_label_end - some_label) != LUT_SIZE
          .fail "LUT should have 64 entries (4)!"
        .endif
        .if (some_label_end-some_label) != some_label_end-some_label
          .fail "Delta calculation is broken!"
        .endif
