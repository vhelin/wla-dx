
; testing label value calculation in wla-gb (the assembler, not the linker)

.MEMORYMAP
DEFAULTSLOT 1
SLOT 0 $0000 $2000
SLOT 1 STArT $2000 sIzE $2000
SLOT 2 START $8000 SIZE $2000
.ENDME

.ROMBANKMAP
BANKSTOTAL 3
BANKSIZE $2000
BANKS 3
.ENDRO

.EMPTYFILL $ff
        
.BANK 1 SLOT 1
.ORG 0

; @BT linked.gb

start:  nop
        or a,a
        pop af
        ld sp,startend

        .define STARTEND = startend
        .define DEFINE = STARTEND+1
        .define DEFINE2 = FORCE_start+1
        .define DEFINE3 = DEFINE2+$2000
        .define DEFINE4 = DEFINE
        .define DEFINE5 = DEFINE+1
        .define STARTEND_PLUS_1 = DEFINE
        .define STARTEND_PLUS_2 = DEFINE+1
        
startend:

        .define DEFINE6 = 1+startend+1
        
        .db "00>"         ; @BT TEST-00 00 START
        .dw DEFINE        ; @BT 07 20
        .db :DEFINE       ; @BT 01
        .db bank(DEFINE)  ; @BT 01
        .dw DEFINE4       ; @BT 07 20
        .db :DEFINE4      ; @BT 01
        .db bank(DEFINE4) ; @BT 01
        .db bank(DEFINE5) ; @BT 01
        .db :DEFINE5      ; @BT 01
        .dw DEFINE5       ; @BT 08 20
        .db "<00"         ; @BT END
        
        .db "01>"         ; @BT TEST-01 01 START
        .db :startend + 1 ; @BT 02
        .db DEFINE6 & 8   ; @BT 08
        .db :DEFINE6      ; @BT 01
        .db "<01"         ; @BT END

        .assert startend == startend
        .assert startend + 2 - 1 == startend + 1
        .assert startend == STARTEND
        .assert startend == DEFINE-1
        .assert STARTEND_PLUS_1 == DEFINE
        .assert STARTEND_PLUS_1+1+1 == DEFINE+2
        .assert startend == $2006
        .assert startend - 1 == $2005
        .assert startend - start == 6
        .assert startend - 1 - start == 5
        .assert STARTEND_PLUS_1 == $2007
        .assert STARTEND_PLUS_1 + 1 == $2008
        .assert STARTEND_PLUS_2 == $2008

        .define STARTEND_LATER = startend+1
        .define FORCE_START = FORCE_start
        .define FORCE_START_PLUS_1 = FORCE_START+1
        
        .section "FORCE" force slot 2 org $101
FORCE_start:

        .define FORCE_START2 = FORCE_start
        .define FORCE_START2_PLUS_1 = FORCE_START2+1
        .define GETBANK = :FORCE_START2_PLUS_1 + 1
        
        .db "02>"                ; @BT TEST-02 02 START
        .db :STARTEND_LATER      ; @BT 01
        .dw STARTEND_LATER       ; @BT 07 20
        .db :STARTEND_PLUS_2     ; @BT 01
        .db STARTEND_PLUS_2 & 8  ; @BT 08
        .db :DEFINE2             ; @BT 01
        .db :DEFINE3             ; @BT 02
        .db DEFINE2 & 0          ; @BT 00
        .db DEFINE3 & 0          ; @BT 00
        .dsb 3, :DEFINE3         ; @BT 02 02 02
        .dw FORCE_START2         ; @BT 01 81
        .dw FORCE_START2_PLUS_1  ; @BT 02 81
        .db :FORCE_start         ; @BT 01
        .db :FORCE_START2        ; @BT 01
        .db 1 + :FORCE_START2_PLUS_1 ; @BT 02
        .db GETBANK+1                ; @BT 03
        .dsb 4, GETBANK+1        ; @BT 03 03 03 03
        .dsb 2, 1 + :FORCE_START2_PLUS_1 ; @BT 02 02
        .dsb 2, 1 + :(FORCE_start+1)     ; @BT 02 02
        .db "<02"                ; @BT END
        .ends

        .assert FORCE_start == $8101
        .assert FORCE_START == $8101
        .assert FORCE_START_PLUS_1 == $8102
        
        .section "OVERWRITE" overwrite slot 1 org $200
OVERWRITE_start:
        .db 1
        .ends

        .assert OVERWRITE_start == $2200
        
        .FUNCTION SUM_AB2(var1,var2) var1*1+var2*1
        .function SUM_ABC4(varA,varB,varC) SUM_AB2(SUM_AB2(varA,varB),varC)
        
        .db "03>"               ; @BT TEST-03 03 START
        .db SUM_ABC4(5,6,7)     ; @BT 12
        .db "<03"               ; @BT END

        .db "04>"                  ; @BT TEST-04 04 START
        .db SUM_ABC4(5+0,6+0,7+0)  ; @BT 12
        .dw SUM_ABC4(startend,1,2) ; @BT 09 20
        .dw SUM_ABC4(1,startend,2) ; @BT 09 20
        .dw SUM_ABC4(startend+1,1,2) ; @BT 0A 20
        .db SUM_ABC4(1,2,3)          ; @BT 06
        .db SUM_ABC4(1,2,3) - SUM_AB2(0+1,2-1) ; @BT 04
        .dw SUM_ABC4(1+1,0+1,SUM_ABC4(0+1,startend+1,1)) ; @BT 0C 20
        .db SUM_ABC4(5+0,6+0,7+1)  ; @BT 13
        .db "<04"                  ; @BT END

        .assert SUM_ABC4(startend+1,1,2) == $200A
        .assert SUM_ABC4(startend+1,1+2,2+2) == SUM_ABC4(1+1,SUM_ABC4(0+0*2,startend+0+1,1+0+2),6-3-1)
