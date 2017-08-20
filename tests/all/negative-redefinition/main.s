.INCLUDE "../simplemap.i"

; https://github.com/vhelin/wla-dx/issues/89
; Redefinition with a negative number failed.

.SECTION "something" FORCE

.DEFINE NEGATIVE -4
.REDEFINE NEGATIVE -2 ; This fails

.ENDS
