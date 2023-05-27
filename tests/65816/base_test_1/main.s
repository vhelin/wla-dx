
;--- SNES HiROM bank map ---

.MEMORYMAP
SLOTSIZE $10000
DEFAULTSLOT 0
SLOT 0 $0000
.ENDME

.ROMBANKSIZE $10000
.ROMBANKS 4

        ; @BT result.rom

.SECTION "TestCode" FORCE base $c0 bank 0 slot 0 org $0000

; code ; compiles to

        .db "03>"               ; @BT TEST-03 03 START
	jml label	        ;5C0000C2 JMP $C20000
			        ; - should be 5C0000C1  JMP $C10000
                                ; @BT 5C 00 00 C1

	rep #$30        	;C230 REP #$30
			        ; - correct
                                ; @BT C2 30

	sep #$20	        ;E220 SEP #$20
			        ; - correct
                                ; @BT E2 20

	lda #:label     	;A901 LDA #$01
			        ; - should be A9C1  LDA #$C1
                                ; @BT A9 C1
        .db "<03"               ; @BT END

	.db "01>"               ; @BT TEST-01 01 START
	.db :CADDR           	; @BT C0
	.db 1 + :CADDR - 1      ; @BT C0
	.dw 1 + :CADDR - 1      ; @BT C0 00
        .db :label_Tip          ; @BT C3
        .db :label_Tip1         ; @BT C2
	.db "<01"               ; @BT END
	.db " "
	.db "02>"               ; @BT TEST-02 02 START
	.dl label_ram_b0_2 	; @BT 01 00 C2
	.dl label_ram_b1_2	; @BT 01 00 C3
	.db "<02"               ; @BT END
.ENDS

        .base $C0
        .BANK 1 SLOT 0
        .ORG $0000
        .SECTION "AnotherSection" FORCE
label:	nop
        .ENDS

        .section "Tail" APPENDTO "AnotherSection" base $c1 bank 0 slot 0
label_Tail:     nop
        .ends

        .section "Tip1" base $c2 bank 0 slot 0
label_Tip1:     nop
        .ends
        
        .base $C2
        .bank 2 slot 0
        .section "Tip" AFTER "Tail"
label_Tip:     nop
        .ends


.ramsection "RAM 1" bank 0 slot 0
label_ram_b0_1:	db
label_ram_b0_2:	db
label_ram_b0_3:	db
.ends

.ramsection "RAM 2" bank 1 slot 0
label_ram_b1_1:	db
label_ram_b1_2:	db
label_ram_b1_3:	db
.ends

