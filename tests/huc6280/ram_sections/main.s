
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


.ramsection "vars 2" bank 3 slot 1
tmp	db
derp dw
.ends

.bank 1 slot 0
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
bl:
si:
di:
cl:
ch:	
	sub2 $20
.ends
	

.macro CALC_PCE_BANK_FROM_WLA_DATABANK
	lda #>(\1 >> 5) ; converts page-address into bank number offset
	clc
	.db $be, $ef
	adc #:\1    	; get proper bank number
	.db $be, $ef
	.dw \1
.endm

.macro UP_VRAM_DBANK_ADDR ;ARGS label, vram_addr, bytes
	CALC_PCE_BANK_FROM_WLA_DATABANK \1
	sta <bl

	lda #<\1
	sta <si
	lda #>\1
	and #$1f    	; mask out page
	sta <si + 1

	lda \2.w + 0
	sta <di
	lda \2.w + 1
	sta <di + 1
	lda #<(\3>>1) 	; bytes
	sta <cl
	lda #>(\3>>1) 	; bytes
	sta <ch
	jsr load_vram
.endm


.bank 0 slot 0
.org 0
.section "code 4"
	UP_VRAM_DBANK_ADDR derp, $8000, 100
load_vram:	nop
.ends


.macro UP_VRAM_ADDR_2 ARGS label, vram_addr, bytes
	.db 1, 2, 3, 4, 5
	lda #:label	; LABEL
	.db 1, 2, 3, 4, 5
	sta <bl
	lda #<label
	sta <si
	lda #>label
	sta <si + 1
	lda vram_addr.w + 0 ; must force .w + 0 due to another bug (Filed)
	sta <di
	lda vram_addr.w + 1
	sta <di + 1
	lda #<(\3/2) 	; words
	sta <cl
	lda #>(\3/2) 	; words
	sta <ch
	jsr load_vram
.endm

.bank 0 slot 0
.org 0
.section "code 5"
	UP_VRAM_ADDR_2 derp, $8000, 100
vram_addr:	nop
.ends
