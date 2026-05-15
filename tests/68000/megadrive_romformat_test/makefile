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

clean:
	rm -f *.o core *~ linked.bin linked.smd linked.md linked_override.bin *.sym
