
;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»
; a small example showing the wla huc6280 syntax
; written by ville helin <vhelin@iki.fi> in 2015
;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»

.INCLUDE "defines.i"

;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»
; main
;»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»»

.ramsection "ram man 1" bank 1 slot 1 align 8
kalkaros_1 db	
.ends

.bank 0 slot 0
	
.section "rom man 1"
start	.db 1
.ends

.define kalkaros_2 kalkaros_1
.export kalkaros_2

.macro derp
	lda \1.w
.endm

.section "rom man 2"
	.dw $6666
end:	.dw kalkaros_2
	.dw $7777
	lda kalkaros_2.w
	.dw $8888
	derp $100
.ends


.ramsection "vars 1" bank 0 slot 1
derps	dw
.ends

.macro sub1          	; subtract byte-sized value
	sec
	sbc \1.w
.endm

.bank 0 slot 0
.org 0
.section "code 1"
	sub1 derps
.ends


.ramsection "vars 2" bank 0 slot 1
derp dw
.ends

.bank 0 slot 0
.org 0
.section "code 2"
	lda derp.w
.ends


.macro sub2          	; subtract byte-sized value
	sec
	sbc \1
.endm

.bank 0 slot 0
.org 0
.section "code 3"
	sub2 $20
.ends
	
