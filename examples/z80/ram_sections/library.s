
;-----------------------------------------------------------
; RAM
;-----------------------------------------------------------

.RAMSECTION "LIB VARS 1"
var1_a dw
var1_b dw
var1_c db
.ENDS

.RAMSECTION "LIB VARS 2"
var2_a dw
var2_b db
var2_c dw
.ENDS

.RAMSECTION "LIB VARS 3"
var3_a db
var3_b db
var3_c dw
.ENDS

;-----------------------------------------------------------
; ROM
;-----------------------------------------------------------

.SECTION "LIB ROM 1"
library_hook:
.dw var1_a, var1_b, var1_c
.dw var3_a, var3_b, var3_c
.ENDS


