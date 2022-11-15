
CC = $(WLAVALGRIND) wla-6502
CFLAGS = -i -o
LD = $(WLAVALGRIND) wlalink
LDFLAGS = -v -i -S

SFILES = main.s
IFILES = 
OFILES = main.o

all: linked.rom check

linked.rom: $(OFILES) makefile
	$(LD) $(LDFLAGS) linkfile linked.rom

main.o: main.s
	$(CC) $(CFLAGS) main.o main.s

check: linked.rom
	byte_tester -s main.s

$(OFILES): $(HFILES)


clean:
	rm -f $(OFILES) core *~ *.lst linked.rom linked.sym
