
CC = $(WLAVALGRIND) wla-z80
CFLAGS = -i -o
LD = $(WLAVALGRIND) wlalink
LDFLAGS = -v -s -i

SFILES = main.s
IFILES = 
OFILES = main.o

all: linked.prg check

linked.prg: $(OFILES) makefile
	$(LD) $(LDFLAGS) linkfile linked.prg

main.o: main.s
	$(CC) $(CFLAGS) main.o main.s

check: linked.prg
	byte_tester -s main.s


$(OFILES): $(HFILES)


clean:
	rm -f $(OFILES) core *~ linked.prg linked.sym main.lst
