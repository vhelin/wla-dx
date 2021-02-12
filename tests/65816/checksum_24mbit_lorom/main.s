.lorom
.memorymap
	slotsize $8000
	defaultslot 0
	slot 0 $8000
.endme

.rombanksize $8000
.rombanks $60

; For a $60-bank (24 Mbit) ROM, SNES sums the first $40 banks (16 Mbit) once,
; and the following $20 banks (8 Mbit) twice.
.define LOW_BANK_BYTE $10
.define HIGH_BANK_BYTE $01
.bank $10
.db LOW_BANK_BYTE
.bank $5f
.db HIGH_BANK_BYTE

.define ROM_TYPE_SUM $20 ; .lorom
.define TEST_MARKER_SUM $3c + $3e + $43*2 + $4b*2 ; "CK>", "<CK"
.define CHECKSUM_SUM $ff*2
.define SUM ROM_TYPE_SUM + TEST_MARKER_SUM + CHECKSUM_SUM + LOW_BANK_BYTE + HIGH_BANK_BYTE*2
.print "Expected checksum: ", hex (SUM & $ffff), "\n"
.print "Expected complement checksum: ", hex ((SUM ~ $ffff) & $ffff), "\n"

.bank 0
.define SNES_CHECKSUM $7fdc
.define SNES_CHECKSUM_SIZE 4
.org SNES_CHECKSUM - 3
.db "CK>"
.org SNES_CHECKSUM + SNES_CHECKSUM_SIZE
.db "<CK"

.computesneschecksum
