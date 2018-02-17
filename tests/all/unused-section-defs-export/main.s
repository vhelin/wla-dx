.INCLUDE "../simplemap.i"

; Exporting and using a variable in a section that is getting discarded
; resulted in the following error:
; The computation of definition "FAULTY_VAR" hasn't been solved.

.SECTION "discarded-section"

start:
.DB 1,3,3,7
end:

.DEFINE FAULTY_VAR end-start
.EXPORT FAULTY_VAR
.DB FAULTY_VAR

.ENDS
