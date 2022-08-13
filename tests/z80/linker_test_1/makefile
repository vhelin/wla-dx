
CC = $(WLAVALGRIND) wla-z80
CFLAGS = -x -o
LD = $(WLAVALGRIND) wlalink
LDFLAGS = -b -d -s

SFILES = setup.s main.s
IFILES = defines.i
OFILES = main.o setup.o

all: linked.prg check

linked.prg: $(OFILES) makefile
	$(LD) $(LDFLAGS) linkfile linked.prg

main.o: main.s defines.i
	$(CC) $(CFLAGS) main.o main.s

setup.o: setup.s defines.i
	$(CC) $(CFLAGS) setup.o setup.s

check: linked.prg
	byte_tester -s main.s

$(OFILES): $(HFILES)


clean:
	rm -f $(OFILES) core *~ linked.prg linked.sym
