
;-----------------------------------------------------------
; ROM
;-----------------------------------------------------------

.SECTION "LIBRARY 1"
LibraryMain:
.DB "01>"
	ABA
	ADCA #$22
	ADCA $A0,X
	ADCA $1f
	ADCA $1234
.DB "<01"
.DB " "
.DB "02>"
	ASLA
	ASLB
	ASL $11, X
	ASL $1234
.DB "<02"
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
	
