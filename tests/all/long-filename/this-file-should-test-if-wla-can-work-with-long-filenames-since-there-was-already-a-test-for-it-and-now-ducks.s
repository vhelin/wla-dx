.INCLUDE "../simplemap.i"

; This test should test long filename support, to make sure that WLA won't
; crash or something.

.BANK 0
.ORGA 0
.DB 0, 1, 2, 3, 4, 5, 6, 7, 8, 9
