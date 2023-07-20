
.MEMORYMAP
DEFAULTSLOT 0
SLOTSIZE $100
SLOT 0 $0000
.ENDME

.ROMBANKSIZE $100
.ROMBANKS 1

.DEFINE HEXSIXHUNDRED $600

// WLA v9.10a had a bug, and we used the following test code
// in fixing it...

; @BT result.rom

.ORG $00
    .DB "01>"   ; @BT TEST-01 01 START
    ; 8-bit accumulator
    SEP #$20    ; @BT e2 20
    .DB 'A'     ; @BT 41
    ADC #$69    ; correct opcode
                ; @BT 69 69
    .DB 'B'     ; @BT 42
    ADC.B #$69  ; correct opcode
                ; @BT 69 69
    .DB 'W'     ; @BT 57
    ADC.W #$69  ; manually generate wrong opcode
                ; @BT 69 69 00
    .DB 'E'     ; @BT 45
    .DB "<01"   ; @BT END

.ORG $20
    .DB "02>"   ; @BT TEST-02 02 START
    ; 16-bit accumulator
    REP #$20    ; @BT c2 20
    .DB 'A'     ; @BT 41
    ADC #$69    ; bug! generates 8-bit opcode
                ; @BT 69 69 00
    .DB 'B'     ; @BT 42
    ADC.B #$69  ; manually generate wrong opcode
                ; @BT 69 69
    .DB 'W'     ; @BT 57
    ADC.W #$69  ; correct opcode
                ; @BT 69 69 00
    .DB 'E'     ; @BT 45
    .DB "<02"   ; @BT END

.ORG $40
    .DB "03>"   ; @BT TEST-03 03 START
    ; 8-bit index
    SEP #$10    ; @BT e2 10
    .DB 'A'     ; @BT 41
    LDX #$A2    ; correct opcode
                ; @BT a2 a2
    .DB 'B'     ; @BT 42
    LDX.B #$A2  ; correct opcode
                ; @BT a2 a2
    .DB 'W'     ; @BT 57
    LDX.W #$A2  ; manually generate wrong opcode
                ; @BT a2 a2 00
    .DB 'E'     ; @BT 45
    .DB "<03"   ; @BT END

.ORG $60
    .DB "04>"   ; @BT TEST-04 04 START
    ; 16-bit index
    REP #$10    ; @BT c2 10
    .DB 'A'     ; @BT 41
    LDX #$A2    ; bug! generates 8-bit opcode
                ; @BT a2 a2 00
    .DB 'B'     ; @BT 42
    LDX.B #$A2  ; manually generate wrong opcode
                ; @BT a2 a2
    .DB 'W'     ; @BT 57
    LDX.W #$A2  ; correct opcode
                ; @BT a2 a2 00
    .DB 'E'     ; @BT 45
    .DB "<04"   ; @BT END

.ORG $80
    .DB "05>"       ; @BT TEST-05 05 START
    ; 8-bit index
    SEP #$10        ; @BT e2 10
    .DB 'A'         ; @BT 41
    LDX #$A2        ; 8-bit
                    ; @BT a2 a2
    .DB 'B'         ; @BT 42
    LDX #$A2.w      ; manually generate wrong opcode
                    ; @BT a2 a2 00
    .DB 'W'         ; @BT 57
    LDX #$0100.w    ; @BT a2 00 01
    .DB 'E'         ; @BT 45
    .DB "<05"       ; @BT END

.ORG $A0
    ; @BT
    .DB "06>"               ; @BT TEST-06 06 START
    ; 8-bit index
    SEP #$10                ; @BT e2 10
    .DB 'A'                 ; @BT 41
    LDX #$A2                ; 8-bit
                            ; @BT a2 a2
    .DB 'B'                 ; @BT 42
    LDX #HEXSIXHUNDRED.w    ; manually generate wrong opcode
                            ; @BT a2 00 06
    MVN $35, $36            ; @BT 54 36 35
    MVN $3536               ; @BT 54 36 35
    MVP $35, $36            ; @BT 44 36 35
    MVP $3536               ; @BT 44 36 35
    .DB "<06"               ; @BT END

        ///////////////////////////////////////////////
        /* testing a little bit some negative values */
        ///////////////////////////////////////////////
        
        .db "07>"               ; @BT TEST-07 07 START
test07a:
test07b:
        .db -2                  ; @BT FE
test07c:
        .db 10-12               ; @BT FE
        .dw -2                  ; @BT FE FF
        .dw 10-12               ; @BT FE FF
        .db test07b-test07a-1   ; @BT FF
        .db test07b-test07a-2   ; @BT FE
        .db test07a-test07c     ; @BT FF
        .db "<07"               ; @BT END

        .db "08>"               ; @BT TEST-08 08 START
        WDM $12                 ; @BT 42 12
        PLB                     ; @BT AB
        .db "<08"               ; @BT END

        ///////////////////////////////////////////////
        // testing STA                               //
        ///////////////////////////////////////////////

        .db "09>"               ; @BT TEST-09 09 START
        STA (<$A)               ; @BT 92 0A
        STA ($A)                ; @BT 92 0A
        STA (<(<$1122))         ; @BT 92 22
        .db "<09"               ; @BT END
        
