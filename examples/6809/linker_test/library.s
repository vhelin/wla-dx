
;-----------------------------------------------------------
; ROM
;-----------------------------------------------------------

.SECTION "LIBRARY 1"
LibraryMain:
.DB "01>"
.block "Hello World"
	ABX
	ADCB #$12
	ADCB ,S
	ADCB A,S
	ADCB B,S
	ADCB ,S+
	ADCB ,S++
	ADCB ,-S
	ADCB ,--S
	ADCB [,S]
	ADCB [A,S]
	ADCB [B,S]
	ADCB [D,S]
	ADCB [,S++]
	ADCB [,--S]	
	ADCB [$12,S]
	ADCB [$1234,S]
	ADCB [$12,PCR]
	ADCB [$1234,PCR]
	ADCB [$1234]
	ADCB $1,S
	ADCB $66,S
	ADCB $1234,S
	ADCB $12,PCR
	ADCB $1234,PCR
	ADCB $1f
	ADCB $1234
.endb
.DB "<01"
.DB " "
.DB "02>"
	EXG A, B
	EXG CC, DP
	EXG D, PC
	EXG X, Y
	EXG U, S
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
	JMP Library03Start.w,X
	JMP Library03Start
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
	STA Library03Start, X
	STA Library03Start
	STA Library03Start.w
.DB "<06"
.DB " "
.DB "07>"
	PSHS PC, U, Y, X, DP, B, A, CC
	PSHU S
	PULS CC, A, B, DP, X, Y, U, PC
	PULU S
.DB "<07"
.DB " "
.DB "08>"
	TFR A, B
	TFR CC, DP
	TFR D, PC
	TFR X, Y
	TFR U, S
.DB "<08"
.DB " "
.DB "09>"
	SWI
	SWI1
	SWI2
	SWI3
.DB "<09"
.DB " "
.DB "10>"
Library10Start:
	LBCC Library10Start
	LBCS Library10Start
	LBEQ Library10Start
	LBGE Library10End
	LBGT Library10End
	LBHI Library10End
Library10End:
.DB "<10"
.ENDS
