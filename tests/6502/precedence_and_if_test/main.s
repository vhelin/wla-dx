
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
        
