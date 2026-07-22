CC = $(WLAVALGRIND) wla-6502
CFLAGS = -o
LD = $(WLAVALGRIND) wlalink
LDFLAGS = -v -s

SFILES = main.s
IFILES =
OFILES = main.o

all: linked.rom check check_after_guard

linked.rom: $(OFILES) makefile
	$(LD) $(LDFLAGS) linkfile linked.rom

main.o: main.s
	$(CC) $(CFLAGS) main.o main.s

after_guard.o: after_guard.s
	$(CC) $(CFLAGS) after_guard.o after_guard.s

check: linked.rom
	byte_tester testsfile

check_after_guard: after_guard.o linkfile_after_guard
	@if $(LD) $(LDFLAGS) linkfile_after_guard after_guard.rom > after_guard.log 2>&1; then \
		echo "SPAN+AFTER guard unexpectedly linked"; \
		exit 1; \
	fi
	grep -F "uses SPAN starting in ROM bank 0, but AFTER \"Anchor\" places it in ROM bank 1" after_guard.log >/dev/null


$(OFILES): $(HFILES)


clean:
	rm -f $(OFILES) after_guard.o core *~ linked.rom after_guard.rom after_guard.log *.sym