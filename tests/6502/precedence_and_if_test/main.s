
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
        
        .db "01>"

        .if 0 > 1 || 0 < 2
        .db $12                 ; $12
        .else
        .db $23
        .endif

        .if 0 > 1 && 0 < 2
        .db $12
        .else
        .db $23                 ; $23
        .endif

        .if (0 > 1) || (2 < 1) || (1 != 2) || (1 == 2)
        .db $12                 ; $12
        .else
        .db $23
        .endif

        .if !((0 > 1) || (2 < 1) || (1 != 2) || (1 == 2))
        .db $12
        .else
        .db $23                 ; $23
        .endif

        .db "<01"

        ////////////////////////////////////////////////////////////////////////

        .define ZERO = 0
        .define ONE  = 1

        ////////////////////////////////////////////////////////////////////////
        
        .db "02>"

        .if !ZERO && ONE && !ZERO && (ZERO+ZERO*2+ONE)
        .db $34                 ; $34
        .else
        .db $45
        .endif

        .if ZERO == 1-1 && ONE == 1 && !ZERO && !(ZERO+ZERO*2)
        .db $34                 ; $34
        .else
        .db $45
        .endif

        .if !(!(!(ZERO == 1-1 && ONE == 1 && !ZERO && !(ZERO+ZERO*2))))
        .db $34
        .else
        .db $45                 ; $45
        .endif

        .if 1 & 3 && 1 | 0 && 7 & 1
        .db $34                 ; $34
        .else
        .db $45
        .endif

        .if !1 + 3 && 1 << 0 && !(!1)
        .db $34                 ; $34
        .else
        .db $45
        .endif

        .if 1
        .db $01                 ; $01
        .else
        .db $00
        .endif
        
        .db "<02"

        ////////////////////////////////////////////////////////////////////////
        
        .db "03>"

        .if defined(ZERO) && defined(ONE) && !defined(TWO)
        .db $01                 ; $01
        .else
        .db $00
        .endif

        .if defined(TWO) || defined(THREE) || !defined(ZERO)
        .db $01
        .else
        .db $00                 ; $00
        .endif
        
        .if defined(ONE)
        .db $01                 ; $01
        .else
        .db $00
        .endif

        .if !defined(ONE)
        .db $01
        .else
        .db $00                 ; $00
        .endif

        .if defined(THREE) == 0
        .db $01                 ; $01
        .else
        .db $00
        .endif

        .if defined(ZERO) == 1
        .db $02                 ; $02
        .else
        .db $00
        .endif
        
        .db "<03"

        ////////////////////////////////////////////////////////////////////////
        
        .db "04>"

        .if exists("main.s")
        .db $11                 ; $11
        .else
        .db $22
        .endif

        .if !exists("main.s")
        .db $11
        .else
        .db $22                 ; $22
        .endif

        .define TESTSFILE "testsfile"

        .if !exists("main.txt") && 1 && exists("linkfile") && exists(TESTSFILE)
        .db $33                 ; $33
        .else
        .db $44
        .endif

        .if exists("main.txt") || 0 & 1 || exists("linkfile") && exists(TESTSFILE)
        .db $44                 ; $44
        .else
        .db $55
        .endif

        .if !defined(HELLO) && defined(ONE) && (ONE > 0)
        .db $55                 ; $55
        .else
        .db $66
        .endif
        
        .db "<04"

        ////////////////////////////////////////////////////////////////////////
        
        .db "05>"
        .dw ~1+2*3-5            ; $ff $ff
        .db 1+1~0+1             ; $03
        .db ~0xfe~1+1           ; $03
        .db 1+(0xfe~0xfe+1)     ; $02
        .db 1+1+1+1-1-1         ; $02
        .db "<05"

        ////////////////////////////////////////////////////////////////////////
        
        .db "06>"

        .if "ABCD" < "ABCE"
        .db $99                 ; $99
        .else
        .db $00
        .endif

        .if "ABCE" < "ABCD"
        .db $99
        .else
        .db $00                 ; $00
        .endif

        .if "ABCE" > "ABCD"
        .db $78                 ; $78
        .else
        .db $89
        .endif

        .if "ABCD" > "ABCE"
        .db $78
        .else
        .db $89                 ; $89
        .endif

        .if "ABCD" <= "ABCD"
        .db $01                 ; $01
        .else
        .db $02
        .endif

        .if "ABCD" <= "ABCE"
        .db $03                 ; $03
        .else
        .db $04
        .endif

        .if "ABCD" >= "ABCD"
        .db $05                 ; $05
        .else
        .db $06
        .endif

        .if "ABCE" >= "ABCD"
        .db $07                 ; $07
        .else
        .db $08
        .endif

        .if "ABCD" == "ABCD"
        .db $09                 ; $09
        .else
        .db $0A
        .endif

        .if "ABCE" != "ABCD"
        .db $0B                 ; $0B
        .else
        .db $0C
        .endif

        .if "HELLO" == "ABCD"
        .db $0D
        .else
        .db $0E                 ; $0E
        .endif

        .if "ABCE" != "ABCE"
        .db $0F
        .else
        .db $10                 ; $10
        .endif

        .db "<06"

        ////////////////////////////////////////////////////////////////////////

        .define MONSTER "terrrmurrrrr"
        .define DUMMY "testing", 1, 2, 3
        .define FILE "main.s"
        .define VERSION 105
        
        ////////////////////////////////////////////////////////////////////////
        
        .db "07>"

        .if MONSTER >= DUMMY
        .db $00
        .else
        .db $01                 ; $01
        .endif

        .if "terrrmurrrrr" != MONSTER || MONSTER < DUMMY && "ABC" != "ABX" || "XXX" <= "ABC"
        .db $02                 ; $02
        .else
        .db $03
        .endif

        .if FILE == "main.s" && VERSION > 102
        .db $04                 ; $04
        .else
        .db $05
        .endif

        .if "HELLO" != MONSTER && "1234" < "5678" && VERSION == VERSION || !1
        .db $06                 ; $06
        .else
        .db $07
        .endif

        .if !("main.o" != FILE)
        .db $08
        .else
        .db $09                 ; $09
        .endif
        
        .db "<07"

        ////////////////////////////////////////////////////////////////////////
        
        .db "08>"
        .db 1+1+1+1-1-1*2+1+1-1 ; $02
        .db 1*2+3-4*2+2*3-1-1*2 ; $00
        .db 1 & 1 + 2 * 3       ; $01
        .db ((1 & 1) + 2) * 3   ; $09
        .db 1 + <$1122 & 3      ; $03
        .db 1 + >$1122 & 3      ; $02
        .db "<08"
        
        ////////////////////////////////////////////////////////////////////////

        .db "09>"

        .if 1 > 0 && exists("main.s")
          .db $00               ; $00
          .if 1 < 0 || "hello" == "moto"
          .db $01
          .else
          .db $02               ; $02
          .endif
        .else
          .db $03
        .endif

        .if 1 > 0
          .db $04               ; $04
        .elif 2 > 1 && 3 != 2
          .db $05
        .endif

        .if 1 < 0
          .db $06
        .elif 2 > 1
          .db $07               ; $07
        .endif

        .if 1 < 0
          .db $08
        .elif 2 > 3
          .db $09
        .else
          .db $0A               ; $0A
        .endif

        .if 1 < 0
          .db $0B
        .elif 2 > 3
          .db $0C
        .elif 4 > 2
          .db $0D               ; $0D
        .elif 5 > 1
          .db $0E
        .else
          .db $0F
        .endif

        .if 1 > 2
          .db $10
        .elif 2 > 1
          .db $11               ; $11
          .if 1 < 0
          .db $12
          .else
          .db $13               ; $13
          .endif
        .elif 3 > 2
          .db $14
        .else
          .db $15
        .endif
        
        .db "<09"

        ////////////////////////////////////////////////////////////////////////

        .db "10>"

        .if 2 > 1
        .db $00                 ; $00
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
          .db $13               ; $13
          .if 1 < 0
          .db $14
          .elif 2 > 1
            .db $15             ; $15
            .if 0 != 0
            .db $20
            .elif 100 > 0
            .db $21             ; $21
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
        
        .db "<10"
        
        ////////////////////////////////////////////////////////////////////////

        .db "11>"

        .if 0 == 1
        .db $00
        .else
          .if 1 == 0
          .db $01
          .elif 4 == 1+1+1+1+1+1-1-1
          .db $02                 ; $02
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
          .db $14               ; $14
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
        .db "<11"
