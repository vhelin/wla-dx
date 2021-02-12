
;--- SNES HiROM bank map ---

.MEMORYMAP
SLOTSIZE $10000
DEFAULTSLOT 0
SLOT 0 $0000
.ENDME

.ROMBANKSIZE $10000
.ROMBANKS 4


.BASE $C0
.BANK 0 SLOT 0
.ORG $0000
.SECTION "TestCode" FORCE

; code ; compiles to

	jml label	;5C0000C2	JMP $C20000
			; - should be 5C0000C1  JMP $C10000

	rep #$30	;C230		REP #$30
			; - correct

	sep #$20	;E220		SEP #$20
			; - correct

	lda #:label	;A901		LDA #$01
			; - should be A9C1  LDA #$C1

	.db "START>BASE:"
	.db :CADDR           	; C0
	.db 1 + :CADDR - 1      ; C0
	.db "<END"
	.db " "
	.db "HERE>"
	.dl label_ram_b0_2 	; 01 00 C0
	.dl label_ram_b1_2	; 01 00 C1
	.db "<HERE"
.ENDS


.BANK 1 SLOT 0
.ORG $0000
.SECTION "AnotherSection" FORCE
label:	nop
.ENDS


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

