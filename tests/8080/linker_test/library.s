
;-----------------------------------------------------------
; ROM
;-----------------------------------------------------------

        ; @BT linked.rom
        
.DEFINE FIVE 5
.DEFINE TWENTY 20
	
.SECTION "LIBRARY 1"
LibraryMain:
.DB "01>"                       ; @BT TEST-01 01 START
	PCHL                    ; @BT E9
	RAR                     ; @BT 1F
	HLT                     ; @BT 76
	XRI $12+FIVE            ; @BT EE 17
	LXI SP, $1234           ; @BT 31 34 12
.DB "<01"                       ; @BT END
.DB " "
.DB "02>"                       ; @BT TEST-02 02 START
	RST 5                   ; @BT EF
	RST FIVE+1              ; @BT F7
.DB "<02"                       ; @BT END
.DB " "
.DB "03>"                       ; @BT TEST-03 03 START
Library03Start:
	ORI TWENTY              ; @BT F6 14
	SUI 1+2+3               ; @BT D6 06
Library03End:
.DB "<03"                       ; @BT END
.DB " "
.DB "04>"                       ; @BT TEST-04 04 START
	MOV A, E                ; @BT 7B
	MOV L,A                 ; @BT 6F
.DB "<04>"                      ; @BT END
.DB " "
.DB "05>"                       ; @BT TEST-05 05 START
	RET                     ; @BT C9
	IN TWENTY-FIVE          ; @BT DB 0F
	EI                      ; @BT FB
.DB "<05"                       ; @BT END
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
