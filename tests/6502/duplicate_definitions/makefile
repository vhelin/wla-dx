
CC = $(WLAVALGRIND) wla-6502
CFLAGS = -o
LD = $(WLAVALGRIND) wlalink
LDFLAGS = -C -S

SFILES = bank0.s bank1.s
IFILES = common.s
OFILES = bank0.o bank1.o

all: linked.rom check

linked.rom: $(OFILES) makefile
	$(LD) $(LDFLAGS) linkfile linked.rom

bank0.o: bank0.s common.s
	$(CC) $(CFLAGS) bank0.o bank0.s

bank1.o: bank1.s common.s
	$(CC) $(CFLAGS) bank1.o bank1.s

check: linked.rom
	sh check_symbols.sh linked.sym


$(OFILES): $(HFILES)


clean:
	rm -f $(OFILES) core *~ linked.rom linked.sym
