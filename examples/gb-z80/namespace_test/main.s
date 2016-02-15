
;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»
; This example means to test "namespaces" in wla-dx. Including "masking",
; discarding, etc...
;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»

.OUTNAME "main.o"

;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»
; project includes
;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»

.incdir  "bogus"
.INCLUDE "gb_memory1.i"
.incdir  ""
.INCLUDE "defines1.i"
.include "cgb_hardware.i"

;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»
; main
;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»

.BANK 0 SLOT 0
.ORG $150

.SECTION "Beginning [NO]" FORCE

; Try making local labels before any parent labels are available. Labels should
; be interpreted as-is.
@b:
	jr @b


MAIN:DI
	LD	SP, stack_ptr-1		;stack_ptr is defined in setup.s
	LD	A, 'C'                -   10
	SUB	A
	LD	($FF00+R_IE), A		;no interrupts at all.

	LD	A, 144
	LDH	(R_WY), A		;window y.

	LD	A, %10000001
	LDH	[R_LCDC], A		;lcd control.

	SUB	A

	call s1.func1
	call s1.func2
	call func3
	call func4
	call s3.func3
	call shared.sharedFunc
	call shared.sharedEntry
	call _globalFunc

	/*
	; Uncomment for some errors
	call sharedFunc
	call sharedEntry
	call shared._localFunc

	jr @b
	*/

	; Test local labels
parent:
	jr @@b
@@b
	jr @a
@@a
	jr @@a
@b
; Uncomment for a compile error
;	jr @@a
	jr @c
@a
	jr @@a
@@a
	jr @b
@c
	jr parent2@continue@continueChild

	; Test bank referencing, calculations
	.db :@a+1
	.db >@a
	.db <@a+1

parent2:
; Give this one more character to get an error (will be too long when turned
; into "parent2@reallyreally...longname".
@reallyreallyreallyreallyreallyreallyreallyreallylongnam:

@continue:
@@continueChild:
	jr @b
@b

	ld hl,_LOOP
	push hl
	ld hl,data
	ldi a,(hl)
	ld h,(hl)
	ld l,a
	ld b,20
-
	dec hl
	dec b
	jr nz,-
	jp hl

_LOOP:	LD	($FF00+R_BGP), A	;background palette.
	INC	A 
	JP	_LOOP

data:
	.dw s3.func3 + 20 ; Test arithmetic on namespaces


.ENDS

.SECTION "Second part of shared namespace" namespace "shared"

sharedEntry:
	call sharedFunc
	; Uncomment for an error
	; call _localFunc
	ret

_globalFunc: ; Should not be called
	jr _globalFunc
.ENDS

_globalFunc:
	ret
