
CC = $(WLAVALGRIND) wla-68000
CFLAGS = -o
LD = $(WLAVALGRIND) wlalink
LDFLAGS = -v -s

SFILES = main.s
OFILES = main.o

all: linked.rom check

linked.rom: $(OFILES) makefile
	$(LD) $(LDFLAGS) linkfile linked.rom

main.o: main.s
	$(CC) $(CFLAGS) main.o main.s

check: linked.rom
	byte_tester testsfile

$(OFILES): $(HFILES)

clean:
	rm -f $(OFILES) core *~ linked.rom *.sym

