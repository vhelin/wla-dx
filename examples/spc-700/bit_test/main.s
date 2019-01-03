; SPC-700 bit addressing instructions
.INCLUDE "defines.i"

main:
    ; Direct page 0
    ; X2: SET1  d
    SET1 $34.0          ; [$0034].1 = 1; [$0034] = $01
    MOV X, $34          ; X = [$0034] = $01
    SET1 $34.1          ; [$0034].2 = 1; [$0034] = $03
    MOV X, $34          ; X = [$0034] = $03
    SET1 $34.2          ; [$0034].3 = 1; [$0034] = $07
    MOV X, $34          ; X = [$0034] = $07
    SET1 $34.3          ; [$0034].4 = 1; [$0034] = $0F
    MOV X, $34          ; X = [$0034] = $0F
    SET1 $34.4          ; [$0034].5 = 1; [$0034] = $1F
    MOV X, $34          ; X = [$0034] = $1F
    SET1 $34.5          ; [$0034].6 = 1; [$0034] = $3F
    MOV X, $34          ; X = [$0034] = $3F
    SET1 $34.6          ; [$0034].7 = 1; [$0034] = $7F
    MOV X, $34          ; X = [$0034] = $7F
    SET1 $34.7          ; [$0034].8 = 1; [$0034] = $FF
    MOV X, $34          ; X = [$0034] = $FF

    ; Y2: CLR1  d
    CLR1 $34.0          ; [$0034].1 = 1; [$0034] = $FE
    MOV X, $34          ; X = [$0034] = $FE
    CLR1 $34.1          ; [$0034].2 = 1; [$0034] = $FC
    MOV X, $34          ; X = [$0034] = $FC
    CLR1 $34.2          ; [$0034].3 = 1; [$0034] = $F8
    MOV X, $34          ; X = [$0034] = $F8
    CLR1 $34.3          ; [$0034].4 = 1; [$0034] = $F0
    MOV X, $34          ; X = [$0034] = $F0
    CLR1 $34.4          ; [$0034].5 = 1; [$0034] = $E0
    MOV X, $34          ; X = [$0034] = $E0
    CLR1 $34.5          ; [$0034].6 = 1; [$0034] = $C0
    MOV X, $34          ; X = [$0034] = $C0
    CLR1 $34.6          ; [$0034].7 = 1; [$0034] = $80
    MOV X, $34          ; X = [$0034] = $80
    CLR1 $34.7          ; [$0034].8 = 1; [$0034] = $00
    MOV X, $34          ; X = [$0034] = $00

    ; Direct page 1
    SETP                ; Set DIRECT_PAGE flag to 1
    ; X2: SET1  d
    SET1 $FF.0          ; [$01FF].1 = 1; [$01FF] = $01
    MOV X, $FF          ; X = [$01FF] = $01
    SET1 $FF.1          ; [$01FF].2 = 1; [$01FF] = $03
    MOV X, $FF          ; X = [$01FF] = $03
    SET1 $FF.2          ; [$01FF].3 = 1; [$01FF] = $07
    MOV X, $FF          ; X = [$01FF] = $07
    SET1 $FF.3          ; [$01FF].4 = 1; [$01FF] = $0F
    MOV X, $FF          ; X = [$01FF] = $0F
    SET1 $FF.4          ; [$01FF].5 = 1; [$01FF] = $1F
    MOV X, $FF          ; X = [$01FF] = $1F
    SET1 $FF.5          ; [$01FF].6 = 1; [$01FF] = $3F
    MOV X, $FF          ; X = [$01FF] = $3F
    SET1 $FF.6          ; [$01FF].7 = 1; [$01FF] = $7F
    MOV X, $FF          ; X = [$01FF] = $7F
    SET1 $FF.7          ; [$01FF].8 = 1; [$01FF] = $FF
    MOV X, $FF          ; X = [$01FF] = $FF

    ; Y2: CLR1  d
    CLR1 $FF.0          ; [$01FF].1 = 1; [$01FF] = $FE
    MOV X, $FF          ; X = [$01FF] = $FE
    CLR1 $FF.1          ; [$01FF].2 = 1; [$01FF] = $FC
    MOV X, $FF          ; X = [$01FF] = $FC
    CLR1 $FF.2          ; [$01FF].3 = 1; [$01FF] = $F8
    MOV X, $FF          ; X = [$01FF] = $F8
    CLR1 $FF.3          ; [$01FF].4 = 1; [$01FF] = $F0
    MOV X, $FF          ; X = [$01FF] = $F0
    CLR1 $FF.4          ; [$01FF].5 = 1; [$01FF] = $E0
    MOV X, $FF          ; X = [$01FF] = $E0
    CLR1 $FF.5          ; [$01FF].6 = 1; [$01FF] = $C0
    MOV X, $FF          ; X = [$01FF] = $C0
    CLR1 $FF.6          ; [$01FF].7 = 1; [$01FF] = $80
    MOV X, $FF          ; X = [$01FF] = $80
    CLR1 $FF.7          ; [$01FF].8 = 1; [$01FF] = $00
    MOV X, $FF          ; X = [$01FF] = $00
    CLRP                ; Set DIRECT_PAGE flag to 0

    ; 0E: TSET1 !a
    ; 4E: TCLR1 !a
    MOV A, #$01
    TSET1 !$C000
    MOV X, !$C000       ; X = [$C000] = $01
    ASL A
    TSET1 !$C000
    MOV X, !$C000       ; X = [$C000] = $03
    ASL A
    TSET1 !$C000
    MOV X, !$C000       ; X = [$C000] = $07
    ASL A
    TSET1 !$C000
    MOV X, !$C000       ; X = [$C000] = $0F
    ASL A
    TSET1 !$C000
    MOV X, !$C000       ; X = [$C000] = $1F
    ASL A
    TSET1 !$C000
    MOV X, !$C000       ; X = [$C000] = $3F
    ASL A
    TSET1 !$C000
    MOV X, !$C000       ; X = [$C000] = $7F
    ASL A
    TSET1 !$C000
    MOV X, !$C000       ; X = [$C000] = $FF

    TCLR1 !$C000
    MOV X, !$C000       ; X = [$C000] = $7F
    LSR A
    TCLR1 !$C000
    MOV X, !$C000       ; X = [$C000] = $3F
    LSR A
    TCLR1 !$C000
    MOV X, !$C000       ; X = [$C000] = $1F
    LSR A
    TCLR1 !$C000
    MOV X, !$C000       ; X = [$C000] = $0F
    LSR A
    TCLR1 !$C000
    MOV X, !$C000       ; X = [$C000] = $07
    LSR A
    TCLR1 !$C000
    MOV X, !$C000       ; X = [$C000] = $03
    LSR A
    TCLR1 !$C000
    MOV X, !$C000       ; X = [$C000] = $01
    LSR A
    TCLR1 !$C000
    MOV X, !$C000       ; X = [$C000] = $00

    MOV A, #$55
    TSET1 !$C000
    MOV X, !$C000       ; X = [$C000] = $55
    MOV A, #$AA
    TSET1 !$C000
    MOV X, !$C000       ; X = [$C000] = $FF

    MOV A, #$55
    TCLR1 !$C000
    MOV X, !$C000       ; X = [$C000] = $AA
    MOV A, #$AA
    TCLR1 !$C000
    MOV X, !$C000       ; X = [$C000] = $00

    ; Load some values into memory
    MOV Y, #$A5
    MOV !$0700, Y      ; $700 = $A5
    MOV Y, #$C3
    MOV !$0801, Y      ; $801 = $C3
    MOV Y, #$1F
    MOV !$0902, Y      ; $902 = $1F

    SETC                ; C = 1
    ; 4A: AND1 C, m.b
    ; 6A: AND1 C, /m.b
    AND1 C, $0700.0     ; C &= 1  = 1
    ; .byte $4A $00 $07
    AND1 C, /$0700.1    ; C &= ~0 = 1
    ; .byte $6A $00 $27
    AND1 C, $0700.1     ; C &= 0  = 0
    ; .byte $4A $00 $27
    SETC                ; C = 1
    AND1 C, $0700.2     ; C &= 1  = 1
    ; .byte $4A $00 $47
    AND1 C, $0700.3     ; C &= 0  = 0
    ; .byte $4A $00 $67

    ; 0A: OR1 C, m.b
    ; 2A: OR1 C, /m.b
    OR1 C, $0801.0      ; C |= 1  = 1
    ; .byte $0A $01 $08
    OR1 C, $0801.0      ; C |= 1  = 1
    ; .byte $0A $01 $08
    CLRC                ; C = 0
    OR1 C, /$0801.0     ; C |= ~1 = 0
    ; .byte $2A $01 $08
    OR1 C, /$0801.1     ; C |= ~1 = 0
    ; .byte $2A $01 $28
    OR1 C, $0801.1      ; C |= 1  = 1
    ; .byte $0A $01 $28
    CLRC                ; C = 0
    OR1 C, $0801.5      ; C |= 0  = 0
    ; .byte $0A $01 $A8
    OR1 C, /$0801.5     ; C |= ~0 = 1
    ; .byte $2A $01 $A8

    CLRC                ; C = 0
    ; 8A: EOR1 C, m.b
    ; EOR1 C, $0902.0     ; C ^= 1  = 1
    .byte $8A $02 $09
    ; EOR1 C, $0902.7     ; C ^= 0  = 1
    .byte $8A $02 $E9
    ; EOR1 C, $0902.0     ; C ^= 1  = 0
    .byte $8A $02 $09
    ; EOR1 C, $0902.7     ; C ^= 0  = 0
    .byte $8A $02 $E9
    CLRC

    ; EA: NOT1 m.b
    NOT1 $0903.1        ; [$0903]:1 ^= 1; [$0903] = $02
    ; .byte $EA $03 $29
    MOV X, !$903        ; X = [$0903] = $02
    NOT1 $0903.6        ; [$0903]:6 ^= 1; [$0903] = $42
    ; .byte $EA $03 $C9
    MOV X, !$903        ; X = [$0903] = $42
    NOT1 $0903.1        ; [$0903]:1 ^= 1; [$0903] = $40
    ; .byte $EA $03 $29
    MOV X, !$903        ; X = [$0903] = $40
    NOT1 $0903.6        ; [$0903]:6 ^= 1; [$0903] = $00
    ; .byte $EA $03 $C9
    MOV X, !$903        ; X = [$0903] = $00

    ; AA: MOV1 C, m.b
    MOV X, #$42
    MOV $36, X
    MOV1 C, $36.0     ; C = [$36]:0 = 0
    ; .byte $AA $36 $00
    MOV1 C, $36.1     ; C = [$36]:1 = 1
    ; .byte $AA $36 $20
    MOV1 C, $36.2     ; C = [$36]:2 = 0
    ; .byte $AA $36 $40
    MOV1 C, $36.3     ; C = [$36]:3 = 0
    ; .byte $AA $36 $60
    MOV1 C, $36.4     ; C = [$36]:4 = 0
    ; .byte $AA $36 $80
    MOV1 C, $36.5     ; C = [$36]:5 = 0
    ; .byte $AA $36 $A0
    MOV1 C, $36.6     ; C = [$36]:6 = 1
    ; .byte $AA $36 $C0
    MOV1 C, $36.7     ; C = [$36]:7 = 0
    ; .byte $AA $36 $E0

    ; CA: MOV1 m.b, C
    SETC
    MOV1 $36.0, C       ; [$36]:0 = C = 1; [$36] = $43
    ; .byte $CA $36 $00
    MOV X, !$36         ; X = [$36] = $01
    MOV1 $36.5, C       ; [$36]:5 = C = 1; [$36] = $63
    ; .byte $CA $36 $A0
    MOV X, !$36         ; X = [$36] = $21
    CLRC
    MOV1 $36.0, C       ; [$36]:0 = C = 0; [$36] = $62
    ; .byte $CA $36 $00
    MOV X, !$36         ; X = [$36] = $20
    MOV1 $36.5, C       ; [$36]:5 = C = 0; [$36] = $42
    ; .byte $CA $36 $A0
    MOV X, !$36         ; X = [$36] = $00

    STOP
