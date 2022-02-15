
CC = $(WLAVALGRIND) wla-z80
CFLAGS = -i -o
LD = $(WLAVALGRIND) wlalink
LDFLAGS = -v -S -i

SFILES = main.s
IFILES = 
OFILES = main.o

all: linked.rom check check2

linked.rom: $(OFILES) makefile
	$(LD) $(LDFLAGS) linkfile linked.rom

main.o: main.s
	$(CC) $(CFLAGS) main.o main.s


check: linked.rom
	byte_tester -s main.s

check2: linked.rom
	./check_symbols.sh linked.sym


$(OFILES): $(HFILES)


clean:
	rm -f $(OFILES) core *.lst *~ linked.rom linked.sym main.lst
