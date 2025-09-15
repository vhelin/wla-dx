
.MEMORYMAP
DEFAULTSLOT 0
SLOTSIZE $200
SLOT 0 $0000
SLOT 1 $0200
.ENDME

.ROMBANKMAP
BANKSTOTAL 10
BANKSIZE $200
BANKS 10
.ENDRO

.printv dec (-(1+2+3-(3+2)+1)-1)
.printt "\n"


.slot 0

.section "bossman" superfree
.repeat $200
  .db 1
.endr
.ends


.bank 0 slot 0
.org 0

; @BT linked.prg

	.db	"meh"
Label:	nop

.macro test_macro args some_number
	ld a,some_number

.endm

	test_macro :Label


	
.section "reptman" force
repoman:
.dw +
.rept 15
    jr nc,+
    nop
  +:add hl,hl          ; hl*=2
.endr
.db "moro!", repoman+6
.ends

.slot 1
.section "16-bit multiplication with .rept" superfree
Multiply16_rept:
; input: de,bc
; output: de*bc
    or a               ; clear carry
    ld hl,$0000        ; zero hl

.rept 15
    rl e
    rl d               ; de<<=1
    jr nc,+
    add hl,bc          ; if a bit was rotated out then add bc to hl
    jr nc,+
    inc de             ; if that hasn't overflowed then inc de
  +:add hl,hl          ; hl*=2
.endr
    rl e
    rl d
    ret nc
    add hl,bc
    ret nc
    inc de
    ret
.ends

.section "16-bit multiplication" free
Multiply16:
; input: de,bc
; output: de*bc
    or a               ; clear carry
    ld hl,$0000        ; zero hl

    rl e
    rl d               ; de<<=1
    jr nc,+
    add hl,bc          ; if a bit was rotated out then add bc to hl
    jr nc,+
    inc de             ; if that hasn't overflowed then inc de
  +:add hl,hl          ; hl*=2
    rl e
    rl d               ; de<<=1
    jr nc,+
    add hl,bc          ; if a bit was rotated out then add bc to hl
    jr nc,+
    inc de             ; if that hasn't overflowed then inc de
  +:add hl,hl          ; hl*=2
    rl e
    rl d               ; de<<=1
    jr nc,+
    add hl,bc          ; if a bit was rotated out then add bc to hl
    jr nc,+
    inc de             ; if that hasn't overflowed then inc de
  +:add hl,hl          ; hl*=2
    rl e
    rl d               ; de<<=1
    jr nc,+
    add hl,bc          ; if a bit was rotated out then add bc to hl
    jr nc,+
    inc de             ; if that hasn't overflowed then inc de
  +:add hl,hl          ; hl*=2
    rl e
    rl d               ; de<<=1
    jr nc,+
    add hl,bc          ; if a bit was rotated out then add bc to hl
    jr nc,+
    inc de             ; if that hasn't overflowed then inc de
  +:add hl,hl          ; hl*=2
    rl e
    rl d               ; de<<=1
    jr nc,+
    add hl,bc          ; if a bit was rotated out then add bc to hl
    jr nc,+
    inc de             ; if that hasn't overflowed then inc de
  +:add hl,hl          ; hl*=2
    rl e
    rl d               ; de<<=1
    jr nc,+
    add hl,bc          ; if a bit was rotated out then add bc to hl
    jr nc,+
    inc de             ; if that hasn't overflowed then inc de
  +:add hl,hl          ; hl*=2
    rl e
    rl d               ; de<<=1
    jr nc,+
    add hl,bc          ; if a bit was rotated out then add bc to hl
    jr nc,+
    inc de             ; if that hasn't overflowed then inc de
  +:add hl,hl          ; hl*=2
    rl e
    rl d               ; de<<=1
    jr nc,+
    add hl,bc          ; if a bit was rotated out then add bc to hl
    jr nc,+
    inc de             ; if that hasn't overflowed then inc de
  +:add hl,hl          ; hl*=2
    rl e
    rl d               ; de<<=1
    jr nc,+
    add hl,bc          ; if a bit was rotated out then add bc to hl
    jr nc,+
    inc de             ; if that hasn't overflowed then inc de
  +:add hl,hl          ; hl*=2
    rl e
    rl d               ; de<<=1
    jr nc,+
    add hl,bc          ; if a bit was rotated out then add bc to hl
    jr nc,+
    inc de             ; if that hasn't overflowed then inc de
  +:add hl,hl          ; hl*=2
    rl e
    rl d               ; de<<=1
    jr nc,+
    add hl,bc          ; if a bit was rotated out then add bc to hl
    jr nc,+
    inc de             ; if that hasn't overflowed then inc de
  +:add hl,hl          ; hl*=2
    rl e
    rl d               ; de<<=1
    jr nc,+
    add hl,bc          ; if a bit was rotated out then add bc to hl
    jr nc,+
    inc de             ; if that hasn't overflowed then inc de
  +:add hl,hl          ; hl*=2
    rl e
    rl d               ; de<<=1
    jr nc,+
    add hl,bc          ; if a bit was rotated out then add bc to hl
    jr nc,+
    inc de             ; if that hasn't overflowed then inc de
  +:add hl,hl          ; hl*=2
    rl e
    rl d               ; de<<=1
    jr nc,+
    add hl,bc          ; if a bit was rotated out then add bc to hl
    jr nc,+
    inc de             ; if that hasn't overflowed then inc de
  +:add hl,hl          ; hl*=2

    rl e
    rl d
    ret nc
    add hl,bc
    ret nc
    inc de
    ret
.ends

        .define TEN 10

        .db "01>"               ; @BT TEST-01 01 START
        .rept 10 INDEX INDEX
        .db INDEX               ; @BT 00 01 02 03 04 05 06 07 08 09
        .endr
        .rept 6 INDEX INDEX STEP -1 START 9
        .db INDEX               ; @BT 09 08 07 06 05 04
        .endr
        .rept 6 STEP -1 START 9 INDEX INDEX
        .db INDEX               ; @BT 09 08 07 06 05 04
        .endr
        .rept 6 START 3 STEP 2 INDEX INDEX
        .db INDEX               ; @BT 03 05 07 09 0B 0D
        .endr
        .rept 1+2+3 INDEX INDEX STEP 100-101 START 11-1-1
        .db INDEX               ; @BT 09 08 07 06 05 04
        .endr
        .rept TEN-4 STEP TEN-1-TEN START 19-TEN INDEX INDEX
        .db INDEX               ; @BT 09 08 07 06 05 04
        .endr
        .rept 6 START 3 STEP 2 INDEX INDEX
        .db INDEX               ; @BT 03 05 07 09 $0B 0x0D
        .endr
        .REPEAT 500 INDEX COUNT START 500
Label_{COUNT}:
        .ENDR
        .REPEAT 10 START 100 STEP -10 INDEX COUNT
        .DB COUNT               ; @BT 0d100 5A 0d80 46 3C 32 28 1E 14 0A
        .ENDR
        .db "<01"               ; @BT END
        
        .db "02>"               ; @BT TEST-02 02 START
        .define COUNTER = TEN-5
        .while COUNTER > 0
        .db COUNTER             ; @BT 0d5 0d4 0d3 0d2 0d1
        .redefine COUNTER = COUNTER-1
        .endr
        .db "<02"               ; @BT END
