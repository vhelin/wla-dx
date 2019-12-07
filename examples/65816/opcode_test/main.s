
.MEMORYMAP
DEFAULTSLOT 0
SLOTSIZE $100
SLOT 0 $0000
.ENDME

.ROMBANKSIZE $100
.ROMBANKS 1

.DEFINE HEXSIXHUNDRED $600

// WLA v9.10a had a bug, and we used the following test code
// in fixing it...

.ORG $00
	.DB "01>"
	; 8-bit accumulator
	SEP #$20
	.DB 'A'
	ADC #$69   ; right opcode
	.DB 'B'
	ADC.B #$69 ; right opcode
	.DB 'W'
	ADC.W #$69 ; manually generate wrong opcode
	.DB 'E'
	.DB "<01"
.ORG $20
	.DB "02>"
	; 16-bit accumulator
	REP #$20
	.DB 'A'
	ADC #$69   ; bug! generates 8-bit opcode
	.DB 'B'
	ADC.B #$69 ; manually generate wrong opcode
	.DB 'W'
	ADC.W #$69 ; right opcode
	.DB 'E'
	.DB "<02"
.ORG $40
	.DB "03>"
	; 8-bit index
	SEP #$10
	.DB 'A'
	LDX #$A2   ; right opcode
	.DB 'B'
	LDX.B #$A2 ; right opcode
	.DB 'W'
	LDX.W #$A2 ; manually generate wrong opcode
	.DB 'E'
	.DB "<03"
.ORG $60
	.DB "04>"
	; 16-bit index
	REP #$10
	.DB 'A'
	LDX #$A2   ; bug! generates 8-bit opcode
	.DB 'B'
	LDX.B #$A2 ; manually generate wrong opcode
	.DB 'W'
	LDX.W #$A2 ; right opcode
	.DB 'E'
	.DB "<04"
.ORG $80
	.DB "05>"
	; 8-bit index
	SEP #$10
	.DB 'A'
	LDX #$A2   ; 8-bit
	.DB 'B'
	LDX #$A2.w ; manually generate wrong opcode
	.DB 'W'
	LDX #$0100.w
	.DB 'E'
	.DB "<05"
.ORG $A0
	.DB "06>"
	; 8-bit index
	SEP #$10
	.DB 'A'
	LDX #$A2   ; 8-bit
	.DB 'B'
	LDX #HEXSIXHUNDRED.w ; manually generate wrong opcode
	.DB "<06"
