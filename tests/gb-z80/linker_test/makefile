
CC = $(WLAVALGRIND) wla-gb
CFLAGS = -x -o
LD = $(WLAVALGRIND) wlalink
LDFLAGS = -d -v -s

SFILES = setup.s exmain.s
IFILES = defines1.i defines2.i
OFILES = main.o setup.o

all: linked.gb check

linked.gb: $(OFILES) makefile
	$(LD) $(LDFLAGS) linkfile linked.gb

main.o: exmain.s defines1.i gb_memory1.i
	$(CC) $(CFLAGS) main.o exmain.s

setup.o: setup.s defines2.i gb_memory2.i
	$(CC) $(CFLAGS) setup.o setup.s

check: linked.gb
	byte_tester -s exmain.s


$(OFILES): $(HFILES)


clean:
	rm -f $(OFILES) core *~ linked.gb linked.sym
