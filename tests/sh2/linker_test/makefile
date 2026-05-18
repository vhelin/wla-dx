CC = $(WLAVALGRIND) wla-sh2
CC68000 = $(WLAVALGRIND) wla-68000
CFLAGS = -i -o
LD = $(WLAVALGRIND) wlalink
LDFLAGS = -i -v -s

SFILES = main.s extra.s mixed_68000.s
OFILES = main.o extra.o mixed_68000.o

all: linked.rom check check_cpu_mismatch

linked.rom: main.o extra.o makefile
	$(LD) $(LDFLAGS) linkfile linked.rom

main.o: main.s
	$(CC) $(CFLAGS) main.o main.s

extra.o: extra.s
	$(CC) $(CFLAGS) extra.o extra.s

mixed_68000.o: mixed_68000.s
	$(CC68000) $(CFLAGS) mixed_68000.o mixed_68000.s

check: linked.rom
	byte_tester -s main.s extra.s

check_cpu_mismatch: main.o mixed_68000.o
	@if $(LD) $(LDFLAGS) linkfile_mixed mixed.rom > mixed.log 2>&1; then cat mixed.log; exit 1; fi
	@grep 'Cannot mix SH-2 file' mixed.log >/dev/null

$(OFILES): $(HFILES)

clean:
	rm -f $(OFILES) core *~ linked.rom mixed.rom mixed.log *.sym *.lst