.lorom
.memorymap
	slotsize $8000
	defaultslot 0
	slot 0 $8000
.endme

.rombanksize $8000
.rombanks $20

; For a $20-bank (8 Mbit) ROM, SNES sums the first $20 banks (8 Mbit) once.
.define LOW_BANK_BYTE $07
.bank $0f
.db LOW_BANK_BYTE
.bank $10
.db LOW_BANK_BYTE
.bank $1f
.db LOW_BANK_BYTE

.define ROM_TYPE_SUM $20 ; .lorom
.define TEST_MARKER_SUM $3c + $3e + $43*2 + $4b*2 ; "CK>", "<CK"
.define CHECKSUM_SUM $ff*2
.define SUM ROM_TYPE_SUM + TEST_MARKER_SUM + CHECKSUM_SUM + LOW_BANK_BYTE*3
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
