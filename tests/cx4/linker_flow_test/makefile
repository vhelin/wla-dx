CC = $(WLAVALGRIND) wla-cx4
CFLAGS = -i -o
LD = $(WLAVALGRIND) wlalink
LDFLAGS = -v -S -i

SFILES = main.s routines.s
IFILES =
OFILES = main.o routines.o

all: linked.rom check

linked.rom: $(OFILES) makefile
	$(LD) $(LDFLAGS) linkfile linked.rom

main.o: main.s
	$(CC) $(CFLAGS) main.o main.s

routines.o: routines.s
	$(CC) $(CFLAGS) routines.o routines.s

check: linked.rom
	byte_tester -s main.s

$(OFILES): $(HFILES)


clean:
	rm -f $(OFILES) core *~ linked.rom linked.sym result.rom result.sym *.lst *.bin

clear: clean