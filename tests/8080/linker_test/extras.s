
;-----------------------------------------------------------
; ROM
;-----------------------------------------------------------

.SECTION "EXTRAS 1"
ExtrasMain:
	PCHL
	RAR
	HLT
.DW var2
.ENDS

.RAMSECTION "VARS"
var1 db
var2 db
var3 db
.ENDS
