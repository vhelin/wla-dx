
;-----------------------------------------------------------
; ROM
;-----------------------------------------------------------

.SECTION "LIBRARY 1"
LibraryMain:
.DB "01>"
	ABA
	ABX
	ADDD #$22
	ADDD $A0,X
	ADDD $1f
	ADDD $1234
.DB "<01"
.DB " "
.DB "02>"
	ASLA
	ASLB
	ASLD
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
	JSR Library03Start,X
	JSR Library03Start
	JSR Library03Start.w
.DB "<04>"
.DB " "
.DB "05>"
	LDS #Library03Start
	LDS Library03Start, X
	LDS Library03Start
	LDS Library03Start.w
.DB "<05"
.DB " "
.DB "06>"
	STD Library03Start, X
	STD Library03Start
	STD Library03Start.w
.DB "<06"
.ENDS
	
