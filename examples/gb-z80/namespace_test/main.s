
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
; This is higher than the old limit of ~64 characters, but now the limit is 255
@reallyreallyreallyreallyreallyreallyreallyreallylongname:

@continue:
@@continueChild:
	jr @b
@b

	ld hl,_LOOP
	push hl
	ld hl,data

	; Test the values of the sizeof labels
	ldi a,(hl)
-
	cp 6
	jr nz,-
	ldi a,(hl)
-
 	cp 4
	jr nz,-

	; Make sure the arithmetic worked (if not it'll jump to some garbage place)
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

@child: ; When calculating "sizeof" for _LOOP, it should skip over these child labels
@@child:

	JP	_LOOP

data:
	.db _sizeof__LOOP ; Should be 6 (skips over child label)
	.db shared._sizeof_sharedEntry ; Should be 4
	.dw s3.func3 + 20 ; Test arithmetic on namespaces

	; Uncomment for error (since _globalFunc is a local name)
	;.db shared._sizeof__globalFunc


.ENDS

.SECTION "Second part of shared namespace" namespace "shared"

sharedEntry:
	call sharedFunc
	; Uncomment for an error
	; call _localFunc
	ret

_globalFunc: ; Should not be called
	jr _globalFunc

	.db _sizeof__globalFunc
.ENDS

_globalFunc:
	ret
