
;-----------------------------------------------------------
; ROM
;-----------------------------------------------------------

.DEFINE FIVE 5
.DEFINE TWENTY 20
	
.SECTION "LIBRARY 1"
LibraryMain:
.DB "01>"
	ACA
	ACI $12
	CFZ $1234
.DB "<01"
.DB " "
.DB "02>"
	INP 1
	INP FIVE
.DB "<02"
.DB " "
.DB "03>"
Library03Start:
	OUT TWENTY
	OUT 31
Library03End:
.DB "<03"
.DB " "
.DB "04>"
	RST FIVE
	RST 0
.DB "<04>"
.DB " "
.DB "05>"
	SBI $ff
.DB "<05"
.ENDS
