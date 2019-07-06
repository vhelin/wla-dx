
;-----------------------------------------------------------
; ROM
;-----------------------------------------------------------

.SECTION "LIBRARY 1"
LibraryMain:
.DB "01>"
	ABA
	ADC A #$22
	ADC A $A0,X
	ADC A $1f
	ADC A $1234
.DB "<01"
.DB " "
.DB "02>"
	ASL A
	ASL B
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
.DB "<04>"
.DB " "
.DB "05>"
	LDS #Library03Start
	LDS Library03Start, X
	LDS Library03Start
	LDS.W Library03Start
.DB "<05"
.DB " "
.DB "06>"
	STA B Library03Start, X
	STA B Library03Start
	STA.W B Library03Start
	STA B Library03Start.w
.DB "<06"
.ENDS
	
