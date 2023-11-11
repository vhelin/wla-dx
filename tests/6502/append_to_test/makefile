
CC = $(WLAVALGRIND) wla-6502
CFLAGS = -i -o
LD = $(WLAVALGRIND) wlalink
LDFLAGS = -i -v -s

SFILES = main.s more.s last.s
IFILES = 
OFILES = main.o more.o last.o

all: linked.rom check

linked.rom: $(OFILES) makefile
	$(LD) $(LDFLAGS) linkfile linked.rom

main.o: main.s
	$(CC) $(CFLAGS) main.o main.s

more.o: more.s
	$(CC) $(CFLAGS) more.o more.s

last.o: last.s
	$(CC) $(CFLAGS) last.o last.s

check: linked.rom
	byte_tester -s main.s
	byte_tester -s more.s


$(OFILES): $(HFILES)


clean:
	rm -f $(OFILES) core *~ linked.rom linked.sym main.lst more.lst last.lst
