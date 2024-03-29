.hirom
.memorymap
	slotsize $10000
	defaultslot 0
	slot 0 $0000
.endme

.rombanksize $10000
.rombanks $10

; For a $10-bank (8 Mbit) ROM, SNES sums the first $10 banks (8 Mbit) once.
.define LOW_BANK_BYTE $07
.bank $01
.db LOW_BANK_BYTE
.bank $0f
.db LOW_BANK_BYTE

.define ROM_TYPE_SUM $21 ; .hirom
.define TEST_MARKER_SUM $3c + $3e + $43*2 + $4b*2 ; "CK>", "<CK"
.define CHECKSUM_SUM $ff*2
.define SUM ROM_TYPE_SUM + TEST_MARKER_SUM + CHECKSUM_SUM + LOW_BANK_BYTE*2
.print "Expected checksum: ", hex (SUM & $ffff), "\n"
.print "Expected complement checksum: ", hex ((SUM ~ $ffff) & $ffff), "\n"

.bank 0
.define SNES_CHECKSUM $ffdc
.define SNES_CHECKSUM_SIZE 4
.org SNES_CHECKSUM - 3
.db "CK>"
.org SNES_CHECKSUM + SNES_CHECKSUM_SIZE
.db "<CK"

; @BT result.rom
; @BT TEST-CK CK START 3C FC C3 03 END

.computesneschecksum
