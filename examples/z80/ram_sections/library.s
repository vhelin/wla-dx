
;-----------------------------------------------------------
; RAM
;-----------------------------------------------------------

.RAMSECTION "LIB VARS 1"
var1_a dw
var1_b dw
var1_c db
.ENDS

.RAMSECTION "LIB VARS 2"
var2_a db
var2_b db
var2_c db
.ENDS

;-----------------------------------------------------------
; ROM
;-----------------------------------------------------------

.SECTION "LIB ROM 1"
library_hook:
.dw var1_a, var1_b, var1_c
.dw var2_a, var2_b, var2_c
.ENDS


