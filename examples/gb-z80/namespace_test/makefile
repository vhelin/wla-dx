
CC = ../../../binaries/wla-gb
CFLAGS = -ox
LD = ../../../binaries/wlalink
LDFLAGS = -dvs

SFILES = setup.s main.s
IFILES = defines1.i defines2.i
OFILES = main.o setup.o

all: $(OFILES) makefile
	$(LD) $(LDFLAGS) linkfile linked.gb

main.o: main.s defines1.i gb_memory1.i
	$(CC) $(CFLAGS) main.s

setup.o: setup.s defines2.i gb_memory1.i
	$(CC) $(CFLAGS) setup.s


$(OFILES): $(HFILES)


clean:
	rm -f $(OFILES) core *~ linked.gb linked.sym
