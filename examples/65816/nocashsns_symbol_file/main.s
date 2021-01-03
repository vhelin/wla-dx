.lorom
.memorymap
	slotsize $8000
	defaultslot 0
	slot 0 $8000
.endme

.rombanksize $8000
.rombanks $20

.bank $0 slot $0
.org $0000
_private_label_8000:
public_label_8001:

.bank $10 slot $0
.org $0000
label_108001:

.define private_define_1234 $1234

.define long_define_abcdef $abcdef
.export long_define_abcdef
