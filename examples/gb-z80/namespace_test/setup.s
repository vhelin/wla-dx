.INCLUDE "gb_memory1.i"
.INCLUDE "defines1.i"
.INCDIR  "../include/"
.INCLUDE "nintendo_logo.i"

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

sharedFunc2: ; Trying to trick wlalink
	jr sharedFunc2

.section "A shared namespace" NAMESPACE "shared"

	sharedFunc:
		call _localFunc
		ret
	_localFunc:
		ret

.ends

.section "Discard [1]"

func1:
	jr func1 ; hang

.ends


.section "Discard [2]"

func2:
	jr func2 ; hang

.ends


;
; just testing here that a. this section gets dropped, and b. the name of the section is ok
;
.section Discard(3) namespace NamelessWarriorSection
	.dw $dead, $beef
hello1:	.dw hello2
hello2:	.dw hello3
hello3:	.dw hello1	
	.dw $dead, $beef
.ends


.section "Don't discard [1]" namespace s1

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
