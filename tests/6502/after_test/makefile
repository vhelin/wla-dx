
CC = $(WLAVALGRIND) wla-6502
CFLAGS = -i -k -o
LD = $(WLAVALGRIND) wlalink
LDFLAGS = -v -d -i -s

SFILES = main.s more.s
IFILES = 
OFILES = main.o more.o

all: linked.rom check

linked.rom: $(OFILES) makefile
	$(LD) $(LDFLAGS) linkfile linked.rom

main.o: main.s
	$(CC) $(CFLAGS) main.o main.s

more.o: more.s
	$(CC) $(CFLAGS) more.o more.s

check: linked.rom
	byte_tester -s main.s
	byte_tester -s more.s


$(OFILES): $(HFILES)


clean:
	rm -f $(OFILES) core *~ *.lst linked.rom linked.sym
