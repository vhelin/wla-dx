
;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»
; a small example showing the wla syntax
; this little program flashes the background colour
; written by ville helin <vhelin@cc.hut.fi> in 1998-2000
;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»

.INCLUDE "gb_memory1.i"
.INCLUDE "defines1.i"
.INCDIR  "../include/"
.INCLUDE "nintendo_logo.i"

;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»
; few definitions to test wla and wlalink
;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»

;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»
; standard stuff?
;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»

.DEFINE first_jump $100
.DEFINE stack_ptr $FFFF
.EXPORT stack_ptr

.BANK 0 SLOT 0
.ORG 0

.BANK 0 SLOT 0
.ORG first_jump

	JP	MAIN

func4:
	ret

.section "Discard [1]"

func1:
	jr func1 ; hang

.ends


.section "Discard [2]"

func2:
	jr func2 ; hang

.ends


.section "Don't discard [1]" namespace "s1"

func1:
	ret
func2:
	call func1 ; Should call the 'local' func1
	call s1.func1
	ret
func3:
	jr func3 ; Hang
func4:
	jr func4 ; Hang

	.dw func1+1 ; Test calculations
.ends



.section "Don't discard [2]"
func3:
	ret
.ends

.section "Don't discard [3]" namespace "s3"
func3:
	ret
.ends
