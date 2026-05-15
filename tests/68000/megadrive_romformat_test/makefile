CC = $(WLAVALGRIND) wla-68000
CFLAGS = -o
LD = $(WLAVALGRIND) wlalink
LDFLAGS = -v -s

all: linked.bin linked.smd linked.md linked_override.bin check

linked.bin: bin.o makefile
	$(LD) $(LDFLAGS) linkfile_bin linked.bin

linked.smd: smd.o makefile
	$(LD) $(LDFLAGS) linkfile_smd linked.smd

linked.md: md.o makefile
	$(LD) $(LDFLAGS) linkfile_md linked.md

linked_override.bin: smd.o makefile
	$(LD) $(LDFLAGS) -O BIN linkfile_smd linked_override.bin

bin.o: bin.s
	$(CC) $(CFLAGS) bin.o bin.s

smd.o: smd.s
	$(CC) $(CFLAGS) smd.o smd.s

md.o: md.s
	$(CC) $(CFLAGS) md.o md.s

check: linked.bin linked.smd linked.md linked_override.bin
	byte_tester testsfile
	byte_tester testsfile_smd
	byte_tester testsfile_md
	byte_tester testsfile_override
	@if $(LD) $(LDFLAGS) linkfile_conflict linked_conflict.bin > conflict.log 2>&1; then \
		echo "conflicting .ROMFORMAT objects unexpectedly linked"; \
		exit 1; \
	fi
	grep -F "Conflicting .ROMFORMAT directives" conflict.log >/dev/null
	cp bin.o reserved.o
	printf 'p' | dd of=reserved.o bs=1 seek=7 count=1 conv=notrunc 2>/dev/null
	@if $(LD) $(LDFLAGS) linkfile_reserved linked_reserved.bin > reserved.log 2>&1; then \
		echo "reserved .ROMFORMAT object unexpectedly linked"; \
		exit 1; \
	fi
	grep -F "Reserved .ROMFORMAT value" reserved.log >/dev/null
	$(LD) $(LDFLAGS) -O FOO linkfile_bin linked_invalid_o.bin > invalid_o.log 2>&1 || true
	grep -F "Unknown Mega Drive ROM file format \"FOO\"" invalid_o.log >/dev/null
	test ! -f linked_invalid_o.bin
	$(LD) $(LDFLAGS) -O > missing_o.log 2>&1 || true
	grep -F -- "-O requires an argument" missing_o.log >/dev/null

clean:
	rm -f *.o core *~ linked.bin linked.smd linked.md linked_override.bin linked_conflict.bin linked_reserved.bin linked_invalid_o.bin *.sym *.log
