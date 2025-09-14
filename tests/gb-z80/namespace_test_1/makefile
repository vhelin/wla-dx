
CC = $(WLAVALGRIND) wla-gb
CFLAGS = -x -o
LD = $(WLAVALGRIND) wlalink
LDFLAGS = -d -v -S -c

SFILES = setup.s main.s
IFILES = defines1.i defines2.i
OFILES = main.o setup.o

all: linked.gb check

linked.gb: $(OFILES) makefile
	$(LD) $(LDFLAGS) linkfile linked.gb

main.o: main.s defines1.i gb_memory1.i
	$(CC) $(CFLAGS) main.o main.s

setup.o: setup.s defines2.i gb_memory1.i
	$(CC) $(CFLAGS) setup.o setup.s

check: linked.gb
	byte_tester -s namespaced.i
	byte_tester -s main.s


$(OFILES): $(HFILES)


clean:
	rm -f $(OFILES) core *~ linked.gb linked.sym
