CC = $(WLAVALGRIND) wla-68000
CFLAGS = -o
LD = $(WLAVALGRIND) wlalink
LDFLAGS = -v -s

all: linked.bin linked_o_bin.bin linked.smd linked.md check

linked.bin: bin.o makefile
	$(LD) $(LDFLAGS) linkfile_bin linked.bin

linked_o_bin.bin: bin.o makefile
	$(LD) $(LDFLAGS) -O BIN linkfile_bin linked_o_bin.bin

linked.smd: smd.o makefile
	$(LD) $(LDFLAGS) -O SMD linkfile_smd linked.smd

linked.md: md.o makefile
	$(LD) $(LDFLAGS) -O MD linkfile_md linked.md

bin.o: bin.s
	$(CC) $(CFLAGS) bin.o bin.s

smd.o: smd.s
	$(CC) $(CFLAGS) smd.o smd.s

md.o: md.s
	$(CC) $(CFLAGS) md.o md.s

check: linked.bin linked_o_bin.bin linked.smd linked.md
	byte_tester testsfile
	byte_tester testsfile_o_bin
	byte_tester testsfile_smd
	byte_tester testsfile_md
	$(LD) $(LDFLAGS) -O FOO linkfile_bin linked_invalid_o.bin > invalid_o.log 2>&1 || true
	grep -F "Unknown Mega Drive ROM file format \"FOO\"" invalid_o.log >/dev/null
	test ! -f linked_invalid_o.bin
	$(LD) $(LDFLAGS) -O > missing_o.log 2>&1 || true
	grep -F -- "-O requires an argument" missing_o.log >/dev/null

clean:
	rm -f *.o core *~ linked.bin linked_o_bin.bin linked.smd linked.md linked_invalid_o.bin *.sym *.log
