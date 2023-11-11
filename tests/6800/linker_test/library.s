
;-----------------------------------------------------------
; ROM
;-----------------------------------------------------------

                ; @BT linked.rom
        
.SECTION "LIBRARY 1"
LibraryMain:
.DB "01>"                       ; @BT TEST-01 01 START
	ABA                     ; @BT 1B
	ADCA #$22               ; @BT 89 22
	ADCA $A0,X              ; @BT A9 A0
	ADCA $1f                ; @BT 99 1F
	ADCA $1234              ; @BT B9 12 34
.DB "<01"                       ; @BT END
.DB " "
.DB "02>"                       ; @BT TEST-02 02 START
	ASLA                    ; @BT 48
	ASLB                    ; @BT 58
	ASL $11, X              ; @BT 68 11
	ASL $1234               ; @BT 78 12 34
.DB "<02"                       ; @BT END
.DB " "
.DB "03>"
Library03Start:
	BCC Library03Start
	BCS Library03Start
	BEQ Library03Start
	BGE Library03End
	BGT Library03End
	BHI Library03End
Library03End:
.DB "<03"
.DB " "
.DB "04>"
	JMP Library03Start,X
	JMP Library03Start
.DB "<04"
.DB " "
.DB "05>"
	LDS #Library03Start
	LDS Library03Start, X
	LDS Library03Start
	LDS.W Library03Start
.DB "<05"
.DB " "
.DB "06>"
	STAB Library03Start, X
	STAB Library03Start
	STAB.W Library03Start
	STAB Library03Start.w
.DB "<06"
.ENDS
	
