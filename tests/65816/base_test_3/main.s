;--- quick SNES HiROM bank map ---

.MEMORYMAP
SLOTSIZE $10000
DEFAULTSLOT 0
SLOT 0 $0000
.ENDME


;.hiROM  ;add this line and everything compiles as it should ??
.BASE $C0


.ROMBANKSIZE $10000    ; Every ROM bank is 64 KBytes in size
.ROMBANKS 4            ; 2Mbits -- Tells WLA that we want to use 4 ROM banks.


.BANK 0 SLOT 0
.ORG $0000


; code ; compiles to
	jml label	;5C0000C2	JMP $C20000
			; - should be 5C0000C1  JMP $C10000

	rep #$30	;C230		REP #$30
			; - correct

	sep #$20	;E220		SEP #$20
			; - correct

	lda #:label	;A9C2		LDA #$C2
			; - should be A9C1  LDA #$C1

.STRUCT teststruct SIZE 8
	testbyte1	db ;dsb $8000
	testbyte2	db ;dsb $8000
.ENDST

        .STRUCT testnest
	    nest1 INSTANCEOF teststruct
        .ENDST

.ENUM 0 EXPORT
	test1 INSTANCEOF teststruct
        test2 INSTANCEOF teststruct
        test3 INSTANCEOF teststruct
        test4 INSTANCEOF teststruct
.ENDE
        
.BANK 1 SLOT 0
.ORG $0000


label:
	nop

        .ramsection "RAMSection" bank 0 slot 0
        ram1 instanceof teststruct
        ram2 instanceof teststruct
        ram3 instanceof teststruct
        ram4 instanceof teststruct
        .ends
        
.STRUCT 1 SIZE 8
    testbyte1   db
.ENDST

.STRUCT 2 SIZE 8
    testbyte2   db
.ENDST

.STRUCT both
        INSTANCEOF 1    ; name1 INSTANCEOF 1
        INSTANCEOF 2    ; name2 INSTANCEOF 2
.ENDST

.ENUM 0 export
    test    INSTANCEOF both
.ENDE
