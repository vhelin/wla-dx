
;-----------------------------------------------------------
; ROM
;-----------------------------------------------------------

.DEFINE FIVE 5
.DEFINE TWENTY 20
	
.SECTION "LIBRARY 1"
LibraryMain:
.DB "01>"
	PCHL
	RAR
	HLT
	XRI $12+FIVE
	LXI SP, $1234
.DB "<01"
.DB " "
.DB "02>"
	RST 5
	RST FIVE+1
.DB "<02"
.DB " "
.DB "03>"
Library03Start:
	ORI TWENTY
	SUI 1+2+3
Library03End:
.DB "<03"
.DB " "
.DB "04>"
	MOV A, E
	MOV L,A
.DB "<04>"
.DB " "
.DB "05>"
	RET
	IN TWENTY-FIVE
	EI
.DB "<05"
.DW ramvar2, othervar2
.ENDS

.RAMSECTION "RAM VARS"
ramvar1 db
ramvar2 db
ramvar3 db
.ENDS

.RAMSECTION "OTHER VARS"
othervar1 db
othervar2 db
othervar3 db
.ENDS
